
/*
 * Private header for OBBigUInt
 */

#ifndef OBBIGUINT_PRIVATE_H
#define OBBIGUINT_PRIVATE_H

#include "../OBBigUInt.h"

/* DATA */

struct OBBigUInt_struct{
  obj base;
  uint32_t *uint_array;
  uint64_t num_ints;
  uint64_t capacity;
};


/* PRIVATE METHODS */

/* constructor allocates an new OBBigUInt with specified capacity. Used for
 * internal allocation in most public arithmetic and bitwise operations */
OBBigUInt * createBigUIntWithCap(uint64_t capacity);

/* determines the number of significant ints in OBBigUInt that will matter in 
 * arithmetic operations */
uint64_t sigIntsInBigUInt(OBBigUInt *a);

/* generates the twos compliment version of the OBBigUInt, with total_len digits
 * if sign needs to be extended*/
OBBigUInt * twosCompBigUInt(OBBigUInt * a, uint64_t total_len);

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void deallocBigUInt(obj *to_dealloc);

#endif
