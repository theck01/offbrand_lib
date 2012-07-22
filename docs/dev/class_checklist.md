
The following guidelines should be followed when writing public methods for a 
new Offbrand class:

* Check that arguments take ranges that are expected, and gracefully handle
  exceptions to the expected.
* Check that all return values (system, Offbrand, any lib...) are valid.
* Check that all parameters stay within defined ranges.

Private methods should follow the same guidelines, although argument checking
may be ommited, as arguments should be properly checked in public methods before
they would be provided to the private methods.
