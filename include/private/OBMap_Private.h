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

/* OBMap PRIVATE CONSTANTS */

/**
 * Map capacity table, where capacities are the nearest prime numbers less than
 * powers of 2.
 */
extern const uint32_t MAP_CAPACITIES[];
/**
 * Number of elements in the map capacity table.
 */
extern const uint32_t NUM_CAPACITIES;
/**
 * Maximum load factor of an OBMap before the map will be resized.
 */
extern const double MAX_LOAD_FACTOR;


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
 *
 * @param key Offbrand compatible class instance used to lookup value within
 * hash table
 * @param value Offbrand compatible class stored within the OBMap at a position
 * denoted by key
 *
 * @return An instance of class OBMapPair
 *
 * @warning All public constructors should call this constructor and intialize
 * individual members as needed, so that all base data is initialized properly.
 */
OBMapPair * createMapPair(obj *key, obj *value);

/**
 * @brief Copy constructor, creates a new OBMapPair with the same key-value of
 * an existing OBMapPair
 *
 * @param mp The OBMap instance to copy
 * 
 * @return An instance of class OBMapPair that contains the same key-value as
 * mp
 */
OBMapPair * copyMapPair(OBMapPair *mp);

/**
 * @brief Replaces existing value in an OBMapPair with the supplied value
 * 
 * @param mp An instance of OBMapPair
 * @param value The new value to replace the existing value
 */
void replaceMapPairValue(OBMapPair *mp, obj *value);

/**
 * @brief Hash function for OBMapPair
 *
 * @param to_hash An obj pointer to an instance of OBMapPair
 *
 * @return Key value (hash) for the given obj pointer to a OBMapPair
 */
obhash_t hashMapPair(const obj *to_hash);

/**
 * @brief Displays an instance of OBMapPair to stderr
 *
 * @param to_print A non-NULL obj pointer to type OBMapPair
 */
void displayMapPair(const obj *to_print);

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
 * @return An instance of class OBMap
 *
 * @warning All public constructors should call this constructor and intialize
 * individual members as needed, so that all base data is initialized properly.
 */
OBMap * createDefaultMap(void);

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
 * @brief Displays an instance of OBMap to stderr
 *
 * @param to_print A non-NULL obj pointer to type OBMap
 */
void displayMap(const obj *to_print);

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
 * @return Index in the hash_table where a result can be found or where a NULL
 * value resides if key was not found
 */
obhash_t findKeyInHashTable(const OBMap *m, const obj *key);

/**
 * @brief Generates an offset from the hash value to rectify collisions
 *
 * @param prev_offset Last offset provided (0 if initiating call for the first
 * time)
 *
 * @return New offset to the next possible location to insert within table
 */
obhash_t collisionOffset(obhash_t prev_offset);

#endif

