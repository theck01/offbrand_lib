/**
 * @file obstring_test.c
 * @brief obstring Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/obstring.h"

/**
 * @brief Main unit testing routine
 */
int main (){

  obstring *str1, *str2, *str3, *null_str;
  obvector *tokens;
  const char *contents;

  str1 = obstring_new("Hello, World!");

  /* Test obstring_new and obstring_cstring */
  contents = obstring_cstring(str1);
  assert(strcmp(contents, "Hello, World!") == 0);


  /* Test obstring_copy_substring and obstring_length */
  str2 = obstring_copy_substring(str1, 0, 5);
  contents = obstring_cstring(str2);
  assert(strcmp(contents, "Hello") == 0);

  ob_release((obj *)str2);

  str2 = obstring_copy_substring(str1, -15, 7);
  contents = obstring_cstring(str2);
  assert(strcmp(contents, "Hello") == 0);

  ob_release((obj *)str2);

  str2 = obstring_copy_substring(str1, 0, obstring_length(str1));
  contents = obstring_cstring(str2);
  assert(strcmp(contents, "Hello, World!") == 0);

  ob_release((obj *)str2);

  null_str = obstring_copy_substring(str1, 20, 2);
  assert(obstring_length(null_str) == 0);

  /* Test obstring_char_at_index */
  assert(obstring_char_at_index(str1, 0) == 'H');
  assert(obstring_char_at_index(str1, -5) == 'o');
  assert(obstring_char_at_index(str1, -20) == '\0');
  assert(obstring_char_at_index(str1, 20) == '\0');
  assert(obstring_char_at_index(null_str, 0) == '\0');

  /* Test String Concatenation */
  str2 = obstring_new(" And hello again!");
  str3 = obstring_concat(str1, str2);
  contents = obstring_cstring(str3);
  assert(strcmp(contents, "Hello, World! And hello again!") == 0);

  ob_release((obj *)str2);

  str2 = obstring_concat(null_str, str1);
  contents = obstring_cstring(str2);
  assert(strcmp(contents, "Hello, World!") == 0);

  /* Test String Comparision */
  assert(ob_compare((obj *)str1, (obj *)str2) == OB_EQUAL_TO);
  assert(ob_compare((obj *)str1, (obj *)str3) == OB_LESS_THAN);
  assert(ob_compare((obj *)str1, (obj *)null_str) == OB_GREATER_THAN);

  ob_release((obj *)str2);
  ob_release((obj *)str3);

  /* Test String Splits */
  str2 = obstring_new("Testing string split   into#!many");
  tokens = obstring_split(str2, " ");
  assert(strcmp(obstring_cstring((obstring *)obvector_obj_at_index(tokens, 0)), "Testing")
         == 0);
  assert(strcmp(obstring_cstring((obstring *)obvector_obj_at_index(tokens, 3)),
                           "into#!many") == 0);

  ob_release((obj *)tokens);

  tokens = obstring_split(str2, "#!");
  assert(strcmp(obstring_cstring((obstring *)obvector_obj_at_index(tokens, 0)),
                           "Testing string split   into") == 0);
  assert(strcmp(obstring_cstring((obstring *)obvector_obj_at_index(tokens, 1)), "many")
         == 0);

  ob_release((obj *)tokens);

  /* Test String Search */

  assert(obstring_find_substring(str2, "Testing") != 0);
  assert(obstring_find_substring(str2, "#!many") != 0);
  assert(obstring_find_substring(str2, "Hello World") == 0);

  /* Test Regex Match */
  str3 = obstring_match_regex(str1, "[Hh]el{1,2}.");
  assert(strcmp(obstring_cstring(str3), "Hello") == 0);
  ob_release((obj *)str3);

  str3 = obstring_match_regex(str2, " *into[#!]{2,2}.*$");
  assert(strcmp(obstring_cstring(str3), "   into#!many") == 0);

  ob_release((obj *)str3);
  ob_release((obj *)str2);
  ob_release((obj *)str1);
  ob_release((obj *)null_str);

  /* TESTS COMPLETE */
  printf("obstring: TESTS PASSED\n");

  return 0;
}
