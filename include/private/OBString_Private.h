/**
 * @file OBString_Private.h
 * @brief OBString Private Interface
 * @author theck
 */

#ifndef OBSTRING_PRIVATE_H
#define OBSTRING_PRIVATE_H

#include "../OBString.h"

/* DATA */

/**
 * @brief OBString internal structure, encapsulating all data needed for
 * an instance of OBString
 */
struct OBString_struct{
  obj base; /**< obj containing reference count and class membership data */
  char *str; /**< encapsulated NUL terminated C string */
  uint64_t length; /**< integer tracking str length */
};


/* PRIVATE METHODS */

/**
 * @brief Default constructor for OBString
 * @return An instance of class OBString
 * @warning All public constructors should call this constructor and intialize
 * individual members as needed, so that all base data is initialized properly.
 */
OBString * createDefaultString(void);

/**
 * @brief Hash function for OBString
 * @param to_hash An obj pointer to an instance of OBString
 * @return Key value (hash) for the given obj pointer to a OBString
 */
obhash_t hashString(const obj *to_hash);

/** 
 * @brief Destructor for OBString
 * @param to_dealloc An obj pointer to an instance of OBString with
 * reference count of 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void deallocString(obj *to_dealloc);

/* ADDITIONAL PRIVATE METHOD DECLARATIONS HERE*/

#endif

