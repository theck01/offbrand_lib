
#include "OBTestObj.h"
#include "OBTestObj_Private.h"

/* PUBLIC METHODS */

/* add arguments to complete initialization as needed, modify OBTestObj.h
 * as well if modifications are made */
OBTestObj * createOBTestObj(void){

  OBTestObj *new_instance = malloc(sizeof(OBTestObj));
  if(!new_instance){
    fprintf(stderr, "OBTestObj: Could not allocate memory for "
                    "new_instance\n");
    return NULL;
  }

  /*initialize reference counting base data*/
  initObj(&(new_instance->base), &destroyOBTestObj);

  /* ADD ADDITIONAL INITIALIZATION HERE */

  return new_instance;
}

/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

void destroyOBTestObj(OBTestObj *to_destroy){

  /* RELEASE ANY OBJECTS CONTAINED BY to_destroy HERE */

  free(to_destroy);
  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
