
/*
 * Private header for Term
 */

#ifndef TERM_PRIVATE_H
#define TERM_PRIVATE_H

#include "../Term.h"

/* DATA */

struct Term_struct{
  obj base;
  uint32_t term;
};


/* PRIVATE METHODS */

/* compare two instances of Term, return 1 if a > b, 0 if a == b, 
 * and -1 if a < b */
int8_t compareTerms(const obj *a, const obj *b);

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void deallocTerm(obj *to_dealloc);


#endif
