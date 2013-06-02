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
  int8_t sign; /**< Sign of integer value, positive or negative */
  int8_t *digits; /**< Digit array, in little endian order */
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
 * @retval OB_GREATER_THAN obj a is greater than b
 * @retval OB_EQUAL_TO obj a is equal to b
 */
int8_t compareInts(const obj *a, const obj *b);
/* Arguments are obj * so that a function pointer can be used for container
 * class sorting/search */

/* @brief Compares magnitudes of two instances of OBInt
 * 
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 * @retval OB_LESS_THAN Magnitude of a a is less than b
 * @retval OB_GREATER_THAN Magnitude of a is greater than to b
 * @retval OB_EQUAL_TO Magnitude of a is equal to b
 */
int8_t compareMagnitudes(const OBInt *a, const OBInt *b);

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
 * @param a A non-NULL pointer to type OBInt, larger or equal to b
 * @param b A non-NULL pointer to type OBInt, smaller or equal to a
 *
 * @return An instance of OBInt, which may or may not have proper sign
 *
 * @warning Proper results guaranteed only for a >= b, a < b will not work and
 * probably will cause segmentation faults
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
 *
 * @details Multiplication performed using the Karatsuba multiplcation algorithm
 */
OBInt * multiplyUnsignedInts(const OBInt *a, const OBInt *b);

/**
 * @brief Creates a new integer as a result of divison or modulus between two 
 * OBInts with sign ignored
 *
 * @param a A non-NULL pointer to type OBInt
 * @param b A non-NULL pointer to type OBInt
 * @param approx A non-NULL pointer to type OBInt used to accumulate a quotient
 * value
 * @param quotient A boolean value, 0 indicates that a remainder 
 * should be returned, non-zero indicates a quotient should be returned
 *
 * @return An instance of OBInt, which may or may not have proper sign
 */
OBInt * reduceUnsignedInts(const OBInt *a, const OBInt *b, const OBInt *approx, 
                           uint8_t quotient);

/**
 * @brief Returns the index where the most significant non-zero digit occurs, or
 * zero if there are no non-zero digits
 *
 * @param a A non-NULL pointer to type OBInt
 *
 * @return An integer index where the most significant non-zero digit in a can
 * be found
 */
uint64_t mostSig(const OBInt *a);

/**
 * @brief Splits the OBInt into two OBInt instances at given index.
 *
 * @param a A non-NULL pointer to type OBInt to be split
 * @param i The integer index where the split occurs, where a-digits[i] is
 * placed in the most significant section of the split
 * @param b1 A reference to a pointer to type OBInt, used to return the most
 * significant section of the OBInt being split
 * @param b0 A reference to a pointer to type OBInt, used to return the least
 * significant section of the OBInt being split
 *
 * @details If index i extends beyond the bounds of a then b1 is set to point
 * to a 0 OBInt and b2 is set to point to a copy of a. a is treated as unsigned
 * for this operation
 */
void splitInt(const OBInt *a, uint64_t i, OBInt **b1, OBInt **b0);

/**int
 * @brief Shifts the OBInt the given number of digits to the left, in place, 
 * essentially unsigned multiplication by 10^m 
 *
 * @param a A non-NULL pointer to type OBInt
 * @param m Number of digits to shift
 */
void shiftInt(OBInt *a, uint64_t m);

/**
 * @brief Returns the unsigned value of the OBInt 
 *
 * @param a A non-NULL pointer to type OBInt
 *
 * @return a positive int64_t representing the value of the argument OBInt
 */
int64_t unsignedValue(const OBInt *a);

/**
 * @brief Sets the number of digits operated on in a primitive from 1 to 17
 *
 * @param digits An integer between 1 and 17 inclusive
 *
 * @warning If digits does not fall in the 1 to 17 inclusive range nothing
 * is done
 * @warning The default value of 17 is most efficient, this method exists for
 * testing purposes only!
 */
void setMaxDigits(uint8_t digits);

#endif

