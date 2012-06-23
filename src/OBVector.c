
#include "OBVector.h"
#include "OBVector_Private.h"

/* PUBLIC METHODS */

OBVector * createOBVector(const uint32_t initial_capacity){

  OBVector *new_instance = malloc(sizeof(OBVector));
  if(!new_instance){
    fprintf(stderr, "OBVector: Could not allocate memory for "
                    "new_instance\n");
    return NULL;
  }

  /* initialize reference counting base data */
  initBase((obj *)new_instance, &deallocVector);

  new_instance->array = malloc(sizeof(obj)*initial_capacity);
  if(!new_instance->array){
    fprintf(stderr, "OBVector: Could not allocate internal array in new "
                    "instance\n");
    free(new_instance);
    return NULL;
  }

  new_instance->capacity = initial_capacity;
  new_instance->num_objs = 0;

  return new_instance;
}

OBVector * copyVector(const OBVector *to_copy){

  int i;
  OBVector *new_vec;

  /* if there is nothing to copy, do nothing */
  if(!to_copy){
    return NULL;
  }

  new_vec = malloc(sizeof(OBVector));
  if(!new_vec){
    fprintf(stderr, "OBVector: Could not allocate memory for "
                    "copy of vector\n");
    return NULL;
  }

  /* initialize reference counting base data */
  initBase((obj *)new_vec, &deallocVector);

  new_vec->array = malloc(sizeof(obj)*to_copy->capacity);
  if(!new_vec->array){
    fprintf(stderr, "OBVector: Could not allocate internal array in new "
                    "instance\n");
    free(new_vec);
    return NULL;
  }
  
  new_vec->num_objs = to_copy->num_objs;
  new_vec->capacity = to_copy->capacity;

  for(i=0; i<to_copy->num_objs; i++){
    new_vec->array[i] = to_copy->array[i];
  }

  return new_vec;
}

uint32_t sizeOfVector(const OBVector *v){
  if(!v){
    return 0;
  }

  return v->num_objs;
}



/* PRIVATE METHODS */

void deallocVector(obj *to_dealloc){

  /* cast generic obj to OBVector */
  OBVector *instance = (OBVector *)to_dealloc;

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE */

  free(instance);
  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
