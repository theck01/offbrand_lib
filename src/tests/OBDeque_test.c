
#include "../../include/offbrand.h"
#include "../../include/OBDeque.h"
#include "../../include/OBTest.h"

int main (){

  OBDeque *test_deque_a, *test_deque_b;
  OBDequeIterator *head_it, *tail_it;
  OBTest *a, *b, *c, *d, *e;

  /* create test objects */
  test_deque_a = createEmptyDeque();
  a = createTest(1);
  b = createTest(2);
  c = createTest(3);
  d = createTest(4);
  e = createTest(5);


  /* test the empty deque */
  assert(isDequeEmpty(test_deque_a) != 0);
  assert(dequeLength(test_deque_a) == 0);
  assert(getDequeHeadIt(test_deque_a) == NULL);
  assert(getDequeTailIt(test_deque_a) == NULL);

  /* test a deque with a single element */
  addDequeHead(test_deque_a, (obj *)a);

  assert(isDequeEmpty(test_deque_a) == 0);
  assert(dequeLength(test_deque_a) == 1);
  assert(referenceCount((obj *)a) == 2);
  assert(getTestID((OBTest *)peekDequeHead(test_deque_a)) == 1);
  assert(getTestID((OBTest *)peekDequeTail(test_deque_a)) == 1);

  head_it = getDequeHeadIt(test_deque_a);
  tail_it = getDequeTailIt(test_deque_a);

  assert(head_it != NULL);
  assert(tail_it != NULL);
  assert(getTestID((OBTest *)peekDequeAtIt(test_deque_a, head_it)) == 1);
  assert(getTestID((OBTest *)peekDequeAtIt(test_deque_a, tail_it)) == 1);

  assert(findObjInDeque(test_deque_a, a, &compareTests));

  /* test removing the only element from the deque */
  removeDequeTail(test_deque_a);

  /* check that element is still referenced by existing iterators */
  assert(referenceCount((obj *)a) == 2);
  release((obj *)head_it);
  release((obj *)tail_it);
  assert(referenceCount((obj *)a) == 1);


  assert(isDequeEmpty(test_deque_a) != 0);
  assert(dequeLength(test_deque_a) == 0);
  assert(getDequeHeadIt(test_deque_a) == NULL);
  assert(getDequeTailIt(test_deque_a) == NULL);

  /* test deque with multiple elements */
  addDequeTail(test_deque_a, (obj *)a);
  addDequeHead(test_deque_a, (obj *)b);
  addDequeTail(test_deque_a, (obj *)c);
  addDequeHead(test_deque_a, (obj *)d);
  addDequeTail(test_deque_a, (obj *)e);

  assert(isDequeEmpty(test_deque_a) == 0);
  assert(dequeLength(test_deque_a) == 5);

  test_deque_b = copyDeque(test_deque_a);
  assert(referenceCount((obj *)a) == 3);

  printf("OBDeque_test: TESTS PASSED\n");
  return 0;
  
}
