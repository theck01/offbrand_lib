
/*
 * Term:
 * [Class description]
 */

#ifndef TERM_H
#define TERM_H

#include "offbrand.h"

/* Class type declaration */
typedef struct Term_struct Term;


/* PUBLIC METHODS */

/* constructor allocates and initializes an instance of Term */
/* expand with additional arguments as needed */
Term * createTerm(void);

/* compare two instances of Term, return 1 if a > b, 0 if a == b, 
 * and -1 if a < b */
int8_t compareTerms(const obj *a, const obj *b);

/* DECLARE ADDITIONAL PUBLIC METHODS HERE */

#endif

