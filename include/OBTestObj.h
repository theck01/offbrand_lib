
/*
 * OBTestObj:
 * Object used to test reference counting and thread safety for all other
 * OffBrand classes.
 */

#ifndef OBTESTOBJ_H
#define OBTESTOBJ_H

#include "OffBrand.h"

/* Class type declaration */
typedef struct OBTestObj_struct OBTestObj;


/* PUBLIC METHODS */

/* constructor allocates and initializes and instance of OBTestObj */
/* expand with additional arguments as needed */
OBTestObj * createOBTestObj(void);

/* lock to_lock to the currently executing thread */
void lockTestObj(OBTestObj *to_lock);

/* unlock to_unlock, so other threads may use the object */
void unlockTestObj(OBTestObj *to_unlock);

#endif

