/**
 * @file OBVector.c
 * @brief OBVector Method Implementation
 * @author anonymous
 */

#include "../../include/OBVector.h"
#include "../../include/private/OBVector_Private.h"

/* PUBLIC METHODS */

OBVector * createVector(uint32_t initial_capacity){

  static const char classname[] = "OBVector";

  OBVector *new_instance = malloc(sizeof(OBVector));
  assert(new_instance != NULL);

  /* initialize reference counting base data */
  initBase((obj *)new_instance, &deallocVector, &hashVector, &compareVectors,
           classname);

  /* a vector with zero capacity cannot be created, create one with a capacity
   * of one */
  if(initial_capacity == 0){
    initial_capacity = 1;
  }

  new_instance->array = malloc(sizeof(obj *)*initial_capacity);
  assert(new_instance->array != NULL);

  new_instance->capacity = initial_capacity;
  new_instance->num_objs = 0;

  return new_instance;
}


OBVector * copyVector(const OBVector *to_copy){

  uint32_t i;
  OBVector *new_vec;

  /* if there is nothing to copy, do nothing */
  assert(to_copy);

  new_vec = createVector(to_copy->capacity);
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
  assert(v != NULL);
  return v->num_objs;
}


void fitVectorToContents(OBVector *v){

  uint32_t i;
  obj **new_array;
  
  assert(v);

  new_array= malloc(sizeof(obj *)*v->num_objs);
  assert(new_array);

  for(i=0; i<v->num_objs; i++){
    new_array[i] = v->array[i];
  }

  v->capacity = v->num_objs;
  free(v->array);
  v->array = new_array;

  return;
}


void addToVector(OBVector *v, obj *to_add){
  assert(v != NULL);
  assert(to_add != NULL);
  insertAtVectorIndex(v, to_add, v->num_objs);
  return;
}


void insertAtVectorIndex(OBVector *v, obj *to_add, int64_t index){

  int64_t i;

  assert(v != NULL);
  assert(to_add != NULL);
  assert(index <= v->num_objs);

  /* if indexing past array bounds, add to the end of the array */
  if(index > v->num_objs) index = v->num_objs;
  /* if negatively indexing, index from the end of the array backwards */
  else if(index < 0){
    if(index < -((int64_t)v->num_objs)) index = 0;
    else index += v->num_objs; 
  }

  resizeVector(v);

  /* shift all entries at or after index by 1 */
  for(i=v->num_objs; i>index; i--){
    v->array[i] = v->array[i-1];
  }

  retain(to_add);
  v->array[index] = to_add;
  v->num_objs++;

  return;
}


void catVectors(OBVector *destination, OBVector *to_append){

  uint64_t i, new_cap;
  obj **new_array;

  assert(destination != NULL);
  assert(to_append != NULL);
  /* make sure that the two vectors will not overflow when concatenated */
  assert(destination->num_objs + to_append->num_objs > destination->num_objs);

  new_cap = destination->capacity;
  while(new_cap < destination->num_objs + to_append->num_objs){
    new_cap <<= 1;
    if(new_cap > UINT32_MAX){
      new_cap = UINT32_MAX;
      break;
    }
  }

  /* ensure that the new capacity is indeed big enough for the concatenation */
  assert(new_cap > destination->num_objs + to_append->num_objs);

  new_array = malloc(sizeof(obj *)*new_cap);
  assert(new_array != NULL);

  /* copy contents of destination first, without retaining because objs are
   * still held in the same container */
  for(i=0; i<destination->num_objs; i++){
    new_array[i] = destination->array[i];
  }

  /* copy contents of to_append */
  for(i=0; i<to_append->num_objs; i++){
    retain((obj *)to_append->array[i]);
    new_array[i+destination->num_objs] = to_append->array[i];
  }

  free(destination->array);
  destination->array = new_array;
  destination->num_objs += to_append->num_objs;
  destination->capacity = new_cap;

  return;
}


void replaceInVector(OBVector *v, obj *new_obj, int64_t index){

  assert(v != NULL);
  assert(new_obj != NULL);

  /* if indexing past array bounds, do nothing */
  if(index > v->num_objs) return;
  /* if negatively indexing, index from the end of the array backwards */
  else if(index < 0){
    if(index < -((int64_t)v->num_objs)) return;
    else index += v->num_objs; 
  }

  retain(new_obj);
  release(v->array[index]);
  v->array[index] = new_obj;

  return;
}


obj * objAtVectorIndex(const OBVector *v, int64_t index){
  assert(v != NULL);

  /* if indexing past array bounds, return NULL */
  if(index > v->num_objs) return NULL;

  /* if negatively indexing, index from the end of the array backwards, where
   * -1 corresponds to the very end of the array (and is equivalent to 
   *  a call to addToVector */
  if(index < 0){
    if(index < -((int64_t)v->num_objs)) return NULL;
    else index += v->num_objs; 
  }

  return v->array[index];
}


uint8_t findObjInVector(const OBVector *v, const obj *to_find){

  uint32_t i;

  assert(v != NULL);
  assert(to_find != NULL);

  for(i=0; i<v->num_objs; i++){
    /* if the object exists in the vector */
    if(compare(to_find, v->array[i]) == OB_EQUAL_TO){
      return 1;
    }
  }

  return 0;
}


