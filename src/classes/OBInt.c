/**
 * @file OBInt.c
 * @brief OBInt Method Implementation
 * @author theck
 */

#include "../../include/OBInt.h"
#include "../../include/private/OBInt_Private.h"

/** maximum number of decimal digits to operate on as one int64_t */
uint8_t int64_max_digits = 17;

/* PUBLIC METHODS */

OBInt * OBIntCreate(int64_t num){
  
  uint64_t i, ds;
  int64_t partial_num;
  OBInt *instance;

  /* find the number of digits in the primitive, ensuring at least one for 0
   * case */
  ds = 1;
  partial_num = num/10;
  while(partial_num){
    partial_num /= 10;
    ds++;
  }

  ds = ds ? ds : 1;

  instance = createDefaultInt(ds);
  if(num < 0){
    instance->sign = -1;
    num *= -1;
  }

  for(i = 0; i < ds; i++){
    instance->digits[i] = num%10;
    num /= 10;
  }

  return instance;
}


int64_t OBIntGetIntValue(const OBInt *a){

  uint64_t i, most_sig;
  int64_t val;

  assert(a != NULL);

  most_sig = mostSig(a);
  val = 0;
  for(i=most_sig; i<= most_sig; i--){
    val *= 10;
    val += a->digits[i];
  }

  return val*a->sign;
}


OBInt * OBIntCreateFromString(const OBString *numstr){ 

  int8_t offset, sign;

  int32_t num;
  size_t strlen, i;
  uint64_t digits;
  OBInt *instance;

  assert(numstr != NULL);

  strlen = stringLength(numstr);
  if(charAtStringIndex(numstr, 0) == '-'){
    sign = -1;
    digits = strlen - 1;
    offset = 1;
  }
  else{
    sign = 1;
    digits = strlen;
    offset = 0;
  }
  assert(strlen > 0);

  instance = createDefaultInt(digits);
  instance->sign = sign;
  for(i=0 ; i<digits; i++){
    num = charAtStringIndex(numstr, i+offset) - '0';
    assert(num >= 0 && num <= 9);
    instance->digits[digits-i-1] = num;
  }

  /* zero is defined to be positive */
  if(OBIntIsZero(instance)) instance->sign = 1;

  return instance;
}


OBString * OBIntGetStringValue(const OBInt *a){

  uint8_t offset = 0;
  uint64_t most_sig, i;
  char *str;
  OBString *result;

  assert(a != NULL);

  if(a->sign == -1) offset = 1;
  most_sig = mostSig(a);
  str = malloc(sizeof(char)*(most_sig+2+offset));

  /* generate cstring from int */
  str[0] = '-'; /* non-negatives will overwrite value in following loop */
  for(i=most_sig; i<=most_sig; i--) str[most_sig-i+offset] = a->digits[i] + '0';
  str[most_sig+1+offset] = '\0';

  result = createString(str);
  free(str);

  return result;
}


OBInt * OBIntCopy(const OBInt *a){

  uint64_t i, most_sig;
  OBInt *copy;

  assert(a != NULL);

  most_sig = mostSig(a);

  copy = createDefaultInt(most_sig+1);
  for(i=0; i<= most_sig; i++) copy->digits[i] = a->digits[i];
  copy->sign = a->sign;

  return copy;
}


uint8_t OBIntIsZero(const OBInt *a){

  uint64_t most_sig;

  assert(a != NULL);

  most_sig = mostSig(a);
  return most_sig == 0 && a->digits[0] == 0;
}


uint8_t OBIntIsNegative(const OBInt *a){ 
  assert(a != NULL);
  return a->sign == -1;
}


OBInt * OBIntAdd(const OBInt *a, const OBInt *b){ 

  int8_t cmp_result;
  OBInt *result;

  assert(a != NULL);
  assert(b != NULL);

  /* a and b are  of same sign */
  if((OBIntIsNegative(a) && OBIntIsNegative(b)) || 
     (!OBIntIsNegative(a) && !OBIntIsNegative(b))){
    result = addUnsignedInts(a,b);
    result->sign = a->sign;
    return result;
  }

  /* a and b are of opposite sign */
  cmp_result = compareMagnitudes(a,b);
  if(cmp_result == OB_LESS_THAN){
    result = subtractUnsignedInts(b,a);
    result->sign = b->sign;
    return result;
  }
  else if(cmp_result == OB_GREATER_THAN){
    result = subtractUnsignedInts(a,b);
    result->sign = a->sign;
    return result;
  }

  /* a and b are equal and opposite, result is 0 */
  return OBIntCreate(0);
}


