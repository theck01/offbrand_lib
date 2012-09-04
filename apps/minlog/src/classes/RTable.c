
#include "../../include/RTable.h"
#include "../../include/private/RTable_Private.h"
#include "../../include/NCube.h"
#include "../../include/Term.h"

/* PUBLIC METHODS */

/* add arguments to complete initialization as needed, modify RTable.h
 * as well if modifications are made */
RTable * createRTable(OBVector *prime_implicants, OBVector *terms){

  uint64_t i, j, num_pis, num_terms;
  uint32_t term_num;
  NCube *tmp;
  RTable *new_instance = malloc(sizeof(RTable));
  assert(new_instance != NULL);

  /* initialize reference counting base of class */
  initRTableBase((obj *)new_instance);

  retain((obj *)prime_implicants);
  new_instance->pis = prime_implicants;
  retain((obj *)terms);
  new_instance->terms = terms;

  num_pis = sizeOfVector(prime_implicants);
  num_terms = sizeOfVector(terms);

  /* verify vectors are of proper format and allocate/initialize 2D array */
  for(i=0; i<num_pis; i++){
    assert(objIsOfClass(objAtVectorIndex(prime_implicants,i), "NCube"));
  }
  
  new_instance->cover_flags = malloc(sizeof(uint8_t *)*num_terms);
  
  for(i=0; i<num_terms; i++){

    assert(objIsOfClass(objAtVectorIndex(terms,i), "Term"));

    new_instance->cover_flags[i] = malloc(sizeof(uint8_t)*num_pis);
    assert(new_instance->cover_flags[i] != NULL);

    term_num = getTermValue((Term *)objAtVectorIndex(terms,i));
    
    /* fill in cover_flag matrix, 1 if assocated term is covered by associated
     * NCube, 0 if not */
    for(j=0; j<num_pis; j++){
      if(nCubeCoversTerm((NCube *)objAtVectorIndex(prime_implicants,j),
                         term_num)){
        new_instance->cover_flags[i][j] = 1;
      }
      else new_instance->cover_flags[i][j] = 0;
    }
  }

  return new_instance;
}


/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


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

  /* cast generic obj to RTable */
  RTable *instance = (RTable *)to_dealloc;

  assert(to_dealloc && objIsOfClass(to_dealloc, "RTable");

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE */

  free(instance);

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
