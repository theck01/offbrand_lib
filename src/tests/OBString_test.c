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

  OBString *str1, *str2;
  char *contents;

  str1 = createString("Hello, World!");

  /* Test createString and getCString */
  contents = getCString(str1);
  assert(strcmp(contents, "Hello, World!") == 0);
  free(contents);

  /* Test copySubstring */
  str2 = copySubstring(str1, 0, 5);
  contents = getCString(str2);
  assert(strcmp(contents, "Hello") == 0);

  free(contents);
  release((obj *)str2);

  str2 = copySubstring(str1, -15, 7);
  contents = getCString(str2);
  assert(strcmp(contents, "Hello") == 0);

  free(contents);
  release((obj *)str2);

  str2 = copySubstring(str1, 0, stringLength(str1)*2);
  contents = getCString(str2);
  assert(strcmp(contents, "Hello, World!") == 0);

  free(contents);
  release((obj *)str2);



  printf("OBString_test: TESTS PASSED\n");

  return 0;
}
