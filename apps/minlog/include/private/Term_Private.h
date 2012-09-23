
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

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void deallocTerm(obj *to_dealloc);


#endif
