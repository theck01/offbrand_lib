
#include "../include/Term.h"
#include "../include/NCube.h"
#include "../include/RTable.h"
#include "../include/minlog_funct.h"
#include "../../../include/OBVector.h"

int main(int argc, char **argv){

  OBVector *terms, *dont_cares, *pis, *essential_pis;
  RTable *reduction_table;
  uint8_t is_minterms;

  if(argc != 2){
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

  return 0;
}
  
