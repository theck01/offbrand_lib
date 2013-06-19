/**
 * @file obint.h
 * @brief obint Public Interface
 * @author theck
 */

#ifndef OBINT_H
#define OBINT_H

#include "offbrand.h"
#include "obstring.h"

/** Class type declaration */
typedef struct obint_struct obint;


/* PUBLIC METHODS */

/**
 * @brief Creates a new obint with the given integer
 *
 * @param num Integer value for new obint
 *
 * @return An instance of obint with value given by num
 */
obint * obint_new(int64_t num);

/**
 * @brief Returns the value stored in obint as an integer
 *
 * @param a A non-NULL pointer to type obint
 *
 * @return A signed 64 bit integer representing value stored in a
 *
 * @warning If the value of the obint cannot be represented in 64 bits then
 * returned value will not represent the true value of the obint
 */
int64_t obint_value(const obint *a);

/**
 * @brief Creates a new obint with value indicated by given string
 *
 * @param numstr A non-NULL pointer to an obstring instance containing only
 * digit characters and possibly one '-' character at the beginning of the
 * string to indicate a negative value
 *
 * @return An instance of obint with value given by numstr
 */
obint * obint_from_string(const obstring *numstr);

/**
 * @brief Creates an instance of obstring containing a string representation
 * of the value of an obint
 *
 * @param a A non-NULL pointer to type obint
 *
 * @return An instance of obstring
 */
obstring * obint_to_string(const obint *a);

/**
 * @brief Creates an instance of obint with the same value as the argument
 * obint
 *
 * @param a A non-NULL pointer to type obint
 *
 * @return A copy of the argument obint
 */
obint * obint_copy(const obint *a);

/**
 * @brief Checks if an instance of obint is zero and returns the boolean truth
 * value
 *
 * @param a A non-NULL pointer to type obint
 *
 * @retval 0 The argument is non-zero
 * @retval non-zero The argument is zero
 */
uint8_t obint_is_zero(const obint *a);

/**
 * @brief Checks if an instance of obint is negative and returns the boolean
 * truth value
 *
 * @param a A non-NULL pointer to type obint
 *
 * @retval 0 The argument is positive
 * @retval non-zero The argument is negative
 */
uint8_t obint_is_negative(const obint *a);

/**
 * @brief Creates a new obint containing the sum of two obint values
 *
 * @param a A non-NULL pointer to type obint
 * @param b A non-NULL pointer to type obint
 *
 * @return An instance of obint with value given by a+b
 */
obint * obint_add(const obint *a, const obint *b);

/**
 * @brief Creates a new obint containing the sum of an obint and a machine
 * integer
 *
 * @param a A non-NULL pointer to type obint
 * @param b A signed 64 bit machine integer
 *
 * @return An instance of obint with value given by a+b
 */
obint * obint_add_primitive(const obint *a, int64_t b);

/**
 * @brief Creates a new obint containing the difference of two obint values
 *
 * @param a A non-NULL pointer to type obint
 * @param b A non-NULL pointer to type obint
 *
 * @return An instance of obint with value given by a-b
 */
obint * obint_subtract(const obint *a, const obint *b);

/**
 * @brief Creates a new obint containing the difference of an obint and a
 * machine integer
 *
 * @param a A non-NULL pointer to type obint
 * @param b A signed 64 bit machine integer
 *
 * @return An instance of obint with value given by a-b
 */
obint * obint_subtract_primitive(const obint *a, int64_t b);

/**
 * @brief Creates a new obint containing the product of two obint values
 *
 * @param a A non-NULL pointer to type obint
 * @param b A non-NULL pointer to type obint
 *
 * @return An instance of obint with value given by a*b
 */
obint * obint_multiply(const obint *a, const obint *b);

/**
 * @brief Creates a new obint containing the product of an obint and a
 * machine integer
 *
 * @param a A non-NULL pointer to type obint
 * @param b A signed 64 bit machine integer
 *
 * @return An instance of obint with value given by a*b
 */
obint * obint_multiply_primitive(const obint *a, int64_t b);

/**
 * @brief Creates a new obint containing the quotient resule of integer division
 * between two obint values,
 *
 * @param a A non-NULL pointer to type obint
 * @param b A non-NULL pointer to type obint
 *
 * @return An instance of obint with value given by a/b
 */
obint * obint_divide(const obint *a, const obint *b);

/**
 * @brief Creates a new obint containing the quotient of an obint and a
 * machine integer
 *
 * @param a A non-NULL pointer to type obint
 * @param b A signed 64 bit machine integer
 *
 * @return An instance of obint with value given by a/b
 */
obint * obint_divide_primitive(const obint *a, int64_t b);

/**
 * @brief Creates a new obint containing the remainder of integer division
 * between two obint values,
 *
 * @param a A non-NULL pointer to type obint
 * @param b A non-NULL pointer to type obint
 *
 * @return An instance of obint with value given by a%b
 */
obint * obint_mod(const obint *a, const obint *b);

/**
 * @brief Creates a new obint containing the remainder of integer division
 * of an obint and a machine integer
 *
 * @param a A non-NULL pointer to type obint
 * @param b A signed 64 bit machine integer
 *
 * @return An instance of obint with value given by a%b
 */
obint * obint_mod_primitive(const obint *a, int64_t b);

#endif

