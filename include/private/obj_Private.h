
#ifndef OBJ_PRIVATE_H
#define OBJ_PRIVATE_H

#include "../offbrand.h"
#ifdef OB_THREADED
#include "../OBLock.h"
#include "OBLock_Private.h"
#endif

typedef uint32_t ref_count_t; /* reference count */

struct obj_struct{
  ref_count_t references;
  dealloc_fptr dealloc;
  hash_fptr hash;
  const char *classname;
#ifdef OB_THREADED
  OBLock lock;
#endif
};

#endif
