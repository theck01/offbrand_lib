/**
 * @file OBVector_Private.h
 * @brief OBVector Private Interface
 * @author theck
 */

#ifndef OBVECTOR_PRIVATE_H
#define OBVECTOR_PRIVATE_H

#include "../OBVector.h"

/* DATA */

/**
 * @brief OBVector internal structure, encapsulating all data needed for
 * an instance of OBVector
 */
struct OBVector_struct{
  obj base; /**< obj containing reference count and class membership data */
  obj **array; /**< intenal dynamically sized array of pointers to Offbrand
                    compatible class instances */
  uint32_t num_objs; /**< Integer count of the number of contained objects */
  uint32_t capacity; /**< Integer count of the capacity of the internal array */
};


/* PRIVATE METHODS */

/**
 * @brief Resizes a vector if the number of objects it contains is equal to its
 * capacity by doubling the potential capacity of the vector
 * @param v Pointer to an instance of OBVector
 */
void resizeVector(OBVector *v);

/**
 * @brief Internal merge sort implementation for an OBVector
 *
 * @param to_sort Primitive array of objects to be sorted
 * @param size Size of to_sort
 * @param compare A function pointer to a comparision function with signature
 * matching the compare_fptr type. If NULL then default pointer comparisions are
 * used
 * @order Accepts OB_LEAST_TO_GREATEST or OB_GREATEST_TO_LEAST as valid sorting
 * orders
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
obj ** recursiveSortContents(obj **to_sort, uint32_t size, int8_t order,
                             compare_fptr funct);

/**
 * @brief Hash function for OBVector
 * @param to_hash An obj pointer to an instance of OBVector
 * @return Key value (hash) for the given obj pointer to an OBVector
 */
obhash_t hashVector(const obj *to_hash);

/**
 * @brief Compares two instances of OBVector
 *
 * @param a A non-NULL obj pointer to type OBVector
 * @param b A non-NULL obj pointer to type OBVector
 *
 * @retval OB_NOT_EQUAL a does not equal b
 * @retval OB_EQUAL_TO a equals b
 */
int8_t compareVectors(const obj *a, const obj *b);

/** 
 * @brief Destructor for OBVector
 * @param to_dealloc An obj pointer to an instance of OBVector with
 * reference count of 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void deallocVector(obj *to_dealloc);

#endif
