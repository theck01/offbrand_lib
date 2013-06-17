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
  a = OBIntCreate(1024);
  assert(OBIntGetIntValue(a) == 1024);
  b = OBIntCreate(9581085);
  assert(OBIntGetIntValue(b) == 9581085);
  c = OBIntCreate(-999);
  assert(OBIntGetIntValue(c) == -999);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

	/* test string integer creation and value methods */
	str1 = createString("12345");
	a = OBIntCreateFromString(str1);
	assert(OBIntGetIntValue(a) == 12345);
	str2 = OBIntGetStringValue(a);
	assert(OBCompare((OBObjType *)str1, (OBObjType *)str2) == OB_EQUAL_TO);
	
	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)str1);
	OBRelease((OBObjType *)str2);

	str1 = createString("-012345");
	a = OBIntCreateFromString(str1);
	OBRelease((OBObjType *)str1);
	assert(OBIntGetIntValue(a) == -12345);
	str1 = createString("-12345");
	str2 = OBIntGetStringValue(a);
	assert(OBCompare((OBObjType *)str1, (OBObjType *)str2) == OB_EQUAL_TO);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)str1);
	OBRelease((OBObjType *)str2);

	/* test copy method */
	a = OBIntCreate(918394);
	b = OBIntCopy(a);
	assert(OBCompare((OBObjType *)a, (OBObjType *)b) == OB_EQUAL_TO);
	assert(a != b);
	assert(a->digits != b->digits);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);

	/* test integer zero and negative methods */
	a = OBIntCreate(1948);
	b = OBIntCreate(-1);
	str1 = createString("-0");
	c = OBIntCreateFromString(str1);
	OBRelease((OBObjType *)str1);
	
	assert(!OBIntIsZero(a));
	assert(!OBIntIsZero(b));
	assert(OBIntIsZero(c));

	assert(!OBIntIsNegative(a));
	assert(OBIntIsNegative(b));
	assert(!OBIntIsNegative(c));

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  /* test integer addition */
  a = OBIntCreate(999);
  b = OBIntAddPrimitive(a, 1111);
  assert(OBIntGetIntValue(b) == 2110);
  c = OBIntAdd(a, b);
  assert(OBIntGetIntValue(c) == 2110 + 999);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  a = OBIntCreate(-999);
  b = OBIntAddPrimitive(a, 1);
  assert(OBIntGetIntValue(b) == -998);
  OBRelease((OBObjType *)b);
  b = OBIntAddPrimitive(a, -1);
  assert(OBIntGetIntValue(b) == -1000);
  OBRelease((OBObjType *)b);
  b = OBIntCreate(1000);
  c = OBIntAdd(a, b);
  assert(OBIntGetIntValue(c) == 1);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  /* test integer subtraction */
  a = OBIntCreate(999);
  b = OBIntSubtractPrimitive(a, 1111);
  assert(OBIntGetIntValue(b) == -112);
  c = OBIntSubtract(a, b);
  assert(OBIntGetIntValue(c) == 999 + 112);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  a = OBIntCreate(-999);
  b = OBIntSubtractPrimitive(a, 1);
  assert(OBIntGetIntValue(b) == -1000);
  OBRelease((OBObjType *)b);
  b = OBIntSubtractPrimitive(a, -1);
  assert(OBIntGetIntValue(b) == -998);
  OBRelease((OBObjType *)b);
  b = OBIntCreate(1000);
  c = OBIntSubtract(a, b);
  assert(OBIntGetIntValue(c) == -1999);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  /* test integer multiplication (size under digit limit for explicit integer
   * arithmetic) */
  a = OBIntCreate(999);
  b = OBIntMultiplyPrimitive(a, -999);
  assert(OBIntGetIntValue(b) == 999*-999);
  c = OBIntMultiply(a, b);
  assert(OBIntGetIntValue(c) == 999*999*-999);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  a = OBIntCreate(-1928491);
  b = OBIntMultiplyPrimitive(a, -58);
  assert(OBIntGetIntValue(b) == -1928491*-58);
  OBRelease((OBObjType *)b);
  b = OBIntCreate(2);
  c = OBIntMultiply(a, b);
  assert(OBIntGetIntValue(c) == -1928491*2);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  a = OBIntCreate(29458);
  b = OBIntMultiplyPrimitive(a, 0);
  assert(OBIntIsZero(b) != 0);

  OBRelease((OBObjType *)a);
  OBRelease((OBObjType *)b);

  /* test integer division (size under digit limit for explicit integer
   * arithmetic) */
  a = OBIntCreate(999);
  b = OBIntDividePrimitive(a, -999);
  assert(OBIntGetIntValue(b) == -1);
  c = OBIntDividePrimitive(a, 33);
  assert(OBIntGetIntValue(c) == 999/33);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  a = OBIntCreate(1940248);
  b = OBIntDividePrimitive(a, -2848);
  assert(OBIntGetIntValue(b) == 1940248/(-2848));
  c = OBIntDivide(a, b);
  assert(OBIntGetIntValue(c) == 1940248/(1940248/(-2848)));

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  /* test integer modulus (size under digit limit for explicit integer
   * arithmetic) */
  a = OBIntCreate(999);
  b = OBIntModPrimitive(a, -999);
  assert(OBIntGetIntValue(b) == 999%(-999));
  c = OBIntModPrimitive(a, 33);
  assert(OBIntGetIntValue(c) == 999%33);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  a = OBIntCreate(1940248);
  b = OBIntModPrimitive(a, -2848);
  assert(OBIntGetIntValue(b) == 1940248%(-2848));
  c = OBIntMod(a, b);
  assert(OBIntGetIntValue(c) == 1940248%(1940248%(-2848)));

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  /* set digit limit for integer arithmetic low, for test purposes */
  setMaxDigits(2);

  /* test integer multiplication (size over digit limit for explicit integer
   * arithmetic) */
  a = OBIntCreate(999);
  b = OBIntMultiplyPrimitive(a, -999);
  assert(OBIntGetIntValue(b) == 999*-999);
  c = OBIntMultiply(a, b);
  assert(OBIntGetIntValue(c) == 999*999*-999);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  a = OBIntCreate(-1928491);
  b = OBIntMultiplyPrimitive(a, -58);
  assert(OBIntGetIntValue(b) == -1928491*-58);
  OBRelease((OBObjType *)b);
  b = OBIntCreate(2);
  c = OBIntMultiply(a, b);
  assert(OBIntGetIntValue(c) == -1928491*2);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  a = OBIntCreate(29458);
  b = OBIntMultiplyPrimitive(a, 0);
  assert(OBIntIsZero(b) != 0);

  OBRelease((OBObjType *)a);
  OBRelease((OBObjType *)b);

  /* test integer division (size under digit limit for explicit integer
   * arithmetic) */
  a = OBIntCreate(999);
  b = OBIntDividePrimitive(a, -999);
  assert(OBIntGetIntValue(b) == -1);
  c = OBIntDividePrimitive(a, 33);
  assert(OBIntGetIntValue(c) == 999/33);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  a = OBIntCreate(1940248);
  b = OBIntDividePrimitive(a, -2848);
  assert(OBIntGetIntValue(b) == 1940248/(-2848));
  c = OBIntDivide(a, b);
  assert(OBIntGetIntValue(c) == 1940248/(1940248/(-2848)));

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  /* test integer modulus (size over digit limit for explicit integer
   * arithmetic) */
  a = OBIntCreate(999);
  b = OBIntModPrimitive(a, -999);
  assert(OBIntGetIntValue(b) == 999%(-999));
  c = OBIntModPrimitive(a, 33);
  assert(OBIntGetIntValue(c) == 999%33);

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  a = OBIntCreate(1940248);
  b = OBIntModPrimitive(a, -2848);
  assert(OBIntGetIntValue(b) == 1940248%(-2848));
  c = OBIntMod(a, b);
  assert(OBIntGetIntValue(c) == 1940248%(1940248%(-2848)));

	OBRelease((OBObjType *)a);
	OBRelease((OBObjType *)b);
	OBRelease((OBObjType *)c);

  printf("OBInt_test: TESTS PASSED\n");
  return 0;
}
