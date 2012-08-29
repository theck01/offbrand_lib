
#include "../../include/Term.h"
#include "../../include/private/Term_Private.h"

/* PUBLIC METHODS */

/* add arguments to complete initialization as needed, modify Term.h
 * as well if modifications are made */
Term * createTerm(uint32_t term){

  Term *new_instance = malloc(sizeof(Term));
  assert(new_instance != NULL);

  /* initialize reference counting base data */
  initTermBase(new_instance);

  new_instance->term = term;
  return new_instance;
}

/* function can be deleted if unneeded */
int8_t compareTerm(const obj *a, const obj *b){
  
  Term *comp_a = (Term *)a;  
  Term *comp_b = (Term *)b;  

  assert(a != NULL && b != NULL);

  if(comp_a->term > comp_b->term) return OB_GREATER_THAN;
  else if(comp_a->term < comp_b->term) return OB_LESS_THAN;
  else return OB_EQUAL_TO;
}

uint32_t getTermValue(Term *a){
  assert(a != NULL);
  return a->term;
}

/* PRIVATE METHODS */

void initTermBase(Term *to_init){

  /* Classname for the this specific class */
  static char *classname = NULL;
  const char stack_classname[] = "Term";

  assert(to_init != NULL);

  if(!classname){
    classname = malloc(sizeof(char) * strlen(stack_classname));
    assert(classname != NULL);
    strcpy(classname, stack_classname);
  }

  /* initialize reference counting base data */
  initBase((obj *)to_init, &deallocTerm, classname);

  return;
}


void deallocTerm(obj *to_dealloc){
  /* cast generic obj to Term */
  Term *instance = (Term *)to_dealloc;
  assert(instance != NULL);
  free(instance);
  return;
}
