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
  (((uint64_t)1)<<32)-5
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
  m->pairs = OBDequeCreate();
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
  copy->pairs = OBDequeCreate();

  /* copy deque manually, internal objects need to be copied as well as Deque
   * itself */
  it = OBDequeGetHeadIterator(to_copy->pairs);
  if(it){
    do{
       mp = copyMapPair((OBMapPair *)OBDequeGetObjectAtIterator(to_copy->pairs, it));
       OBDequeAddTail(copy->pairs, (OBObjType *)mp);
       OBRelease((OBObjType *)mp);
    }while(OBDequeIterateNext(to_copy->pairs, it));
  }
  OBRelease((OBObjType *)it);

  rehashMap(copy); /* cannot grab to_copy->hash_table directly, stored iterators
                      must point to values within copy, not to copy */

  return copy;
}


void addToMap(OBMap *m, OBObjType *key, OBObjType *value){

  OBMapPair *mp;
  OBDequeIterator *it;
  obhash_t hash_value;

  assert(m);
  
  hash_value = findKeyInHashTable(m, key);
  it = (OBDequeIterator *)objAtVectorIndex(m->hash_table, hash_value);

  /* If the key already exists in the map then overwrite the existing
   * value */
  if(it){
    mp = (OBMapPair *)OBDequeGetObjectAtIterator(m->pairs, it);
    replaceMapPairValue(mp, value);
    return;
  }

  /* if add operation will overload the map then resize the map */
  if((OBDequeGetLength(m->pairs)+1)/MAP_CAPACITIES[m->cap_idx] > MAX_LOAD_FACTOR)
    increaseMapSize(m);

  mp = createMapPair(key, value);
  OBDequeAddTail(m->pairs, (OBObjType *)mp);
  OBRelease((OBObjType *)mp); /* map deque has only reference to mp */

  assert(it = OBDequeGetTailIterator(m->pairs));
  addToHashTable(m, it);
  OBRelease((OBObjType *)it); /* map vector hash only reference to it */
  
  return; 
}


OBObjType * lookupMapKey(const OBMap *m, const OBObjType *key){

  OBDequeIterator *it;
  OBMapPair *mp;
  obhash_t hash_value;

  assert(m);

  hash_value = findKeyInHashTable(m, key);
  it = (OBDequeIterator *)objAtVectorIndex(m->hash_table, hash_value);

  if(!it) return NULL;

  mp = (OBMapPair *)OBDequeGetObjectAtIterator(m->pairs, it);
  return mp->value;
}

  
void removeMapKey(OBMap *m, OBObjType *key){

  OBDequeIterator *it;
  obhash_t hash_value;

  assert(m);

  hash_value = findKeyInHashTable(m, key);
  it = (OBDequeIterator *)objAtVectorIndex(m->hash_table, hash_value);

  if(!it) return;

  OBDequeRemoveObjectAtIterator(m->pairs, it);
  rehashMap(m);

  return;
}
  

void rehashMap(OBMap *m){

  OBDequeIterator *it, *it_copy;

  assert(m);

  OBRelease((OBObjType *)m->hash_table);
  m->hash_table = createVector(MAP_CAPACITIES[m->cap_idx]);

  it = OBDequeGetHeadIterator(m->pairs);
  if(!it) return;

  do{
    it_copy = OBDequeIteratorCopy(it);
    addToHashTable(m, it_copy);
    OBRelease((OBObjType *)it_copy);
  }while(OBDequeIterateNext(m->pairs, it));

  OBRelease((OBObjType *)it);

  return;
}


void clearMap(OBMap *m){

  assert(m);

  OBRelease((OBObjType *)m->hash_table);
  OBRelease((OBObjType *)m->pairs);

  m->hash_table = createVector(MAP_CAPACITIES[m->cap_idx]);
  m->pairs = OBDequeCreate();
}


/* OBMapPair PRIVATE METHODS */

OBMapPair * createMapPair(OBObjType *key, OBObjType *value){

  static const char classname[] = "OBMapPair";
  OBMapPair *new_instance = malloc(sizeof(OBMapPair));
  assert(new_instance != NULL);

  /* initialize base class data */
  OBInitBase((OBObjType *)new_instance, &deallocMapPair, &hashMapPair, NULL, 
           &displayMapPair, classname);

  OBRetain(key);
  new_instance->key = key;
  OBRetain(value);
  new_instance->value = value;

  return new_instance;
}


OBMapPair * copyMapPair(OBMapPair *mp){
  assert(mp);
  return createMapPair(mp->key, mp->value);
}


void replaceMapPairValue(OBMapPair *mp, OBObjType *value){

  assert(mp);
  
  OBRetain(value);
  OBRelease(mp->value);
  mp->value = value;
  return;
}

