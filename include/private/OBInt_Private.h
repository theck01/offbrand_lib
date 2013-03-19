/**
 * @file OBInt_Private.h
 * @brief OBInt Private Interface
 * @author theck
 */

#ifndef OBINT_PRIVATE_H
#define OBINT_PRIVATE_H

#include "../OBInt.h"

/* DATA */

/**
 * @brief OBInt internal structure, encapsulating all data needed for
 * an instance of OBInt
 */
struct OBInt_struct{
  obj base; /**< obj containing reference count and class membership data */
  uint8_t sign; /**< Sign of integer value, positive or negative */
  uint8_t digits; /**< Digit array, in little endian order */
  uint64_t num_digits; /**< Number of digits in the digit array */
};


/* PRIVATE METHODS */

/**
 * @brief Default constructor for OBInt
 * @param num_digits Approximate number of digits required to store the entire,
 * always overestimate to ensure enough space is reserved
 * @return An instance of class OBInt
 * @warning All public constructors should call this constructor and intialize
 * individual members as needed, so that all base data is initialized properly.
 */
OBInt * createDefaultInt(uint64_t num_digits);

/**
 * @brief Hash function for OBInt
 * @param to_hash An obj pointer to an instance of OBInt
 * @return Key value (hash) for the given obj pointer to a OBInt
 */
obhash_t hashInt(const obj *to_hash);

/**
 * @brief Compares two instances of OBInt
 *
 * @param a A non-NULL obj pointer to type OBInt
 * @param b A non-NULL obj pointer to type OBInt
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t compareInts(const obj *a, const obj *b);
/* Arguments are obj * so that a function pointer can be used for container
 * class sorting/search */

/**
 * @brief Descriptor for an instance of OBInt, prints relevant
 * information about the class to stderr
 *
 * @param to_print A non-NULL obj pointer to an instance of type
 * OBInt
 */
void displayInt(const obj *to_print);

/** 
 * @brief Destructor for OBInt
 * @param to_dealloc An obj pointer to an instance of OBInt with
 * reference count of 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void deallocInt(obj *to_dealloc);

/**
 * @brief Creates a new integer as a result of addition between two OBInts
 * with sign value ignored
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 *
 * @return An instance of OBInt, which may or may not have proper sign
 */
OBInt * addUnsignedInts(const OBInt *a, const OBInt *b);

/**
 * @brief Creates a new integer as a result of subtraction between two OBInts
 * with sign ignored
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 *
 * @return An instance of OBInt, which may or may not have proper sign
 */
OBInt * subtractUnsignedInts(const OBInt *a, const OBInt *b);

/**
 * @brief Creates a new integer as a result of multiplication between two OBInts
 * with sign ignored
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 *
 * @return An instance of OBInt, which may or may not have proper sign
 */
OBInt * multiplyUnsignedInts(const OBInt *a, const OBInt *b);

/**
 * @brief Creates a new integer as a result of multiplication between two OBInts
 * with sign ignored
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 * @param c A non-NULL pointer to type OBInt used to accumulate a quotient value
 * @param quotient_or_remainder A boolean value, 0 indicates that a remainder 
 * should be returned, non-zero indicates a quotient should be returned
 *
 * @return An instance of OBInt, which may or may not have proper sign
 */
OBInt * reduceUnsignedInts(const OBInt *a, const OBInt *b, const OBInt *c, 
                           uint8_t quotient_or_remainder);

/**
 * @brief Returns the index where the most significant non-zero digit occurs, or
 * zero if there are no non-zero digits
 *
 * @param a A non-NULL pointer to type OBInt
 *
 * @return An integer index where the most significant non-zero digit in a can
 * be found
 */
uint64_t mostSigNonZero(const OBInt *a);

#endif

