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
 * @brief Destructor for %CODECLASSNAME%
 * @param to_dealloc An obj pointer to an instance of %CODECLASSNAME%
 * @warning Do not call manually, release will call automatically when the instances reference count drops to 0!
 */
void dealloc%CODECLASSNAME%(obj *to_dealloc);

/* ADDITIONAL PRIVATE METHOD DECLARATIONS HERE*/

#endif

