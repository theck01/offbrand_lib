
#include "../../include/%CODECLASSNAME%.h"
#include "../../include/private/%CODECLASSNAME%_Private.h"

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

  /* initialize reference counting base data */
  initBase((obj *)new_instance, &dealloc%CODECLASSNAME%);

  /* ADD CLASS SPECIFIC INITIALIZATION HERE */

  return new_instance;
}

/* function can be deleted if unneeded */
int8_t compare%CODECLASSNAME%(const obj *a, const obj *b){
  
  %CODECLASSNAME% *comp_a = (%CODECLASSNAME% *)a;  
  %CODECLASSNAME% *comp_b = (%CODECLASSNAME% *)b;  

  /* add specific comparison logic, following the description in the header
   * file */
}


/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

void dealloc%CODECLASSNAME%(obj *to_dealloc){

  /* cast generic obj to %CODECLASSNAME% */
  %CODECLASSNAME% *instance = (%CODECLASSNAME% *)to_dealloc;

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE */

  free(instance);

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
