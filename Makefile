
# Directories
BIN = bin
BIN_CLASS = bin/classes
BIN_TEST = bin/tests
PUBLIC = include
PRIVATE = include/private
SRC = src
CLASSES = src/classes
TESTS = src/tests

# Compiler Info
CC = gcc
CFLAGS = -Wall -Wextra -g #Common flags for all
OFLAGS = $(CFLAGS) -c	 #Flags for .o output files

# common dependencies for many classes/tests
TEST_DEP = $(BIN)/offbrand_stdlib.o $(BIN_CLASS)/OBTest.o

# Enumerate/Find Objects to build
STD_LIBS = $(BIN)/offbrand_stdlib.o 

CLASS_SOURCES := $(wildcard $(CLASSES)/*.c)
ALL_CLASSES = $(patsubst $(CLASSES)/%.c, $(BIN_CLASS)/%.o, $(CLASS_SOURCES))

TEST_SOURCES := $(wildcard $(TESTS)/*.c)
ALL_TESTS = $(patsubst $(TESTS)/%.c, $(BIN_TEST)/%, $(TEST_SOURCES))


# MAIN BUILD
all: prepare $(STD_LIBS) $(ALL_CLASSES)	$(ALL_TESTS)
	@echo 

# Hand builds (STD_LIBS)
$(BIN)/offbrand_stdlib.o: $(SRC)/offbrand_stdlib.c $(PUBLIC)/offbrand.h
	$(CC) $(OFLAGS) $< -o $@

# Build class objects
$(BIN_CLASS)/%.o: $(CLASSES)/%.c $(PUBLIC)/%.h $(PRIVATE)/%_Private.h
	$(CC) $(OFLAGS) $< -o $@

# Build tests executables (special builds encountered first)
$(BIN_TEST)/OBInt_test: $(TESTS)/OBInt_test.c $(BIN_CLASS)/OBInt.o \
	$(BIN_CLASS)/OBString.o $(BIN_CLASS)/OBVector.o $(TEST_DEP)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_TEST)/OBMap_test: $(TESTS)/OBMap_test.c $(BIN_CLASS)/OBMap.o \
	$(BIN_CLASS)/OBDeque.o $(BIN_CLASS)/OBVector.o $(TEST_DEP)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_TEST)/OBString_test: $(TESTS)/OBString_test.c $(BIN_CLASS)/OBString.o \
	$(BIN_CLASS)/OBVector.o $(TEST_DEP)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_TEST)/%_test: $(TESTS)/%_test.c $(BIN_CLASS)/%.o $(TEST_DEP)
	$(CC) $(CFLAGS) $^ -o $@


# OPTIONAL BUILDS
# Clean previous build
clean: prepare
	rm -f $(STD_LIBS)
	rm -f $(ALL_TESTS)
	rm -f $(ALL_CLASSES)

# Compile the library from scratch and run tests
fresh: clean all test

# Prepare bin/ output directories
prepare:
	@scripts/prepare

# Print information about configuration
print:
	@echo "BIN: $(BIN)"
	@echo "BIN_CLASS: $(BIN_CLASS)"
	@echo "BIN_TEST: $(BIN_TEST)"
	@echo "PUBLIC: $(PUBLIC)"
	@echo "PRIVATE: $(PRIVATE)"
	@echo "SRC: $(SRC)"
	@echo "CLASSES: $(CLASSES)"
	@echo "TESTS: $(TESTS)"
	@echo
	@echo "Compiler: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "OFLAGS: $(OFLAGS)"
	@echo
	@echo "CLASS FILES:"
	@echo "$(CLASS_SOURCES)"
	@echo "TEST FILES:"
	@echo "$(TEST_SOURCES)"

# Run all test scripts
test: 
	@echo "Running all data strucutres tests..."
	@echo 
	@scripts/run_bin $(BIN_TEST)
