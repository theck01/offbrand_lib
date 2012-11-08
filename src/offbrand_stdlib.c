
#ifndef OB_THREADED
#include "../include/offbrand.h"
#else
#include "../include/offbrand_threaded.h"
#endif

#include "../include/private/obj_Private.h"

void initBase(obj *instance, dealloc_fptr dealloc, hash_fptr hash,
              const char *classname){

  assert(classname != NULL);

  *instance = malloc(sizeof(struct obj_struct));

  assert((*instance) != NULL);

  (*instance)->references = 1;
  (*instance)->dealloc = dealloc;
  (*instance)->hash = hash;
  (*instance)->classname = classname;

#ifdef OB_THREADED
  initLock(&((*instance)->lock));
#endif

  return;
}

obj * release(obj *instance){

  assert(instance != NULL);
  assert(*instance != NULL); /* assertion protects against release of iterator
                                types */

  /* if no other part of the program references the instance, destroy it */
  if(--((*instance)->references) <= 0){
#ifdef OB_THREADED
    deallocLock(&((*instance)->lock)); /* free mutex / conds in lock if
                                          threaded */
#endif
    (*instance)->dealloc(instance); /*class specific memory cleanup called*/
    free((struct obj_struct *)*instance); /* free reference counted base */ 
    free(instance); /* free the entire object */
    return NULL;
  }

  return instance;
}

void retain(obj *instance){

  assert(instance != NULL);

  if((*instance)->references < UINT32_MAX) ++((*instance)->references);

  return;
}


uint8_t objIsOfClass(const obj *a, const char *classname){
  assert(a != NULL && classname != NULL);
  if(strcmp((*a)->classname, classname) == 0) return 1;
  else return 0;
}


uint8_t sameClass(const obj *a, const obj *b){
  assert(a != NULL && b != NULL);
  return objIsOfClass(a, (*b)->classname);
}


obhash_t hash(const obj *to_hash){

  obhash_t value;
  /* if hash function is not specified, or is the hash function in the stdlib
   * is specified then use the default hashing equation */
  if(!(*to_hash)->hash || (*to_hash)->hash == &hash){
    /* scramble the to_hash address to generate hash value */
    value = (obhash_t)to_hash ^ ((obhash_t)to_hash)<<32;
    value = (value << 6) ^ (value >> 10);
    value = (value >> 7) ^ (value << 12);
    return value;
  }
  else{
    return (*to_hash)->hash(to_hash);
  }
}

int8_t objCompare(const obj *a, const obj *b){

  assert(a != NULL && b != NULL);

  if(a > b){
    return 1;
  }
  else if(a == b){
    return 0;
  }
  else{
    return -1;
  }
}
