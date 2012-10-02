
/*
 * Private header for OBDeque
 */

#ifndef OBDEQUE_PRIVATE_H
#define OBDEQUE_PRIVATE_H

#include "../OBDeque.h"

/* DATA */

/* internal data type, represents a single node in the doubly linked list.
 * Called an OBDequeIterator as an external facing type, and an OBDequeNode
 * for internal class use */
typedef struct OBDequeIterator_struct{
  obj *stored;
  struct OBDequeNode_struct *next;
  struct OBDequeNode_struct *prev;
} OBDequeNode;

struct OBDeque_struct{
  obj base;
  OBDequeNode *head;
  OBDequeNode *tail;
};


/* PRIVATE METHODS */

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

/*PRIVATE METHOD DECLARATIONS HERE*/

#endif
