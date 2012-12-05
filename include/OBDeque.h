
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
typedef struct OBDequeIterator_struct OBDequeIterator;


/* PUBLIC METHODS */

/* constructor, creates a new OBDeque with not contents and a reference count
 * of 1 */
OBDeque * createEmptyDeque(void);

/* copy constructor, creates a new OBDeque with its own memory (a deep copy)
 * that references the same objs */
OBDeque * copyDeque(const OBDeque *to_copy);

/* returns non-zero if the deque is empty, and 0 if it contains elements */
uint8_t isDequeEmpty(const OBDeque *deque);

/* returns the number of elements contained within the deque */
uint64_t dequeLength(const OBDeque *deque);

/* returns an OBDequeIterator representing the head of the Deque, returns
 * NULL if the deque is empty */
OBDequeIterator * getDequeHeadIt(const OBDeque *deque);

/* returns an OBDequeIterator representing the tail of the Deque, returns
 * NULL if the deque is empty */
OBDequeIterator * getDequeTailIt(const OBDeque *deque);

/* updates the OBDequeIterator to the next position in the deque closer to the
 * tail, returns 0 if there are no elements closer to the tail, 1 if there
 * are more elements closer to the tail. */
uint8_t iterateDequeNext(OBDequeIterator *it);

/* updates the OBDequeIterator to the next position in the deque closer to the
 * head, returns 0 if there are no elements closer to the head, 1 if there are
 * more elements closer to the head */
uint8_t iterateDequePrev(OBDequeIterator *it);

/* add obj to Deque head, retaining the obj once */
void addAtDequeHead(OBDeque *deque, obj *to_add);

/* add obj to Deque tail, retaining the obj once */
void addAtDequeTail(OBDeque *deque, obj *to_add);

/* add obj to Deque at the iterator, pushing everything including the obj at the
 * iterator one position toward the tail. If iterator is NULL then add to the 
 * tail of the Deque. The iterator then points to the same position in the Deque
 * (the position of the newly added obj). Retains obj once */
void addAtDequeIt(OBDeque *deque, OBDequeIterator *it, obj *to_add);

/* joins d1 and d2 into one deque by appending the contents of d2 to the end of
 * d1, and returning the result as a new deque */
OBDeque * joinDeques(const OBDeque *d1, const OBDeque *d2);

/* function returns 1 if the provided object is found within the deque, 0 if
 * not. If using a specific class comparision operator (other than the default
 * obj comparision) then the deque must contain only that class of objects. The
 * function will fail assertions if the deque is a heterogenous collection. If
 * the comparition function pointer is null then the default pointer comparision
 * will be used */
uint8_t findObjInDeque(const OBDeque *deque, const obj *to_find,
                       compare_fptr compare);

/* sorts queue using the compare function passed in as an argument. ASSUMES
 * THAT ALL CONTAINED OBJ ARE OF THE SAME CLASS, assertions will fail if not.
 * Order (lowest to highest or highest to lowest) specified by sorting macros
 * defined in offbrand.h. Uses the merge sort algorithm. */
void sortDeque(OBDeque *deque, compare_fptr compare, const int8_t order);

/* peek at the obj stored in the Deque head. Returns a pointer to the actual
 * object, do not dereference unless the calling code already has a reference
 * that it would like to get rid of */
obj * peekDequeHead(const OBDeque *deque);

/* peek at the obj stored in the Deque tail. Returns a pointer to the actual
 * object, do not dereference unless the calling code already has a reference
 * that it would like to get rid of */
obj * peekDequeTail(const OBDeque *deque);

/* peek at the obj stored in the Deque at the position indicated by the iterator
 * Returns a pointer to the actual object, do not dereference unless the calling
 * code already has a reference that it would like to get rid of */
obj * peekDequeAtIt(const OBDeque *deque, const OBDequeIterator *it);

/* remove obj from Deque head, releasing that obj and returning it if it still
 * has a reference, NULL if not */
void removeDequeHead(OBDeque *deque);

/* remove obj from Deque tail, releasing that obj and returning it if it still
 * has a reference, NULL if not */
void removeDequeTail(OBDeque *deque);

/* remove obj form Deque at the position indicated by the iterator, returning
 * the obj at that location if it has a reference, NULL if not. The iterator
 * points to the next obj in the queue after the call (closer to the tail) */
void removeDequeAtIt(OBDeque *deque, OBDequeIterator *it);

/* removes all obj's from the Deque, leaving the Deque empty */
void clearDeque(OBDeque *deque);

#endif
