
/*
 * RTable:
 * Class used to find essential prime implicants to a boolean function, given a
 * pair of OBVectors, one with all prime implicants (NCube objects) and one with
 * all terms (Term objects).
 *
 * RTable is a shortened version of "Reduction Table", and is meant to model the
 * table used in the Quine-McClusky method to determine what prime implicants
 * are essential prime implicants
 */

#ifndef RTABLE_H
#define RTABLE_H

#include "../../../include/offbrand.h"
#include "../../../include/OBVector.h"

/* Class type declaration */
typedef struct RTable_struct RTable;


/* PUBLIC METHODS */

/* constructor allocates and initializes an instance of RTable, fails if a the
 * OBVector prime_implicants contains anything but NCubes, and terms contains
 * anything but Terms */
RTable * createRTable(OBVector *prime_implicants, OBVector *terms);

/* Function initializes the reduction process and returns an OBVector containing
 * all essential prime implicants (NCube objects) */
OBVector * findEssentialPIs(RTable *table);

#endif

