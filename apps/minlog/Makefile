
# Directories
BIN = bin
BIN_OBJECTS = bin/objects
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
ALL_DEP = ../../bin/objects/offbrand_stdlib.o
EXE_DEP = $(ALL_DEP) $(BIN_OBJECTS)/NCube.o $(BIN_OBJECTS)/Term.o \
					$(BIN_FUNCT)/minlog_funct.o ../../bin/objects/obvector.o

# Enumerate/Find Objects to build
CLASS_SOURCES := $(wildcard $(CLASSES)/*.c)
ALL_CLASSES = $(patsubst $(CLASSES)/%.c, $(BIN_OBJECTS)/%.o, $(CLASS_SOURCES))

TEST_SOURCES := $(wildcard $(TESTS)/*.c)
ALL_TESTS = $(patsubst $(TESTS)/%.c, $(BIN_TEST)/%, $(TEST_SOURCES))

FUNCT_SOURCES := $(wildcard $(FUNCTS)/*.c)
ALL_FUNCTS = $(patsubst $(FUNCTS)/%.c, $(BIN_FUNCT)/%.o, $(FUNCT_SOURCES))


# MAIN BUILD
all: prepare $(ALL_CLASSES) $(ALL_TESTS) $(ALL_FUNCTS) $(BIN)/minlog

# Functions Build 
$(BIN_FUNCT)/%.o: $(FUNCTS)/%.c $(PUBLIC)/%.h
	$(CC) $(OFLAGS) $< -o $@

# Generic Class Build
$(BIN_OBJECTS)/%.o: $(CLASSES)/%.c $(PUBLIC)/%.h $(PRIVATE)/%_Private.h
	$(CC) $(OFLAGS) $< -o $@

# NCube_test build
$(BIN_TEST)/NCube_test: $(TESTS)/NCube_test.c $(EXE_DEP)
	$(CC) $(CFLAGS) $^ -o $@

# RTable_test build
$(BIN_TEST)/RTable_test: $(TESTS)/RTable_test.c $(EXE_DEP)
	$(CC) $(CFLAGS) $^ -o $@

# Generic Test Build
$(BIN_TEST)/%_test: $(TESTS)/%_test.c $(BIN_OBJECTS)/%.o $(ALL_DEP)
	$(CC) $(CFLAGS) $^ -o $@

# Main program Build
$(BIN)/minlog: $(SRC)/main.c $(EXE_DEP) $(BIN_OBJECTS)/RTable.o
	$(CC) $(CFLAGS) $^ -o $@


# OPTIONAL BUILDS
test: clean debug
	@echo
	@echo "Running tests on minlog data structures after successful compile..."
	@../../scripts/run_bin $(BIN_TEST)

debug: CFLAGS += -g
debug: all

prepare:
	@scripts/prepare

package:
	@scripts/package

clean: prepare
	rm -f $(ALL_FUNCTS)
	rm -f $(ALL_CLASSES)
	rm -f $(ALL_TESTS)
