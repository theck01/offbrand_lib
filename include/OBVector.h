
/*
 * OBVector:
 * Automatically resizing array of obj's (any Offbrand builtin or compatible
 * class
 */

#ifndef OBVECTOR_H
#define OBVECTOR_H

#include "offbrand.h"
#include <stdio.h>

/* Class type declaration */
typedef struct OBVector_struct OBVector;


/* PUBLIC METHODS */

/* constructor allocates and initializes an instance of OBVector with the
 * initial capacity specified*/
OBVector * createVector(uint32_t initial_capacity);

/* creates a new OBVector that contains the same pointer so obj's. The OBVector
 * data is copied, but the objects contained within are not */
OBVector * copyVector(const OBVector *to_copy); 

/* returns the number of elements contained within the given OBVector */
uint32_t sizeOfVector(const OBVector *v);

/* resizes the vector to the current number of elements, so no extra array space
 * is wasted. Should be used only after a vector will remain constant for the
 * duration of its lifespan, if memory is tight. */
void fitVectorToContents(OBVector *v); 

/* adds obj to end of vector */
void addToVector(OBVector *v, obj *to_add);

/* adds obj to the vector at specified index */ 
void insertAtVectorIndex(OBVector *v, obj *to_add, uint32_t index);

/* replace obj at index. Does nothing if index is not less than number of items
 * contained within vector. Returns 0 if object is replaced, 1 if out of range*/
void replaceInVector(OBVector *v, obj *new_obj, const uint32_t index);

/* returns the object stored in the OBVector at index. Returns NULL if index is
 * out of bounds */
obj * objAtVectorIndex(const OBVector *v, const uint32_t index);

/* checks to see if object, or equivalent as determined by compare function
 * pointer for specific class, can be found within vector. If compare function
 * argument it NULL then pointer comparisons are used. Returns 1 if found, 0 if 
 * not */
uint8_t findObjInVector(const OBVector *v, const obj *to_find,
                        compare_fptr compare);

/* sorts vector using the compare function passed in as an argument. ASSUMES
 * THAT ALL OBJ INSIDE ARE OF THE SAME CLASS, partial sort may occur if classes
 * found to be different at runtime. Order (lowest to highest or highest to 
 * lowest) specified by sorting macros defined in sort_obj.h. Uses the merge 
 * sort algorithm. Returns 0 on success, 1 on error */
void sortVector(OBVector *v, const compare_fptr compare, const int8_t order);

/* remove obj at end of vector, releasing the vectors reference on the obj */
void removeFromVectorEnd(OBVector *v);

/* remove obj at the specified index in the vector, releasing the vectors
 * reference on the obj. If the index is not within vector range, function does
 * nothing */
void removeFromVectorIndex(OBVector *v, uint32_t index);

/* releases all objs contained within OBVector, and sets it to empty */
void clearVector(OBVector *v);

#endif

