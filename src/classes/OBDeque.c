/**
 * @file OBDeque.c
 * @brief OBDeque Method Implementation
 * @author theck
 */

#include "../../include/OBDeque.h"
#include "../../include/private/OBDeque_Private.h"
#include "../../include/OBTest.h"

/* PUBLIC METHODS */


OBDeque * OBDequeCreate(void){
  return OBDequeCreateDefault();
}


OBDeque * OBDequeCopy(const OBDeque *to_copy){

  OBTypeRef element;
  OBDequeIterator *iter;
  OBDeque *copy;
  
  assert(to_copy);

  copy = OBDequeCreateDefault();

  iter = OBDequeGetHeadIterator(to_copy);
  
  if(!iter) return copy; /* if the list is empty, return empty copy */

  /* while there are elements in the deque to copy add to the tail of the
   * deque */
  do{
    element = OBDequeGetObjectAtIterator(to_copy, iter);
    OBDequeAddTail(copy, element);
  } while(OBDequeIterateNext(to_copy, iter));

  OBRelease(iter);

  return copy;
}


uint8_t OBDequeIsEmpty(const OBDeque *deque){
  assert(deque);
  return deque->length == 0;
}


uint64_t OBDequeGetLength(const OBDeque *deque){
  assert(deque);
  return deque->length;
}


OBDequeIterator * OBDequeGetHeadIterator(const OBDeque *deque){
  assert(deque);
  return OBDequeIteratorCreate(deque, deque->head);
}


OBDequeIterator * OBDequeGetTailIterator(const OBDeque *deque){
  assert(deque);
  return OBDequeIteratorCreate(deque, deque->tail);
}


OBDequeIterator * OBDequeIteratorCopy(const OBDequeIterator *it){
  return OBDequeIteratorCreate(it->deque, it->node);
}


uint8_t OBDequeIterateNext(const OBDeque *deque, OBDequeIterator *it){

  assert(deque);
  assert(it);
  assert(it->deque == deque);

  if(!it->node) return 0;

  /* update the iterator if the next node exists, and return 1 */
  if(it->node->next){
    OBRetain((OBObjType *)it->node->next);
    OBRelease((OBObjType *)it->node);
    it->node = it->node->next;
    return 1;
  }

  /* else the iterator is at the end of the list, return 0 */
  return 0;
}


uint8_t OBDequeIteratePrevious(const OBDeque *deque, OBDequeIterator *it){

  assert(deque);
  assert(it);
  assert(it->deque == deque);

  if(!it->node) return 0;

  /* update the iterator if the prev node exists, and return 1 */
  if(it->node->prev){
    OBRetain((OBObjType *)it->node->prev);
    OBRelease((OBObjType *)it->node);
    it->node = it->node->prev;
    return 1;
  }

  /* else the iterator is at the end of the list, return 0 */
  return 0;
}


