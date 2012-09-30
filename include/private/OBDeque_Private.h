
/*
 * Private header for OBDeque
 */

#ifndef OBDEQUE_PRIVATE_H
#define OBDEQUE_PRIVATE_H

#include "../OBDeque.h"

/* DATA */

struct OBDeque_struct{
  obj base;
  /* Additional private data added here, MUST COME AFTER THE obj */
};


/* PRIVATE METHODS */

/* default constructor allocates and initializes an instance of OBDeque.
 * ALL OTHER CONSTRUCTOR METHODS SHOULD CALL THIS DEFAULT CONSTRUCTOR, which
 * should set up a bare bones instance of the class that others will initialize.
 * Ensures base obj is properly initialized.
 * Add additional arguments as needed */
OBDeque * createDefaultOBDeque(void);

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void deallocOBDeque(obj *to_dealloc);

/*PRIVATE METHOD DECLARATIONS HERE*/

#endif
