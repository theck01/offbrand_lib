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
OBInt * createIntWithInt(int64_t num);

/**
 * @brief Creates a new OBInt with value indicated by given string
 *
 * @param numstr A non-NULL pointer to an OBString instance containing only 
 * digit characters and possibly one '-' character at the beginning of the 
 * string to indicate a negative value
 *
 * @return An instance of OBInt with value given by numstr
 */
OBInt * intFromString(const OBString *numstr);

/**
 * @brief Creates an instance of OBString containing a string representation
 * of the value of an OBInt
 *
 * @param a A non-NULL pointer to type OBInt
 *
 * @return An instance of OBString
 */
OBString * stringFromInt(const OBInt *a);


/**
 * @brief Creates a new OBInt containing the sum of two OBInt values
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 *
 * @return An instance of OBInt with value given by a+b
 */
OBInt * addInts(const OBInt *a, const OBInt *b);

/**
 * @brief Creates a new OBInt containing the difference of two OBInt values
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 *
 * @return An instance of OBInt with value given by a-b
 */
OBInt * subtractInts(const OBInt *a, const OBInt *b);

/**
 * @brief Creates a new OBInt containing the product of two OBInt values
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 *
 * @return An instance of OBInt with value given by a*b
 */
OBInt * multiplyInts(const OBInt *a, const OBInt *b);

/**
 * @brief Creates a new OBInt containing the quotient resule of integer division
 * between two OBInt values,
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 *
 * @return An instance of OBInt with value given by a/b
 */
OBInt * divideInts(const OBInt *a, const OBInt *b);

/**
 * @brief Creates a new OBInt containing the remainder of integer division
 * between two OBInt values,
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 *
 * @return An instance of OBInt with value given by a%b
 */
OBInt * modInts(const OBInt *a, const OBInt *b);

#endif

