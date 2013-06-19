/**
 * @file obvector.h
 * @brief obvector Public Interface
 * @author theck
 */

#ifndef OBVECTOR_H
#define OBVECTOR_H

#include "offbrand.h"

/** Class type declaration */
typedef struct obvector_struct obvector;


/* PUBLIC METHODS */

/**
 * @brief Constructor, creates a new instance of obvector with a given initial
 * capacity
 *
 * @param initial_capacity Integer size for the vector capacity
 *
 * @return Pointer to the newly created vector
 */
obvector * obvector_new(uint32_t initial_capacity);

/**
 * @brief Copy Constructor, creates a new obvector that is a copy of an instance
 * of another obvector.
 *
 * @details The new copy is a shallow copy, it references the same obj pointers
 * rather that creating unique copies of each contained obj
 *
 * @param to_copy The obvector instance to be copied
 * @return A new instance of obvector that is a shallow copy of to_copy
 */
obvector * obvector_copy(const obvector *to_copy);

/**
 * @brief Number of elements encompassed within the obvector
 *
 * @param v A pointer to an instance of obvector
 *
 * @return An integer corresponding to the length required to span all elements
 * contained within the vector (including all NULL elements added by the user)
 */
uint32_t obvector_length(const obvector *v);

/**
 * @brief Stores the obj at the associated index in a vector, overwriting
 * previous obj stored at that index and resizing the vector as needed
 *
 * @param v A pointer to an instance of obvector
 * @param to_add A pointer to any Offbrand compativle class instance
 * @param index An integer index that may be positive to index from the
 * beginning of the vector or negative to index from the end of the vector
 * (where index = -x associates to element at [length of v] - x)
 *
 * @details Storing NULL at an index in the vector is equivalent to removing
 * that object from the vector (if it only is found in that single vector
 * positition). Storing NULL beyond vector length has no effect on vector
 * length
 *
 * @warning Positive indexing can occur past vector length, negative indexing
 * is limited to the range [-1, -(length of v)]
 */
void obvector_store_at_index(obvector *v, obj *to_add, int64_t index);

/**
 * @brief Accesses the Offbrand compatile class instance stored an index in an
 * obvector
 *
 * @param v A pointer to an instance of obvector
 * @param index An integer index that may be positive to index from the
 * beginning of the vector or negative to index from the end of the vector
 * (where index = -x associates to element at [size of v] - x)
 *
 * @retval NULL When index is out of range of obvector
 * @retval obj* When index is in obvector range
 *
 * @warning Do not call release on returned object unless the calling
 * code already had a reference to the object before calling objAtVectorIndex
 * that it wishes to relenquish
 * @warning Positive indexing can occur past vector length, negative indexing
 * is limited to the range [-1, -(length of v)]
 */
obj * obvector_obj_at_index(const obvector *v, int64_t index);

/**
 * @brief Adds the contents of on vector to the end of another, concatenating
 * the two.
 *
 * @param destination obvector that will be extended with the contents of
 * to_append
 * @param to_append obvector whos contents will be added to the end of
 * destination
 */
void obvector_concat(obvector *destination, obvector *to_append);

/**
 * @brief Searches for an instance of any Offbrand compatible class in an
 * obvector using a comparision function
 *
 * @param v A pointer to an instance of obvector
 * @param to_find A pointer to an instance of any Offbrand compatible class
 *
 * @retval 0 to_find was not found in the obvector
 * @retval 1 to_find exists in the obvector
 *
 * @warning Specify NULL as the comparision function if the given obvector is
 * known to contain instances of may different classes else the function will
 * likely cause the program to be aborted
 */
uint8_t obvector_find_obj(const obvector *v, const obj *to_find);

/**
 * @brief Sorts an obvector from least-to-greatest or greatest-to-least using
 * the standard compare function
 *
 * @param v A pointer to an instance of obvector
 * @param order Accepts OB_LEAST_TO_GREATEST or OB_GREATEST_TO_LEAST as valid
 * sorting orders
 *
 * @warning If called on an obvector containing instances of multiple Offbrand
 * classes the call will likely not sort members properly but will still likely
 * reorder internal contents
 * @warning Sorting may appear to shrink vector as NULL values interspersed with
 * valid objects will be consolidated and removed
 */
void obvector_sort(obvector *v, int8_t order);

/**
 * @brief Sorts an obvector from least-to-greatest or greatest-to-least using a
 * specified comparision function
 *
 * @param v A pointer to an instance of obvector
 * @param order Accepts OB_LEAST_TO_GREATEST or OB_GREATEST_TO_LEAST as valid
 * sorting orders
 * @param funct A compare_fptr to a function that returns an int8_t when given
 * two obj * arguments
 *
 * @warning Sorting may appear to shrink vector as NULL values interspersed with
 * valid objects will be consolidated and removed
 */
void obvector_sort_with_funct(obvector *v, int8_t order, ob_compare_fptr funct);

/**
 * @brief Removes all objects from an obvector, leaving it empty
 * @param v A pointer to an instance of obvector
 */
void obvector_clear(obvector *v);

#endif

