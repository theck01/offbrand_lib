/**
 * @file OBMap.h
 * @brief OBMap Public Interface
 * @author theck
 */

#ifndef OBMAP_H
#define OBMAP_H

#include "offbrand.h"

/* Class type declaration */
typedef struct OBMap_struct OBMap;


/* PUBLIC METHODS */

/**
 * @brief Constructor, creates a new, empty, OBMap instance
 * @return Pointer to the newly created OBMap instance
 */
OBMap * createMap(void);

/**
 * @brief Add a key-value pair to an OBMap
 *
 * @param map, Pointer to an instance of OBMap
 * @param key, Pointer to any Offbrand compatible class to use as a lookup key
 * @param key, Pointer to any Offbrand compatible class 
 *
 * @details If the key is already contained within the map then the old key
 * is replaced with the provided key
 */
void addToMap(OBMap *map, obj *key, obj *value);

/**
 * @brief Lookup the value stored at key in an OBMap
 *
 * @param map, Pointer to an instance of OBMap
 * @param key, Pointer to any Offbrand compatible class to use as a lookup key
 *
 * @retval NULL Key not found in OBMap instance
 * @retval non-NULL Value found at key in OBMap
 */
obj * lookupMapKey(const OBMap *map, const obj *key);

/**
 * @brief Removes a key-value pair from an OBMap
 * 
 * @param map, Pointer to an instance of OBMap
 * @param key, Pointer to any Offbrand compatible class to use as a lookup key
 * 
 * @details If no key exists then the funciton will silently do nothing
 */
void removeMapKey(OBMap *map, obj *key);

/**
 * @brief Clears an OBMap of all key-value pairs, essentially restoring it
 * to the initial empty state that existed after creation
 *
 * @param map, Pointer to an instance of OBMap
 */
void clearMap(OBMap *map);

#endif

