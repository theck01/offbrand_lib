
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

/* Global Types */

/* Pointer to function that compares two instances of the same Offbrand
 * compatible classes. Function should return -1 if first object is less than
 * the second, 0 if they are equal, and 1 if the first is greater than the
 * second */
typedef int8_t (*compare_fptr)(const obj *, const obj*);

#endif

