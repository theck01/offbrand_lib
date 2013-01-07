
/*
 * Private header for NCube
 */

#ifndef NCUBE_PRIVATE_H
#define NCUBE_PRIVATE_H

#include "../NCube.h"

/* DATA */

struct NCube_struct{
  obj base;
  uint32_t *terms;         /* array of all minterms or maxterms  contained in
                              the NCube */
  uint32_t dont_cares;     /* each bit of the uint32_t is associated with a
                              single dont care bit */
  uint8_t order;
  uint8_t prime_implicant; /* boolean, indicates if term is a prime implicant */
  uint8_t all_dont_cares;  /* boolean, indicates if the term is composed of only
                              dont cares */
  uint8_t essential;
};


/* PRIVATE METHODS */

/* default constructor, other constructors will call this one */
NCube * createNCubeWithOrder(uint8_t order);

/* compare two instances of NCube, return 0 if they are equal, 1 if they are not
 * NCubes cannot be considered greater than or less than each other */
int8_t compareNCubes(const obj *a, const obj *b);

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void deallocNCube(obj *to_dealloc);

#endif
