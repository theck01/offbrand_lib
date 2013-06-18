/**
 * @file OBTest.c
 * @brief OBTest Method Implementation
 * @author theck
 */

#include "../../include/OBTest.h"
#include "../../include/private/OBTest_Private.h"

/* PUBLIC METHODS */

OBTest * OBTestCreate(uint32_t id){

  static const char classname[] = "OBTest";

  OBTest *new_instance = malloc(sizeof(OBTest));
  assert(new_instance != NULL);

  /*initialize reference counting base data*/
  OBInitBase((OBObjType *)new_instance, &OBTestDealloc, &OBTestHash, &OBTestCompare,
           &OBTestDisplay, classname);

  new_instance->id = id;
  return new_instance;
}


uint32_t OBTestGetID(OBTest *a){
  assert(a != NULL);
  return a->id;
}

/* PRIVATE METHODS */

obhash_t OBTestHash(OBTypeRef to_hash){
  assert(to_hash);
  return (obhash_t)(((OBTest *)to_hash)->id);
}

int8_t OBTestCompare(OBTypeRef a, OBTypeRef b){

  assert(a != NULL);
  assert(b != NULL);
  assert(OBObjIsOfClass(a, "OBTest"));
  assert(OBObjIsOfClass(b, "OBTest"));

  if(((OBTest *)a)->id >((OBTest *)b)->id) return OB_GREATER_THAN;
  if(((OBTest *)a)->id == ((OBTest *)b)->id) return OB_EQUAL_TO;
  return OB_LESS_THAN;
}

void OBTestDisplay(OBTypeRef test){
  assert(test != NULL);
  assert(OBObjIsOfClass(test, "OBTest"));
  fprintf(stderr, "Test ID: %u\n", ((OBTest *)test)->id);
}

void OBTestDealloc(OBTypeRef to_dealloc){
  assert(to_dealloc != NULL);
  return;
}
