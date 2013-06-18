/**
 * @file OBDeque_Private.h
 * @brief OBDeque Private Interace
 * @author theck
 */

#ifndef OBDEQUE_PRIVATE_H
#define OBDEQUE_PRIVATE_H

#include "../OBDeque.h"

/* OBDequeNode TYPE */

/**
 * @brief OBDequeNode internal structure, encapsulating the data needed for an
 * node within a doubly linked list. Internally referenced only
 */
typedef struct OBDequeNode_struct{
  OBObjType base; /**< obj containing reference count and class membership data */
  OBTypeRef stored; /**< obj stored within the node in the deque */
  struct OBDequeNode_struct *next; /**< Pointer to the next node in the list */
  struct OBDequeNode_struct *prev; /**< Pointer to the prev node in the list */
} OBDequeNode;

/* OBDequeNode PRIVATE METHODS */

/**
 * @brief Constructor, creates a new OBDequeNode containing an obj
 *
 * @param to_store A non-NULL instance of any Offbrand compatible class
 *
 * @return A new instance of OBDeque node storing to_store and with NULL
 * references to next and prev nodes
 */
OBDequeNode * OBDequeNodeCreate(OBTypeRef to_store);

/**
 * @brief Destructor for OBDequeNode
 * @param to_dealloc An obj pointer to an instance of OBDequeNode with reference
 * count 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void OBDequeNodeDealloc(OBTypeRef to_dealloc);


/* OBDequeIterator type */

/**
 * @brief OBDequeNodeIterator internal structure, encapsulating the data needed
 * for an iterator of a doubly linked list.
 */
struct OBDequeIterator_struct{
  OBObjType base; /**< obj containing reference count and class membership data */
  const OBDeque *deque; /**< OBDeque that the iterator is bound to */
  OBDequeNode *node; /**< The OBDequeNode that the iterator references within
                       deque */
};

/* OBDequeIterator Private Methods */

/**
 * @brief Constructor, creates an instance of OBDequeIterator bound to an
 * OBDeque and an OBDequeNode within that deque
 *
 * @param deque An instance of OBDeque
 * @param node An instance of OBDequeNode contained within deque
 *
 * @return An instance of OBDequeIterator
 */
struct OBDequeIterator_struct * OBDequeIteratorCreate(const OBDeque *deque, 
                                                    OBDequeNode *node);

/**
 * @brief Destructor for OBDequeIterator
 * @param to_dealloc An obj pointer to an instance of OBDequeIterator with 
 * reference count 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void OBDequeIteratorDealloc(OBTypeRef to_dealloc);


/* OBDeque Type */

/**
 * @brief OBDeque internal structure, encapsulating the data needed for a 
 * doubly linked list
 */
struct OBDeque_struct{
  OBObjType base; /**< obj containing reference count and class membership data */
  OBDequeNode *head; /**< pointer to the OBDequeNode at the head of the deque */
  OBDequeNode *tail; /**< pointer to the OBDequeNode at the tail of the deque */
  uint64_t length; /**< integer length of the deque (or number of elements
                     stored within) */
};


/* OBDeque Private Methods */

/**
 * @brief Default constructor for OBDeque
 *
 * @return An instance of class OBDeque
 *
 * @warning All public constructors should call this constructor and initialize
 * individual members as needed, so that all base data is initialized properly
 */
OBDeque * OBDequeCreateDefault(void);

/**
 * @brief Internal merge sort implementation for an OBDeque
 *
 * @param deque Static OBDeque to be sorted
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
OBDeque OBDequeSortRecursive(OBDeque deque, int8_t order, obcompare_fptr funct);

/**
 * @brief Hash function for OBDeque
 * @param to_hash An obj pointer to an instance of OBDeque
 * @return Key value (hash) for the given obj pointer to a OBDeque
 */
obhash_t OBDequeHash(OBTypeRef to_hash);

/**
 * @brief Compares two instances of OBDeque
 *
 * @param a A non-NULL obj pointer to type OBDeque
 * @param b A non-NULL obj pointer to type OBDeque
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t OBDequeCompare(OBTypeRef a, OBTypeRef b);

/**
 * @brief Displays information about an OBDeque to stderr
 *
 * @param to_print A non-NULL obj pointer to type OBDeque
 */
void OBDequeDisplay(OBTypeRef to_print);

/**
 * @brief Destructor for OBDeque
 * @param to_dealloc An obj pointer to an instance of OBDeque with 
 * reference count 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void OBDequeDealloc(OBTypeRef to_dealloc);


#endif
