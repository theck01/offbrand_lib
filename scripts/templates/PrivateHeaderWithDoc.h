/**
 * @file %CODECLASSNAME%_Private.h
 * @author %CLASSAUTHOR%
 * @brief %CODECLASSNAME% Private Interface
 */

#ifndef %MACROCLASSNAME%_PRIVATE_H
#define %MACROCLASSNAME%_PRIVATE_H

#include "../%CODECLASSNAME%.h"

/* DATA */

/**
 * @brief Internal structure encapsulating all data needed for %CODECLASSNAME%
 */
struct %CODECLASSNAME%_struct{
  obj base; /**< obj containing reference count and class membership data */
  /* Additional private data added here, MUST COME AFTER THE obj */
};


/* PRIVATE METHODS */

/* default constructor allocates and initializes an instance of %CODECLASSNAME%.
 * ALL OTHER CONSTRUCTOR METHODS SHOULD CALL THIS DEFAULT CONSTRUCTOR, which
 * should set up a bare bones instance of the class that others will initialize.
 * Ensures base obj is properly initialized.
 * Add additional arguments as needed */

/**
 * @brief Default constructor for %CODECLASSNAME%
 * @return An instance of class %CODECLASSNAME%
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

