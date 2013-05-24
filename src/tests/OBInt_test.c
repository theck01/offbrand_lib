/**
 * @file OBInt_test.c
 * @brief OBInt Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/OBInt.h"
#include "../../include/OBTest.h"

/**
 * @brief Main unit testing routine
 */
int main (){

  OBInt *a, *b;

  /* test creation and value methods */
  a = createIntWithInt(1024);
  assert(intValue(a) == 1024);
  b = createIntWithInt(9581085);
  assert(intValue(b) == 9581085);

  printf("OBInt_test: TESTS PASSED\n");
  return 0;
}
