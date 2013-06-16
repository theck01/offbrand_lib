/**
 * @file OBTest_test.c
 * @brief OBTest Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/OBTest.h"

/**
 * @brief Main unit testing routine
 */
int main(){

  int i;
  OBTest *test_obj, *a, *b;
  test_obj = createTest(1);
  a = createTest(3);
  b = createTest(3);

  /* retain object, reference count should be 4 */
  for(i=0; i<3; i++){
    OBRetain((OBObjType *)test_obj);
  }

  if(OBReferenceCount((OBObjType *)test_obj) != 4){
    fprintf(stderr, "OBTest_test: reference count not incrememted correctly by "
                    "retain\nTEST FAILED\n");
    exit(1);
  }

  /* release object until test_obj is deallocated or until release has been
   * called more than 4 times */
  i=0;
  while(++i < 5 && OBRelease((OBObjType *)test_obj) != NULL){}

  if(i<4){
    fprintf(stderr, "OBTest_test: test_obj released before reference count "
                    "reached zero, TEST FAILED\n");
    exit(1);
	}
  else if(i>4){
    fprintf(stderr, "OBTest_test: test_obj not released when reference count "
                    "reached zero\n");
    exit(1);
	}

  if(!sameClass((OBObjType *)a, (OBObjType *)b)){
    fprintf(stderr, "OBTest_test: Two OBTest objects were not of the same "
                    "class, TEST FAILED\n");
    exit(1);
  }

  if(!objIsOfClass((OBObjType *)a, "OBTest")){
    fprintf(stderr, "OBTest_test: An OBTest object was not considered to be an "
                    "OBTest object during check, TEST FAILED\n");
    exit(1);
  }

  hash((OBObjType *)a);

  printf("OBTest_test: TEST PASSED\n");
  return 0;
}
