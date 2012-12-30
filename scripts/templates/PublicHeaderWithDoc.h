/**
 * @file %CODECLASSNAME%.h
 * @author %CLASSAUTHOR%
 * @brief %CODECLASSNAME% Public Interface
 */

#ifndef %MACROCLASSNAME%_H
#define %MACROCLASSNAME%_H

#include "offbrand.h"

/* Class type declaration */
typedef struct %CODECLASSNAME%_struct %CODECLASSNAME%;


/* PUBLIC METHODS */

/**
 * @brief Compares two instances of %CODECLASSNAME%
 *
 * @param a A non-NULL obj pointer to type %CODECLASSNAME%
 * @param b A non-NULL obj pointer to type %CODECLASSNAME%
 *
 * @retval -1 obj a is less than b
 * @retval 0 obj a is equivalent to b
 * @retval 1 obj a is greater than b
 */
int8_t compare%CODECLASSNAME%s(const obj *a, const obj *b);
/* Arguments are obj * so that a function pointer can be used for container
 * class sorting/search
 * DEFINE AT LEAST ONE PUBLIC CONSTRUCTOR METHOD THAT CALLS PRIVATE DEFAULT
 * CONSTRUCTOR */

/* DECLARE ADDITIONAL PUBLIC METHODS HERE */

#endif

