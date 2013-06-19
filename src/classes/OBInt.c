/**
 * @file obint.c
 * @brief obint Method Implementation
 * @author theck
 */

#include "../../include/obint.h"
#include "../../include/private/obint_private.h"

/** maximum number of decimal digits to operate on as one int64_t */
uint8_t int64_max_digits = 17;

/* PUBLIC METHODS */

obint * obint_new(int64_t num){

  uint64_t i, ds;
  int64_t partial_num;
  obint *instance;

  /* find the number of digits in the primitive, ensuring at least one for 0
   * case */
  ds = 1;
  partial_num = num/10;
  while(partial_num){
    partial_num /= 10;
    ds++;
  }

  ds = ds ? ds : 1;

  instance = obint_create_default(ds);
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


int64_t obint_value(const obint *a){

  uint64_t i, most_sig;
  int64_t val;

  assert(a != NULL);

  most_sig = obint_most_sig(a);
  val = 0;
  for(i=most_sig; i<= most_sig; i--){
    val *= 10;
    val += a->digits[i];
  }

  return val*a->sign;
}


obint * obint_from_string(const obstring *numstr){

  int8_t offset, sign;

  int32_t num;
  uint32_t strlen, i;
  uint64_t digits;
  obint *instance;

  assert(numstr != NULL);

  strlen = obstring_length(numstr);
  if(obstring_char_at_index(numstr, 0) == '-'){
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

  instance = obint_create_default(digits);
  instance->sign = sign;
  for(i=0 ; i<digits; i++){
    num = obstring_char_at_index(numstr, i+offset) - '0';
    assert(num >= 0 && num <= 9);
    instance->digits[digits-i-1] = num;
  }

  /* zero is defined to be positive */
  if(obint_is_zero(instance)) instance->sign = 1;

  return instance;
}


obstring * obint_to_string(const obint *a){

  uint8_t offset = 0;
  uint64_t most_sig, i;
  char *str;
  obstring *result;

  assert(a != NULL);

  if(a->sign == -1) offset = 1;
  most_sig = obint_most_sig(a);
  str = malloc(sizeof(char)*(most_sig+2+offset));

  /* generate cstring from int */
  str[0] = '-'; /* non-negatives will overwrite value in following loop */
  for(i=most_sig; i<=most_sig; i--) str[most_sig-i+offset] = a->digits[i] + '0';
  str[most_sig+1+offset] = '\0';

  result = obstring_new(str);
  free(str);

  return result;
}


obint * obint_copy(const obint *a){

  uint64_t i, most_sig;
  obint *copy;

  assert(a != NULL);

  most_sig = obint_most_sig(a);

  copy = obint_create_default(most_sig+1);
  for(i=0; i<= most_sig; i++) copy->digits[i] = a->digits[i];
  copy->sign = a->sign;

  return copy;
}


uint8_t obint_is_zero(const obint *a){

  uint64_t most_sig;

  assert(a != NULL);

  most_sig = obint_most_sig(a);
  return most_sig == 0 && a->digits[0] == 0;
}


uint8_t obint_is_negative(const obint *a){
  assert(a != NULL);
  return a->sign == -1;
}


obint * obint_add(const obint *a, const obint *b){

  int8_t cmp_result;
  obint *result;

  assert(a != NULL);
  assert(b != NULL);

  /* a and b are  of same sign */
  if((obint_is_negative(a) && obint_is_negative(b)) ||
     (!obint_is_negative(a) && !obint_is_negative(b))){
    result = obint_add_unsigned(a,b);
    result->sign = a->sign;
    return result;
  }

  /* a and b are of opposite sign */
  cmp_result = obint_compare_magnitudes(a,b);
  if(cmp_result == OB_LESS_THAN){
    result = obint_subtract_unsigned(b,a);
    result->sign = b->sign;
    return result;
  }
  else if(cmp_result == OB_GREATER_THAN){
    result = obint_subtract_unsigned(a,b);
    result->sign = a->sign;
    return result;
  }

  /* a and b are equal and opposite, result is 0 */
  return obint_new(0);
}


obint * obint_add_primitive(const obint *a, int64_t b){

  obint *wrapper, *result;

  assert(a != NULL);

  wrapper = obint_new(b);
  result = obint_add(a, wrapper);
  ob_release((obj *)wrapper);

  return result;
}


obint * obint_subtract(const obint *a, const obint *b){

  int8_t cmp_result;
  obint *result;

  assert(a != NULL);
  assert(b != NULL);

  /* a and b are  of opposite sign */
  if((!obint_is_negative(a) && obint_is_negative(b)) ||
     (obint_is_negative(a) && !obint_is_negative(b))){
    result = obint_add_unsigned(a,b);
    result->sign = a->sign;
    return result;
  }

  /* a and b are of same sign */
  cmp_result = obint_compare_magnitudes(a,b);
  if(cmp_result == OB_LESS_THAN){
    result = obint_subtract_unsigned(b,a);
    result->sign = b->sign * -1;
    return result;
  }
  else if(cmp_result == OB_GREATER_THAN){
    result = obint_subtract_unsigned(a,b);
    result->sign = a->sign;
    return result;
  }

  /* a and b are equal and same, result is 0 */
  return obint_new(0);
}


obint * obint_subtract_primitive(const obint *a, int64_t b){

  obint *wrapper, *result;

  assert(a != NULL);

  wrapper = obint_new(b);
  result = obint_subtract(a, wrapper);
  ob_release((obj *)wrapper);

  return result;
}


obint * obint_multiply(const obint *a, const obint *b){

  obint *result;

  assert(a != NULL);
  assert(b != NULL);

  result = obint_multiply_unsigned(a,b);
  result->sign = a->sign * b->sign;

  return result;
}


obint * obint_multiply_primitive(const obint *a, int64_t b){

  obint *wrapper, *result;

  assert(a != NULL);

  wrapper = obint_new(b);
  result = obint_multiply(a, wrapper);
  ob_release((obj *)wrapper);

  return result;
}


obint * obint_divide(const obint *a, const obint *b){

  obint *result, *seed;

  assert(a != NULL);
  assert(b != NULL);
  assert(obint_is_zero(b) == 0);

  seed = obint_new(0);
  result = obint_reduce(a, b, seed, 1);
  result->sign = a->sign * b->sign;
  ob_release((obj *)seed);

  return result;
}


obint * obint_divide_primitive(const obint *a, int64_t b){

  obint *wrapper, *result;

  assert(a != NULL);
  assert(b != 0);

  wrapper = obint_new(b);
  result = obint_divide(a, wrapper);
  ob_release((obj *)wrapper);

  return result;
}


obint * obint_mod(const obint *a, const obint *b){

  obint *result, *seed;

  assert(a != NULL);
  assert(b != NULL);
  assert(obint_is_zero(b) == 0);

  seed = obint_new(0);
  result = obint_reduce(a, b, seed, 0);
  result->sign = a->sign;
  ob_release((obj *)seed);

  return result;
}


obint * obint_mod_primitive(const obint *a, int64_t b){

  obint *wrapper, *result;

  assert(a != NULL);
  assert(b != 0);

  wrapper = obint_new(b);
  result = obint_mod(a, wrapper);
  ob_release((obj *)wrapper);

  return result;
}


/* PRIVATE METHODS */

/* add arguments to complete initialization as needed, modify
 * obint_Private.h as well if modifications are made */
obint * obint_create_default(uint64_t num_digits){

  static const char classname[] = "obint";
  obint *new_instance = malloc(sizeof(obint));
  assert(new_instance != NULL);

  /* initialize base class data */
  ob_init_base((obj *)new_instance, &obint_destroy, &obint_hash,
               &obint_compare, &obint_display, classname);

  new_instance->sign = 1; /* positive by default */

  new_instance->digits = malloc(sizeof(int8_t)*num_digits);
  assert(new_instance->digits != NULL);
  memset(new_instance->digits, 0, num_digits);

  new_instance->num_digits = num_digits;

  return new_instance;
}


ob_hash_t obint_hash(const obj *to_hash){

  static int8_t init = 0;
  static ob_hash_t seed = 0;

  uint64_t i;
  ob_hash_t value;
  obint *instance = (obint *)to_hash;

  assert(to_hash);
  assert(ob_has_class(to_hash, "obint"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }

  value = seed;

  i = obint_most_sig(instance);

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


int8_t obint_compare(const obj *a, const obj *b){

  int8_t magnitude_comp;
  const obint *comp_a = (obint *)a;
  const obint *comp_b = (obint *)b;

  assert(a);
  assert(b);
  assert(ob_has_class(a, "obint"));
  assert(ob_has_class(b, "obint"));

  /* if signs are equal magnitude comparision is required */
  if(comp_a->sign == comp_b->sign){

    magnitude_comp = obint_compare_magnitudes(comp_a, comp_b);

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


int8_t obint_compare_magnitudes(const obint *a, const obint *b){

  uint64_t i,j;

  i = obint_most_sig(a);
  j = obint_most_sig(b);

  if(i < j) return OB_LESS_THAN;
  if(i > j) return OB_GREATER_THAN;

  /* number of digits is equal, resort to digit by digit comparision */
  for( ; i<a->num_digits; i--){
    if(a->digits[i] < b->digits[i]) return OB_LESS_THAN;
    if(a->digits[i] > b->digits[i]) return OB_GREATER_THAN;
  }

  return OB_EQUAL_TO;
}


void obint_display(const obj *to_print){

  obstring *str;
  const obint *instance = (obint *)to_print;

  assert(to_print);
  assert(ob_has_class(to_print, "obint"));

  str = obint_to_string(instance);
  fprintf(stderr, "Value:\n  %s\n", obstring_cstring(str));

  ob_release((obj *)str);

  return;
}


void obint_destroy(obj *to_dealloc){

  /* cast generic obj to obint */
  obint *instance = (obint *)to_dealloc;

  assert(to_dealloc);
  assert(ob_has_class(to_dealloc, "obint"));

  free(instance->digits);

  return;
}


obint * obint_add_unsigned(const obint *a, const obint *b){

  uint64_t i, large_most_sig, small_most_sig;
  int8_t carry = 0;
  obint *result;
  const obint *larger, *smaller;

  larger = obint_most_sig(a) > obint_most_sig(b) ? a : b;
  smaller = obint_most_sig(a) <= obint_most_sig(b) ? a : b;

  large_most_sig = obint_most_sig(larger);
  small_most_sig = obint_most_sig(smaller);

  /* reserve enough space for the result, +2 required for actual size of larger
   * and possible extra digit needed to catch any addition overflow */
  result = obint_create_default(large_most_sig+2);

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


obint * obint_subtract_unsigned(const obint *a, const obint *b){

  uint64_t i, a_most_sig, b_most_sig;
  obint *result;

  result = obint_copy(a);

  a_most_sig = obint_most_sig(a);
  b_most_sig = obint_most_sig(b);

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


obint * obint_multiply_unsigned(const obint *a, const obint *b){

  uint64_t large_most_sig, small_most_sig, split_point;
  const obint *larger, *smaller;
  obint *x0, *x1, *y0, *y1; /* split versions of a and b */
  obint *z0, *z1, *z1a, *z1b, *z1c, *z1d, *z2; /* partial results of Karatsuba
                                                  algorithm */
  obint *partial_result, *result;

  larger = obint_most_sig(a) > obint_most_sig(b) ? a : b;
  smaller = obint_most_sig(a) <= obint_most_sig(b) ? a : b;

  large_most_sig = obint_most_sig(larger);
  small_most_sig = obint_most_sig(smaller);

  /* base case, a and b are single digits */
  if(large_most_sig + small_most_sig < int64_max_digits)
    return obint_new(obint_magnitude(a) * obint_magnitude(b));

  split_point = large_most_sig/2+1;
  obint_split(a, split_point, &x1, &x0);
  obint_split(b, split_point, &y1, &y0);

  z2 = obint_multiply_unsigned(x1, y1);
  z0 = obint_multiply_unsigned(x0, y0);

  z1a = obint_add_unsigned(x1, x0);
  z1b = obint_add_unsigned(y1, y0);
  z1c = obint_multiply_unsigned(z1a, z1b);

  ob_release((obj *)x0);
  ob_release((obj *)x1);
  ob_release((obj *)y0);
  ob_release((obj *)y1);
  ob_release((obj *)z1a);
  ob_release((obj *)z1b);

  z1d = obint_subtract_unsigned(z1c, z2);
  ob_release((obj *)z1c);
  z1 = obint_subtract_unsigned(z1d, z0);
  ob_release((obj *)z1d);

  obint_shift(z2, 2*split_point);
  obint_shift(z1, split_point);
  partial_result = obint_add_unsigned(z2, z1);

  ob_release((obj *) z2);
  ob_release((obj *) z1);

  result = obint_add_unsigned(partial_result, z0);

  ob_release((obj *) partial_result);
  ob_release((obj *) z0);

  return result;
}


obint * obint_reduce(const obint *a, const obint *b, const obint *approx,
                           uint8_t quotient){

  int8_t comp_val;
  int64_t a_val, b_val, result_val;
  uint64_t a_most_sig, b_most_sig, a_min, b_min, i, j;
  obint *partial; /* partial results */
  obint *product; /* quotient approximation multiplied by b */
  obint *new_approx; /* improved approximation */
  obint *new_dividend; /* reduced dividend */
  obint *result; /* approximation */

  a_most_sig = obint_most_sig(a);
  b_most_sig = obint_most_sig(b);

  /* if operation can be computed using integer arithmetic do so */
  if(a_most_sig < int64_max_digits && b_most_sig < int64_max_digits){

    a_val = obint_magnitude(a);
    b_val = obint_magnitude(b);

    if(quotient){
      result_val = a_val/b_val;
      partial = obint_new(result_val);
      result = obint_add(approx, partial);
      ob_release((obj *)partial);
    }
    else{
      result_val = a_val%b_val;
      result = obint_new(result_val);
    }

    return result;
  }

  /* if a < b (so a/b = 0, a%b = a) */
  comp_val = obint_compare_magnitudes(a, b);
  if(comp_val == OB_LESS_THAN){
    if(quotient) return obint_copy(approx);
    else return obint_copy(a);
  }
  /* if a == b (so a/b = 1, a%b = 0) */
  else if(comp_val == OB_EQUAL_TO){
    if(quotient) return obint_add_primitive(approx,1);
    else return obint_new(0);
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
  partial = obint_new(a_val/b_val);
  obint_shift(partial, i-j);
  product = obint_multiply_unsigned(partial, b);

  comp_val = obint_compare_magnitudes(product, a);

  /* if approximation is too great then increment b_val to account for remaining
   * portion of b that is causing over approximation */
  if(comp_val == OB_GREATER_THAN){
    ob_release((obj *)partial);
    ob_release((obj *)product);

    b_val++;
    partial = obint_new(a_val/b_val);
    obint_shift(partial, i-j);
    product = obint_multiply_unsigned(partial, b);
  }

  new_dividend = obint_subtract_unsigned(a, product);
  ob_release((obj *)product);
  new_approx = obint_add_unsigned(approx, partial);
  ob_release((obj *)partial);

  result = obint_reduce(new_dividend, b, new_approx, quotient);

  ob_release((obj *)new_approx);
  ob_release((obj *)new_dividend);

  return result;
}


uint64_t obint_most_sig(const obint *a){

  uint64_t i;

  i = a->num_digits - 1;
  while(!a->digits[i] && i>0) i--;
  return i;
}


void obint_split(const obint *a, uint64_t i, obint **b1, obint **b0){

  if(a->num_digits <= i){
    *b1 = obint_create_default(1);
    *b0 = obint_create_default(a->num_digits);
    memcpy((*b0)->digits, a->digits, a->num_digits);
  }
  else{
    *b1 = obint_create_default(a->num_digits - i);
    memcpy((*b1)->digits, (a->digits + i), a->num_digits-i);
    *b0 = obint_create_default(i);
    memcpy((*b0)->digits, (a->digits), i);
  }

  return;
}


void obint_shift(obint *a, uint64_t m){

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


int64_t obint_magnitude(const obint *a){

  uint64_t i, most_sig;
  int64_t val;

  most_sig = obint_most_sig(a);
  val = 0;

  for(i=most_sig ; i <= most_sig; i--){
    val *= 10;
    val += a->digits[i];
  }

  return val;
}


void obint_set_max_digits(uint8_t digits){
  if(digits >= 2 && digits <= 17) int64_max_digits = digits;
}
