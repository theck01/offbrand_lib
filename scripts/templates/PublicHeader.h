
/*
 * %CODECLASSNAME%:
 * [Class description]
 */

#ifndef %MACROCLASSNAME%_H
#define %MACROCLASSNAME%_H

#include "offbrand.h"

/* Class type declaration */
typedef struct %CODECLASSNAME%_struct %CODECLASSNAME%;


/* PUBLIC METHODS */

/* constructor allocates and initializes an instance of %CODECLASSNAME% */
/* expand with additional arguments as needed */
%CODECLASSNAME% * create%CODECLASSNAME%(void);

/* compare two instances of %CODECLASSNAME%, return 1 if a > b, 0 if a == b, 
 * and -1 if a < b */
int8_t compare%CODECLASSNAME%(const obj *a, const obj *b);

/* DECLARE ADDITIONAL PUBLIC METHODS HERE */

#endif

