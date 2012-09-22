
#include "../../include/RTable.h"
#include "../../include/private/RTable_Private.h"
#include "../../include/Term.h"
#include "../../include/minlog_funct.h"

/* PUBLIC METHODS */

/* add arguments to complete initialization as needed, modify RTable.h
 * as well if modifications are made */
RTable * createRTable(const OBVector *prime_implicants, const OBVector *terms){

  uint64_t i, num_pis, num_terms;
  uint32_t term_num;
  RTable *new_instance = malloc(sizeof(RTable));
  assert(new_instance != NULL);

  /* initialize reference counting base of class */
  initRTableBase(new_instance);

  new_instance->pis = copyVector(prime_implicants);
  new_instance->terms = copyVector(terms);

  num_pis = sizeOfVector(new_instance->pis);
  num_terms = sizeOfVector(new_instance->terms);

  /* verify vectors are of proper format and allocate/initialize 2D array */
  for(i=0; i<num_pis; i++){
    assert(objIsOfClass(objAtVectorIndex(new_instance->pis,i), "NCube"));
  }
  
  new_instance->cover_flags = malloc(sizeof(uint8_t *)*num_terms);
  
  for(i=0; i<num_terms; i++){

    assert(objIsOfClass(objAtVectorIndex(new_instance->terms,i), "Term"));

    new_instance->cover_flags[i] = malloc(sizeof(uint8_t)*num_pis);
    assert(new_instance->cover_flags[i] != NULL);

    term_num = getTermValue((Term *)objAtVectorIndex(new_instance->terms,i));
    
    initTermCoverArray(new_instance->pis, term_num,
                       new_instance->cover_flags[i]);
  }

  new_instance->essential_pis = NULL;

  return new_instance;
}


OBVector * findEssentialPIs(RTable *table){

  uint32_t i, j, num_terms, num_pis, is_resolved;
  OBVector *unknown_pis, *unresolved_terms, *sub_essentials;
  RTable *sub_table;

  assert(table != NULL);

  /* if the essential pis have already been found */
  if(table->essential_pis){
    return copyVector(table->essential_pis);
  }

  /* create vectors */
  table->essential_pis = createVector(1);
  assert(table->essential_pis != NULL);
  unresolved_terms = createVector(1);
  assert(unresolved_terms != NULL);
  unknown_pis = createVector(1);
  assert(unknown_pis != NULL);

  num_pis = sizeOfVector(table->pis);
  num_terms = sizeOfVector(table->terms);

  /* find terms that are not covered by a known essential pi */
  for(i=0; i<num_terms; i++){

    /* by default the term is not resolved */
    is_resolved = 0;
    for(j=0; j<num_pis; j++){
      /* if term is covered by a known essential cube it is resolved */
      if(table->cover_flags[i][j] == 1 && 
         isNCubeEssential((NCube *)objAtVectorIndex(table->pis, j))){
        is_resolved = 1; 
        break;
      }
    }

    /* if cube is not covered by a known essential cube, add it to the vector
     * of unresolved terms */
    if(!is_resolved){
      addToVector(unresolved_terms, objAtVectorIndex(table->terms, i));
    }
  }

  /* compose essential and unknown NCube vectors */
  for(i=0; i<num_pis; i++){
    if(isNCubeEssential((NCube *)objAtVectorIndex(table->pis, i)))
      addToVector(table->essential_pis, objAtVectorIndex(table->pis, i));
    else addToVector(unknown_pis, objAtVectorIndex(table->pis, i));
  }

  /* if all terms are unresolved, resort to brute force combination
   * checking via Petricks Method */
  if(sizeOfVector(unresolved_terms) == sizeOfVector(table->terms)){
    sub_essentials = petricksReduce(unknown_pis, unresolved_terms);
    catVectors(table->essential_pis, sub_essentials);
    sub_essentials = (OBVector *)release((obj *)sub_essentials);
  }
  /* else if only some terms are resolved, but not all, recursively create a sub
   * RTable and use it to further reduce pis */
  else if(sizeOfVector(unresolved_terms) > 0){
    sub_table = createRTable(unknown_pis, unresolved_terms);
    sub_essentials = findEssentialPIs(sub_table);
    sub_table = (RTable *)release((obj *)sub_table);
    catVectors(table->essential_pis, sub_essentials);
    sub_essentials = (OBVector *)release((obj *)sub_essentials);
  }

  /* all terms are resolved, and all essential NCubes found */

  /* release unneeded vectors */

  return copyVector(table->essential_pis);
}


/* PRIVATE METHODS */

void initRTableBase(RTable *to_init){

  /* Classname for the this specific class */
  static char *classname = NULL;
  const char stack_classname[] = "RTable";

  assert(to_init != NULL);

  if(!classname){
    classname = malloc(sizeof(char) * (strlen(stack_classname)+1));
    assert(classname != NULL);
    strcpy(classname, stack_classname);
  }

  /* initialize reference counting base data */
  initBase((obj *)to_init, &deallocRTable, classname);

  return;
}


void deallocRTable(obj *to_dealloc){

  uint64_t i, num_terms;

  /* cast generic obj to RTable */
  RTable *instance = (RTable *)to_dealloc;

  assert(to_dealloc && objIsOfClass(to_dealloc, "RTable"));

  num_terms = sizeOfVector(instance->terms);
  for(i=0; i<num_terms; i++){
    free(instance->cover_flags[i]);
  }

  free(instance->cover_flags);
  release((obj *)instance->pis);
  release((obj *)instance->terms);
  release((obj *)instance->essential_pis);

  free(instance);

  return;
}


