
#include "../../include/Term.h"
#include "../../include/private/Term_Private.h"

/* PUBLIC METHODS */

/* add arguments to complete initialization as needed, modify Term.h
 * as well if modifications are made */
Term * createTerm(void){

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

  /* ADD CLASS SPECIFIC INITIALIZATION HERE */

  return new_instance;
}

/* function can be deleted if unneeded */
int8_t compareTerm(const obj *a, const obj *b){
  
  Term *comp_a = (Term *)a;  
  Term *comp_b = (Term *)b;  

  /* add specific comparison logic, following the description in the header
   * file */
}


/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

void deallocTerm(obj *to_dealloc){

  /* cast generic obj to Term */
  Term *instance = (Term *)to_dealloc;

  /* PERFORM CLASS SPECIFIC MEMORY MANAGEMENT ON instance HERE */

  free(instance);

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
