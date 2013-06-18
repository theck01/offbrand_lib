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
  primes = OBVectorCreateWithCapacity(1);

  // seed primes vector with initial prime 2. Note the cast of 
  // the OBInt to obj, an OBVector stores the generic obj type rather than
  // any specific type
  candidate = OBIntCreate(2);
  OBVectorStoreAtIndex(primes, candidate, 0);

  // release the candidate so the vector maintains the only reference to the
  // prime number. release operates on the generic obj type, so the pointer
  // is cast
  OBRelease(candidate);

  // begin the main prime finding loop
  candidate = OBIntCreate(3);
  while(1){

    maybe_prime = 1;

    // search for a prime factor of candidate
    for(i=0; i<OBVectorGetLength(primes); i++){
      remainder = OBIntMod(candidate, (OBInt *)OBVectorObjectAtIndex(primes, i));
      // if the remainder is 0 then the candidate number is not prime
      if(OBIntIsZero(remainder)) maybe_prime = 0;
      OBRelease(remainder);
      if(!maybe_prime) break;
    }

    // if the number is still maybe prime then it is now definitely prime,
    // add it to the end of the primes vector
    if(maybe_prime){
      OBVectorStoreAtIndex(primes, candidate, OBVectorGetLength(primes));
      numstr = OBIntGetStringValue(candidate);
      printf("Prime found: %s\n", OBStringGetCString(numstr));
      OBRelease(numstr);
    }

    // increment candidate by 2 and release old candidate, it is not needed
    // by main loop
    next = OBIntAddPrimitive(candidate, 2);
    OBRelease(candidate);
    candidate = next;
  }
}

