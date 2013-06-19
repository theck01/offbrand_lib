
/**
 * @file obdeque.h
 * @brief obdeque Public Interface
 * @author
 */

#ifndef OBDEQUE_H
#define OBDEQUE_H

#include "offbrand.h"

/** Class type declaration */
typedef struct obdeque_struct obdeque;

/** Class specific iterator type declaration. */
typedef struct obdeque_iterator_struct obdeque_iterator;


/* PUBLIC METHODS */

/**
 * @brief Constructor, creates a new instance of obdeque with no contents
 * @return Pointer to a newly created and initialized instance of obdeque
 */
obdeque * obdeque_new(void);

/**
 * @brief Copy Constructor, creates a new obdeque that contains the same
 * contents as an obdeque instance
 *
 * @param to_copy An instance of obdeque to copy
 *
 * @return Pointer to a newly created and initialized instance of obdeque
 */
obdeque * obdeque_copy(const obdeque *to_copy);

/**
 * @brief The emptyness state of an obdeque, either empty or non-empty
 *
 * @param deque An instance of obdeque
 *
 * @retval 0 The deque is empty
 * @retval non-zero The deque contains elements
 */
uint8_t obdeque_is_empty(const obdeque *deque);

/**
 * @brief Number of elements stored within an obdeque
 *
 * @param deque An instance of obdeque
 *
 * @return An integer corresponding the the number of objects in the deque
 */
uint64_t obdeque_length(const obdeque *deque);

/**
 * @brief Constructor for an iterator for an obdeque that is directed at the
 * head of the obdeque.
 *
 * @param deque An instance of obdeque
 *
 * @retval NULL The obdeque provided is empty and cannot be iterated
 * @retval non-NULL An obdeque_iterator instance bound to the provided instance
 * of obdeque and directed at the head of that obdeque
 *
 * @warning The obdeque_iterator created by this method MUST be released by the
 * user, else a memory leak will occur
 */
obdeque_iterator * obdeque_head_iterator(const obdeque *deque);

/**
 * @brief Constructor for an iterator for an obdeque that is directed at the
 * tail of the obdeque.
 *
 * @param deque An instance of obdeque
 *
 * @retval NULL The obdeque provided is empty and cannot be iterated
 * @retval non-NULL An obdeque_iterator instance bound to the provided instance
 * and directed at the tail of that obdeque
 *
 * @warning The obdeque_iterator created by this method MUST be released by the
 * user, else a memory leak will occur
 */
obdeque_iterator * obdeque_tail_iterator(const obdeque *deque);

/**
 * @brief Copy Constructor for an obdeque_iterator from another obdeque_iterator
 *
 * @param it An instance of obdeque_iterator
 *
 * @return An obdeque_iterator instance bound to the same obdeque as the
 * provided obdeque_iterator and directect at the same location within that
 * obdeque
 *
 * @warning The obdeque_iterator created by this method MUST be released by the
 * user, else a memory leak will occur
 */
obdeque_iterator * obdeque_copy_iterator(const obdeque_iterator *it);

/**
 * @brief Advance an obdeque_iterator bound to an obdeque to the next element
 * within the obdeque closer to the tail of the obdeque
 *
 * @param deque An instance of obdeque
 * @param it An intstance of obdeque_iterator that is bound to deque
 *
 * @retval non-zero Advancement was successful.
 * @retval 0 Advancement failed because no more elements exist in the obdeque
 * closer to the deque tail
 */
uint8_t obdeque_iterate_next(const obdeque *deque, obdeque_iterator *it);

/**
 * @brief Advance an obdeque_iterator bound to an obdeque to the previous element
 * within the obdeque closer to the obdeque head
 *
 * @param deque An instance of obdeque
 * @param it An intstance of obdeque_iterator that is bound to deque
 *
 * @retval non-zero Advancement was successful.
 * @retval 0 Advancement failed because no more elements exist in the obdeque
 * closer to the deque head
 */
uint8_t obdeque_iterate_prev(const obdeque *deque, obdeque_iterator *it);

/**
 * @brief Add an obj to the head of an obdeque
 *
 * @param deque An instance of obdeque
 * @param to_add Any instance of an Offbrand compatible class to add to deque
 */
void obdeque_add_at_head(obdeque *deque, obj *to_add);

/**
 * @brief Add an obj to the tail of an obdeque
 *
 * @param deque An instance of obdeque
 * @param to_add Any instance of an Offbrand compatible class to add to deque
 */
