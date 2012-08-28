
#include "../../include/minlog_funct.h"

uint8_t numOneBits(uint32_t num){

  uint8_t i;
  uint8_t bit_count = 0;

  for(i=0; i<32; i++) bit_count += (num >> i)%2;

  return bit_count;
}


OBVector * findLargestPrimeImplicants(OBVector *terms, OBVector *dont_cares){

  uint32_t i, j, k, maxi, maxj;
  int32_t loop;
  OBVector *result, *cube_vectors, *cur_cube_vector, *prev_cube_vector;
  NCube *tmp_cube, *a, *b;
  Term *tmp_term;

  if(!terms || !dont_cares){
    fprintf(stderr, "findLargestPrimeImplicants: Unexpected NULL arguments\n");
    return NULL;
  }

  maxi = sizeOfVector(terms);
  if(maxi = 0){
    fprintf(stderr, "findLargestPrimeImplicants: Cannot find prime implicants "
                    "not given any terms\n");
    return NULL;
  }

  /* create vector for 0 cubes */
  cur_cube_vector = createVector(sizeOfVector(terms)+sizeOfVector(dont_cares));
  if(!cur_cube_vector){
    fprintf(stderr, "findLargestPrimeImplicants: Could not create vector for "
                    "0-cubes, returning NULL\n");
    return NULL;
  }

  /* create all cubes associated with terms */
  for(i=0; i<maxi; i++){
    /* get term for cube creation */
    tmp_term = (Term *)objAtVectorIndex(terms, i);
    if(!objIsOfClass((obj *)tmp_term, "Term")){
      fprintf(stderr, "findLargestPrimeImplicants: Term vector contains "
                      "non-terms, returning NULL\n");
      release((obj *)cur_cube_vector);
      return NULL;
    }

    /* create cube from term, which is not a dont care cube */
    tmp_cube = createNCube(getTermValue(tmp_term), 0);
    if(!tmp_cube){
      fprintf(stderr, "findLargestPrimeImplicants: Could not create cube for a "
                      "term, returning NULL\n");
      release((obj *)cur_cube_vector);
      return NULL;
    }

    /* add cube to cur_cube_vector */
    if(addToVector(cur_cube_vector, (obj *)tmp_cube)){
      fprintf(stderr, "findLargestPrimeImplicants: Could not add cube to "
                      "vector, returning NULL\n");
      return NULL;
    }

    /* release tmp_cube so only vector maintains valid reference */
    release((obj *)tmp_cube);
  }

  maxi = sizeOfVector(dont_cares)

  /* create all cubes associated with dont cares */
  for(i=0; i<maxi; i++){
    /* get term for cube creation */
    tmp_term = (Term *)objAtVectorIndex(dont_cares, i);
    if(!objIsOfClass((obj *)tmp_term, "Term")){
      fprintf(stderr, "findLargestPrimeImplicants: Term vector contains "
                      "non-terms, returning NULL\n");
      release((obj *)cur_cube_vector);
      return NULL;
    }

    /* create cube from term, which is not a dont care cube */
    tmp_cube = createNCube(getTermValue(tmp_term), 1);
    if(!tmp_cube){
      fprintf(stderr, "findLargestPrimeImplicants: Could not create cube for a "
                      "term, returning NULL\n");
      release((obj *)cur_cube_vector);
      return NULL;
    }

    /* add cube to cur_cube_vector */
    if(addToVector(cur_cube_vector, (obj *)tmp_cube)){
      fprintf(stderr, "findLargestPrimeImplicants: Could not add cube to "
                      "vector, returning NULL\n");
      return NULL;
    }

    /* release tmp_cube so only vector maintains valid reference */
    release((obj *)tmp_cube);
  } 

  /* create vector to contain vectors of NCubes (each sub vector contains cubes
   * of all the same order) */
  cube_vectors = createVector(1);
  if(!cube_vectors){
    fprintf(stderr, "findLargestPrimeImplicants: Could not create vector of "
                    "vectors, returning NULL\n");
    release((obj *)cur_cube_vector);
    return NULL;
  }
  
  /* add 0 cube vector to vector of vectors */
  if(addToVector(cube_vectors, (obj *)cur_cube_vector)){
    fprintf(stderr, "findLargestPrimeImplicants: Could not add 0-cube vector "
                    "to matrix of cubes\n");
    release((obj *)cur_cube_vector);
    return NULL;
  }

  /* release cur_cube_vector so only cube_vectors maintains valid reference */
  release((obj *)cur_cube_vector);

  /* while cubes can still be merged into larger cubes */
  k = 0;
  loop = 1;
  while(loop>0){

    /* by default set loop to stop after this run */
    loop = -1; 

    /* get previous cube vector, and create new vector for next order of cubes*/
    prev_cube_vector = (OBVector *)objAtVectorIndex(cube_vectors, k);
    cur_cube_vector = createVector(sizeOfVector(prev_cube_vector)/4);
    if(!cur_cube_vector){
      fprintf(stderr, "findLargestPrimeImplicants: Could not create vector for "
                      "higher order cubes, returning NULL\n");
      release((obj *)cube_vectors);
      return NULL;
    }

    /* for all pairs cubes, attempt to merge */
    maxi = sizeOfVector(prev_cube_vector);
    for(i=0; i<maxi-1; i++){

      a = (NCube *)objAtVectorIndex(prev_cube_vector, i);

      for(j=i+1; j<maxi; j++){

        b = (NCube *)objAtVectorIndex(prev_cube_vector, i);
        
        /*if cubes can be merged */
        if((tmp_cube = mergeNCubes(a, b))){
          if(addToVector(cur_cube_vector, (obj *)tmp_cube)){
            fprintf(stderr, "findLargestPrimeImplicants: Could not add new "
                            "cube to vector, returning NULL\n");
            release((obj *)tmp_cube);
            release((obj *)cur_cube_vector);
            release((obj *)cube_vectors);
            return NULL;
          }

          /* release tmp_cube so that vector has only valid reference */
          release((obj *)tmp_cube);

          /* increment loop to indicate that another larger cube was created */
          loop++;
        }
      }
    }
    
    if(addToVector(cube_vectors, (obj *)cur_cube_vector)){
      fprintf(stderr, "findLargestPrimeImplicants: Could not add new "
                      "cube vector to matrix, returning NULL\n");
      release((obj *)cur_cube_vector);
      release((obj *)cube_vectors);
      return NULL;
    }

    /* release cur_cube_vector so cube_vectors maintains only valid reference */
    release((obj *)cur_cube_vector);

    /* increment k to work on next order of cube vectors */
    k++;
  }

  /* Create final vector of only prime implicant cubes */
  result = createVector(4);
  if(!result){
    fprintf(stderr, "findLargestPrimeImplicants: Could not create result "
                    "vector, returning NULL\n");
    release((obj *)cube_vectors);
  }

  maxi = sizeOfVector(cube_vectors);
  for(i=0; i<maxi; i++){

    cur_cube_vector = (OBVector *)objAtVectorIndex(cube_vectors, i);
    
    maxj = sizeOfVector(cur_cube_vector);
    for(j=0; j<maxj; j++){

      tmp_cube = (NCube *)objAtVectorIndex(cur_cube_vector, j);
      if(isNCubePrimeImplicant(tmp_cube)){
        if(addToVector(result, (obj *)tmp_cube)){
          fprintf(stderr, "findLargestPrimeImplicants: Could not add prime "
                          "implicant to result vector, returning NULL\n");
          release((obj *)result);
          release((obj *)cube_vectors);
          return NULL;
        }
      }
    }
  }

  release((obj *)cube_vectors);
  return result;
}


