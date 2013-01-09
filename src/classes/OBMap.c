/**
 * @file OBMap.c
 * @brief OBMap Method Implementation
 * @author theck
 */

#include "../../include/OBMap.h"
#include "../../include/private/OBMap_Private.h"

/* PUBLIC METHODS */


/* DEFINE PUBLIC METHODS HERE */


/* PRIVATE METHODS */

/* add arguments to complete initialization as needed, modify 
 * OBMap_Private.h as well if modifications are made */
OBMap * createDefaultOBMap(void){

  static const char classname[] = "OBMap";
  OBMap *new_instance = malloc(sizeof(OBMap));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocOBMap, &hashOBMap,
           &compareOBMaps, classname);

  /* ADD CLASS SPECIFIC INITIALIZATION HERE */

  return new_instance;
}


obhash_t hashOBMap(const obj *to_hash){

  static int8_t init = 0;
  static obhash_t seed = 0;

  obhash_t value;
  OBMap *instance = (OBMap *)to_hash;

  assert(to_hash);
  assert(objIsOfClass(to_hash, "OBMap"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;

  /* Implement a hash function suitable for uniquely itentifying
   * OBMap instances if default hash is not adequate. If no
   * hash function is desired then be sure to pass NULL as the hash argument
   * for initBase in createDefaultOBMap to use the default hash 
   * function. Returning the stdlib hash function here can cause concurrency
   * issues in C versions less than C11 (even though it will work for single 
   * thread programs). If unneeded and above considerations are met then
   * this function can be deleted from all OBMap Files*/

  return 0;
}


int8_t compareOBMaps(const obj *a, const obj *b){
  
  const OBMap *comp_a = (OBMap *)a;  
  const OBMap *comp_b = (OBMap *)b;  

  assert(a);
  assert(b);
  assert(objIsOfClass(a, "OBMap"));
  assert(objIsOfClass(b, "OBMap"));

  /* add specific comparison logic, following the description in the header
   * file. If no comparison function is desired then be sure to pass NULL to
   * the compare argument of initBase in the createDefaultOBMap
   * function to use the default pointer comparision Returning the stdlib 
   * compare function here can cause concurrency issues in C versions less than
   * C11 (even though it will work for single thread programs) If unneeded and 
   * above considerations are met then this function can be deleted from all 
   * OBMap Files*/
  return OB_EQUAL_TO;
}


void deallocOBMap(obj *to_dealloc){

  /* cast generic obj to OBMap */
  OBMap *instance = (OBMap *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBMap"));

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE BUT DO NOT
   * FREE INSTANCE, THE LIBRARY WILL DO THAT */

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
