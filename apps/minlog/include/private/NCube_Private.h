
/*
 * Private header for NCube
 */

#ifndef NCUBE_PRIVATE_H
#define NCUBE_PRIVATE_H

#include "../NCube.h"

/* DATA */

struct NCube_struct{
  obj base;
  uint32_t *terms;         /* array of all minterms or maxterms  contained in */
  uint32_t order;          /* the NCube */
  uint8_t prime_implicant; /* boolean, indicates if term is a prime implicant */
};


/* PRIVATE METHODS */

/* default constructor, other constructors will call this one */
NCube * createNCubeWithOrder(uint8_t order);

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void deallocNCube(obj *to_dealloc);

#endif
