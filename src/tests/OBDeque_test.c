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
  test_deque_a = OBDequeCreate();
  a = createTest(1);
  b = createTest(2);
  c = createTest(3);
  d = createTest(4);
  e = createTest(5);


  /* test the empty deque */
  assert(OBDequeIsEmpty(test_deque_a) != 0);
  assert(OBDequeLength(test_deque_a) == 0);
  assert(OBDequeGetHeadIterator(test_deque_a) == NULL);
  assert(OBDequeGetTailIterator(test_deque_a) == NULL);

  /* test a deque with a single element */
  OBDequeAddHead(test_deque_a, (OBObjType *)a);

  assert(OBDequeIsEmpty(test_deque_a) == 0);
  assert(OBDequeLength(test_deque_a) == 1);
  assert(OBReferenceCount((OBObjType *)a) == 2);
  assert(getTestID((OBTest *)OBDequeFirstObject(test_deque_a)) == 1);
  assert(getTestID((OBTest *)OBDequeLastObject(test_deque_a)) == 1);

  head_it = OBDequeGetHeadIterator(test_deque_a);
  tail_it = OBDequeGetTailIterator(test_deque_a);

  assert(head_it != NULL);
  assert(tail_it != NULL);
  assert(getTestID((OBTest *)OBDequeObjAtIterator(test_deque_a, head_it)) == 1);
  assert(getTestID((OBTest *)OBDequeObjAtIterator(test_deque_a, tail_it)) == 1);

  assert(OBDequeContains(test_deque_a, (OBObjType *)a));

  /* test removing the only element from the deque */
  OBDequeRemoveLastObject(test_deque_a);

  /* check that element is still referenced because of existing iterators */
  assert(OBReferenceCount((OBObjType *)a) == 2);
  OBRelease((OBObjType *)head_it);
  OBRelease((OBObjType *)tail_it);
  assert(OBReferenceCount((OBObjType *)a) == 1);


  assert(OBDequeIsEmpty(test_deque_a) != 0);
  assert(OBDequeLength(test_deque_a) == 0);
  assert(OBDequeGetHeadIterator(test_deque_a) == NULL);
  assert(OBDequeGetTailIterator(test_deque_a) == NULL);

  /* test deque with multiple elements */
  OBDequeAddTail(test_deque_a, (OBObjType *)a);
  OBDequeAddHead(test_deque_a, (OBObjType *)b);
  OBDequeAddTail(test_deque_a, (OBObjType *)c);
  OBDequeAddHead(test_deque_a, (OBObjType *)d);
  OBDequeAddTail(test_deque_a, (OBObjType *)e);

  assert(OBDequeIsEmpty(test_deque_a) == 0);
  assert(OBDequeLength(test_deque_a) == 5);

  /* test deque removal functions */
  head_it = OBDequeGetHeadIterator(test_deque_a);
  OBDequeRemoveAtIterator(test_deque_a, head_it);
  OBRelease((OBObjType *)head_it);

  head_it = OBDequeGetHeadIterator(test_deque_a);
  do{
    assert(getTestID((OBTest *)OBDequeObjAtIterator(test_deque_a, head_it)) != 4);
  }while(OBDequeIterateNext(test_deque_a, head_it));

  OBRelease((OBObjType *)head_it);
  OBDequeAddHead(test_deque_a, (OBObjType *)d);

  test_deque_b = OBDequeCopy(test_deque_a);

  assert(OBReferenceCount((OBObjType *)a) == 3);
  assert(OBCompare((OBObjType *)test_deque_a, (OBObjType *)test_deque_b) == OB_EQUAL_TO);
  assert(OBHash((OBObjType *)test_deque_a) ==  OBHash((OBObjType *)test_deque_b));

  OBDequeSort(test_deque_a, OB_LEAST_TO_GREATEST);

  assert(OBDequeLength(test_deque_a) == 5);
  assert(OBCompare((OBObjType *)test_deque_a, (OBObjType *)test_deque_b) == OB_NOT_EQUAL);
  assert(OBHash((OBObjType *)test_deque_a) !=  OBHash((OBObjType *)test_deque_b));

  i = 1;
  head_it = OBDequeGetHeadIterator(test_deque_a);
  do{
    assert(getTestID((OBTest *)OBDequeObjAtIterator(test_deque_a, head_it)) == i);
    i++;
  }while(OBDequeIterateNext(test_deque_a, head_it));

  OBRelease((OBObjType *)head_it);
  OBRelease((OBObjType *)test_deque_a);
  assert(OBReferenceCount((OBObjType *)a) == 2);

  test_deque_a = OBDequeCopy(test_deque_b);
  joined_deque = OBDequeJoin(test_deque_a, test_deque_b);
  OBDequeSort(joined_deque, OB_GREATEST_TO_LEAST);

  assert(OBDequeLength(joined_deque) == 10);
  assert(OBReferenceCount((OBObjType *)a) == 5); /* the joined deque has two references,
                                            as a is contained within twice */
  
  tail_it = OBDequeGetTailIterator(joined_deque);
  OBDequeRemoveLastObject(joined_deque);
  assert(OBDequeIteratePrevious(joined_deque, tail_it) == 0);
  assert(OBDequeIterateNext(joined_deque, tail_it) == 0);
  OBRelease((OBObjType *)tail_it);

  tail_it = OBDequeGetTailIterator(joined_deque);
  OBDeuqueAddAtIterator(joined_deque, tail_it, (OBObjType *)a);
  OBRelease((OBObjType *)tail_it);

  tail_it = OBDequeGetTailIterator(joined_deque);
  copy_it = OBDequeIteratorCopy(tail_it);
  OBRelease((OBObjType *)tail_it);
  tail_it = copy_it;
  

  for(i=0; i<10; i++){
    assert(getTestID((OBTest *)OBDequeObjAtIterator(joined_deque, tail_it)) == i/2+1);
    if(i != 9) assert(OBDequeIteratePrevious(joined_deque, tail_it));
  }

  assert(OBDequeIteratePrevious(joined_deque, tail_it) == 0);

  OBRelease((OBObjType *)tail_it);

  OBDequeClear(test_deque_a);
  OBDequeClear(test_deque_b);

  for(i=0; i<10; i++){
    OBDequeRemoveFirstObject(joined_deque);
  }

  assert(OBDequeIsEmpty(joined_deque) != 0);
  assert(OBDequeLength(joined_deque) == 0);
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
