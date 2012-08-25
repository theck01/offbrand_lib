
#include "../../include/NCube.h"
#include "../../include/private/NCube_Private.h"
#include "../../include/misc_functions.h"

/* PUBLIC METHODS */

NCube * createNCube(uint32_t term){

  NCube *new_instance;
  
  if(term >= (1 << 27)){
    fprintf(stderr, "NCube: Cannot create NCube for term %i, which is too "
                    "large\n", term);
    return NULL;
  }

  new_instance = createNCubeWithOrder(0);
  if(!new_instance){
    fprintf(stderr, "NCube: Could not create new NCube\n");
    return NULL;
  }

  new_instance->terms[0] = term;
  return new_instance;
}


NCube * mergeNCubes(NCube *a, NCube *b){

  uint32_t xor_term, i, j, numterms;
  NCube *result;

  /* if dont care bits are not the same, do not merge */
  if(a->dont_cares != b->dont_cares){
    return NULL;
  }

  /* if the terms do not differ by only a single bit (a power of two), do
   * not merge */
  xor_term = a->terms[0] ^ b->terms[0];
  if(numOneBits(xor_term) != 1){
    return NULL;
  }

  result = createNCubeWithOrder(a->order + 1);
  if(!result){
    fprintf(stderr, "NCube: Could not create new cube when mergining\n");
    return NULL;
  }
  
  i=0;
  j=0;
  numterms = 1 << a->order;

  /* merge sort terms into new resultant term, lowest to highest */
  while(i<numterms && j<numterms){
    if(a->terms[i] < b->terms[j]){
      result->terms[i+j] = a->terms[i];
      i++;
    }
    else{
      result->terms[i+j] = b->terms[j];
      j++;
    }
  }
  /* add remaining terms from cube, loops does nothing if all terms from cube
   * already added */
  for( ; i<numterms; i++){
    result->terms[i+j] = a->terms[i];
  }
  for( ; j<numterms; j++){
    result->terms[i+j] = b->terms[j];
  }

  /* add in additional dont care bit */
  result->dont_cares = a->dont_cares | xor_term;

  /* set a and b as non prime implicants */
  a->prime_implicant = 0;
  b->prime_implicant = 0;

  return result;
}


/* function can be deleted if unneeded */
int8_t compareNCube(const obj *a, const obj *b){
  
  uint32_t i, max_i;
  NCube *comp_a = (NCube *)a;  
  NCube *comp_b = (NCube *)b;  

  if(comp_a->order != comp_b->order){
    return 1;
  }

  max_i = 1 << comp_a->order;
  for(i=0; i<max_i; i++){
    if(comp_a->terms[i] != comp_b->terms[i]){
      return 1;
    }
  }

  return 0;
}


uint8_t nCubeCoversTerm(const NCube *a, uint32_t term){

  uint32_t i, max_i;
  
  max_i = 1 << a->order;
  for(i=0; i<max_i; i++){
    if(a->terms[i] == term){
      return 1;
    }
  }

  return 0;
}


uint8_t isNCubePrimeImplicant(const NCube *a){
  return a->prime_implicant;
}


/* PRIVATE METHODS */

NCube * createNCubeWithOrder(uint8_t order){
  
  NCube *new_cube;

  if(order > 27){
    fprintf(stderr, "NCube: Cannot create an NCube of order greater than 27\n");
    return NULL;
  }

  new_cube = malloc(sizeof(NCube));
  if(!new_cube){
    fprintf(stderr, "NCube: Could not allocate memory for new NCube\n");
    return NULL;
  }

  /* initialize reference counting base data */
  if(initBase((obj *)new_cube, &deallocNCube)){
    fprintf(stderr, "NCube: Base obj could not be initialized\n");
    return NULL;
  }

  new_cube->terms = malloc(sizeof(uint32_t)*(1<<order));
  if(!new_cube){
    fprintf(stderr, "NCube: Could not allocate memory for term array in new "
                    "NCube\n");
    return NULL;
  }

  new_cube->dont_cares = 0;
  new_cube->order = order;
  new_cube->prime_implicant = 1;
  return new_cube;
}


void deallocNCube(obj *to_dealloc){
  /* cast generic obj to NCube */
  NCube *instance = (NCube *)to_dealloc;
  free(instance->terms);
  free(instance);
  return;
}
