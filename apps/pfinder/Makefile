
CC=gcc
FLAGS=-Wall -Wextra -g

all: pfinder

pfinder: main.c
	$(CC) $(FLAGS) main.c ../../bin/classes/OBInt.o ../../bin/classes/OBString.o \
		../../bin/classes/OBVector.o ../../bin/offbrand_stdlib.o -o pfinder

clean:
	rm pfinder
