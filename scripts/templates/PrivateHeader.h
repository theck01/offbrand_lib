
/*
 * Private header for %CODECLASSNAME%
 */

#ifndef %MACROCLASSNAME%_PRIVATE_H
#define %MACROCLASSNAME%_PRIVATE_H

#include "../%CODECLASSNAME%.h"

/* DATA */

struct %CODECLASSNAME%_struct{
  obj base;
  /* Additional private data added here, MUST COME AFTER THE obj */
};


/* PRIVATE METHODS */

/* default constructor allocates and initializes an instance of %CODECLASSNAME%.
 * ALL OTHER CONSTRUCTOR METHODS SHOULD CALL THIS DEFAULT CONSTRUCTOR, which
 * should set up a bare bones instance of the class that others will initialize.
 * Ensures base obj is properly initialized.
 * Add additional arguments as needed */
%CODECLASSNAME% * createDefault%CODECLASSNAME%(void);

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void dealloc%CODECLASSNAME%(obj *to_dealloc);

/*PRIVATE METHOD DECLARATIONS HERE*/

#endif
