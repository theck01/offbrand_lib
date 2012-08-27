
#ifndef MISC_FUNCTIONS_H
#define MISC_FUNCTIONS_H

#include "../../../include/offbrand.h"
#include "../../../include/OBVector.h"

/* function returns the number of 1 bits in the integer */
uint8_t numOneBits(uint32_t num);

/* function takes an OBVector containing all the Term objects required to
 * represent the unminimized function (either minterms or maxterms), and returns
 * a vector of the largest possible NCubes that completely cover all of the
 * terms */
OBVector * findLargestPrimeImplicants(OBVector *terms);

#endif
