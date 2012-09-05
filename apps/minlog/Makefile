
# Directories
BIN = bin
BIN_CLASS = bin/classes
BIN_TEST = bin/tests
BIN_FUNCT = bin/funct
PUBLIC = include
PRIVATE = include/private
SRC = src
CLASSES = src/classes
TESTS = src/tests
FUNCTS = src/funct

# Compiler Info
CC = gcc
CFLAGS = -Wall -Wextra
OFLAGS = $(CFLAGS) -c

# Executable Dependencies
ALL_DEP = ../../bin/offbrand_stdlib.o
EXE_DEP = $(ALL_DEP) $(BIN_CLASS)/NCube.o $(BIN_CLASS)/Term.o \
					$(BIN_FUNCT)/minlog_funct.o ../../bin/classes/OBVector.o

# Enumerate/Find Objects to build
CLASS_SOURCES := $(wildcard $(CLASSES)/*.c)
ALL_CLASSES = $(patsubst $(CLASSES)/%.c, $(BIN_CLASS)/%.o, $(CLASS_SOURCES))

TEST_SOURCES := $(wildcard $(TESTS)/*.c)
ALL_TESTS = $(patsubst $(TESTS)/%.c, $(BIN_TEST)/%, $(TEST_SOURCES))

FUNCT_SOURCES := $(wildcard $(FUNCTS)/*.c)
ALL_FUNCTS = $(patsubst $(FUNCTS)/%.c, $(BIN_FUNCT)/%.o, $(FUNCT_SOURCES))


# MAIN BUILD
all: $(ALL_CLASSES) $(ALL_TESTS) $(ALL_FUNCTS)

# Functions Build 
$(BIN_FUNCT)/%.o: $(FUNCTS)/%.c $(PUBLIC)/%.h
	$(CC) $(OFLAGS) $< -o $@

# Generic Class Build
$(BIN_CLASS)/%.o: $(CLASSES)/%.c $(PUBLIC)/%.h $(PRIVATE)/%_Private.h
	$(CC) $(OFLAGS) $< -o $@

# NCube_test build
$(BIN_TEST)/NCube_test: $(TESTS)/NCube_test.c $(EXE_DEP)
	$(CC) $(CFLAGS) $^ -o $@

# RTable_test build
$(BIN_TEST)/RTable_test: $(TESTS)/RTable_test.c $(EXE_DEP)
	$(CC) $(CFLAGS) $^ -o $@

# Generic Test Build
$(BIN_TEST)/%_test: $(TESTS)/%_test.c $(BIN_CLASS)/%.o $(ALL_DEP)
	$(CC) $(CFLAGS) $^ -o $@


# OPTIONAL BUILDS
test: clean debug
	@echo "Running tests on minlog data structures after successful compile..."
	@run-parts $(BIN_TEST)

debug: CFLAGS += -g
debug: all

clean:
	rm -f $(ALL_FUNCTS)
	rm -f $(ALL_CLASSES)
	rm -f $(ALL_TESTS)


