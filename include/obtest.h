
/**
 * @file obtest.h
 * @brief obtest Public Interface
 * @author theck
 */

#ifndef OBTEST_H
#define OBTEST_H

#include "offbrand.h"

/** Class type declaration */
typedef struct obtest_struct obtest;

/**
 * @brief Constructor, creates an instance of obtest with a specified id
 * @param id Integer id for the created obtest
 * @return Instance of obtest with id as given by parameter
 */
obtest * obtest_new(uint32_t id);

/**
 * @brief Returns the id of an obtest instance
 * @param a Pointer to an instance of obtest
 * @return id value of the obtest argument
 */
uint32_t obtest_id(obtest *a);

#endif

