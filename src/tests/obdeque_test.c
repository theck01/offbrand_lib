/**
 * @file obdeque_test.c
 * @brief OBVector Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/obdeque.h"
#include "../../include/obtest.h"

/** main unit test routine */
int main (){

  obdeque *test_deque_a, *test_deque_b, *joined_deque;
  obdeque_iterator *head_it, *tail_it, *copy_it;
  obtest *a, *b, *c, *d, *e;
  uint32_t i;

  /* create test objects */
  test_deque_a = obdeque_new();
  a = obtest_new(1);
  b = obtest_new(2);
  c = obtest_new(3);
  d = obtest_new(4);
  e = obtest_new(5);


  /* test the empty deque */
  assert(obdeque_is_empty(test_deque_a) != 0);
  assert(obdeque_length(test_deque_a) == 0);
  assert(obdeque_head_iterator(test_deque_a) == NULL);
  assert(obdeque_tail_iterator(test_deque_a) == NULL);

  /* test a deque with a single element */
  obdeque_add_at_head(test_deque_a, (obj *)a);

  assert(obdeque_is_empty(test_deque_a) == 0);
  assert(obdeque_length(test_deque_a) == 1);
  assert(ob_reference_count((obj *)a) == 2);
  assert(obtest_id((obtest *)obdeque_obj_at_head(test_deque_a)) == 1);
  assert(obtest_id((obtest *)obdeque_obj_at_tail(test_deque_a)) == 1);

  head_it = obdeque_head_iterator(test_deque_a);
  tail_it = obdeque_tail_iterator(test_deque_a);

  assert(head_it != NULL);
  assert(tail_it != NULL);
  assert(obtest_id((obtest *)obdeque_obj_at_iterator(test_deque_a, head_it)) == 1);
  assert(obtest_id((obtest *)obdeque_obj_at_iterator(test_deque_a, tail_it)) == 1);

  assert(obdeque_find_obj(test_deque_a, (obj *)a));

  /* test removing the only element from the deque */
  obdeque_remove_tail(test_deque_a);

  /* check that element is still referenced because of existing iterators */
  assert(ob_reference_count((obj *)a) == 2);
  ob_release((obj *)head_it);
  ob_release((obj *)tail_it);
  assert(ob_reference_count((obj *)a) == 1);


  assert(obdeque_is_empty(test_deque_a) != 0);
  assert(obdeque_length(test_deque_a) == 0);
  assert(obdeque_head_iterator(test_deque_a) == NULL);
  assert(obdeque_tail_iterator(test_deque_a) == NULL);

  /* test deque with multiple elements */
  obdeque_add_at_tail(test_deque_a, (obj *)a);
  obdeque_add_at_head(test_deque_a, (obj *)b);
  obdeque_add_at_tail(test_deque_a, (obj *)c);
  obdeque_add_at_head(test_deque_a, (obj *)d);
  obdeque_add_at_tail(test_deque_a, (obj *)e);

  assert(obdeque_is_empty(test_deque_a) == 0);
  assert(obdeque_length(test_deque_a) == 5);

  /* test deque removal functions */
  head_it = obdeque_head_iterator(test_deque_a);
  obdeque_remove_at_iterator(test_deque_a, head_it);
  ob_release((obj *)head_it);

  head_it = obdeque_head_iterator(test_deque_a);
  do{
    assert(obtest_id((obtest *)obdeque_obj_at_iterator(test_deque_a, head_it)) != 4);
  }while(obdeque_iterate_next(test_deque_a, head_it));

  ob_release((obj *)head_it);
  obdeque_add_at_head(test_deque_a, (obj *)d);

  test_deque_b = obdeque_copy(test_deque_a);

  assert(ob_reference_count((obj *)a) == 3);
  assert(ob_compare((obj *)test_deque_a, (obj *)test_deque_b) == OB_EQUAL_TO);
  assert(ob_hash((obj *)test_deque_a) ==  ob_hash((obj *)test_deque_b));

  obdeque_sort(test_deque_a, OB_LEAST_TO_GREATEST);

  assert(obdeque_length(test_deque_a) == 5);
  assert(ob_compare((obj *)test_deque_a, (obj *)test_deque_b) == OB_NOT_EQUAL);
  assert(ob_hash((obj *)test_deque_a) !=  ob_hash((obj *)test_deque_b));

  i = 1;
  head_it = obdeque_head_iterator(test_deque_a);
  do{
    assert(obtest_id((obtest *)obdeque_obj_at_iterator(test_deque_a, head_it)) == i);
    i++;
  }while(obdeque_iterate_next(test_deque_a, head_it));

  ob_release((obj *)head_it);
  ob_release((obj *)test_deque_a);
  assert(ob_reference_count((obj *)a) == 2);

  test_deque_a = obdeque_copy(test_deque_b);
  joined_deque = obdeque_join(test_deque_a, test_deque_b);
  obdeque_sort(joined_deque, OB_GREATEST_TO_LEAST);

  assert(obdeque_length(joined_deque) == 10);
  assert(ob_reference_count((obj *)a) == 5); /* the joined deque has two references,
                                            as a is contained within twice */

  tail_it = obdeque_tail_iterator(joined_deque);
  obdeque_remove_tail(joined_deque);
  assert(obdeque_iterate_prev(joined_deque, tail_it) == 0);
  assert(obdeque_iterate_next(joined_deque, tail_it) == 0);
  ob_release((obj *)tail_it);

  tail_it = obdeque_tail_iterator(joined_deque);
  obdeque_add_at_iterator(joined_deque, tail_it, (obj *)a);
  ob_release((obj *)tail_it);

  tail_it = obdeque_tail_iterator(joined_deque);
  copy_it = obdeque_copy_iterator(tail_it);
  ob_release((obj *)tail_it);
  tail_it = copy_it;


  for(i=0; i<10; i++){
    assert(obtest_id((obtest *)obdeque_obj_at_iterator(joined_deque, tail_it)) == i/2+1);
    if(i != 9) assert(obdeque_iterate_prev(joined_deque, tail_it));
  }

  assert(obdeque_iterate_prev(joined_deque, tail_it) == 0);

  ob_release((obj *)tail_it);

  obdeque_clear(test_deque_a);
  obdeque_clear(test_deque_b);

  for(i=0; i<10; i++){
    obdeque_remove_head(joined_deque);
  }

  assert(obdeque_is_empty(joined_deque) != 0);
  assert(obdeque_length(joined_deque) == 0);
  assert(ob_reference_count((obj *)a) == 1);

  ob_release((obj *)test_deque_a);
  ob_release((obj *)test_deque_b);
  ob_release((obj *)joined_deque);
  ob_release((obj *)e);
  ob_release((obj *)d);
  ob_release((obj *)c);
  ob_release((obj *)b);
  ob_release((obj *)a);

  printf("obdeque: TESTS PASSED\n");
  return 0;
}
