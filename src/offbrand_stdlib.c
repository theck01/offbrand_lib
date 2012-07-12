
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
  return 0;
}

obj * release(obj *instance){

  dealloc_fptr destructor;

  /* if no other part of the program references the instance, destroy it */
  if(--((*instance)->references) <= 0){
    destructor = (*instance)->dealloc; /*save pointer to class deallocator */
    free(*instance); /*free reference counted portion of object*/
    destructor(instance); /*class specific memory cleanup called*/
    return NULL;
  }

  return instance;
}

void retain(obj *instance){
  ++((*instance)->references);
  return;
}

uint8_t sameClass(const obj *a, const obj *b){
  if((*a)->dealloc == (*b)->dealloc){
    return 1;
  }

  return 0;
}
uint8_t defaultCompare(obj *a, obj *b){

  if(!a || !b){
    fprintf(stderr, "offbrand_stdlib: NULL argument(s) passed to "
                    "defaultCompare\n");
    return OB_COMPARE_ERR;
  }

  if(a == b){
    return 0;
  }

  return 1;
}
