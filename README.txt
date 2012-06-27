
OffBrand:

The OffBrand library is a collection of reference counted and thread safe 
generic data structures for the C programming language. The library includes
bash scripts to assist in the creation and testing of classes compatible with
the framework.


File Structure:

The OffBrand library is composed of entirely of incomplete types, to create C 
style "classes". Each class is composed of three files:
  - Public Header
  - Private Header
  - Source File
The public header acts as an interface, any declarations in the public header
can be used by any other module. The private header contains the definitions
of the data stored in the incomplete type, and declarations of all private
functions and methods. The source file contains all the definitions for all 
methods declared in either header.

The private header file should only be included by the class source file, no
other classes should have access. This enforces data protection and forces all
data structure allocation on the heap.


Conventions:

Much like the Linux system interface, OffBrand classes and functions return 0 
when they succeed, and a non zero value when they fail.


OffBrand Compatible Class Creation:

A OffBrand compatible class should be created using the mkobc (MaKe OffBrand
Class) command included in the installation of the OffBrand library. Run the 
mkobc script with the following syntax:
  $ mkobc <class name>
The command will create three files for the class in the current directory with
some basic boiler plate code already included. DO NOT ALTER THE CODE ALREADY
INCLUDED! The code is essential for creating a class that will work with the
OffBrand framework. All additional code should be added to the created source
files in the appropriate locations, indicated by comments in the files.

If additional constructors need be created copy the code provided in the default
constructor and add code in the designated areas as needed.

obj:

obj is the base struct for all OffBrand objects. Casting all OffBrand
compatible instances to obj allows each data structure to be "generic", where
it can hold instances of any OffBrand compatible type. There is no planned
support for heterogenous generic containers (containers that hold data of
different types). While not explicitly prevented in anyway by the code, it is
recommended that the user know what they are doing before using OffBrand
containers in a heterogenous manner. The glaring issue of heterogenous
containers is no way to determine at which class an obj * is pointing.

obj contains the data for reference counts of all objects and a function
pointer to the destructor function of the class. All obj are initialized with
a reference count of 1 when created, and the destructor for the class is called
automatically when the reference count reaches 0.

To modify the reference count the following methods are defined, which take as
an argument a pointer to any OffBrand class cast to obj *

  obj * release(obj *); Decrements reference count by 1 and frees object if
                            reference count equals 0
  void retain(obj *); Increments reference count by 1


Thread Safety:

All data structures included in the OffBrand library are thread-safe. This
thread safety is enforced internally to all classes, the user does not need to
call any particular functions to guarantee that operations will complete as
desired in a multithreaded environment.

All OffBrand compatible classes that are intended to operate in a multi-threaded
environment must implement their own internal thread safety protocol, the
library only protects classes that are pre-existing in the library.
