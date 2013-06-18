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

  str1 = OBStringCreate("Hello, World!");

  /* Test createString and getCString */
  contents = OBStringGetCString(str1);
  assert(strcmp(contents, "Hello, World!") == 0);
  

  /* Test copySubstring and stringLength */
  str2 = OBStringGetSubstringCopyRange(str1, 0, 5);
  contents = OBStringGetCString(str2);
  assert(strcmp(contents, "Hello") == 0);

  OBRelease((OBObjType *)str2);

  str2 = OBStringGetSubstringCopyRange(str1, -15, 7);
  contents = OBStringGetCString(str2);
  assert(strcmp(contents, "Hello") == 0);

  OBRelease((OBObjType *)str2);

  str2 = OBStringGetSubstringCopyRange(str1, 0, OBStringGetLength(str1));
  contents = OBStringGetCString(str2);
  assert(strcmp(contents, "Hello, World!") == 0);

  OBRelease((OBObjType *)str2);

  null_str = OBStringGetSubstringCopyRange(str1, 20, 2);
  assert(OBStringGetLength(null_str) == 0);

  /* Test charAtStringIndex */
  assert(OBStringGetCharAtIndex(str1, 0) == 'H');
  assert(OBStringGetCharAtIndex(str1, -5) == 'o');
  assert(OBStringGetCharAtIndex(str1, -20) == '\0');
  assert(OBStringGetCharAtIndex(str1, 20) == '\0');
  assert(OBStringGetCharAtIndex(null_str, 0) == '\0');

  /* Test String Concatenation */
  str2 = OBStringCreate(" And hello again!");
  str3 = OBStringConcatenate(str1, str2);
  contents = OBStringGetCString(str3);
  assert(strcmp(contents, "Hello, World! And hello again!") == 0);

  OBRelease((OBObjType *)str2);

  str2 = OBStringConcatenate(null_str, str1);
  contents = OBStringGetCString(str2);
  assert(strcmp(contents, "Hello, World!") == 0);

  /* Test String Comparision */
  assert(OBCompare((OBObjType *)str1, (OBObjType *)str2) == OB_EQUAL_TO);
  assert(OBCompare((OBObjType *)str1, (OBObjType *)str3) == OB_LESS_THAN);
  assert(OBCompare((OBObjType *)str1, (OBObjType *)null_str) == OB_GREATER_THAN);

  OBRelease((OBObjType *)str2);
  OBRelease((OBObjType *)str3);

  /* Test String Splits */
  str2 = OBStringCreate("Testing string split   into#!many");
  tokens = OBStringComponentsSeparatedBy(str2, " ");
  assert(strcmp(OBStringGetCString((OBString *)OBVectorObjectAtIndex(tokens, 0)), "Testing")
         == 0);
  assert(strcmp(OBStringGetCString((OBString *)OBVectorObjectAtIndex(tokens, 3)),
                           "into#!many") == 0);

  OBRelease((OBObjType *)tokens);

  tokens = OBStringComponentsSeparatedBy(str2, "#!");
  assert(strcmp(OBStringGetCString((OBString *)OBVectorObjectAtIndex(tokens, 0)),
                           "Testing string split   into") == 0);
  assert(strcmp(OBStringGetCString((OBString *)OBVectorObjectAtIndex(tokens, 1)), "many")
         == 0);

  OBRelease((OBObjType *)tokens);

  /* Test String Search */

  assert(OBStringContains(str2, "Testing") != 0);
  assert(OBStringContains(str2, "#!many") != 0);
  assert(OBStringContains(str2, "Hello World") == 0);

  /* Test Regex Match */
  str3 = OBStringMatchRegex(str1, "[Hh]el{1,2}.");
  assert(strcmp(OBStringGetCString(str3), "Hello") == 0);
  OBRelease((OBObjType *)str3);

  str3 = OBStringMatchRegex(str2, " *into[#!]{2,2}.*$");
  assert(strcmp(OBStringGetCString(str3), "   into#!many") == 0);

  OBRelease((OBObjType *)str3);
  OBRelease((OBObjType *)str2);
  OBRelease((OBObjType *)str1);
  OBRelease((OBObjType *)null_str);
    
  /* TESTS COMPLETE */
  printf("OBString_test: TESTS PASSED\n");

  return 0;
}
