
#ifndef OB_THREADED
#include "../include/offbrand.h"
#else
#include "../include/offbrand_threaded.h"
#endif

#include "../include/private/obj_Private.h"

uint8_t initBase(obj *instance, dealloc_fptr dealloc, const char *classname){

  *instance = malloc(sizeof(struct obj_struct));

  if(!(*instance)){
    return 1;
  }
  (*instance)->references = 1;
  (*instance)->dealloc = dealloc;
  (*instance)->classname = classname;

#ifdef OB_THREADED
  initLock(&((*instance)->lock));
#endif

  return 0;
}

obj * release(obj *instance){

  /* quitely do nothing if attempting to release NULL */
  if(!instance){
    return NULL;
  }

  /* if no other part of the program references the instance, destroy it */
  if(--((*instance)->references) <= 0){
#ifdef OB_THREADED
    deallocLock(&((*instance)->lock)); /* free mutex / conds in lock if
                                          threaded */
#endif
    free((struct obj_struct *)*instance); /* free reference counted base */ 
    (*instance)->dealloc(instance); /*class specific memory cleanup called*/
    return NULL;
  }

  return instance;
}

void retain(obj *instance){

  if(!instance){
    fprintf(stderr, "offbrand_stdlib: Cannot release NULL\n");
    return;
  }

  if((*instance)->references < UINT32_MAX) ++((*instance)->references);

  return;
}

uint8_t objIsOfClass(const obj *a, const char *classname){
  if(!a){
    fprintf(stderr, "offbrand_stdlib: NULL is not of any class\n");
    return 1;
  }

  if(strcmp((*a)->classname, classname) == 0) return 0;
  else return 1;
}


uint8_t sameClass(const obj *a, const obj *b){
  
  if(!a || !b){
    fprintf(stderr, "offbrand_stdlib: Cannot compare NULL(s) in sameClass\n");
    return 0;
  }

  return objIsOfClass(a, (*b)->classname);
}


int8_t objCompare(const obj *a, const obj *b){

  if(!a || !b){
    fprintf(stderr, "offbrand_stdlib: NULL argument(s) passed to "
                    "defaultCompare\n");
    return OB_COMPARE_ERR;
  }

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
