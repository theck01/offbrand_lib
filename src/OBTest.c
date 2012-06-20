
#include "../include/OBTest.h"
#include "../include/private/OBTest_Private.h"

OBTest * createOBTest(void){

  OBTest *new_instance = malloc(sizeof(OBTest));
  if(!new_instance){
    fprintf(stderr, "OBTest: Could not allocate memory for "
                    "new_instance\n");
    return NULL;
  }

  /*initialize reference counting base data*/
  initBase((obj *)new_instance, &deallocOBTest);

  return new_instance;
}

void deallocOBTest(obj *to_dealloc){

  /*cast generic obj to OBTest class*/
  OBTest *instance = (OBTest *)to_dealloc;
  free(instance);
  return;
}
