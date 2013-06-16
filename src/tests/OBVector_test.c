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

  main_vec = createVector(1);
  singleton = createTest(7);

  for(i=0; i<6; i++){
    tests[i] = createTest(i);
    storeAtVectorIndex(main_vec, (OBObjType *)tests[i], i);

    if(referenceCount((OBObjType *)tests[i]) != 2){
      fprintf(stderr, "OBVector_test: vector did not properly retain element "
                      "TEST FAILED\n");
      exit(1);
    }

    release((OBObjType *)tests[i]); /* vector should maintain only references */
  }

  if(vectorLength(main_vec) != 6){
    fprintf(stderr, "OBVector_test: vector size is off, TEST FAILED\n");
    exit(1);
  }

  for(i=0; i<6; i++){
    if(getTestID((OBTest *)objAtVectorIndex(main_vec,i)) != i){
      fprintf(stderr, "OBVector_test: vector does not contain added contents "
                      "in expected order\nTEST FAILED\n");
      printf("%d\n", i);
      exit(1);
    }
  }

  storeAtVectorIndex(main_vec, (OBObjType *)singleton, 12);
  if(vectorLength(main_vec) != 13){
    fprintf(stderr, "OBVector_test: store did not properly extend length "
                    "TEST FAILED\n");
    exit(1);
  }


  storeAtVectorIndex(main_vec, NULL, 12);
  if(vectorLength(main_vec) != 6){
    fprintf(stderr, "OBVector_test: remove did not properly get rid of element "
                    "TEST FAILED\n");
    exit(1);
  }

  if(referenceCount((OBObjType *)singleton) != 1){
    fprintf(stderr, "OBVector_test: vector did not release element on removal "
                    "TEST FAILED\n");
    exit(1);
  }

  tmp = (OBTest *)objAtVectorIndex(main_vec, 3);
  retain((OBObjType *)tmp);

  storeAtVectorIndex(main_vec, (OBObjType *)singleton, 3);
  if(referenceCount((OBObjType *)tmp) != 1){
    fprintf(stderr, "OBVector_test: vector did not release element on "
                    "replacement, TEST FAILED\n");
    exit(1);
  }

  if(vectorLength(main_vec) != 6 ||
     getTestID((OBTest *)objAtVectorIndex(main_vec, 3)) != 
     getTestID(singleton)){
    fprintf(stderr, "OBVector_test: element was not added properly to vector "
                    "at index\nTEST FAILED\n");
    exit(1);
  }

  storeAtVectorIndex(main_vec, (OBObjType *)tmp, 3);

  /* reduce reference count to 1, so only vector holds reference to obj */
  release((OBObjType *)tmp);

  if(findObjInVector(main_vec, (OBObjType *)singleton)){
    fprintf(stderr, "OBVector_test: vector did not replace element correctly, "
                    "or was not found\nTEST FAILED\n");
    exit(1);
  }
  
  storeAtVectorIndex(main_vec, (OBObjType *)singleton, 10);
  if(vectorLength(main_vec) != 11){
    fprintf(stderr, "OBVector_test: store did not properly extend length "
                    "TEST FAILED\n");
    exit(1);
  }

  sortVector(main_vec, OB_LEAST_TO_GREATEST);
  if(vectorLength(main_vec) != 7){
    fprintf(stderr, "OBVector_test: sort did not properly remove NULLs "
                    "TEST FAILED\n");
    fprintf(stderr, "OBVector_test: vector size == %d\n", 
                    vectorLength(main_vec));
    for(i=0; i<vectorLength(main_vec); i++){
      if(objAtVectorIndex(main_vec, i)) printf("object\n");
      else printf("NULL\n");
    }
    exit(1);
  }

  id = 0;
  for(i=0; i<vectorLength(main_vec); i++){
    if(id > getTestID((OBTest *)objAtVectorIndex(main_vec, i))){
      fprintf(stderr, "OBVector_test: vector was not sorted least to "
                      "greatest, TEST FAILED\n");
      exit(1);
    }
    id = getTestID((OBTest *)objAtVectorIndex(main_vec, i));
  }

  storeAtVectorIndex(main_vec, NULL, 6);
  storeAtVectorIndex(main_vec, (OBObjType *)singleton, 10);

  sortVector(main_vec, OB_GREATEST_TO_LEAST);
  if(vectorLength(main_vec) != 7){
    fprintf(stderr, "OBVector_test: second sort did not properly remove NULLs "
                    "TEST FAILED\n");
    exit(1);
  }

  id = 10;
  for(i=0; i<vectorLength(main_vec); i++){
    if(id < getTestID((OBTest *)objAtVectorIndex(main_vec, i))){
      fprintf(stderr, "OBVector_test: vector was not sorted greatest to "
                      "least, TEST FAILED\n");
      exit(1);
    }
    id = getTestID((OBTest *)objAtVectorIndex(main_vec, i));
  }
  
  copy_vec = copyVector(main_vec);

  assert(compare((OBObjType *)copy_vec, (OBObjType *)main_vec) == OB_EQUAL_TO);
  assert(hash((OBObjType *)copy_vec) == hash((OBObjType *)main_vec));

  release((OBObjType *)singleton); /* release singleton once so it is only referenced
                                by containing vectors */

  for(i=0; i<vectorLength(main_vec); i++){
    if(referenceCount((OBObjType *)(OBTest *)objAtVectorIndex(main_vec, i)) != 2){
      fprintf(stderr, "OBVector_test: vector contents not retained on copy, "
                      "TEST FAILED\n");
      exit(1);
    }
  }
  
  /* get obj at index and retain it so it isnt deallocated */
  tmp = (OBTest *)objAtVectorIndex(main_vec, 3);
  retain((OBObjType *)tmp);

  storeAtVectorIndex(main_vec, NULL, 3);

  assert(compare((OBObjType *)copy_vec, (OBObjType *)main_vec) == OB_NOT_EQUAL);
  assert(hash((OBObjType *)copy_vec) != hash((OBObjType *)main_vec));

  if(findObjInVector(main_vec, (OBObjType *)tmp)){
    fprintf(stderr, "OBVector_test: Item was not removed properly from vector"
                    "index, TEST FAILED\n");
    exit(1);
  }

  storeAtVectorIndex(main_vec, (OBObjType *)tmp, 3);
  assert(vectorLength(main_vec) == 7);
  catVectors(main_vec, copy_vec);
  assert(vectorLength(main_vec) == 14);

  for(i=0; i<vectorLength(main_vec)/2; i++){
    if(getTestID((OBTest *)objAtVectorIndex(main_vec,i)) !=
       getTestID((OBTest *)objAtVectorIndex(main_vec,i+7))){
      fprintf(stderr, "OBVector_test: Copy of vector not appended properly, "
                      "TEST FAILED\n");
      exit(1);
    }
  }

  release((OBObjType *)main_vec);
  release((OBObjType *)copy_vec);

  if(referenceCount((OBObjType *)tmp) != 1){
    fprintf(stderr, "OBVector_test: releases of OBVector container did not "
                    "properly manage\ncontained OBTest reference count, TEST "
                    "FAILED\n");
    exit(1);
  }

  release((OBObjType *)tmp);

  printf("OBVector_test: TESTS PASSED\n");
  return 0;
}
