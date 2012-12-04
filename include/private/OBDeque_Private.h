
/*
 * Private header for OBDeque
 */

#ifndef OBDEQUE_PRIVATE_H
#define OBDEQUE_PRIVATE_H

#include "../OBDeque.h"

#define OBDEQUENODE_NEXT 0
#define OBDEQUENODE_PREV 1

/* OBDequeNode Type */

/* internal data type, represents a single node in the doubly linked list.
 * for internal class use */
typedef struct OBDequeNode_struct{
  obj base;
  obj *stored;
  struct OBDequeNode_struct *next;
  struct OBDequeNode_struct *prev;
} OBDequeNode;

/* OBDequeNode Private Methods */

/* creates a new OBDequeNode with associated object and next/prev pointers
 * set to NULL */
OBDequeNode * createDequeNode(obj *to_store);

/* frees the node while maintaining deque continuity */
void deallocDequeNode(obj *to_dealloc);


/* OBDequeIterator type */

/* external data type, represents an iterator used externally to traverse
 * the deque. */
struct OBDequeIterator_struct{
  obj base;
  OBDeque *deque;
  OBDequeNode *node;
};

/* OBDequeIterator Private Methods */

/* Constructor, builds an iterator for the OBDeque starting at the given node.
 * Retains the DequeNode once, so that as long as the iterator exists and
 * has a reference to that node it will exist. Advancing or regressing the
 * iterator will change which node is being pointed to */
struct OBDequeIterator_struct * createDequeIterator(OBDeque *deque, 
                                                    OBDequeNode *node);

/* Destructor, destroys the iterator and releases the associated deque */
void deallocDequeIterator(obj *to_dealloc);


/* OBDeque Type */

struct OBDeque_struct{
  obj base;
  OBDequeNode *head;
  OBDequeNode *tail;
  uint64_t length;
};


/* OBDeque Private Methods */

/* default constructor allocates and initializes an instance of OBDeque.
 * ALL OTHER CONSTRUCTOR METHODS SHOULD CALL THIS DEFAULT CONSTRUCTOR, which
 * should set up a bare bones instance of the class that others will initialize.
 * Ensures base obj is properly initialized.
 * Add additional arguments as needed */
OBDeque * createDefaultDeque(void);

/* recursive merge sort operation. runs on static variables to reduce the memory
 * management overhead */
OBDeque recursiveSort(OBDeque deque, const compare_fptr compare,
                      const int8_t order);

/* deallocator, frees instance of class back to memory. Releases any contained
 * objs once. Should not be called manually, instance will be destroyed when 
 * reference count reaches 0 */
void deallocDeque(obj *to_dealloc);


#endif
