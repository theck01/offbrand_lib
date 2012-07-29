
#include "../../include/OBBigUInt.h"
#include "../../include/private/OBBigUInt_Private.h"

/* PUBLIC METHODS */

/* add arguments to complete initialization as needed, modify OBBigUInt.h
 * as well if modifications are made */
OBBigUInt * createZeroBigUInt(void){

  OBBigUInt *new_instance = createBigUIntWithCap(1);
  if(!new_instance){
    fprintf(stderr, "OBBigUInt: Could not create new_instance\n");
    return NULL;
  }
  
  new_instance->uint_array[0] = 0;
  new_instance->num_ints = 1;

  return new_instance;
}


OBBigUInt * createBigUIntFromNum(uint32_t number){

  OBBigUInt *new_instance = createBigUIntWithCap(1);
  if(!new_instance){
    fprintf(stderr, "OBBigUInt: Could not create new_instance\n");
    return NULL;
  }

  new_instance->uint_array[0] = number;
  new_instance->num_ints = 1;

  return new_instance;
}


OBBigUInt * createBigUIntFromStr(char *num_str){

  uint64_t len = strlen(num_str);

  /* create an instance with maximum possible capacity required
   * 4 binary digits per single decimal digit, 32 binary digits per uint32_t
  OBBigUInt *new_instance = createBigUIntWithCap(((4*len)/32)+1);

  * determine most effective decimal to binary algorithm */
  return NULL; /* placeholder */
}


OBBigUInt * copyBigUInt(OBBigUInt *to_copy){

  uint64_t i;

  OBBigUInt *new_instance = createBigUIntWithCap(to_copy->num_ints);
  if(!new_instance){
    fprintf(stderr, "OBBigUInt: Could not create new_instance\n");
    return NULL;
  }

  for(i=0; i<to_copy->num_ints; i++){
    new_instance->uint_array[i] = to_copy->uint_array[i];
  }

  new_instance->num_ints = to_copy->num_ints;

  return new_instance;
}


int8_t compareBigUInt(const obj *a, const obj *b){
  
  OBBigUInt comp_a = (OBBigUInt *)a;  
  OBBigUInt comp_b = (OBBigUInt *)b;  

  /* add specific comparison logic, following the description in the header
   * file */
}


/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

OBBigUInt * createBigUIntWithCap(uint64_t capacity){

 OBBigUInt *new_instance = malloc(sizeof(OBBigUInt));
  if(!new_instance){
    fprintf(stderr, "OBBigUInt: Could not allocate memory for "
                    "new_instance\n");
    return NULL;
  }

  /* initialize reference counting base data */
  initBase((obj *)new_instance, &deallocBigUInt);

  new_instance->uint_array = malloc(sizeof(uint32_t)*capacity);
  if(!new_instance->uint_array){
    fprintf(stderr, "OBBigUInt: Could not allocate memory for "
                    "internal integer array\n");
    free(new_instance);
    return NULL;
  }

  new_instance->num_ints = 0;
  new_instance->capacity = capacity;

  return new_instance;
}

void deallocBigUInt(obj *to_dealloc){

  /* cast generic obj to OBBigUInt */
  OBBigUInt *instance = (OBBigUInt *)to_dealloc;

  free(instance->uint_array);
  free(instance);

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
