/**
 * @file OBDeque_test.c
 * @brief OBVector Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/OBDeque.h"
#include "../../include/OBTest.h"

/** main unit test routine */
int main (){

  OBDeque *test_deque_a, *test_deque_b, *joined_deque;
  OBDequeIterator *head_it, *tail_it, *copy_it;
  OBTest *a, *b, *c, *d, *e;
  uint32_t i;

  /* create test objects */
  test_deque_a = createDeque();
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
  addDequeHead(test_deque_a, (OBObjType *)a);

  assert(isDequeEmpty(test_deque_a) == 0);
  assert(dequeLength(test_deque_a) == 1);
  assert(OBReferenceCount((OBObjType *)a) == 2);
  assert(getTestID((OBTest *)objAtDequeHead(test_deque_a)) == 1);
  assert(getTestID((OBTest *)objAtDequeTail(test_deque_a)) == 1);

  head_it = getDequeHeadIt(test_deque_a);
  tail_it = getDequeTailIt(test_deque_a);

  assert(head_it != NULL);
  assert(tail_it != NULL);
  assert(getTestID((OBTest *)objAtDequeIt(test_deque_a, head_it)) == 1);
  assert(getTestID((OBTest *)objAtDequeIt(test_deque_a, tail_it)) == 1);

  assert(findObjInDeque(test_deque_a, (OBObjType *)a));

  /* test removing the only element from the deque */
  removeDequeTail(test_deque_a);

  /* check that element is still referenced because of existing iterators */
  assert(OBReferenceCount((OBObjType *)a) == 2);
  OBRelease((OBObjType *)head_it);
  OBRelease((OBObjType *)tail_it);
  assert(OBReferenceCount((OBObjType *)a) == 1);


  assert(isDequeEmpty(test_deque_a) != 0);
  assert(dequeLength(test_deque_a) == 0);
  assert(getDequeHeadIt(test_deque_a) == NULL);
  assert(getDequeTailIt(test_deque_a) == NULL);

  /* test deque with multiple elements */
  addDequeTail(test_deque_a, (OBObjType *)a);
  addDequeHead(test_deque_a, (OBObjType *)b);
  addDequeTail(test_deque_a, (OBObjType *)c);
  addDequeHead(test_deque_a, (OBObjType *)d);
  addDequeTail(test_deque_a, (OBObjType *)e);

  assert(isDequeEmpty(test_deque_a) == 0);
  assert(dequeLength(test_deque_a) == 5);

  /* test deque removal functions */
  head_it = getDequeHeadIt(test_deque_a);
  removeDequeAtIt(test_deque_a, head_it);
  OBRelease((OBObjType *)head_it);

  head_it = getDequeHeadIt(test_deque_a);
  do{
    assert(getTestID((OBTest *)objAtDequeIt(test_deque_a, head_it)) != 4);
  }while(iterateDequeNext(test_deque_a, head_it));

  OBRelease((OBObjType *)head_it);
  addDequeHead(test_deque_a, (OBObjType *)d);

  test_deque_b = copyDeque(test_deque_a);

  assert(OBReferenceCount((OBObjType *)a) == 3);
  assert(OBCompare((OBObjType *)test_deque_a, (OBObjType *)test_deque_b) == OB_EQUAL_TO);
  assert(OBHash((OBObjType *)test_deque_a) ==  OBHash((OBObjType *)test_deque_b));

  sortDeque(test_deque_a, OB_LEAST_TO_GREATEST);

  assert(dequeLength(test_deque_a) == 5);
  assert(OBCompare((OBObjType *)test_deque_a, (OBObjType *)test_deque_b) == OB_NOT_EQUAL);
  assert(OBHash((OBObjType *)test_deque_a) !=  OBHash((OBObjType *)test_deque_b));

  i = 1;
  head_it = getDequeHeadIt(test_deque_a);
  do{
    assert(getTestID((OBTest *)objAtDequeIt(test_deque_a, head_it)) == i);
    i++;
  }while(iterateDequeNext(test_deque_a, head_it));

  OBRelease((OBObjType *)head_it);
  OBRelease((OBObjType *)test_deque_a);
  assert(OBReferenceCount((OBObjType *)a) == 2);

  test_deque_a = copyDeque(test_deque_b);
  joined_deque = joinDeques(test_deque_a, test_deque_b);
  sortDeque(joined_deque, OB_GREATEST_TO_LEAST);

  assert(dequeLength(joined_deque) == 10);
  assert(OBReferenceCount((OBObjType *)a) == 5); /* the joined deque has two references,
                                            as a is contained within twice */
  
  tail_it = getDequeTailIt(joined_deque);
  removeDequeTail(joined_deque);
  assert(iterateDequePrev(joined_deque, tail_it) == 0);
  assert(iterateDequeNext(joined_deque, tail_it) == 0);
  OBRelease((OBObjType *)tail_it);

  tail_it = getDequeTailIt(joined_deque);
  addAtDequeIt(joined_deque, tail_it, (OBObjType *)a);
  OBRelease((OBObjType *)tail_it);

  tail_it = getDequeTailIt(joined_deque);
  copy_it = copyDequeIterator(tail_it);
  OBRelease((OBObjType *)tail_it);
  tail_it = copy_it;
  

  for(i=0; i<10; i++){
    assert(getTestID((OBTest *)objAtDequeIt(joined_deque, tail_it)) == i/2+1);
    if(i != 9) assert(iterateDequePrev(joined_deque, tail_it));
  }

  assert(iterateDequePrev(joined_deque, tail_it) == 0);

  OBRelease((OBObjType *)tail_it);

  clearDeque(test_deque_a);
  clearDeque(test_deque_b);

  for(i=0; i<10; i++){
    removeDequeHead(joined_deque);
  }

  assert(isDequeEmpty(joined_deque) != 0);
  assert(dequeLength(joined_deque) == 0);
  assert(OBReferenceCount((OBObjType *)a) == 1);

  OBRelease((OBObjType *)test_deque_a);
  OBRelease((OBObjType *)test_deque_b);
  OBRelease((OBObjType *)joined_deque);
  OBRelease((OBObjType *)e);
  OBRelease((OBObjType *)d);
  OBRelease((OBObjType *)c);
  OBRelease((OBObjType *)b);
  OBRelease((OBObjType *)a);

  printf("OBDeque_test: TESTS PASSED\n");
  return 0;
}
