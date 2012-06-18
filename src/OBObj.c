
#include "../include/OBObj.h"
#include "../include/private/OBObj_Private.h"

void initObj(OBObj *o, destroy_fptr dealloc){
  o->references = 1;
  o->dealloc = dealloc;
  return;
}

OBObj * releaseObj(OBObj *o){

  /* if no other part of the program references the instance, destroy it */
  if(--(o->references) <= 0){
    o->dealloc(o); /*calls designated destructor on current OBObj instance*/
    return NULL;
  }

  return o;
}

void retainObj(OBObj *o){
  ++(o->references);
  return;
}
