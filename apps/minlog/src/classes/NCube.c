
#include "../../include/NCube.h"
#include "../../include/private/NCube_Private.h"

/* PUBLIC METHODS */

NCube * createNCube(uint32_t term){

  NCube *new_instance;
  
  if(term >= (1 << 27)){
    fprintf(stderr, "NCube: Cannot create NCube for term %i, which is too "
                    "large\n", term);
    return NULL;
  }

  new_instance = createNCubeWithOrder(1);
  if(!new_instance){
    fprintf(stderr, "NCube: Could not create new NCube\n");
    return NULL;
  }

  new_instance->terms[0] = term;
  return new_instance;
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
    if(a->terms[i] = term){
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

  
  /* initialize reference counting base data */
  if(initBase((obj *)new_instance, &deallocNCube)){
    fprintf(stderr, "NCube: Base obj could not be initialized\n");
    return NULL;
  }

  new_cube = malloc(sizeof(NCube));
  if(!new_cube){
    fprintf(stderr, "NCube: Could not allocate memory for new NCube\n");
    return NULL;
  }

  new_cube->terms = malloc(sizeof(uint32_t)*(1<<order));
  if(!new_cube){
    fprintf(stderr, "NCube: Could not allocate memory for term array in new "
                    "NCube\n");
    return NULL;
  }

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
