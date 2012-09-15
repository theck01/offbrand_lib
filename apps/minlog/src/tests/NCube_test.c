
#include "../../../../include/offbrand.h"
#include "../../include/NCube.h"

int main (){

  NCube *c0, *c1, *c2, *c3, *c5, *c7, *c13, *c15;
  NCube *c01, *c23, *c57, *c1315;
  NCube *c0123, *c571315;

  c0 = createNCube(0, 0);
  c1 = createNCube(1, 0);
  c2 = createNCube(2, 0);
  c3 = createNCube(3, 1);
  c5 = createNCube(5, 1);
  c7 = createNCube(7, 1);
  c13 = createNCube(13, 1);
  c15 = createNCube(15, 1);

  if(!c0 || !c1 || !c2 || !c3 || !c5 || !c7 || !c13 || !c15){
    fprintf(stderr, "NCube_test: Could not create one of the NCubes required "
                    "for testing, TEST FAILED\n");
    exit(1);
  }

  if(isNCubePrimeImplicant(c3)){
    fprintf(stderr, "NCube_test: Dont care cube is considered prime implicant, "
                    "TEST FAILED\n");
    exit(1);
  }

  if(!(c01 = mergeNCubes(c0, c1))){
    fprintf(stderr, "NCube_test: Terms 0 and 1 could not be merged, TEST "
                    "FAILED\n");
    exit(1);
  }

  if(!(c23 = mergeNCubes(c2, c3))){
    fprintf(stderr, "NCube_test: Terms 2 and 3 could not be merged, TEST "
                    "FAILED\n");
    exit(1);
  }

  if((c57 = mergeNCubes(c5, c15))){
    fprintf(stderr, "NCube_test: Terms 5 and 15 were falsely merged, TEST "
                    "FAILED\n");
    exit(1);
  }

  if((c57 = mergeNCubes(c5, c01))){
    fprintf(stderr, "NCube_test: Terms 5 and 01 were falsely merged, TEST "
                    "FAILED\n");
    exit(1);
  }
  
  if(!(c57 = mergeNCubes(c5, c7))){
    fprintf(stderr, "NCube_test: Terms 5 and 7 could not be merged, TEST "
                    "FAILED\n");
    exit(1);
  }

  if(!(c1315 = mergeNCubes(c13, c15))){
    fprintf(stderr, "NCube_test: Terms 13 and 15 could not be merged, TEST "
                    "FAILED\n");
    exit(1);
  }

  if(!(c0123 = mergeNCubes(c01, c23))){
    fprintf(stderr, "NCube_test: Terms 01 and 23 could not be merged, TEST "
                    "FAILED\n");
    exit(1);
  }

  if(!(c571315 = mergeNCubes(c57, c1315))){
    fprintf(stderr, "NCube_test: Terms 57 and 1315 could not be merged, TEST "
                    "FAILED\n");
    exit(1);
  }

  if(!nCubeCoversTerm(c0123, 3)){
    fprintf(stderr, "NCube_test: NCube 0123 should cover 3 but doesn't, TEST "
                    "FAILED\n");
    exit(1);
  }

  if(nCubeCoversTerm(c0123, 15)){
    fprintf(stderr, "NCube_test: NCube 0123 should not cover 15 but does, TEST "
                    "FAILED\n");
    exit(1);
  }

  if(isNCubePrimeImplicant(c15) || isNCubePrimeImplicant(c23) ||
     !isNCubePrimeImplicant(c0123)){
    fprintf(stderr, "NCube_test: Prime implicant status not properly "
                    "transferred, TEST FAILED\n");
    exit(1);
  }

  if(isNCubePrimeImplicant(c571315)){
    fprintf(stderr, "NCube_test: Large NCube of dont cares is considered a "
                    "prime implicant, TEST FAILED\n");
    exit(1);
  }

  if(isNCubeEssential(c0123)){
    fprintf(stderr, "NCube_test: NCube should not be considered essential but "
                    "still is, TEST FAILED\n");
    exit(1);
  }

  markNCubeAsEssential(c0123);
  if(!isNCubeEssential(c0123)){
    fprintf(stderr, "NCube_test: NCube should be considered essential but "
                    "is not, TEST FAILED\n");
    exit(1);
  }

  if(strcmp(nCubeStr(c0123,1,3),"A`")){
    fprintf(stderr, "NCube_test: NCube SOP string was not properly generated "
                    "TEST FAILED\n");
    exit(1);
  }

  if(strcmp(nCubeStr(c0123,0,5),"(A`+B`+C`)")){
    fprintf(stderr, "NCube_test: NCube POS string was not properly generated "
                    "TEST FAILED\n");
    fprintf(stderr, "%s\n", nCubeStr(c0123,0,5));
    exit(1);
  }

  printf("NCube_test: TESTS PASSED\n");
  return 0;
}
