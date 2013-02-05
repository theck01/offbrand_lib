/**
 * @file OBInt_Private.h
 * @brief OBInt Private Interface
 * @author theck
 */

#ifndef OBINT_PRIVATE_H
#define OBINT_PRIVATE_H

#include "../OBInt.h"

/* DATA */

/**
 * @brief OBInt internal structure, encapsulating all data needed for
 * an instance of OBInt
 */
struct OBInt_struct{
  obj base; /**< obj containing reference count and class membership data */
  /* Additional private data added here, MUST COME AFTER THE obj */
};


/* PRIVATE METHODS */

/**
 * @brief Default constructor for OBInt
 * @return An instance of class OBInt
 * @warning All public constructors should call this constructor and intialize
 * individual members as needed, so that all base data is initialized properly.
 */
OBInt * createDefaultInt(void);

/**
 * @brief Hash function for OBInt
 * @param to_hash An obj pointer to an instance of OBInt
 * @return Key value (hash) for the given obj pointer to a OBInt
 */
obhash_t hashInt(const obj *to_hash);

/**
 * @brief Compares two instances of OBInt
 *
 * @param a A non-NULL obj pointer to type OBInt
 * @param b A non-NULL obj pointer to type OBInt
 *
 * @retval OB_LESS_THAN obj a is less than b
 * @retval OB_GREATER_THAN obj a is equivalent to b
 * @retval OB_EQUAL_TO obj a is greater than b
 */
int8_t compareInts(const obj *a, const obj *b);
/* Arguments are obj * so that a function pointer can be used for container
 * class sorting/search */

/**
 * @brief Descriptor for an instance of OBInt, prints relevant
 * information about the class to stderr
 *
 * @param to_print A non-NULL obj pointer to an instance of type
 * OBInt
 */
void displayInt(const obj *to_print);

/** 
 * @brief Destructor for OBInt
 * @param to_dealloc An obj pointer to an instance of OBInt with
 * reference count of 0
 * @warning Do not call manually, release will call automatically when the
 * instances reference count drops to 0!
 */
void deallocInt(obj *to_dealloc);

/* ADDITIONAL PRIVATE METHOD DECLARATIONS HERE*/

#endif

