
#include "../../include/OBDeque.h"
#include "../../include/private/OBDeque_Private.h"

/* PUBLIC METHODS */


OBDeque * createEmptyDeque(void){
  return createDefaultOBDeque():
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
  } while(iterateDequeNext(iter));

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
  return createDequeIterator(deque, deque->head);
}


uint8_t iterateDequeNext(OBDequeIterator *it){

  assert(it);

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


uint8_t iterateDequePrev(OBDequeIterator *it){

  assert(it);

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


void addAtDequeHead(OBDeque *deque, obj *to_add){
  
  OBDequeNode *new_node;
  
  assert(deque);
  assert(to_add);

  new_node = createDequeNode(to_add);

  /* retain the obj to account for new deque reference */
  retain(to_add);

  /* set node data */
  new_node->next = deque->head;

  /* update deque data */
  /* if a head aleady exists update head prev */
  if(deque->head) deque->head->prev = new_node;
  else deque->tail = new_node /* else set tail, which should not be NULL 
                                 anymore*/
  deque->head = new_node;
  deque->length++;

  return;
}


void addAtDequeHead(OBDeque *deque, obj *to_add){

  OBDequeNode *new_node;
  
  assert(deque);
  assert(to_add);

  new_node = createDequeNode(to_add);

  /* retain the obj to account for new deque reference */
  retain(to_add);

  /* set node data */
  new_node->prev = deque->tail;

  /* update deque data */
  /* if a tail aleady exists update tail next */
  if(deque->tail) deque->tail->next = new_node;
  else deque->head = new_node /* else set head, which should not be NULL
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

  new_node = createDequeNode(to_add);

  /* retain the object to account for new deque reference */
  retain(to_add);

  /* set node data */
  new_node->prev = it->node->prev;
  new_node->next = it->node;
  it->node->prev->next = new_node;
  it->node->prev = new_node;

  deque->length++;

  /* update iterator to newly inserted node */
  assert(iterateDequePrev(it));

  /* update deque data if the iterator is pointing at the head (added node
   * cannot be the tail) */
  if(it->node == deque->head) deque->head = new_node;

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
    addDequeTail(copy, element);
  } while(iterateDequeNext(it));

  release((obj *)it);

  return joined;
}

uint8_t findObjInDeque(const OBDeque *deque, const obj *to_find,
                       compare_fptr compare){

  OBDequeIterator *it;

  assert(deque);
  assert(to_find);
  if(!compare) compare = &objCompare;

  it = getDequeHeadIt(deque);
  if(!it) return 0; /* obj is not in an empty list */
  
  do{
    if(compare(peekDequeAtIt(deque, it), to_find) == OB_EQUAL_TO) return 1;
  } while(iterateDequeNext(it));

  return 0;
}


void sortDeque(OBDeque *deque, const compare_fptr compare, const int8_t order){

  compare_fptr compare_funct;
  OBDequeIterator *it;

  assert(deque);

  /* if comparision function was not added use simple pointer comparator */
  if(!compare) compare_funct = &objCompare;
  else compare_funct = compare;

  /* INCOMPLETE  IMPLEMENTATION */
}
  


obj * peekDequeHead(const OBDeque *deque){
  assert(deque);
  return deque->head->stored;
}


obj * peekDequeTail(const OBDeque *deque){
  assert(deque);
  return deque->tail->stored;
}


obj * peekDequeAtIt(const OBDeque *deque, const OBDequeIterator *it){

  assert(deque);
  assert(it);
  assert(it->deque == deque); /* assert that iterator belongs to provided 
                                 deque */
  return it->node->stored;
}


void removeDequeHead(OBDeque *deque){

  OBDequeNode *temp_node;

  assert(deque);

  /* return if the list is empty */
  if(!(temp_node = deque->head)) return;

  /* set tail to NULL if removing the last element from the list */
  if(!(deque->head = deque->head->next)) deque->tail = NULL;

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
  if(!(temp_node = deque->tail)) return NULL;

  /* set head to NULL if removing the last element from the list */
  if(!(deque->tail = deque->tail->prev)) deque->head = NULL;

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

  temp_node = it->node;

  /* if removing the last element from the list */
  if(temp_node == deque->head && temp_node == deque->tail){
    deque->head = NULL;
    deque->tail = NULL;
  }
  else{ /* join list pieces separated by the node */
    temp_node->prev->next = temp_node->next;
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

  deque->length = 0;
  
  return;
}


/* PRIVATE METHODS */

/* add arguments to complete initialization as needed, modify 
 * OBDeque_Private.h as well if modifications are made */
OBDeque * createDefaultDeque(void){

  static const char classname[] = "OBDeque";
  OBDeque *new_instance = malloc(sizeof(OBDeque));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocDeque, NULL, classname);

  /* ADD CLASS SPECIFIC INITIALIZATION HERE */

  return new_instance;
}


void deallocDeque(obj *to_dealloc){

  /* cast generic obj to OBDeque */
  OBDeque *instance = (OBDeque *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBDeque"));

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE BUT DO NOT
   * FREE INSTANCE, THE LIBRARY WILL DO THIS FOR THE USER */

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
