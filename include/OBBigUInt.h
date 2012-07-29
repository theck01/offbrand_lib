
/*
 * OBBigUInt:
 * Little-endian unsigned integer with maximum size of (2^32)^(2^64). This max
 * number would take 64 exbibytes of memory, take over 10^20 digits to represent
 * in decimal format.
 */

#ifndef OBBIGUINT_H
#define OBBIGUINT_H

#include "offbrand.h"
#include <string.h>

/* Class type declaration */
typedef struct OBBigUInt_struct OBBigUInt;


/* BASIC CONSTRUCTORS */

/* constructor allocates and initializes an instance of OBBigUInt */
/* with initial value of 0 */
OBBigUInt * createZeroBigUInt(void);

/* constructor allocates and initializes an instance of OBBigUInt with the
 * integer value specified */
OBBigUInt * createBigUIntFromNum(uint32_t number);

/* constructor allocates and initializes an instance of OBBigUInt from a number
 * represented by the null terminated character string */
OBBigUInt * createBigUIntFromStr(char *num_str);

/* constructor allocates and initializes an instance a copy of the OBBigUInt
 * argument */
OBBigUInt * copyBigUInt(OBBigUInt *to_copy);


/* ARITHMETIC OPERATIONS */

/* allocates and initializes an instance of OBBigUInt that is the
 * sum of the two OBBigUInt arguments */
OBBigUInt * addBigUInts(OBBigUInt *a, OBBigUInt *b);

/* allocates and initializes an instance of OBBigUInt that is the
 * difference of the OBBigUInt minuend and subtrahend. If the result is negative
 * then return an OBBigUInt representing 0 */
OBBigUInt * subtractBigUInts(OBBigUInt *minuend, OBBigUInt *subtrahend);

/* allocates and initializes an instance of OBBigUInt that is the
 * product of the OBBigUInt arguments */
OBBigUInt * multiplyBigUInts(OBBigUInt *a, OBBigUInt *b);

/* allocates and initializes an instance of OBBigUInt that is the
 * quotient of the OBBigUInt dividend divided by divisor. Performs integer
 * division, all results are rounded down to the next lowest integer */
OBBigUInt * divideBigUInts(OBBigUInt *dividend, OBBigUInt *divisor);

/* allocates and initializes an instance of OBBigUInt that is the
 * remainter of the OBBigUInt dividend divided by divisor. */
OBBigUInt * modBigUInts(OBBigUInt *dividend, OBBigUInt *divisor);


/* BITWISE OPERATIONS */

/* allocates and initializes an instance of OBBigUInt that is the result of
 * OBBigUInt a binary shifted to the left by shift_size binary digits */
OBigUInt * shiftBigUIntLeft(OBBigUInt *a, OBBigUInt *shift_size);

/* allocates and initializes an instance of OBBigUInt that is the result of
 * OBBigUInt a binary shifted to the right by shift_size binary digits */
OBBigUInt * shiftBigUIntLeft(OBBigUInt *a, OBBigUInt *shift_size);

/* allocates and initializes an instance of OBBigUInt that is the result of
 * bitwise ANDing the two OBBigUInt arguments */
OBBigUInt * andBigUInts(OBBigUInt *a, OBBigUInt *b);

/* allocates and initializes an instance of OBBigUInt that is the result of
 * bitwise ORing the two OBBigUInt arguments */
OBBigUInt * orBigUInts(OBBigUInt *a, OBBigUInt *b);

/* allocates and initializes an instance of OBBigUInt that is the result of
 * bitwise XORing the two OBBigUInt arguments */
OBBigUInt * xorBigUInts(OBBigUInt *a, OBBigUInt *b);

/* allocates and initializes an instance of OBBigUInt that is the result of
 * bitwise NOT of a */
OBBigUInt * notBigUInts(OBBigUInt *a);


/* MISC OPERATIONS */

/* compare two instances of the OBBigUInt, return 1 if a > b, 0 if a == b, and 
 * -1 if a < b */
int8_t compareBigUInt(const obj *a, const obj *b);

#endif

