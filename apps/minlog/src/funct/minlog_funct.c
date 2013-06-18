
#include "../../include/minlog_funct.h"
#include "../../include/NCube.h"
#include "../../include/Term.h"
#include <regex.h>

uint8_t numOneBits(uint32_t num){

  uint8_t i;
  uint8_t bit_count = 0;

  for(i=0; i<32; i++) bit_count += (num >> i)%2;

  return bit_count;
}

uint8_t testBit(uint32_t num, uint8_t bit_num){
  assert(bit_num < 32);
  return (num >> bit_num) & 1;
}


uint8_t parseEqnString(const char *eqnstr, OBVector *terms,
                       OBVector *dont_cares, uint8_t verbose){

  uint8_t retval;
  char termstr[1024], dcstr[1024], *curhead; /* substrings that contain terms 
                                                and dont cares (dc), and pointer
                                                to current head of string*/
  char cur_term[1024];   /*current term string representaion */
  uint32_t cur_term_int; /*current term integer representaion */
  uint32_t i;
  int16_t dc_start, terms_start;
  regex_t dc_regex, term_regex, sop_regex, pos_regex; /* various compiled
                                                         regexs */
  regmatch_t match;
  Term *new_term_obj;

  /* compile regular expressions */
  assert(regcomp(&dc_regex, "[Dd]", REG_EXTENDED) == 0);
  assert(regcomp(&term_regex, "[[:digit:]]+", REG_EXTENDED) == 0);
  assert(regcomp(&pos_regex, "M", REG_EXTENDED) == 0);
  assert(regcomp(&sop_regex, "m", REG_EXTENDED) == 0);

  /* clear vectors of terms and dont_cares */
  OBVectorClear(terms);
  OBVectorClear(dont_cares);

  /* determine where terms and dont cares are located in the string */
  if(!regexec(&dc_regex, eqnstr, 1, &match, REG_NOTBOL|REG_NOTEOL))
    dc_start = match.rm_so;
  else dc_start = -1;

  if(!regexec(&pos_regex, eqnstr, 1, &match, REG_NOTBOL|REG_NOTEOL)){
    terms_start = match.rm_so;
    retval = MINLOG_MAXTERMS;
  }
  else if(!regexec(&sop_regex, eqnstr, 1, &match, REG_NOTBOL|REG_NOTEOL)){
    terms_start = match.rm_so;
    retval = MINLOG_MINTERMS;
  }
  else{
    fprintf(stderr, "minlog:parseEqnStr - Improper equation format, could\n"
                    "not find a m/M to indicate start of minterms or "
                    "maxterms respectively\n");
    exit(1);
  }

  /* copy term portions into proper srings */
  if(dc_start == -1){
    strncpy(termstr, eqnstr, 1024);
    termstr[1023] = '\0'; /* ensure last character is a NULL terminator */
  }
  else if(dc_start < terms_start){
    strncpy(dcstr, eqnstr, terms_start);
    dcstr[terms_start] = '\0';
    strncpy(termstr, eqnstr+terms_start, 1024 - terms_start);
    termstr[1023] = '\0'; /* ensure last character is a NULL terminator */
  }
  else{
    strncpy(termstr, eqnstr, dc_start);
    termstr[dc_start] = '\0';
    strncpy(dcstr, eqnstr+dc_start, 1024 - dc_start);
    dcstr[1023] = '\0'; /* ensure last character is a NULL terminator */
  }

  /* read in all terms from termstr */
  curhead = termstr;
  while(!regexec(&term_regex, curhead, 1, &match, REG_NOTBOL|REG_NOTEOL)){

    strncpy(cur_term, curhead + match.rm_so, match.rm_eo - match.rm_so);
    cur_term[match.rm_eo - match.rm_so] = '\0';

    cur_term_int = atoi(cur_term);

    new_term_obj = createTerm(cur_term_int);
    OBVectorStoreAtIndex(terms,new_term_obj, OBVectorGetLength(terms));
    OBRelease(new_term_obj);

    curhead += match.rm_eo;
  }

  /* read in all terms from dcstr, if it exists */
  if(dc_start != -1){
    curhead = dcstr;
    while(!regexec(&term_regex, curhead, 1, &match, REG_NOTBOL|REG_NOTEOL)){

      strncpy(cur_term, curhead + match.rm_so, match.rm_eo - match.rm_so);
      cur_term[match.rm_eo - match.rm_so] = '\0';

      cur_term_int = atoi(cur_term);

      new_term_obj = createTerm(cur_term_int);
      OBVectorStoreAtIndex(dont_cares, new_term_obj,
                         OBVectorGetLength(dont_cares));
      OBRelease(new_term_obj);

      curhead += match.rm_eo;
    }
  }

  /* free regexs */
  regfree(&term_regex);
  regfree(&dc_regex);
  regfree(&pos_regex);
  regfree(&sop_regex);

  /* check that some terms were read in */
  if(OBVectorGetLength(terms) == 0){
    fprintf(stderr, "minlog:parseEqnStr - No terms supplied in the "
                    "equation,\nProgram exits due to bad equation format\n");
    exit(1);
  }

  /* print out results of parse, if desired */
  if(verbose){

    if(retval == MINLOG_MINTERMS) printf("Minterms parsed from equation:\n");
    else printf("Maxterms parsed from equation:\n");
    for(i=0; i<OBVectorGetLength(terms); i++){
      printf("%u\n", getTermValue((Term *)OBVectorObjectAtIndex(terms, i)));
    }

    printf("\n");

    if(OBVectorGetLength(dont_cares) > 0){
      printf("Dont cares parsed from equation:\n");
      for(i=0; i<OBVectorGetLength(dont_cares); i++){
        printf("%u\n", getTermValue((Term *)OBVectorObjectAtIndex(dont_cares, i)));
      }
    }
    else printf("Without any dont care terms\n");
  }
  
  return retval;
}


