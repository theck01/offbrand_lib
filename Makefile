
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
CFLAGS = -Wall -Wextra #Common flags for all
OFLAGS = $(CFLAGS) -c	 #Flags for .o output files
TFLAGS = -lpthread -D OB_THREADED #Extra flags for threaded programs

# common dependencies for many classes/tests
TEST_DEP = $(BIN)/offbrand_stdlib.o $(BIN_CLASS)/OBTest.o
THREAD_DEP = $(BIN)/offbrand_t_stdlib.o $(BIN)/offbrand_threadlib.o \
						 $(BIN_CLASS)/OBTest.o

# Enumerate/Find Objects to build
STD_LIBS = $(BIN)/offbrand_stdlib.o $(BIN)/offbrand_t_stdlib.o \
					 $(BIN)/offbrand_threadlib.o

CLASS_SOURCES := $(wildcard $(CLASSES)/*.c)
ALL_CLASSES = $(patsubst $(CLASSES)/%.c, $(BIN_CLASS)/%.o, $(CLASS_SOURCES))

TEST_SOURCES := $(wildcard $(TESTS)/*.c)
ALL_TESTS = $(patsubst $(TESTS)/%.c, $(BIN_TEST)/%, $(TEST_SOURCES))


# MAIN BUILD
all: prepare $(STD_LIBS) $(ALL_CLASSES)	$(ALL_TESTS)

# Hand builds (STD_LIBS)
$(BIN)/offbrand_stdlib.o: $(SRC)/offbrand_stdlib.c $(PUBLIC)/offbrand.h
	$(CC) $(OFLAGS) $< -o $@
$(BIN)/offbrand_t_stdlib.o: $(SRC)/offbrand_stdlib.c $(PUBLIC)/offbrand.h
	$(CC) $(OFLAGS) $(TFLAGS) $< -o $@
$(BIN)/offbrand_threadlib.o: $(SRC)/offbrand_threadlib.c \
	$(PUBLIC)/offbrand_threaded.h
	$(CC) $(OFLAGS) $(TFLAGS) $< -o $@

# Build class objects
$(BIN_CLASS)/%.o: $(CLASSES)/%.c $(PUBLIC)/%.h $(PRIVATE)/%_Private.h
	$(CC) $(OFLAGS) $< -o $@

# Build tests executables
$(BIN_TEST)/%_test: $(TESTS)/%_test.c $(BIN_CLASS)/%.o $(TEST_DEP)
	$(CC) $(CFLAGS) $^ -o $@


# OPTIONAL BUILDS
test: clean debug
	@echo
	@echo "Running tests on data structures after successful compilation..."
	@run-parts $(BIN_TEST)

debug: CFLAGS += -g
debug: all

# Prepare bin/ output directories
prepare:
	@scripts/prepare

# Clean previous build
clean: prepare
	rm -f $(STD_LIBS)
	rm -f $(ALL_TESTS)
	rm -f $(ALL_CLASSES)

print:
	@echo "BIN_LIB: $(BIN_LIB)"
	@echo "BIN_TEST: $(BIN_TEST)"
	@echo "PUBLIC: $(PUBLIC)"
	@echo "PRIVATE: $(PRIVATE)"
	@echo "SRC: $(SRC)"
	@echo "TESTS: $(TESTS)"
	@echo
	@echo "Compiler: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "OFLAGS: $(OFLAGS)"
	@echo "TFLAGS: $(TFLAGS)"
	@echo
	@echo "Classes: $(ALL_CLASSES)"
	@echo "Tests: $(ALL_TESTS)"