obhash_t hashMapPair(OBTypeRef to_hash){

  static int8_t init = 0;
  static obhash_t seed = 0;

  obhash_t value;
  OBMapPair *instance = (OBMapPair *)to_hash;

  assert(to_hash);
  assert(OBObjIsOfClass(to_hash, "OBMapPair"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;

  value += OBHash(instance->key);
  value += OBHash(instance->value);

  value += value << 3;
  value ^= value >> 11;
  value += value << 15;

  return value;
}

void displayMapPair(OBTypeRef to_print){

  OBMapPair *mp = (OBMapPair *)to_print;

  assert(to_print != NULL);
  assert(OBObjIsOfClass(to_print, "OBMapPair"));
  
  fprintf(stderr, "  [key]\n");
  OBDisplay(mp->key);
  fprintf(stderr, "  [value]\n");
  OBDisplay(mp->value);
  return;
}


void deallocMapPair(OBTypeRef to_dealloc){

  /* cast generic obj to OBMap */
  OBMapPair *instance = (OBMapPair *)to_dealloc;

  assert(to_dealloc);
  assert(OBObjIsOfClass(to_dealloc, "OBMapPair"));

  OBRelease((OBObjType *)instance->key);
  OBRelease((OBObjType *)instance->value);

  return;
}
  


/* OBMap PRIVATE METHODS */

OBMap * createDefaultMap(void){

  static const char classname[] = "OBMap";
  OBMap *new_instance = malloc(sizeof(OBMap));
  assert(new_instance != NULL);

  /* initialize base class data */
  OBInitBase(new_instance, &deallocMap, &hashMap,
           &compareMaps, &displayMap, classname);

  new_instance->hash_table = NULL;
  new_instance->pairs = NULL;
  new_instance->cap_idx = 0;
  new_instance->collisions = 0;

  return new_instance;
}


obhash_t hashMap(OBTypeRef to_hash){

  static int8_t init = 0;
  static obhash_t seed = 0;

  obhash_t value;
  OBDequeIterator *it;
  OBMap *instance = (OBMap *)to_hash;

  assert(to_hash);
  assert(OBObjIsOfClass(to_hash, "OBMap"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;

  it = OBDequeGetHeadIterator(instance->pairs);
  if(!it) return value;

  /* perform commutative hash, so order of addition to table does not matter */
  do{
    value += OBHash(OBDequeGetObjectAtIterator(instance->pairs, it));
  }while(OBDequeIterateNext(instance->pairs, it));

  OBRelease((OBObjType *)it);

  value += value << 3;
  value ^= value >> 11;
  value += value << 15;
  
  return value;
}


int8_t compareMaps(OBTypeRef a, OBTypeRef b){
  
  assert(a);
  assert(b);
  assert(OBObjIsOfClass(a, "OBMap"));
  assert(OBObjIsOfClass(b, "OBMap"));

  if(OBHash(a) == OBHash(b)) return OB_EQUAL_TO;
  return OB_NOT_EQUAL;
}


void displayMap(OBTypeRef to_print){
  
  OBMap *m = (OBMap *)to_print;
  OBDequeIterator *it;

  assert(to_print != NULL);
  assert(OBObjIsOfClass(to_print, "OBMap"));
  fprintf(stderr, "OBMap with key-value pairs:\n");

  it = OBDequeGetHeadIterator(m->pairs);

  if(!it) return;

  do{
    OBDisplay(OBDequeGetObjectAtIterator(m->pairs, it));
  }while(OBDequeIterateNext(m->pairs, it));

  fprintf(stderr, "  [map end]\n");

  OBRelease((OBObjType *)it);

  return;
}


void deallocMap(OBTypeRef to_dealloc){

  /* cast generic obj to OBMap */
  OBMap *instance = (OBMap *)to_dealloc;

  assert(to_dealloc);
  assert(OBObjIsOfClass(to_dealloc, "OBMap"));

  OBRelease((OBObjType *)instance->hash_table);
  OBRelease((OBObjType *)instance->pairs);

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

  pair = (OBMapPair *)OBDequeGetObjectAtIterator(m->pairs, it);
  hash_value = OBHash(pair->key)%MAP_CAPACITIES[m->cap_idx];
  offset = 0;

  while(objAtVectorIndex(m->hash_table, hash_value)){
    offset = collisionOffset(offset);
    hash_value = (hash_value+offset)%MAP_CAPACITIES[m->cap_idx];
    m->collisions++;
  }

  storeAtVectorIndex(m->hash_table, (OBObjType *)it, hash_value);
}


obhash_t findKeyInHashTable(const OBMap *m, const OBObjType *key){
  
  obhash_t hash_value, offset;
  OBMapPair *mp;
  OBDequeIterator *it;

  hash_value = OBHash(key)%MAP_CAPACITIES[m->cap_idx];
  offset = 0;
  
  while((it = (OBDequeIterator *)objAtVectorIndex(m->hash_table, hash_value))){
    mp = (OBMapPair *)OBDequeGetObjectAtIterator(m->pairs, it);
    if(OBCompare(mp->key, key) == OB_EQUAL_TO)
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

