
minlog - A boolean logic minimization application. Program takes as an input a
boolean function in minterm or maxterm form, and outputs the resulting function
in minimal product of sums or sum of products form. minlog is based on the 
Quine-McCuskey method for minimizing boolean functions.

To build and run the minlog application, run the commands:
$ make
$ bin/minlog

The bin/minlog command will output the usage of the minlog app. To run the
actual version run:
$ bin/minlog [boolean equation]

NOTE:
The coding style of this application does not follow the standard style
of the offbrand library.
