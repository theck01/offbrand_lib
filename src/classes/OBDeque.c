/**
 * @file obdeque.c
 * @brief obdeque Method Implementation
 * @author theck
 */

#include "../../include/obdeque.h"
#include "../../include/private/obdeque_private.h"

/* PUBLIC METHODS */


obdeque * obdeque_new(void){
  return obdeque_create_default();
}


obdeque * obdeque_copy(const obdeque *to_copy){

  obj *element;
  obdeque_iterator *iter;
  obdeque *copy;

  assert(to_copy);

  copy = obdeque_create_default();

  iter = obdeque_head_iterator(to_copy);

  if(!iter) return copy; /* if the list is empty, return empty copy */

  /* while there are elements in the deque to copy add to the tail of the
   * deque */
  do{
    element = obdeque_obj_at_iterator(to_copy, iter);
    obdeque_add_at_tail(copy, element);
  } while(obdeque_iterate_next(to_copy, iter));

  ob_release((obj *)iter);

  return copy;
}


uint8_t obdeque_is_empty(const obdeque *deque){
  assert(deque);
  return deque->length == 0;
}


uint64_t obdeque_length(const obdeque *deque){
  assert(deque);
  return deque->length;
}


obdeque_iterator * obdeque_head_iterator(const obdeque *deque){
  assert(deque);
  return obdeque_new_iterator(deque, deque->head);
}


obdeque_iterator * obdeque_tail_iterator(const obdeque *deque){
  assert(deque);
  return obdeque_new_iterator(deque, deque->tail);
}


obdeque_iterator * obdeque_copy_iterator(const obdeque_iterator *it){
  return obdeque_new_iterator(it->deque, it->node);
}


uint8_t obdeque_iterate_next(const obdeque *deque, obdeque_iterator *it){

  assert(deque);
  assert(it);
  assert(it->deque == deque);

  if(!it->node) return 0;

  /* update the iterator if the next node exists, and return 1 */
  if(it->node->next){
    ob_retain((obj *)it->node->next);
    ob_release((obj *)it->node);
    it->node = it->node->next;
    return 1;
  }

  /* else the iterator is at the end of the list, return 0 */
  return 0;
}


uint8_t obdeque_iterate_prev(const obdeque *deque, obdeque_iterator *it){

  assert(deque);
  assert(it);
  assert(it->deque == deque);

  if(!it->node) return 0;

  /* update the iterator if the prev node exists, and return 1 */
  if(it->node->prev){
    ob_retain((obj *)it->node->prev);
    ob_release((obj *)it->node);
    it->node = it->node->prev;
    return 1;
  }

  /* else the iterator is at the end of the list, return 0 */
  return 0;
}


void obdeque_add_at_head(obdeque *deque, obj *to_add){

  obdeque_node *new_node;

  assert(deque);
  assert(to_add);

  /* creating deque node with to_add ob_retains to account for the deque's
   * reference */
  new_node = obdeque_new_node(to_add);

  /* set node data */
  new_node->next = deque->head;

  /* update deque data */
  /* if a head aleady exists update head prev */
  if(deque->head) deque->head->prev = new_node;
  else deque->tail = new_node; /* else set tail, which should not be NULL
                                 anymore*/
  deque->head = new_node;
  deque->length++;

  return;
}


void obdeque_add_at_tail(obdeque *deque, obj *to_add){

  obdeque_node *new_node;

  assert(deque);
  assert(to_add);

  /* creating deque node with to_add ob_retains to account for the deque's
   * reference */
  new_node = obdeque_new_node(to_add);

  /* set node data */
  new_node->prev = deque->tail;

  /* update deque data */
  /* if a tail aleady exists update tail next */
  if(deque->tail) deque->tail->next = new_node;
  else deque->head = new_node; /* else set head, which should not be NULL
                                 anymore */
  deque->tail = new_node;
  deque->length++;

  return;
}


void obdeque_add_at_iterator(obdeque *deque, obdeque_iterator *it, obj *to_add){

  obdeque_node *new_node;

  assert(deque);
  assert(it);
  assert(it->deque == deque);
  assert(to_add);

  if(!it->node){
    obdeque_add_at_head(deque, to_add);
    it->node = deque->head;
  }

  /* creating deque node with to_add ob_retains to account for the deque's
   * reference */
  new_node = obdeque_new_node(to_add);

  /* set node data */
  new_node->prev = it->node->prev;
  new_node->next = it->node;
  if(it->node->prev) it->node->prev->next = new_node;
  it->node->prev = new_node;

  deque->length++;

  /* update deque data if the iterator is pointing at the head (added node
   * cannot be the new tail) */
  if(it->node == deque->head) deque->head = new_node;

  /* update iterator to newly inserted node */
  assert(obdeque_iterate_prev(deque, it));

  return;
}


