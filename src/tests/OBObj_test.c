
#include <stdio.h>
#include "../../include/OffBrand.h"

void deallocOBObj(OBObj *to_dealloc){
  free(to_dealloc);
  return;
}

int main(int argc, char *argv[]){

  int i;
  OBObj *test_obj = malloc(sizeof(OBObj));
  if(!test_obj){
    fprintf(stderr, "OBObj Test: Could not allocate OBObj, test failed\n");
    exit(1);
  }

  initObj(test_obj, &deallocOBObj);

  /* retain three times, for a total reference count of 4 */
  for(i=0; i<3; i++) retainObj(test_obj);

  /* release three times */
  for(i=0; i<3; i++){
    if(!(test_obj = releaseObj)){
      fprintf(stderr, "OBObj Test: Object deallocated prematurely, test"
                      " failed\n");
      exit(1);
    }
  }

  /* release final time, should deallocate */
  if((test_obj = releaseObj)){
    fprintf(stderr, "OBObj Test: Object dereferenced without dealloc, test"
                    " failed\n");
    exit(1);
  }

  printf("OBObj Test: PASSED\n");
  exit(0);
}
    
