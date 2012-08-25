
#include "../../include/misc_functions.h"

uint8_t numOneBits(uint32_t num){

  uint8_t i;
  uint8_t bit_count = 0;

  for(i=0; i<32; i++) bit_count += (num >> i)%2;

  return bit_count;
}
