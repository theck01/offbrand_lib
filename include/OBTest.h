
/**
 * @file OBTest.h
 * @brief OBTest Public Interface
 * @author theck
 */

#ifndef OBTEST_H
#define OBTEST_H

#include "offbrand.h"

typedef struct OBTest_struct OBTest;

/**
 * @brief Constructor, creates an instance of OBTest with a specified id
 * @param id Integer id for the created OBTest
 * @return Instance of OBTest with id as given by parameter
 */
OBTest * createTest(uint32_t id);

/**
 * @brief Returns the id of an OBTest instance
 * @param a Pointer to an instance of OBTest
 * @return id value of the OBTest argument
 */
uint32_t getTestID(OBTest *a);

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

#endif

