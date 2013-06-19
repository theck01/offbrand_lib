/**
 * @file %CLASSNAME%_Private.h
 * @brief %CLASSNAME% Private Interface
 * @author %CLASSAUTHOR%
 */

#ifndef %MACRONAME%_PRIVATE_H
#define %MACRONAME%_PRIVATE_H

#include "../%CLASSNAME%.h"

/* DATA */

/**
 * @brief %CLASSNAME% internal structure, encapsulating all data needed for
 * an instance of %CLASSNAME%
 */
struct %CLASSNAME%_struct{
  obj base; /**< obj containing reference count and class membership data */
  /* Additional private data added here, MUST COME AFTER THE obj */
};


/* PRIVATE METHODS */

/**
 * @brief Default constructor for %CLASSNAME%
 * @return An instance of class %CLASSNAME%
 * @warning All public constructors should call this constructor and intialize
 * individual members as needed, so that all base data is initialized properly.
 */
%CLASSNAME% * %CLASSNAME%_create_default(void);

/**
 * @brief Hash function for %CLASSNAME%
 * @param to_hash An obj pointer to an instance of %CLASSNAME%
 * @return Key value (hash) for the given obj pointer to a %CLASSNAME%
 */
ob_hash_t %CLASSNAME%_hash(const obj *to_hash);

/**
 * @brief Compares two instances of %CLASSNAME%
 *
 * @param a A non-NULL obj pointer to type %CLASSNAME%
 * @param b A non-NULL obj pointer to type %CLASSNAME%
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t %CLASSNAME%_compare(const obj *a, const obj *b);
/* Arguments are obj * so that a function pointer can be used for container
 * class sorting/search */

/**
 * @brief Descriptor for an instance of %CLASSNAME%, prints relevant
 * information about the class to stderr
 *
 * @param to_print A non-NULL obj pointer to an instance of type
 * %CLASSNAME%
 */
void %CLASSNAME%_display(const obj *to_print);

/**
 * @brief Destructor for %CLASSNAME%
 * @param to_dealloc An obj pointer to an instance of %CLASSNAME% with
 * reference count of 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void %CLASSNAME%_destroy(obj *to_dealloc);

/* ADDITIONAL PRIVATE METHOD DECLARATIONS HERE*/

#endif

