
#ifndef OBOBJ_H
#define OBOBJ_H

#include "OffBrand.h" /*header required by all OffBrand classes*/

typedef struct OBObj_struct OBObj;

typedef uint32_t ref_count_t; /* reference count */
typedef void (*destroy_fptr)(OBObj *); /* function pointer to a deallocator for
                                          any OffBrand compatible class */

/* initializes an instance of OBObj with specified pointer and reference count
 * of 1 */
void initObj(OBObj *o, destroy_fptr dealloc);

/* decrements reference count on OBObj, frees memory if reference count is
 * reduced to 0. Returns a pointer to the object if it still exists, NULL if
 * it is no longer referenced and was destroyed */
OBObj * releaseObj(OBObj *o);

/* increments reference count on OBObj by one */
void retainObj(OBObj *o);

#endif
