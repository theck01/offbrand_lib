
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
EXE_DEP = ../../bin/offbrand_stdlib.o

# Enumerate/Find Objects to build
CLASS_SOURCES := $(wildcard $(CLASSES)/*.c)
ALL_CLASSES = $(patsubst $(CLASSES)/%.c, $(BIN_CLASS)/%.o, $(CLASS_SOURCES))

TEST_SOURCES := $(wildcard $(TESTS)/*.c)
ALL_TESTS = $(patsubst $(TESTS)/%.c, $(BIN_TEST)/%.o, $(TEST_SOURCES))

FUNCT_SOURCES := $(wildcard $(FUNCTS)/*.c)
ALL_FUNCTS = $(patsubst $(FUNCTS)/%.c, $(BIN_FUNCT)/%.o, $(FUNCT_SOURCES))


# MAIN BUILD
all: $(ALL_FUNCTS) $(ALL_CLASSES) $(ALL_TESTS)

# Build function objects
$(BIN_FUNCT)/%.o: $(FUNCTS)/%.c $(PUBLIC)/%.h
	$(CC) $(OFLAGS) $< -o $@

# Build class objects
$(BIN_CLASS)/%.o: $(CLASSES)/%.c $(PUBLIC)/%.h $(PRIVATE)/%_Private.h
	$(CC) $(OFLAGS) $< -o $@

# Build test executables
$(BIN_TEST)/%_test: $(TESTS)/%_test.c $(BIN_CLASS)/%.o $(EXE_DEP)
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


