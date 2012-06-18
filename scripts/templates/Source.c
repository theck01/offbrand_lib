
#include "%CODECLASSNAME%.h"
#include "%CODECLASSNAME%_Private.h"

/* PUBLIC METHODS */

/* add arguments to complete initialization as needed, modify %CODECLASSNAME%.h
 * as well if modifications are made */
%CODECLASSNAME% * create%CODECLASSNAME%(void){

  %CODECLASSNAME% *new_instance = malloc(sizeof(%CODECLASSNAME%));
  if(!new_instance){
    fprintf(stderr, "%CODECLASSNAME%: Could not allocate memory for "
                    "new_instance\n");
    return NULL;
  }

  /*initialize reference counting base data*/
  initObj(&(new_instance->base), &destroy%CODECLASSNAME%);

  /* ADD ADDITIONAL INITIALIZATION HERE */

  return new_instance;
}

/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

void destroy%CODECLASSNAME%(%CODECLASSNAME% *to_destroy){

  /* RELEASE ANY OBJECTS CONTAINED BY to_destroy HERE */

  free(to_destroy);
  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
