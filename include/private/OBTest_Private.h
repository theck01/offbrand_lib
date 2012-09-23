
/*
 * Private header for OBTest
 */
#ifndef OBTEST_PRIVATE_H
#define OBTEST_PRIVATE_H

#include "../OBTest.h"

/* DATA */
struct OBTest_struct{
  obj base;
  uint32_t id;
};

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void deallocTest(obj *to_dealloc);

#endif