obdeque * obdeque_join(const obdeque *d1, const obdeque *d2){

  obdeque *joined;
  obdeque_iterator *it;
  obj *element;

  assert(d1);
  assert(d2);

  joined = obdeque_copy(d1);

  it = obdeque_head_iterator(d2);

  if(!it) return joined; /* if the list is empty, return joined list */

  /* while there are elements in the deque to copy add to the tail of the
   * deque */
  do{
    element = obdeque_obj_at_iterator(d2, it);
    obdeque_add_at_tail(joined, element);
  } while(obdeque_iterate_next(d2, it));

  ob_release((obj *)it);

  return joined;
}

uint8_t obdeque_find_obj(const obdeque *deque, const obj *to_find){

  obdeque_iterator *it;
  uint8_t retval = 0;

  assert(deque);
  assert(to_find);

  it = obdeque_head_iterator(deque);
  if(!it) return 0; /* obj is not in an empty list */

  do{
    if(ob_compare(obdeque_obj_at_iterator(deque, it), to_find) == OB_EQUAL_TO){
      retval = 1;
      break;
    }
  } while(obdeque_iterate_next(deque, it));

  ob_release((obj *)it);

  return retval;
}


void obdeque_sort(obdeque *deque, int8_t order){
  obdeque_sort_with_funct(deque, order, &ob_compare);
}


void obdeque_sort_with_funct(obdeque *deque, int8_t order, ob_compare_fptr funct){

  obdeque sorted; /* stack variable to remove internal memory management
                     burden */

  assert(deque);
  assert(order == OB_LEAST_TO_GREATEST || order == OB_GREATEST_TO_LEAST);
  assert(funct);

  sorted = obdeque_recursive_sort(*deque, order, funct);

  /* connect head and tail of newly sorted list to deque */
  deque->head = sorted.head;
  deque->tail = sorted.tail;

  return;
}


obj * obdeque_obj_at_head(const obdeque *deque){
  assert(deque);
  if(deque->head) return deque->head->stored;
  return NULL;
}


obj * obdeque_obj_at_tail(const obdeque *deque){
  assert(deque);
  if(deque->tail) return deque->tail->stored;
  return NULL;
}


obj * obdeque_obj_at_iterator(const obdeque *deque, const obdeque_iterator *it){

  assert(deque);
  assert(it);
  assert(it->deque == deque); /* assert that iterator belongs to provided
                                 deque */
  if(!it->node) return NULL; /* if the iterator is empty return NULL */
  return it->node->stored;
}


void obdeque_remove_head(obdeque *deque){

  obdeque_node *temp_node;

  assert(deque);

  /* return if the list is empty */
  if(!(temp_node = deque->head)) return;

  /* repair deque after removal */
  deque->head = deque->head->next;
  if(deque->head) deque->head->prev = NULL;
  else deque->tail = NULL;

  deque->length--;

  /* set temp_node next and prev to null, to separate the node completely from
   * the deque if it lives on through a reference in an iterator */
  temp_node->next = NULL;
  temp_node->prev = NULL;

  ob_release((obj *)temp_node);

  return;
}


void obdeque_remove_tail(obdeque *deque){

  obdeque_node *temp_node;

  assert(deque);

  /* return NULL if the list is empty */
  if(!(temp_node = deque->tail)) return;

  /* repair deque after removal */
  deque->tail = deque->tail->prev;
  if(deque->tail) deque->tail->next = NULL;
  else deque->head = NULL;

  deque->length--;

  /* set temp_node next and prev to null, to separate the node completely from
   * the deque if it lives on through a reference in an iterator */
  temp_node->next = NULL;
  temp_node->prev = NULL;

  ob_release((obj *)temp_node);

  return;
}


void obdeque_remove_at_iterator(obdeque *deque, obdeque_iterator *it){

  obdeque_node *temp_node;

  assert(deque);
  assert(it);
  assert(deque == it->deque); /* ensure that iterator is associated with given
                                 deque */
  if(!it->node) return; /* if iterator points to no node (an empty deque)
                           do nothing */

  if(it->node == deque->head) deque->head = it->node->next;
  if(it->node == deque->tail) deque->tail = it->node->prev;
  if(it->node->prev) it->node->prev->next = it->node->next;
  if(it->node->next) it->node->next->prev = it->node->prev;

  deque->length--;

  temp_node = it->node;

  /* advance iterator to next valid node */
  if(!obdeque_iterate_next(deque, it) && !obdeque_iterate_prev(deque, it))
    it->node = NULL;

  /* set temp_node next and prev to null, to separate the node completely from
   * the deque if it lives on through a reference in an iterator */
  temp_node->next = NULL;
  temp_node->prev = NULL;

  ob_release((obj *)temp_node); /* ob_release reference that deque holds on node */

  return;
}


