/**
 * @file OBString.c
 * @brief OBString Method Implementation
 * @author theck
 */

#include "../../include/OBString.h"
#include "../../include/private/OBString_Private.h"

/* PUBLIC METHODS */

/* function can be deleted if unneeded */
int8_t compareStrings(const obj *a, const obj *b){
  
  const OBString *comp_a = (OBString *)a;  
  const OBString *comp_b = (OBString *)b;  

  assert(objIsOfClass(a, "OBString"));
  assert(objIsOfClass(b, "OBString"));

  /* add specific comparison logic, following the description in the header
   * file */
  return OB_EQUAL_TO;
}


/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

/* add arguments to complete initialization as needed, modify 
 * OBString_Private.h as well if modifications are made */
OBString * createDefaultString(void){

  static const char classname[] = "OBString";
  OBString *new_instance = malloc(sizeof(OBString));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocString, &hashString, &compareStrings,
           classname);

  new_instance->str = NULL;
  new_instance->length = 0;

  return new_instance;
}

obhash_t hashString(const obj *to_hash){
  return hash(to_hash);
}


void deallocString(obj *to_dealloc){

  /* cast generic obj to OBString */
  OBString *instance = (OBString *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBString"));

  if(instance->str) free(instance->str);

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
