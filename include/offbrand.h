
/*
 * OFFBRAND STANDARD LIBRARY
 *
 * Includes all data structures required reference counting, as well as common
 * types required by most/all Offbrand classes. Defines the interface for the 
 * reference counting API and other common functions.
 */

#ifndef OFFBRAND_LIB
#define OFFBRAND_LIB

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "obj.h" /*generic pointer for all OffBrand data structures*/

/* OFFBRAND CONSTANTS */

/* comparison constants */
#define OB_EQUAL_TO 0
#define OB_GREATER_THAN 1
#define OB_LESS_THAN -1

/* sorting order */
#define OB_LEAST_TO_GREATEST -1
#define OB_GREATEST_TO_LEAST 1

/* OFFBRAND PRIMITIVE TYPES */

/* function pointer to a deallocator for any OffBrand compatible class */
typedef void (*dealloc_fptr)(obj *);

/* Pointer to function that compares two instances of the same Offbrand
 * compatible classes. Function should return -1 if first object is less than
 * the second, 0 if they are equal, and 1 if the first is greater than the
 * second */
typedef int8_t (*compare_fptr)(const obj *, const obj*);


/* OFFBRAND STANDARD LIB */

/* initializes an instance of any class with specified dealloc function,
 * default reference count of 1, and classname */
void initBase(obj *instance, dealloc_fptr dealloc, const char *classname);

/* decrements reference count on OBObj, frees memory if reference count is
 * reduced to 0. Returns a pointer to the object if it still exists, NULL if
 * it is no longer referenced and was destroyed */
obj * release(obj *instance);

/* increments reference count on OBObj by one */
void retain(obj *instance);

/* performs string comparision on classname of object a and the provided null
 * terminated character string, returns 0 if they are equal, 1 if the classnames
 * do not match */
uint8_t objIsOfClass(const obj *a, const char *classname);

/* performs string comparision on classnames of objects a and b, returns 0 if
 * they have the same classname (and are of the same class), or 1 if they have
 * differing classnames */
uint8_t sameClass(const obj *a, const obj *b);

/* default comparision, checks to see that the two pointer values are equal.
 * Used in sorting and finding algorithms for basic containers. Returns 0 if
 * equal, 1 if false */
int8_t objCompare(const obj *a, const obj *b);

#endif