void obdeque_clear(obdeque *deque){

  obdeque_node *tmp, *next;

  assert(deque);

  tmp = deque->head;

  while(tmp){

    next = tmp->next;

    /* remove references to other nodes, individual nodes may live on due to
     * references from iterators */
    tmp->next = NULL;
    tmp->prev = NULL;

    ob_release((obj *)tmp);
    tmp = next;
  }

  deque->head = NULL;
  deque->tail = NULL;
  deque->length = 0;

  return;
}


/* PRIVATE METHODS */


/* obdeque_node Private Methods */

obdeque_node * obdeque_new_node(obj *to_store){

  static const char classname[] = "obdeque_node";
  obdeque_node *new_instance;

  assert(to_store != NULL);

  new_instance = malloc(sizeof(obdeque_node));
  assert(new_instance != NULL);

  /* initialize base class data */
  ob_init_base((obj *)new_instance, &obdeque_destroy_node, NULL, NULL, NULL,
               classname);

  ob_retain(to_store);
  new_instance->stored = to_store;
  new_instance->next = NULL;
  new_instance->prev = NULL;

  return new_instance;
}


void obdeque_destroy_node(obj *to_dealloc){

  /* cast generic obj to obdeque_node */
  obdeque_node *instance = (obdeque_node *)to_dealloc;

  assert(to_dealloc);
  assert(ob_has_class(to_dealloc, "obdeque_node"));

  ob_release(instance->stored);

  return;
}


/* obdeque_iterator Private Methods */

obdeque_iterator * obdeque_new_iterator(const obdeque *deque, obdeque_node *node){

  static const char classname[] = "obdeque_iterator";
  obdeque_iterator *new_instance;

  if(!node) return NULL; /* return nothing when iterating from an empty
                            deque */

  new_instance = malloc(sizeof(obdeque_iterator));
  assert(new_instance != NULL);

  /* initialize base class data */
  ob_init_base((obj *)new_instance, &obdeque_destroy_iterator, NULL, NULL, NULL,
               classname);

  ob_retain((obj *)node);
  new_instance->node = node;
  new_instance->deque = deque;

  return new_instance;
}


void obdeque_destroy_iterator(obj *to_dealloc){

  /* cast generic obj to obdeque_node */
  obdeque_iterator *instance = (obdeque_iterator *)to_dealloc;

  assert(to_dealloc);
  assert(ob_has_class(to_dealloc, "obdeque_iterator"));

  ob_release((obj *)instance->node);

  return;
}


/* obdeque Private Methods */

/* add arguments to complete initialization as needed, modify
 * obdeque_Private.h as well if modifications are made */
obdeque * obdeque_create_default(void){

  static const char classname[] = "obdeque";
  obdeque *new_instance = malloc(sizeof(obdeque));
  assert(new_instance != NULL);

  /* initialize base class data */
  ob_init_base((obj *)new_instance, &obdeque_destroy, &obdeque_hash,
               &obdeque_compare, &obdeque_display, classname);

  new_instance->head = NULL;
  new_instance->tail = NULL;
  new_instance->length = 0;

  return new_instance;
}


/* private recursive sort method uses stack variables to take advantage of
 * static memory management */
