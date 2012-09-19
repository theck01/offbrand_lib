
#include "../include/Term.h"
#include "../include/NCube.h"
#include "../include/RTable.h"
#include "../include/minlog_funct.h"
#include "../../../include/OBVector.h"

#define MAX_EQN_SIZE 2048

int main(int argc, char **argv){

  OBVector *terms, *dont_cares, *pis, *essential_pis;
  RTable *reduction_table;
  uint8_t is_minterms, num_var;
  uint32_t max_term, numchar, substrlen; 
  int i;
  char eqnstr[MAX_EQN_SIZE];

  if(argc < 2){
    fprintf(stderr,
            "\n%s: A logic minimization application\n\n"
            "Usage:\n"
            "%s <boolean eqn>\n\n"
            "<boolean eqn> is a boolean function represented as a group of\n"
            "minterms or maxterms and any don't care terms. The format for\n"
            "boolean function input is as follows:\n\n"
            "M denotes the function is composed of maxterms, and precedes all\n"
            "  maxterm values\n"
            "m denotes the function is composed of minterms, and precedes all\n"
            "  minterm values\n"
            "d or D denotes the start of don't care terms, and precedes all\n"
            "  don't care terms\n\n"
            "M and m should not both exist in the same function at the same\n"
            "time, else ERRORS WILL OCCUR. m/M must precede all terms that\n"
            "the function must cover, d must precede all terms that the\n"
            "function can cover if it helps minimize the function further\n\n"
            "Example program calls:\n"
            "  VALID:   %s m(1,2,3,4)+d(5,6)\n"
            "  VALID:   %s d 1 4 5 6 M 8 9\n"
            "  INVALID: %s M(1,4,5)+m(3,7,10)+d(12,14)\n"
            "  INVALID: %s 10 4 d 58 39 M\n\n", argv[0], argv[0], argv[0], 
                                                argv[0], argv[0], argv[0]);
    exit(1);
  }

  /* compose eqnstr of all strings after argv[0] */
  numchar = 0;
  for(i=1; i<argc; i++){

    /* verify that copying the argv string into eqnstr will not overflow 
     * array */
    substrlen = strlen(argv[i]);
    assert(numchar+substrlen < MAX_EQN_SIZE);

    strcpy(eqnstr + numchar, argv[i]);
    numchar+=substrlen;
    eqnstr[numchar++] = ' ';
  }
  eqnstr[numchar] = '\0'; /* add null terminator to string */
    
  terms = createVector(32);
  dont_cares = createVector(32);


  /* parse verbosely for testing */
  is_minterms = parseEqnString(eqnstr, terms, dont_cares, 1);

  /* find largest term, found by sorting and picking first term. Inefficient but
   * concise compared to iterating through each term and checking results */
  sortVector(terms, &compareTerms, OB_GREATEST_TO_LEAST);
  max_term = getTermValue((Term *)objAtVectorIndex(terms, 0));
  num_var = 0;
  /* increase number of variables until 2^num_var > max_term */
  while(1u<<num_var <= max_term) num_var++;

  /* find prime implicants */
  pis = findLargestPrimeImplicants(terms, dont_cares);

  release((obj *)dont_cares);

  /* create the table used to reduce the prime implicants to the minimal
   * essential prime implicants */
  reduction_table = createRTable(pis, terms);
  
  release((obj *)terms);
  release((obj *)pis);

  /* find the minimal function representation */
  essential_pis = findEssentialPIs(reduction_table); 

  release((obj *)reduction_table);
  
  /* print the result to stdout */
  printEqnVector(essential_pis, is_minterms, num_var);

  release((obj *)essential_pis);

  return 0;
}
  
