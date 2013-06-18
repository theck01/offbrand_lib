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
 * @author theck, danhd123/icodestuff
 */

#ifndef OBJ_PRIVATE_H
#define OBJ_PRIVATE_H

#include "../offbrand.h"

/**
 * @brief Base struct used for within all Offbrand compatible classes that
 * tracks information common to all classes.
 */
struct OBObjStruct{
  obref_count_t references; /**< reference count for each instance */
  obdealloc_fptr dealloc; /**< pointer to class specific deallocation function */
  obhash_fptr hash; /**< pointer to class specific hash function */
  obcompare_fptr compare; /**< pointer to class specific comparison function */
  obdisplay_fptr display; /**< pointer to the class specific display function */
  const char *classname; /**< C String classname to which the instance 
                           belongs */
};

#endif
