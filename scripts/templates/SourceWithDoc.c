/**
 * @file %CODECLASSNAME%.c
 * @brief %CODECLASSNAME% Method Implementation
 * @author %CLASSAUTHOR%
 */

#include "../../include/%CODECLASSNAME%.h"
#include "../../include/private/%CODECLASSNAME%_Private.h"

/* PUBLIC METHODS */

/* function can be deleted if unneeded */
int8_t compare%CODECLASSNAME%s(const obj *a, const obj *b){
  
  const %CODECLASSNAME% *comp_a = (%CODECLASSNAME% *)a;  
  const %CODECLASSNAME% *comp_b = (%CODECLASSNAME% *)b;  

  assert(objIsOfClass(a, "%CODECLASSNAME%"));
  assert(objIsOfClass(b, "%CODECLASSNAME%"));

  /* add specific comparison logic, following the description in the header
   * file */
  return OB_EQUAL_TO;
}


/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

/* add arguments to complete initialization as needed, modify 
 * %CODECLASSNAME%_Private.h as well if modifications are made */
%CODECLASSNAME% * createDefault%CODECLASSNAME%(void){

  static const char classname[] = "%CODECLASSNAME%";
  %CODECLASSNAME% *new_instance = malloc(sizeof(%CODECLASSNAME%));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &dealloc%CODECLASSNAME%, &hash%CODECLASSNAME%,
           classname);

  /* ADD CLASS SPECIFIC INITIALIZATION HERE */

  return new_instance;
}

obhash_t hash%CODECLASSNAME%(const obj *to_hash){

  /* Implement a hash function suitable for uniquely itentifying
   * %CODECLASSNAME% instances if the default hash below is not adequate */
  return defaultHash(to_hash);

}


void dealloc%CODECLASSNAME%(obj *to_dealloc){

  /* cast generic obj to %CODECLASSNAME% */
  %CODECLASSNAME% *instance = (%CODECLASSNAME% *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "%CODECLASSNAME%"));

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE BUT DO NOT
   * FREE INSTANCE, THE LIBRARY WILL DO THIS FOR THE USER */

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
