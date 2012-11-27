
/*
 * OBDeque:
 * A double ended queue class that can be used as is and also forms the basis 
 * for the OBStack and OBQueue
 */

#ifndef OBDEQUE_H
#define OBDEQUE_H

#include "offbrand.h"

/* Class type declaration */
typedef struct OBDeque_struct OBDeque;

/* iterator type declaration. */
typedef struct OBDequeIterator OBDequeIterator;


/* PUBLIC METHODS */

/* constructor, creates a new OBDeque with not contents and a reference count
 * of 1 */
OBDeque * createEmptyDeque(void);

/* copy constructor, creates a new OBDeque with its own memory (a deep copy)
 * that references the same objs */
OBDeque * copyDeque(OBDeque *to_copy);

/* returns an OBDequeIterator representing the head of the Deque, returns
 * NULL if the deque is empty */
OBDequeIterator * getDequeHeadIt(OBDeque *deque);

/* returns an OBDequeIterator representing the tail of the Deque, returns
 * NULL if the deque is empty */
OBDequeIterator * getDequeTailIt(OBDeque *deque);

/* updates the OBDequeIterator to the next position in the deque closer to the
 * tail, returns 0 if there are no elements closer to the tail, 1 if there
 * are more elements closer to the tail. */
uint8_t nextDequeIterator(OBDequeIterator *it);

/* updates the OBDequeIterator to the next position in the deque closer to the
 * head, returns 0 if there are no elements closer to the head, 1 if there are
 * more elements closer to the head */
uint8_t prevDequeIterator(OBDequeIterator *it);

/* add obj to Deque head, retaining the obj once */
void addAtDequeHead(OBDeque *deque, obj *to_add);

/* add obj to Deque tail, retaining the obj once */
void addAtDequeTail(OBDeque *deque, obj *to_add);

/* add obj to Deque at the iterator, pushing everything including the obj at the
 * iterator one position toward the tail. If iterator is NULL then add to the 
 * tail of the Deque. The iterator then points to the same position in the Deque
 * (the position of the newly added obj). Retains obj once */
void addAtDequeIt(OBDeque *deque, OBDequeIterator *it, obj *to_add);

/* peek at the obj stored in the Deque head. Returns a pointer to the actual
 * object, do not dereference unless the calling code already has a reference
 * that it would like to get rid of */
obj * peekDequeHead(OBDeque *deque);

/* peek at the obj stored in the Deque tail. Returns a pointer to the actual
 * object, do not dereference unless the calling code already has a reference
 * that it would like to get rid of */
obj * peekDequeTail(OBDeque *deque);

/* peek at the obj stored in the Deque at the position indicated by the iterator
 * Returns a pointer to the actual object, do not dereference unless the calling
 * code already has a reference that it would like to get rid of */
obj * peekDequeAtIt(OBDeque *deque, OBDequeIterator *it);

/* remove obj from Deque head, releasing that obj and returning it if it still
 * has a reference, NULL if not */
obj * removeDequeHead(OBDeque *deque);

/* remove obj from Deque tail, releasing that obj and returning it if it still
 * has a reference, NULL if not */
obj * removeDequeTail(OBDeque *deque);

/* remove obj form Deque at the position indicated by the iterator, returning
 * the obj at that location if it has a reference, NULL if not. The iterator
 * points to the next obj in the queue after the call (closer to the tail) */
obj * removeDequeAtIt(OBDeque *deque, OBDequeIterator *it);

#endif

