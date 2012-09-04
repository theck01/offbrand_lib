
#include "../../include/offbrand.h"
#include "../../include/OBVector.h"
#include "../../include/private/OBVector_Private.h" /* For testing purposes
                                                       only */
#include "../../include/OBTest.h"


int main(){

  uint32_t i, id;
  OBTest *tests[6], *singleton, *tmp;
  OBVector *main_vec, *copy_vec;

  main_vec = createVector(1);
  singleton = createTest(7);

  for(i=0; i<6; i++){
    tests[i] = createTest(i);
    addToVector(main_vec, (obj *)tests[i]);

    if(getTestReferences(tests[i]) != 2){
      fprintf(stderr, "OBVector_test: vector did not properly retain element "
                      "TEST FAILED\n");
      exit(1);
    }

    release((obj *)tests[i]); /* vector should maintain only references */
  }

  if(sizeOfVector(main_vec) != 6){
    fprintf(stderr, "OBVector_test: vector size is off, TEST FAILED\n");
    exit(1);
  }

  fitVectorToContents(main_vec);
  if(main_vec->capacity != 6){
    fprintf(stderr, "OBVector_test: vector did not fit size to contents, "
                    "TEST FAILED\n");
    exit(1);
  }
  
  for(i=0; i<6; i++){
    if(getTestID((OBTest *)objAtVectorIndex(main_vec,i)) != i){
      fprintf(stderr, "OBVector_test: vector does not contain added contents "
                      "in expected order\nTEST FAILED\n");
      exit(1);
    }
  }

  addToVector(main_vec, (obj *)singleton);

  removeFromVectorEnd(main_vec);
  if(sizeOfVector(main_vec) != 6){
    fprintf(stderr, "OBVector_test: remove did not properly get rid of element "
                    "TEST FAILED\n");
    exit(1);
  }

  if(getTestReferences(singleton) != 1){
    fprintf(stderr, "OBVector_test: vector did not release element on removal "
                    "TEST FAILED\n");
    exit(1);
  }

  tmp = (OBTest *)objAtVectorIndex(main_vec, 3);
  retain((obj *)tmp);

  replaceInVector(main_vec, (obj *)singleton, 3);
  if(getTestReferences(tmp) != 1){
    fprintf(stderr, "OBVector_test: vector did not release element on "
                    "replacement, TEST FAILED\n");
    exit(1);
  }

  insertAtVectorIndex(main_vec, (obj *)tmp, 4);

  if(sizeOfVector(main_vec) != 7 ||
     getTestID((OBTest *)objAtVectorIndex(main_vec, 4)) != getTestID(tmp)){
    fprintf(stderr, "OBVector_test: element was not added properly to vector "
                    "at index\nTEST FAILED\n");
    exit(1);
  }

  /* reduce reference count to 1, so only vector holds reference to obj */
  release((obj *)tmp);

  if(!findObjInVector(main_vec, (obj *)singleton, &compareTests)){
    fprintf(stderr, "OBVector_test: vector did not replace element correctly, "
                    "or was not found\nTEST FAILED\n");
    exit(1);
  }
  
  sortVector(main_vec, &compareTests, OB_LEAST_TO_GREATEST);

  id = 0;
  for(i=0; i<sizeOfVector(main_vec); i++){
    if(id > getTestID((OBTest *)objAtVectorIndex(main_vec, i))){
      fprintf(stderr, "OBVector_test: vector was not sorted least to "
                      "greatest, TEST FAILED\n");
      exit(1);
    }
    id = getTestID((OBTest *)objAtVectorIndex(main_vec, i));
  }

  sortVector(main_vec, &compareTests, OB_GREATEST_TO_LEAST);

  id = 10;
  for(i=0; i<sizeOfVector(main_vec); i++){
    if(id < getTestID((OBTest *)objAtVectorIndex(main_vec, i))){
      fprintf(stderr, "OBVector_test: vector was not sorted greatest to "
                      "least, TEST FAILED\n");
      exit(1);
    }
    id = getTestID((OBTest *)objAtVectorIndex(main_vec, i));
  }
  
  copy_vec = copyVector(main_vec);

  release((obj *)singleton); /* release singleton once so it is only referenced
                                by containing vectors */

  for(i=0; i<sizeOfVector(main_vec); i++){
    if(compareTests(objAtVectorIndex(main_vec, i),
                    objAtVectorIndex(copy_vec, i))){
      fprintf(stderr, "OBVector_test: vector contents not copied correctly, "
                      "TEST FAILED\n");
      exit(1);
    }
    
    if(getTestReferences((OBTest *)objAtVectorIndex(main_vec, i)) != 2){
      fprintf(stderr, "OBVector_test: vector contents not retained on copy, "
                      "TEST FAILED\n");
      exit(1);
    }
  }

  /* get obj at index and retain it so it isnt deallocated */
  tmp = (OBTest *)objAtVectorIndex(main_vec, 3);
  retain((obj *)tmp);

  removeFromVectorIndex(main_vec, 3);

  for(i=0; i<sizeOfVector(main_vec); i++){
    if(getTestID((OBTest *)objAtVectorIndex(main_vec,i)) == getTestID(tmp)){
      fprintf(stderr, "OBVector_test: Item was not removed properly from vector"
                      "index, TEST FAILED\n");
      exit(1);
    }
  }

  catVectors(main_vec, copy_vec);
  sortVector(main_vec, &compareTests, OB_GREATEST_TO_LEAST);

  for(i=0; i<sizeOfVector(main_vec)-1; i+=2){
    if(getTestID((OBTest *)objAtVectorIndex(main_vec,i)) !=
       getTestID((OBTest *)objAtVectorIndex(main_vec,i))){
      fprintf(stderr, "OBVector_test: Copy of vector not appended properly, "
                      "TEST FAILED\n");
      exit(1);
    }
  }

  printf("OBVector_test: TESTS PASSED\n");
  return 0;
}
