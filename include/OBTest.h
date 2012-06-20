
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
OBTest * createOBTest(void);

#endif

