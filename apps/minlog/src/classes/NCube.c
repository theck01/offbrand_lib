
#include "../../include/NCube.h"
#include "../../include/private/NCube_Private.h"
#include "../../include/minlog_funct.h"

/* PUBLIC METHODS */

NCube * createNCube(uint32_t term, uint8_t is_dont_care){

  NCube *new_instance;

  assert(term < (1 << 27));

  new_instance = createNCubeWithOrder(0);
  assert(new_instance != NULL);

  new_instance->terms[0] = term;
  if(is_dont_care){
    new_instance->prime_implicant = 0;
    new_instance->all_dont_cares = 1;
  }

  return new_instance;
}


NCube * mergeNCubes(NCube *a, NCube *b){

  uint32_t xor_term, i, j, numterms;
  NCube *result;

  assert(a != NULL && b != NULL);

  /* if dont care bits are not the same, do not merge */
  if(a->dont_cares != b->dont_cares){
    return NULL;
  }

  /* if the terms do not differ by only a single bit (a power of two), do
   * not merge */
  xor_term = a->terms[0] ^ b->terms[0];
  if(numOneBits(xor_term) != 1){
    return NULL;
  }

  result = createNCubeWithOrder(a->order + 1);

  i=0;
  j=0;
  numterms = 1 << a->order;
  /* merge sort terms into new resultant term, lowest to highest */
  while(i<numterms && j<numterms){
    if(a->terms[i] < b->terms[j]){
      result->terms[i+j] = a->terms[i];
      i++;
    }
    else{
      result->terms[i+j] = b->terms[j];
      j++;
    }
  }
  /* add remaining terms from cube, loops does nothing if all terms from cube
   * already added */
  for( ; i<numterms; i++){
    result->terms[i+j] = a->terms[i];
  }
  for( ; j<numterms; j++){
    result->terms[i+j] = b->terms[j];
  }

  /* add in additional dont care bit */
  result->dont_cares = a->dont_cares | xor_term;

  /* set a and b as non prime implicants */
  a->prime_implicant = 0;
  b->prime_implicant = 0;

  /* if new cube is composed of sub cubes of all dont cares, new cube is not a
   * prime implicant, and it is composed entirely of dont cares as well */
  if(a->all_dont_cares && b->all_dont_cares){
    result->prime_implicant = 0;
    result->all_dont_cares = 1;
  }

  return result;
}


uint8_t nCubeCoversTerm(const NCube *a, uint32_t term){

  uint32_t i, max_i;

  assert(a != NULL);

  max_i = 1 << a->order;
  for(i=0; i<max_i; i++){
    if(a->terms[i] == term){
      return 1;
    }
  }

  return 0;
}


uint8_t isNCubePrimeImplicant(const NCube *a){
  assert(a != NULL);
  return a->prime_implicant;
}


void markNCubeAsEssential(NCube *a){
  assert(a != NULL);
  a->essential = 1;
  return;
}


uint8_t isNCubeEssential(const NCube *a){
  assert(a != NULL);
  return a->essential;
}


uint8_t orderOfNCube(const NCube *a){
  assert(a != NULL);
  return a->order;
}


char * nCubeStr(const NCube *a, uint8_t is_sop, uint8_t num_var){

  uint8_t i, k;
  char *str;
  char curvar = 'A'; /* assumes that curvar can be incremented, resulting in B,
                        C, D ... */

  assert(a != NULL && num_var < 27);

  str = malloc(sizeof(char) * 256);
  assert(str != NULL);

  /* if all possible terms are encompased in term, return a string of 1 */
  if(a->order > num_var){
    strcpy(str, "1");
    return str;
  }

  k = 0;

  if(!is_sop) str[k++] = '(';

  for(i=num_var-1; i<num_var; i--){

    /* if the current variable is not a dont care, add it to string */
    if(!testBit(a->dont_cares, i)){
      /* add the char representation of the variable */
      str[k++] = curvar;
      /* if the bit for the variable is 0, add an additional ' (to indicate a
       * notted variable) */
      if(!testBit(a->terms[0],i)) str[k++] = '`';

      /* if not is_sop, add '+' char */
      if(!is_sop){
        str[k++] = '+';
      }
    }

    curvar++;
  }

  /* if the term is product of sums, add final parenthesis and overwrite final
   * "+" */
  if(!is_sop){
    k--;
    str[k++] = ')';
  }

  str[k] = '\0';

  return str;
}


/* PRIVATE METHODS */

NCube * createNCubeWithOrder(uint8_t order){

  static const char classname[] = "NCube";

  NCube *new_cube;

  assert(order <= 27);

  new_cube = malloc(sizeof(NCube));
  assert(new_cube != NULL);

  /* initialize reference counting base data */
  ob_init_base((obj *)new_cube, &deallocNCube, NULL, &compareNCubes, NULL,
               classname);

  new_cube->terms = malloc(sizeof(uint32_t)*(1<<order));
  assert(new_cube->terms != NULL);

  new_cube->dont_cares = 0;
  new_cube->order = order;
  new_cube->prime_implicant = 1;
  new_cube->all_dont_cares = 0;
  new_cube->essential = 0;

  return new_cube;
}


int8_t compareNCubes(const obj *a, const obj *b){

  uint32_t i, max_i;
  NCube *comp_a = (NCube *)a;
  NCube *comp_b = (NCube *)b;

  assert(a != NULL && b != NULL);
  assert(ob_has_class(a, "NCube") && ob_has_class(b, "NCube"));

  if(comp_a->order != comp_b->order){
    return OB_NOT_EQUAL;
  }

  max_i = 1 << comp_a->order;
  for(i=0; i<max_i; i++){
    if(comp_a->terms[i] != comp_b->terms[i]){
      return OB_NOT_EQUAL;
    }
  }

  return OB_EQUAL_TO;
}


void deallocNCube(obj *to_dealloc){
  /* cast generic obj to NCube */
  NCube *instance = (NCube *)to_dealloc;
  assert(instance != NULL);
  assert(ob_has_class(to_dealloc, "NCube"));
  free(instance->terms);
  return;
}
