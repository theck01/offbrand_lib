
/*
 * Private header for OBVector
 */

#ifndef OBVECTOR_PRIVATE_H
#define OBVECTOR_PRIVATE_H

#include "../OBVector.h"

/* DATA */
struct OBVector_struct{
  obj base;
  obj **array;
  uint32_t num_objs;
  uint32_t capacity;
};


/* PRIVATE METHODS */

/* function performs all initialization on new instances OBVector, MUST
 * BE CALLED IMMEDIATELY AFTER INSTANTIATION. This function may be called by a
 * default constructor that all other constructors build off of */
void initVectorBase(OBVector *to_init);

/* resizes vector if num_objs == capacity, does nothing if not. Doubles vector 
 * capacity */
void resizeVector(OBVector *v);

/* recusive implementation of the merge sort algorithm to be applied to the
 * OBVector */
obj ** recursiveSortContents(obj **to_sort, uint32_t size,
                             const compare_fptr compare, int8_t order);

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void deallocVector(obj *to_dealloc);

#endif
