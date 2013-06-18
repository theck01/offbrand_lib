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

  test_map = OBMapCreate(); 
  a = OBTestCreate(1);
  b = OBTestCreate(2);
  c = OBTestCreate(3);
  d = OBTestCreate(103923);
  e = OBTestCreate(1+MAP_CAPACITIES[0]); /* ensures hash collision with a */
  f = OBTestCreate(1+2*MAP_CAPACITIES[0]);
  g = OBTestCreate(1+3*MAP_CAPACITIES[0]);
  h = OBTestCreate(1+4*MAP_CAPACITIES[0]);

  assert(OBHash((OBObjType *)a)%MAP_CAPACITIES[0] == OBHash((OBObjType *)e)%MAP_CAPACITIES[0]);

  assert(OBMapGetValueForKey(test_map, (OBObjType *)a) == NULL);

  OBMapAdd(test_map, (OBObjType *)a, (OBObjType *)b);
  test = (OBTest *)OBMapGetValueForKey(test_map, (OBObjType *)a);

  assert(test);
  assert(OBCompare((OBObjType *)b, (OBObjType *)test) == OB_EQUAL_TO);
  assert(OBReferenceCount((OBObjType *)test) > 1);

  OBMapAdd(test_map, (OBObjType *)e, (OBObjType *)c);
  OBMapAdd(test_map, (OBObjType *)f, (OBObjType *)d);
  OBMapAdd(test_map, (OBObjType *)g, (OBObjType *)e);
  OBMapAdd(test_map, (OBObjType *)h, (OBObjType *)f);
  test = (OBTest *)OBMapGetValueForKey(test_map, (OBObjType *)a);

  assert(OBCompare((OBObjType *)b, (OBObjType *)test) == OB_EQUAL_TO);
  assert(OBCompare((OBObjType *)c, (OBObjType *)test) != OB_EQUAL_TO);

  test = (OBTest *)OBMapGetValueForKey(test_map, (OBObjType *)e);
  
  assert(OBCompare((OBObjType *)c, (OBObjType *)test) == OB_EQUAL_TO);
  assert(OBCompare((OBObjType *)b, (OBObjType *)test) != OB_EQUAL_TO);
  
  test = (OBTest *)OBMapGetValueForKey(test_map, (OBObjType *)a);
  OBMapAdd(test_map, (OBObjType *)a, NULL); /* remove binding of a -> b */

  assert(OBMapGetValueForKey(test_map, (OBObjType *)a) == NULL);
  assert(OBReferenceCount((OBObjType *)test) == 1);
  
  test = (OBTest *)OBMapGetValueForKey(test_map, (OBObjType *)h);
  assert(OBCompare((OBObjType *)f, (OBObjType *)test) == OB_EQUAL_TO);

  map_copy = OBMapCopy(test_map);
  OBMapAdd(test_map, (OBObjType *)e, NULL); /* remove binding of e -> c */
  test = (OBTest *)OBMapGetValueForKey(map_copy, (OBObjType *)e);

  assert(OBCompare((OBObjType *)test, (OBObjType *)c) == OB_EQUAL_TO);
  
  OBMapRemoveKey(test_map, (OBObjType *)e);
  test = (OBTest *)OBMapGetValueForKey(map_copy, (OBObjType *)e);

  assert(OBCompare((OBObjType *)test, (OBObjType *)c) == OB_EQUAL_TO);

  test = (OBTest *)OBMapGetValueForKey(test_map, (OBObjType *)e);

  assert(test == NULL);

  assert(OBHash((OBObjType *)test_map) != OBHash((OBObjType *)map_copy));
  assert(OBCompare((OBObjType *)test_map, (OBObjType *)map_copy) != OB_EQUAL_TO);

  OBMapAdd(test_map, (OBObjType *)e, (OBObjType *)c);

  assert(OBHash((OBObjType *)test_map) == OBHash((OBObjType *)map_copy));
  assert(OBCompare((OBObjType *)test_map, (OBObjType *)map_copy) == OB_EQUAL_TO);

  OBMapClear(map_copy);

  assert(OBMapGetValueForKey(map_copy, (OBObjType *)e) == NULL);
  assert(OBMapGetValueForKey(map_copy, (OBObjType *)a) == NULL);
  assert(OBMapGetValueForKey(map_copy, (OBObjType *)f) == NULL);

  for(i=0; i<ARRAY_SIZE; i++){
    test_array[i] = OBTestCreate(i);
    OBMapAdd(test_map, (OBObjType *)test_array[i], (OBObjType *)test_array[i]);
  }

  for(i=0; i<ARRAY_SIZE; i++)
    assert(OBCompare((OBObjType *) test_array[i], 
                   OBMapGetValueForKey(test_map, (OBObjType *)test_array[i])) ==OB_EQUAL_TO);

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
