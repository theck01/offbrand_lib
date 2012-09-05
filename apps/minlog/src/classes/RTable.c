
#include "../../include/RTable.h"
#include "../../include/private/RTable_Private.h"
#include "../../include/NCube.h"
#include "../../include/Term.h"

/* PUBLIC METHODS */

/* add arguments to complete initialization as needed, modify RTable.h
 * as well if modifications are made */
RTable * createRTable(OBVector *prime_implicants, OBVector *terms){

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
    
    processTermCoverArray(new_instance->terms, term_num, 
                          new_instance->cover_flags[i]);
  }

  return new_instance;
}


/* PRIVATE METHODS */

void initRTableBase(RTable *to_init){

  /* Classname for the this specific class */
  static char *classname = NULL;
  const char stack_classname[] = "RTable";

  assert(to_init != NULL);

  if(!classname){
    classname = malloc(sizeof(char) * strlen(stack_classname));
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

  free(instance);

  return;
}


void processTermCoverArray(OBVector *cubes, uint32_t term, uint8_t *array){
  
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

