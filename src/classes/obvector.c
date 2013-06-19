/**
 * @file obvector.c
 * @brief obvector Method Implementation
 * @author theck
 */

#include "../../include/obvector.h"
#include "../../include/private/obvector_private.h"

/* PUBLIC METHODS */

obvector * obvector_new(uint32_t initial_capacity){

  uint32_t i, new_cap;

  new_cap = 1;
  while(initial_capacity > new_cap) new_cap *= 2;
  if(new_cap > UINT32_MAX) new_cap = UINT32_MAX;

  obvector *new_instance = obvector_create_default(new_cap);
  for(i=0; i<new_cap; i++) new_instance->array[i] = NULL;

  return new_instance;
}


obvector * obvector_copy(const obvector *to_copy){

  uint32_t i;
  obvector *new_vec;

  /* if there is nothing to copy, do nothing */
  assert(to_copy);

  new_vec = obvector_create_default(to_copy->capacity);
  new_vec->length = to_copy->length;

  for(i=0; i<to_copy->capacity; i++){
    ob_retain(to_copy->array[i]);
    new_vec->array[i] = to_copy->array[i];
  }

  return new_vec;
}


uint32_t obvector_length(const obvector *v){
  assert(v != NULL);
  return v->length;
}


void obvector_store_at_index(obvector *v, obj *to_store, int64_t index){

  assert(v != NULL);

  /* if negatively indexing, index from the end of the array backwards */
  if(index < 0) index += v->length;

  assert(index < UINT32_MAX); /* assert not indexing beyond capacity */
  assert(index >= 0); /* assert not negative indexing after offset */

  /* ensure vector can store element at index */
  obvector_resize(v, (uint32_t)index);

  ob_retain(to_store);
  ob_release(v->array[index]);
  v->array[index] = to_store;

  /* find vector length if modifying beyond known length */
  if(index+1 >= v->length)
    v->length = obvector_find_valid_precursor(v->array, index) + 1;

  return;
}


obj * obvector_obj_at_index(const obvector *v, int64_t index){

  assert(v != NULL);

  /* if negatively indexing, index from the end of the array backwards */
  if(index < 0) index += v->length;
  assert(index < UINT32_MAX); /* assert not indexing beyond capacity */
  assert(index >= 0); /* assert not negative indexing beyond 0 index */

  if(index < v->length) return v->array[index];
  return NULL;
}


void obvector_concat(obvector *destination, obvector *to_append){

  uint64_t i;

  assert(destination != NULL);
  assert(to_append != NULL);
  /* make sure that the two vectors will not overflow when concatenated */
  assert(destination->length + to_append->length >= destination->length);

  /* ensure vector can store all elements in to_append */
  obvector_resize(destination, destination->length + to_append->length - 1);

  /* copy contents of to_append */
  for(i=0; i<to_append->length; i++){
    ob_retain((obj *)to_append->array[i]);
    destination->array[i+destination->length] = to_append->array[i];
  }

  destination->length += to_append->length;

  return;
}


uint8_t obvector_find_obj(const obvector *v, const obj *to_find){

  uint32_t i;

  assert(v != NULL);
  assert(to_find != NULL);

  for(i=0; i<v->length; i++){
    /* if the object exists in the vector */
    if(ob_compare(to_find, v->array[i]) == OB_EQUAL_TO){
      return 1;
    }
  }

  return 0;
}


void obvector_sort(obvector *v, int8_t order){
  obvector_sort_with_funct(v, order, &ob_compare);
}


void obvector_sort_with_funct(obvector *v, int8_t order, ob_compare_fptr funct){

  obj **sorted;

  assert(v != NULL);
  assert(funct != NULL);
  assert(order == OB_LEAST_TO_GREATEST || order == OB_GREATEST_TO_LEAST);

  sorted = obvector_recursive_sort(v->array, v->capacity, order, funct);

  free(v->array);
  v->array = sorted;
  v->length = obvector_find_valid_precursor(v->array, v->length-1) + 1;

  return;
}


void obvector_clear(obvector *v){

  uint32_t i;

  assert(v != NULL);

  for(i=0; i<v->capacity; i++){
    ob_release(v->array[i]);
  }

  v->length = 0;

  return;
}


/* PRIVATE METHODS */


