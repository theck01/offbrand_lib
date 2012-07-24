
#include "../../include/OBBigUInt.h"
#include "../../include/private/OBBigUInt_Private.h"

/* PUBLIC METHODS */

/* add arguments to complete initialization as needed, modify OBBigUInt.h
 * as well if modifications are made */
OBBigUInt * createBigUInt(void){

  OBBigUInt *new_instance = malloc(sizeof(OBBigUInt));
  if(!new_instance){
    fprintf(stderr, "OBBigUInt: Could not allocate memory for "
                    "new_instance\n");
    return NULL;
  }

  /* initialize reference counting base data */
  initBase((obj *)new_instance, &deallocBigUInt);

  /* ADD CLASS SPECIFIC INITIALIZATION HERE */

  return new_instance;
}

/* function can be deleted if unneeded */
int8_t compareBigUInt(const obj *a, const obj *b){
  
  OBBigUInt comp_a = (OBBigUInt *)a;  
  OBBigUInt comp_b = (OBBigUInt *)b;  

  /* add specific comparison logic, following the description in the header
   * file */
}


/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

void deallocBigUInt(obj *to_dealloc){

  /* cast generic obj to OBBigUInt */
  OBBigUInt *instance = (OBBigUInt *)to_dealloc;

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE */

  free(instance);

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
