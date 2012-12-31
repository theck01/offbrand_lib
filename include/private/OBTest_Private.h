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

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
/**
 * @brief Destructor for OBTest
 * @param to_dealloc An obj pointer to an instance of OBTest with reference
 * count of 0
 * @warning Do not call manually, release will call automatically when
 * instance reference count drops to 0!
 */
void deallocTest(obj *to_dealloc);

#endif
