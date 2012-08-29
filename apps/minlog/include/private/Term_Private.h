
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

/* function performs all initialization on new instances Term, MUST
 * BE CALLED IMMEDIATELY AFTER INSTANTIATION. This function may be called by a
 * default constructor that all other constructors build off of */
void initTermBase(Term *to_init);

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void deallocTerm(obj *to_dealloc);


#endif
