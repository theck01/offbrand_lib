
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

void deallocOBTest(obj *to_dealloc);

#endif
