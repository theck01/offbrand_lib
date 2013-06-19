/**
 * @file obvector_private.h
 * @brief obvector Private Interface
 * @author theck
 */

#ifndef OBVECTOR_PRIVATE_H
#define OBVECTOR_PRIVATE_H

#include "../obvector.h"

/* DATA */

/**
 * @brief obvector internal structure, encapsulating all data needed for
 * an instance of obvector
 */
struct obvector_struct{
  obj base; /**< obj containing reference count and class membership data */
  obj **array; /**< intenal dynamically sized array of pointers to Offbrand
                    compatible class instances */
  uint32_t length; /**< Integer size to find all objects stored in Vector */
  uint32_t capacity; /**< Integer count of the capacity of the internal array */
};


/* PRIVATE METHODS */

/**
 * @brief Create the default obvector
 * @param initial_capacity Capacity of the vector to be created
 * @return A new, partially initialized instance of obvector
 */
obvector * obvector_create_default(uint32_t initial_capacity);

/**
 * @brief Resizes a vector if the number of objects it contains is equal to its
 * capacity by doubling the potential capacity of the vector
 * @param v Pointer to an instance of obvector
 * @param index Index that vector must be resized to contain
 */
void obvector_resize(obvector *v, uint32_t index);

/**
 * @brief Internal merge sort implementation for an obvector
 *
 * @param to_sort Primitive array of objects to be sorted
 * @param size Size of to_sort
 * @param order Accepts OB_LEAST_TO_GREATEST or OB_GREATEST_TO_LEAST as valid
 * sorting orders
 * @param funct A compare_fptr to a function that returns an int8_t when given
 * two obj * arguments
 *
 * @return The sorted primitive array of objects (a new primitive array, not
 * to_sort)
 *
 * @warning There is little to no parameter checking in this function, all
 * sorting should use the publicly accessable function which calls this method
 * internally.
 */
obj ** obvector_recursive_sort(obj **to_sort, uint32_t size, int8_t order,
                               ob_compare_fptr funct);

/**
 * @brief Hash function for obvector
 * @param to_hash An obj pointer to an instance of obvector
 * @return Key value (hash) for the given obj pointer to an obvector
 */
ob_hash_t obvector_hash(const obj *to_hash);

/**
 * @brief Compares two instances of obvector
 *
 * @param a A non-NULL obj pointer to type obvector
 * @param b A non-NULL obj pointer to type obvector
 *
 * @retval OB_NOT_EQUAL a does not equal b
 * @retval OB_EQUAL_TO a equals b
 */
int8_t obvector_compare(const obj *a, const obj *b);

/**
 * @brief Display function for an instance of OBString
 *
 * @param to_print A non-NULL obj pointer to type OBString
 */
void obvector_display(const obj *to_print);

/**
 * @brief Destructor for obvector
 * @param to_dealloc An obj pointer to an instance of obvector with
 * reference count of 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void obvector_destroy(obj *to_dealloc);

/* PRIVATE UTILITY METHODS */

/**
 * @brief Searches an array of obj for the first encountered non-NULL pointer,
 * returning the index where this pointer is found
 *
 * @param array Array of pointers to instances of Offbrand compatible classes
 * @param index Index from which to begin searching
 *
 * @retval <UINT32_MAX Index where a non-NULL pointer was found
 * @retval UINT32_MAX No non-NULL pointer was found
 */
uint32_t obvector_find_valid_precursor(obj **array, uint32_t index);

#endif
