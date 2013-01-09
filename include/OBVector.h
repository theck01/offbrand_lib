/**
 * @file OBVector.h
 * @brief OBVector Public Interface
 * @author theck
 */

#ifndef OBVECTOR_H
#define OBVECTOR_H

#include "offbrand.h"

/* Class type declaration */
typedef struct OBVector_struct OBVector;


/* PUBLIC METHODS */

/**
 * @brief Constructor, creates a new instance of OBVector with a given initial
 * capacity
 * @param initial_capacity Integer size for the vector capacity
 * @return Pointer to the newly created vector
 */
OBVector * createVector(uint32_t initial_capacity);

/**
 * @brief Copy Constructor, creates a new OBVector that is a copy of an instance
 * of another OBVector.
 *
 * @details The new copy is a shallow copy, it references the same obj pointers
 * rather that creating unique copies of each contained obj
 *
 * @param to_copy The OBVector instance to be copied
 * @return A new instance of OBVector that is a shallow copy of to_copy
 */
OBVector * copyVector(const OBVector *to_copy); 

/**
 * @brief Gets the number of elements contained within an OBVector instance
 * @param v A pointer to an instance of OBVector
 * @return An integer size corresponding to the number of elements contained
 * within the vector
 */
uint32_t sizeOfVector(const OBVector *v);

/**
 * @brief Resizes an OBVector instance with a capacity equaling the number of
 * elements contained within.
 * @param v A pointer to an instance of OBVector
 */
void fitVectorToContents(OBVector *v); 

/**
 * @brief Adds an instance of any Offbrand compatible class to the end of an
 * OBVector, increasing its size by one.
 * @param v A pointer to an instance of OBVector
 * @param to_add A pointer to any Offbrand compativle class instance
 */
void addToVector(OBVector *v, obj *to_add);

/**
 * @brief Adds an instance of any Offbrand compatible class to the index,
 * shifting all obj at and after that index to index+1
 *
 * @param v A pointer to an instance of OBVector
 * @param to_add A pointer to any Offbrand compativle class instance
 * @param index An integer index that may be positive to index from the
 * beginning of the vector or negative to index from the end of the vector
 * (where index = -x associates to element at [size of v] - x)
 *
 * @warning If the magnitude of the index is greater than the number of items
 * in the vector then to_add will be added at the end of the vector (if index is
 * positive) or at the beginning of the vector (if index is negative) rather
 * than at the specified index to maintain vector density
 */
void insertAtVectorIndex(OBVector *v, obj *to_add, int64_t index);

/**
 * @brief Adds the contents of on vector to the end of another, concatenating
 * the two.
 *
 * @param destination OBVector that will be extended with the contents of
 * to_append
 * @param to_append OBVector whos contents will be added to the end of 
 * destination
 */
void catVectors(OBVector *destination, OBVector *to_append);

/**
 * @brief Replaces the instance of any Offbrand compatible class at the index,
 * with a new instance of an Offbrand compatible class
 *
 * @param v A pointer to an instance of OBVector
 * @param new_obj A pointer to any Offbrand compativle class instance
 * @param index An integer index that may be positive to index from the
 * beginning of the vector or negative to index from the end of the vector
 * (where index = -x associates to element at [size of v] - x)
 *
 * @warning If the magnitude of the index is greater than the number of items
 * in the vector then the function will silently to nothing.
 */
void replaceInVector(OBVector *v, obj *new_obj, int64_t index);

/**
 * @brief Accesses the Offbrand compatile class instance stored an index in an
 * OBVector
 *
 * @param v A pointer to an instance of OBVector
 * @param index An integer index that may be positive to index from the
 * beginning of the vector or negative to index from the end of the vector
 * (where index = -x associates to element at [size of v] - x)
 *
 * @retval NULL When index is out of range of OBVector
 * @retval obj* When index is in OBVector range
 *
 * @warning Do not call release on returned object unless the calling
 * code already had a reference to the object before calling objAtVectorIndex
 * that it wishes to relenquish
 */
obj * objAtVectorIndex(const OBVector *v, int64_t index);

/**
 * @brief Searches for an instance of any Offbrand compatible class in an
 * OBVector using a comparision function
 *
 * @param v A pointer to an instance of OBVector
 * @param to_find A pointer to an instance of any Offbrand compatible class
 * @param compare A function pointer to a comparision function with signature
 * matching the compare_fptr type. If NULL then default pointer comparisions are
 * used
 *
 * @retval 0 to_find was not found in the OBVector
 * @retval 1 to_find exists in the OBVector
 *
 * @warning Specify NULL as the comparision function if the given OBVector is
 * known to contain instances of may different classes else the function will
 * likely cause the program to be aborted
 */
uint8_t findObjInVector(const OBVector *v, const obj *to_find);

/**
 * @brief Sorts an OBVector from least-to-greatest or greatest-to-least using
 * the standard compare function
 *
 * @param v A pointer to an instance of OBVector
 * @order Accepts OB_LEAST_TO_GREATEST or OB_GREATEST_TO_LEAST as valid sorting
 * orders
 *
 * @warning If called on an OBVector containing instances of multiple Offbrand
 * classes the call will likely not sort members properly but will still likely
 * reorder internal contents
 */
void sortVector(OBVector *v, int8_t order);

/**
 * @brief Sorts an OBVector from least-to-greatest or greatest-to-least using a
 * specified comparision function 
 *
 * @param v A pointer to an instance of OBVector
 * @order Accepts OB_LEAST_TO_GREATEST or OB_GREATEST_TO_LEAST as valid sorting
 * orders
 * @param funct A compare_fptr to a function that returns an int8_t when given 
 * two obj * arguments
 */
void sortVectorWithFunct(OBVector *v, int8_t order, compare_fptr funct);

/**
 * @brief Removes the object at the end of the vector, doing nothing if the
 * vector is already empty
 * @param v A pointer to an instance of OBVector
 */
void removeFromVectorEnd(OBVector *v);

/**
 * @brief Removes the object an index in an OBVector, shifting all objects
 * at indecies greater than index by -1 and doing nothing if the vector is empty
 *
 * @param v A pointer to an instance of OBVector
 * @param index An integer index that may be positive to index from the
 * beginning of the vector or negative to index from the end of the vector
 * (where index = -x associates to element at [size of v] - x)
 *
 * @warning If the magnitude of the index is greater than the number of items
 * in the vector then the function will silently to nothing.
 */
void removeFromVectorIndex(OBVector *v, int64_t index);

/**
 * @brief Removes all objects from an OBVector, leaving it empty
 * @param v A pointer to an instance of OBVector
 */
void clearVector(OBVector *v);

#endif

