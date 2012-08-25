
/*
 * Term:
 * Wrapper around a uint32_t, to create an Offbrand compatible class. Represents
 * a single minterm or maxterm
 */

#ifndef TERM_H
#define TERM_H

#include "../../../include/offbrand.h"

/* Class type declaration */
typedef struct Term_struct Term;


/* PUBLIC METHODS */

/* constructor allocates and initializes an instance of Term */
/* expand with additional arguments as needed */
Term * createTerm(uint32_t term);

/* compare two instances of Term, return 1 if a > b, 0 if a == b, 
 * and -1 if a < b */
int8_t compareTerms(const obj *a, const obj *b);

/* get term value as an integer */
int32_t getTermValue(Term *a);

#endif

