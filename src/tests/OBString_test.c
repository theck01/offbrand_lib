/**
 * @file OBString_test.c
 * @brief OBString Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/OBString.h"
#include "../../include/OBTest.h"

/**
 * @brief Main unit testing routine
 */
int main (){

  OBString *str1, *str2, *str3, *null_str;
  OBVector *tokens;
  const char *contents;

  str1 = createString("Hello, World!");
  null_str = copySubstring(str1, 20, 2);

  /* Test createString and getCString */
  contents = getCString(str1);
  assert(strcmp(contents, "Hello, World!") == 0);
  

  /* Test copySubstring and stringLength */
  str2 = copySubstring(str1, 0, 5);
  contents = getCString(str2);
  assert(strcmp(contents, "Hello") == 0);

  release((obj *)str2);

  str2 = copySubstring(str1, -15, 7);
  contents = getCString(str2);
  assert(strcmp(contents, "Hello") == 0);

  release((obj *)str2);

  str2 = copySubstring(str1, 0, stringLength(str1));
  contents = getCString(str2);
  assert(strcmp(contents, "Hello, World!") == 0);

  release((obj *)str2);

  null_str = copySubstring(str1, 20, 2);
  assert(stringLength(null_str) == 0);

  /* Test charAtStringIndex */
  assert(charAtStringIndex(str1, 0) == 'H');
  assert(charAtStringIndex(str1, -5) == 'o');
  assert(charAtStringIndex(str1, -20) == '\0');
  assert(charAtStringIndex(str1, 20) == '\0');
  assert(charAtStringIndex(null_str, 0) == '\0');

  /* Test String Concatenation */
  str2 = createString(" And hello again!");
  str3 = concatenateStrings(str1, str2);
  contents = getCString(str3);
  assert(strcmp(contents, "Hello, World! And hello again!") == 0);

  release((obj *)str2);

  str2 = concatenateStrings(null_str, str1);
  contents = getCString(str2);
  assert(strcmp(contents, "Hello, World!") == 0);

  /* Test String Comparision */
  assert(compare((obj *)str1, (obj *)str2) == OB_EQUAL_TO);
  assert(compare((obj *)str1, (obj *)str3) == OB_LESS_THAN);
  assert(compare((obj *)str1, (obj *)null_str) == OB_GREATER_THAN);

  release((obj *)str2);
  release((obj *)str3);

  /* Test String Splits */
  str2 = createString("Testing string split   into#!many");
  tokens = splitString(str2, " ");
  assert(strcmp(getCString((OBString *)objAtVectorIndex(tokens, 0)), "Testing")
         == 0);
  assert(strcmp(getCString((OBString *)objAtVectorIndex(tokens, 3)),
                           "into#!many") == 0);

  release((obj *)tokens);

  tokens = splitString(str2, "#!");
  assert(strcmp(getCString((OBString *)objAtVectorIndex(tokens, 0)),
                           "Testing string split   into") == 0);
  assert(strcmp(getCString((OBString *)objAtVectorIndex(tokens, 1)), "many")
         == 0);


  printf("OBString_test: TESTS PASSED\n");

  return 0;
}
