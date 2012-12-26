
#include "../../include/OBDeque.h"
#include "../../include/private/OBDeque_Private.h"
#include "../../include/OBTest.h"

/* PUBLIC METHODS */


OBDeque * createEmptyDeque(void){
  return createDefaultDeque();
}


OBDeque * copyDeque(const OBDeque *to_copy){

  obj *element;
  OBDequeIterator *iter;
  OBDeque *copy;
  
  assert(to_copy);

  copy = createEmptyDeque();

  iter = getDequeHeadIt(to_copy);
  
  if(!iter) return copy; /* if the list is empty, return empty copy */

  /* while there are elements in the deque to copy add to the tail of the
   * deque */
  do{
    element = peekDequeAtIt(to_copy, iter);
    addDequeTail(copy, element);
  } while(iterateDequeNext(to_copy, iter));

  release((obj *)iter);

  return copy;
}


uint8_t isDequeEmpty(const OBDeque *deque){
  assert(deque);
  return deque->length == 0;
}


uint64_t dequeLength(const OBDeque *deque){
  assert(deque);
  return deque->length;
}


OBDequeIterator * getDequeHeadIt(const OBDeque *deque){
  assert(deque);
  return createDequeIterator(deque, deque->head);
}


OBDequeIterator * getDequeTailIt(const OBDeque *deque){
  assert(deque);
  return createDequeIterator(deque, deque->tail);
}


uint8_t iterateDequeNext(const OBDeque *deque, OBDequeIterator *it){

  assert(deque);
  assert(it);
  assert(it->deque == deque);

  if(!it->node) return 0;

  /* update the iterator if the next node exists, and return 1 */
  if(it->node->next){
    retain((obj *)it->node->next);
    release((obj *)it->node);
    it->node = it->node->next;
    return 1;
  }

  /* else the iterator is at the end of the list, return 0 */
  return 0;
}


uint8_t iterateDequePrev(const OBDeque *deque, OBDequeIterator *it){

  assert(deque);
  assert(it);
  assert(it->deque == deque);

  if(!it->node) return 0;

  /* update the iterator if the prev node exists, and return 1 */
  if(it->node->prev){
    retain((obj *)it->node->prev);
    release((obj *)it->node);
    it->node = it->node->prev;
    return 1;
  }

  /* else the iterator is at the end of the list, return 0 */
  return 0;
}


