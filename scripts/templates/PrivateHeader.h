
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

/* function performs all initialization on new instances %CODECLASSNAME%, MUST
 * BE CALLED IMMEDIATELY AFTER INSTANTIATION. This function may be called by a
 * default constructor that all other constructors build off of */
uint8_t init%CODECLASSNAME%Base(%CODECLASSNAME% *to_init);

/* deallocator, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void dealloc%CODECLASSNAME%(obj *to_dealloc);

/*PRIVATE METHOD DECLARATIONS HERE*/

#endif
