/**
 * @file OBMap_test.c
 * @brief OBMap Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/OBMap.h"
#include "../../include/private/OBMap_Private.h"
#include "../../include/OBTest.h"

/**
 * @brief Main unit testing routine
 */
int main (){

  /* ADD NECESSARY UNIT TESTS HERE */
  OBMap *test_map, *map_copy;
  OBTest *a, *b, *c, *d, *e, *f, *g, *h;
  OBTest *test;

  test_map = createMap(); 
  a = createTest(1);
  b = createTest(2);
  c = createTest(3);
  d = createTest(103923);
  e = createTest(1+MAP_CAPACITIES[0]); /* ensures hash collision with a */
  f = createTest(1+2*MAP_CAPACITIES[0]);
  g = createTest(1+3*MAP_CAPACITIES[0]);
  h = createTest(1+4*MAP_CAPACITIES[0]);

  assert(hash((obj *)a)%MAP_CAPACITIES[0] == hash((obj *)e)%MAP_CAPACITIES[0]);

  assert(lookupMapKey(test_map, (obj *)a) == NULL);

  addToMap(test_map, (obj *)a, (obj *)b);
  test = (OBTest *)lookupMapKey(test_map, (obj *)a);

  assert(test);
  assert(compare((obj *)b, (obj *)test) == OB_EQUAL_TO);
  assert(referenceCount((obj *)test) > 1);

  addToMap(test_map, (obj *)e, (obj *)c);
  addToMap(test_map, (obj *)f, (obj *)d);
  addToMap(test_map, (obj *)g, (obj *)e);
  addToMap(test_map, (obj *)h, (obj *)f);
  test = (OBTest *)lookupMapKey(test_map, (obj *)a);

  assert(compare((obj *)b, (obj *)test) == OB_EQUAL_TO);
  assert(compare((obj *)c, (obj *)test) != OB_EQUAL_TO);

  test = (OBTest *)lookupMapKey(test_map, (obj *)e);
  
  assert(compare((obj *)c, (obj *)test) == OB_EQUAL_TO);
  assert(compare((obj *)b, (obj *)test) != OB_EQUAL_TO);
  
  test = (OBTest *)lookupMapKey(test_map, (obj *)a);
  addToMap(test_map, (obj *)a, NULL); /* remove binding of a -> b */

  assert(lookupMapKey(test_map, (obj *)a) == NULL);
  assert(referenceCount((obj *)test) == 1);
  
  test = (OBTest *)lookupMapKey(test_map, (obj *)h);
  assert(compare((obj *)f, (obj *)test) == OB_EQUAL_TO);

  printf("OBMap_test: TESTS PASSED\n");
  return 0;
}
