
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
  if(initTestBase(new_instance)){
    fprintf(stderr, "OBTest: Could not initialize base obj\n");
    return NULL;
  }

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

uint8_t initTestBase(OBTest *to_init){

  /* Classname for the this specific class */
  static char *classname = NULL;
  const char stack_classname[] = "OBTest";

  if(!classname){
    classname = malloc(sizeof(char) * strlen(stack_classname) + 1);
    if(!classname){
      fprintf(stderr, "OBTest: Could not allocate static classname "
                      "for all instances\nClass checking functions will not "
                      "work until classname allocated\n");
      return 1;
    }
    strcpy(classname, stack_classname); 
  }

  /* initialize reference counting base data */
  if(initBase((obj *)to_init, &deallocTest, classname)){
    fprintf(stderr, "OBTest: Could not initialize base obj\n");
    return 1;
  }

  return 0;
}

void deallocTest(obj *to_dealloc){
  free((OBTest *)to_dealloc);
  return;
}
