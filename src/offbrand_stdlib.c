
#include "../include/offbrand.h"
#include "../include/private/obj_Private.h"

void initBase(obj *instance, dealloc_fptr dealloc, hash_fptr hash,
              compare_fptr compare, const char *classname){

  assert(classname != NULL);

  *instance = malloc(sizeof(struct obj_struct));

  assert((*instance) != NULL);

  (*instance)->references = 1;
  (*instance)->dealloc = dealloc;
  (*instance)->hash = hash;
  (*instance)->compare = compare;
  (*instance)->classname = classname;

  return;
}

obj * release(obj *instance){

  assert(instance != NULL);
  assert(*instance != NULL); /* assertion protects against double dealloc */

  /* if no other part of the program references the instance, destroy it */
  if(--((*instance)->references) <= 0){
    (*instance)->dealloc(instance); /*class specific memory cleanup called*/
    free((struct obj_struct *)*instance); /* free reference counted base */ 
    (*instance) = NULL; /* mark instance as deallocated */
    free(instance); /* free the entire object */
    return NULL;
  }

  return instance;
}

void retain(obj *instance){

  assert(instance != NULL);
  assert(*instance != NULL); /* assertion protects against double dealloc */

  if((*instance)->references < UINT32_MAX) ++((*instance)->references);

  return;
}

uint32_t referenceCount(obj *instance){
  assert(instance != NULL);
  assert(*instance != NULL); /* assertion protects against double dealloc */
  return (*instance)->references;
}



uint8_t objIsOfClass(const obj *a, const char *classname){
  assert(a != NULL && classname != NULL);
  assert(*a != NULL); /* assertion protects against double 
                                       dealloc */
  if(strcmp((*a)->classname, classname) == 0) return 1;
  else return 0;
}


uint8_t sameClass(const obj *a, const obj *b){
  assert(a != NULL && b != NULL);
  assert(*a != NULL && *b != NULL); /* assertion protects against double 
                                       dealloc */
  return objIsOfClass(a, (*b)->classname);
}


obhash_t hash(const obj *to_hash){

  obhash_t retval;

#if __STD_C_VERSION__ >= 201112L
  static _Thread_local int call_count = 0; /* number of calls to the compare
                                              function before a return */
#else
  static int call_count = 0; /* number of calls to the compare
                                function before a return */
#endif

  call_count++;

  assert(to_hash);
  assert(*to_hash); /* assertion protects against double dealloc */

  if((*to_hash)->hash && call_count == 1) retval = (*to_hash)->hash(to_hash);
  else{ 
    retval = (obhash_t)to_hash ^ ((obhash_t)to_hash)<<15;
    retval = (retval << 6) ^ (retval >> 10);
    retval = (retval >> 7) ^ (retval << 12);
  }

  call_count--;

  return retval;
}

int8_t compare(const obj *a, const obj *b){

  int8_t retval;

#if __STD_C_VERSION__ >= 201112L
  static _Thread_local int call_count = 0; /* number of calls to the compare
                                              function before a return */
#else
  static int call_count = 0; /* number of calls to the compare
                                function before a return */
#endif

  call_count++;

  assert(a != NULL && b != NULL);
  assert(*a != NULL && *b != NULL); /* assertion protects against double 
                                       dealloc */

  if(sameClass(a, b) && call_count == 1 && (*a)->compare != NULL) 
    retval = (*a)->compare(a, b);
  else if(a == b) retval = OB_EQUAL_TO;
  else retval = OB_NOT_EQUAL;

  call_count--;
  
  return retval;
}
