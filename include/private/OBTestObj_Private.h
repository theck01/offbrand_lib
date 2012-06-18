
/*
 * Private header for OBTestObj
 */

#ifndef OBTESTOBJ_PRIVATE_H
#define OBTESTOBJ_PRIVATE_H

#include "OBTestObj.h"

/* DATA */

struct OBTestObj_struct{
  OBObj base;
  pthread_mutex_t lock;
};


/* PRIVATE METHODS */

/* destructor, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void destroyOBTestObj(OBTestObj *to_destroy);

#endif
