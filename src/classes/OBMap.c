/**
 * @file obmap.c
 * @brief obmap Method Implementation
 * @author theck
 */

#include "../../include/obmap.h"
#include "../../include/private/obmap_private.h"

/* PRIVATE obmap CONSTANT VALUES */

/* capacity table, where capacities are the nearest prime numbers less than
 * powers of 2 */
const uint32_t MAP_CAPACITIES[] = {
  (1<<7)-1,
  (1<<8)-5,
  (1<<9)-3,
  (1<<10)-3,
  (1<<11)-9,
  (1<<12)-3,
  (1<<13)-1,
  (1<<14)-3,
  (1<<15)-19,
  (1<<16)-15,
  (1<<17)-1,
  (1<<18)-5,
  (1<<19)-1,
  (1<<20)-3,
  (1<<21)-9,
  (1<<22)-3,
  (1<<23)-15,
  (1<<24)-3,
  (1<<25)-39,
  (1<<26)-5,
  (1<<27)-39,
  (1<<28)-57,
  (1<<29)-3,
  (1<<30)-35,
  ((uint64_t)1<<31)-1,
  (((uint64_t)1)<<32)-5
};

const uint32_t NUM_CAPACITIES =
                              sizeof(MAP_CAPACITIES)/sizeof(MAP_CAPACITIES[0]);

const double MAX_LOAD_FACTOR = 0.75;


/* PUBLIC METHODS */

obmap * obmap_new(void){
  return obmap_new_with_capacity(MAP_CAPACITIES[0]);
}


obmap * obmap_new_with_capacity(uint32_t capacity){

  obmap *m = obmap_create_default();

  uint32_t i = 0;
  while(MAP_CAPACITIES[i] < capacity && i < NUM_CAPACITIES - 1) i++;

  m->hash_table = obvector_new(MAP_CAPACITIES[i]);
  m->pairs = obdeque_new();
  m->cap_idx = i;

  return m;
}


obmap * obmap_copy(const obmap *to_copy){

  obmap *copy;
  obmap_pair *mp;
  obdeque_iterator *it;

  assert(to_copy);

  copy = obmap_create_default();

  copy->cap_idx = to_copy->cap_idx;
  copy->collisions = to_copy->collisions;
  copy->pairs = obdeque_new();

  /* copy deque manually, internal objects need to be copied as well as Deque
   * itself */
  it = obdeque_head_iterator(to_copy->pairs);
  if(it){
    do{
       mp = obmap_copy_pair((obmap_pair *)obdeque_obj_at_iterator(to_copy->pairs, it));
       obdeque_add_at_tail(copy->pairs, (obj *)mp);
       ob_release((obj *)mp);
    }while(obdeque_iterate_next(to_copy->pairs, it));
  }
  ob_release((obj *)it);

  obmap_rehash(copy); /* cannot grab to_copy->hash_table directly, stored
                         iterators must point to values within copy, not to copy */

  return copy;
}


void obmap_insert(obmap *m, obj *key, obj *value){

  obmap_pair *mp;
  obdeque_iterator *it;
  ob_hash_t hash_value;

  assert(m);

  hash_value = obmap_find_key(m, key);
  it = (obdeque_iterator *)obvector_obj_at_index(m->hash_table, hash_value);

  /* If the key already exists in the map then overwrite the existing
   * value */
  if(it){
    mp = (obmap_pair *)obdeque_obj_at_iterator(m->pairs, it);
    obmap_replace_pair_value(mp, value);
    return;
  }

  /* if add operation will overload the map then resize the map */
  if((obdeque_length(m->pairs)+1)/MAP_CAPACITIES[m->cap_idx] > MAX_LOAD_FACTOR)
    obmap_increase_size(m);

  mp = obmap_new_pair(key, value);
  obdeque_add_at_tail(m->pairs, (obj *)mp);
  ob_release((obj *)mp); /* map deque has only reference to mp */

  assert(it = obdeque_tail_iterator(m->pairs));
  obmap_add_to_table(m, it);
  ob_release((obj *)it); /* map vector hash only reference to it */

  return;
}


