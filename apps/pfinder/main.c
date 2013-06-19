#include "../../include/offbrand.h"
#include "../../include/obint.h"
#include "../../include/obstring.h"
#include "../../include/obvector.h"

int main(){

  uint64_t i, maybe_prime;
  obint *candidate, *next, *remainder;
  obstring *numstr;
  obvector *primes;

  // create vector with a small initial capacity
  primes = obvector_new(1);

  // seed primes vector with initial prime 2. Note the cast of
  // the obint to obj, an obvector stores the generic obj type rather than
  // any specific type
  candidate = obint_new(2);
  obvector_store_at_index(primes, (obj *)candidate, 0);

  // ob_release the candidate so the vector maintains the only reference to the
  // prime number. ob_release operates on the generic obj type, so the pointer
  // is cast
  ob_release((obj *)candidate);

  // begin the main prime finding loop
  candidate = obint_new(3);
  while(1){

    maybe_prime = 1;

    // search for a prime factor of candidate
    for(i=0; i<obvector_length(primes); i++){
      remainder = obint_mod(candidate, (obint *)obvector_obj_at_index(primes, i));
      // if the remainder is 0 then the candidate number is not prime
      if(obint_is_zero(remainder)) maybe_prime = 0;
      ob_release((obj *)remainder);
      if(!maybe_prime) break;
    }

    // if the number is still maybe prime then it is now definitely prime,
    // add it to the end of the primes vector
    if(maybe_prime){
      obvector_store_at_index(primes, (obj *)candidate, obvector_length(primes));
      numstr = obint_to_string(candidate);
      printf("Prime found: %s\n", obstring_cstring(numstr));
      ob_release((obj *)numstr);
    }

    // increment candidate by 2 and ob_release old candidate, it is not needed
    // by main loop
    next = obint_add_primitive(candidate, 2);
    ob_release((obj *)candidate);
    candidate = next;
  }
}

