
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

  /* initialize reference counting base of class */
  if(init%CODECLASSNAME%Base((obj *)new_instance)){
    fprintf(stderr, "%CODECLASSNAME%: Could not initialize new instance\n");
    return NULL;
  }

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

uint8_t init%CODECLASSNAME%Base(%CODECLASSNAME% *to_init){

  /* Classname for the this specific class */
  static char *classname = NULL;
  const char stack_classname[] = "%CODECLASSNAME%";

  if(!classname){
    classname = malloc(sizeof(char) * strlen(stack_classname));
    if(!classname){
      fprintf(stderr, "%CODECLASSNAME%: Could not allocate static classname "
                      "for all instances\nClass checking functions will not "
                      "work until classname allocated\n");
      return 1;
    }
    else strcpy(classname, stack_classname);
  }

  /* initialize reference counting base data */
  if(initBase((obj *)to_init, &dealloc%CODECLASSNAME%, classname)){
    fprintf(stderr, "%CODECLASSNAME%: Could not initialize base obj\n");
    return 1;
  }

  return 0;
}


void dealloc%CODECLASSNAME%(obj *to_dealloc){

  /* cast generic obj to %CODECLASSNAME% */
  %CODECLASSNAME% *instance = (%CODECLASSNAME% *)to_dealloc;

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE */

  free(instance);

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
