
#include "../../include/OBVector.h"
#include "../../include/private/OBVector_Private.h"

/* PUBLIC METHODS */

OBVector * createVector(uint32_t initial_capacity){

  OBVector *new_instance = malloc(sizeof(OBVector));
  assert(new_instance != NULL);

  /* initialize reference counting base data */
  initVectorBase(new_instance);

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
  assert(v != NULL && to_add != NULL);
  insertAtVectorIndex(v, to_add, v->num_objs);
  return;
}


void insertAtVectorIndex(OBVector *v, obj *to_add, uint32_t index){

  uint64_t i;

  assert(v != NULL && to_add != NULL && index <= v->num_objs);

  /* if the index is beyond the end of the current vector, display error but
   * attempt to add to the end */

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

  assert(destination != NULL && to_append != NULL);

  new_cap = destination->capacity;
  while(new_cap < destination->num_objs + to_append->num_objs){
    new_cap *= new_cap;
    /* assert that overflow did not occur */
    assert(new_cap > destination->capacity); 
  }

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


void replaceInVector(OBVector *v, obj *new_obj, const uint32_t index){

  assert(v != NULL && new_obj != NULL && index < v->num_objs);

  retain(new_obj);
  release(v->array[index]);
  v->array[index] = new_obj;

  return;
}


obj * objAtVectorIndex(const OBVector *v, const uint32_t index){
  assert(v != NULL && index < v->num_objs);
  return v->array[index];
}


uint8_t findObjInVector(const OBVector *v, const obj *to_find,
                        compare_fptr compare){

  uint32_t i;

  assert(v != NULL && to_find != NULL);

  /* custom comparison function was not added, use simple pointer comparator */
  if(!compare) compare = &objCompare;

  for(i=0; i<v->num_objs; i++){
    /* if the object exists in the vector */
    if(compare(to_find, v->array[i]) == 0){
      return 1;
    }
  }

  return 0;
}


void sortVector(OBVector *v, const compare_fptr compare,
                   const int8_t order){

  compare_fptr compare_funct;
  obj **sorted;

  assert(v != NULL);

  /* custom comparison function was not added, use simple pointer comparator */
  if(!compare) compare_funct = &objCompare;
  else compare_funct = compare;

  sorted = recursiveSortContents(v->array, v->num_objs, compare_funct, order);

  free(v->array);
  v->array = sorted;

  return;
}

void removeFromVectorEnd(OBVector *v){
  assert(v != NULL);
  return removeFromVectorIndex(v, v->num_objs-1);
}

void removeFromVectorIndex(OBVector *v, uint32_t index){
  
  uint32_t i;

  assert(v != NULL && index < v->num_objs);

  /* if the vector is empty, do nothing */
  if(v->num_objs < 1){
    return;
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


/* PRIVATE METHODS */

void initVectorBase(OBVector *to_init){

  /* Classname for the this specific class */
  static char *classname = NULL;
  const char stack_classname[] = "OBVector";

  assert(to_init != NULL);

  if(!classname){
    classname = malloc(sizeof(char) * strlen(stack_classname));
    assert(classname != NULL);
    strcpy(classname, stack_classname);
  }

  /* initialize reference counting base data */
  initBase((obj *)to_init, &deallocVector, classname);
  return;
}

void deallocVector(obj *to_dealloc){

  uint32_t i;

  /* cast generic obj to OBVector */
  OBVector *instance = (OBVector *)to_dealloc;
  assert(instance != NULL && objIsOfClass(to_dealloc, "OBVector"));

  /* release all objects stored inside vector */
  for(i=0; i<instance->num_objs; i++){
    release((obj *)instance->array[i]);
  }

  free(instance->array);
  free(instance);
  return;
}

void resizeVector(OBVector *v){

  uint32_t i, new_cap;
  obj **array;

  if(v->num_objs == v->capacity){
    
    /* if maximum size has been reached print msg */
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

obj ** recursiveSortContents(obj **to_sort, uint32_t size,
                             const compare_fptr compare, int8_t order){
  
  uint32_t i,j, split;
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
  left_sorted = recursiveSortContents(to_sort, split, compare, order);
  right_sorted = recursiveSortContents(to_sort+split, size-split, compare, 
                                       order);
  
  sorted = malloc(sizeof(obj *)*size);
  assert(sorted != NULL);

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
