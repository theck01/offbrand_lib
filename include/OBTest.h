
/*
 * OBTest:
 * Object used to verify proper reference counting in more complex OffBrand
 * container objects. A placeholder object and nothing more.
 */

#ifndef OBTEST_H
#define OBTEST_H

#include "offbrand.h"

typedef struct OBTest_struct OBTest;

/* constructor allocates and initializes and instance of OBTest */
OBTest * createTest(uint32_t id);

/* gets the assigned id from OBTest *a */
uint32_t getTestID(OBTest *a);

/* compares the assigned ids of OBTest a and b. Returns -1 for a < b, 0 for 
 * a == b, and 1 for a > b */
int8_t compareTests(const obj *a, const obj *b);

#endif

