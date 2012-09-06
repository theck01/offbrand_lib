
/*
 * Private header for RTable
 */

#ifndef RTABLE_PRIVATE_H
#define RTABLE_PRIVATE_H

#include "../RTable.h"

/* DATA */

struct RTable_struct{
  obj base;
  OBVector *pis;         /* shorthand for Prime Implicants */
  OBVector *terms;
  OBVector *essential_pis;
  uint8_t **cover_flags; /* flags that indicate whether a term is covered by a
                            prime implicant, where the first dimension
                            corresponds to the prime implicant, and the second
                            corresponds to the term. */

};


/* PRIVATE METHODS */

/* function performs all initialization on new instances RTable, MUST
 * BE CALLED IMMEDIATELY AFTER INSTANTIATION. This function may be called by a
 * default constructor that all other constructors build off of */
void initRTableBase(RTable *to_init);

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void deallocRTable(obj *to_dealloc);

/* used in constructor to determine which Terms covered by which NCubes and
 * which NCubes can be determined to be essential without extra effort */
void initTermCoverArray(OBVector *cubes, uint32_t term, uint8_t *array);

/* brute force search through all combinations of terms to find the minimal
 * term coverage. Returns a vector of NCubes with optimal coverage with a
 * single reference */
OBVector * petricksReduce(OBVector *unresolved_cubes,
                          OBVector *unresolved_terms);


#endif
