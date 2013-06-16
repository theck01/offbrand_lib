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
  OBObjType base; /**< obj containing reference count and class membership data */
  char *str; /**< encapsulated NUL terminated C string */
  uint32_t length; /**< integer tracking str length */
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
obhash_t hashString(const OBObjType *to_hash);

/**
 * @brief Compares two instances of OBString
 *
 * @param a A non-NULL obj pointer to type OBString
 * @param b A non-NULL obj pointer to type OBString
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t compareStrings(const OBObjType *a, const OBObjType *b);

/**
 * @brief Display function for an instance of OBString
 *
 * @param str A non-NULL obj pointer to type OBString
 */
void displayString(const OBObjType *str);

/** 
 * @brief Destructor for OBString
 * @param to_dealloc An obj pointer to an instance of OBString with
 * reference count of 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void deallocString(OBObjType *to_dealloc);

/* ADDITIONAL PRIVATE METHOD DECLARATIONS HERE*/

#endif

