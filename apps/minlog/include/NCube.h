
/*
 * NCube:
 * Offbrand compatible class that models an NCube, a construct used to group
 * minterms or maxterms together to simplify boolean equations
 */

#ifndef NCUBE_H
#define NCUBE_H

#include "../../../include/offbrand.h"

/* Class type declaration */
typedef struct NCube_struct NCube;


/* PUBLIC METHODS */

/* constructor allocates and initializes an instance of NCube with the term
 * (minterm or maxterm) specified, which is by default non essential but a
 * prime implicant */
NCube * createNCube(uint32_t term, uint8_t is_dont_care);

/* constructor allocates and initializes an instance of NCube with by merging
 * the two NCubes into a single higher order cube. If cubes cannot be merged
 * return NULL */
NCube * mergeNCubes(NCube *a, NCube *b); 

/* boolean function, returns 1 if NCube covers term specified, 0 if not */
uint8_t nCubeCoversTerm(const NCube *a, uint32_t term);

/* boolean function, returns 1 if NCube is a prime implicant, 0 if not */
uint8_t isNCubePrimeImplicant(const NCube *a);

/* function sets a flag that marks the NCube as essential */
void markNCubeAsEssential(NCube *a);

/* boolean function, returns 1 if NCube is marked as essential, 0 if not */
uint8_t isNCubeEssential(const NCube *a);

/* function returns the number of variables whos value is a don't care 
 * in NCube */
uint8_t orderOfNCube(const NCube *a);

/* function returns string representing the NCube as a term, either in sum of
 * products (SOP) or product of sums (POS) format. More significant variables
 * (or bits) are associated with letters earlier in the alphabet (A is the most
 * significant bit). sop is a boolean indicating whether sum of products is
 * desired (non zero value) or product of sums (0 value), and num_var is the
 * number of variables in the function that the NCube is a part of */
char * nCubeStr(const NCube *a, uint8_t is_sop, uint8_t num_var);


#endif

