/**
 * @file obtest_test.c
 * @brief obtest Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/obtest.h"

/**
 * @brief Main unit testing routine
 */
int main(){

  int i;
  obtest *test_obj, *a, *b;
  test_obj = obtest_new(1);
  a = obtest_new(3);
  b = obtest_new(3);

  /* ob_retain object, reference count should be 4 */
  for(i=0; i<3; i++){
    ob_retain((obj *)test_obj);
  }

  if(ob_reference_count((obj *)test_obj) != 4){
    fprintf(stderr, "obtest_test: reference count not incrememted correctly by "
                    "ob_retain\nTEST FAILED\n");
    exit(1);
  }

  /* ob_release object until test_obj is deallocated or until ob_release has been
   * called more than 4 times */
  i=0;
  while(++i < 5 && ob_release((obj *)test_obj) != NULL){}

  if(i<4){
    fprintf(stderr, "obtest_test: test_obj ob_released before reference count "
                    "reached zero, TEST FAILED\n");
    exit(1);
	}
  else if(i>4){
    fprintf(stderr, "obtest_test: test_obj not ob_released when reference count "
                    "reached zero\n");
    exit(1);
	}

  if(!ob_has_same_class((obj *)a, (obj *)b)){
    fprintf(stderr, "obtest_test: Two obtest objects were not of the same "
                    "class, TEST FAILED\n");
    exit(1);
  }

  if(!ob_has_class((obj *)a, "obtest")){
    fprintf(stderr, "obtest_test: An obtest object was not considered to be an "
                    "obtest object during check, TEST FAILED\n");
    exit(1);
  }

  ob_hash((obj *)a);

  printf("obtest: TEST PASSED\n");
  return 0;
}
