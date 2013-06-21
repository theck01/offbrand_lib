/**
 * @file obint_private.h
 * @brief obint Private Interface
 * @author theck
 */

#ifndef OBINT_PRIVATE_H
#define OBINT_PRIVATE_H

#include "../obint.h"

/* DATA */

/**
 * @brief obint internal structure, encapsulating all data needed for
 * an instance of obint
 */
struct obint_struct{
  obj base; /**< obj containing reference count and class membership data */
  int8_t sign; /**< Sign of integer value, positive or negative */
  int8_t *digits; /**< Digit array, in little endian order */
  uint64_t num_digits; /**< Number of digits in the digit array */
};


/* PRIVATE METHODS */

/**
 * @brief Default constructor for obint
 * @param num_digits Approximate number of digits required to store the entire,
 * always overestimate to ensure enough space is reserved
 * @return An instance of class obint
 * @warning All public constructors should call this constructor and intialize
 * individual members as needed, so that all base data is initialized properly.
 */
obint * obint_create_default(uint64_t num_digits);

/**
 * @brief Hash function for obint
 * @param to_hash An obj pointer to an instance of obint
 * @return Key value (hash) for the given obj pointer to a obint
 */
ob_hash_t obint_hash(const obj *to_hash);

/**
 * @brief Compares two instances of obint
 *
 * @param a A non-NULL obj pointer to type obint
 * @param b A non-NULL obj pointer to type obint
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is greater than b
 * @retval OB_EQUAL_TO obj a is equal to b
 */
int8_t obint_compare(const obj *a, const obj *b);
/* Arguments are obj * so that a function pointer can be used for container
 * class sorting/search */

/**
 * @brief Compares magnitudes of two instances of obint
 *
 * @param a A non-NULL pointer to type obint
 * @param b A non-NULL pointer to type obint
 * @retval OB_LESS_THAN Magnitude of a a is less than b
 * @retval OB_GREATER_THAN Magnitude of a is greater than to b
 * @retval OB_EQUAL_TO Magnitude of a is equal to b
 */
int8_t obint_compare_magnitudes(const obint *a, const obint *b);

/**
 * @brief Descriptor for an instance of obint, prints relevant
 * information about the class to stderr
 *
 * @param to_print A non-NULL obj pointer to an instance of type
 * obint
 */
void obint_display(const obj *to_print);

/**
 * @brief Destructor for obint
 * @param to_dealloc An obj pointer to an instance of obint with
 * reference count of 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void obint_destroy(obj *to_dealloc);

/**
 * @brief Creates a new integer as a result of addition between two obints
 * with sign value ignored
 *
 * @param a A non-NULL pointer to type obint
 * @param b A non-NULL pointer to type obint
 *
 * @return An instance of obint, which may or may not have proper sign
 */
obint * obint_add_unsigned(const obint *a, const obint *b);

/**
 * @brief Creates a new integer as a result of subtraction between two obints
 * with sign ignored
 *
 * @param a A non-NULL pointer to type obint, larger or equal to b
 * @param b A non-NULL pointer to type obint, smaller or equal to a
 *
 * @return An instance of obint, which may or may not have proper sign
 *
 * @warning Proper results guaranteed only for a >= b, a < b will not work and
 * probably will cause segmentation faults
 */
obint * obint_subtract_unsigned(const obint *a, const obint *b);

/**
 * @brief Creates a new integer as a result of multiplication between two obints
 * with sign ignored
 *
 * @param a A non-NULL pointer to type obint
 * @param b A non-NULL pointer to type obint
 *
 * @return An instance of obint, which may or may not have proper sign
 *
 * @details Multiplication performed using the Karatsuba multiplcation algorithm
 */
obint * obint_multiply_unsigned(const obint *a, const obint *b);

/**
 * @brief Creates a new integer as a result of divison or modulus between two
 * obints with sign ignored
 *
 * @param a A non-NULL pointer to type obint
 * @param b A non-NULL pointer to type obint
 * @param approx A non-NULL pointer to type obint used to accumulate a quotient
 * value
 * @param quotient A boolean value, 0 indicates that a remainder
 * should be returned, non-zero indicates a quotient should be returned
 *
 * @return An instance of obint, which may or may not have proper sign
 */
obint * obint_reduce(const obint *a, const obint *b, const obint *approx,
                     uint8_t quotient);

/**
 * @brief Returns the index where the most significant non-zero digit occurs, or
 * zero if there are no non-zero digits
 *
 * @param a A non-NULL pointer to type obint
 *
 * @return An integer index where the most significant non-zero digit in a can
 * be found
 */
uint64_t obint_most_sig(const obint *a);

/**
 * @brief Splits the obint into two obint instances at given index.
 *
 * @param a A non-NULL pointer to type obint to be split
 * @param i The integer index where the split occurs, where a-digits[i] is
 * placed in the most significant section of the split
 * @param b1 A reference to a pointer to type obint, used to return the most
 * significant section of the obint being split
 * @param b0 A reference to a pointer to type obint, used to return the least
 * significant section of the obint being split
 *
 * @details If index i extends beyond the bounds of a then b1 is set to point
 * to a 0 obint and b2 is set to point to a copy of a. a is treated as unsigned
 * for this operation
 */
void obint_split(const obint *a, uint64_t i, obint **b1, obint **b0);

/**int
 * @brief Shifts the obint the given number of digits to the left, in place,
 * essentially unsigned multiplication by 10^m
 *
 * @param a A non-NULL pointer to type obint
 * @param m Number of digits to shift
 */
void obint_shift(obint *a, uint64_t m);

/**
 * @brief Returns the unsigned value of the obint
 *
 * @param a A non-NULL pointer to type obint
 *
 * @return a positive int64_t representing the value of the argument obint
 */
int64_t obint_magnitude(const obint *a);

/**
 * @brief Sets the number of digits operated on in a primitive from 1 to 17
 *
 * @param digits An integer between 2 and 17 inclusive
 *
 * @warning If digits does not fall in the 2 to 17 inclusive range nothing
 * is done
 * @warning The default value of 17 is most efficient, this method exists for
 * testing purposes only!
 */
void obint_set_max_digits(uint8_t digits);

#endif

