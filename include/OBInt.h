/**
 * @file OBInt.h
 * @brief OBInt Public Interface
 * @author theck
 */

#ifndef OBINT_H
#define OBINT_H

#include "offbrand.h"
#include "OBString.h"

/** Class type declaration */
typedef struct OBInt_struct OBInt;


/* PUBLIC METHODS */

/**
 * @brief Creates a new OBInt with the given integer
 *
 * @param num Integer value for new OBInt
 *
 * @return An instance of OBInt with value given by num
 */
OBInt * OBIntCreate(int64_t num);

/**
 * @brief Returns the value stored in OBInt as an integer
 *
 * @param a A non-NULL pointer to type OBInt
 *
 * @return A signed 64 bit integer representing value stored in a
 *
 * @warning If the value of the OBInt cannot be represented in 64 bits then
 * returned value will not represent the true value of the OBInt
 */
int64_t OBIntGetIntValue(const OBInt *a);

/**
 * @brief Creates a new OBInt with value indicated by given string
 *
 * @param numstr A non-NULL pointer to an OBString instance containing only 
 * digit characters and possibly one '-' character at the beginning of the 
 * string to indicate a negative value
 *
 * @return An instance of OBInt with value given by numstr
 */
OBInt * OBIntCreateFromString(const OBString *numstr);

/**
 * @brief Creates an instance of OBString containing a string representation
 * of the value of an OBInt
 *
 * @param a A non-NULL pointer to type OBInt
 *
 * @return An instance of OBString
 */
OBString * OBIntGetStringValue(const OBInt *a);

/**
 * @brief Creates an instance of OBInt with the same value as the argument
 * OBInt
 *
 * @param a A non-NULL pointer to type OBInt
 *
 * @return A copy of the argument OBInt
 */
OBInt * OBIntCopy(const OBInt *a);

/**
 * @brief Checks if an instance of OBInt is zero and returns the boolean truth
 * value
 *
 * @param a A non-NULL pointer to type OBInt
 *
 * @retval 0 The argument is non-zero
 * @retval non-zero The argument is zero
 */
uint8_t OBIntIsZero(const OBInt *a);

/**
 * @brief Checks if an instance of OBInt is negative and returns the boolean
 * truth value
 *
 * @param a A non-NULL pointer to type OBInt
 *
 * @retval 0 The argument is positive
 * @retval non-zero The argument is negative
 */
uint8_t OBIntIsNegative(const OBInt *a);

/**
 * @brief Creates a new OBInt containing the sum of two OBInt values
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 *
 * @return An instance of OBInt with value given by a+b
 */
OBInt * OBIntAdd(const OBInt *a, const OBInt *b);

/**
 * @brief Creates a new OBInt containing the sum of an OBInt and a machine
 * integer
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A signed 64 bit machine integer
 *
 * @return An instance of OBInt with value given by a+b
 */
OBInt * OBIntAddPrimitive(const OBInt *a, int64_t b);

/**
 * @brief Creates a new OBInt containing the difference of two OBInt values
 *
 * @param a A non-NULL pointer to type OBInt, the minuend
 * @param b A non-NULL pointer to type OBInt, the subtrahend
 *
 * @return An instance of OBInt with value given by a-b
 */
OBInt * OBIntSubtract(const OBInt *a, const OBInt *b);

/**
 * @brief Creates a new OBInt containing the difference of an OBInt and a 
 * machine integer
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A signed 64 bit machine integer
 *
 * @return An instance of OBInt with value given by a-b
 */
OBInt * OBIntSubtractPrimitive(const OBInt *a, int64_t b);

/**
 * @brief Creates a new OBInt containing the product of two OBInt values
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 *
 * @return An instance of OBInt with value given by a*b
 */
OBInt * OBIntMultiply(const OBInt *a, const OBInt *b);

/**
 * @brief Creates a new OBInt containing the product of an OBInt and a 
 * machine integer
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A signed 64 bit machine integer
 *
 * @return An instance of OBInt with value given by a*b
 */
OBInt * OBIntMultiplyPrimitive(const OBInt *a, int64_t b);

/**
 * @brief Creates a new OBInt containing the quotient resule of integer division
 * between two OBInt values,
 *
 * @param a A non-NULL pointer to type OBInt, the dividend
 * @param b A non-NULL pointer to type OBInt, the divisor
 *
 * @return An instance of OBInt with value given by a/b
 */
OBInt * OBIntDivide(const OBInt *a, const OBInt *b);

/**
 * @brief Creates a new OBInt containing the quotient of an OBInt and a 
 * machine integer
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A signed 64 bit machine integer
 *
 * @return An instance of OBInt with value given by a/b
 */
OBInt * OBIntDividePrimitive(const OBInt *a, int64_t b);

/**
 * @brief Creates a new OBInt containing the remainder of integer division
 * between two OBInt values,
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 *
 * @return An instance of OBInt with value given by a%b
 */
OBInt * OBIntMod(const OBInt *a, const OBInt *b);

/**
 * @brief Creates a new OBInt containing the remainder of integer division
 * of an OBInt and a machine integer
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A signed 64 bit machine integer
 *
 * @return An instance of OBInt with value given by a%b
 */
OBInt * OBIntModPrimitive(const OBInt *a, int64_t b);

#endif

