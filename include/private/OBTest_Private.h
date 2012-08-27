
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

/* function performs all initialization on new instances OBTest, MUST
 * BE CALLED IMMEDIATELY AFTER INSTANTIATION. This function may be called by a
 * default constructor that all other constructors build off of */
uint8_t initTestBase(OBTest *to_init);

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void deallocTest(obj *to_dealloc);

#endif
