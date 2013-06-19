/**
 * @file obdeque_private.h
 * @brief obdeque Private Interace
 * @author theck
 */

#ifndef OBDEQUE_PRIVATE_H
#define OBDEQUE_PRIVATE_H

#include "../obdeque.h"

/* obdeque_node TYPE */

/**
 * @brief obdeque_node internal structure, encapsulating the data needed for an
 * node within a doubly linked list. Internally referenced only
 */
typedef struct obdeque_node_struct{
  obj base; /**< obj containing reference count and class membership data */
  obj *stored; /**< obj stored within the node in the deque */
  struct obdeque_node_struct *next; /**< Pointer to the next node in the list */
  struct obdeque_node_struct *prev; /**< Pointer to the prev node in the list */
} obdeque_node;

/* obdeque_node PRIVATE METHODS */

/**
 * @brief Constructor, creates a new obdeque_node containing an obj
 *
 * @param to_store A non-NULL instance of any Offbrand compatible class
 *
 * @return A new instance of obdeque node storing to_store and with NULL
 * references to next and prev nodes
 */
obdeque_node * obdeque_new_node(obj *to_store);

/**
 * @brief Destructor for obdeque_node
 * @param to_dealloc An obj pointer to an instance of obdeque_node with
 * reference count 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void obdeque_destroy_node(obj *to_dealloc);


/* obdeque_iterator type */

/**
 * @brief obdeque_nodeIterator internal structure, encapsulating the data needed
 * for an iterator of a doubly linked list.
 */
struct obdeque_iterator_struct{
  obj base; /**< obj containing reference count and class membership data */
  const obdeque *deque; /**< obdeque that the iterator is bound to */
  obdeque_node *node; /**< The obdeque_node that the iterator references within
                       deque */
};

/* obdeque_iterator Private Methods */

/**
 * @brief Constructor, creates an instance of obdeque_iterator bound to an
 * obdeque and an obdeque_node within that deque
 *
 * @param deque An instance of obdeque
 * @param node An instance of obdeque_node contained within deque
 *
 * @return An instance of obdeque_iterator
 */
struct obdeque_iterator_struct * obdeque_new_iterator(const obdeque *deque,
                                                      obdeque_node *node);

/**
 * @brief Destructor for obdeque_iterator
 * @param to_dealloc An obj pointer to an instance of obdeque_iterator with
 * reference count 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void obdeque_destroy_iterator(obj *to_dealloc);


/* obdeque Type */

/**
 * @brief obdeque internal structure, encapsulating the data needed for a
 * doubly linked list
 */
struct obdeque_struct{
  obj base; /**< obj containing reference count and class membership data */
  obdeque_node *head; /**< pointer to the obdeque_node at the head of the deque */
  obdeque_node *tail; /**< pointer to the obdeque_node at the tail of the deque */
  uint64_t length; /**< integer length of the deque (or number of elements
                     stored within) */
};


/* obdeque Private Methods */

/**
 * @brief Default constructor for obdeque
 *
 * @return An instance of class obdeque
 *
 * @warning All public constructors should call this constructor and initialize
 * individual members as needed, so that all base data is initialized properly
 */
obdeque * obdeque_create_default(void);

/**
 * @brief Internal merge sort implementation for an obdeque
 *
 * @param deque Static obdeque to be sorted
 * @param order Accepts OB_LEAST_TO_GREATEST or OB_GREATEST_TO_LEAST as valid
 * sorting orders
 * @param funct A compare_fptr to a function that returns an int8_t when given
 * two obj * arguments
 *
 * @return A sorted static deque
 *
 * @warning There is little to no parameter checking in this function, all
 * sorting should use the publicly accessable function which calls this method
 * internally.
 */
obdeque obdeque_recursive_sort(obdeque deque, int8_t order,
                               ob_compare_fptr funct);

/**
 * @brief Hash function for obdeque
 * @param to_hash An obj pointer to an instance of obdeque
 * @return Key value (hash) for the given obj pointer to a obdeque
 */
ob_hash_t obdeque_hash(const obj *to_hash);

/**
 * @brief Compares two instances of obdeque
 *
 * @param a A non-NULL obj pointer to type obdeque
 * @param b A non-NULL obj pointer to type obdeque
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t obdeque_compare(const obj *a, const obj *b);

/**
 * @brief Displays information about an obdeque to stderr
 *
 * @param to_print A non-NULL obj pointer to type obdeque
 */
void obdeque_display(const obj *to_print);

/**
 * @brief Destructor for obdeque
 * @param to_dealloc An obj pointer to an instance of obdeque with
 * reference count 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void obdeque_destroy(obj *to_dealloc);


#endif
