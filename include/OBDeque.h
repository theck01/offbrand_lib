
/**
 * @file OBDeque.h
 * @brief OBDeque Public Interface
 * @author
 */

#ifndef OBDEQUE_H
#define OBDEQUE_H

#include "offbrand.h"

/** Class type declaration */
typedef struct OBDeque_struct OBDeque;

/** Class specific iterator type declaration. */
typedef struct OBDequeIterator_struct OBDequeIterator;


/* PUBLIC METHODS */

/**
 * @brief Constructor, creates a new instance of OBDeque with no contents
 * @return Pointer to a newly created and initialized instance of OBDeque
 */
OBDeque * createDeque(void);

/**
 * @brief Copy Constructor, creates a new OBDeque that contains the same
 * contents as an OBDeque instance
 *
 * @param to_copy An instance of OBDeque to copy
 *
 * @return Pointer to a newly created and initialized instance of OBDeque
 */
OBDeque * copyDeque(const OBDeque *to_copy);

/**
 * @brief The emptyness state of an OBDeque, either empty or non-empty
 * 
 * @param deque An instance of OBDeque
 *
 * @retval 0 The deque is empty
 * @retval non-zero The deque contains elements
 */
uint8_t isDequeEmpty(const OBDeque *deque);

/**
 * @brief Number of elements stored within an OBDeque
 *
 * @param deque An instance of OBDeque
 *
 * @return An integer corresponding the the number of objects in the deque
 */
uint64_t dequeLength(const OBDeque *deque);

/**
 * @brief Constructor for an iterator for an OBDeque that is directed at the
 * head of the OBDeque.
 *
 * @param deque An instance of OBDeque
 *
 * @retval NULL The OBDeque provided is empty and cannot be iterated
 * @retval non-NULL An OBDequeIterator instance bound to the provided instance 
 * of OBDeque and directed at the head of that OBDeque
 *
 * @warning The OBDequeIterator created by this method MUST be released by the
 * user, else a memory leak will occur
 */
OBDequeIterator * getDequeHeadIt(const OBDeque *deque);

/**
 * @brief Constructor for an iterator for an OBDeque that is directed at the
 * tail of the OBDeque.
 *
 * @param deque An instance of OBDeque
 *
 * @retval NULL The OBDeque provided is empty and cannot be iterated
 * @retval non-NULL An OBDequeIterator instance bound to the provided instance 
 * and directed at the tail of that OBDeque
 *
 * @warning The OBDequeIterator created by this method MUST be released by the
 * user, else a memory leak will occur
 */
OBDequeIterator * getDequeTailIt(const OBDeque *deque);

/**
 * @brief Copy Constructor for an OBDequeIterator from another OBDequeIterator
 *
 * @param it An instance of OBDequeIterator
 *
 * @return An OBDequeIterator instance bound to the same OBDeque as the provided
 * OBDequeIterator and directect at the same location within that OBDeque
 *
 * @warning The OBDequeIterator created by this method MUST be released by the
 * user, else a memory leak will occur
 */
OBDequeIterator * copyDequeIterator(const OBDequeIterator *it);

/**
 * @brief Advance an OBDequeIterator bound to an OBDeque to the next element
 * within the OBDeque closer to the tail of the OBDeque
 *
 * @param deque An instance of OBDeque
 * @param it An intstance of OBDequeIterator that is bound to deque
 *
 * @retval non-zero Advancement was successful.
 * @retval 0 Advancement failed because no more elements exist in the OBDeque
 * closer to the deque tail
 */
uint8_t iterateDequeNext(const OBDeque *deque, OBDequeIterator *it);

/**
 * @brief Advance an OBDequeIterator bound to an OBDeque to the previous element
 * within the OBDeque closer to the OBDeque head
 *
 * @param deque An instance of OBDeque
 * @param it An intstance of OBDequeIterator that is bound to deque
 *
 * @retval non-zero Advancement was successful.
 * @retval 0 Advancement failed because no more elements exist in the OBDeque
 * closer to the deque head
 */
uint8_t iterateDequePrev(const OBDeque *deque, OBDequeIterator *it);

/**
 * @brief Add an obj to the head of an OBDeque
 *
 * @param deque An instance of OBDeque
 * @param to_add Any instance of an Offbrand compatible class to add to deque
 */
void addDequeHead(OBDeque *deque, OBObjType *to_add);

/**
 * @brief Add an obj to the tail of an OBDeque
 *
 * @param deque An instance of OBDeque
 * @param to_add Any instance of an Offbrand compatible class to add to deque
 */
