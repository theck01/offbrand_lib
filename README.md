# Offbrand:

The Offbrand library is a collection of reference counted generic data 
structures with hooks for thread synchronization implemented in the C 
programming language. The library includes bash scripts to assist in the
creation and testing of classes compatible with the framework.

## Requirements:

The Offbrand library requires a reasonably current version of GCC and make to be
installed on the users system. The library also utilizes the pthread library for
threading, so the user must be running a system with pthreads installed as well.

Windows is currently unsupported.

## Documentaion:

All external documentation can be found in the doc/ subdirectory. "doc/info.txt"
provides an outline of the library and gives insight into its design. The best
source for information on standard libraries and classes are the relevant header
files ("include/offbrand.h" and "include/offbrand_threaded." contain information
on the standard libraries and threaded libraries respectivly). Comments in these
files are intended to provide all of the information desired about user facing
APIs.


## Contact

Send any suggestions, bug reports, etc to:
tyler.heck@gmail.com
