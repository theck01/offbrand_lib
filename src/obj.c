
#include "../include/obj.h"
#include "../include/private/obj_private.h"

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
