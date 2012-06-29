
#include "../include/offbrand.h"

uint8_t defaultCompare(obj *a, obj *b){
  if(a == b){
    return 0;
  }

  return 1;
}
