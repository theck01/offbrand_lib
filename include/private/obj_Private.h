/**
 * @file obj_Private.h
 * @brief Base type for all Offbrand compatible classes.
 *
 * @details
 * The obj type is used to track reference counts, class specific function 
 * pointers, and class membership. Each Offbrand compatible class includes an
 * instance of obj as it's base member so that functions can operate generically
 * on all Offbrand classes as if they were objs
 *
 * @author theck
 */

#ifndef OBJ_PRIVATE_H
#define OBJ_PRIVATE_H

#include "../offbrand.h"

/**
 * @brief Base struct used for within all Offbrand compatible classes that
 * tracks information common to all classes.
 */
struct obj_struct{
  ref_count_t references; /**< reference count for each instance */
  dealloc_fptr dealloc; /**< pointer to class specific deallocation function */
  hash_fptr hash; /**< pointer to class specific hash function */
  const char *classname; /**< C String classname to which the instance belongs */
};

/**
 * @brief Computes the hash value of the instance using a hash function that
 * access only common attributes to all Offbrand compatible class instances.
 *
 * @param to_hash An instance of any Offbrand compatible class
 * @return Hash value
 */
obhash_t defaultHash(const obj *to_hash);

#endif
