
#include "../include/offbrand.h"

uint8_t defaultCompare(obj *a, obj *b){

  if(!a || !b){
    fprintf(stderr, "offbrand_stdlib: NULL argument(s) passed to "
                    "defaultCompare\n");
    return OB_COMPARE_ERR;
  }

  if(a == b){
    return 0;
  }

  return 1;
}
