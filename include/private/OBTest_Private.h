/**
 * @file OBTest_Private.h
 * @brief OBTest Private Interface
 * @author theck
 */

#ifndef OBTEST_PRIVATE_H
#define OBTEST_PRIVATE_H

#include "../OBTest.h"

/* DATA */

/**
 * @brief OBTest internal structure, encapsulating all data need for an instance
 * of OBTest
 */
struct OBTest_struct{
  obj base; /**< obj containing reference count and class membership data */
  uint32_t id; /**< integer id to identify different instances of OBTest */
};

/**
 * @brief Hash function for OBTest
 * @param to_hash An obj pointer to an instance of OBString
 * @return Key value (hash) for the given obj pointer to a OBString 
 */
obhash_t hashTest(const obj *to_hash);

/**
 * @brief Compares two instances of OBTest
 *
 * @param a A non-NULL obj pointer to type OBTest
 * @param b A non-NULL obj pointer to type OBTest
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t compareTests(const obj *a, const obj *b);

/**
 * @brief Destructor for OBTest
 * @param to_dealloc An obj pointer to an instance of OBTest with reference
 * count of 0
 * @warning Do not call manually, release will call automatically when
 * instance reference count drops to 0!
 */
void deallocTest(obj *to_dealloc);

#endif
