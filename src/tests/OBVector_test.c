/**
 * @file OBVector_test.c
 * @brief OBVector Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/OBVector.h"
#include "../../include/private/OBVector_Private.h" /* For testing purposes
                                                       only */
#include "../../include/OBTest.h"

/** main unit testing routine */
int main(){

  uint32_t i, id;
  OBTest *tests[6], *singleton, *tmp;
  OBVector *main_vec, *copy_vec;

  main_vec = OBVectorCreateWithCapacity(1);
  singleton = OBTestCreate(7);

  for(i=0; i<6; i++){
    tests[i] = OBTestCreate(i);
    OBVectorStoreAtIndex(main_vec, (OBObjType *)tests[i], i);

    if(OBReferenceCount((OBObjType *)tests[i]) != 2){
      fprintf(stderr, "OBVector_test: vector did not properly retain element "
                      "TEST FAILED\n");
      exit(1);
    }

    OBRelease((OBObjType *)tests[i]); /* vector should maintain only references */
  }

  if(OBVectorGetLength(main_vec) != 6){
    fprintf(stderr, "OBVector_test: vector size is off, TEST FAILED\n");
    exit(1);
  }

  for(i=0; i<6; i++){
    if(OBTestGetID((OBTest *)OBVectorObjectAtIndex(main_vec,i)) != i){
      fprintf(stderr, "OBVector_test: vector does not contain added contents "
                      "in expected order\nTEST FAILED\n");
      printf("%d\n", i);
      exit(1);
    }
  }

  OBVectorStoreAtIndex(main_vec, (OBObjType *)singleton, 12);
  if(OBVectorGetLength(main_vec) != 13){
    fprintf(stderr, "OBVector_test: store did not properly extend length "
                    "TEST FAILED\n");
    exit(1);
  }


  OBVectorStoreAtIndex(main_vec, NULL, 12);
  if(OBVectorGetLength(main_vec) != 6){
    fprintf(stderr, "OBVector_test: remove did not properly get rid of element "
                    "TEST FAILED\n");
    exit(1);
  }

  if(OBReferenceCount((OBObjType *)singleton) != 1){
    fprintf(stderr, "OBVector_test: vector did not release element on removal "
                    "TEST FAILED\n");
    exit(1);
  }

  tmp = (OBTest *)OBVectorObjectAtIndex(main_vec, 3);
  OBRetain((OBObjType *)tmp);

  OBVectorStoreAtIndex(main_vec, (OBObjType *)singleton, 3);
  if(OBReferenceCount((OBObjType *)tmp) != 1){
    fprintf(stderr, "OBVector_test: vector did not release element on "
                    "replacement, TEST FAILED\n");
    exit(1);
  }

  if(OBVectorGetLength(main_vec) != 6 ||
     OBTestGetID((OBTest *)OBVectorObjectAtIndex(main_vec, 3)) != 
     OBTestGetID(singleton)){
    fprintf(stderr, "OBVector_test: element was not added properly to vector "
                    "at index\nTEST FAILED\n");
    exit(1);
  }

  OBVectorStoreAtIndex(main_vec, (OBObjType *)tmp, 3);

  /* reduce reference count to 1, so only vector holds reference to obj */
  OBRelease((OBObjType *)tmp);

  if(OBVectorContains(main_vec, (OBObjType *)singleton)){
    fprintf(stderr, "OBVector_test: vector did not replace element correctly, "
                    "or was not found\nTEST FAILED\n");
    exit(1);
  }
  
  OBVectorStoreAtIndex(main_vec, (OBObjType *)singleton, 10);
  if(OBVectorGetLength(main_vec) != 11){
    fprintf(stderr, "OBVector_test: store did not properly extend length "
                    "TEST FAILED\n");
    exit(1);
  }

  OBVectorSort(main_vec, OB_LEAST_TO_GREATEST);
  if(OBVectorGetLength(main_vec) != 7){
    fprintf(stderr, "OBVector_test: sort did not properly remove NULLs "
                    "TEST FAILED\n");
    fprintf(stderr, "OBVector_test: vector size == %d\n", 
                    OBVectorGetLength(main_vec));
    for(i=0; i<OBVectorGetLength(main_vec); i++){
      if(OBVectorObjectAtIndex(main_vec, i)) printf("object\n");
      else printf("NULL\n");
    }
    exit(1);
  }

  id = 0;
  for(i=0; i<OBVectorGetLength(main_vec); i++){
    if(id > OBTestGetID((OBTest *)OBVectorObjectAtIndex(main_vec, i))){
      fprintf(stderr, "OBVector_test: vector was not sorted least to "
                      "greatest, TEST FAILED\n");
      exit(1);
    }
    id = OBTestGetID((OBTest *)OBVectorObjectAtIndex(main_vec, i));
  }

  OBVectorStoreAtIndex(main_vec, NULL, 6);
  OBVectorStoreAtIndex(main_vec, (OBObjType *)singleton, 10);

  OBVectorSort(main_vec, OB_GREATEST_TO_LEAST);
  if(OBVectorGetLength(main_vec) != 7){
    fprintf(stderr, "OBVector_test: second sort did not properly remove NULLs "
                    "TEST FAILED\n");
    exit(1);
  }

  id = 10;
  for(i=0; i<OBVectorGetLength(main_vec); i++){
    if(id < OBTestGetID((OBTest *)OBVectorObjectAtIndex(main_vec, i))){
      fprintf(stderr, "OBVector_test: vector was not sorted greatest to "
                      "least, TEST FAILED\n");
      exit(1);
    }
    id = OBTestGetID((OBTest *)OBVectorObjectAtIndex(main_vec, i));
  }
  
  copy_vec = OBVectorCopy(main_vec);

  assert(OBCompare((OBObjType *)copy_vec, (OBObjType *)main_vec) == OB_EQUAL_TO);
  assert(OBHash((OBObjType *)copy_vec) == OBHash((OBObjType *)main_vec));

  OBRelease((OBObjType *)singleton); /* release singleton once so it is only referenced
                                by containing vectors */

  for(i=0; i<OBVectorGetLength(main_vec); i++){
    if(OBReferenceCount((OBObjType *)(OBTest *)OBVectorObjectAtIndex(main_vec, i)) != 2){
      fprintf(stderr, "OBVector_test: vector contents not retained on copy, "
                      "TEST FAILED\n");
      exit(1);
    }
  }
  
  /* get obj at index and retain it so it isnt deallocated */
  tmp = (OBTest *)OBVectorObjectAtIndex(main_vec, 3);
  OBRetain((OBObjType *)tmp);

  OBVectorStoreAtIndex(main_vec, NULL, 3);

  assert(OBCompare((OBObjType *)copy_vec, (OBObjType *)main_vec) == OB_NOT_EQUAL);
  assert(OBHash((OBObjType *)copy_vec) != OBHash((OBObjType *)main_vec));

  if(OBVectorContains(main_vec, (OBObjType *)tmp)){
    fprintf(stderr, "OBVector_test: Item was not removed properly from vector"
                    "index, TEST FAILED\n");
    exit(1);
  }

  OBVectorStoreAtIndex(main_vec, (OBObjType *)tmp, 3);
  assert(OBVectorGetLength(main_vec) == 7);
  OBVectorConcatenateVector(main_vec, copy_vec);
  assert(OBVectorGetLength(main_vec) == 14);

  for(i=0; i<OBVectorGetLength(main_vec)/2; i++){
    if(OBTestGetID((OBTest *)OBVectorObjectAtIndex(main_vec,i)) !=
       OBTestGetID((OBTest *)OBVectorObjectAtIndex(main_vec,i+7))){
      fprintf(stderr, "OBVector_test: Copy of vector not appended properly, "
                      "TEST FAILED\n");
      exit(1);
    }
  }

  OBRelease((OBObjType *)main_vec);
  OBRelease((OBObjType *)copy_vec);

  if(OBReferenceCount((OBObjType *)tmp) != 1){
    fprintf(stderr, "OBVector_test: releases of OBVector container did not "
                    "properly manage\ncontained OBTest reference count, TEST "
                    "FAILED\n");
    exit(1);
  }

  OBRelease((OBObjType *)tmp);

  printf("OBVector_test: TESTS PASSED\n");
  return 0;
}
