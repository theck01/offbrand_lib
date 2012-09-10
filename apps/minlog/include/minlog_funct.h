
#ifndef MISC_FUNCTIONS_H
#define MISC_FUNCTIONS_H

#include "../../../include/offbrand.h"
#include "../../../include/OBVector.h"

#define MINTERMS 0
#define MAXTERMS 1

/* function returns the number of 1 bits in the integer */
uint8_t numOneBits(uint32_t num);

/* function takes an OBVector containing all the Term objects required to
 * represent the unminimized function (either minterms or maxterms), and returns
 * a vector of the largest possible NCubes that completely cover all of the
 * terms */
OBVector * findLargestPrimeImplicants(const OBVector *terms,
                                      const OBVector *dont_cares);

/* function takes a string representing the equation (read from user) parses it
 * for terms and dont_cares, and creates new Term object for each, adding them
 * to the proper provided terms and dont_cares array. Function returns
 * MINTERMS if equation is represented by minterms, MAXTERMS if the equation is
 * represented by MAXTERMS. If verbose flag is non-zero print out detailed
 * information about terms parsed from string */
uint8_t parseEqnString(const char *eqnstr, OBVector *terms, 
                       OBVector *dont_cares, uint8_t verbose);

#endif
