
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
  new_instance->num_uints = 1;

  return new_instance;
}


OBBigUInt * createBigUIntFromNum(uint32_t number){

  OBBigUInt *new_instance = createBigUIntWithCap(1);
  if(!new_instance){
    fprintf(stderr, "OBBigUInt: Could not create new_instance\n");
    return NULL;
  }

  new_instance->uint_array[0] = number;
  new_instance->num_uints = 1;

  return new_instance;
}


OBBigUInt * createBigUIntFromIntArray(uint32_t *uint_array, uint64_t num_uints){

  uint64_t i;
  OBBigUInt *new_instance;
  
  if(!int_str, || num_uints == 0){
    fprintf(stderr, "OBBigUInt: Could not create BigUInt from NULL "
                    "uint_array\n");
    return NULL;
  }

  new_instance = createBigUIntWithCap(num_uints);
  if(!new_intstance){
    fprintf(stderr, "OBBigUInt: Could not create new instance in "
                    "createBigUIntFromIntStr\n");
    return NULL;
  }

  for(i=0; i<num_uints; i++){
    new_instance->uint_array[i] = uint_array[i];
  }

  new_instance->num_uints = sigIntsInBigUInt(new_instance);
  new_instance->capacity = num_uints;

  return new_instance;
}


OBBigUInt * copyBigUInt(OBBigUInt *to_copy){

  uint64_t i;

  OBBigUInt *new_instance = createBigUIntWithCap(to_copy->num_uints);
  if(!new_instance){
    fprintf(stderr, "OBBigUInt: Could not create new_instance\n");
    return NULL;
  }

  for(i=0; i<to_copy->num_uints; i++){
    new_instance->uint_array[i] = to_copy->uint_array[i];
  }

  new_instance->num_uints = to_copy->num_uints;

  return new_instance;
}

OBBigUInt * addBigUInts(OBBigUInt *a, OBBigUInt *b){

  uint64_t i, sum, carry, mask_hi, maxcap;
  OBBigUInt  *largest, *smallest, *result;
  
  if(!a || !b){
    fprintf(stderr, "OBBigUInt: Unexpected NULL argument(s) passed to "
                    "addBigUInts\n");
    return NULL;
  }

  if(a->num_uints > b->num_uints) maxcap = a->num_uints + 1;
  else maxcap = b->num_uints + 1;

  /* allocate result array with the maximum possible size for the answer */
  if(a->num_uints > b->num_uints){
    maxcap = a->num_uints + 1;
    largest = a;
    smallest = b;
  }
  else{
    maxcap = b->num_uints + 1;
    largest = b;
    smallest = a;
  }

  result = createBigUIntWithCap(maxcap);
  if(!result){
    fprintf(stderr, "OBBigUInt: Could not create result OBBigUInt during "
                    "addition\n");
    return NULL;
  }

  carry = 0;
  for(i=0; i<largest->num_uints; i++){
    /* if all ints of the smaller number have been added do not include smaller
     * in calculation */
    if(i>=smallest->num_uints){
      sum = largest->uint_array[i] + carry;
      carry = 0;
    }
    else{
      sum = largest->uint_array[i] + smallest->uint_array[i] + carry;
      carry = sum >> 32;
    }

    result->uint_array[i] = (uint32_t)sum;
  }

  result->uint_array[i] = (uint32_t)carry;
  result->num_uints = sigIntsInBigUInt(result);

  return result;
}
  

OBBigUInt * subtractBigUInts(OBBigUInt *minuend, OBBigUInt *subtrahend){

  OBBigUInt  *twos_comp, *result, *fitted_result;
  
  if(!minuend || !subtrahend){
    fprintf(stderr, "OBBigUInt: Unexpected NULL argument(s) passed to "
                    "subtractBigUInts\n");
    return NULL;
  }

  if(compareBigUInts((obj *)minuend, (obj *)subtrahend) != OB_GREATER_THAN){
    result = createZeroBigUInt();
    if(!result){
      fprintf(stderr, "OBBigUInt: Could not create zero OBBigUInt as result\n");
    }
    return result;
  }

  twos_comp = twosCompBigUInt(subtrahend, minuend->num_uints);
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
   * addition which should be discarded, and then recalculate num_uints */
  result->uint_array[result->capacity - 1] = 0;
  result->num_uints = sigIntsInBigUInt(result);

  /* if subtraction results in a large amount of unused space, refit with a 
   * copy operation */
  if(((double)result->num_uints)/result->capacity < OBBIGUINT_MEM_USAGE_RATIO){
    fitted_result = copyBigUInt(result);
    release((obj *)result); /* destroy original with wasted space */
  }
  else{
    fitted_result = result;
  }

  return fitted_result;
}

