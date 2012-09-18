
#ifndef MISC_FUNCTIONS_H
#define MISC_FUNCTIONS_H

#include "../../../include/offbrand.h"
#include "../../../include/OBVector.h"

#define MINLOG_MINTERMS 0
#define MINLOG_MAXTERMS 1

/* function returns the number of 1 bits in the integer */
uint8_t numOneBits(uint32_t num);

/* function returns the value of the bit in the uint32_t corresponding to
 * bit_num, bit_num of 0 corresponding to the least significant bit */
uint8_t testBit(uint32_t num, uint8_t bit_num);

/* function takes a string representing the equation (read from user) parses it
 * for terms and dont_cares, and creates new Term object for each, adding them
 * to the proper provided terms and dont_cares array. Function returns
 * MINTERMS if equation is represented by minterms, MAXTERMS if the equation is
 * represented by MAXTERMS. If verbose flag is non-zero print out detailed
 * information about terms parsed from string */
uint8_t parseEqnString(const char *eqnstr, OBVector *terms, 
                       OBVector *dont_cares, uint8_t verbose);

/* function takes an OBVector containing all the Term objects required to
 * represent the unminimized function (either minterms or maxterms), and returns
 * a vector of the largest possible NCubes that completely cover all of the
 * terms */
OBVector * findLargestPrimeImplicants(const OBVector *terms,
                                      const OBVector *dont_cares);

/* function prints the equation represented by the vector of essential prime
 * implicant NCubes, essential_pis, in Sum of Products form if is_sop != 0, or
 * Product of Sums form if is_sop = 0, with the give number of variables */
void printEqnVector(const OBVector *essential_pis, uint8_t is_sop,
                    uint32_t num_var);


#endif
