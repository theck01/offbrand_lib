/**
 * @file obj_private.h
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
  ob_ref_count_t references; /**< reference count for each instance */
  ob_dealloc_fptr dealloc; /**< pointer to class specific deallocation
                                function */
  ob_hash_fptr hash; /**< pointer to class specific hash function */
  ob_compare_fptr compare; /**< pointer to class specific comparison
                                function */
  ob_display_fptr display; /**< pointer to the class specific display
                                function */
  const char *classname; /**< C String classname to which the instance
                              belongs */
};

#endif
