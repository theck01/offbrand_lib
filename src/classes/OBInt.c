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
OBInt * createDefaultInt(uint64_t num_digits){

  static const char classname[] = "OBInt";
  OBInt *new_instance = malloc(sizeof(OBInt));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocInt, &hashInt, &compareInts, 
           &displayInt, classname);


  new_instance->sign = 1; /* positive by default */

  new_instance->digits = malloc(sizeof(uint8_t)*num_digits);
  assert(new_instance->digits != NULL);
  memset(new_instance->digits, 0, num_digits);

  new_instance->num_digits = num_digits;

  return new_instance;
}


obhash_t hashInt(const obj *to_hash){

  static int8_t init = 0;
  static obhash_t seed = 0;

  uint64_t i;
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

  i = mostSigNonZero(instance);

  /* A version of Jenkin's one at a time hash function */
  for( ; i<instance->num_digits; i--){
    value += instance->digits[i];
    value += value << 10;
    value ^= value >> 6;
  }

  value += value << 3;
  value ^= value >> 11;
  value *= value << 15;
  return value;
}


int8_t compareInts(const obj *a, const obj *b){
  
  uint64_t i,j;
  const OBInt *comp_a = (OBInt *)a;  
  const OBInt *comp_b = (OBInt *)b;  

  assert(a);
  assert(b);
  assert(objIsOfClass(a, "OBInt"));
  assert(objIsOfClass(b, "OBInt"));

  i = mostSigNonZero(comp_a);
  j = mostSigNonZero(comp_b);

  if(i<j) return OB_LESS_THAN;
  if(i>j) return OB_GREATER_THAN;

  /* number of digits is equal, resort to digit by digit comparision */
  for( ; i<comp_a->num_digits; i--){
    if(comp_a->digits[i] < comp_b->digits[i]) return OB_LESS_THAN;
    if(comp_a->digits[i] > comp_b->digits[i]) return OB_LESS_THAN;
  }

  /* all digits are equal, OBInts must be equal */
  return OB_EQUAL_TO;
}


void displayInt(const obj *to_print){

  OBString *str;
  const OBInt *instance = (OBInt *)to_print;
  
  assert(to_print);
  assert(objIsOfClass(to_print, "OBInt"));

  str = stringFromInt(instance);
  fprintf(stderr, "OBInt with value:\n  %s\n", getCString(str));
  
  release((obj *)str);

  return;
}


void deallocInt(obj *to_dealloc){

  /* cast generic obj to OBInt */
  OBInt *instance = (OBInt *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBInt"));

  free(instance->digits);

  return;
}


OBInt * addUnsignedInts(const OBInt *a, const OBInt *b){

  uint64_t i, large_most_sig, small_most_sig;
  uint8_t carry = 0;
  OBInt *result;
  const OBInt *larger, *smaller;

  larger = mostSigNonZero(a) > mostSigNonZero(b) ? a : b;
  smaller = mostSigNonZero(a) <= mostSigNonZero(b) ? a : b;

  large_most_sig = mostSigNonZero(larger);
  small_most_sig = mostSigNonZero(smaller);

  /* reserve enough space for the result, +2 required for actual size of larger
   * and possible extra digit needed to catch any addition overflow */
  result = createDefaultInt(large_most_sig+2);

  for(i=0; i<= small_most_sig; i++){
    result->digits[i] = (larger->digits[i] + smaller->digits[i] + carry)%10;
    carry = (larger->digits[i] + smaller->digits[i] + carry)/10;
  }

  for( ; i<= large_most_sig; i++){
    result->digits[i] = (larger->digits[i] + carry)%10;
    carry = (larger->digits[i] + carry)/10;
  }

  result->digits[i] = carry;

  return result;
}


OBInt * subtractUnsignedInts(const OBInt *a, const OBInt *b){

  uint64_t i, j, a_most_sig, b_most_sig;
  OBInt *result;

  a_most_sig = mostSigNonZero(a);

  result = createDefaultInt(a_most_sig);
  memcpy(result->digits, a->digits, a_most_sig);

  b_most_sig = mostSigNonZero(b);

  for(i=0; i<b_most_sig; i++){

    /* perform borrow operation if needed */
    if(result->digits[i] < b->digits[i]){
      j = i+1;
      while(!result->digits[j]) j++;
      result->digits[j]--;
      for(--j; j>i; j--) result->digits[j] += 9;
      result->digits[j] += 10;
    }

    result->digits[i] -= b->digits[i];
  }

  return result;
}


OBInt * multiplyUnsignedInts(const OBInt *a, const OBInt *b){

  uint64_t large_most_sig, small_most_sig, split_point;
  const OBInt *larger, *smaller;
  OBInt *x0, *x1, *y0, *y1; /* split versions of a and b */
  OBInt *z0, *z1, *z1a, *z1b, *z1c, *z1d, *z2; /* partial results of 
                                                  Karatsuba algorithm */
  OBInt *partial_result, *result;

  larger = mostSigNonZero(a) > mostSigNonZero(b) ? a : b;
  smaller = mostSigNonZero(a) <= mostSigNonZero(b) ? a : b;

  large_most_sig = mostSigNonZero(larger);
  small_most_sig = mostSigNonZero(smaller);

  /* base case, a and b are single digits */
  if(large_most_sig == 1 && small_most_sig == 1) 
    return createIntWithInt(a->digits[0]*b->digits[0]);

  split_point = large_most_sig/2;
  splitInt(a, split_point, &x1, &x0);
  splitInt(b, split_point, &y1, &y0);

  z2 = multiplyUnsignedInts(x1, y1);
  z0 = multiplyUnsignedInts(x0, y0);

  z1a = addUnsignedInts(x1, x0);
  z1b = addUnsignedInts(x1, x0);
  z1c = multiplyUnsignedInts(z1a, z1b);

  release((obj *)z1a);
  release((obj *)z1b);

  z1d = subtractUnsignedInts(z1c, z2);
  release((obj *)z1c);
  z1 = subtractUnsignedInts(z1d, z0);
  release((obj *)z1d);

  shiftInt(z2, 2*split_point);
  shiftInt(z1, split_point);
  partial_result = addUnsignedInts(z2, z1);

  release((obj *) z2);
  release((obj *) z1);

  result = addUnsignedInts(partial_result, z0);
  
  release((obj *) partial_result);
  release((obj *) z0);

  return result;
}


OBInt * reduceUnsignedInts(const OBInt *a, const OBInt *b, const OBInt *c,
                           uint8_t quotient_or_remainder){

  

}



uint64_t mostSigNonZero(const OBInt *a){

  uint64_t i;

  i = a->num_digits - 1;
  while(!a->digits[i] && i>0) i--;
  return i;
}


void splitInt(const OBInt *a, uint64_t i, OBInt **b1, OBInt **b0){

  if(a->num_digits >= i){
    *b1 = createDefaultInt(1);
    *b0 = createDefaultInt(a->num_digits);
    memcpy((*b0)->digits, a->digits, a->num_digits);
  }
  else{
    *b1 = createDefaultInt(a->num_digits - i);
    memcpy((*b1)->digits, (a->digits + i), a->num_digits-i);
    *b0 = createDefaultInt(i);
    memcpy((*b0)->digits, (a->digits), i);
  }

  return;
}


void shiftInt(OBInt *a, uint64_t m){

  uint8_t *digits = malloc(sizeof(uint8_t)*(a->num_digits + m));
  assert(digits);

  memset(digits, 0, m);
  memcpy(digits+m, a->digits, a->num_digits);

  free(a->digits);
  a->digits = digits;
  a->num_digits += m;

  return;
}