OBInt * OBIntAddPrimitive(const OBInt *a, int64_t b){ 

  OBInt *wrapper, *result;

  assert(a != NULL);

  wrapper = OBIntCreate(b);
  result = OBIntAdd(a, wrapper);
  OBRelease((OBObjType *)wrapper);

  return result;
}


OBInt * OBIntSubtract(const OBInt *a, const OBInt *b){ 

  int8_t cmp_result;
  OBInt *result;

  assert(a != NULL);
  assert(b != NULL);

  /* a and b are  of opposite sign */
  if((!OBIntIsNegative(a) && OBIntIsNegative(b)) || 
     (OBIntIsNegative(a) && !OBIntIsNegative(b))){
    result = addUnsignedInts(a,b);
    result->sign = a->sign;
    return result;
  }

  /* a and b are of same sign */
  cmp_result = compareMagnitudes(a,b);
  if(cmp_result == OB_LESS_THAN){
    result = subtractUnsignedInts(b,a);
    result->sign = b->sign * -1;
    return result;
  }
  else if(cmp_result == OB_GREATER_THAN){
    result = subtractUnsignedInts(a,b);
    result->sign = a->sign;
    return result;
  }

  /* a and b are equal and same, result is 0 */
  return OBIntCreate(0);
}


OBInt * OBIntSubtractPrimitive(const OBInt *a, int64_t b){

  OBInt *wrapper, *result;

  assert(a != NULL);

  wrapper = OBIntCreate(b);
  result = OBIntSubtract(a, wrapper);
  OBRelease((OBObjType *)wrapper);

  return result;
}


OBInt * OBIntMultiply(const OBInt *a, const OBInt *b){

  OBInt *result;

  assert(a != NULL);
  assert(b != NULL);

  result = multiplyUnsignedInts(a,b);
  result->sign = a->sign * b->sign;

  return result;
}


OBInt * OBIntMultiplyPrimitive(const OBInt *a, int64_t b){

  OBInt *wrapper, *result;

  assert(a != NULL);

  wrapper = OBIntCreate(b);
  result = OBIntMultiply(a, wrapper);
  OBRelease((OBObjType *)wrapper);

  return result;
}


OBInt * OBIntDivide(const OBInt *a, const OBInt *b){

  OBInt *result, *seed;

  assert(a != NULL);
  assert(b != NULL);
  assert(OBIntIsZero(b) == 0);

  seed = OBIntCreate(0);
  result = reduceUnsignedInts(a, b, seed, 1);
  result->sign = a->sign * b->sign;
  OBRelease((OBObjType *)seed);

  return result;
}


OBInt * OBIntDividePrimitive(const OBInt *a, int64_t b){

  OBInt *wrapper, *result;

  assert(a != NULL);
  assert(b != 0);

  wrapper = OBIntCreate(b);
  result = OBIntDivide(a, wrapper);
  OBRelease((OBObjType *)wrapper);

  return result;
}


OBInt * OBIntMod(const OBInt *a, const OBInt *b){

  OBInt *result, *seed;

  assert(a != NULL);
  assert(b != NULL);
  assert(OBIntIsZero(b) == 0);

  seed = OBIntCreate(0);
  result = reduceUnsignedInts(a, b, seed, 0);
  result->sign = a->sign;
  OBRelease((OBObjType *)seed);

  return result;
}


OBInt * OBIntModPrimitive(const OBInt *a, int64_t b){

  OBInt *wrapper, *result;

  assert(a != NULL);
  assert(b != 0);

  wrapper = OBIntCreate(b);
  result = OBIntMod(a, wrapper);
  OBRelease((OBObjType *)wrapper);

  return result;
}


/* PRIVATE METHODS */

/* add arguments to complete initialization as needed, modify 
 * OBInt_Private.h as well if modifications are made */
OBInt * createDefaultInt(uint64_t num_digits){

  static const char classname[] = "OBInt";
  OBInt *new_instance = malloc(sizeof(OBInt));
  assert(new_instance != NULL);

  /* initialize base class data */
  OBInitBase((OBObjType *)new_instance, &deallocInt, &hashInt, &compareInts, 
           &displayInt, classname);

  new_instance->sign = 1; /* positive by default */

  new_instance->digits = malloc(sizeof(int8_t)*num_digits);
  assert(new_instance->digits != NULL);
  memset(new_instance->digits, 0, num_digits);

  new_instance->num_digits = num_digits;

  return new_instance;
}