void sortVector(OBVector *v, int8_t order){

  obj **sorted;

  assert(v != NULL);

  /* custom comparison function was not added, use simple pointer comparator */
  sorted = recursiveSortContents(v->array, v->num_objs, order, &compare);

  free(v->array);
  v->array = sorted;

  return;
}


void sortVectorWithFunct(OBVector *v, int8_t order, compare_fptr funct){

  obj **sorted;

  assert(v != NULL);
  assert(funct != NULL);

  /* custom comparison function was not added, use simple pointer comparator */
  sorted = recursiveSortContents(v->array, v->num_objs, order, funct);

  free(v->array);
  v->array = sorted;

  return;
}


void removeFromVectorEnd(OBVector *v){
  assert(v != NULL);
  return removeFromVectorIndex(v, v->num_objs-1);
}


void removeFromVectorIndex(OBVector *v, int64_t index){
  
  int64_t i;

  assert(v != NULL);

  /* if the vector is empty, do nothing */
  if(v->num_objs < 1){
    return;
  }

  /* if indexing past array bounds do nothing */
  if(index > v->num_objs) return;

  /* if negatively indexing, index from the end of the array backwards, where
   * -1 corresponds to the very end of the array (and is equivalent to 
   *  a call to addToVector */
  if(index < 0){
    if(index < -((int64_t)v->num_objs)) return;
    else index += v->num_objs; 
  }

  /* release object being removed */
  release(v->array[index]);
  
  /* fill in space left by object removal */
  for(i=index+1; i<v->num_objs; i++){
    v->array[i-1] = v->array[i];
  }

  v->num_objs--;

  return;
}

void clearVector(OBVector *v){

  uint32_t i;

  assert(v != NULL);

  for(i=0; i<v->num_objs; i++){
    release(v->array[i]);
  }

  v->num_objs = 0;

  return;
}


/* PRIVATE METHODS */


void resizeVector(OBVector *v){

  uint32_t i, new_cap;
  obj **array;

  if(v->num_objs == v->capacity){
    
    /* if maximum size has been reached fail*/
    assert(v->capacity != UINT32_MAX);
    
    new_cap = v->capacity*2;

    if(new_cap > UINT32_MAX){
      new_cap = UINT32_MAX;
    }
    
    array = malloc(sizeof(obj *)*new_cap);
    assert(array != NULL);

    for(i=0; i<v->num_objs; i++){
      array[i] = v->array[i];
    }

    free(v->array);
    v->array = array;
    v->capacity = new_cap;
  }

  return;
}


obj ** recursiveSortContents(obj **to_sort, uint32_t size, int8_t order,
                             compare_fptr funct){
  
  uint32_t i,j, split;
  int8_t comp_result;
  obj **left_sorted, **right_sorted;
  obj **sorted;

   /* base case, if the vector is of size one, its sorted */
  if(size <= 1){
    sorted = malloc(sizeof(obj *)*size);
    assert(sorted != NULL);
    *sorted = *to_sort;
    return sorted;
  }
  
  split = size/2;

  /* sort left half and right half of array */
  left_sorted = recursiveSortContents(to_sort, split, order, funct);
  right_sorted = recursiveSortContents(to_sort+split, size-split, 
                                       order, funct);
  
  sorted = malloc(sizeof(obj *)*size);
  assert(sorted != NULL);

  /* merge sorted halves */
  i=0;
  j=0;

  /* while there are elements in both halves to be added to sorted */
  while(i < split && j < (size-split)){
    
    /* if the comparison of the left to the right matches the desired order,
     * then the next item to go in the sorted array is from the left */
    if((comp_result = funct(left_sorted[i], right_sorted[j])) == order){
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


obhash_t hashVector(const obj *to_hash){

  static int8_t init = 0;
  static obhash_t seed;

  uint32_t i;
  obhash_t value;
  OBVector *instance = (OBVector *)to_hash;

  assert(to_hash);
  assert(objIsOfClass(to_hash, "OBVector"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }
  
  value = seed;
  for(i=0; i<instance->num_objs; i++){
    value += hash(instance->array[i]);
    value += value << 10;
    value ^= value >> 6;
  }

  value += value << 3;
  value ^= value >> 11;
  value += value << 15;

  return value;
}


int8_t compareVectors(const obj *a, const obj *b){

  uint32_t i;
  const OBVector *comp_a = (OBVector *)a;
  const OBVector *comp_b = (OBVector *)b;

  assert(a);
  assert(b);
  assert(objIsOfClass(a, "OBVector"));
  assert(objIsOfClass(b, "OBVector"));

  if(comp_a->num_objs != comp_b->num_objs) return OB_NOT_EQUAL;

  for(i=0; i<comp_a->num_objs; i++)
    if(compare(comp_a->array[i], comp_b->array[i]) != OB_EQUAL_TO)
      return OB_NOT_EQUAL;

  return OB_EQUAL_TO;
}


void deallocVector(obj *to_dealloc){

  /* cast generic obj to OBVector */
  OBVector *instance = (OBVector *)to_dealloc;

  assert(instance != NULL);
  assert(objIsOfClass(to_dealloc, "OBVector"));

  clearVector(instance); /* release all objs contained in vector */

  free(instance->array);
  return;
}

