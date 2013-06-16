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

  assert(hash((OBObjType *)a)%MAP_CAPACITIES[0] == hash((OBObjType *)e)%MAP_CAPACITIES[0]);

  assert(lookupMapKey(test_map, (OBObjType *)a) == NULL);

  addToMap(test_map, (OBObjType *)a, (OBObjType *)b);
  test = (OBTest *)lookupMapKey(test_map, (OBObjType *)a);

  assert(test);
  assert(compare((OBObjType *)b, (OBObjType *)test) == OB_EQUAL_TO);
  assert(OBReferenceCount((OBObjType *)test) > 1);

  addToMap(test_map, (OBObjType *)e, (OBObjType *)c);
  addToMap(test_map, (OBObjType *)f, (OBObjType *)d);
  addToMap(test_map, (OBObjType *)g, (OBObjType *)e);
  addToMap(test_map, (OBObjType *)h, (OBObjType *)f);
  test = (OBTest *)lookupMapKey(test_map, (OBObjType *)a);

  assert(compare((OBObjType *)b, (OBObjType *)test) == OB_EQUAL_TO);
  assert(compare((OBObjType *)c, (OBObjType *)test) != OB_EQUAL_TO);

  test = (OBTest *)lookupMapKey(test_map, (OBObjType *)e);
  
  assert(compare((OBObjType *)c, (OBObjType *)test) == OB_EQUAL_TO);
  assert(compare((OBObjType *)b, (OBObjType *)test) != OB_EQUAL_TO);
  
  test = (OBTest *)lookupMapKey(test_map, (OBObjType *)a);
  addToMap(test_map, (OBObjType *)a, NULL); /* remove binding of a -> b */

  assert(lookupMapKey(test_map, (OBObjType *)a) == NULL);
  assert(OBReferenceCount((OBObjType *)test) == 1);
  
  test = (OBTest *)lookupMapKey(test_map, (OBObjType *)h);
  assert(compare((OBObjType *)f, (OBObjType *)test) == OB_EQUAL_TO);

  map_copy = copyMap(test_map);
  addToMap(test_map, (OBObjType *)e, NULL); /* remove binding of e -> c */
  test = (OBTest *)lookupMapKey(map_copy, (OBObjType *)e);

  assert(compare((OBObjType *)test, (OBObjType *)c) == OB_EQUAL_TO);
  
  removeMapKey(test_map, (OBObjType *)e);
  test = (OBTest *)lookupMapKey(map_copy, (OBObjType *)e);

  assert(compare((OBObjType *)test, (OBObjType *)c) == OB_EQUAL_TO);

  test = (OBTest *)lookupMapKey(test_map, (OBObjType *)e);

  assert(test == NULL);

  assert(hash((OBObjType *)test_map) != hash((OBObjType *)map_copy));
  assert(compare((OBObjType *)test_map, (OBObjType *)map_copy) != OB_EQUAL_TO);

  addToMap(test_map, (OBObjType *)e, (OBObjType *)c);

  assert(hash((OBObjType *)test_map) == hash((OBObjType *)map_copy));
  assert(compare((OBObjType *)test_map, (OBObjType *)map_copy) == OB_EQUAL_TO);

  clearMap(map_copy);

  assert(lookupMapKey(map_copy, (OBObjType *)e) == NULL);
  assert(lookupMapKey(map_copy, (OBObjType *)a) == NULL);
  assert(lookupMapKey(map_copy, (OBObjType *)f) == NULL);

  for(i=0; i<ARRAY_SIZE; i++){
    test_array[i] = createTest(i);
    addToMap(test_map, (OBObjType *)test_array[i], (OBObjType *)test_array[i]);
  }

  for(i=0; i<ARRAY_SIZE; i++)
    assert(compare((OBObjType *) test_array[i], 
                   lookupMapKey(test_map, (OBObjType *)test_array[i])) ==OB_EQUAL_TO);

  OBRelease((OBObjType *)a);
  OBRelease((OBObjType *)b);
  OBRelease((OBObjType *)c);
  OBRelease((OBObjType *)d);
  OBRelease((OBObjType *)e);
  OBRelease((OBObjType *)f);
  OBRelease((OBObjType *)g);
  OBRelease((OBObjType *)h);

  OBRelease((OBObjType *)test_map);
  OBRelease((OBObjType *)map_copy);

  for(i=0; i<ARRAY_SIZE; i++)
    OBRelease((OBObjType *)test_array[i]);
    
  printf("OBMap_test: TESTS PASSED\n");

  return 0;
}
