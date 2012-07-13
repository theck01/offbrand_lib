
#include "../include/OBTest.h"
#include "../include/private/OBTest_Private.h"


/* PUBLIC METHODS */

OBTest * createTest(uint32_t id){

  OBTest *new_instance = malloc(sizeof(OBTest));
  if(!new_instance){
    fprintf(stderr, "OBTest: Could not allocate memory for "
                    "new_instance\n");
    return NULL;
  }

  /*initialize reference counting base data*/
  initBase((obj *)new_instance, &deallocOBTest);

  new_instance->id = id;
  return new_instance;
}


uint32_t getTestID(OBTest *a){
  if(!a){
    fprintf(stderr, "OBTest: Unexpected NULL value(s) passed to "
                    "getTestID\n");
    return UINT32_MAX;
  }

  return a->id;
}


int8_t compareTests(OBTest *a, OBTest *b){

  if(!a || !b){
    fprintf(stderr, "OBTest: Unexpected NULL value(s) passed to "
                    "compareTests\n");
    return OB_COMPARE_ERR;
  }

  if(a->id > b->id) return 1;
  if(a->id == b->id) return 0;
  return -1;
}

int getTestReferences(OBTest *a){

  if(!a){
    fprintf(stderr, "OBTest: Unexpected NULL arguments passed to "
                    "getTestReferences\n");
    return -1;
  }

  return (int)(*a)->references;
}


/* PRIVATE METHODS */

void deallocOBTest(obj *to_dealloc){
  return;
}
