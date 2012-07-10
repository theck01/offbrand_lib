
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
ALL_DEPEND = $(PUBLIC)/offbrand.h $(PUBLIC)/obj.h
BIN_DEPEND = $(BIN_LIB)/obj.o $(BIN_LIB)/offbrand_stdlib.o

# START BUILD
all_tests: $(BIN_TEST)/obj_test

$(BIN_TEST)/obj_test: $(ALL_DEPEND) $(BIN_DEPEND) $(BIN_LIB)/OBTest.o \
	$(TESTS)/obj_test.c
	$(CC) $(CFLAGS) $(BIN_LIB)/obj.o $(BIN_LIB)/OBTest.o $(TESTS)/obj_test.c -o \
		$(BIN_TEST)/obj_test

$(BIN_LIB)/OBTest.o: $(ALL_DEPEND) $(BIN_DEPEND) $(SRC)/OBTest.c  \
	$(PUBLIC)/OBTest.h $(PRIVATE)/OBTest_Private.h
	$(CC) $(OFLAGS) $(SRC)/OBTest.c -o $(BIN_LIB)/OBTest.o

$(BIN_LIB)/offbrand_stdlib.o: $(ALL_DEPEND) \
	$(SRC)/offbrand_stdlib.c
	$(CC) $(OFLAGS) $(SRC)/offbrand_stdlib.c -o $(BIN_LIB)/offbrand_stdlib.o

$(BIN_LIB)/obj.o: $(ALL_DEPEND) $(SRC)/obj.c $(PRIVATE)/obj_private.h
	$(CC) $(OFLAGS) $(SRC)/obj.c -o $(BIN_LIB)/obj.o

# Clean previous build
clean:
	rm -rf $(BIN_LIB)
	mkdir $(BIN_LIB)
	rm -rf $(BIN_TEST)
	mkdir $(BIN_TEST)
