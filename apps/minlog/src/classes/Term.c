
#include "../../include/Term.h"
#include "../../include/private/Term_Private.h"

/* PUBLIC METHODS */

/* add arguments to complete initialization as needed, modify Term.h
 * as well if modifications are made */
Term * createTerm(uint32_t term){

  Term *new_instance = malloc(sizeof(Term));
  if(!new_instance){
    fprintf(stderr, "Term: Could not allocate memory for "
                    "new_instance\n");
    return NULL;
  }

  /* initialize reference counting base data */
  if(initBase((obj *)new_instance, &deallocTerm)){
    fprintf(stderr, "Term: Could not initialize base obj\n");
    return NULL;
  }

  new_instance->term = term;
  return new_instance;
}

/* function can be deleted if unneeded */
int8_t compareTerm(const obj *a, const obj *b){
  
  Term *comp_a = (Term *)a;  
  Term *comp_b = (Term *)b;  

  if(comp_a->term > comp_b->term) return OB_GREATER_THAN;
  else if(comp_a->term < comp_b->term) return OB_LESS_THAN;
  else return OB_EQUAL_TO;
}

int32_t getTermValue(Term *a){
  if(!a) return -1;
  else return a->term;
}

/* PRIVATE METHODS */

void deallocTerm(obj *to_dealloc){
  /* cast generic obj to Term */
  Term *instance = (Term *)to_dealloc;
  free(instance);
  return;
}