obj * obmap_lookup(const obmap *m, const obj *key){

  obdeque_iterator *it;
  obmap_pair *mp;
  ob_hash_t hash_value;

  assert(m);

  hash_value = obmap_find_key(m, key);
  it = (obdeque_iterator *)obvector_obj_at_index(m->hash_table, hash_value);

  if(!it) return NULL;

  mp = (obmap_pair *)obdeque_obj_at_iterator(m->pairs, it);
  return mp->value;
}


void obmap_remove(obmap *m, obj *key){

  obdeque_iterator *it;
  ob_hash_t hash_value;

  assert(m);

  hash_value = obmap_find_key(m, key);
  it = (obdeque_iterator *)obvector_obj_at_index(m->hash_table, hash_value);

  if(!it) return;

  obdeque_remove_at_iterator(m->pairs, it);
  obmap_rehash(m);

  return;
}


void obmap_rehash(obmap *m){

  obdeque_iterator *it, *it_copy;

  assert(m);

  ob_release((obj *)m->hash_table);
  m->hash_table = obvector_new(MAP_CAPACITIES[m->cap_idx]);

  it = obdeque_head_iterator(m->pairs);
  if(!it) return;

  do{
    it_copy = obdeque_copy_iterator(it);
    obmap_add_to_table(m, it_copy);
    ob_release((obj *)it_copy);
  }while(obdeque_iterate_next(m->pairs, it));

  ob_release((obj *)it);

  return;
}


void obmap_clear(obmap *m){

  assert(m);

  ob_release((obj *)m->hash_table);
  ob_release((obj *)m->pairs);

  m->hash_table = obvector_new(MAP_CAPACITIES[m->cap_idx]);
  m->pairs = obdeque_new();
}


/* obmap_pair PRIVATE METHODS */

obmap_pair * obmap_new_pair(obj *key, obj *value){

  static const char classname[] = "obmap_pair";
  obmap_pair *new_instance = malloc(sizeof(obmap_pair));
  assert(new_instance != NULL);

  /* initialize base class data */
  ob_init_base((obj *)new_instance, &obmap_destroy_pair, &obmap_hash_pair, NULL,
           &obmap_display_pair, classname);

  ob_retain(key);
  new_instance->key = key;
  ob_retain(value);
  new_instance->value = value;

  return new_instance;
}


obmap_pair * obmap_copy_pair(obmap_pair *mp){
  assert(mp);
  return obmap_new_pair(mp->key, mp->value);
}


void obmap_replace_pair_value(obmap_pair *mp, obj *value){

  assert(mp);

  ob_retain(value);
  ob_release(mp->value);
  mp->value = value;
  return;
}

