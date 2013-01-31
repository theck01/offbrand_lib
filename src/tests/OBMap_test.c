/**
 * @file OBMap_test.c
 * @brief OBMap Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/OBMap.h"
#include "../../include/private/OBMap_Private.h"
#include "../../include/OBTest.h"

/** Size of array to use in testing larger Map capacities */
#define ARRAY_SIZE 2048

/**
 * @brief Main unit testing routine
 */
int main (){

  uint32_t i;

  OBMap *test_map, *map_copy;
  OBTest *a, *b, *c, *d, *e, *f, *g, *h;
  OBTest *test_array[ARRAY_SIZE];
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

  map_copy = copyMap(test_map);
  addToMap(test_map, (obj *)e, NULL); /* remove binding of e -> c */
  test = (OBTest *)lookupMapKey(map_copy, (obj *)e);

  assert(compare((obj *)test, (obj *)c) == OB_EQUAL_TO);
  
  removeMapKey(test_map, (obj *)e);
  test = (OBTest *)lookupMapKey(map_copy, (obj *)e);

  assert(compare((obj *)test, (obj *)c) == OB_EQUAL_TO);

  test = (OBTest *)lookupMapKey(test_map, (obj *)e);

  assert(test == NULL);

  assert(hash((obj *)test_map) != hash((obj *)map_copy));
  assert(compare((obj *)test_map, (obj *)map_copy) != OB_EQUAL_TO);

  addToMap(test_map, (obj *)e, (obj *)c);

  assert(hash((obj *)test_map) == hash((obj *)map_copy));
  assert(compare((obj *)test_map, (obj *)map_copy) == OB_EQUAL_TO);

  clearMap(map_copy);

  assert(lookupMapKey(map_copy, (obj *)e) == NULL);
  assert(lookupMapKey(map_copy, (obj *)a) == NULL);
  assert(lookupMapKey(map_copy, (obj *)f) == NULL);

  for(i=0; i<ARRAY_SIZE; i++){
    test_array[i] = createTest(i);
    addToMap(test_map, (obj *)test_array[i], (obj *)test_array[i]);
  }

  for(i=0; i<ARRAY_SIZE; i++)
    assert(compare((obj *) test_array[i], 
                   lookupMapKey(test_map, (obj *)test_array[i])) ==OB_EQUAL_TO);

  release((obj *)a);
  release((obj *)b);
  release((obj *)c);
  release((obj *)d);
  release((obj *)e);
  release((obj *)f);
  release((obj *)g);
  release((obj *)h);

  release((obj *)test_map);
  release((obj *)map_copy);

  for(i=0; i<ARRAY_SIZE; i++)
    release((obj *)test_array[i]);
    
  printf("OBMap_test: TESTS PASSED\n");

  return 0;
}
