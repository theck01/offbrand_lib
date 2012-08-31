
#include "../../include/%CODECLASSNAME%.h"
#include "../../include/private/%CODECLASSNAME%_Private.h"

/* PUBLIC METHODS */

/* add arguments to complete initialization as needed, modify %CODECLASSNAME%.h
 * as well if modifications are made */
%CODECLASSNAME% * create%CODECLASSNAME%(void){

  %CODECLASSNAME% *new_instance = malloc(sizeof(%CODECLASSNAME%));
  assert(new_instance != NULL);

  /* initialize reference counting base of class */
  init%CODECLASSNAME%Base((obj *)new_instance);

  /* ADD CLASS SPECIFIC INITIALIZATION HERE */

  return new_instance;
}

/* function can be deleted if unneeded */
int8_t compare%CODECLASSNAME%(const obj *a, const obj *b){
  
  const %CODECLASSNAME% *comp_a = (%CODECLASSNAME% *)a;  
  const %CODECLASSNAME% *comp_b = (%CODECLASSNAME% *)b;  

  assert(objIsOfClass(a, "%CODECLASSNAME") && 
         objIsOfClass(b, "%CODECLASSNAME%"));

  /* add specific comparison logic, following the description in the header
   * file */
}


/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

void init%CODECLASSNAME%Base(%CODECLASSNAME% *to_init){

  /* Classname for the this specific class */
  static char *classname = NULL;
  const char stack_classname[] = "%CODECLASSNAME%";

  assert(to_init != NULL);

  if(!classname){
    classname = malloc(sizeof(char) * strlen(stack_classname));
    assert(classname != NULL);
    else strcpy(classname, stack_classname);
  }

  /* initialize reference counting base data */
  initBase((obj *)to_init, &dealloc%CODECLASSNAME%, classname);

  return;
}


void dealloc%CODECLASSNAME%(obj *to_dealloc){

  /* cast generic obj to %CODECLASSNAME% */
  %CODECLASSNAME% *instance = (%CODECLASSNAME% *)to_dealloc;

  assert(to_dealloc && objIsOfClass(to_dealloc, "%CODECLASSNAME%");

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE */

  free(instance);

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
