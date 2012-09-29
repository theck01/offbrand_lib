
/*
 * OBDeque:
 * [Class description]
 */

#ifndef OBDEQUE_H
#define OBDEQUE_H

#include "offbrand.h"

/* Class type declaration */
typedef struct OBDeque_struct OBDeque;


/* PUBLIC METHODS */

/* DEFINE AT LEAST ONE PUBLIC CONSTRUCTOR METHOD THAT CALLS PRIVATE DEFAULT
 * CONSTRUCTOR */

/* compare two instances of OBDeque, return 1 if a > b, 0 if a == b, 
 * and -1 if a < b */
/* Arguments are obj * so that a function pointer can be used for container
 * class sorting/search */
int8_t compareOBDeques(const obj *a, const obj *b);

/* DECLARE ADDITIONAL PUBLIC METHODS HERE */

#endif

