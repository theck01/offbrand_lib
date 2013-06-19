
#include "../../include/Term.h"
#include "../../include/private/Term_Private.h"

/* PUBLIC METHODS */

Term * createTerm(uint32_t term){

  static char classname[] = "Term";

  Term *new_instance = malloc(sizeof(Term));
  assert(new_instance != NULL);

  /* initialize reference counting base data */
  ob_init_base((obj *)new_instance, &deallocTerm, NULL, &compareTerms, NULL,
               classname);

  new_instance->term = term;
  return new_instance;
}

uint32_t getTermValue(Term *a){
  assert(a != NULL);
  return a->term;
}

/* PRIVATE METHODS */

int8_t compareTerms(const obj *a, const obj *b){

  Term *comp_a = (Term *)a;
  Term *comp_b = (Term *)b;

  assert(a != NULL && b != NULL);
  assert(ob_has_class(a, "Term") && ob_has_class(b, "Term"));

  if(comp_a->term > comp_b->term) return OB_GREATER_THAN;
  else if(comp_a->term < comp_b->term) return OB_LESS_THAN;
  else return OB_EQUAL_TO;
}

void deallocTerm(obj *to_dealloc){
  /* cast generic obj to Term */
  Term *instance = (Term *)to_dealloc;
  assert(instance != NULL);
  assert(ob_has_class(to_dealloc, "Term"));
  return;
}