void initTermCoverArray(const OBVector *cubes, uint32_t term, uint8_t *array){
  
  NCube *candidate;
  uint32_t i, num_cubes, num_match;

  num_cubes = sizeOfVector(cubes);
  num_match = 0;
  for(i=0; i<num_cubes; i++){
    if(nCubeCoversTerm((NCube *)objAtVectorIndex(cubes, i), term)){
      array[i] = 1;
      num_match++;
      candidate = (NCube *)objAtVectorIndex(cubes, i);
    }
    else array[i] = 0;
  }

  /* at least one cube should cover a term */
  assert(num_match != 0);

  /* if only one cube covers that term it must be essential, mark it as so */
  if(num_match == 1) markNCubeAsEssential(candidate);

  return;
}


OBVector * petricksReduce(const OBVector *unresolved_cubes,
                          const OBVector *unresolved_terms){

  NCube ***petricks_cubes, *tmp;
  uint32_t i, num_terms, *counts, *cur_idxs;
  uint64_t total_combos;
  OBVector *cur_group, *best_group; /* group of NCubes that completely covers
                                       given terms */
  double cur_avg_order, best_avg_order; /* the better the avg_order, the better
                                           the cube grouping */

  best_group = NULL;
  num_terms = sizeOfVector(unresolved_terms);

  counts = malloc(sizeof(uint32_t)*num_terms);
  assert(counts != NULL);
  cur_idxs = malloc(sizeof(uint32_t)*num_terms);
  assert(cur_idxs != NULL);

  /* init count */
  for(i=0; i<num_terms; i++){
    cur_idxs[i] = 0;
  }

  petricks_cubes = initPetricksData(unresolved_cubes, unresolved_terms, counts);

  /* calculate problem space, notify user for time estimate */
  total_combos = 1;
  for(i=0; i<num_terms; i++){
    total_combos *= counts[i];
  }
  fprintf(stderr, "\nResorting to brute force Petricks method to find final "
                  "essential terms\n"
                  "Searching for best coverage from %llu possible coverages..."
                  "\n", total_combos);

  best_avg_order = -1;

  /* iterate through all possible combinations of cubes that completely cover
   * the given terms */
  while(1){
    
    cur_group = createVector(num_terms);
    cur_avg_order = 0;

    /* get current group, and sum of orders */
    for(i=0; i<num_terms; i++){
      /* returning null values... */
      tmp = petricks_cubes[i][cur_idxs[i]];

      /* if the NCube is not already in the group (and accounted for) then add
       * it */
      if(!findObjInVector(cur_group, (obj *)tmp, &compareNCubes)){
        addToVector(cur_group, (obj *)tmp);
        cur_avg_order += orderOfNCube(tmp);
      } 
    }

    cur_avg_order /= sizeOfVector(cur_group);

    /* if average order of cur_group is better than best_group, there is a new
     * best group */
    if(cur_avg_order > best_avg_order){
      best_avg_order = cur_avg_order;
      if(best_group) release((obj *)best_group);
      best_group = cur_group;
    }
    /* else release the current group, its not the best */
    else release((obj *)cur_group);

    if(nextCoverCombo(counts, cur_idxs, num_terms)) break;
  }

  /* free all unneeded memory */
  for(i=0; i<num_terms; i++){
    free(petricks_cubes[i]);
  }
  free(petricks_cubes);
  free(counts);
  free(cur_idxs);
  
  return best_group;
}


NCube *** initPetricksData(const OBVector *unresolved_cubes,
                           const OBVector *unresolved_terms, uint32_t *counts){

  NCube ***petricks_cubes;
  uint32_t i, j, k, cur_term, num_terms, num_cubes;

  num_terms = sizeOfVector(unresolved_terms);
  num_cubes = sizeOfVector(unresolved_cubes);

  petricks_cubes = malloc(sizeof(NCube *)*num_terms);
  assert(petricks_cubes != NULL);

  /* for all unresolved terms */
  for(i=0; i<num_terms; i++){

    /* init count (number of cubes matching term) to 0 */
    counts[i] = 0;
    cur_term = getTermValue((Term *)objAtVectorIndex(unresolved_terms, i));

    /* for all unresolved cubes, count matching */
    for(j=0; j<num_cubes; j++){
      if(nCubeCoversTerm((NCube *)objAtVectorIndex(unresolved_cubes, j),
                         cur_term)) counts[i]++;
    }
    
    assert(counts[i] > 0);

    petricks_cubes[i] = malloc(sizeof(NCube *)*counts[i]);
    assert(petricks_cubes[i] != NULL);

    k=0;

    /* for all unresolved cubes, add to petricks_cubes if cube covers term */
    for(j=0; j<num_cubes; j++){
      if(nCubeCoversTerm((NCube *)objAtVectorIndex(unresolved_cubes, j),
                         cur_term))
        petricks_cubes[i][k++] = (NCube *)objAtVectorIndex(unresolved_cubes,j);
    }
  }

  return petricks_cubes;
}


uint8_t nextCoverCombo(const uint32_t *max_idxs, uint32_t *cur_idxs,
                       uint32_t array_size){

  uint32_t i, idxs_overflowed;

  idxs_overflowed = 0;

  /* increment combo counter */
  for(i=0; i<array_size; i++){
    /* if incrementing array position i causes count to overflow, set to 0 and
     * increment overflow count */
    if(++cur_idxs[i] >= max_idxs[i]){
      cur_idxs[i] = 0;
      idxs_overflowed++;
    }
    /* else if not overflow occured, nothing need be done for increment */
    else break;
  }

  /* if array_size number of overflows occured the combo counter reset to 0,
   * return 1 */
  if(idxs_overflowed == array_size){
    return 1;
  }

  return 0;
}
