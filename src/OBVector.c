
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
    retain((obj *)new_vec->array[i]); /*retain all objects, the new vector also
                                        references them */
  }

  return new_vec;
}

uint32_t sizeOfVector(const OBVector *v){
  if(!v){
    return 0;
  }

  return v->num_objs;
}


uint8_t fitVectorToContents(OBVector *v){

  int i;
  obj *new_array;
  
  if(!v){
    fprintf(stderr, "OBVector: NULL argument passed to fitVectorToContents\n");
    return 1;
  }

  new_array= malloc(sizeof(obj)*v->num_objs);
  if(new_array){
    fprintf(stderr, "OBVector: Could not allocate internal array in new "
                    "instance\n");
    return 1;
  }

  for(i=0; i<v->num_items; i++){
    new_array[i] = v->array[i];
  }

  v->capacity = v->num_objs;
  free(v->array);
  v->array = new_array;

  return 0;
}


uint8_t addToVector(OBVector *v, const obj *to_add){

  if(!v || !a){
    fprintf(stderr, "OBVector: NULL argument(s) passed to addToVector\n");
    return 1;
  }

  /* resizes vector if needed, processing error if one occured */
  if(resizeVector(v)){
    fprintf(stderr, "OBVector: resize failed, cannot add new obj\n");
    return 1;
  }

  v->array[v->num_objs++] = (obj *)to_add;
  retain(obj);

  return 0;
}

uint8_t replaceInVector(OBVector *v, const obj *new_obj, const uint32_t index){

  if(!v || !a){
    fprintf(stderr, "OBVector: NULL argument(s) passed to replaceInVector\n");
    return 1;
  }

  if(index >= v->num_objs){
    fprintf(stderr, "OBVector: attempting to access %i, which is out of vector\n"
                    "item range, 0-%i\n", index, v->num_objs);
    return 1;
  }

  release(v->array[index]);
  retain(new_obj);
  v->array[index] = new_obj;

  return 0;
}


obj * objAtVectorIndex(const OBVector *v, const uint32_t index){

  if(!v){
    fprintf(stderr, "OBVector: NULL argument passed to objAtVectorIndex\n");
    return NULL;
  }

  if(index < v->num_objs){
    fprintf(stderr, "OBVector: attempting to access %i, which is out of vector\n"
                    "item range, 0-%i\n", index, v->num_objs);
    return NULL;
  }

  return v->array[index];
}


uint8_t findObjInVector(const OBVector *v, const obj *to_find,
                        const compare_fptr compare){

  int i;

  if(!v || !to_find){
    fprintf(stderr, "OBVector: Unexpected NULL argument(s) passed to "
                    "findObjInVector\n");
    return 0;
  }

  if(compare == NULL){
    compare = &defaultCompare;
  }

  for(i=0; i<v->num_objs; i++){
    /* if the object exists in the vector */
    if(compare(to_find, v->array[i]) == 0){
      return 1;
    }
  }

  /* object not found */
  return 0;
}


void removeFromVectorEnd(OBVector *v){
  release((obj *)v->array[--v->num_objs]);
  return
}

/* PRIVATE METHODS */

void deallocVector(obj *to_dealloc){

  uint32_t i;

  /* cast generic obj to OBVector */
  OBVector *instance = (OBVector *)to_dealloc;

  /* release all objects stored inside vector */
  for(i=0; i<to_dealloc->num_objs; i++){
    release((obj *)to_dealloc->array[i]);
  }

  free(to_dealloc->array);
  free(to_dealloc);
  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
