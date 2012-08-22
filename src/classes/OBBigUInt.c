
#include "../../include/OBBigUInt.h"
#include "../../include/private/OBBigUInt_Private.h"

#define MEM_USAGE_RATIO 0.5 /* num_int to capacity ratio before
                                         refit OBBigUInt */

/* PUBLIC METHODS */

OBBigUInt * createBigUIntFromNum(uint32_t number){

  OBBigUInt *new_instance = createBigUIntWithCap(1);
  if(!new_instance){
    fprintf(stderr, "OBBigUInt: Could not create new_instance from number\n");
    return NULL;
  }

  new_instance->uint_array[0] = number;
  new_instance->num_uints = 1;

  return new_instance;
}


OBBigUInt * createBigUIntFromIntArray(uint32_t *uint_array,
                                      uint64_t num_uints){

  uint64_t i;
  OBBigUInt *new_instance;
  
  if(!uint_array || num_uints == 0){
    fprintf(stderr, "OBBigUInt: Could not create BigUInt from empty "
                    "uint_array\n");
    return NULL;
  }

  new_instance = createBigUIntWithCap(num_uints);
  if(!new_instance){
    fprintf(stderr, "OBBigUInt: Could not create new instance in "
                    "createBigUIntFromIntArray\n");
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

  uint64_t i, sum, carry;
  OBBigUInt  *largest, *smallest, *result;
  
  if(!a || !b){
    fprintf(stderr, "OBBigUInt: Unexpected NULL argument(s) passed to "
                    "addBigUInts\n");
    return NULL;
  }

  /* allocate result array with the maximum possible size for the answer */
  if(a->num_uints > b->num_uints){
    largest = a;
    smallest = b;
  }
  else{
    largest = b;
    smallest = a;
  }

  result = createBigUIntWithCap(largest->num_uints + 1);
  if(!result){
    fprintf(stderr, "OBBigUInt: Could not create result OBBigUInt during "
                    "addition\n");
    return NULL;
  }

  carry = 0;
  for(i=0; i<smallest->num_uints; i++){
    sum = ((uint64_t)largest->uint_array[i]) + 
          ((uint64_t)smallest->uint_array[i]) + carry;
    carry = sum >> 32;
    result->uint_array[i] = (uint32_t)sum;
  }

  for( ; i<largest->num_uints; i++){
    sum = ((uint64_t)largest->uint_array[i]) + carry;
    carry = sum >> 32;
    result->uint_array[i] = (uint32_t)sum;
  }

  result->uint_array[i] = (uint32_t)carry;
  result->num_uints = sigIntsInBigUInt(result);

  return result;
}
  

OBBigUInt * subtractBigUInts(OBBigUInt *minuend,
                             OBBigUInt *subtrahend){

  OBBigUInt  *twos_comp, *result, *fitted_result;
  
  if(!minuend || !subtrahend){
    fprintf(stderr, "OBBigUInt: Unexpected NULL argument(s) passed to "
                    "subtractBigUInts\n");
    return NULL;
  }

  if(compareBigUInts((obj *)minuend, (obj *)subtrahend) != OB_GREATER_THAN){
    result = createBigUIntFromNum(0);
    if(!result){
      fprintf(stderr, "OBBigUInt: Could not create zero OBBigUInt as result "
                      "in subtractBigUInts\n");
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

  release((obj *)twos_comp); /* destroy now unneeded twos_comp */
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
  if(((double)result->num_uints)/result->capacity < MEM_USAGE_RATIO){
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
  OBBigUInt *x, *shift_x, *y, *shift_y, *z, *tmp;

  /* OBBigUInts equalling halves of a and b */
  OBBigUInt *a1, *a0, *b1, *b0;

  /* final result */
  OBBigUInt *result;

  /* init pointers */
  tmp = shift_x = shift_y = x = y = z = a1 = b1 = a0 = b0 = result = NULL;

  if(!a || !b){
    fprintf(stderr, "OBBigUInt: Cannot multiply NULL argument(s)\n");
    return NULL;
  }

  /* base case, BigUInts are small enough to compute directly */
  if(a->num_uints == 1 && b->num_uints == 1){
    result = createBigUIntWithCap(2);
    if(!result){
      fprintf(stderr, "OBBigUInt: Could not create result\n");
      return NULL;
    }

    mult_result = ((uint64_t)a->uint_array[0])*((uint64_t)b->uint_array[0]);
    result->uint_array[0] = (uint32_t)mult_result;
    result->uint_array[1] = (uint32_t)(mult_result>>32);
    result->num_uints = sigIntsInBigUInt(result);

    return result;
  }

  /* NULL results from partial calculations will be ignored form this point on,
   * they will just cause all other partial calculations affected to also return
   * NULL */

  /* determine the position where a and b should be split in half*/
  if(a->num_uints > b->num_uints) m=a->num_uints/2;
  else m=b->num_uints/2;

  /* split into halves depending on the size of a */
  if(a->num_uints > m){
    a0 = createBigUIntFromIntArray(a->uint_array, m);
    a1 = createBigUIntFromIntArray(a->uint_array + m,
                                   a->num_uints - m);
  }
  else{
    a0 = copyBigUInt(a);
    a1 = createBigUIntFromNum(0);
  }

  /* do the same for b */
  if(b->num_uints > m){
    b0 = createBigUIntFromIntArray(b->uint_array, m); 
    b1 = createBigUIntFromIntArray(b->uint_array + m,
                                   b->num_uints - m);
  }
  else{
    b0 = copyBigUInt(b);
    b1 = createBigUIntFromNum(0);
  }
  
  /* calculate partial multiplications, freeing unneeded memory as soon as
   * possible */ 

 /* get the y = (a0 + a1)(b0 + b1) part */
  x = addBigUInts(a1,a0);
  z = addBigUInts(b1,b0);
  y = multiplyBigUInts(x,z);
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
  tmp = subtractBigUInts(y,x);
  release((obj *)y);
  y = subtractBigUInts(tmp,z);
  release((obj *)tmp);

  /* get result = x*B^(2m) + y*B^m + z, where B is 2^32, a single uint32_t */
  shift_x = shiftBigUIntLeft(x, 2*m, 0);
  release((obj *)x);
  shift_y = shiftBigUIntLeft(y, m, 0);
  release((obj *)y);
  tmp = addBigUInts(shift_x, shift_y);
  release((obj *)shift_x);
  release((obj *)shift_y);
  result = addBigUInts(tmp, z);
  release((obj *)tmp);
  release((obj *)z);

  if(!result){
    fprintf(stderr, "OBBigUInt: Intermediate operation(s) in multiplyBigUInts "
                    "failed, so multiply failed\n");
  }

  return result;
}


OBBigUInt * divideBigUInts(OBBigUInt *dividend, OBBigUInt *divisor){

  OBBigUInt *quotient, *seed;
  
  if(!dividend || !divisor){
    fprintf(stderr, "OBBigUInt: Unexpected NULL arguments passed to "
                    "divideBigUInts\n");
    return NULL;
  }

  seed = createBigUIntFromNum(0);
  if(!seed){
    fprintf(stderr, "OBBigUInt: Could not create 0 seed for quotient in "
                    "divideBigUInts\n");
    return NULL;
  }

  if(!(quotient = recursiveDivide(dividend, divisor, seed))){
    fprintf(stderr, "OBBigUInt: divideBigUInt failed\n");
  }

  release((obj *)seed);
  return quotient;
}


OBBigUInt * modBigUInts(OBBigUInt *dividend, OBBigUInt *divisor){

  OBBigUInt *remainder;
  
  if(!dividend || !divisor){
    fprintf(stderr, "OBBigUInt: Unexpected NULL argument(s) passed to "
                    "modBigUInts\n");
    return NULL;
  }

  if(!(remainder = recursiveDivide(dividend, divisor, NULL))){
    fprintf(stderr, "OBBigUInt: modBigUInt failed\n");
  }

  return remainder;
}


OBBigUInt * shiftBigUIntLeft(OBBigUInt *a, uint64_t uint32_shifts,
                             uint8_t bit_shift){

  uint64_t capacity, i;
  OBBigUInt *result;

  if(!a){
    fprintf(stderr, "OBBigUInt: Unexpected NULL argument passed to "
                    "shiftBigUIntLeft\n");
    return NULL;
  }

  /* force bit_shift to be a value from 0-31, add extra to uint32_shifts */
  uint32_shifts += bit_shift/32;
  bit_shift %= 32;

  /* if direction is positive, do a left shift (increase bit significance) */
  capacity = a->num_uints + uint32_shifts + 1;
  /* if addition overflow occured */
  if(capacity < a->num_uints){
    fprintf(stderr, "OBBigUInt: shiftBigUIntLeft exceeds past max capacity,  "
                    "attempting to return 0\n");
    return createBigUIntFromNum(0);
  }

  result = createBigUIntWithCap(capacity);
  if(!result){
    fprintf(stderr, "OBBigUInt: Could not create result BigUInt in "
                    "shiftBigUInt\n");
    return NULL;
  }

  /* for all values less than a (i cycles after 0, back to UINT64_MAX) */
  for(i=a->num_uints-1; i<UINT64_MAX; i--){
    if(bit_shift){
      result->uint_array[i+uint32_shifts+1] += a->uint_array[i]>>(32-bit_shift);
      result->uint_array[i+uint32_shifts] += a->uint_array[i] << bit_shift;
    }
    else result->uint_array[i+uint32_shifts] = a->uint_array[i];
  }

  result->num_uints = sigIntsInBigUInt(result);
  return result;
}


OBBigUInt * shiftBigUIntRight(OBBigUInt *a, uint64_t uint32_shifts,
                              uint8_t bit_shift){

  uint64_t capacity, i;
  OBBigUInt *result;

  if(!a){
    fprintf(stderr, "OBBigUInt: Unexpected NULL argument passed to "
                    "shiftBigUIntLeft\n");
    return NULL;
  }

  /* force bit_shift to be a value from 0-31, add extra to uint32_shifts */
  uint32_shifts += bit_shift/32;
  bit_shift %= 32;

  capacity = a->num_uints - uint32_shifts;
  /* if subtraction overflow occured */
  if(capacity > a->num_uints){
    return createBigUIntFromNum(0);
  }

  result = createBigUIntWithCap(capacity);
  if(!result){
    fprintf(stderr, "OBBigUInt: Could not create result BigUInt in "
                    "shiftBigUInt\n");
    return NULL;
  }

  for(i=a->num_uints-1; i>uint32_shifts; i--){
    if(bit_shift){
      result->uint_array[i-uint32_shifts] += a->uint_array[i] >> bit_shift;
      result->uint_array[i-uint32_shifts-1] += a->uint_array[i]<<(32-bit_shift);
    }
    else result->uint_array[i-uint32_shifts] = a->uint_array[i];
  }
  
  /* include final bits missed in for loop */
  if(bit_shift)
    result->uint_array[0] += a->uint_array[uint32_shifts] >> bit_shift;
  else result->uint_array[0] = a->uint_array[uint32_shifts];

  result->num_uints = sigIntsInBigUInt(result);
  return result;
}

OBBigUInt * andBigUInts(OBBigUInt *a, OBBigUInt *b){

  OBBigUInt *result;
  uint64_t i, capacity;

  if(!a || !b){
    fprintf(stderr, "OBBigUInt: Unexpected NULL arguments(s) passed to "
                    "andBigUInts\n");
    return NULL;
  }

  if(a->num_uints > b->num_uints) capacity = b->num_uints;
  else capacity = a->num_uints;

  result = createBigUIntWithCap(capacity);
  if(!result){
    fprintf(stderr, "OBBigUInt: Could not create result BigUInt in "
                    "andBigUInts\n");
    return NULL;
  }

  for(i=0; i<capacity; i++){
    result->uint_array[i] = a->uint_array[i] & b->uint_array[i];
  }

  result->num_uints = sigIntsInBigUInt(result);
  return result;
}
  

OBBigUInt * orBigUInts(OBBigUInt *a, OBBigUInt *b){

  OBBigUInt *result, *largest, *smallest;
  uint64_t i;

  if(!a || !b){
    fprintf(stderr, "OBBigUInt: Unexpected NULL arguments(s) passed to "
                    "orBigUInts\n");
    return NULL;
  }

  if(a->num_uints > b->num_uints){
    largest = a;
    smallest = b;
  }
  else{
    largest = b;
    smallest = a;
  }

  result = createBigUIntWithCap(largest->num_uints);
  if(!result){
    fprintf(stderr, "OBBigUInt: Could not create result BigUInt in "
                    "orBigUInts\n");
    return NULL;
  }

  for(i=0; i<largest->num_uints; i++){
    if(i < smallest->num_uints)      
      result->uint_array[i] = smallest->uint_array[i] | largest->uint_array[i];
    else result->uint_array[i] = largest->uint_array[i];
  }

  result->num_uints = sigIntsInBigUInt(result);
  return result;
}


OBBigUInt * xorBigUInts(OBBigUInt *a, OBBigUInt *b){

  OBBigUInt *result, *largest, *smallest;
  uint64_t i;

  if(!a || !b){
    fprintf(stderr, "OBBigUInt: Unexpected NULL arguments(s) passed to "
                    "xorBigUInts\n");
    return NULL;
  }

  if(a->num_uints > b->num_uints){
    largest = a;
    smallest = b;
  }
  else{
    largest = b;
    smallest = a;
  }

  result = createBigUIntWithCap(largest->num_uints);
  if(!result){
    fprintf(stderr, "OBBigUInt: Could not create result BigUInt in "
                    "xorBigUInts\n");
    return NULL;
  }

  for(i=0; i<largest->num_uints; i++){
    if(i < smallest->num_uints)      
      result->uint_array[i] = smallest->uint_array[i] ^ largest->uint_array[i];
    else result->uint_array[i] = largest->uint_array[i];
  }

  result->num_uints = sigIntsInBigUInt(result);
  return result;
}

OBBigUInt * notBigUInt(OBBigUInt *a){

  OBBigUInt *result;
  uint64_t i;

  if(!a){
    fprintf(stderr, "OBBigUInt: Unexpected NULL arguments passed to "
                    "notBigUInt\n");
    return NULL;
  }

  result = createBigUIntWithCap(a->num_uints);
  if(!result){
    fprintf(stderr, "OBBigUInt: Could not create result BigUInt in "
                    "andBigUInts\n");
    return NULL;
  }

  for(i=0; i<a->num_uints; i++){
    result->uint_array[i] = ~(a->uint_array[i]);
  }

  result->num_uints = sigIntsInBigUInt(result);
  return result;
}

int8_t compareBigUInts(obj *a, obj *b){
  
  uint64_t i;
  OBBigUInt *comp_a = (OBBigUInt *)a;  
  OBBigUInt *comp_b = (OBBigUInt *)b;  

  if(!a || !b){
    fprintf(stderr, "OBBigUInt: Unexpected NULL argument(s) passed to "
                    "compareBigUInts\n");
    return OB_COMPARE_ERR;
  }

  /* compare size of BigUInts first */
  if(comp_a->num_uints > comp_b->num_uints) return OB_GREATER_THAN;
  else if(comp_a->num_uints < comp_b->num_uints) return OB_LESS_THAN;
  else{ /*if size is equivalent, compare each individual element */
    for(i = comp_a->num_uints - 1; i<comp_a->num_uints; i--){
      if(comp_a->uint_array[i] > comp_b->uint_array[i]) return OB_GREATER_THAN;
      else if(comp_a->uint_array[i] < comp_b->uint_array[i]) 
        return OB_LESS_THAN;
    }
  }

  /* all entries in BigUInts must be equal to reach this point */
  return OB_EQUAL_TO;
}

void printBigUInt(OBBigUInt *a){
  
  uint64_t i, j;
  char hex[9];

  printf("OBBigUInt with %llu 32 bit words\n", a->num_uints);
  printf("Most Significant 32 Bit Word\n");
  for(i=a->num_uints-1; i<a->num_uints; i--){
    sprintf(hex, "%x", a->uint_array[i]);
    printf("  0x");
    for(j=0; j<8-strlen(hex); j++){
      printf("0");
    }
    printf("%s\n", hex);
  }
  printf("Least Significant 32 Bit Word\n");
  return;
}


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

  new_instance->num_uints = 1;
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
  
  uint64_t i, maxcap;
  OBBigUInt *twos_comp;

  if(total_len > a->num_uints) maxcap = total_len;
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
      twos_comp->uint_array[i] = (~a->uint_array[i]) + 1;
      break;
    }
    /* havent found least significant non-zero int */
    twos_comp->uint_array[i] = 0;
  }

  /* not all remaining ints in a */
  for(i++; i<a->num_uints; i++){
    twos_comp->uint_array[i] = ~(a->uint_array[i]);
  }

  /* set any extra capacity space to all 1's (sign extension */
  for(; i<twos_comp->capacity; i++){
    twos_comp->uint_array[i] = UINT32_MAX;
  }

  twos_comp->num_uints = sigIntsInBigUInt(twos_comp);
  return twos_comp;
}


OBBigUInt * recursiveDivide(OBBigUInt *dividend, OBBigUInt *divisor,
                            OBBigUInt *quotient){
  
  uint64_t approx_quotient, approx_dividend, approx_divisor, approx_capacity;
  OBBigUInt *result, *approximate, *check_num, *new_dividend;
  
  result = approximate = check_num = new_dividend = NULL;

  /*base case, dividend is smaller than divisor, result is 0 */
  if(compareBigUInts((obj *)dividend, (obj *)divisor) == OB_LESS_THAN){
    if(quotient){
      result = copyBigUInt(quotient);
    }
    else{
      result = copyBigUInt(dividend);
    }
    return result;
  }

  /* approximate dividend with first two significant digits, and divisor with
   * first significant digit */
  approx_dividend = (uint64_t)dividend->uint_array[dividend->num_uints-1];
  approx_dividend <<= 32;
  approx_dividend += (uint64_t)dividend->uint_array[dividend->num_uints-2];
  approx_divisor = (uint64_t)divisor->uint_array[divisor->num_uints-1];
  
  /* compute the approximate solution */
  approx_quotient = approx_dividend/approx_divisor;
  if(approx_quotient == 0) approx_quotient = 1; /*fix case when approximations 
                                                  nearly equally or do divide*/
  approx_capacity = dividend->num_uints - divisor->num_uints;

  do{
    /* release previous bad approximates, or NULL if first iteration */
    release((obj *)approximate);
    release((obj *)check_num);

    /* create approximate with one extra space for large approx_quotient */
    approximate = createBigUIntWithCap(approx_capacity + 1);
    if(!approximate){
      fprintf(stderr, "OBBigUInt: Could not create approximate solution "
                      "in recursiveDivide\n");
      return NULL;
    }
    approximate->uint_array[approx_capacity - 1] = (uint32_t)approx_quotient;
    approximate->uint_array[approx_capacity] = (uint32_t)(approx_quotient>>32);
    approximate->num_uints = sigIntsInBigUInt(approximate);

    /* lower approx quotient in case approximate division is too great. Decrease
     * by about 1/64 of approx quotient, extra terms help maintain approximate
     * proportions as approx approaches 1*/
    approx_quotient -=(approx_quotient/2 - approx_quotient/4 - approx_quotient/8
                      - approx_quotient/16 - approx_quotient/32
                      - approx_quotient/64);
    if(approx_quotient == 0) approx_quotient = 1; /* fix case when approximation
                                                     should be 1 but misses */
    
    check_num = multiplyBigUInts(divisor, approximate);
    if(!check_num){
      fprintf(stderr, "OBBigUInt: Could not create check against dividend "
                      "in recursiveDivide\n");
      release((obj *)approximate);
      return NULL;
    }
  }while(compareBigUInts((obj *)check_num, (obj *)dividend) == OB_GREATER_THAN);

  /* if returning quotient then add approximate to current quotient, else dont
   * bother for modulus operation*/
  if(quotient){
    result = addBigUInts(quotient, approximate);
    if(!result){
      fprintf(stderr, "OBBigUInt: Could not create newer quotient "
                      "approximation in recursiveDivide\n");
      release((obj *)approximate);
      release((obj *)check_num);
      release((obj *)quotient);
      return NULL;
    }
    release((obj *)quotient);
  }
  
  release((obj *)approximate);
  
  /* continue to calculate quotient to next degree of accurary */
  new_dividend = subtractBigUInts(dividend, check_num);
  release((obj *)check_num);
  if(!new_dividend){
    fprintf(stderr, "OBBigUInt: Could not create newest dividend in "
                    "recursiveDivide\n");
    release((obj *)result);
    return NULL;
  }

  approximate = result;
  result = recursiveDivide(new_dividend, divisor, approximate);
  release((obj *)approximate);
  release((obj *)new_dividend);
  return result;
}


void deallocBigUInt(obj *to_dealloc){
  /* cast generic obj to OBBigUInt */
  OBBigUInt *instance = (OBBigUInt *)to_dealloc;
  free(instance->uint_array);
  free(instance);
  return;
}
