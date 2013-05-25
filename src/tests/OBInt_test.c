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

	release((obj *)a);
	release((obj *)b);

	/* test string integer creation and value methods */
	str1 = createString("12345");
	a = intFromString(str1);
	assert(intValue(a) == 12345);
	str2 = stringFromInt(a);
	assert(compare((obj *)str1, (obj *)str2) == OB_EQUAL_TO);
	
	release((obj *)a);
	release((obj *)str1);
	release((obj *)str2);

	str1 = createString("-012345");
	a = intFromString(str1);
	release((obj *)str1);
	assert(intValue(a) == -12345);
	str1 = createString("-12345");
	str2 = stringFromInt(a);
	assert(compare((obj *)str1, (obj *)str2) == OB_EQUAL_TO);

	release((obj *)a);
	release((obj *)str1);
	release((obj *)str2);

	/* test copy method */
	a = createIntWithInt(918394);
	b = copyInt(a);
	assert(compare((obj *)a, (obj *)b) == OB_EQUAL_TO);
	assert(a != b);
	assert(a->digits != b->digits);

	release((obj *)a);
	release((obj *)b);

	/* test integer zero and negative methods */
	a = createIntWithInt(1948);
	b = createIntWithInt(-1);
	str1 = createString("-0");
	c = intFromString(str1);
	release(str1);
	
	assert(!isIntZero(a));
	assert(!isIntZero(b));
	assert(isIntZero(c));

	assert(!isIntNegative(a));
	assert(isIntNegative(b));
	assert(!isIntNegative(c));

	release((obj *)a);
	release((obj *)b);
	release((obj *)c);

  printf("OBInt_test: TESTS PASSED\n");
  return 0;
}
