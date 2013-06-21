/**
 * @file obmap_test.c
 * @brief obmap Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/obmap.h"
#include "../../include/private/obmap_private.h"
#include "../../include/obtest.h"

/** Size of array to use in testing larger Map capacities */
#define ARRAY_SIZE 2048

/**
 * @brief Main unit testing routine
 */
int main (){

  uint32_t i;

  obmap *test_map, *map_copy;
  obtest *a, *b, *c, *d, *e, *f, *g, *h;
  obtest *test_array[ARRAY_SIZE];
  obtest *test;

  test_map = obmap_new();
  a = obtest_new(1);
  b = obtest_new(2);
  c = obtest_new(3);
  d = obtest_new(103923);
  e = obtest_new(1+MAP_CAPACITIES[0]); /* ensures hash collision with a */
  f = obtest_new(1+2*MAP_CAPACITIES[0]);
  g = obtest_new(1+3*MAP_CAPACITIES[0]);
  h = obtest_new(1+4*MAP_CAPACITIES[0]);

  assert(ob_hash((obj *)a)%MAP_CAPACITIES[0] == ob_hash((obj *)e)%MAP_CAPACITIES[0]);

  assert(obmap_lookup(test_map, (obj *)a) == NULL);

  obmap_insert(test_map, (obj *)a, (obj *)b);
  test = (obtest *)obmap_lookup(test_map, (obj *)a);

  assert(test);
  assert(ob_compare((obj *)b, (obj *)test) == OB_EQUAL_TO);
  assert(ob_reference_count((obj *)test) > 1);

  obmap_insert(test_map, (obj *)e, (obj *)c);
  obmap_insert(test_map, (obj *)f, (obj *)d);
  obmap_insert(test_map, (obj *)g, (obj *)e);
  obmap_insert(test_map, (obj *)h, (obj *)f);
  test = (obtest *)obmap_lookup(test_map, (obj *)a);

  assert(ob_compare((obj *)b, (obj *)test) == OB_EQUAL_TO);
  assert(ob_compare((obj *)c, (obj *)test) != OB_EQUAL_TO);

  test = (obtest *)obmap_lookup(test_map, (obj *)e);

  assert(ob_compare((obj *)c, (obj *)test) == OB_EQUAL_TO);
  assert(ob_compare((obj *)b, (obj *)test) != OB_EQUAL_TO);

  test = (obtest *)obmap_lookup(test_map, (obj *)a);
  obmap_insert(test_map, (obj *)a, NULL); /* remove binding of a -> b */

  assert(obmap_lookup(test_map, (obj *)a) == NULL);
  assert(ob_reference_count((obj *)test) == 1);

  test = (obtest *)obmap_lookup(test_map, (obj *)h);
  assert(ob_compare((obj *)f, (obj *)test) == OB_EQUAL_TO);

  map_copy = obmap_copy(test_map);
  obmap_insert(test_map, (obj *)e, NULL); /* remove binding of e -> c */
  test = (obtest *)obmap_lookup(map_copy, (obj *)e);

  assert(ob_compare((obj *)test, (obj *)c) == OB_EQUAL_TO);

  obmap_remove(test_map, (obj *)e);
  test = (obtest *)obmap_lookup(map_copy, (obj *)e);

  assert(ob_compare((obj *)test, (obj *)c) == OB_EQUAL_TO);

  test = (obtest *)obmap_lookup(test_map, (obj *)e);

  assert(test == NULL);

  assert(ob_hash((obj *)test_map) != ob_hash((obj *)map_copy));
  assert(ob_compare((obj *)test_map, (obj *)map_copy) != OB_EQUAL_TO);

  obmap_insert(test_map, (obj *)e, (obj *)c);

  assert(ob_hash((obj *)test_map) == ob_hash((obj *)map_copy));
  assert(ob_compare((obj *)test_map, (obj *)map_copy) == OB_EQUAL_TO);

  obmap_clear(map_copy);

  assert(obmap_lookup(map_copy, (obj *)e) == NULL);
  assert(obmap_lookup(map_copy, (obj *)a) == NULL);
  assert(obmap_lookup(map_copy, (obj *)f) == NULL);

  for(i=0; i<ARRAY_SIZE; i++){
    test_array[i] = obtest_new(i);
    obmap_insert(test_map, (obj *)test_array[i], (obj *)test_array[i]);
  }

  for(i=0; i<ARRAY_SIZE; i++)
    assert(ob_compare((obj *) test_array[i],
                   obmap_lookup(test_map, (obj *)test_array[i])) ==OB_EQUAL_TO);

  ob_release((obj *)a);
  ob_release((obj *)b);
  ob_release((obj *)c);
  ob_release((obj *)d);
  ob_release((obj *)e);
  ob_release((obj *)f);
  ob_release((obj *)g);
  ob_release((obj *)h);

  ob_release((obj *)test_map);
  ob_release((obj *)map_copy);

  for(i=0; i<ARRAY_SIZE; i++)
    ob_release((obj *)test_array[i]);

  printf("obmap: TESTS PASSED\n");

  return 0;
}
