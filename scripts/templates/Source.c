/**
 * @file %CODECLASSNAME%.c
 * @brief %CODECLASSNAME% Method Implementation
 * @author %CLASSAUTHOR%
 */

#include "../../include/%CODECLASSNAME%.h"
#include "../../include/private/%CODECLASSNAME%_Private.h"

/* PUBLIC METHODS */


/* DEFINE PUBLIC METHODS HERE */


/* PRIVATE METHODS */

/* add arguments to complete initialization as needed, modify 
 * %CODECLASSNAME%_Private.h as well if modifications are made */
%CODECLASSNAME% * createDefault%CODECLASSNAME%(void){

  static const char classname[] = "%CODECLASSNAME%";
  %CODECLASSNAME% *new_instance = malloc(sizeof(%CODECLASSNAME%));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &dealloc%CODECLASSNAME%, &hash%CODECLASSNAME%,
           &compare%CODECLASSNAME%s, classname);

  /* ADD CLASS SPECIFIC INITIALIZATION HERE */

  return new_instance;
}


obhash_t hash%CODECLASSNAME%(const obj *to_hash){

  static int8_t init = 0;
  static obhash_t seed = 0;

  obhash_t value;
  %CODECLASSNAME% *instance = (%CODECLASSNAME% *)to_hash;

  assert(to_hash);
  assert(objIsOfClass(to_hash, "%CODECLASSNAME%"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;

  /* Implement a hash function suitable for uniquely itentifying
   * %CODECLASSNAME% instances if default hash is not adequate. If no
   * hash function is desired then be sure to pass NULL as the hash argument
   * for initBase in createDefault%CODECLASSNAME% to use the default hash 
   * function. Returning the stdlib hash function here can cause concurrency
   * issues in C versions less than C11 (even though it will work for single 
   * thread programs). If unneeded and above considerations are met then
   * this function can be deleted from all %CODECLASSNAME% Files*/

  return 0;
}


int8_t compare%CODECLASSNAME%s(const obj *a, const obj *b){
  
  const %CODECLASSNAME% *comp_a = (%CODECLASSNAME% *)a;  
  const %CODECLASSNAME% *comp_b = (%CODECLASSNAME% *)b;  

  assert(a);
  assert(b);
  assert(objIsOfClass(a, "%CODECLASSNAME%"));
  assert(objIsOfClass(b, "%CODECLASSNAME%"));

  /* add specific comparison logic, following the description in the header
   * file. If no comparison function is desired then be sure to pass NULL to
   * the compare argument of initBase in the createDefault%CODECLASSNAME%
   * function to use the default pointer comparision Returning the stdlib 
   * compare function here can cause concurrency issues in C versions less than
   * C11 (even though it will work for single thread programs) If unneeded and 
   * above considerations are met then this function can be deleted from all 
   * %CODECLASSNAME% Files*/
  return OB_EQUAL_TO;
}


void dealloc%CODECLASSNAME%(obj *to_dealloc){

  /* cast generic obj to %CODECLASSNAME% */
  %CODECLASSNAME% *instance = (%CODECLASSNAME% *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "%CODECLASSNAME%"));

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE BUT DO NOT
   * FREE INSTANCE, THE LIBRARY WILL DO THAT */

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