obhash_t hashInt(OBTypeRef to_hash){

  static int8_t init = 0;
  static obhash_t seed = 0;

  uint64_t i;
  obhash_t value;
  OBInt *instance = (OBInt *)to_hash;

  assert(to_hash);
  assert(OBObjIsOfClass(to_hash, "OBInt"));

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


int8_t compareInts(OBTypeRef a, OBTypeRef b){
  
  int8_t magnitude_comp;
  const OBInt *comp_a = (OBInt *)a;  
  const OBInt *comp_b = (OBInt *)b;  

  assert(a);
  assert(b);
  assert(OBObjIsOfClass(a, "OBInt"));
  assert(OBObjIsOfClass(b, "OBInt"));

  /* if signs are equal magnitude comparision is required */
  if(comp_a->sign == comp_b->sign){

    magnitude_comp = compareMagnitudes(comp_a, comp_b);

    /* reverse magnitude comparision if both are negative */
    if(comp_a->sign == -1){
      if(magnitude_comp == OB_LESS_THAN) magnitude_comp = OB_GREATER_THAN;
      else if(magnitude_comp == OB_GREATER_THAN) magnitude_comp = OB_LESS_THAN;
    }

    return magnitude_comp;
  }

  if(comp_a->sign == -1) return OB_LESS_THAN;
  return OB_GREATER_THAN;
}


int8_t compareMagnitudes(const OBInt *a, const OBInt *b){

  uint64_t i,j;

  i = mostSig(a);
  j = mostSig(b);

  if(i < j) return OB_LESS_THAN;
  if(i > j) return OB_GREATER_THAN;

  /* number of digits is equal, resort to digit by digit comparision */
  for( ; i<a->num_digits; i--){
    if(a->digits[i] < b->digits[i]) return OB_LESS_THAN;
    if(a->digits[i] > b->digits[i]) return OB_GREATER_THAN;
  }

  return OB_EQUAL_TO;
}


void displayInt(OBTypeRef to_print){

  OBString *str;
  const OBInt *instance = (OBInt *)to_print;
  
  assert(to_print);
  assert(OBObjIsOfClass(to_print, "OBInt"));

  str = OBIntGetStringValue(instance);
  fprintf(stderr, "Value:\n  %s\n", getCString(str));
  
  OBRelease((OBObjType *)str);

  return;
}


void deallocInt(OBTypeRef to_dealloc){

  /* cast generic obj to OBInt */
  OBInt *instance = (OBInt *)to_dealloc;

  assert(to_dealloc);
  assert(OBObjIsOfClass(to_dealloc, "OBInt"));

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

  result = OBIntCopy(a);

  a_most_sig = mostSig(a);
  b_most_sig = mostSig(b);

  for(i=0; i<=b_most_sig; i++){
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
    return OBIntCreate(unsignedValue(a) * unsignedValue(b));

  split_point = large_most_sig/2+1;
  splitInt(a, split_point, &x1, &x0);
  splitInt(b, split_point, &y1, &y0);

  z2 = multiplyUnsignedInts(x1, y1);
  z0 = multiplyUnsignedInts(x0, y0);

  z1a = addUnsignedInts(x1, x0);
  z1b = addUnsignedInts(y1, y0);
  z1c = multiplyUnsignedInts(z1a, z1b);

  OBRelease((OBObjType *)x0);
  OBRelease((OBObjType *)x1);
  OBRelease((OBObjType *)y0);
  OBRelease((OBObjType *)y1);
  OBRelease((OBObjType *)z1a);
  OBRelease((OBObjType *)z1b);

  z1d = subtractUnsignedInts(z1c, z2);
  OBRelease((OBObjType *)z1c);
  z1 = subtractUnsignedInts(z1d, z0);
  OBRelease((OBObjType *)z1d);

  shiftInt(z2, 2*split_point);
  shiftInt(z1, split_point);
  partial_result = addUnsignedInts(z2, z1);

  OBRelease((OBObjType *) z2);
  OBRelease((OBObjType *) z1);

  result = addUnsignedInts(partial_result, z0);
  
  OBRelease((OBObjType *) partial_result);
  OBRelease((OBObjType *) z0);

  return result;
}


OBInt * reduceUnsignedInts(const OBInt *a, const OBInt *b, const OBInt *approx,
                           uint8_t quotient){
  
  int8_t comp_val;
  int64_t a_val, b_val, result_val;
  uint64_t a_most_sig, b_most_sig, a_min, b_min, i, j;
  OBInt *partial; /* partial results */
  OBInt *product; /* quotient approximation multiplied by b */
  OBInt *new_approx; /* improved approximation */
  OBInt *new_dividend; /* reduced dividend */
  OBInt *result; /* approximation */

  a_most_sig = mostSig(a);
  b_most_sig = mostSig(b);

  /* if operation can be computed using integer arithmetic do so */
  if(a_most_sig < int64_max_digits && b_most_sig < int64_max_digits){

    a_val = unsignedValue(a);
    b_val = unsignedValue(b);

    if(quotient){
      result_val = a_val/b_val;
      partial = OBIntCreate(result_val);
      result = OBIntAdd(approx, partial);
      OBRelease((OBObjType *)partial);
    }
    else{
      result_val = a_val%b_val;
      result = OBIntCreate(result_val);
    }

    return result;
  }

  /* if a < b (so a/b = 0, a%b = a) */
  comp_val = compareMagnitudes(a, b);
  if(comp_val == OB_LESS_THAN){
    if(quotient) return OBIntCopy(approx);
    else return OBIntCopy(a);
  }
  /* if a == b (so a/b = 1, a%b = 0) */
  else if(comp_val == OB_EQUAL_TO){
    if(quotient) return OBIntAddPrimitive(approx,1);
    else return OBIntCreate(0);
  }

  /* else recursive division approximation */

  /* generate machine integers for approximation */

  /* generate b_val to have one less digit than int64_max_digits, to ensure 
   * b_val < a_val */
  if(b_most_sig > (uint64_t)(int64_max_digits - 1)){
    b_min = b_most_sig - (int64_max_digits - 1) + 1;
  }
  else{
    b_min = 0;
  }
  b_val = 0;
  for(j=b_most_sig; j>=b_min && j<b->num_digits; j--){
    b_val *= 10;
    b_val += b->digits[j];
  }

  a_val = 0;
  a_min = a_most_sig > int64_max_digits ? a_most_sig-int64_max_digits+1 : 0;

  /* a_val approximation must be cutoff at the same point as b_val 
   * approximation, to protect against overflow */
  if(a_min < b_min) a_min = b_min;
  for(i=a_most_sig; i>=a_min && i<a->num_digits; i--){
    a_val *= 10;
    a_val += a->digits[i];
  }

  /* perform approximation division, shift into proper decimal place, and add
   * to approximation */
  partial = OBIntCreate(a_val/b_val);
  shiftInt(partial, i-j);
  product = multiplyUnsignedInts(partial, b);

  comp_val = compareMagnitudes(product, a);
  
  /* if approximation is too great then increment b_val to account for remaining
   * portion of b that is causing over approximation */
  if(comp_val == OB_GREATER_THAN){
    OBRelease((OBObjType *)partial);
    OBRelease((OBObjType *)product);

    b_val++; 
    partial = OBIntCreate(a_val/b_val);
    shiftInt(partial, i-j);
    product = multiplyUnsignedInts(partial, b);
  }

  new_dividend = subtractUnsignedInts(a, product);
  OBRelease((OBObjType *)product);
  new_approx = addUnsignedInts(approx, partial);
  OBRelease((OBObjType *)partial);

  result = reduceUnsignedInts(new_dividend, b, new_approx, quotient);

  OBRelease((OBObjType *)new_approx);
  OBRelease((OBObjType *)new_dividend);

  return result;
}


uint64_t mostSig(const OBInt *a){

  uint64_t i;

  i = a->num_digits - 1;
  while(!a->digits[i] && i>0) i--;
  return i;
}


void splitInt(const OBInt *a, uint64_t i, OBInt **b1, OBInt **b0){

  if(a->num_digits <= i){
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

  int8_t *digits;

  /* if shifting by zero do nothing */
  if(m == 0) return;
  
  digits = malloc(sizeof(int8_t)*(a->num_digits + m));
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
  if(digits >= 2 && digits <= 17) int64_max_digits = digits;
}