void addDequeHead(OBDeque *deque, obj *to_add){
  
  OBDequeNode *new_node;
  
  assert(deque);
  assert(to_add);

  /* creating deque node with to_add retains to account for the deque's
   * reference */
  new_node = createDequeNode(to_add);

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


void addDequeTail(OBDeque *deque, obj *to_add){

  OBDequeNode *new_node;
  
  assert(deque);
  assert(to_add);

  /* creating deque node with to_add retains to account for the deque's
   * reference */
  new_node = createDequeNode(to_add);

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


void addAtDequeIt(OBDeque *deque, OBDequeIterator *it, obj *to_add){

  OBDequeNode *new_node;
  
  assert(deque);
  assert(it);
  assert(it->deque == deque);
  assert(to_add);

  if(!it->node){
    addDequeHead(deque, to_add);
    it->node = deque->head;
  }

  /* creating deque node with to_add retains to account for the deque's
   * reference */
  new_node = createDequeNode(to_add);

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
  assert(iterateDequePrev(deque, it));

  return;
}


OBDeque * joinDeques(const OBDeque *d1, const OBDeque *d2){

  OBDeque *joined;
  OBDequeIterator *it;
  obj *element;

  assert(d1);
  assert(d2);

  joined = copyDeque(d1);

  it = getDequeHeadIt(d2);
  
  if(!it) return joined; /* if the list is empty, return joined list */

  /* while there are elements in the deque to copy add to the tail of the
   * deque */
  do{
    element = peekDequeAtIt(d2, it);
    addDequeTail(joined, element);
  } while(iterateDequeNext(d2, it));

  release((obj *)it);

  return joined;
}

uint8_t findObjInDeque(const OBDeque *deque, const obj *to_find,
                       compare_fptr compare){

  OBDequeIterator *it;
  uint8_t retval = 0;

  assert(deque);
  assert(to_find);
  if(!compare) compare = &objCompare;

  it = getDequeHeadIt(deque);
  if(!it) return 0; /* obj is not in an empty list */
  
  do{
    if(compare(peekDequeAtIt(deque, it), to_find) == OB_EQUAL_TO){
      retval = 1;
      break;
    }
  } while(iterateDequeNext(deque, it));

  release((obj *)it);

  return retval;
}


void sortDeque(OBDeque *deque, compare_fptr compare, const int8_t order){

  OBDeque sorted; /* stack variable to remove internal memory management
                     burden */

  assert(deque);
  assert(order == OB_LEAST_TO_GREATEST || order == OB_GREATEST_TO_LEAST);

  /* if comparision function was not added use simple pointer comparator */
  if(!compare) compare = &objCompare;

  sorted = recursiveSort(*deque, compare, order);

  /* connect head and tail of newly sorted list to deque */
  deque->head = sorted.head;
  deque->tail = sorted.tail;

  return;
}
  


obj * peekDequeHead(const OBDeque *deque){
  assert(deque);
  if(deque->head) return deque->head->stored;
  return NULL;
}


obj * peekDequeTail(const OBDeque *deque){
  assert(deque);
  if(deque->tail) return deque->tail->stored;
  return NULL;
}


obj * peekDequeAtIt(const OBDeque *deque, const OBDequeIterator *it){

  assert(deque);
  assert(it);
  assert(it->deque == deque); /* assert that iterator belongs to provided 
                                 deque */
  if(!it->node) return NULL; /* if the iterator is empty return NULL */
  return it->node->stored;
}


void removeDequeHead(OBDeque *deque){

  OBDequeNode *temp_node;

  assert(deque);

  /* return if the list is empty */
  if(!(temp_node = deque->head)) return;

  /* repair deque after removal */
  deque->head = deque->head->next;
  if(!deque->head) deque->tail = NULL;
  else deque->head->prev = NULL;

  deque->length--;

  /* set temp_node next and prev to null, to separate the node completely from
   * the deque if it lives on through a reference in an iterator */
  temp_node->next = NULL;
  temp_node->prev = NULL;

  release((obj *)temp_node);

  return;
}


void removeDequeTail(OBDeque *deque){

  OBDequeNode *temp_node;

  assert(deque);

  /* return NULL if the list is empty */
  if(!(temp_node = deque->tail)) return;

  /* repair deque after removal */
  deque->tail = deque->tail->prev;
  if(!deque->tail) deque->head = NULL;
  else deque->tail->next = NULL;

  deque->length--;

  /* set temp_node next and prev to null, to separate the node completely from
   * the deque if it lives on through a reference in an iterator */
  temp_node->next = NULL;
  temp_node->prev = NULL;

  release((obj *)temp_node);

  return;
}


void removeDequeAtIt(OBDeque *deque, OBDequeIterator *it){

  OBDequeNode *temp_node;

  assert(deque);
  assert(it);
  assert(deque == it->deque); /* ensure that iterator is associated with given
                                 deque */
  if(!it->node) return; /* if iterator points to no node (an empty deque)
                           do nothing */

  temp_node = it->node;
  it->node = it->node->next;

  /* if removing the last element from the list */
  if(temp_node == deque->head && temp_node == deque->tail){
    deque->head = NULL;
    deque->tail = NULL;
  }
  else{ /* join list pieces separated by the node */
    if(temp_node != deque->head)
      temp_node->prev->next = temp_node->next;
    if(temp_node != deque->tail)
      temp_node->next->prev = temp_node->prev;
  }

  deque->length--;

  /* set temp_node next and prev to null, to separate the node completely from
   * the deque if it lives on through a reference in an iterator */
  temp_node->next = NULL;
  temp_node->prev = NULL;

  release((obj *)temp_node);

  return;
}


void clearDeque(OBDeque *deque){

  OBDequeNode *tmp, *next;

  assert(deque);

  tmp = deque->head;

  while(tmp){

    next = tmp->next;

    /* remove references to other nodes, individual nodes may live on due to
     * references from iterators */
    tmp->next = NULL;
    tmp->prev = NULL;

    release((obj *)tmp);
    tmp = next;
  }

  deque->head = NULL;
  deque->tail = NULL;
  deque->length = 0;
  
  return;
}


/* PRIVATE METHODS */


/* OBDequeNode Private Methods */

OBDequeNode * createDequeNode(obj *to_store){

  static const char classname[] = "OBDequeNode";
  OBDequeNode *new_instance;
  
  assert(to_store != NULL);

  new_instance = malloc(sizeof(OBDequeNode));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocDequeNode, NULL, classname);

  retain(to_store);
  new_instance->stored = to_store;
  new_instance->next = NULL;
  new_instance->prev = NULL;

  return new_instance;
}


void deallocDequeNode(obj *to_dealloc){

  /* cast generic obj to OBDequeNode */
  OBDequeNode *instance = (OBDequeNode *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBDequeNode"));

  release(instance->stored);

  return;
}


/* OBDequeIterator Private Methods */

OBDequeIterator * createDequeIterator(const OBDeque *deque, OBDequeNode *node){

  static const char classname[] = "OBDequeIterator";
  OBDequeIterator *new_instance;
  
  if(!node) return NULL; /* return nothing when iterating from an empty
                            deque */

  new_instance = malloc(sizeof(OBDequeIterator));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocDequeIterator, NULL, classname);

  retain((obj *)node);
  new_instance->node = node;
  new_instance->deque = deque;

  return new_instance;
}


void deallocDequeIterator(obj *to_dealloc){

  /* cast generic obj to OBDequeNode */
  OBDequeIterator *instance = (OBDequeIterator *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBDequeIterator"));

  release((obj *)instance->node);

  return;
}


/* OBDeque Private Methods */

/* add arguments to complete initialization as needed, modify 
 * OBDeque_Private.h as well if modifications are made */
OBDeque * createDefaultDeque(void){

  static const char classname[] = "OBDeque";
  OBDeque *new_instance = malloc(sizeof(OBDeque));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocDeque, NULL, classname);

  new_instance->head = NULL;
  new_instance->tail = NULL;
  new_instance->length = 0;

  return new_instance;
}


/* private recursive sort method uses stack variables to take advantage of
 * static memory management */
OBDeque recursiveSort(OBDeque deque, const compare_fptr compare,
                      const int8_t order){

  uint64_t i, half_length;
  OBDeque left_sorted, right_sorted;
  OBDequeNode *curnode;

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
  right_sorted = recursiveSort(right_sorted, compare, order);
  left_sorted = recursiveSort(left_sorted, compare, order);

  /* set the head of the combined list appropriately */
  if(compare(right_sorted.head->stored, left_sorted.head->stored) == order){
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

    if(compare(right_sorted.head->stored, left_sorted.head->stored)
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


void deallocDeque(obj *to_dealloc){

  /* cast generic obj to OBDeque */
  OBDeque *instance = (OBDeque *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBDeque"));

  clearDeque(instance);

  return;
}

