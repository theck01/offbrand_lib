/**
 * @file OBInt.c
 * @brief OBInt Method Implementation
 * @author theck
 */

#include "../../include/OBInt.h"
#include "../../include/private/OBInt_Private.h"

/* PUBLIC METHODS */


/* DEFINE PUBLIC METHODS HERE */


/* PRIVATE METHODS */

/* add arguments to complete initialization as needed, modify 
 * OBInt_Private.h as well if modifications are made */
OBInt * createDefaultInt(void){

  static const char classname[] = "OBInt";
  OBInt *new_instance = malloc(sizeof(OBInt));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocInt, &hashInt, &compareInts, 
           &displayInt, classname);

  /* ADD CLASS SPECIFIC INITIALIZATION HERE */

  return new_instance;
}


obhash_t hashInt(const obj *to_hash){

  static int8_t init = 0;
  static obhash_t seed = 0;

  obhash_t value;
  OBInt *instance = (OBInt *)to_hash;

  assert(to_hash);
  assert(objIsOfClass(to_hash, "OBInt"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;

  /* Implement a hash function suitable for uniquely itentifying
   * OBInt instances. If instance address munging is an acceptable
   * hash then this method can be deleted and NULL should be supplied to the
   * hash_funct argument of initBase in createdDefaultInt */

  return 0;
}


int8_t compareInts(const obj *a, const obj *b){
  
  const OBInt *comp_a = (OBInt *)a;  
  const OBInt *comp_b = (OBInt *)b;  

  assert(a);
  assert(b);
  assert(objIsOfClass(a, "OBInt"));
  assert(objIsOfClass(b, "OBInt"));

  /* add specific comparison logic, following the description in the header
   * file. If pointer based comparision is all that is needed then this method 
   * can be deleted and NULL should be supplied to the compare_funct argument of
   * initBase in createdDefaultInt */

  return OB_EQUAL_TO;
}


void displayInt(const obj *to_print){

  const OBInt *instance = (OBInt *)to_print;
  
  assert(to_print);
  assert(objIsOfClass(to_print, "OBInt"));

  /* add class specific display logic. If printing the classname and address
   * of an instance is all that is needed then this method can be deleted and
   * NULL should be supplid to the display_funct argument of initBase in
   * createDefaultInt */

  return;
}


void deallocInt(obj *to_dealloc){

  /* cast generic obj to OBInt */
  OBInt *instance = (OBInt *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBInt"));

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE BUT DO NOT
   * FREE INSTANCE, THE LIBRARY WILL DO THAT */

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
