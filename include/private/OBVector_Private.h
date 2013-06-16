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
  OBObjType base; /**< obj containing reference count and class membership data */
  OBObjType **array; /**< intenal dynamically sized array of pointers to Offbrand
                    compatible class instances */
  uint32_t length; /**< Integer size to find all objects stored in Vector */
  uint32_t capacity; /**< Integer count of the capacity of the internal array */
};


/* PRIVATE METHODS */

/**
 * @brief Create the default OBVector
 * @param initial_capacity Capacity of the vector to be created
 * @return A new, partially initialized instance of OBVector
 */
OBVector * createDefaultVector(uint32_t initial_capacity);

/**
 * @brief Resizes a vector if the number of objects it contains is equal to its
 * capacity by doubling the potential capacity of the vector
 * @param v Pointer to an instance of OBVector
 * @param index Index that vector must be resized to contain
 */
void resizeVector(OBVector *v, uint32_t index);

/**
 * @brief Internal merge sort implementation for an OBVector
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
OBObjType ** recursiveSortContents(OBObjType **to_sort, uint32_t size, int8_t order,
                             obcompare_fptr funct);

/**
 * @brief Hash function for OBVector
 * @param to_hash An obj pointer to an instance of OBVector
 * @return Key value (hash) for the given obj pointer to an OBVector
 */
obhash_t hashVector(const OBObjType *to_hash);

/**
 * @brief Compares two instances of OBVector
 *
 * @param a A non-NULL obj pointer to type OBVector
 * @param b A non-NULL obj pointer to type OBVector
 *
 * @retval OB_NOT_EQUAL a does not equal b
 * @retval OB_EQUAL_TO a equals b
 */
int8_t compareVectors(const OBObjType *a, const OBObjType *b);

/**
 * @brief Display function for an instance of OBString
 *
 * @param to_print A non-NULL obj pointer to type OBString
 */
void displayVector(const OBObjType *to_print);

/** 
 * @brief Destructor for OBVector
 * @param to_dealloc An obj pointer to an instance of OBVector with
 * reference count of 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void deallocVector(OBObjType *to_dealloc);

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
uint32_t findValidPrecursorIndex(OBObjType **array, uint32_t index);

#endif
