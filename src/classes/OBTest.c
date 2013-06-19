/**
 * @file obtest.c
 * @brief obtest Method Implementation
 * @author theck
 */

#include "../../include/obtest.h"
#include "../../include/private/obtest_private.h"

/* PUBLIC METHODS */

obtest * obtest_new(uint32_t id){

  static const char classname[] = "obtest";

  obtest *new_instance = malloc(sizeof(obtest));
  assert(new_instance != NULL);

  /*initialize reference counting base data*/
  ob_init_base((obj *)new_instance, &obtest_destroy, &obtest_hash,
               &obtest_compare, &obtest_display, classname);

  new_instance->id = id;
  return new_instance;
}


uint32_t obtest_id(obtest *a){
  assert(a != NULL);
  return a->id;
}

/* PRIVATE METHODS */

ob_hash_t obtest_hash(const obj *to_hash){
  assert(to_hash);
  return (ob_hash_t)(((obtest *)to_hash)->id);
}

int8_t obtest_compare(const obj *a, const obj *b){

  assert(a != NULL);
  assert(b != NULL);
  assert(ob_has_class(a, "obtest"));
  assert(ob_has_class(b, "obtest"));

  if(((obtest *)a)->id >((obtest *)b)->id) return OB_GREATER_THAN;
  if(((obtest *)a)->id == ((obtest *)b)->id) return OB_EQUAL_TO;
  return OB_LESS_THAN;
}

void obtest_display(const obj *test){
  assert(test != NULL);
  assert(ob_has_class(test, "obtest"));
  fprintf(stderr, "Test ID: %u\n", ((obtest *)test)->id);
}

void obtest_destroy(obj *to_dealloc){
  assert(to_dealloc != NULL);
  return;
}
