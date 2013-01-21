
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

  if(!instance) return NULL;

  /* if no other part of the program references the instance, destroy it */
  if(--((*instance)->references) <= 0){

    /* call class specific memory cleanup, if it exists */
    if((*instance)->dealloc)
      (*instance)->dealloc(instance); 

    free((struct obj_struct *)*instance); /* free reference counted base */ 
    free(instance); /* free the entire object */

    return NULL;
  }

  return instance;
}

void retain(obj *instance){

  if(!instance) return;

  assert((*instance)->references < UINT32_MAX); /* reference count > UINT32_MAX
                                                   cannot be handled by lib */
  ++((*instance)->references);

  return;
}

uint32_t referenceCount(obj *instance){
  if(!instance) return 0;
  return (*instance)->references;
}



uint8_t objIsOfClass(const obj *a, const char *classname){

  if(!a){
    if(strcmp(classname, "NULL") == 0) return 1;
    return 0;
  }
  if(strcmp((*a)->classname, classname) == 0) return 1;
  else return 0;
}


uint8_t sameClass(const obj *a, const obj *b){
  /* if both NULL, both are of the "NULL" class */
  if(!a && !b) return 1;
  else if(!a || !b) return 0;
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

  if(!to_hash) return 0;

  call_count++;

  if((*to_hash)->hash && call_count == 1) retval = (*to_hash)->hash(to_hash);
  else{ 
    retval = (obhash_t)to_hash;
    retval += (retval << 6);
    retval ^= (retval >> 10);
    retval += (retval >> 7);
    retval ^= (retval << 12);
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

  if(a == NULL && b == NULL) return OB_EQUAL_TO;
  else if(a == NULL || b == NULL) return OB_NOT_EQUAL;

  call_count++;

  if(sameClass(a, b) && call_count == 1 && (*a)->compare != NULL) 
    retval = (*a)->compare(a, b);
  else if(a == b) retval = OB_EQUAL_TO;
  else retval = OB_NOT_EQUAL;

  call_count--;
  
  return retval;
}
