/**
 * @file OBMap_Private.h
 * @brief OBMap Private Interface
 * @author theck
 */

#ifndef OBMAP_PRIVATE_H
#define OBMAP_PRIVATE_H

#include "../OBMap.h"
#include "../OBVector.h"
#include "../OBDeque.h"

/* OBMAP CONSTANTS */

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


/* OBMapPair DATA */

/**
 * @brief OBMap pair internal structure, encapsulates a key-value pair for the
 * given map
 */
typedef struct OBMapPair_struct{
  obj base; /**< obj containing reference count and class membership data */
  obj *key; /**< obj pointer to the key used to lookup within the hash */
  obj *value; /**< obj pointer to the value stored in the hash */
} OBMapPair;

/* OBMapPair PRIVATE METHODS */

/**
 * @brief Default constructor for an OBMapPair
 * @return An instance of class OBMapPair
 * @warning All public constructors should call this constructor and intialize
 * individual members as needed, so that all base data is initialized properly.
 */
OBMapPair * createMapPair(obj *key, obj *value);

/** 
 * @brief Destructor for OBMapPair
 *
 * @param to_dealloc An obj pointer to an instance of OBMap with
 * reference count of 0
 *
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void deallocMapPair(obj *to_dealloc); 


/* OBMap DATA */

/**
 * @brief OBMap internal structure, encapsulating all data needed for
 * an instance of OBMap
 */
struct OBMap_struct{
  obj base; /**< obj containing reference count and class membership data */
  uint8_t cap_idx; /**< index within MAP_CAPACITIES used as lookup for
                     capacity of map */
  OBVector *hash_table; /**< map lookup table */
  OBDeque *pairs; /**< A dense list of all MapPairs within the map, for speedy
                    rehash */
  uint32_t collisions; /**< variable that tracks the number of hashing
                         colisions encountered when adding keys to the table */
};

/* OBMap PRIVATE METHODS */

/**
 * @brief Default constructor for OBMap
 *
 * @param cap_idx Lookup value for capacity within MAP_CAPACITIES table
 *
 * @return An instance of class OBMap
 *
 * @warning All public constructors should call this constructor and intialize
 * individual members as needed, so that all base data is initialized properly.
 */
OBMap * createDefaultMap(uint8_t cap_idx);

/**
 * @brief Hash function for OBMap
 *
 * @param to_hash An obj pointer to an instance of OBMap
 *
 * @return Key value (hash) for the given obj pointer to a OBMap
 */
obhash_t hashMap(const obj *to_hash);

/**
 * @brief Compares two instances of OBMap
 *
 * @param a A non-NULL obj pointer to type OBMap
 * @param b A non-NULL obj pointer to type OBMap
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t compareMaps(const obj *a, const obj *b);
/* Arguments are obj * so that a function pointer can be used for container
 * class sorting/search */

/** 
 * @brief Destructor for OBMap
 *
 * @param to_dealloc An obj pointer to an instance of OBMap with
 * reference count of 0
 *
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void deallocMap(obj *to_dealloc);

/**
 * @brief Increases the size of the map to the next capacity within 
 * MAP_CAPACITIES array
 *
 * @param to_size OBMap to resize
 */
void increaseMapSize(OBMap *to_size);

/**
 * @brief Adds an OBDequeIterator to the proper location within the OBMap
 *
 * @param m The OBMap to add the iterator to
 * @param it The OBDequeIterator to add to the hash table
 */
void addToHashTable(OBMap *m, OBDequeIterator *it);

/**
 * @brief Finds a key within the hash table, it exists
 *
 * @param m The OBMap in which to search for the key
 * @param key The key to serach for within the OBMap
 *
 * retval NULL Key was not found in the OBMap
 * retavl non-NULL Pointer to and OBDequeIterator, pinpointing key-value
 * pair in pairs list
 */
OBDequeIterator * findKeyInHashTable(OBMap *m, obj *key);

/**
 * @brief Generates an offset from the hash value to rectify collisions
 *
 * @param prev_offset Last offset provided (0 if initiating call for the first
 * time)
 *
 * @return New offset to the next possible location to insert within table
 */
uint32_t collisionOffset(uint32_t prev_offset);

#endif

