/**
 * @file OBInt.c
 * @brief OBInt Method Implementation
 * @author theck
 */

#include "../../include/OBInt.h"
#include "../../include/private/OBInt_Private.h"

/* maximum number of decimal digits in a int64_t */
uint8_t int64_max_digits = 17;

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

  new_instance->digits = malloc(sizeof(int8_t)*num_digits);
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

  i = mostSig(instance);

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

  i = mostSig(comp_a);
  j = mostSig(comp_b);

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
  int8_t carry = 0;
  OBInt *result;
  const OBInt *larger, *smaller;

  larger = mostSig(a) > mostSig(b) ? a : b;
  smaller = mostSig(a) <= mostSig(b) ? a : b;

  large_most_sig = mostSig(larger);
  small_most_sig = mostSig(smaller);

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

  uint64_t i, a_most_sig, b_most_sig;
  OBInt *result;

  result = copyInt(a);

  a_most_sig = mostSig(a);
  b_most_sig = mostSig(b);

  for(i=0; i<b_most_sig; i++){
    /* perform borrow operation if needed */
    if(result->digits[i] < b->digits[i]){
      result->digits[i+1]--;
      result->digits[i] += 10;
    }

    result->digits[i] -= b->digits[i];
  }

  /* propagate any final borrow */
  while(i < a_most_sig && result->digits[i] < 0){
    result->digits[i+1]--;
    result->digits[i] += 10;
    i++;
  }

  return result;
}


OBInt * multiplyUnsignedInts(const OBInt *a, const OBInt *b){

  uint64_t large_most_sig, small_most_sig, split_point;
  const OBInt *larger, *smaller;
  OBInt *x0, *x1, *y0, *y1; /* split versions of a and b */
  OBInt *z0, *z1, *z1a, *z1b, *z1c, *z1d, *z2; /* partial results of Karatsuba 
                                                  algorithm */
  OBInt *partial_result, *result;

  larger = mostSig(a) > mostSig(b) ? a : b;
  smaller = mostSig(a) <= mostSig(b) ? a : b;

  large_most_sig = mostSig(larger);
  small_most_sig = mostSig(smaller);

  /* base case, a and b are single digits */
  if(large_most_sig + small_most_sig < int64_max_digits)
    return createIntWithInt(unsignedValue(a) * unsignedValue(b));

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


OBInt * reduceUnsignedInts(const OBInt *a, const OBInt *b, const OBInt *approx,
                           uint8_t quotient){
  
  int8_t comp_val;
  int64_t a_val, b_val, result_val;
  uint64_t a_most_sig, b_most_sig, i, j;
  OBInt *partial; /* partial results */
  OBInt *product; /* quotient approximation multiplied by b */
  OBInt *new_approx; /* improved approximation */
  OBInt *new_dividend; /* reduced dividend */
  OBInt *result; /* approximation */


  a_most_sig = mostSig(a);
  b_most_sig = mostSig(b);

  /* if operation can be computed outright do so */
  if(a_most_sig < int64_max_digits && b_most_sig < int64_max_digits){

    a_val = unsignedValue(a);
    b_val = unsignedValue(b);

    if(quotient) result_val = a_val/b_val;
    else result_val = a_val%b_val;

    return createIntWithInt(result_val);
  }

  /* if a < b (so a/b = 0, a%b = a) */
  comp_val = compareInts((obj *)a, (obj *)b);
  if(comp_val == OB_LESS_THAN){
    if(quotient) return copyInt(approx);
    else return copyInt(a);
  }
  /* if a == b (so a/b = 1, a%b = 0) */
  else if(comp_val == OB_EQUAL_TO){
    if(quotient) return addIntAndPrim(approx,1);
    else return createIntWithInt(0);
  }

  /* else recursive division approximation */

  /* generate machine integers for approximation */
  a_val = 0;
  for(i=a_most_sig; i>a_most_sig - int64_max_digits; i--){
    a_val *= 10;
    a_val += a->digits[i];
  }
  
  b_val = 0;
  for(j=b_most_sig; j>b_most_sig - int64_max_digits; j--){
    b_val *= 10;
    b_val += b->digits[j];
  }

  /* increment b_val to account for possible remaining portion in b to 
   * ensure that approximate result is not too great does not overflow */
  if(b_most_sig >= int64_max_digits) b_val++; 

  /* perform approximation division, shift into proper decimal place, and add
   * to approximation */
  partial = createIntWithInt(a_val/b_val);
  shiftInt(partial, i-j);
  product = multiplyUnsignedInts(partial, b);
  new_dividend = subtractUnsignedInts(a, product);
  release((obj *)product);
  new_approx = addUnsignedInts(a, partial);
  release((obj *)partial);

  result = reduceUnsignedInts(new_dividend, b, new_approx, quotient);

  release((obj *)new_approx);
  release((obj *)new_dividend);

  return result;
}


uint64_t mostSig(const OBInt *a){

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

  int8_t *digits = malloc(sizeof(int8_t)*(a->num_digits + m));
  assert(digits);

  memset(digits, 0, m);
  memcpy(digits+m, a->digits, a->num_digits);

  free(a->digits);
  a->digits = digits;
  a->num_digits += m;

  return;
}


int64_t unsignedValue(const OBInt *a){
  
  uint64_t i, most_sig;
  int64_t val;

  most_sig = mostSig(a);
  val = 0;

  for(i=most_sig ; i <= most_sig; i--){
    val *= 10;
    val += a->digits[i];
  }

  return val;
}


void setMaxDigits(uint8_t digits){
  if(digits >= 1 && digits <= 17) int64_max_digits = digits;
}