/* Performs Multiplication using the Karatsuba Multiplication algorithm. This
 * algorithm reduces the complexity of the multiplication from O(n^2) to
 * O(3n^log(3)) */
OBBigUInt * multiplyBigUInts(OBBigUInt *a, OBBigUInt *b){

  uint64_t mult_result, m;

  /* OBBigUInts resulting from partial multiplcations */
  OBBigUInt *x, *shift_x, *y, *shift_y, *z, tmp;

  /* OBBigUInts equalling halves of a and b */
  OBBigUInt *a1, *a0, *b1, *b0;

  /* final result */
  OBBigUInt *result;

  /* init pointers */
  u = v = x = y = z = a1 = b1 = a0 = b0 = result = NULL;

  if(!a || !b){
    fprintf(stderr, "OBBigUInt: Cannot multiply NULL argument(s)\n");
    return NULL;
  }

  /* base case, BigUInts are small enough to compute directly */
  if(a->num_uints <= 1 && b->num_uints <= 1){
    result = createBigUIntWithCap(2);
    if(!result){
      goto MULT_ERR;  
    }
    mult_result = ((uint64_t)a->uint_array[0])*((uint64_t)b->uint_array[0]);
    result->uint_array[0] = (uint32_t)mult_result;
    result->uint_array[1] = (uint32_t)(mult_result>>32);
    return result;
  }

  /* NULL results from partial calculations will be ignored form this point on,
   * they will just cause all other partial calculations affected to also return
   * NULL */

  /* determine the position where a and b should be split in half*/
  if(a->num_uints > b->num_uints) m=a->num_uints/2+1;
  else m=b->num_uints/2+1;

  /* split into halves depending on the size of a */
  if(a->num_uints > m){
    a0 = createBigUIntFromIntArray(a->uint_array, m);
    a1 = createBigUIntFromIntArray(a->uint_array + m,
                                   a->num_uints - m);
  }
  else{
    a0 = copyBigUInt(a);
    a1 = createZeroBigUInt();
  }
  /* do the same for b */
  if(b->num_uints > m){
    b0 = createBigUIntFromIntArray(b->uint_array, m); 
    b1 = createBigUIntFromIntArray(b->uint_array + m,
                                   b->num_uints - m);
  }
  else{
    b0 = copyBigUInt(b);
    b1 = createZeroBigUInt();
  }
  
  /* calculate partial multiplications, freeing unneeded memory as soon as
   * possible */ 

  /* get the y = (a0 + a1)(b0 + b1) part */
  x = sumBigUInts(a1,a0);
  z = sumBigUints(b1,b0);
  y = multiplyBigUInts(u,w);
  release((obj *)x);
  release((obj *)z);

  /* get the x = a1*b1 and z = a0*b0 part */
  x = multiplyBigUInts(a1,b1);
  z = multiplyBigUInts(a0,b0);
  release((obj *)a1);
  release((obj *)a0);
  release((obj *)b1);
  release((obj *)b0);

  /* refine y part to y = (a0 + a1)(b0 + b1) - a1*b1 - a0*b0 */
  tmp = subtractBigUints(y,x);
  release((obj *)y);
  y = subtractBigUInts(tmp,z);
  release((obj *)tmp);

  /* get result = x*B^(2m) + y*B^m + z, where B is 2^32, a single uint32_t */
  shift_x = multShift(x, 2*m);
  release((obj *)x);
  shift_y = multShift(y, m);
  release((obj *)y);
  tmp = sumBigUInts(shift_x, shift_y);
  release((obj *)shift_x);
  release((obj *)shift_y);
  result = sumBigUInts(tmp, z);
  release((obj *)tmp);
  release((obj *)z);

  if(!result){
    fprintf(stderr, "OBBigUInt: Intermediate operation(s) in multiplyBigUInts "
                    "failed, so multiply failed\n");
  }

  return result;
}


