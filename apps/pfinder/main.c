#include "../../include/offbrand.h"
#include "../../include/OBInt.h"
#include "../../include/OBString.h"
#include "../../include/OBVector.h"

int main(){

  uint64_t i, maybe_prime;
  OBInt *candidate, *next, *remainder;
  OBString *numstr;
  OBVector *primes;

  // create vector with a small initial capacity
  primes = createVector(1);

  // seed primes vector with initial prime 2. Note the cast of 
  // the OBInt to obj, an OBVector stores the generic obj type rather than
  // any specific type
  candidate = createIntWithInt(2);
  storeAtVectorIndex(primes, (obj *)candidate, 0);

  // release the candidate so the vector maintains the only reference to the
  // prime number. release operates on the generic obj type, so the pointer
  // is cast
  release((obj *)candidate);

  // begin the main prime finding loop
  candidate = createIntWithInt(3);
  while(1){

    maybe_prime = 1;

    // search for a prime factor of candidate
    for(i=0; i<vectorLength(primes); i++){
      remainder = modInts(candidate, (OBInt *)objAtVectorIndex(primes, i));
      // if the remainder is 0 then the candidate number is not prime
      if(isIntZero(remainder)) maybe_prime = 0;
      release((obj *)remainder);
      if(!maybe_prime) break;
    }

    // if the number is still maybe prime then it is now definitely prime,
    // add it to the end of the primes vector
    if(maybe_prime){
      storeAtVectorIndex(primes, (obj *)candidate, vectorLength(primes));
      numstr = stringFromInt(candidate);
      printf("Prime found: %s\n", getCString(numstr));
      release((obj *)numstr);
    }

    // increment candidate by 2 and release old candidate, it is not needed
    // by main loop
    next = addIntAndPrim(candidate, 2); 
    release((obj *)candidate);
    candidate = next;
  }
}