obdeque obdeque_recursive_sort(obdeque deque, int8_t order, ob_compare_fptr funct){

  uint64_t i, half_length;
  obdeque left_sorted, right_sorted;
  obdeque_node *curnode;

  /* base case, a deque of size 0 or 1 is sorted */
  if(deque.length < 2) return deque;

  half_length = deque.length/2;

  left_sorted.head = deque.head;
  left_sorted.tail = deque.head;

  /* iterate through half of the deque to split it in half */
  for(i=1; i<half_length; i++) left_sorted.tail = left_sorted.tail->next;

  right_sorted.head = left_sorted.tail->next;
  right_sorted.tail = deque.tail;

  /* separate right and left deques */
  right_sorted.head->prev = NULL;
  left_sorted.tail->next = NULL;

  left_sorted.length = half_length;
  right_sorted.length = deque.length - half_length;

  /* sort deque halves recursively */
  right_sorted = obdeque_recursive_sort(right_sorted, order, funct);
  left_sorted = obdeque_recursive_sort(left_sorted, order, funct);

  /* set the head of the combined list appropriately */
  if(funct(right_sorted.head->stored, left_sorted.head->stored) == order){
    deque.head = right_sorted.head;
    right_sorted.head = right_sorted.head->next;
  }
  else{
    deque.head = left_sorted.head;
    left_sorted.head = left_sorted.head->next;
  }

  curnode = deque.head;

  /* merge lists with proper sequencing */
  while(right_sorted.head && left_sorted.head){

    if(funct(right_sorted.head->stored, left_sorted.head->stored)
        == order){
      curnode->next = right_sorted.head;
      right_sorted.head->prev = curnode;
      right_sorted.head = right_sorted.head->next;
    }
    else{
      curnode->next = left_sorted.head;
      left_sorted.head->prev = curnode;
      left_sorted.head = left_sorted.head->next;
    }

    curnode = curnode->next;
  }

  /* merge in remenants of lists, after one has been depleted */
  while(right_sorted.head){
    curnode->next = right_sorted.head;
    right_sorted.head->prev = curnode;
    right_sorted.head = right_sorted.head->next;
    curnode = curnode->next;
  }

  while(left_sorted.head){
    curnode->next = left_sorted.head;
    left_sorted.head->prev = curnode;
    left_sorted.head = left_sorted.head->next;
    curnode = curnode->next;
  }

  /* curnode should be the last node in the list, set appropriately */
  deque.tail = curnode;

  return deque;
}


ob_hash_t obdeque_hash(const obj *to_hash){

  static int8_t init = 0;
  ob_hash_t value;
  static ob_hash_t seed = 0;
  obdeque *instance = (obdeque *)to_hash;
  obdeque_iterator *it;

  assert(to_hash);
  assert(ob_has_class(to_hash, "obdeque"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;

  it = obdeque_head_iterator(instance);
  if(!it) return value;

  do{
    value += ob_hash(obdeque_obj_at_iterator(instance, it));
    value += value << 10;
    value ^= value >> 6;
  }while(obdeque_iterate_next(instance, it));

  ob_release((obj *)it);

  value += value << 3;
  value ^= value >> 11;
  value += value << 15;

  return value;
}


int8_t obdeque_compare(const obj *a, const obj *b){

  int8_t retval = OB_EQUAL_TO; /* assume equality and disprove if not */
  const obdeque *comp_a = (obdeque *)a;
  const obdeque *comp_b = (obdeque *)b;
  obdeque_iterator *a_it, *b_it;

  assert(a);
  assert(b);
  assert(ob_has_class(a, "obdeque"));
  assert(ob_has_class(b, "obdeque"));

  if(comp_a->length != comp_b->length) return OB_NOT_EQUAL;

  a_it = obdeque_head_iterator(comp_a);
  b_it = obdeque_head_iterator(comp_b);

  if(!a_it && !b_it) return OB_EQUAL_TO;

  if(a_it && b_it){
    do{
      if(ob_compare(obdeque_obj_at_iterator(comp_a, a_it),
                    obdeque_obj_at_iterator(comp_b, b_it)) !=
       OB_EQUAL_TO){
      retval = OB_NOT_EQUAL;
      break;
    }
    }while(obdeque_iterate_next(comp_a, a_it) &&
           obdeque_iterate_next(comp_b, b_it));
  }
  else retval = OB_NOT_EQUAL;

  ob_release((obj *)a_it);
  ob_release((obj *)b_it);

  return retval;
}

void obdeque_display(const obj *to_print){

  obdeque *d = (obdeque *)to_print;
  obdeque_iterator *it;

  assert(to_print != NULL);
  assert(ob_has_class(to_print, "obdeque"));
  fprintf(stderr, "obdeque with %llu elements\n"
                  "  [deque head]", obdeque_length(d));

  it = obdeque_head_iterator(d);

  if(!it) return;

  do{
    ob_display(obdeque_obj_at_iterator(d, it));
    fprintf(stderr, "\n");
  }while(obdeque_iterate_next(d, it));

  ob_release((obj *)it);

  fprintf(stderr, "  [deque tail]\n");

  return;
}


void obdeque_destroy(obj *to_dealloc){

  /* cast generic obj to obdeque */
  obdeque *instance = (obdeque *)to_dealloc;

  assert(to_dealloc);
  assert(ob_has_class(to_dealloc, "obdeque"));

  obdeque_clear(instance);

  return;
}

