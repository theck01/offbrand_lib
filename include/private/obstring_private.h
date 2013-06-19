/**
 * @file obstring_private.h
 * @brief obstring Private Interface
 * @author theck
 */

#ifndef OBSTRING_PRIVATE_H
#define OBSTRING_PRIVATE_H

#include "../obstring.h"

/* DATA */

/**
 * @brief obstring internal structure, encapsulating all data needed for
 * an instance of obstring
 */
struct obstring_struct{
  obj base; /**< obj containing reference count and class membership data */
  char *str; /**< encapsulated NUL terminated C string */
  uint32_t length; /**< integer tracking str length */
};


/* PRIVATE METHODS */

/**
 * @brief Default constructor for obstring
 * @return An instance of class obstring
 * @warning All public constructors should call this constructor and intialize
 * individual members as needed, so that all base data is initialized properly.
 */
obstring * obstring_create_default(void);

/**
 * @brief Hash function for obstring
 * @param to_hash An obj pointer to an instance of obstring
 * @return Key value (hash) for the given obj pointer to a obstring
 */
ob_hash_t obstring_hash(const obj *to_hash);

/**
 * @brief Compares two instances of obstring
 *
 * @param a A non-NULL obj pointer to type obstring
 * @param b A non-NULL obj pointer to type obstring
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t obstring_compare(const obj *a, const obj *b);

/**
 * @brief Display function for an instance of obstring
 *
 * @param str A non-NULL obj pointer to type obstring
 */
void obstring_display(const obj *str);

/**
 * @brief Destructor for obstring
 * @param to_dealloc An obj pointer to an instance of obstring with
 * reference count of 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void obstring_destroy(obj *to_dealloc);

#endif