OBVector * findLargestPrimeImplicants(const OBVector *terms, 
                                      const OBVector *dont_cares){

  uint32_t i, j, k, maxi, maxj;
  int32_t loop;
  OBVector *result, *cube_vectors, *cur_cube_vector, *prev_cube_vector;
  NCube *tmp_cube, *a, *b;
  Term *tmp_term;

  assert(terms != NULL && OBVectorGetLength(terms) != 0);

  maxi = OBVectorGetLength(terms);

  /* create vector for 0 cubes */
  cur_cube_vector = OBVectorCreateWithCapacity(OBVectorGetLength(terms)+OBVectorGetLength(dont_cares));

  /* create all cubes associated with terms */
  for(i=0; i<maxi; i++){
    /* get term for cube creation */
    tmp_term = (Term *)OBVectorObjectAtIndex(terms, i);
    assert(OBObjIsOfClass(tmp_term, "Term"));

    /* create cube from term, which is not a dont care cube */
    tmp_cube = createNCube(getTermValue(tmp_term), 0);

    /* add cube to cur_cube_vector */
    OBVectorStoreAtIndex(cur_cube_vector, tmp_cube,
                       OBVectorGetLength(cur_cube_vector));

    /* release tmp_cube so only vector maintains valid reference */
    OBRelease(tmp_cube);
  }

  /* if dont_care terms are supplied, create cubes for them */
  if(dont_cares){

    maxi = OBVectorGetLength(dont_cares);

    for(i=0; i<maxi; i++){
      /* get term for cube creation */
      tmp_term = (Term *)OBVectorObjectAtIndex(dont_cares, i);
      assert(OBObjIsOfClass(tmp_term, "Term"));

      /* create cube from term, which is not a dont care cube */
      tmp_cube = createNCube(getTermValue(tmp_term), 1);

      /* add cube to cur_cube_vector */
      OBVectorStoreAtIndex(cur_cube_vector, tmp_cube,
                         OBVectorGetLength(cur_cube_vector));

      /* release tmp_cube so only vector maintains valid reference */
      OBRelease(tmp_cube);
    }
  }


  /* create vector to contain vectors of NCubes (each sub vector contains cubes
   * of all the same order) */
  cube_vectors = OBVectorCreateWithCapacity(1);
  
  /* add 0 cube vector to vector of vectors */
  OBVectorStoreAtIndex(cube_vectors, cur_cube_vector,
                     OBVectorGetLength(cube_vectors));

  /* release cur_cube_vector so only cube_vectors maintains valid reference */
  OBRelease(cur_cube_vector);

  /* while cubes can still be merged into larger cubes */
  k = 0;
  loop = 1;
  while(loop>0){

    /* by default set loop to stop after this run, two cubes of current order
     * must be created to continue search for additional higher order cubes */
    loop = -1; 

    /* get previous cube vector, and create new vector for next order of cubes*/
    prev_cube_vector = (OBVector *)OBVectorObjectAtIndex(cube_vectors, k);
    cur_cube_vector = OBVectorCreateWithCapacity(OBVectorGetLength(prev_cube_vector)/4);

    /* for all pairs cubes, attempt to merge */
    maxi = OBVectorGetLength(prev_cube_vector);
    for(i=0; i<maxi-1; i++){

      a = (NCube *)OBVectorObjectAtIndex(prev_cube_vector, i);

      for(j=i+1; j<maxi; j++){

        b = (NCube *)OBVectorObjectAtIndex(prev_cube_vector, j);
        /*if cubes can be merged */

        /* if the cubes can be merged, and an equivalent cube is not already in
         * the cur cube vector */
        if((tmp_cube = mergeNCubes(a, b))){
          if(!OBVectorContains(cur_cube_vector,tmp_cube)){
            OBVectorStoreAtIndex(cur_cube_vector, tmp_cube,
                               OBVectorGetLength(cur_cube_vector));
            /* increment loop to indicate that larger cube was created */
            loop++;
          } 
          /* release tmp_cube whether or not it was added to the vector, either
           * freeing or leaving vector with only valid reference */
          OBRelease(tmp_cube);
        }
      }
    }
    
    OBVectorStoreAtIndex(cube_vectors, cur_cube_vector,
                       OBVectorGetLength(cube_vectors));
    /* release cur_cube_vector so cube_vectors maintains only valid reference */
    OBRelease(cur_cube_vector);
    /* increment k to work on next order of cube vectors */
    k++;
  }

  /* Create final vector of only prime implicant cubes */
  result = OBVectorCreateWithCapacity(1);

  maxi = OBVectorGetLength(cube_vectors);
  for(i=0; i<maxi; i++){
    
    cur_cube_vector = (OBVector *)OBVectorObjectAtIndex(cube_vectors, i);
    maxj = OBVectorGetLength(cur_cube_vector);

    for(j=0; j<maxj; j++){
      tmp_cube = (NCube *)OBVectorObjectAtIndex(cur_cube_vector, j);
      if(isNCubePrimeImplicant(tmp_cube)){
        OBVectorStoreAtIndex(result, tmp_cube, OBVectorGetLength(result));
      }
    }
  }

  OBRelease(cube_vectors);
  return result;
}

void printEqnVector(const OBVector *essential_pis, uint8_t is_sop, 
                    uint32_t num_var){

  uint32_t i;
  char *cubestr;

  printf("\nReduced Equation:\n");
  for(i=0; i<OBVectorGetLength(essential_pis); i++){

    assert(OBObjIsOfClass(OBVectorObjectAtIndex(essential_pis, i), "NCube"));
    cubestr = nCubeStr((NCube *)OBVectorObjectAtIndex(essential_pis, i), is_sop,
                        num_var);
    printf("%s", cubestr);
    free(cubestr);
    
    /* if printing terms in sum of products, print '+' in proper places */
    if(is_sop && i != OBVectorGetLength(essential_pis)-1) printf("+");
  }

  printf("\n");

  return;
}