void obdeque_add_at_tail(obdeque *deque, obj *to_add);

/**
 * @brief Add an obj to an obdeque at position before the element specified by
 * an obdeque_iterator
 *
 * @param deque An instance of obdeque
 * @param it An instance of obdeque_iterator bound to deque
 * @param to_add Any instance of an Offbrand compatible class to add to deque
 */
void obdeque_add_at_iterator(obdeque *deque, obdeque_iterator *it,
                             obj *to_add);

/**
 * @brief creates a new obdeque that contains the ordered contents of two
 * obdeques
 *
 * @param d1 First obdeque to copy into the resultant obdeque
 * @param d2 Second obdeque to copy onto the end of the resultant obdeque
 *
 * @return An obdeque instance that contains the contents of d1 followed by d2
 */
obdeque * obdeque_join(const obdeque *d1, const obdeque *d2);

/**
 * @brief Searches an obdeque for an obj
 *
 * @param deque An instance of obdeque
 * @param to_find An instance of any Offbrand compatible class to search for
 * within the deque
 *
 * @retval 0 to_find not found within deque
 * @retval 1 to_find found within the deque
 */
uint8_t obdeque_find_obj(const obdeque *deque, const obj *to_find);

/**
 * @brief Sorts an obdeque from least-to-greatest or greatest-to-least using
 * the standard compare function
 *
 * @param deque An instance of obdeque
 * @param order Accepts OB_LEAST_TO_GREATEST or OB_GREATEST_TO_LEAST as valid
 * sorting orders
 *
 * @warning If called on an obdeque containing instances of multiple Offbrand
 * compatible classes the call will likely not sort members in any expected
 * order, but will reorder components
 */
void obdeque_sort(obdeque *deque, int8_t order);

/**
 * @brief Sorts an obdeque from least-to-greatest or greatest-to-least using
 * a specified comparision function
 *
 * @param deque An instance of obdeque
 * @param order Accepts OB_LEAST_TO_GREATEST or OB_GREATEST_TO_LEAST as valid
 * sorting orders
 * @param funct A pointer to a comparision function that returns a int8_t when
 * given two obj * arguments
 */
void obdeque_sort_with_funct(obdeque *deque, int8_t order,
                             ob_compare_fptr funct);

/**
 * @brief Peek at the obj stored at the head of an obdeque
 *
 * @param deque An instance of obdeque
 *
 * @retval NULL No elements exist within deque
 * @retval non-NULL The element stored at the head of deque
 */
obj * obdeque_obj_at_head(const obdeque *deque);

/**
 * @brief Peek at the obj stored at the tail of an obdeque
 *
 * @param deque An instance of obdeque
 *
 * @retval NULL No elements exist within deque
 * @retval non-NULL The element stored at the tail of deque
 */
obj * obdeque_obj_at_tail(const obdeque *deque);

/**
 * @brief Peek at the obj stored within a obdeque stored at the position denoted
 * by an obdeque_iterator bound to that obdeque
 *
 * @param deque An instance of obdeque
 * @param it An instance of obdeque_iterator bound to deque
 *
 * @retval NULL No elements exist within deque
 * @retval non-NULL The element stored at the tail of deque
 */
obj * obdeque_obj_at_iterator(const obdeque *deque, const obdeque_iterator *it);


/**
 * @brief Remove the obj stored at the head of an obdeque, shrinking the obdeque
 * by one element
 * @param deque An instance of obdeque
 */
void obdeque_remove_head(obdeque *deque);

/**
 * @brief Remove the obj stored at the tail of an obdeque, shrinking the obdeque
 * by one element
 * @param deque An instance of obdeque
 */
void obdeque_remove_tail(obdeque *deque);

/**
 * @brief Remove the obj stored within an obdeque at the position denoted by the
 * obdeque_iterator bound to that obdeque, shrinking the obdeque by one element
 *
 * @param deque An instance of obdeque
 * @param it An instance of obdeque_iterator bound to deque
 *
 * @details The provided obdeque_iterator will be advanced toward the obdeque
 * tail, unless removing the tail then it will be advanced to the element before
 * the tail (NULL if no more elements exist)
 */
void obdeque_remove_at_iterator(obdeque *deque, obdeque_iterator *it);

/**
 * @brief removes all obj's from the Deque, leaving the Deque empty
 * @param deque An instance of obdeque
 */
void obdeque_clear(obdeque *deque);

#endif
