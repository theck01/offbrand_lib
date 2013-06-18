/**
 * @file OBMap.h
 * @brief OBMap Public Interface
 * @author theck, danhd123
 */

#ifndef OBMAP_H
#define OBMAP_H

#include "offbrand.h"

/** Class type declaration */
typedef struct OBMap_struct OBMap;


/* PUBLIC METHODS */

/**
 * @brief Constructor, creates a new, empty, OBMap instance with a small initial
 * capacity
 * @return Pointer to the newly created OBMap instance
 */
OBMap * OBMapCreate(void);

/**
 * @brief Constructor, creates a new, empty OBMap instance with capacity given
 * @param capacity Capacity required for OBMap instance
 * @return Pointer to the newly created OBMap instance
 */
OBMap *OBMapCreateWithCapacity(uint32_t capacity);

/**
 * @brief Copy constructor, creates a new OBMap with the exact same contents
 * of another OBMap
 * @param to_copy Pointer to an instance of OBMap to copy
 * @return A copy of the provided OBMap
 */
OBMap * OBMapCopy(const OBMap *to_copy);

/**
 * @brief Add a key-value pair to an OBMap
 *
 * @param m Pointer to an instance of OBMap
 * @param key Pointer to any Offbrand compatible class to use as a lookup key
 * @param value Pointer to any Offbrand compatible class
 *
 * @details If the key is already contained within the m then the old value
 * stored at that key is replaced with the new value
 */
void OBMapAdd(OBMap *m, OBTypeRef key, OBTypeRef value);

/**
 * @brief Lookup the value stored at key in an OBMap
 *
 * @param m Pointer to an instance of OBMap
 * @param key Pointer to any Offbrand compatible class to use as a lookup key
 *
 * @retval NULL Key not found in OBMap instance or key bound to NULL value
 * @retval non-NULL Value found at key in OBMap
 */
OBTypeRef OBMapGetValueForKey(const OBMap *m, OBTypeRef key);

/**
 * @brief Removes a key-value pair from an OBMap
 * 
 * @param m Pointer to an instance of OBMap
 * @param key Pointer to any Offbrand compatible class to use as a lookup key
 * 
 * @details If no key exists then the funciton will silently do nothing
 *
 * @warning This function runs in O(n), and is inherently more expensive
 * than an add or lookup.
 */
void OBMapRemoveKey(OBMap *m, OBTypeRef key);

/**
 * @brief Rehashes all elements contained in an OBMap
 *
 * @param m Pointer to an instance of OBMap
 *
 * @details This method is useful for ensuring that mutable keys will still lead
 * to valid lookups of the associated values even after key(s) have been altered
 */
void OBMapRehash(OBMap *m);

/**
 * @brief Clears an OBMap of all key-value pairs, essentially restoring it
 * to the initial empty state that existed after creation
 *
 * @param m Pointer to an instance of OBMap
 */
void OBMapClear(OBMap *m);

#endif

