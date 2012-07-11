
# Directories
BIN_LIB = bin/lib
BIN_TEST = bin/tests
PUBLIC = include
PRIVATE = include/private
SRC = src
TESTS = src/tests

# Compiler Info
CC = gcc
CFLAGS = -Wall -Wextra
OFLAGS = $(CFLAGS) -c

# Find Dependencies
OBJ_SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(BIN_LIB)/%.o, $(OBJ_SOURCES))

TEST_SOURCES := $(wildcard $(TESTS)/*.c)
TEST_EXE = $(patsubst $(TESTS)/%.c, $(BIN_TEST)/%, $(TEST_SOURCES))

# START BUILD
all: $(OBJECTS) $(TEST_EXE)

# Build class objects
$(BIN_LIB)/%.o: $(SRC)/%.c
	$(CC) $(OFLAGS) $^ -o $@

# Build tests
$(BIN_TEST)/%_test: $(TESTS)/%_test.c $(BIN_LIB)/%.o  $(BIN_LIB)/OBTest.o
	$(CC) $(CFLAGS) $^ -o $@


# Clean previous build
clean:
	rm -rf $(BIN_LIB)
	mkdir $(BIN_LIB)
	rm -rf $(BIN_TEST)
	mkdir $(BIN_TEST)

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
	@echo
	@echo "Objects: $(OBJECTS)"
	@echo "Tests: $(TEST_EXE)"
