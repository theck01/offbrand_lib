
#include "../../include/OBBigUInt.h"
#include "../../include/private/OBBigUInt_Private.h"

#define OBBIGUINT_MEM_USAGE_RATIO 0.5 /* num_int to capacity ratio before
                                         refit OBBigUInt *//

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

   * Research base n -> base m conversions. Treat the string as a decimal number
   * and treat the result as base 32 *
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

OBBigUInt * addBigUInts(OBBigUInt *a, OBBigUInt *b){

  uint64_t i, sum, carry, mask_hi, maxcap;
  OBBigUInt  *largest, *smallest, *result;
  
  if(a->num_ints > b->num_ints) maxcap = a->num_ints + 1;
  else maxcap = b->num_ints + 1;

  /* allocate result array with the maximum possible size for the answer */
  if(a->num_ints > b->num_ints){
    maxcap = a->num_ints + 1;
    largest = a;
    smallest = b;
  }
  else{
    maxcap = b->num_ints + 1;
    largest = b;
    smallest = a;
  }

  result = createBigUIntWithCap(a->num_ints + 1);
  if(!result){
    fprintf(stderr, "OBBigUInt: Could not create result OBBigUInt during "
                    "addition\n");
    return NULL;
  }

  carry = 0;
  for(i=0; i<largest->num_ints; i++){
    /* if all ints of the smaller number have been added do not include smaller
     * in calculation */
    if(i>=smallest->num_ints){
      sum = largest->int_array[i] + carry;
      carry = 0;
    }
    else{
      sum = largest->int_array[i] + smallest->int_array[i] + carry;
      carry = sum >> 32;
    }

    result->int_array[i] = (uint32_t)sum;
  }

  result->int_array[i] = (uint32_t)carry;
  result->num_ints = sigIntsInBigUInt(result);

  return result;
}
  

OBBigUInt * subtractBigUInts(OBBigUInt *minuend, OBBigUInt *subtrahend){

  uint64_t i, diff, carry, mask_hi, maxcap;
  OBBigUInt  *twos_comp, *result, *fitted_result;
  
  if(compareBigUInts((obj *)minuend, (obj *)subtrahend) != OB_GREATER_THAN){
    result = createZeroBigUInt();
    if(!result){
      fprintf(stderr, "OBBigUInt: Could not create zero OBBigUInt as result\n");
    }
    return result;
  }

  twos_comp = twosCompBigUInt(subtrahend, minuend->num_ints);
  if(!twos_comp){
    fprintf(stderr, "OBBigUInt: Could not get result of twos compliment for "
                    "subtraction\n");
    return NULL;
  }

  result = addBigUInts(minuend, twos_comp);
  release(twos_comp); /* destroy now unneeded twos_comp */
  if(!result){
    fprintf(stderr, "OBBigUInt: Could not get result from twos compliment "
                    "addition (subtraction)\n");
    return NULL;
  }

  /* set most significan int to 0, this is overflow from twos compliment
   * addition which should be discarded, and then recalculate num_ints */
  result->int_array[result->capacity - 1] = 0;
  result->num_ints = sigIntsInBigUInt(result);

  /* if subtraction results in a large amount of unused space, refit with a 
   * copy operation */
  if(((double)result->num_ints)/result->capacity < OBBIGUINT_MEM_USAGE_RATIO){
    fitted_result = copyBigUInt(result);
    release((obj *)result); /* destroy original with wasted space */
  }
  else{
    fitted_result = result;
  }

  return fitted_result;
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


uint64_t sigIntsInBigUInt(OBBigUInt *a){

  uint64_t i;
  for(i=a->capacity-1; i>0; i--){
    if(a->int_array[i] > 0){
      break;
    }
  }
  
  /* i is the index of the first non-zero element, i+1 is to total number */
  return i+1;
}


OBBigUInt * twosCompBigUInt(OBBigUInt *a, uint64_t total_len){
  
  uint64_t i, maxcap, sign_extend;
  OBBigUInt *twos_comp;

  if(total_len > a->capacity) maxcap = total_len;
  else maxcap = a->capacity;

  twos_comp = createBigUIntWithCap(maxcap);
  if(!twos_comp){
    fprintf(stderr, "OBBigUInt: Could not create OBBigUInt for twos "
                    "compliment\n");

    return NULL;
  }

  /* find first non zero int to twos compliment individually */
  for(i=0; i<a->num_ints; i++){
    /* if reached least significant non-zero int, take its twos compliment */
    if(a->int_array[i] > 0){
      twos_comp->int_array[i] = (~(a->int_array[i])) + 1;
      break;
    }
    /* havent found least significant non-zero int */
    twos_comp->int_array[i] = 0;
  }

  /* not all remaining ints in a */
  for(++i; i<a->num_ints; i++){
    twos_comp->int_array[i] = ~(a->int_array[i]);
  }

  /* set any extra capacity space to all 1's (sign extension) if twos_comp of a
   * has a 1 in the most significan bit posistion */
  sign_extend = 0;
  if((twos_comp->int_array[i-1] >> 31) == 1) sign_extend = UINT32_MAX;
  for(; i<twos_comp->capacity; i++){
    twos_comp->int_array[i] = sign_extend;
  }

  twos_comp->num_ints = sigIntsInBigUInt(twos_comp);
  return twos_comp;
}


void deallocBigUInt(obj *to_dealloc){

  /* cast generic obj to OBBigUInt */
  OBBigUInt *instance = (OBBigUInt *)to_dealloc;
  free(instance->uint_array);
  free(instance);
  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
