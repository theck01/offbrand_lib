/**
 * @file obint_test.c
 * @brief obint Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/obint.h"
#include "../../include/private/obint_private.h"
#include "../../include/obstring.h"

/**
 * @brief Main unit testing routine
 */
int main (){

  obint *a, *b, *c;
  obstring *str1, *str2;

  /* test machine integer creation and value methods */
  a = obint_new(1024);
  assert(obint_value(a) == 1024);
  b = obint_new(9581085);
  assert(obint_value(b) == 9581085);
  c = obint_new(-999);
  assert(obint_value(c) == -999);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  /* test string integer creation and value methods */
  str1 = obstring_new("12345");
  a = obint_from_string(str1);
  assert(obint_value(a) == 12345);
  str2 = obint_to_string(a);
  assert(ob_compare((obj *)str1, (obj *)str2) == OB_EQUAL_TO);

  ob_release((obj *)a);
  ob_release((obj *)str1);
  ob_release((obj *)str2);

  str1 = obstring_new("-012345");
  a = obint_from_string(str1);
  ob_release((obj *)str1);
  assert(obint_value(a) == -12345);
  str1 = obstring_new("-12345");
  str2 = obint_to_string(a);
  assert(ob_compare((obj *)str1, (obj *)str2) == OB_EQUAL_TO);

  ob_release((obj *)a);
  ob_release((obj *)str1);
  ob_release((obj *)str2);

  /* test copy method */
  a = obint_new(918394);
  b = obint_copy(a);
  assert(ob_compare((obj *)a, (obj *)b) == OB_EQUAL_TO);
  assert(a != b);
  assert(a->digits != b->digits);

  ob_release((obj *)a);
  ob_release((obj *)b);

  /* test integer zero and negative methods */
  a = obint_new(1948);
  b = obint_new(-1);
  str1 = obstring_new("-0");
  c = obint_from_string(str1);
  ob_release((obj *)str1);

  assert(!obint_is_zero(a));
  assert(!obint_is_zero(b));
  assert(obint_is_zero(c));

  assert(!obint_is_negative(a));
  assert(obint_is_negative(b));
  assert(!obint_is_negative(c));

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  /* test integer addition */
  a = obint_new(999);
  b = obint_add_primitive(a, 1111);
  assert(obint_value(b) == 2110);
  c = obint_add(a, b);
  assert(obint_value(c) == 2110 + 999);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  a = obint_new(-999);
  b = obint_add_primitive(a, 1);
  assert(obint_value(b) == -998);
  ob_release((obj *)b);
  b = obint_add_primitive(a, -1);
  assert(obint_value(b) == -1000);
  ob_release((obj *)b);
  b = obint_new(1000);
  c = obint_add(a, b);
  assert(obint_value(c) == 1);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  /* test integer subtraction */
  a = obint_new(999);
  b = obint_subtract_primitive(a, 1111);
  assert(obint_value(b) == -112);
  c = obint_subtract(a, b);
  assert(obint_value(c) == 999 + 112);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  a = obint_new(-999);
  b = obint_subtract_primitive(a, 1);
  assert(obint_value(b) == -1000);
  ob_release((obj *)b);
  b = obint_subtract_primitive(a, -1);
  assert(obint_value(b) == -998);
  ob_release((obj *)b);
  b = obint_new(1000);
  c = obint_subtract(a, b);
  assert(obint_value(c) == -1999);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  /* test integer multiplication (size under digit limit for explicit integer
   * arithmetic) */
  a = obint_new(999);
  b = obint_multiply_primitive(a, -999);
  assert(obint_value(b) == 999*-999);
  c = obint_multiply(a, b);
  assert(obint_value(c) == 999*999*-999);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  a = obint_new(-1928491);
  b = obint_multiply_primitive(a, -58);
  assert(obint_value(b) == -1928491*-58);
  ob_release((obj *)b);
  b = obint_new(2);
  c = obint_multiply(a, b);
  assert(obint_value(c) == -1928491*2);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  a = obint_new(29458);
  b = obint_multiply_primitive(a, 0);
  assert(obint_is_zero(b) != 0);

  ob_release((obj *)a);
  ob_release((obj *)b);

  /* test integer division (size under digit limit for explicit integer
   * arithmetic) */
  a = obint_new(999);
  b = obint_divide_primitive(a, -999);
  assert(obint_value(b) == -1);
  c = obint_divide_primitive(a, 33);
  assert(obint_value(c) == 999/33);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  a = obint_new(1940248);
  b = obint_divide_primitive(a, -2848);
  assert(obint_value(b) == 1940248/(-2848));
  c = obint_divide(a, b);
  assert(obint_value(c) == 1940248/(1940248/(-2848)));

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  /* test integer modulus (size under digit limit for explicit integer
   * arithmetic) */
  a = obint_new(999);
  b = obint_mod_primitive(a, -999);
  assert(obint_value(b) == 999%(-999));
  c = obint_mod_primitive(a, 33);
  assert(obint_value(c) == 999%33);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  a = obint_new(1940248);
  b = obint_mod_primitive(a, -2848);
  assert(obint_value(b) == 1940248%(-2848));
  c = obint_mod(a, b);
  assert(obint_value(c) == 1940248%(1940248%(-2848)));

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  /* set digit limit for integer arithmetic low, for test purposes */
  obint_set_max_digits(2);

  /* test integer multiplication (size over digit limit for explicit integer
   * arithmetic) */
  a = obint_new(999);
  b = obint_multiply_primitive(a, -999);
  assert(obint_value(b) == 999*-999);
  c = obint_multiply(a, b);
  assert(obint_value(c) == 999*999*-999);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  a = obint_new(-1928491);
  b = obint_multiply_primitive(a, -58);
  assert(obint_value(b) == -1928491*-58);
  ob_release((obj *)b);
  b = obint_new(2);
  c = obint_multiply(a, b);
  assert(obint_value(c) == -1928491*2);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  a = obint_new(29458);
  b = obint_multiply_primitive(a, 0);
  assert(obint_is_zero(b) != 0);

  ob_release((obj *)a);
  ob_release((obj *)b);

  /* test integer division (size under digit limit for explicit integer
   * arithmetic) */
  a = obint_new(999);
  b = obint_divide_primitive(a, -999);
  assert(obint_value(b) == -1);
  c = obint_divide_primitive(a, 33);
  assert(obint_value(c) == 999/33);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  a = obint_new(1940248);
  b = obint_divide_primitive(a, -2848);
  assert(obint_value(b) == 1940248/(-2848));
  c = obint_divide(a, b);
  assert(obint_value(c) == 1940248/(1940248/(-2848)));

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  /* test integer modulus (size over digit limit for explicit integer
   * arithmetic) */
  a = obint_new(999);
  b = obint_mod_primitive(a, -999);
  assert(obint_value(b) == 999%(-999));
  c = obint_mod_primitive(a, 33);
  assert(obint_value(c) == 999%33);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  a = obint_new(1940248);
  b = obint_mod_primitive(a, -2848);
  assert(obint_value(b) == 1940248%(-2848));
  c = obint_mod(a, b);
  assert(obint_value(c) == 1940248%(1940248%(-2848)));

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);

  printf("obint: TESTS PASSED\n");
  return 0;
}
