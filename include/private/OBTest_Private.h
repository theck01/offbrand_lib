/**
 * @file obtest_private.h
 * @brief obtest Private Interface
 * @author theck
 */

#ifndef OBTEST_PRIVATE_H
#define OBTEST_PRIVATE_H

#include "../obtest.h"

/* DATA */

/**
 * @brief obtest internal structure, encapsulating all data need for an instance
 * of obtest
 */
struct obtest_struct{
  obj base; /**< obj containing reference count and class membership data */
  uint32_t id; /**< integer id to identify different instances of obtest */
};

/**
 * @brief Hash function for obtest
 * @param to_hash An obj pointer to an instance of OBString
 * @return Key value (hash) for the given obj pointer to a OBString
 */
ob_hash_t obtest_hash(const obj *to_hash);

/**
 * @brief Compares two instances of obtest
 *
 * @param a A non-NULL obj pointer to type obtest
 * @param b A non-NULL obj pointer to type obtest
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t obtest_compare(const obj *a, const obj *b);

/**
 * @brief Display function for an instance of obtest
 *
 * @param test A non-NULL obj pointer to type obtest
 */
void obtest_display(const obj *test);

/**
 * @brief Destructor for obtest
 * @param to_dealloc An obj pointer to an instance of obtest with reference
 * count of 0
 * @warning Do not call manually, release will call automatically when
 * instance reference count drops to 0!
 */
void obtest_destroy(obj *to_dealloc);

#endif
