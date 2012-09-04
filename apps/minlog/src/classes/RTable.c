
#include "../../include/RTable.h"
#include "../../include/private/RTable_Private.h"

/* PUBLIC METHODS */

/* add arguments to complete initialization as needed, modify RTable.h
 * as well if modifications are made */
RTable * createRTable(OBVector *prime_implicants, OBVector *terms){

  RTable *new_instance = malloc(sizeof(RTable));
  assert(new_instance != NULL);

  /* initialize reference counting base of class */
  initRTableBase((obj *)new_instance);

  
  

  return new_instance;
}


/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

void initRTableBase(RTable *to_init){

  /* Classname for the this specific class */
  static char *classname = NULL;
  const char stack_classname[] = "RTable";

  assert(to_init != NULL);

  if(!classname){
    classname = malloc(sizeof(char) * strlen(stack_classname));
    assert(classname != NULL);
    strcpy(classname, stack_classname);
  }

  /* initialize reference counting base data */
  initBase((obj *)to_init, &deallocRTable, classname);

  return;
}


void deallocRTable(obj *to_dealloc){

  /* cast generic obj to RTable */
  RTable *instance = (RTable *)to_dealloc;

  assert(to_dealloc && objIsOfClass(to_dealloc, "RTable");

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE */

  free(instance);

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
