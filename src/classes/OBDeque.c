
#include "../../include/OBDeque.h"
#include "../../include/private/OBDeque_Private.h"

/* PUBLIC METHODS */

/* function can be deleted if unneeded */
int8_t compareOBDeques(const obj *a, const obj *b){
  
  const OBDeque *comp_a = (OBDeque *)a;  
  const OBDeque *comp_b = (OBDeque *)b;  

  assert(objIsOfClass(a, "%CODECLASSNAME"));
  assert(objIsOfClass(b, "OBDeque"));

  /* add specific comparison logic, following the description in the header
   * file */

  return OB_EQUAL_TO;
}


/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

/* add arguments to complete initialization as needed, modify 
 * OBDeque_Private.h as well if modifications are made */
OBDeque * createDefaultOBDeque(void){

  static const char classname[] = "OBDeque";
  OBDeque *new_instance = malloc(sizeof(OBDeque));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocOBDeque, NULL, classname);

  /* ADD CLASS SPECIFIC INITIALIZATION HERE */

  return new_instance;
}


void deallocOBDeque(obj *to_dealloc){

  /* cast generic obj to OBDeque */
  OBDeque *instance = (OBDeque *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBDeque"));

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE BUT DO NOT
   * FREE INSTANCE, THE LIBRARY WILL DO THIS FOR THE USER */

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
