/**
 * @file obmap.h
 * @brief obmap Public Interface
 * @author theck
 */

#ifndef OBMAP_H
#define OBMAP_H

#include "offbrand.h"

/** Class type declaration */
typedef struct obmap_struct obmap;


/* PUBLIC METHODS */

/**
 * @brief Constructor, creates a new, empty, obmap instance with a small initial
 * capacity
 * @return Pointer to the newly created obmap instance
 */
obmap * obmap_new(void);

/**
 * @brief Constructor, creates a new, empty obmap instance with capacity given
 * @param capacity Capacity required for obmap instance
 * @return Pointer to the newly created obmap instance
 */
obmap * obmap_new_with_capacity(uint32_t capacity);

/**
 * @brief Copy constructor, creates a new obmap with the exact same contents
 * of another obmap
 * @param to_copy Pointer to an instance of obmap to copy
 * @return A copy of the provided obmap
 */
obmap * obmap_copy(const obmap *to_copy);

/**
 * @brief Add a key-value pair to an obmap
 *
 * @param m Pointer to an instance of obmap
 * @param key Pointer to any Offbrand compatible class to use as a lookup key
 * @param value Pointer to any Offbrand compatible class
 *
 * @details If the key is already contained within the m then the old value
 * stored at that key is replaced with the new value
 */
void obmap_insert(obmap *m, obj *key, obj *value);

/**
 * @brief Lookup the value stored at key in an obmap
 *
 * @param m Pointer to an instance of obmap
 * @param key Pointer to any Offbrand compatible class to use as a lookup key
 *
 * @retval NULL Key not found in obmap instance or key bound to NULL value
 * @retval non-NULL Value found at key in obmap
 */
obj * obmap_lookup(const obmap *m, const obj *key);

/**
 * @brief Removes a key-value pair from an obmap
 *
 * @param m Pointer to an instance of obmap
 * @param key Pointer to any Offbrand compatible class to use as a lookup key
 *
 * @details If no key exists then the funciton will silently do nothing
 *
 * @warning This function runs in O(n), and is inherently more expensive
 * than an add or lookup.
 */
void obmap_remove(obmap *m, obj *key);

/**
 * @brief Rehashes all elements contained in an obmap
 *
 * @param m Pointer to an instance of obmap
 *
 * @details This method is useful for ensuring that mutable keys will still lead
 * to valid lookups of the associated values even after key(s) have been altered
 */
void obmap_rehash(obmap *m);

/**
 * @brief Clears an obmap of all key-value pairs, essentially restoring it
 * to the initial empty state that existed after creation
 *
 * @param m Pointer to an instance of obmap
 */
void obmap_clear(obmap *m);

#endif

