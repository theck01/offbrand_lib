
/*
 * Private header for OBTest
 */
#ifndef OBTEST_PRIVATE_H
#define OBTEST_PRIVATE_H

#include "../OBTest.h"

/* DATA */
struct OBTest_struct{
  obj base;
};

void deallocOBTest(obj *to_dealloc);

#endif