OBBigUInt * divideBigUInts(OBBigUInts *dividend, OBBigUInt *divisor){


}


int8_t compareBigUInt(const obj *a, const obj *b){
  
  uint64_t i;
  OBBigUInt comp_a = (OBBigUInt *)a;  
  OBBigUInt comp_b = (OBBigUInt *)b;  

  if(!a || !b){
    fprintf(stderr, "OBBigUInt: Unexpected NULL argument(s) passed to "
                    "compareBigUInts\n");
    return NULL;
  }

  /* compare size of BigUInts first */
  if(a->num_uints > b->num_uints) return OB_GREATER_THAN;
  else if(a->num_uints < b->num_uints) return OB_LESS_THAN;
  else{ /*if size is equivalent, compare each individual element */
    for(i = a->num_uints - 1; i>=0; i++){
      if(a->uint_array[i] > b->uint_array[i]) return OB_GREATER_THAN;
      else if(a->uint_array[i] < b->uint_array[i]) return OB_LESS_THAN;
    }
  }

  /* all entries in BigUInts must be equal to reach this point */
  return OB_EQUAL_TO;
}


/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

OBBigUInt * createBigUIntWithCap(uint64_t capacity){

  uint64_t i;
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

  for(i=0; i<capacity; i++){
    new_instance->uint_array[i] = 0;
  }

  new_instance->num_uints = 0;
  new_instance->capacity = capacity;

  return new_instance;
}


uint64_t sigIntsInBigUInt(OBBigUInt *a){

  uint64_t i;
  for(i=a->capacity-1; i>0; i--){
    if(a->uint_array[i] > 0){
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
  for(i=0; i<a->num_uints; i++){
    /* if reached least significant non-zero int, take its twos compliment */
    if(a->uint_array[i] > 0){
      twos_comp->uint_array[i] = (~(a->uint_array[i])) + 1;
      break;
    }
    /* havent found least significant non-zero int */
    twos_comp->uint_array[i] = 0;
  }

  /* not all remaining ints in a */
  for(++i; i<a->num_uints; i++){
    twos_comp->uint_array[i] = ~(a->uint_array[i]);
  }

  /* set any extra capacity space to all 1's (sign extension) if twos_comp of a
   * has a 1 in the most significan bit posistion */
  sign_extend = 0;
  if((twos_comp->uint_array[i-1] >> 31) == 1) sign_extend = UINT32_MAX;
  for(; i<twos_comp->capacity; i++){
    twos_comp->uint_array[i] = sign_extend;
  }

  twos_comp->num_uints = sigIntsInBigUInt(twos_comp);
  return twos_comp;
}


OBBigUInt * multShift(OBBigUInt *to_shift, uint64_t num_uints){

  uint64_t i;
  OBBigUInt *result = createBigUIntWithCap(to_shift->num_uints + num_uints);
  if(!result){
    fprintf(stderr, "OBBigUInt: Could not create the result of the "
                    "multShift\n");
    return NULL;
  }

  for(i=0; i<to_shift->num_uints; i++){
    result->uint_array[i+num_ints] = to_shift->uint_array[i];
  }

  return result;
}

OBBigUInt * recursiveDivide(OBBigUInt *dividend, OBBigUInt *divisor,
                            OBBigUInt *quotient, uint8_t return_quotient){
  
  OBBigUInt *result;
  if(compareBigUInts(dividend, divisor) == OB_LESS_THAN){
    /* if returning quotient seed result with 0, else return current dividend */
    if(return_quotient){
      result = createZeroBigUInt();
    }
    else{
      result = copyBigUInt(dividend);
    }
    return result;
  }
}

void deallocBigUInt(obj *to_dealloc){

  /* cast generic obj to OBBigUInt */
  OBBigUInt *instance = (OBBigUInt *)to_dealloc;
  free(instance->uint_array);
  free(instance);
  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
