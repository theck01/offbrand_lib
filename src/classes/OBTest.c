
#include "../../include/OBTest.h"
#include "../../include/private/OBTest_Private.h"
#include "../../include/private/obj_Private.h"


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


int8_t compareTests(const obj *a, const obj *b){

  if(!a || !b){
    fprintf(stderr, "OBTest: Unexpected NULL value(s) passed to "
                    "compareTests\n");
    return OB_COMPARE_ERR;
  }

  if(((OBTest *)a)->id >((OBTest *)b)->id) return 1;
  if(((OBTest *)a)->id == ((OBTest *)b)->id) return 0;
  return -1;
}

uint32_t getTestReferences(OBTest *a){

  if(!a){
    fprintf(stderr, "OBTest: Unexpected NULL arguments passed to "
                    "getTestReferences\n");
    return -1;
  }

  /* complex dereferencing to access reference count. FOR TESTING ONLY! */
  return (uint32_t)((*((obj *)a))->references);
}


/* PRIVATE METHODS */

void deallocOBTest(obj *to_dealloc){
  to_dealloc++; /* does nothing except suppress compiler warning */
  return;
}
