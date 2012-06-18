
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

# Groups
ALL_DEPEND = $(PUBLIC)/OffBrand.h $(PUBLIC)/OBObj.h

# START BUILD
all_tests: $(BIN_TEST)/OBObj_test $(ALL_DEPEND)

$(BIN_TEST)/OBObj_test: $(BIN_LIB)/OBObj.o $(TESTS)/OBObj_test.c $(ALL_DEPEND)
	$(CC) $(CFLAGS) $(BIN_LIB)/OBObj.o $(TESTS)/OBObj_test.c -o \
		$(BIN_TEST)/OBObj_test

$(BIN_LIB)/OBObj.o: $(SRC)/OBObj.c $(ALL_DEPEND)
	$(CC) $(OFLAGS) $(SRC)/OBObj.c -o $(BIN_LIB)/OBObj.o


# Clean previous build
clean:
	rm -rf $(BIN_LIB)
	mkdir $(BIN_LIB)
	rm -rf $(BIN_TEST)
	mkdir $(BIN_TEST)
