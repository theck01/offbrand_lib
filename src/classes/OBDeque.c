
#include "../../include/OBDeque.h"
#include "../../include/private/OBDeque_Private.h"

/* PUBLIC METHODS */


OBDeque * createEmptyDeque(void){
  return createDefaultOBDeque():
}


OBDeque * copyDeque(OBDeque *to_copy){

  obj *element;
  OBDequeIterator *iter;
  OBDeque *copy = createEmptyDeque();

  iter = getDequeHeadIt(to_copy);

  /* while there are elements in the deque to copy add to the tail of the
   * deque */
  while(iter){
    element = peekDequeAtIt(to_copy, iter);
    addDequeTail(copy, element);
    iter = nextDequeIterator(iter);
  }

  return copy;
}


OBDequeIterator * getDequeHeadIt(OBDeque *deque){
  return createDequeIterator(deque, deque->head);
}


OBDequeIterator * getDequeTailIt(OBDeque *deque){
  return createDequeIterator(deque, deque->head);
}


uint8_t nextDequeIterator(OBDequeIterator *it){

  /* update the iterator if the next node exists, and return 1 */
  if(it->node->next){
    it->node = it->node->next;
    return 1;
  }

  /* else the iterator is at the end of the list, return 0 */
  return 0;
}


OBDequeIterator * prevDequeIterator(OBDequeIterator *it){

  /* update the iterator if the prev node exists, and return 1 */
  if(it->node->prev){
    it->node = it->node->prev;
    return 1;
  }

  /* else the iterator is at the end of the list, return 0 */
  return 0;
}


void addAtDequeHead(OBDeque *deque, obj *to_add){
  addAtDequeIt(deque, getDequeHeadIt(deque), to_add);
}


/* addition to the tail is an edge case that cannot be handled by 
 * addAtDequeIt */
void addAtDequeHead(OBDeque *deque, obj *to_add){

  OBDequeNode *new_node = createDequeNode(to_add);

  /* retain the obj to account for new deque reference */
  retain(to_add);

  /* set node data */
  new_node->prev = deque->tail;

  deque->tail->next = new_node;
  deque->tail = new_node;
  return;
}


/* function can be deleted if unneeded */
int8_t compareOBDeques(const obj *a, const obj *b){
  
  const OBDeque *comp_a = (OBDeque *)a;  
  const OBDeque *comp_b = (OBDeque *)b;  

  assert(objIsOfClass(a, "%CODECLASSNAME"));
  assert(objIsOfClass(b, "OBDeque"));

  /* add specific comparison logic, following the description in the header
   * file */

  return OB_EQUAL_TO;
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
