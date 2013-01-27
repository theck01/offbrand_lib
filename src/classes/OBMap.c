/**
 * @file OBMap.c
 * @brief OBMap Method Implementation
 * @author theck
 */

#include "../../include/OBMap.h"
#include "../../include/private/OBMap_Private.h"

/* PUBLIC METHODS */

OBMap * createMap(void){
  return createMapWithCapacity(MAP_CAPACITIES[0]);
}


OBMap * createMapWithCapacity(uint32_t capacity){

  OBMap *m = createDefaultMap();

  uint32_t i = 0;
  while(MAP_CAPACITIES[i] < capacity && i < NUM_CAPACITIES - 1) i++;

  m->hash_table = createVector(MAP_CAPACITIES[i]);
  m->pairs = createDeque();
  m->cap_idx = i;
  
  return m;
}


OBMap * copyMap(const OBMap *to_copy){

  OBMap *copy;
  
  assert(to_copy);

  copy = createDefaultMap();
  copy->hash_table = copyVector(to_copy->hash_table);
  copy->pairs = copyDeque(to_copy->pairs);
  copy->cap_idx = to_copy->cap_idx;

  return copy;
}


void rehashMap(OBMap *m){}

/* OBMapPair PRIVATE METHODS */

OBMapPair * createMapPair(obj *key, obj *value){

  static const char classname[] = "OBMapPair";
  OBMapPair *new_instance = malloc(sizeof(OBMapPair));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocMapPair, NULL, NULL, classname);

  retain(key);
  new_instance->key = key;
  retain(value);
  new_instance->value = value;

  return new_instance;
}


void deallocMapPair(obj *to_dealloc){

  /* cast generic obj to OBMap */
  OBMapPair *instance = (OBMapPair *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBMapPair"));

  release((obj *)instance->key);
  release((obj *)instance->value);

  return;
}
  


OBMap * createDefaultMap(void){

  static const char classname[] = "OBMap";
  OBMap *new_instance = malloc(sizeof(OBMap));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocMap, &hashMap,
           &compareMaps, classname);

  new_instance->hash_table = NULL;
  new_instance->pairs = NULL;
  new_instance->cap_idx = 0;
  new_instance->collisions = 0;

  return new_instance;
}


obhash_t hashMap(const obj *to_hash){

  static int8_t init = 0;
  static obhash_t seed = 0;

  obhash_t value;
  OBDequeIterator *it;
  OBMap *instance = (OBMap *)to_hash;

  assert(to_hash);
  assert(objIsOfClass(to_hash, "OBMap"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;

  it = getDequeHeadIt(instance->pairs);
  if(!it) return value;

  /* perform commutative hash, so order of addition to table does not matter */
  do{
    value ^= hash(objAtDequeIt(instance->pairs, it));
  }while(iterateDequeNext(instance->pairs, it));

  release((obj *)it);

  value += value << 3;
  value ^= value >> 11;
  value += value << 15;
  
  return value;
}


int8_t compareMaps(const obj *a, const obj *b){
  
  assert(a);
  assert(b);
  assert(objIsOfClass(a, "OBMap"));
  assert(objIsOfClass(b, "OBMap"));

  if(hash(a) == hash(b)) return OB_EQUAL_TO;
  return OB_NOT_EQUAL;
}


void deallocMap(obj *to_dealloc){

  /* cast generic obj to OBMap */
  OBMap *instance = (OBMap *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBMap"));

  release((obj *)instance->hash_table);
  release((obj *)instance->pairs);

  return;
}


void increaseMapSize(OBMap *to_size){

  assert(to_size);
  assert(to_size->cap_idx < NUM_CAPACITIES);

  to_size->cap_idx++;
  to_size->collisions = 0; /* before resize reset collisions */

  rehashMap(to_size);
}


void addToHashTable(OBMap *m, OBDequeIterator *it){

  obhash_t hash_value, offset;
  OBMapPair *pair;

  assert(m);
  assert(it);

  pair = (OBMapPair *)objAtDequeIt(m->pairs, it);
  hash_value = hash(pair->key)%MAP_CAPACITIES[m->cap_idx];
  offset = 0;

  while(objAtVectorIndex(m->hash_table, hash_value)){
    offset = collisionOffset(offset);
    hash_value = (hash_value+offset)%MAP_CAPACITIES[m->cap_idx];
    m->collisions++;
  }

  storeAtVectorIndex(m->hash_table, (obj *)it, hash_value);
}


OBDequeIterator * findKeyInHashTable(const OBMap *m, const obj *key){
  
  obhash_t hash_value, offset;
  OBMapPair *mp;
  OBDequeIterator *it;

  hash_value = hash(key);
  offset = 0;
  
  while((it = (OBDequeIterator *)objAtVectorIndex(m->hash_table, hash_value))){
    mp = (OBMapPair *)objAtDequeIt(m->pairs, it);
    if(compare(mp->key, key) == OB_EQUAL_TO)
      break;
    offset = collisionOffset(offset);
    hash_value = (hash_value+offset)%MAP_CAPACITIES[m->cap_idx];
  }

  return it; /* either a valid OBDequeIterator or NULL, depending on whether
                an item was found */
}


obhash_t collisionOffset(obhash_t prev_offset){
  if(prev_offset == 0) return 1;
  else if(prev_offset == 1) return 2;
  else return prev_offset*prev_offset - 1;
}


