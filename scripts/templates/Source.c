/**
 * @file %CLASSNAME%.c
 * @brief %CLASSNAME% Method Implementation
 * @author %CLASSAUTHOR%
 */

#include "../../include/%CLASSNAME%.h"
#include "../../include/private/%CLASSNAME%_Private.h"

/* PUBLIC METHODS */


/* DEFINE PUBLIC METHODS HERE */


/* PRIVATE METHODS */

/* add arguments to complete initialization as needed, modify 
 * %CLASSNAME%_Private.h as well if modifications are made */
%CLASSNAME% * createDefault%METHODCLASSNAME%(void){

  static const char classname[] = "%CLASSNAME%";
  %CLASSNAME% *new_instance = malloc(sizeof(%CLASSNAME%));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase(new_instance, &dealloc%METHODCLASSNAME%, 
           &hash%METHODCLASSNAME%, &compare%METHODCLASSNAME%s, 
           &display%METHODCLASSNAME%, classname);

  /* ADD CLASS SPECIFIC INITIALIZATION HERE */

  return new_instance;
}


obhash_t hash%METHODCLASSNAME%(const obj *to_hash){

  static int8_t init = 0;
  static obhash_t seed = 0;

  obhash_t value;
  %CLASSNAME% *instance = (%CLASSNAME% *)to_hash;

  assert(to_hash);
  assert(objIsOfClass(to_hash, "%CLASSNAME%"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;

  /* Implement a hash function suitable for uniquely itentifying
   * %CLASSNAME% instances. If instance address munging is an acceptable
   * hash then this method can be deleted and NULL should be supplied to the
   * hash_funct argument of initBase in createdDefault%METHODCLASSNAME% */

  return 0;
}


int8_t compare%METHODCLASSNAME%s(const obj *a, const obj *b){
  
  const %CLASSNAME% *comp_a = (%CLASSNAME% *)a;  
  const %CLASSNAME% *comp_b = (%CLASSNAME% *)b;  

  assert(a);
  assert(b);
  assert(objIsOfClass(a, "%CLASSNAME%"));
  assert(objIsOfClass(b, "%CLASSNAME%"));

  /* add specific comparison logic, following the description in the header
   * file. If pointer based comparision is all that is needed then this method 
   * can be deleted and NULL should be supplied to the compare_funct argument of
   * initBase in createdDefault%METHODCLASSNAME% */

  return OB_EQUAL_TO;
}


void display%METHODCLASSNAME%(const obj *to_print){

  const %CLASSNAME% *instance = (%CLASSNAME% *)to_print;
  
  assert(to_print);
  assert(objIsOfClass(to_print, "%CLASSNAME%"));

  /* add class specific display logic. If printing the classname and address
   * of an instance is all that is needed then this method can be deleted and
   * NULL should be supplid to the display_funct argument of initBase in
   * createDefault%METHODCLASSNAME% */

  return;
}


void dealloc%METHODCLASSNAME%(obj *to_dealloc){

  /* cast generic obj to %CLASSNAME% */
  %CLASSNAME% *instance = (%CLASSNAME% *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "%CLASSNAME%"));

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE BUT DO NOT
   * FREE INSTANCE, THE LIBRARY WILL DO THAT */

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
