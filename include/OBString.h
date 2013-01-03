/**
 * @file OBString.h
 * @brief OBString Public Interface
 * @author theck
 */

#ifndef OBSTRING_H
#define OBSTRING_H

#include "offbrand.h"

/* Class type declaration */
typedef struct OBString_struct OBString;


/* PUBLIC METHODS */

/**
 * @brief Compares two instances of OBString
 *
 * @param a A non-NULL obj pointer to type OBString
 * @param b A non-NULL obj pointer to type OBString
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t compareStrings(const obj *a, const obj *b);
/* Arguments are obj * so that a function pointer can be used for container
 * class sorting/search
 * DEFINE AT LEAST ONE PUBLIC CONSTRUCTOR METHOD THAT CALLS PRIVATE DEFAULT
 * CONSTRUCTOR */

/* DECLARE ADDITIONAL PUBLIC METHODS HERE */

#endif

