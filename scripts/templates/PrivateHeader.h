/**
 * @file %CODECLASSNAME%_Private.h
 * @brief %CODECLASSNAME% Private Interface
 * @author %CLASSAUTHOR%
 */

#ifndef %MACROCLASSNAME%_PRIVATE_H
#define %MACROCLASSNAME%_PRIVATE_H

#include "../%CODECLASSNAME%.h"

/* DATA */

/**
 * @brief %CODECLASSNAME% internal structure, encapsulating all data needed for
 * an instance of %CODECLASSNAME%
 */
struct %CODECLASSNAME%_struct{
  obj base; /**< obj containing reference count and class membership data */
  /* Additional private data added here, MUST COME AFTER THE obj */
};


/* PRIVATE METHODS */

/**
 * @brief Default constructor for %CODECLASSNAME%
 * @return An instance of class %CODECLASSNAME%
 * @warning All public constructors should call this constructor and intialize
 * individual members as needed, so that all base data is initialized properly.
 */
%CODECLASSNAME% * createDefault%CODECLASSNAME%(void);

/**
 * @brief Hash function for %CODECLASSNAME%
 * @param to_hash An obj pointer to an instance of %CODECLASSNAME%
 * @return Key value (hash) for the given obj pointer to a %CODECLASSNAME%
 */
obhash_t hash%CODECLASSNAME%(const obj *to_hash);

/**
 * @brief Compares two instances of %CODECLASSNAME%
 *
 * @param a A non-NULL obj pointer to type %CODECLASSNAME%
 * @param b A non-NULL obj pointer to type %CODECLASSNAME%
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t compare%CODECLASSNAME%s(const obj *a, const obj *b);
/* Arguments are obj * so that a function pointer can be used for container
 * class sorting/search */

/** 
 * @brief Destructor for %CODECLASSNAME%
 * @param to_dealloc An obj pointer to an instance of %CODECLASSNAME% with
 * reference count of 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void dealloc%CODECLASSNAME%(obj *to_dealloc);

/* ADDITIONAL PRIVATE METHOD DECLARATIONS HERE*/

#endif

