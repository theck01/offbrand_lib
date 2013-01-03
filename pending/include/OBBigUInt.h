
/*
 * OBBigUInt:
 * Big-endian unsigned integer with maximum size of (2^32)^(2^64). This max
 * number would take 64 exbibytes of memory, and over 10^20 digits to represent
 * in decimal format.
 */

#ifndef OBBIGUINT_H
#define OBBIGUINT_H

#include "offbrand.h"
#include <string.h>

/* Class type declaration */
typedef struct OBBigUInt_struct OBBigUInt;


/* BASIC CONSTRUCTORS */

/* constructor allocates and initializes an instance of OBBigUInt with the
 * integer value specified */
OBBigUInt * createBigUIntFromNum(uint32_t number);

/* constructor allocates and initializes an instance of OBBigUInt from the
 * array of integers, which are in little endian order */
OBBigUInt * createBigUIntFromIntArray(const uint32_t *int_str,
                                      uint64_t num_uints);

/* constructor allocates and initializes an instance a copy of the OBBigUInt
 * argument */
OBBigUInt * copyBigUInt(const OBBigUInt *to_copy);


/* ARITHMETIC OPERATIONS */

/* allocates and initializes an instance of OBBigUInt that is the
 * sum of the two OBBigUInt arguments */
OBBigUInt * addBigUInts(const OBBigUInt *a, const OBBigUInt *b);

/* allocates and initializes an instance of OBBigUInt that is the
 * difference of the OBBigUInt minuend and subtrahend. If the result is negative
 * then return an OBBigUInt representing 0 */
OBBigUInt * subtractBigUInts(const OBBigUInt *minuend,
                             const OBBigUInt *subtrahend);

/* allocates and initializes an instance of OBBigUInt that is the
 * product of the OBBigUInt arguments */
OBBigUInt * multiplyBigUInts(const OBBigUInt *a, const OBBigUInt *b);

/* allocates and initializes an instance of OBBigUInt that is the
 * quotient of the OBBigUInt dividend divided by divisor. Performs integer
 * division, all results are rounded down to the next lowest integer */
OBBigUInt * divideBigUInts(const OBBigUInt *dividend, const OBBigUInt *divisor);

/* allocates and initializes an instance of OBBigUInt that is the
 * remainter of the OBBigUInt dividend divided by divisor. */
OBBigUInt * modBigUInts(const OBBigUInt *dividend, const OBBigUInt *divisor);


/* BITWISE OPERATIONS */

/* allocates and initializes an instance of OBBigUInt that is the result of
 * OBBigUInt a binary shifted to the left by uint32_shifts number of 
 * uint32_t digits, and bit_shift number of individual bits. uint32_shifts
 * can be any number, but the actual bit shift will be bit_shift%32. Add
 * additional values to uint32_shifts for single bit shifts greater than
 * 32*/
OBBigUInt * shiftBigUIntLeft(const OBBigUInt *a, uint64_t uint32_shifts,
                            uint8_t bit_shift);

/* allocates and initializes an instance of OBBigUInt that is the result of
 * OBBigUInt a binary shift to the right by uint32_shifts number of 
 * uint32_t digits, and bit_shift number of individual bits. uint32_shifts
 * can be any number, but the actual bit shift will be bit_shift%32. Add
 * additional values to uint32_shifts for single bit shifts greater than
 * 32*/
OBBigUInt * shiftBigUIntRight(const OBBigUInt *a, uint64_t uint32_shifts,
                              uint8_t bit_shift);

/* allocates and initializes an instance of OBBigUInt that is the result of
 * bitwise ANDing the two OBBigUInt arguments */
OBBigUInt * andBigUInts(const OBBigUInt *a, const OBBigUInt *b);

/* allocates and initializes an instance of OBBigUInt that is the result of
 * bitwise ORing the two OBBigUInt arguments */
OBBigUInt * orBigUInts(const OBBigUInt *a, const OBBigUInt *b);

/* allocates and initializes an instance of OBBigUInt that is the result of
 * bitwise XORing the two OBBigUInt arguments */
OBBigUInt * xorBigUInts(const OBBigUInt *a, const OBBigUInt *b);

/* allocates and initializes an instance of OBBigUInt that is the result of
 * bitwise NOT of a */
OBBigUInt * notBigUInts(const OBBigUInt *a);


/* MISC OPERATIONS */

/* compare two instances of the OBBigUInt, return 1 if a > b, 0 if a == b, and 
 * -1 if a < b */
int8_t compareBigUInts(const obj *a, const obj *b);

/* prints the hex format of a as a list of uin32_t's, with the most
 * significant word listed first */
void printBigUInt(const OBBigUInt *a);

#endif

