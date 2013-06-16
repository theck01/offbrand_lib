/**
 * @file offbrand_stdlib.c
 * @brief Standard Library Implementation
 * @author theck
 */

#include "../include/offbrand.h"
#include "../include/private/obj_Private.h"

void OBInitBase(OBTypeRef tr_instance, obdealloc_fptr dealloc_funct, obhash_fptr hash_funct,
              obcompare_fptr compare_funct, obdisplay_fptr display_funct,
              const char *classname){
  OBObjType *instance = (OBObjType *)tr_instance;
  assert(classname != NULL);

  *instance = malloc(sizeof(struct OBObjStruct));

  assert((*instance) != NULL);

  (*instance)->references = 1;

  (*instance)->dealloc = dealloc_funct;

  if(hash_funct != &hash) (*instance)->hash = hash_funct;
  else (*instance)->hash = NULL;

  if(compare_funct != &compare) (*instance)->compare = compare_funct;
  else (*instance)->compare = NULL;

  if(display_funct != &display) (*instance)->display = display_funct;
  else (*instance)->display = NULL;

  (*instance)->classname = classname;

  return;
}


OBTypeRef OBRelease(OBTypeRef instance){

  if(!instance) return NULL;
  OBObjType *localInstance = (OBObjType *)instance;
  /* if no other part of the program references the instance, destroy it */
  if(--((*localInstance)->references) <= 0){

    /* call class specific memory cleanup, if it exists */
    if((*localInstance)->dealloc)
      (*localInstance)->dealloc(localInstance);

    free((struct OBObjStruct *)*localInstance); /* free reference counted base */
    free(localInstance); /* free the entire object */

    return NULL;
  }

  return instance;
}


OBTypeRef OBRetain(OBTypeRef instance){

  if(!instance) return NULL;

  OBObjType *localInstance = (OBObjType *)instance;
  assert((*localInstance)->references < UINT32_MAX); /* reference count > UINT32_MAX
                                                   cannot be handled by lib */
  ++((*localInstance)->references);

  return instance;
}


obref_count_t OBReferenceCount(OBObjType *instance){
  if(!instance) return 0;
  return (*instance)->references;
}


uint8_t objIsOfClass(const OBObjType *a, const char *classname){

  if(!a){
    if(strcmp(classname, "NULL") == 0) return 1;
    return 0;
  }
  if(strcmp((*a)->classname, classname) == 0) return 1;
  else return 0;
}


uint8_t sameClass(const OBObjType *a, const OBObjType *b){
  /* if both NULL, both are of the "NULL" class */
  if(!a && !b) return 1;
  else if(!a || !b) return 0;
  return objIsOfClass(a, (*b)->classname);
}


obhash_t hash(const OBObjType *to_hash){

  obhash_t retval;

  if(!to_hash) return 0;

  if((*to_hash)->hash) retval = (*to_hash)->hash(to_hash);
  else{
    retval = (obhash_t)to_hash;
    retval += (retval << 6);
    retval ^= (retval >> 10);
    retval += (retval >> 7);
    retval ^= (retval << 12);
  }

  return retval;
}


int8_t compare(const OBObjType *a, const OBObjType *b){

  int8_t retval;

  if(a == NULL && b == NULL) return OB_EQUAL_TO;
  else if(a == NULL || b == NULL) return OB_NOT_EQUAL;


  if(sameClass(a, b) && (*a)->compare != NULL)
    retval = (*a)->compare(a, b);
  else if(a == b) retval = OB_EQUAL_TO;
  else retval = OB_NOT_EQUAL;

  return retval;
}


void display(const OBObjType *to_print){
  if(!to_print){
    fprintf(stderr, "NULL value\n");
    return;
  }

  fprintf(stderr, "Instance of class %s, at address 0x%p\n",
                  (*to_print)->classname, to_print);
  if((*to_print)->display) (*to_print)->display(to_print);
}

