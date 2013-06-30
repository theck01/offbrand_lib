/**
 * @file offbrand.h
 * @brief offbrand Standard Library
 *
 * @details
 * The standard library defines all data structures and function calls
 * required for memory management via reference count, class membership tests,
 * hashing, and others.
 *
 * @author theck
 */

#ifndef OFFBRAND_LIB
#define OFFBRAND_LIB

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <time.h>

/** Comparison Constant, two elements are equal */
#define OB_EQUAL_TO 0
/** Comparison Constant, the first element is greater than the second */
#define OB_GREATER_THAN 1
/** Comparison Constant, the first element is less than the second */
#define OB_LESS_THAN -1
/** Comparison Constant, the first element is not equal to the second, and no
 * other relation ship can be infered */
#define OB_NOT_EQUAL 2

/** Sorting Order Constant, sort elements from least to greatest*/
#define OB_LEAST_TO_GREATEST -1
/** Sorting Order Constant, sort elements from greatest to least*/
#define OB_GREATEST_TO_LEAST 1

/**
 * basic generic type used to track reference counts, store class specific
 * function pointers, and form the basis for all generic container classes
 * and functions.
 */
typedef struct obj_struct * obj;

/**
 * reference count, tracks references to instances of offbrand compatible
 * classes
 */
typedef uint32_t ob_ref_count_t;

/** function pointer to a deallocator for any OffBrand compatible class */
typedef void (*ob_dealloc_fptr)(obj *);

/** hash value, used returned from hash functions */
typedef size_t ob_hash_t;

/** function pointer to a hash function for any offbrand compatible class */
typedef ob_hash_t (*ob_hash_fptr)(const obj *);

/**
 * function pointer to a comparision function that takes pointers to any two
 * offbrand compatible classes and returns one of the comparision constants
 * listed in the constants section.
 */
typedef int8_t (*ob_compare_fptr)(const obj *, const obj *);

/** function pointer to a display function for any offbrand compatible class */
typedef void (*ob_display_fptr)(const obj *);


/* OFFBRAND STANDARD LIB */

/**
 * @brief Initializes instances of all offbrand compatible classes with a
 * reference count of 1, a deallocator, a hash function, and the provided class
 * name
 *
 * @param instance An newly allocated instance of any offbrand compatible class
 * @param dealloc_funct Function pointer to the deallocator for the instances
 * class
 * @param hash_funct Function pointer to the hash function for the instances
 * class
 * @param compare_funct Function pointer to the compare function for the
 * instances class
 * @param display_funct Function pointer to the display function for the
 * instances class
 * @param classname C string containing instances classname.
 */
void ob_init_base(obj *instance, ob_dealloc_fptr dealloc_funct,
                  ob_hash_fptr hash_funct, ob_compare_fptr compare_funct,
                  ob_display_fptr display_funct, const char *classname);

/**
 * @brief Decrements the instances reference count by 1. If the reference count
 * is reduced to 0 then release automatically calls the instances deallocator.
 *
 * @param instance An instance of any offbrand compatible class
 * @retval &instance Reference count was decremented but the instance is still
 * referenced
 * @retval NULL Reference count was decremented to 0 and deallocator was called
 * on instance
 */
obj * ob_release(obj *instance);

/**
 * @brief Increments the instances reference count by 1, indicating that the
 * calling code has "referenced" that instance for later use.
 *
 * @param instance An instance of any offbrand compatible class
 *
 * @return A reference to the argument instance.
 */
obj * ob_retain(obj *instance);

/**
 * @brief Returns the current reference count of the given instance.
 *
 * @param instance An instance of any offbrand compatible class
 * @return An unsigned integer reference count
 */
uint32_t ob_reference_count(obj *instance);

/**
 * @brief Checks that the given obj is of the provided class
 *
 * @param a An instance of any offbrand compatible class
 * @param classname An C string describing a class name
 *
 * @retval 0 a is not an instance of classname
 * @retval non-zero a is an instance of classname
 */
uint8_t ob_has_class(const obj *a, const char *classname);

/**
 * @brief Checks that two objs are of the same class
 *
 * @param a An instance of any offbrand compatible class
 * @param b An instance of any offbrand compatible class
 *
 * @retval 0 a and b are not of the same class
 * @retval non-zero a and b are of the same class
 */
uint8_t ob_has_same_class(const obj *a, const obj *b);

/**
 * @brief Computes the hash value of the instance using a class specific hash
 * function if one is available or the defaultHash function if not
 *
 * @param to_hash An instance of any offbrand compatible class
 * @return Hash value
 */
ob_hash_t ob_hash(const obj *to_hash);

/**
 * @brief comparision operator between any two offbrand compatible classes
 *
 * @param a An instance of any offbrand compatible class
 * @param b An instance of any offbrand compatible class
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 * @retval OB_NOT_EQUAL obj a is not equal to b, but no other relationship can
 * be infered
 */
int8_t ob_compare(const obj *a, const obj *b);

/**
 * @brief display operation prints information about the instance of any
 * offbrand compatible class to stderr, for debug
 *
 * @param to_print An instance of any offbrand compativle class
 */
void ob_display(const obj *to_print);

#endif

