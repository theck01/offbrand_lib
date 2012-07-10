
#ifndef OBJ_PRIVATE_H
#define OBJ_PRIVATE_H

#ifdef OFFBRAND_THREADED_H
#include "../OBLock.h"
#include "OBLock_private.h"
#endif


struct obj_struct{
  ref_count_t references;
  dealloc_fptr dealloc;
#ifdef OFFBRAND_THREADED_H
  OBLock lock;
#endif
};

#endif
