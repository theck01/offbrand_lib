
/*
 * Private header for OBDeque
 */

#ifndef OBDEQUE_PRIVATE_H
#define OBDEQUE_PRIVATE_H

#include "../OBDeque.h"

#define OBDEQUENODE_NEXT 0
#define OBDEQUENODE_PREV 1

/* OBDequeNode/OBDequeIterator Type */

/* internal data type, represents a single node in the doubly linked list.
 * Called an OBDequeIterator as an external facing type, and an OBDequeNode
 * for internal class use */
typedef struct OBDequeIterator_struct{
  void *null; /* value is always NULL, so that if an OBDequeNode is released
                 the release code will fail an assertion */
  obj *stored;
  struct OBDequeNode_struct *next;
  struct OBDequeNode_struct *prev;
} OBDequeNode;

/* OBDequeNode Private Methods */

/* creates a new, uninitialized version of OBDequeNode. The null member will be
 * set to null */
OBDequeNode * createDequeNode(void);

/* function causes node to release its internal memory and take on the memory of
 * either its next or previous node, maintaining Deque continuity. Used
 * internally in remove* functions */
void absorbDequeNode(OBDequeNode *node, uint8_t is_next);


/* OBDeque Type */

struct OBDeque_struct{
  obj base;
  OBDequeNode *head;
  OBDequeNode *tail;
};


/* OBDeque Private Methods */

/* default constructor allocates and initializes an instance of OBDeque.
 * ALL OTHER CONSTRUCTOR METHODS SHOULD CALL THIS DEFAULT CONSTRUCTOR, which
 * should set up a bare bones instance of the class that others will initialize.
 * Ensures base obj is properly initialized.
 * Add additional arguments as needed */
OBDeque * createDefaultOBDeque(void);

/* deallocator, frees instance of class back to memory. Releases any contained
 * objs once. Should not be called manually, instance will be destroyed when 
 * reference count reaches 0 */
void deallocOBDeque(obj *to_dealloc);


#endif
