
#include "../../include/OBVector.h"
#include "../../include/private/OBVector_Private.h"

/* PUBLIC METHODS */

OBVector * createVector(uint32_t initial_capacity){

  OBVector *new_instance = malloc(sizeof(OBVector));
  if(!new_instance){
    fprintf(stderr, "OBVector: Could not allocate memory for "
                    "new_instance\n");
    return NULL;
  }

  /* initialize reference counting base data */
  if(initBase((obj *)new_instance, &deallocVector)){
    fprintf(stderr, "OBVector: Could not initialize base obj\n");
    return NULL;
  }

  /* a vector with zero capacity cannot be created, create one with a capacity
   * of one */
  if(initial_capacity == 0){
    initial_capacity = 1;
  }

  new_instance->array = malloc(sizeof(obj *)*initial_capacity);
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

  uint32_t i;
  OBVector *new_vec;

  /* if there is nothing to copy, do nothing */
  if(!to_copy){
    return NULL;
  }

  new_vec = createVector(to_copy->capacity);
  if(!new_vec){
    fprintf(stderr, "OBVector: Could not create vector copy\n");
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

  uint32_t i;
  obj **new_array;
  
  if(!v){
    fprintf(stderr, "OBVector: NULL argument passed to fitVectorToContents\n");
    return 1;
  }

  new_array= malloc(sizeof(obj *)*v->num_objs);
  if(!new_array){
    fprintf(stderr, "OBVector: Could not allocate internal array in new "
                    "instance\n");
    return 1;
  }

  for(i=0; i<v->num_objs; i++){
    new_array[i] = v->array[i];
  }

  v->capacity = v->num_objs;
  free(v->array);
  v->array = new_array;

  return 0;
}


uint8_t addToVector(OBVector *v, obj *to_add){
  return insertAtVectorIndex(v, to_add, v->num_objs);
}

uint8_t insertAtVectorIndex(OBVector *v, obj *to_add, uint32_t index){

  uint32_t i;

  if(!v || !to_add){
    fprintf(stderr, "OBVector: NULL argument(s) passed to addToVector\n");
    return 1;
  }

  /* if the index is beyond the end of the current vector, display error but
   * attempt to add to the end */
  if(index > v->num_objs){
    fprintf(stderr, "OBVector: Attempting to add obj beyond valid range. "
                    "Adding to the end instead\n");
    index = v->num_objs;
  }

  if(resizeVector(v)){
    fprintf(stderr, "OBVector: Could not resize vector to accommadate new "
                    "obj\n");
    return 1;
  }

  /* shift all entries at or after index by 1 */
  for(i=v->num_objs; i>index; i--){
    v->array[i] = v->array[i-1];
  }

  retain(to_add);
  v->array[index] = to_add;
  v->num_objs++;

  return 0;
}

uint8_t replaceInVector(OBVector *v, obj *new_obj, const uint32_t index){

  if(!v || !new_obj){
    fprintf(stderr, "OBVector: NULL argument(s) passed to replaceInVector\n");
    return 1;
  }

  if(index >= v->num_objs){
    fprintf(stderr, "OBVector: attempting to access %i, which is out of vector "
                    "item range, 0-%i\n", index, v->num_objs);
    return 1;
  }

  retain(new_obj);
  release(v->array[index]);
  v->array[index] = new_obj;

  return 0;
}


obj * objAtVectorIndex(const OBVector *v, const uint32_t index){

  if(!v){
    fprintf(stderr, "OBVector: NULL argument passed to objAtVectorIndex\n");
    return NULL;
  }


  if(index >= v->num_objs){
    fprintf(stderr, "OBVector: attempting to access %i, which is out of vector "
                    "item range, 0-%i\n", index, v->num_objs);
    return NULL;
  }

  return v->array[index];
}


uint8_t findObjInVector(const OBVector *v, const obj *to_find,
                        compare_fptr compare){

  uint32_t i;

  if(!v || !to_find){
    fprintf(stderr, "OBVector: Unexpected NULL argument(s) passed to "
                    "findObjInVector\n");
    return 0;
  }

  /* custom comparison function was not added, use simple pointer comparator */
  if(compare == NULL){
    compare = &objCompare;
  }

  for(i=0; i<v->num_objs; i++){
    /* if the object exists in the vector */
    if(compare(to_find, v->array[i]) == 0){
      return 1;
    }
  }

  return 0;
}


uint8_t sortVector(OBVector *v, const compare_fptr compare,
                   const int8_t order){

  obj **sorted;

  if(!v || !compare){
    fprintf(stderr, "OBVector: Unexpected NULL argument(s) passed to "
                    "findObjInVector\n");
    return 1;
  }

  sorted = recursiveSortContents(v->array, v->num_objs, compare, order);
  if(!sorted){
    fprintf(stderr, "OBVector: Sorting operation failed\n");
    return 1;
  }

  free(v->array);
  v->array = sorted;

  return 0;
}

void removeFromVectorEnd(OBVector *v){
  return removeFromVectorIndex(v, v->num_objs-1);
}

void removeFromVectorIndex(OBVector *v, uint32_t index){
  
  uint32_t i;

  if(!v){
    fprintf(stderr, "OBVector: NULL argument passed to removeFromVector\n");
    return;
  }

  /* if the index is beyond the end of the current vector, display error but
   * attempt to add to the end */
  if(index >= v->num_objs){
    fprintf(stderr, "OBVector: Attempting to remove obj beyond valid range\n");
    return;
  }

  /* if the vector is empty, do nothing */
  if(v->num_objs < 1){
    fprintf(stderr, "OBVector: Cannot remove element from empty array\n");
    return;
  }

  /* release object being removed */
  release(v->array[index]);
  
  for(i=index+1; i<v->num_objs; i++){
    v->array[i-1] = v->array[i];
  }

  v->num_objs--;

  return;
}


/* PRIVATE METHODS */

void deallocVector(obj *to_dealloc){

  uint32_t i;

  /* cast generic obj to OBVector */
  OBVector *instance = (OBVector *)to_dealloc;

  /* release all objects stored inside vector */
  for(i=0; i<instance->num_objs; i++){
    release((obj *)instance->array[i]);
  }

  free(instance->array);
  free(instance);
  return;
}

uint8_t resizeVector(OBVector *v){

  uint32_t i, new_cap;
  obj **array;

  if(v->num_objs == v->capacity){
    
    /* if maximum size has been reached print msg */
    if(v->capacity == UINT32_MAX){
      fprintf(stderr, "OBVector: Maximum vector capacity of 2^32 objs "
                      "reached\n");
      return 1;
    }
    
    new_cap = v->capacity*2;

    if(new_cap > UINT32_MAX){
      new_cap = UINT32_MAX;
    }
    
    array = malloc(sizeof(obj *)*new_cap);
    if(!array){
      fprintf(stderr, "OBVector: Could not allocate larger internal array for "
                      "resize\n");
      return 1;
    }

    for(i=0; i<v->num_objs; i++){
      array[i] = v->array[i];
    }

    free(v->array);
    v->array = array;
    v->capacity = new_cap;
  }

  return 0;
}

obj ** recursiveSortContents(obj **to_sort, uint32_t size,
                             const compare_fptr compare, int8_t order){
  
  uint32_t i,j, split;
  obj **left_sorted, **right_sorted;
  obj **sorted;

   /* base case, if the vector is of size one, its sorted */
  if(size <= 1){
    sorted = malloc(sizeof(obj *)*size);
    if(!sorted){
      fprintf(stderr, "OBVector: Could not allocate internal array during "
                      "sort\n");
      return NULL;
    }
    *sorted = *to_sort;
    return sorted;
  }
  
  split = size/2;

  /* sort left half and right half of array */
  left_sorted = recursiveSortContents(to_sort, split, compare, order);
  right_sorted = recursiveSortContents(to_sort+split, size-split, compare, 
                                       order);
  
  /* if sorting a half failed, return NULL */
  if(!left_sorted || !right_sorted){
    return NULL;
  }

  sorted = malloc(sizeof(obj *)*size);
  if(!sorted){
    fprintf(stderr, "OBVector: Could not allocate internal array during "
                    "sort\n");
    return NULL;
  }

  /* merge sorted halves */
  i=0;
  j=0;

  /* while there are elements in both halves to be added to sorted */
  while(i < split && j < (size-split)){
    
    /* if the comparison of the left to the right matches the desired order,
     * then the next item to go in the sorted array is from the left */
    if(compare(left_sorted[i], right_sorted[j]) == order){
      sorted[i+j] = left_sorted[i];
      i++;
    }
    else{
      sorted[i+j] = right_sorted[j];
      j++;
    }
  }

  /* place remaining elements from remaining half in sorted */
  if(i == split){
    while(j<(size - split)){
      sorted[i+j] = right_sorted[j];
      j++;
    }
  }
  else{
    while(i<split){
      sorted[i+j] = left_sorted[i];
      i++;
    }
  }

  /* free sorted half arrays */
  free(left_sorted);
  free(right_sorted);

  return sorted;
}
