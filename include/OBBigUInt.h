
/*
 * OBBigUInt:
 * Unsigned integer with unbound size.
 */

#ifndef OBBIGUINT_H
#define OBBIGUINT_H

#include "offbrand.h"

/* Class type declaration */
typedef struct OBBigUInt_struct OBBigUInt;


/* PUBLIC METHODS */

/* constructor allocates and initializes and instance of OBBigUInt */
/* expand with additional arguments as needed */
OBBigUInt * createBigUInt(void);

/* compare two instances of the class, return 1 if a > b, 0 if a == b, and -1 if
 * a < b */
int8_t compareBigUInt(const obj *a, const obj *b);

/* DECLARE ADDITIONAL PUBLIC METHODS HERE */

#endif

