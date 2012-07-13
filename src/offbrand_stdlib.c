
#include "../include/offbrand.h"
#include "../include/private/obj_private.h"

/* NOTE: init OBLock if OB_THREADED defined in method */
uint8_t initBase(obj *instance, dealloc_fptr dealloc){
  *instance = malloc(sizeof(struct obj_struct));
  if(!(*instance)){
    return 1;
  }
  (*instance)->references = 1;
  (*instance)->dealloc = dealloc;

#ifdef OB_THREADED
  initLock(&((*instance)->lock));
#endif

  return 0;
}

obj * release(obj *instance){

  if(!instance){
    fprintf(stderr, "release: Cannot release NULL\n");
    return NULL;
  }

  writeLock(instance);

  /* if no other part of the program references the instance, destroy it */
  if(--((*instance)->references) <= 0){
    (*instance)->dealloc(instance); /*class specific memory cleanup called*/

#ifdef OB_THREADED
    deallocLock(&((*instance)->lock)); /* free mutex / conds in lock if
                                          threaded */
#endif

    free(*instance); /*free reference counted portion of object*/
    destructor(instance); /*class specific memory cleanup called*/
    return NULL;
  }

  writeUnlock(instance);

  return instance;
}

void retain(obj *instance){

  if(!instance){
    fprintf(stderr, "offbrand_stdlib: Cannot release NULL\n");
    return NULL;
  }

  writeLock(instance);
  if((*instance)->references < UINT32_MAX) ++((*instance)->references);
  writeUnlock(instance);

  return;
}

uint8_t sameClass(const obj *a, const obj *b){
  
  uint8_t retval = 0;

  if(!a || !b){
    fprintf(stderr, "offbrand_stdlib: Cannot compare NULL(s) in sameClass\n");
    return 0;
  }

  readLock(a);
  readLock(b);

  if((*a)->dealloc == (*b)->dealloc){
    retval = 1;
  }

  readUnlock(a);
  readUnlock(b);

  return retval;
}


uint8_t defaultCompare(const obj *a, const obj *b){

  uint8_t retval = 0;

  if(!a || !b){
    fprintf(stderr, "offbrand_stdlib: NULL argument(s) passed to "
                    "defaultCompare\n");
    return OB_COMPARE_ERR;
  }

  readLock(a);
  readLock(b);

  if(a != b){
    retval = 1;
  }

  readUnlock(a);
  readUnlock(b);

  return retval;
}
