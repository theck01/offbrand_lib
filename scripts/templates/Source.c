
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
  initBase((obj *)new_instance, &dealloc%CODECLASSNAME%);

  /* ADD CLASS SPECIFIC INITIALIZATION HERE */

  return new_instance;
}

/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

void dealloc%CODECLASSNAME%(obj *to_dealloc){

  /*cast generic obj to specific class*/
  %CODECLASSNAME% *instance = (%CODECLASSNAME% *)to_dealloc;

  /*PERFORM CLASS SPECIFIC MEMORY MANAGEMENT HERE*/

  free(to_destroy);
  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