void addDequeTail(OBDeque *deque, OBObjType *to_add);

/**
 * @brief Add an obj to an OBDeque at position before the element specified by
 * an OBDequeIterator
 *
 * @param deque An instance of OBDeque
 * @param it An instance of OBDequeIterator bound to deque
 * @param to_add Any instance of an Offbrand compatible class to add to deque
 */
void addAtDequeIt(OBDeque *deque, OBDequeIterator *it, OBObjType *to_add);

/**
 * @brief creates a new OBDeque that contains the ordered contents of two 
 * OBDeques
 * 
 * @param d1 First OBDeque to copy into the resultant OBDeque
 * @param d2 Second OBDeque to copy onto the end of the resultant OBDeque
 *
 * @return An OBDeque instance that contains the contents of d1 followed by d2
 */
OBDeque * joinDeques(const OBDeque *d1, const OBDeque *d2);

/**
 * @brief Searches an OBDeque for an obj
 *
 * @param deque An instance of OBDeque
 * @param to_find An instance of any Offbrand compatible class to search for
 * within the deque
 *
 * @retval 0 to_find not found within deque
 * @retval 1 to_find found within the deque
 */
uint8_t findObjInDeque(const OBDeque *deque, const OBObjType *to_find);

/**
 * @brief Sorts an OBDeque from least-to-greatest or greatest-to-least using
 * the standard compare function
 *
 * @param deque An instance of OBDeque
 * @param order Accepts OB_LEAST_TO_GREATEST or OB_GREATEST_TO_LEAST as valid
 * sorting orders
 *
 * @warning If called on an OBDeque containing instances of multiple Offbrand
 * compatible classes the call will likely not sort members in any expected
 * order, but will reorder components
 */
void sortDeque(OBDeque *deque, int8_t order);

/**
 * @brief Sorts an OBDeque from least-to-greatest or greatest-to-least using
 * a specified comparision function
 *
 * @param deque An instance of OBDeque
 * @param order Accepts OB_LEAST_TO_GREATEST or OB_GREATEST_TO_LEAST as valid
 * sorting orders
 * @param funct A pointer to a comparision function that returns a int8_t when
 * given two obj * arguments
 */
void sortDequeWithFunct(OBDeque *deque, int8_t order, obcompare_fptr funct);

/**
 * @brief Peek at the obj stored at the head of an OBDeque
 *
 * @param deque An instance of OBDeque
 *
 * @retval NULL No elements exist within deque
 * @retval non-NULL The element stored at the head of deque
 */
OBObjType * objAtDequeHead(const OBDeque *deque);

/**
 * @brief Peek at the obj stored at the tail of an OBDeque
 *
 * @param deque An instance of OBDeque
 *
 * @retval NULL No elements exist within deque
 * @retval non-NULL The element stored at the tail of deque
 */
OBObjType * objAtDequeTail(const OBDeque *deque);

/**
 * @brief Peek at the obj stored within a OBDeque stored at the position denoted
 * by an OBDequeIterator bound to that OBDeque
 *
 * @param deque An instance of OBDeque
 * @param it An instance of OBDequeIterator bound to deque
 *
 * @retval NULL No elements exist within deque
 * @retval non-NULL The element stored at the tail of deque
 */
OBObjType * objAtDequeIt(const OBDeque *deque, const OBDequeIterator *it);


/**
 * @brief Remove the obj stored at the head of an OBDeque, shrinking the OBDeque
 * by one element
 * @param deque An instance of OBDeque
 */
void removeDequeHead(OBDeque *deque);

/**
 * @brief Remove the obj stored at the tail of an OBDeque, shrinking the OBDeque
 * by one element
 * @param deque An instance of OBDeque
 */
void removeDequeTail(OBDeque *deque);

/**
 * @brief Remove the obj stored within an OBDeque at the position denoted by the
 * OBDequeIterator bound to that OBDeque, shrinking the OBDeque by one element
 *
 * @param deque An instance of OBDeque
 * @param it An instance of OBDequeIterator bound to deque
 *
 * @details The provided OBDequeIterator will be advanced toward the OBDeque
 * tail, unless removing the tail then it will be advanced to the element before
 * the tail (NULL if no more elements exist)
 */
void removeDequeAtIt(OBDeque *deque, OBDequeIterator *it);

/**
 * @brief removes all obj's from the Deque, leaving the Deque empty
 * @param deque An instance of OBDeque
 */
void clearDeque(OBDeque *deque);

#endif
