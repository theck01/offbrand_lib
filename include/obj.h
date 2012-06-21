
#ifndef OBJ_H
#define OBJ_H

#include <stdlib.h>
#include <stdint.h>

typedef struct obj_struct * obj;

typedef uint32_t ref_count_t; /* reference count */
typedef void (*dealloc_fptr)(obj *); /* function pointer to a deallocator for
                                          any OffBrand compatible class */

/* initializes an instance of any class with specified dealloc function and
 * default reference count of 1. Returns 0 on success, 1 on failure */
uint8_t initBase(obj *instance, dealloc_fptr dealloc);

/* decrements reference count on OBObj, frees memory if reference count is
 * reduced to 0. Returns a pointer to the object if it still exists, NULL if
 * it is no longer referenced and was destroyed */
obj * release(obj *instance);

/* increments reference count on OBObj by one */
void retain(obj *instance);

/* compares dealloc functions for the arguments to determine that they are from
 * the same class. Depends on a class having a single unique deallocation,
 * which is required for all Offbrand compatible classes. returns 1 if instances
 * of same class, 0 if not */
uint8_t sameClass(const obj *a, const obj *b);

#endif
