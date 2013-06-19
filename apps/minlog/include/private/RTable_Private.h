
/*
 * Private header for RTable
 */

#ifndef RTABLE_PRIVATE_H
#define RTABLE_PRIVATE_H

#include "../RTable.h"
#include "../NCube.h"

/* DATA */

struct RTable_struct{
  obj base;
  obvector *pis;         /* shorthand for Prime Implicants */
  obvector *terms;
  obvector *essential_pis;
  uint8_t **cover_flags; /* flags that indicate whether a term is covered by a
                            prime implicant, where the first dimension
                            corresponds to the prime implicant, and the second
                            corresponds to the term. */

};


/* PRIVATE METHODS */

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void deallocRTable(obj *to_dealloc);

/* used in constructor to determine which Terms covered by which NCubes and
 * which NCubes can be determined to be essential without extra effort */
void initTermCoverArray(const obvector *cubes, uint32_t term, uint8_t *array);

/* brute force search through all combinations of terms to find the minimal
 * term coverage. Returns a vector of NCubes with optimal coverage with a
 * single reference */
obvector * petricksReduce(const obvector *unresolved_cubes,
                          const obvector *unresolved_terms, uint8_t num_var);

/* function initializes two pieces of data, a 2 diminsional "matrix" of NCubes,
 * where the first dimension corresponds to term and the second corresponds to
 * all cubes which cover that term, and the counts array (supplied as an
 * argument) containing the number of cubes that cover each term. Counts array
 * must be preallocated with the same size as the number of unresolved terms */
NCube *** initPetricksData(const obvector *unresolved_cubes,
                           const obvector *unresolved_terms, uint32_t *counts);

/* function increments "counter" represented by cur_idxs array, with maximum
 * counts at each array position measured by max_idxs. Both cur_idxs and
 * max_idxs have array_size terms. Returns 0 if the next cover combo is valid,
 * else returns 1 (meaning all combos have been traversed and counter reset) */
uint8_t nextCoverCombo(const uint32_t *max_idxs, uint32_t *cur_idxs,
                       uint32_t array_size);


#endif
