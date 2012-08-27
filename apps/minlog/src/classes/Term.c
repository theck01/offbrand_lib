
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
  if(initTermBase(new_instance)){
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

uint8_t initTermBase(Term *to_init){

  /* Classname for the this specific class */
  static char *classname = NULL;
  const char stack_classname[] = "Term";

  if(!classname){
    classname = malloc(sizeof(char) * strlen(stack_classname));
    if(!classname){
      fprintf(stderr, "Term: Could not allocate static classname "
                      "for all instances\nClass checking functions will not "
                      "work until classname allocated\n");
      return 1;
    }
    else strcpy(classname, stack_classname);
  }

  /* initialize reference counting base data */
  if(initBase((obj *)to_init, &deallocTerm, classname)){
    fprintf(stderr, "Term: Could not initialize base obj\n");
    return 1;
  }

  return 0;
}


void deallocTerm(obj *to_dealloc){
  /* cast generic obj to Term */
  Term *instance = (Term *)to_dealloc;
  free(instance);
  return;
}
