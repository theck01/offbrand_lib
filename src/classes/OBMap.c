/**
 * @file OBMap.c
 * @brief OBMap Method Implementation
 * @author theck
 */

#include "../../include/OBMap.h"
#include "../../include/private/OBMap_Private.h"

/* PUBLIC METHODS */


/* DEFINE PUBLIC METHODS HERE */


/* OBMapPair PRIVATE METHODS */

OBMap * createMapPair(obj *key, obj *value){

  static const char classname[] = "OBMapPair";
  OBMap *new_instance = malloc(sizeof(OBMapPair));
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
  OBMap *instance = (OBMapPair *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBMapPair"));

  release((obj *)instance->key);
  release((obj *)instance->value);

  return;
}
  


OBMap * createDefaultMap(uint8_t cap_idx){

  static const char classname[] = "OBMap";
  OBMap *new_instance = malloc(sizeof(OBMap));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocMap, &hashMap,
           &compareMaps, classname);

  new_instance->hash_table = createVector(MAP_CAPACITIES[cap_idx]);
  new_instance->pairs = createDeque();
  new_instance->cap_idx = cap_idx;
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

