/**
 * @file obmap_private.h
 * @brief obmap Private Interface
 * @author theck
 */

#ifndef OBMAP_PRIVATE_H
#define OBMAP_PRIVATE_H

#include "../obmap.h"
#include "../obvector.h"
#include "../obdeque.h"

/* obmap PRIVATE CONSTANTS */

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
 * Maximum load factor of an obmap before the map will be resized.
 */
extern const double MAX_LOAD_FACTOR;


/* obmap_pair DATA */

/**
 * @brief obmap pair internal structure, encapsulates a key-value pair for the
 * given map
 */
typedef struct obmap_pair_struct{
  obj base; /**< obj containing reference count and class membership data */
  obj *key; /**< obj pointer to the key used to lookup within the hash */
  obj *value; /**< obj pointer to the value stored in the hash */
} obmap_pair;

/* obmap_pair PRIVATE METHODS */

/**
 * @brief Default constructor for an obmap_pair
 *
 * @param key Offbrand compatible class instance used to lookup value within
 * hash table
 * @param value Offbrand compatible class stored within the obmap at a position
 * denoted by key
 *
 * @return An instance of class obmap_pair
 *
 * @warning All public constructors should call this constructor and intialize
 * individual members as needed, so that all base data is initialized properly.
 */
obmap_pair * obmap_new_pair(obj *key, obj *value);

/**
 * @brief Copy constructor, creates a new obmap_pair with the same key-value of
 * an existing obmap_pair
 *
 * @param mp The obmap instance to copy
 *
 * @return An instance of class obmap_pair that contains the same key-value as
 * mp
 */
obmap_pair * obmap_copy_pair(obmap_pair *mp);

/**
 * @brief Replaces existing value in an obmap_pair with the supplied value
 *
 * @param mp An instance of obmap_pair
 * @param value The new value to replace the existing value
 */
void obmap_replace_pair_value(obmap_pair *mp, obj *value);

/**
 * @brief Hash function for obmap_pair
 *
 * @param to_hash An obj pointer to an instance of obmap_pair
 *
 * @return Key value (hash) for the given obj pointer to a obmap_pair
 */
ob_hash_t obmap_hash_pair(const obj *to_hash);

/**
 * @brief Displays an instance of obmap_pair to stderr
 *
 * @param to_print A non-NULL obj pointer to type obmap_pair
 */
void obmap_display_pair(const obj *to_print);

/**
 * @brief Destructor for obmap_pair
 *
 * @param to_dealloc An obj pointer to an instance of obmap with
 * reference count of 0
 *
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void obmap_destroy_pair(obj *to_dealloc);


/* obmap DATA */

/**
 * @brief obmap internal structure, encapsulating all data needed for
 * an instance of obmap
 */
struct obmap_struct{
  obj base; /**< obj containing reference count and class membership data */
  uint8_t cap_idx; /**< index within MAP_CAPACITIES used as lookup for
                     capacity of map */
  obvector *hash_table; /**< map lookup table */
  obdeque *pairs; /**< A dense list of all MapPairs within the map, for speedy
                    rehash */
  uint32_t collisions; /**< variable that tracks the number of hashing
                         colisions encountered when adding keys to the table */
};

/* obmap PRIVATE METHODS */

/**
 * @brief Default constructor for obmap
 *
 * @return An instance of class obmap
 *
 * @warning All public constructors should call this constructor and intialize
 * individual members as needed, so that all base data is initialized properly.
 */
obmap * obmap_create_default(void);

/**
 * @brief Hash function for obmap
 *
 * @param to_hash An obj pointer to an instance of obmap
 *
 * @return Key value (hash) for the given obj pointer to a obmap
 */
ob_hash_t obmap_hash(const obj *to_hash);

/**
 * @brief Compares two instances of obmap
 *
 * @param a A non-NULL obj pointer to type obmap
 * @param b A non-NULL obj pointer to type obmap
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t obmap_compare(const obj *a, const obj *b);
/* Arguments are obj * so that a function pointer can be used for container
 * class sorting/search */

/**
 * @brief Displays an instance of obmap to stderr
 *
 * @param to_print A non-NULL obj pointer to type obmap
 */
void obmap_display(const obj *to_print);

/**
 * @brief Destructor for obmap
 *
 * @param to_dealloc An obj pointer to an instance of obmap with
 * reference count of 0
 *
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void obmap_destroy(obj *to_dealloc);

/**
 * @brief Increases the size of the map to the next capacity within
 * MAP_CAPACITIES array
 *
 * @param to_size obmap to resize
 */
void obmap_increase_size(obmap *to_size);

/**
 * @brief Adds an obdeque_iterator to the proper location within the obmap
 *
 * @param m The obmap to add the iterator to
 * @param it The obdeque_iterator to add to the hash table
 */
void obmap_add_to_table(obmap *m, obdeque_iterator *it);

/**
 * @brief Finds a key within the hash table, it exists
 *
 * @param m The obmap in which to search for the key
 * @param key The key to serach for within the obmap
 *
 * @return Index in the hash_table where a result can be found or where a NULL
 * value resides if key was not found
 */
ob_hash_t obmap_find_key(const obmap *m, const obj *key);

/**
 * @brief Generates an offset from the hash value to rectify collisions
 *
 * @param prev_offset Last offset provided (0 if initiating call for the first
 * time)
 *
 * @return New offset to the next possible location to insert within table
 */
ob_hash_t obmap_offset_collision(ob_hash_t prev_offset);

#endif

