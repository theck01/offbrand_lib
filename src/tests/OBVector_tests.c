
#include "../../include/offbrand.h"
#include "../../include/OBVector.h"
#include "../../include/private/OBVector_private.h" /* For testing purposes
                                                       only */
#include "../../include/OBTest.h"


int main(){

  int i, id;
  OBTest *tests[6], *singleton, *tmp;
  OBVector *main_vec, *copy_vec;

  main_vec = createVector(1);
  if(!main_vec){
    fprintf(stderr, "OBVector_test: could not allocated memory for OBVector, "
                    "TEST FAILED\n");
    exit(1);
  }

  if(!(singleton = createTest(7))){
    fprintf(stderr, "OBVector_test: could not allocated memory for OBTest, "
                    "TEST FAILED\n");
    exit(1);
  }

  for(i=0; i<6; i++){
    tests[i] = createTest(i);
    if(!tests[i]){
      fprintf(stderr, "OBVector_test: could not allocated memory for OBTest, "
                      "TEST FAILED\n");
      exit(1);
    }

    if(addToVector(main_vec, tests[i])){
      fprintf(stderr, "OBVector_test: could add elements to vector, "
                      "TEST FAILED\n");
      exit(1);
    }

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

  if(fitVectorToContents(main_vec) || main_vec->capacity != 6){
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

  if(addToVector(main_vec, singleton)){
    fprintf(stderr, "OBVector_test: could add singleton to vector, "
                    "TEST FAILED\n");
    exit(1);
  }

  removeFromVector(main_vec);
  if(sizeOfVector(main_vec != 6)){
    fprintf(stderr, "OBVector_test: remove did not properly get rid of element "
                    "TEST FAILED\n");
    exit(1);
  }

  if(getTestReferences(singleton) != 1){
    fprintf(stderr, "OBVector_test: vector did not release element on removal "
                    "TEST FAILED\n");
    exit(1);
  }

  if(!(tmp = (OBTest *)objAtVectorIndex(main_vec, 3))){
    fprintf(stderr, "OBVector_test: could not access obj in vector properly "
                    "TEST FAILED\n");
    exit(1);
  }

  retain((obj *)tmp);
  replaceInVector(main_vec, singleton, 3);
  if(getTestReferences(tmp) != 1){
    fprintf(stderr, "OBVector_test: vector did not release element on "
                    "replacement, TEST FAILED\n");
    exit(1);
  }

  if(!findObjInVector(main_vec, singleton, &compareTests)){
    fprintf(stderr, "OBVector_test: vector did not replace element correctly, "
                    "or was not found\nTEST FAILED\n");
    exit(1);
  }
  
  if(sortVector(main_vec, &compareTests, OB_LEAST_TO_GREATEST)){
    fprintf(stderr, "OBVector_test: vector could not be sorted, TEST FAILED\n");
    exit(1);
  }

  id = 0;
  for(i=0; i<sizeOfVector(main_vec); i++){
    if(id > getTestID((OBTest *)objAtVectorIndex(main_vec, i))){
      fprintf(stderr, "OBVector_test: vector was not sorted least to "
                      "greatest, TEST FAILED\n");
      exit(1);
    }
    id = getTestID((OBTest *)objAtVectorIndex(main_vec, i));
  }

  if(sortVector(main_vec, &compareTests, OB_GREATEST_TO_LEAST)){
    fprintf(stderr, "OBVector_test: vector could not be sorted, TEST FAILED\n");
    exit(1);
  }

  id = 10;
  for(i=0; i<sizeOfVector(main_vec); i++){
    if(id < getTestID((OBTest *)objAtVectorIndex(main_vec, i))){
      fprintf(stderr, "OBVector_test: vector was not sorted greatest to "
                      "least, TEST FAILED\n");
      exit(1);
    }
    id = getTestID((OBTest *)objAtVectorIndex(main_vec, i));
  }
  
  if(!(copy_vec = copyVector(main_vec))){
    fprintf(stderr, "OBVector_test: vector could not be copied, TEST FAILED\n");
    exit(1);
  }

  release((obj *)singleton); /* release singleton once so it is only referenced
                                by containing vectors */
  for(i=0; i<sizeOfVector(main_vec); i++){
    if(compareTests((OBTest *)objAtVectorIndex(main_vec, i),
                    (OBTest *)objAtVectorIndex(copy_vec, i))){
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

  printf("OBVector_test: TESTS PASSED\n");
  return 0;
}
