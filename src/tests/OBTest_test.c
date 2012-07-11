
#include <stdio.h>
#include "../../include/offbrand.h"
#include "../../include/OBTest.h"

int main(){

  int i;
  OBTest *test_obj = createOBTest();
  if(!test_obj){
    fprintf(stderr, "OBTest_test: could not allocate memory for test, TEST "
                    "FAILD\n");
    exit(1);
  }

  /* retain object, reference count should be 4 */
  for(i=0; i<3; i++){
    retain((obj *)test_obj);
  }

  /* release object until test_obj is deallocated or until release has been
   * called more than 4 times */
  i=0;
  while(++i < 5 && release((obj *)test_obj) != NULL){}

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
  else{
    printf("OBTest_test: TEST PASSED\n");
  }

  return 0;
}
