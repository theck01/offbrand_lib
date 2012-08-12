
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
 * OBBigUInt a binary shifted to the left by 32_bit_shifts number of 
 * uint32_t digits, and bit_shift number of individual bits. 32_bit_shifts
 * can be any number, but the actual bit shift will be bit_shift%32. Add
 * additional values to 32_bit_shifts for single bit shifts greater than
 * 32*/
OBigUInt * shiftBigUIntLeft(const OBBigUInt *a, uint64_t 32_bit_shifts,
                            uint8_t bit_shift);

/* allocates and initializes an instance of OBBigUInt that is the result of
 * OBBigUInt a binary shift to the right by 32_bit_shifts number of 
 * uint32_t digits, and bit_shift number of individual bits. 32_bit_shifts
 * can be any number, but the actual bit shift will be bit_shift%32. Add
 * additional values to 32_bit_shifts for single bit shifts greater than
 * 32*/
OBBigUInt * shiftBigUIntRight(const OBBigUInt *a, uint64_t 32_bit_shifts,
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
int8_t compareBigUInt(const obj *a, const obj *b);

#endif

