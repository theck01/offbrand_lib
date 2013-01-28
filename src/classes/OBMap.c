/**
 * @file OBMap.c
 * @brief OBMap Method Implementation
 * @author theck
 */

#include "../../include/OBMap.h"
#include "../../include/private/OBMap_Private.h"

/* PRIVATE OBMap CONSTANT VALUES */

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
  ((uint64_t)1<<32)-5
};

const uint32_t NUM_CAPACITIES = 
                              sizeof(MAP_CAPACITIES)/sizeof(MAP_CAPACITIES[0]);

const double MAX_LOAD_FACTOR = 0.75;


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
  OBMapPair *mp;
  OBDequeIterator *it;
  
  assert(to_copy);

  copy = createDefaultMap();

  copy->cap_idx = to_copy->cap_idx;
  copy->collisions = to_copy->collisions;
  copy->pairs = createDeque();

  /* copy deque manually, internal objects need to be copied as well as Deque
   * itself */
  it = getDequeHeadIt(to_copy->pairs);
  if(it){
    do{
       mp = copyMapPair((OBMapPair *)objAtDequeIt(to_copy->pairs, it));
       addDequeTail(copy->pairs, (obj *)mp);
    }while(iterateDequeNext(to_copy->pairs, it));
  }

  rehashMap(copy); /* cannot grab to_copy->hash_table directly, stored iterators
                      must point to values within copy, not to copy */

  return copy;
}


void addToMap(OBMap *m, obj *key, obj *value){

  OBMapPair *mp;
  OBDequeIterator *it;
  obhash_t hash_value;

  assert(m);
  
  hash_value = findKeyInHashTable(m, key);
  it = (OBDequeIterator *)objAtVectorIndex(m->hash_table, hash_value);

  /* If the key already exists in the map then overwrite the existing
   * value */
  if(it){
    mp = (OBMapPair *)objAtDequeIt(m->pairs, it);
    replaceMapPairValue(mp, value);
    return;
  }

  /* if add operation will overload the map then resize the map */
  if((dequeLength(m->pairs)+1)/MAP_CAPACITIES[m->cap_idx] > MAX_LOAD_FACTOR)
    increaseMapSize(m);

  mp = createMapPair(key, value);
  addDequeTail(m->pairs, (obj *)mp);
  release((obj *)mp); /* map deque has only reference to mp */

  assert(it = getDequeTailIt(m->pairs));
  addToHashTable(m, it);
  release((obj *)it); /* map vector hash only reference to it */
  
  return; 
}


obj * lookupMapKey(const OBMap *m, const obj *key){

  OBDequeIterator *it;
  OBMapPair *mp;
  obhash_t hash_value;

  assert(m);

  hash_value = findKeyInHashTable(m, key);
  it = (OBDequeIterator *)objAtVectorIndex(m->hash_table, hash_value);

  if(!it) return NULL;

  mp = (OBMapPair *)objAtDequeIt(m->pairs, it);
  return mp->value;
}

  
void removeMapKey(OBMap *m, obj *key){

  OBDequeIterator *it;
  obhash_t hash_value;

  assert(m);

  hash_value = findKeyInHashTable(m, key);
  it = (OBDequeIterator *)objAtVectorIndex(m->hash_table, hash_value);

  if(!it) return;

  removeDequeAtIt(m->pairs, it);
  rehashMap(m);

  return;
}
  

void rehashMap(OBMap *m){

  OBDequeIterator *it, *it_copy;

  assert(m);

  release((obj *)m->hash_table);
  m->hash_table = createVector(MAP_CAPACITIES[m->cap_idx]);

  it = getDequeHeadIt(m->pairs);
  if(!it) return;

  do{
    it_copy = copyDequeIterator(it);
    addToHashTable(m, it_copy);
    release((obj *)it_copy);
  }while(iterateDequeNext(m->pairs, it));

  return;
}


void clearMap(OBMap *m){

  assert(m);

  release((obj *)m->hash_table);
  release((obj *)m->pairs);

  m->hash_table = createVector(MAP_CAPACITIES[m->cap_idx]);
  m->pairs = createDeque();
}


/* OBMapPair PRIVATE METHODS */

OBMapPair * createMapPair(obj *key, obj *value){

  static const char classname[] = "OBMapPair";
  OBMapPair *new_instance = malloc(sizeof(OBMapPair));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocMapPair, &hashMapPair, NULL, classname);

  retain(key);
  new_instance->key = key;
  retain(value);
  new_instance->value = value;

  return new_instance;
}


OBMapPair * copyMapPair(OBMapPair *mp){
  assert(mp);
  return createMapPair(mp->key, mp->value);
}


void replaceMapPairValue(OBMapPair *mp, obj *value){

  assert(mp);
  
  retain(value);
  release(mp->value);
  mp->value = value;
  return;
}

obhash_t hashMapPair(const obj *to_hash){

  static int8_t init = 0;
  static obhash_t seed = 0;

  obhash_t value;
  OBMapPair *instance = (OBMapPair *)to_hash;

  assert(to_hash);
  assert(objIsOfClass(to_hash, "OBMapPair"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;

  value += hash(instance->key);
  value += hash(instance->value);

  value += value << 3;
  value ^= value >> 11;
  value += value << 15;

  return value;
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
  


/* OBMap PRIVATE METHODS */

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
    value += hash(objAtDequeIt(instance->pairs, it));
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
  assert(to_size->cap_idx < NUM_CAPACITIES-1); /* cannot resize beyond largest
                                                  capacity */

  to_size->cap_idx+=2;
  if(to_size->cap_idx >= NUM_CAPACITIES) to_size->cap_idx = NUM_CAPACITIES-1;
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


obhash_t findKeyInHashTable(const OBMap *m, const obj *key){
  
  obhash_t hash_value, offset;
  OBMapPair *mp;
  OBDequeIterator *it;

  hash_value = hash(key)%MAP_CAPACITIES[m->cap_idx];
  offset = 0;
  
  while((it = (OBDequeIterator *)objAtVectorIndex(m->hash_table, hash_value))){
    mp = (OBMapPair *)objAtDequeIt(m->pairs, it);
    if(compare(mp->key, key) == OB_EQUAL_TO)
      break;
    offset = collisionOffset(offset);
    hash_value = (hash_value+offset)%MAP_CAPACITIES[m->cap_idx];
  }

  return hash_value; /* the hash value where key is actually found */
}


obhash_t collisionOffset(obhash_t prev_offset){
  if(prev_offset == 0) return 1;
  else if(prev_offset == 1) return 2;
  else return prev_offset*prev_offset - 1;
}


