
/*
 * Private header for %CODECLASSNAME%
 */

#ifndef %MACROCLASSNAME%_PRIVATE_H
#define %MACROCLASSNAME%_PRIVATE_H

#include "%CODECLASSNAME%.h"

/* DATA */

struct %CODECLASSNAME%_struct{
  OBObj base; /*always first member of struct, always statically allocated*/
  /* Additional private data added here, MUST COME AFTER THE SBObj */
};


/* PRIVATE METHODS */

/* destructor, frees instance of class back to memory. Should not be called
 * manually, instance will be destroyed when reference count reaches 0 */
void destroy%CODECLASSNAME%(%CODECLASSNAME% *to_destroy);

/*PRIVATE METHOD DECLARATIONS HERE*/

#endif
