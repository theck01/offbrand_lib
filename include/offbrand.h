
/*
 * Basic header for Offbrand Library. For pthread support include
 * offbrand_threaded.h
 */

#ifndef OFFBRAND_LIB
#define OFFBRAND_LIB

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "obj.h" /*generic pointer for all OffBrand data structures*/

/* Global Constants */

/* sorting order */
#define OB_LEAST_TO_GREATEST -1
#define OB_GREATEST_TO_LEAST 1

/* error codes */
#define OB_COMPARE_ERR 2 /* comparision failed */


/* Global Types */

/* Pointer to function that compares two instances of the same Offbrand
 * compatible classes. Function should return -1 if first object is less than
 * the second, 0 if they are equal, and 1 if the first is greater than the
 * second */
typedef int8_t (*compare_fptr)(const obj *, const obj*);


/* Global Lib */

/* default comparision, checks to see that the two pointer values are equal.
 * Used in sorting and finding algorithms for basic containers. Returns 0 if
 * equal, 1 if false */
int8_t defaultCompare(obj *a, obj *b);

#endif

