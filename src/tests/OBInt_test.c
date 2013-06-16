/**
 * @file OBInt_test.c
 * @brief OBInt Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/OBInt.h"
#include "../../include/private/OBInt_Private.h"
#include "../../include/OBString.h"

/**
 * @brief Main unit testing routine
 */
int main (){

  OBInt *a, *b, *c;
	OBString *str1, *str2;

  /* test machine integer creation and value methods */
  a = createIntWithInt(1024);
  assert(intValue(a) == 1024);
  b = createIntWithInt(9581085);
  assert(intValue(b) == 9581085);
  c = createIntWithInt(-999);
  assert(intValue(c) == -999);

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

	/* test string integer creation and value methods */
	str1 = createString("12345");
	a = intFromString(str1);
	assert(intValue(a) == 12345);
	str2 = stringFromInt(a);
	assert(compare((OBObjType *)str1, (OBObjType *)str2) == OB_EQUAL_TO);
	
	release((OBObjType *)a);
	release((OBObjType *)str1);
	release((OBObjType *)str2);

	str1 = createString("-012345");
	a = intFromString(str1);
	release((OBObjType *)str1);
	assert(intValue(a) == -12345);
	str1 = createString("-12345");
	str2 = stringFromInt(a);
	assert(compare((OBObjType *)str1, (OBObjType *)str2) == OB_EQUAL_TO);

	release((OBObjType *)a);
	release((OBObjType *)str1);
	release((OBObjType *)str2);

	/* test copy method */
	a = createIntWithInt(918394);
	b = copyInt(a);
	assert(compare((OBObjType *)a, (OBObjType *)b) == OB_EQUAL_TO);
	assert(a != b);
	assert(a->digits != b->digits);

	release((OBObjType *)a);
	release((OBObjType *)b);

	/* test integer zero and negative methods */
	a = createIntWithInt(1948);
	b = createIntWithInt(-1);
	str1 = createString("-0");
	c = intFromString(str1);
	release((OBObjType *)str1);
	
	assert(!isIntZero(a));
	assert(!isIntZero(b));
	assert(isIntZero(c));

	assert(!isIntNegative(a));
	assert(isIntNegative(b));
	assert(!isIntNegative(c));

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  /* test integer addition */
  a = createIntWithInt(999);
  b = addIntAndPrim(a, 1111);
  assert(intValue(b) == 2110);
  c = addInts(a, b);
  assert(intValue(c) == 2110 + 999);

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  a = createIntWithInt(-999);
  b = addIntAndPrim(a, 1);
  assert(intValue(b) == -998);
  release((OBObjType *)b);
  b = addIntAndPrim(a, -1);
  assert(intValue(b) == -1000);
  release((OBObjType *)b);
  b = createIntWithInt(1000);
  c = addInts(a, b);
  assert(intValue(c) == 1);

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  /* test integer subtraction */
  a = createIntWithInt(999);
  b = subtractIntWithPrim(a, 1111);
  assert(intValue(b) == -112);
  c = subtractInts(a, b);
  assert(intValue(c) == 999 + 112);

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  a = createIntWithInt(-999);
  b = subtractIntWithPrim(a, 1);
  assert(intValue(b) == -1000);
  release((OBObjType *)b);
  b = subtractIntWithPrim(a, -1);
  assert(intValue(b) == -998);
  release((OBObjType *)b);
  b = createIntWithInt(1000);
  c = subtractInts(a, b);
  assert(intValue(c) == -1999);

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  /* test integer multiplication (size under digit limit for explicit integer
   * arithmetic) */
  a = createIntWithInt(999);
  b = multiplyIntAndPrim(a, -999);
  assert(intValue(b) == 999*-999);
  c = multiplyInts(a, b);
  assert(intValue(c) == 999*999*-999);

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  a = createIntWithInt(-1928491);
  b = multiplyIntAndPrim(a, -58);
  assert(intValue(b) == -1928491*-58);
  release((OBObjType *)b);
  b = createIntWithInt(2);
  c = multiplyInts(a, b);
  assert(intValue(c) == -1928491*2);

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  a = createIntWithInt(29458);
  b = multiplyIntAndPrim(a, 0);
  assert(isIntZero(b) != 0);

  release((OBObjType *)a);
  release((OBObjType *)b);

  /* test integer division (size under digit limit for explicit integer
   * arithmetic) */
  a = createIntWithInt(999);
  b = divideIntWithPrim(a, -999);
  assert(intValue(b) == -1);
  c = divideIntWithPrim(a, 33);
  assert(intValue(c) == 999/33);

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  a = createIntWithInt(1940248);
  b = divideIntWithPrim(a, -2848);
  assert(intValue(b) == 1940248/(-2848));
  c = divideInts(a, b);
  assert(intValue(c) == 1940248/(1940248/(-2848)));

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  /* test integer modulus (size under digit limit for explicit integer
   * arithmetic) */
  a = createIntWithInt(999);
  b = modIntWithPrim(a, -999);
  assert(intValue(b) == 999%(-999));
  c = modIntWithPrim(a, 33);
  assert(intValue(c) == 999%33);

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  a = createIntWithInt(1940248);
  b = modIntWithPrim(a, -2848);
  assert(intValue(b) == 1940248%(-2848));
  c = modInts(a, b);
  assert(intValue(c) == 1940248%(1940248%(-2848)));

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  /* set digit limit for integer arithmetic low, for test purposes */
  setMaxDigits(2);

  /* test integer multiplication (size over digit limit for explicit integer
   * arithmetic) */
  a = createIntWithInt(999);
  b = multiplyIntAndPrim(a, -999);
  assert(intValue(b) == 999*-999);
  c = multiplyInts(a, b);
  assert(intValue(c) == 999*999*-999);

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  a = createIntWithInt(-1928491);
  b = multiplyIntAndPrim(a, -58);
  assert(intValue(b) == -1928491*-58);
  release((OBObjType *)b);
  b = createIntWithInt(2);
  c = multiplyInts(a, b);
  assert(intValue(c) == -1928491*2);

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  a = createIntWithInt(29458);
  b = multiplyIntAndPrim(a, 0);
  assert(isIntZero(b) != 0);

  release((OBObjType *)a);
  release((OBObjType *)b);

  /* test integer division (size under digit limit for explicit integer
   * arithmetic) */
  a = createIntWithInt(999);
  b = divideIntWithPrim(a, -999);
  assert(intValue(b) == -1);
  c = divideIntWithPrim(a, 33);
  assert(intValue(c) == 999/33);

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  a = createIntWithInt(1940248);
  b = divideIntWithPrim(a, -2848);
  assert(intValue(b) == 1940248/(-2848));
  c = divideInts(a, b);
  assert(intValue(c) == 1940248/(1940248/(-2848)));

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  /* test integer modulus (size over digit limit for explicit integer
   * arithmetic) */
  a = createIntWithInt(999);
  b = modIntWithPrim(a, -999);
  assert(intValue(b) == 999%(-999));
  c = modIntWithPrim(a, 33);
  assert(intValue(c) == 999%33);

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  a = createIntWithInt(1940248);
  b = modIntWithPrim(a, -2848);
  assert(intValue(b) == 1940248%(-2848));
  c = modInts(a, b);
  assert(intValue(c) == 1940248%(1940248%(-2848)));

	release((OBObjType *)a);
	release((OBObjType *)b);
	release((OBObjType *)c);

  printf("OBInt_test: TESTS PASSED\n");
  return 0;
}
