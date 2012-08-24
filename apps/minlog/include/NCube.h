
/*
 * NCube:
 * Offbrand compatible class that models an NCube, a construct used to group
 * minterms or maxterms together to simplify boolean equations
 */

#ifndef NCUBE_H
#define NCUBE_H

#include "offbrand.h"

/* Class type declaration */
typedef struct NCube_struct NCube;


/* PUBLIC METHODS */

/* constructor allocates and initializes an instance of NCube with the term
 * (minterm or maxterm) specified */
NCube * createNCube(uint32_t term);

/* constructor allocates and initializes an instance of NCube with by merging
 * the two NCubes into a single higher order cube. If cubes cannot be merged
 * return NULL */
NCube * mergeNCubes(const NCube *a, const NCube *b); 

/* compare two instances of NCube, return 0 if they are equal, 1 if they are not
 * NCubes cannot be considered greater than or less than each other */
int8_t compareNCubes(const obj *a, const obj *b);

/* boolean function, returns 1 if NCube covers term specified, 0 if not */
uint8_t nCubeCoversTerm(const NCube *a, uint32_t term);

/* boolean function, returns 1 if NCube is a prime implicant, 0 if not */
uint8_t isNCubePrimeImplicant(const NCube *a);

#endif

