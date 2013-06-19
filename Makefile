
# Directories
BIN = bin
BIN_OBJECT = bin/objects
BIN_TEST = bin/tests
DOCS = docs/doxygen
LIB_ARCHIVE = $(BIN)/offbrand.a
PUBLIC = include
PRIVATE = include/private
SRC = src
CLASSES = src/classes
TESTS = src/tests

# Compiler Info
AR = ar
ARFLAGS = rvs
CC = gcc
CFLAGS = -Wall -Wextra -g #Common flags for all
OFLAGS = $(CFLAGS) -c	 #Flags for .o output files
# common dependencies for many classes/tests
TEST_DEP = $(LIB_ARCHIVE)

# Enumerate/Find Objects to build
STD_LIBS = $(BIN_OBJECT)/offbrand_stdlib.o 

DOC_FILES := $(wildcard $(DOCS)/*.dox)
PUBLIC_HEADERS := $(wildcard $(PUBLIC)/*.h)
PRIVATE_HEADERS := $(wildcard $(PRIVATE)/*.h)

CLASS_SOURCES := $(wildcard $(CLASSES)/*.c)
ALL_CLASSES = $(patsubst $(CLASSES)/%.c, $(BIN_OBJECT)/%.o, $(CLASS_SOURCES))

TEST_SOURCES := $(wildcard $(TESTS)/*.c)
ALL_TESTS = $(patsubst $(TESTS)/%.c, $(BIN_TEST)/%, $(TEST_SOURCES))


# MAIN BUILD
all: prepare $(STD_LIBS) $(ALL_CLASSES)	$(ALL_TESTS) $(LIB_ARCHIVE)
	@echo 

# Hand builds (STD_LIBS)
$(BIN_OBJECT)/offbrand_stdlib.o: $(SRC)/offbrand_stdlib.c $(PUBLIC)/offbrand.h
	$(CC) $(OFLAGS) $< -o $@

# Build class objects
$(BIN_OBJECT)/%.o: $(CLASSES)/%.c $(PUBLIC)/%.h $(PRIVATE)/%_private.h
	$(CC) $(OFLAGS) $< -o $@

# Build tests executables (special builds encountered first)
$(BIN_TEST)/%_test: $(TESTS)/%_test.c $(TEST_DEP)
	$(CC) $(CFLAGS) $^ -o $@

# Build library archive
$(LIB_ARCHIVE): $(ALL_CLASSES) $(STD_LIBS)
	$(AR) $(ARFLAGS) $@ $^


# OPTIONAL BUILDS
# Clean previous build
clean: prepare
	rm -f $(STD_LIBS)
	rm -f $(ALL_TESTS)
	rm -f $(ALL_CLASSES)
	rm -f $(LIB_ARCHIVE)

# build documentation with the optional doxygen dependency
documentation: 
	@doxygen docs/doxygen.conf

# Compile the library from scratch and run tests
fresh: clean all test

# Prepare bin/ output directories
prepare:
	@scripts/prepare

# Print information about configuration
print:
	@echo "BIN: $(BIN)"
	@echo "BIN_OBJECT: $(BIN_OBJECT)"
	@echo "BIN_TEST: $(BIN_TEST)"
	@echo "DOCS: $(DOCS)"
	@echo "LIB_ARCHIVE: $(LIB_ARCHIVE)"
	@echo "PUBLIC: $(PUBLIC)"
	@echo "PRIVATE: $(PRIVATE)"
	@echo "SRC: $(SRC)"
	@echo "CLASSES: $(CLASSES)"
	@echo "TESTS: $(TESTS)"
	@echo
	@echo "Archiver: $(AR)"
	@echo "ARFLAGS: $(ARFLAGS)"
	@echo "Compiler: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "OFLAGS: $(OFLAGS)"
	@echo
	@echo "DOC FILES:"
	@echo "$(DOC_FILES)"
	@echo
	@echo "PUBLIC HEADERS:"
	@echo "$(PUBLIC_HEADERS)"
	@echo
	@echo "PRIVATE HEADERS:"
	@echo "$(PRIVATE_HEADERS)"
	@echo
	@echo "CLASS FILES:"
	@echo "$(CLASS_SOURCES)"
	@echo
	@echo "TEST FILES:"
	@echo "$(TEST_SOURCES)"

# Run all test scripts
test: 
	@echo "Running all data strucutres tests..."
	@echo 
	@scripts/run_bin $(BIN_TEST)
