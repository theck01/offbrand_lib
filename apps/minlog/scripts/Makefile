
# Makefile for the minlog logic minimization application, which uses code from
# the Offbrand library of generic C data structures.

# Directories
BIN = bin
BIN_CLASS = bin/classes
BIN_FUNCT = bin/funct
PUBLIC = include
PRIVATE = include/private
SRC = src
CLASSES = src/classes
FUNCTS = src/funct

# Compiler Info
CC = gcc
CFLAGS = -Wall -Wextra #Common flags for all
OFLAGS = $(CFLAGS) -c  #Flags for .o output files

# Find all classes, tests, and functions to build
CLASS_SOURCES := $(wildcard $(CLASSES)/*.c)
ALL_CLASSES = $(patsubst $(CLASSES)/%.c, $(BIN_CLASS)/%.o, $(CLASS_SOURCES))

FUNCT_SOURCES := $(wildcard $(FUNCTS)/*.c)
ALL_FUNCTS = $(patsubst $(FUNCTS)/%.c, $(BIN_FUNCT)/%.o, $(FUNCT_SOURCES))

# MAIN BUILD
all: $(ALL_FUNCTS) $(ALL_CLASSES) $(BIN)/minlog

# offbrand_stdlib build
$(BIN_FUNCT)/offbrand_stdlib.o: $(FUNCTS)/offbrand_stdlib.c $(PUBLIC)/offbrand.h
	$(CC) $(OFLAGS) $< -o $@

# Functions Build
$(BIN_FUNCT)/%.o: $(FUNCTS)/%.c $(PUBLIC)/%.h
	$(CC) $(OFLAGS) $< -o $@

# Minlog classes Build
$(BIN_CLASS)/%.o: $(CLASSES)/%.c $(PUBLIC)/%.h $(PRIVATE)/%_Private.h
	$(CC) $(OFLAGS) $< -o $@

# offbrand classes Build
$(BIN_CLASS)/%.o: $(CLASSES)/%.c $(PUBLIC)/%.h $(PRIVATE)/%_private.h
	$(CC) $(OFLAGS) $< -o $@

# main program build
$(BIN)/minlog: $(SRC)/main.c $(ALL_FUNCTS) $(ALL_CLASSES)
	$(CC) $(CFLAGS) $^ -o $@

# CLEAN BUILD
clean:
	rm -f $(ALL_FUNCTS)
	rm -f $(ALL_CLASSES)
	rm -f $(BIN)/minlog