ob_hash_t obmap_hash_pair(const obj *to_hash){

  static int8_t init = 0;
  static ob_hash_t seed = 0;

  ob_hash_t value;
  obmap_pair *instance = (obmap_pair *)to_hash;

  assert(to_hash);
  assert(ob_has_class(to_hash, "obmap_pair"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;

  value += ob_hash(instance->key);
  value += ob_hash(instance->value);

  value += value << 3;
  value ^= value >> 11;
  value += value << 15;

  return value;
}

void obmap_display_pair(const obj *to_print){

  obmap_pair *mp = (obmap_pair *)to_print;

  assert(to_print != NULL);
  assert(ob_has_class(to_print, "obmap_pair"));

  fprintf(stderr, "  [key]\n");
  ob_display(mp->key);
  fprintf(stderr, "  [value]\n");
  ob_display(mp->value);
  return;
}


void obmap_destroy_pair(obj *to_dealloc){

  /* cast generic obj to obmap */
  obmap_pair *instance = (obmap_pair *)to_dealloc;

  assert(to_dealloc);
  assert(ob_has_class(to_dealloc, "obmap_pair"));

  ob_release((obj *)instance->key);
  ob_release((obj *)instance->value);

  return;
}



/* obmap PRIVATE METHODS */

obmap * obmap_create_default(void){

  static const char classname[] = "obmap";
  obmap *new_instance = malloc(sizeof(obmap));
  assert(new_instance != NULL);

  /* initialize base class data */
  ob_init_base((obj *)new_instance, &obmap_destroy, &obmap_hash,
           &obmap_compare, &obmap_display, classname);

  new_instance->hash_table = NULL;
  new_instance->pairs = NULL;
  new_instance->cap_idx = 0;
  new_instance->collisions = 0;

  return new_instance;
}


ob_hash_t obmap_hash(const obj *to_hash){

  static int8_t init = 0;
  static ob_hash_t seed = 0;

  ob_hash_t value;
  obdeque_iterator *it;
  obmap *instance = (obmap *)to_hash;

  assert(to_hash);
  assert(ob_has_class(to_hash, "obmap"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;

  it = obdeque_head_iterator(instance->pairs);
  if(!it) return value;

  /* perform commutative hash, so order of addition to table does not matter */
  do{
    value += ob_hash(obdeque_obj_at_iterator(instance->pairs, it));
  }while(obdeque_iterate_next(instance->pairs, it));

  ob_release((obj *)it);

  value += value << 3;
  value ^= value >> 11;
  value += value << 15;

  return value;
}


int8_t obmap_compare(const obj *a, const obj *b){

  assert(a);
  assert(b);
  assert(ob_has_class(a, "obmap"));
  assert(ob_has_class(b, "obmap"));

  if(ob_hash(a) == ob_hash(b)) return OB_EQUAL_TO;
  return OB_NOT_EQUAL;
}


void obmap_display(const obj *to_print){

  obmap *m = (obmap *)to_print;
  obdeque_iterator *it;

  assert(to_print != NULL);
  assert(ob_has_class(to_print, "obmap"));
  fprintf(stderr, "obmap with key-value pairs:\n");

  it = obdeque_head_iterator(m->pairs);

  if(!it) return;

  do{
    ob_display(obdeque_obj_at_iterator(m->pairs, it));
  }while(obdeque_iterate_next(m->pairs, it));

  fprintf(stderr, "  [map end]\n");

  ob_release((obj *)it);

  return;
}


void obmap_destroy(obj *to_dealloc){

  /* cast generic obj to obmap */
  obmap *instance = (obmap *)to_dealloc;

  assert(to_dealloc);
  assert(ob_has_class(to_dealloc, "obmap"));

  ob_release((obj *)instance->hash_table);
  ob_release((obj *)instance->pairs);

  return;
}


void obmap_increase_size(obmap *to_size){

  assert(to_size);
  assert(to_size->cap_idx < NUM_CAPACITIES-1); /* cannot resize beyond largest
                                                  capacity */

  to_size->cap_idx+=2;
  if(to_size->cap_idx >= NUM_CAPACITIES) to_size->cap_idx = NUM_CAPACITIES-1;
  to_size->collisions = 0; /* before resize reset collisions */

  obmap_rehash(to_size);
}


void obmap_add_to_table(obmap *m, obdeque_iterator *it){

  ob_hash_t hash_value, offset;
  obmap_pair *pair;

  assert(m);
  assert(it);

  pair = (obmap_pair *)obdeque_obj_at_iterator(m->pairs, it);
  hash_value = ob_hash(pair->key)%MAP_CAPACITIES[m->cap_idx];
  offset = 0;

  while(obvector_obj_at_index(m->hash_table, hash_value)){
    offset = obmap_offset_collision(offset);
    hash_value = (hash_value+offset)%MAP_CAPACITIES[m->cap_idx];
    m->collisions++;
  }

  obvector_store_at_index(m->hash_table, (obj *)it, hash_value);
}


ob_hash_t obmap_find_key(const obmap *m, const obj *key){

  ob_hash_t hash_value, offset;
  obmap_pair *mp;
  obdeque_iterator *it;

  hash_value = ob_hash(key)%MAP_CAPACITIES[m->cap_idx];
  offset = 0;

  while((it = (obdeque_iterator *)obvector_obj_at_index(m->hash_table, hash_value))){
    mp = (obmap_pair *)obdeque_obj_at_iterator(m->pairs, it);
    if(ob_compare(mp->key, key) == OB_EQUAL_TO)
      break;
    offset = obmap_offset_collision(offset);
    hash_value = (hash_value+offset)%MAP_CAPACITIES[m->cap_idx];
  }

  return hash_value; /* the hash value where key is actually found */
}


ob_hash_t obmap_offset_collision(ob_hash_t prev_offset){
  if(prev_offset == 0) return 1;
  else if(prev_offset == 1) return 2;
  else return prev_offset*prev_offset - 1;
}