obvector * obvector_create_default(uint32_t initial_capacity){

  static const char classname[] = "obvector";

  obvector *new_instance = malloc(sizeof(obvector));
  assert(new_instance != NULL);

  /* initialize reference counting base data */
  ob_init_base((obj *)new_instance, &obvector_destroy, &obvector_hash, &obvector_compare,
           &obvector_display, classname);

  /* a vector with zero capacity cannot be created, create one with a capacity
   * of one */
  if(initial_capacity == 0){
    initial_capacity = 1;
  }

  new_instance->array = malloc(initial_capacity*sizeof(obj *));
  assert(new_instance->array != NULL);

  new_instance->capacity = initial_capacity;
  new_instance->length = 0;

  return new_instance;
}


void obvector_resize(obvector *v, uint32_t index){

  uint32_t i;
  uint64_t new_cap;
  obj **array;

  assert(index < UINT32_MAX);

  if(index < v->capacity) return;

  /* find the next power of two capacity that can store the index */
  new_cap = v->capacity;
  while(index+1 > new_cap) new_cap *= 2;
  if(new_cap > UINT32_MAX) new_cap = UINT32_MAX;

  array = malloc(new_cap*sizeof(obj *));

  assert(array != NULL);
  for(i=0; i<v->capacity; i++) array[i] = v->array[i];
  for(i=v->capacity; i<new_cap; i++) array[i] = NULL;

  free(v->array);
  v->array = array;
  v->capacity = new_cap;

  return;
}


obj ** obvector_recursive_sort(obj **to_sort, uint32_t size, int8_t order,
                             ob_compare_fptr funct){

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
  left_sorted = obvector_recursive_sort(to_sort, split, order, funct);
  right_sorted = obvector_recursive_sort(to_sort+split, size-split,
                                       order, funct);

  sorted = malloc(sizeof(obj *)*size);
  assert(sorted != NULL);

  /* merge sorted halves */
  i=0;
  j=0;

  /* while there are elements in both halves to be added to sorted */
  while(i < split && j < (size-split)){

    /* if the comparison of the left to the right matches the desired order,
     * then the next item to go in the sorted array is from the left. Ensure
     * that NULL is left at the tail of the sorted array as well */
    if(funct(left_sorted[i], right_sorted[j]) == order ||
        right_sorted[j] == NULL){
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


ob_hash_t obvector_hash(const obj *to_hash){

  static int8_t init = 0;
  static ob_hash_t seed;

  uint32_t i;
  ob_hash_t value;
  obvector *instance = (obvector *)to_hash;

  assert(to_hash);
  assert(ob_has_class(to_hash, "obvector"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;
  for(i=0; i<instance->length; i++){
    value += ob_hash(instance->array[i]);
    value += value << 10;
    value ^= value >> 6;
  }

  value += value << 3;
  value ^= value >> 11;
  value += value << 15;

  return value;
}


int8_t obvector_compare(const obj *a, const obj *b){

  uint32_t i;
  const obvector *comp_a = (obvector *)a;
  const obvector *comp_b = (obvector *)b;

  assert(a);
  assert(b);
  assert(ob_has_class(a, "obvector"));
  assert(ob_has_class(b, "obvector"));

  if(comp_a->length != comp_b->length) return OB_NOT_EQUAL;

  for(i=0; i<comp_a->length; i++)
    if(ob_compare(comp_a->array[i], comp_b->array[i]) != OB_EQUAL_TO)
      return OB_NOT_EQUAL;

  return OB_EQUAL_TO;
}


void obvector_display(const obj *to_print){

  uint32_t i;
  obvector *v = (obvector *)to_print;

  assert(to_print != NULL);
  assert(ob_has_class(to_print, "obvector"));
  fprintf(stderr, "obvector with %u elements\n", v->length);

  for(i=0; i<v->length; i++){
    fprintf(stderr, "[index: %u]\n", i);
    ob_display(v->array[i]);
    fprintf(stderr, "\n");
  }

  fprintf(stderr, "[vector end]\n");
}



void obvector_destroy(obj *to_dealloc){

  /* cast generic obj to obvector */
  obvector *instance = (obvector *)to_dealloc;

  assert(instance != NULL);
  assert(ob_has_class(to_dealloc, "obvector"));

  obvector_clear(instance); /* ob_release all objs contained in vector */
  free(instance->array);

  return;
}


/* PRIVATE UTILITY METHODS */

uint32_t obvector_find_valid_precursor(obj **array, uint32_t index){
  while(index < UINT32_MAX && !array[index]) index--;
  return index;
}
