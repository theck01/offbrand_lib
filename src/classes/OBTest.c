
#include "../../include/OBTest.h"
#include "../../include/private/OBTest_Private.h"
#include "../../include/private/obj_Private.h"


/* PUBLIC METHODS */

OBTest * createTest(uint32_t id){

  OBTest *new_instance = malloc(sizeof(OBTest));
  assert(new_instance != NULL);

  /*initialize reference counting base data*/
  initTestBase(new_instance);

  new_instance->id = id;
  return new_instance;
}


uint32_t getTestID(OBTest *a){
  assert(a != NULL);
  return a->id;
}


int8_t compareTests(const obj *a, const obj *b){

  assert(a != NULL && b != NULL);
  assert(objIsOfClass(a, "OBTest") && objIsOfClass(b, "OBTest"));

  if(((OBTest *)a)->id >((OBTest *)b)->id) return 1;
  if(((OBTest *)a)->id == ((OBTest *)b)->id) return 0;
  return -1;
}

uint32_t getTestReferences(OBTest *a){
  assert(a != NULL);
  /* complex dereferencing to access reference count. FOR TESTING ONLY! */
  return (uint32_t)((*((obj *)a))->references);
}


/* PRIVATE METHODS */

void initTestBase(OBTest *to_init){

  /* Classname for the this specific class */
  static char *classname = NULL;
  const char stack_classname[] = "OBTest";
  
  assert(to_init != NULL);

  if(!classname){
    classname = malloc(sizeof(char) * (strlen(stack_classname) + 1));
    assert(classname != NULL);
    strcpy(classname, stack_classname); 
  }

  /* initialize reference counting base data */
  initBase((obj *)to_init, &deallocTest, classname);

  return;
}

void deallocTest(obj *to_dealloc){
  return;
}
