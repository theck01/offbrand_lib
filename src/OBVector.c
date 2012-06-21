
#include "OBVector.h"
#include "OBVector_Private.h"

/* PUBLIC METHODS */

/* add arguments to complete initialization as needed, modify OBVector.h
 * as well if modifications are made */
OBVector * createOBVector(void){

  OBVector *new_instance = malloc(sizeof(OBVector));
  if(!new_instance){
    fprintf(stderr, "OBVector: Could not allocate memory for "
                    "new_instance\n");
    return NULL;
  }

  /* initialize reference counting base data */
  initBase((obj *)new_instance, &deallocOBVector);

  /* ADD CLASS SPECIFIC INITIALIZATION HERE */

  return new_instance;
}

/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

void deallocOBVector(obj *to_dealloc){

  /* cast generic obj to OBVector */
  OBVector *instance = (OBVector *)to_dealloc;

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE */

  free(instance);
  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