void OBDequeAddHead(OBDeque *deque, OBTypeRef to_add){
  
  OBDequeNode *new_node;
  
  assert(deque);
  assert(to_add);

  /* creating deque node with to_add retains to account for the deque's
   * reference */
  new_node = OBDequeNodeCreate(to_add);

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


void OBDequeAddTail(OBDeque *deque, OBTypeRef to_add){

  OBDequeNode *new_node;
  
  assert(deque);
  assert(to_add);

  /* creating deque node with to_add retains to account for the deque's
   * reference */
  new_node = OBDequeNodeCreate(to_add);

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


void OBDeuqueAddAtIterator(OBDeque *deque, OBDequeIterator *it, OBTypeRef to_add){

  OBDequeNode *new_node;
  
  assert(deque);
  assert(it);
  assert(it->deque == deque);
  assert(to_add);

  if(!it->node){
    OBDequeAddHead(deque, to_add);
    it->node = deque->head;
  }

  /* creating deque node with to_add retains to account for the deque's
   * reference */
  new_node = OBDequeNodeCreate(to_add);

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
  assert(OBDequeIteratePrevious(deque, it));

  return;
}


OBDeque * OBDequeJoin(const OBDeque *d1, const OBDeque *d2){

  OBDeque *joined;
  OBDequeIterator *it;
  OBTypeRef element;

  assert(d1);
  assert(d2);

  joined = OBDequeCopy(d1);

  it = OBDequeGetHeadIterator(d2);
  
  if(!it) return joined; /* if the list is empty, return joined list */

  /* while there are elements in the deque to copy add to the tail of the
   * deque */
  do{
    element = OBDequeGetObjectAtIterator(d2, it);
    OBDequeAddTail(joined, element);
  } while(OBDequeIterateNext(d2, it));

  OBRelease((OBObjType *)it);

  return joined;
}

uint8_t OBDequeContains(const OBDeque *deque, OBTypeRef to_find){

  OBDequeIterator *it;
  uint8_t retval = 0;

  assert(deque);
  assert(to_find);

  it = OBDequeGetHeadIterator(deque);
  if(!it) return 0; /* obj is not in an empty list */
  
  do{
    if(OBCompare(OBDequeGetObjectAtIterator(deque, it), to_find) == OB_EQUAL_TO){
      retval = 1;
      break;
    }
  } while(OBDequeIterateNext(deque, it));

  OBRelease((OBObjType *)it);

  return retval;
}


void OBDequeSort(OBDeque *deque, int8_t order){
  OBDequeSortWithFunction(deque, order, &OBCompare);
}
  

void OBDequeSortWithFunction(OBDeque *deque, int8_t order, obcompare_fptr funct){

  OBDeque sorted; /* stack variable to remove internal memory management
                     burden */

  assert(deque);
  assert(order == OB_LEAST_TO_GREATEST || order == OB_GREATEST_TO_LEAST);
  assert(funct);

  sorted = OBDequeSortRecursive(*deque, order, funct);

  /* connect head and tail of newly sorted list to deque */
  deque->head = sorted.head;
  deque->tail = sorted.tail;

  return;
}


OBTypeRef OBDequeGetFirstObject(const OBDeque *deque){
  assert(deque);
  if(deque->head) return deque->head->stored;
  return NULL;
}


OBTypeRef OBDequeGetLastObject(const OBDeque *deque){
  assert(deque);
  if(deque->tail) return deque->tail->stored;
  return NULL;
}


OBTypeRef OBDequeGetObjectAtIterator(const OBDeque *deque, const OBDequeIterator *it){

  assert(deque);
  assert(it);
  assert(it->deque == deque); /* assert that iterator belongs to provided 
                                 deque */
  if(!it->node) return NULL; /* if the iterator is empty return NULL */
  return it->node->stored;
}


void OBDequeRemoveFirstObject(OBDeque *deque){

  OBDequeNode *temp_node;

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

  OBRelease((OBObjType *)temp_node);

  return;
}


void OBDequeRemoveLastObject(OBDeque *deque){

  OBDequeNode *temp_node;

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

  OBRelease((OBObjType *)temp_node);

  return;
}


void OBDequeRemoveObjectAtIterator(OBDeque *deque, OBDequeIterator *it){

  OBDequeNode *temp_node;

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
  if(!OBDequeIterateNext(deque, it) && !OBDequeIteratePrevious(deque, it))
    it->node = NULL;

  /* set temp_node next and prev to null, to separate the node completely from
   * the deque if it lives on through a reference in an iterator */
  temp_node->next = NULL;
  temp_node->prev = NULL;

  OBRelease((OBObjType *)temp_node); /* release reference that deque holds on node */

  return;
}


void OBDequeClear(OBDeque *deque){

  OBDequeNode *tmp, *next;

  assert(deque);

  tmp = deque->head;

  while(tmp){

    next = tmp->next;

    /* remove references to other nodes, individual nodes may live on due to
     * references from iterators */
    tmp->next = NULL;
    tmp->prev = NULL;

    OBRelease((OBObjType *)tmp);
    tmp = next;
  }

  deque->head = NULL;
  deque->tail = NULL;
  deque->length = 0;
  
  return;
}


/* PRIVATE METHODS */


/* OBDequeNode Private Methods */

OBDequeNode * OBDequeNodeCreate(OBTypeRef to_store){

  static const char classname[] = "OBDequeNode";
  OBDequeNode *new_instance;
  
  assert(to_store != NULL);

  new_instance = malloc(sizeof(OBDequeNode));
  assert(new_instance != NULL);

  /* initialize base class data */
  OBInitBase((OBObjType *)new_instance, &OBDequeNodeDealloc, NULL, NULL, NULL, classname);

  OBRetain(to_store);
  new_instance->stored = to_store;
  new_instance->next = NULL;
  new_instance->prev = NULL;

  return new_instance;
}


void OBDequeNodeDealloc(OBTypeRef to_dealloc){

  /* cast generic obj to OBDequeNode */
  OBDequeNode *instance = (OBDequeNode *)to_dealloc;

  assert(to_dealloc);
  assert(OBObjIsOfClass(to_dealloc, "OBDequeNode"));

  OBRelease(instance->stored);

  return;
}


/* OBDequeIterator Private Methods */

OBDequeIterator * OBDequeIteratorCreate(const OBDeque *deque, OBDequeNode *node){

  static const char classname[] = "OBDequeIterator";
  OBDequeIterator *new_instance;
  
  if(!node) return NULL; /* return nothing when iterating from an empty
                            deque */

  new_instance = malloc(sizeof(OBDequeIterator));
  assert(new_instance != NULL);

  /* initialize base class data */
  OBInitBase((OBObjType *)new_instance, &OBDequeIteratorDealloc, NULL, NULL, NULL,
           classname);

  OBRetain((OBObjType *)node);
  new_instance->node = node;
  new_instance->deque = deque;

  return new_instance;
}


void OBDequeIteratorDealloc(OBTypeRef to_dealloc){

  /* cast generic obj to OBDequeNode */
  OBDequeIterator *instance = (OBDequeIterator *)to_dealloc;

  assert(to_dealloc);
  assert(OBObjIsOfClass(to_dealloc, "OBDequeIterator"));

  OBRelease((OBObjType *)instance->node);

  return;
}


/* OBDeque Private Methods */

/* add arguments to complete initialization as needed, modify 
 * OBDeque_Private.h as well if modifications are made */
OBDeque * OBDequeCreateDefault(void){

  static const char classname[] = "OBDeque";
  OBDeque *new_instance = malloc(sizeof(OBDeque));
  assert(new_instance != NULL);

  /* initialize base class data */
  OBInitBase((OBObjType *)new_instance, &OBDequeDealloc, &OBDequeHash, &OBDequeCompare,
           &OBDequeDisplay, classname);

  new_instance->head = NULL;
  new_instance->tail = NULL;
  new_instance->length = 0;

  return new_instance;
}


/* private recursive sort method uses stack variables to take advantage of
 * static memory management */
OBDeque OBDequeSortRecursive(OBDeque deque, int8_t order, obcompare_fptr funct){

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
  right_sorted = OBDequeSortRecursive(right_sorted, order, funct);
  left_sorted = OBDequeSortRecursive(left_sorted, order, funct);

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


obhash_t OBDequeHash(OBTypeRef to_hash){

  static int8_t init = 0;
  obhash_t value;
  static obhash_t seed = 0;
  OBDeque *instance = (OBDeque *)to_hash;
  OBDequeIterator *it;

  assert(to_hash);
  assert(OBObjIsOfClass(to_hash, "OBDeque"));
  
  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;

  it = OBDequeGetHeadIterator(instance);
  if(!it) return value;

  do{
    value += OBHash(OBDequeGetObjectAtIterator(instance, it));
    value += value << 10;
    value ^= value >> 6;
  }while(OBDequeIterateNext(instance, it));

  OBRelease((OBObjType *)it);

  value += value << 3;
  value ^= value >> 11;
  value += value << 15;

  return value;
}


int8_t OBDequeCompare(OBTypeRef a, OBTypeRef b){

  int8_t retval = OB_EQUAL_TO; /* assume equality and disprove if not */
  const OBDeque *comp_a = (OBDeque *)a;
  const OBDeque *comp_b = (OBDeque *)b;
  OBDequeIterator *a_it, *b_it;

  assert(a);
  assert(b);
  assert(OBObjIsOfClass(a, "OBDeque"));
  assert(OBObjIsOfClass(b, "OBDeque"));

  if(comp_a->length != comp_b->length) return OB_NOT_EQUAL;

  a_it = OBDequeGetHeadIterator(comp_a);
  b_it = OBDequeGetHeadIterator(comp_b);

  if(!a_it && !b_it) return OB_EQUAL_TO;

  if(a_it && b_it){
    do{
      if(OBCompare(OBDequeGetObjectAtIterator(comp_a, a_it), OBDequeGetObjectAtIterator(comp_b, b_it)) !=
       OB_EQUAL_TO){
      retval = OB_NOT_EQUAL;
      break;
    }
    }while(OBDequeIterateNext(comp_a, a_it) && OBDequeIterateNext(comp_b, b_it));
  }
  else retval = OB_NOT_EQUAL;

  OBRelease((OBObjType *)a_it);
  OBRelease((OBObjType *)b_it);
  
  return retval;
}

void OBDequeDisplay(OBTypeRef to_print){
  
  OBDeque *d = (OBDeque *)to_print;
  OBDequeIterator *it;

  assert(to_print != NULL);
  assert(OBObjIsOfClass(to_print, "OBDeque"));
  fprintf(stderr, "OBDeque with %llu elements\n"
                  "  [deque head]", OBDequeGetLength(d));

  it = OBDequeGetHeadIterator(d);

  if(!it) return;

  do{
    OBDisplay(OBDequeGetObjectAtIterator(d, it));
    fprintf(stderr, "\n");
  }while(OBDequeIterateNext(d, it));

  OBRelease((OBObjType *)it);

  fprintf(stderr, "  [deque tail]\n");

  return;
}


void OBDequeDealloc(OBTypeRef to_dealloc){

  /* cast generic obj to OBDeque */
  OBDeque *instance = (OBDeque *)to_dealloc;

  assert(to_dealloc);
  assert(OBObjIsOfClass(to_dealloc, "OBDeque"));

  OBDequeClear(instance);

  return;
}

