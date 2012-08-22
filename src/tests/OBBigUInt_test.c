
#include "../../include/offbrand.h"
#include "../../include/OBBigUInt.h"
#include "../../include/OBTest.h"

/* UINT32_MAX = 4294967295 */

int main (){

  /* various OBBigUInts needed for testing */
  OBBigUInt *two, *zero, *a, *b, *c, *add_result, *sub_result, *mult_result;
  OBBigUInt *div_result, *mod_result, *shift_left_result, *shift_right_result;
//  OBBigUInt *and_result, *or_result;
//  OBBigUInt *xor_result, *not_result;

  uint32_t num1[3] = {1234567890, 987654321, 2910};
  uint32_t num2[3] = {2143658709, 4162738490, 982647};

  two = createBigUIntFromNum(2);
  zero = createBigUIntFromNum(0);
  if(!two || !zero){
    fprintf(stderr, "OBBigUInt_test: Could not allocate two or zero, TEST "
                    "FAILED\n");
    exit(1);
  }

  a = createBigUIntFromIntArray(num1, 3);
  b = createBigUIntFromIntArray(num2, 3);
  c = createBigUIntFromIntArray(num1, 3);
  if(!a || !b || !c){
    fprintf(stderr, "OBBigUInt_test: Could not allocate a or b, TEST FAILED\n");
    exit(1);
  }

  if(compareBigUInts((obj *)a, (obj *)b) != OB_LESS_THAN){
    fprintf(stderr, "OBBigUInt_test: Less than comparision failed, "
                    "TEST FAILED\n");
    exit(1);
  }

  if(compareBigUInts((obj *)b, (obj *)c) != OB_GREATER_THAN){
    fprintf(stderr, "OBBigUInt_test: Greater than comparision failed, "
                    "TEST FAILED\n");
    exit(1);
  }

  if(compareBigUInts((obj *)a,(obj *)c) != OB_EQUAL_TO){
    fprintf(stderr, "OBBigUInt_test: Equal to comparision failed, "
                    "TEST FAILED\n");
    exit(1);
  }

  /* release uneeded object */
  release((obj *)c);

  add_result = addBigUInts(a, b);
  if(!add_result){
    fprintf(stderr, "OBBigUInt_test: Addition returns NULL, TEST FAILED\n");
    exit(1);
  }

  sub_result = subtractBigUInts(add_result, a);
  if(!sub_result){
    fprintf(stderr, "OBBigUInt_test: Subtraction returns NULL, TEST FAILED\n");
    exit(1);
  }

  if(compareBigUInts((obj *)b, (obj *)sub_result) != OB_EQUAL_TO){
    fprintf(stderr, "OBBigUInt_test: addition and subtraction did not form "
                    "inverse operations\nTEST FAILED\n");
    exit(1);
  }

  shift_left_result = shiftBigUIntLeft(a, 4, 24);
  if(!shift_left_result){
    fprintf(stderr, "OBBigUInt_test: Left shift returns NULL, TEST FAILED\n");
    exit(1);
  }

  shift_right_result = shiftBigUIntRight(shift_left_result,4,24);
  if(!shift_right_result){
    fprintf(stderr, "OBBigUInt_test: Right shift returns NULL, TEST FAILED\n");
    exit(1);
  }

  if(compareBigUInts((obj *)a, (obj *)shift_right_result) != OB_EQUAL_TO){
    fprintf(stderr, "OBBigUInt_test: Right and left shift are not inverse "
                    "operations, TEST FAILED\n");
    exit(1);
  }

  mult_result = multiplyBigUInts(a,b);
  if(!mult_result){
    fprintf(stderr, "OBBigUInt_test: Multiply returns NULL, TEST FAILED\n");
    exit(1);
  }

  div_result = divideBigUInts(mult_result, b);
  if(!div_result){
    fprintf(stderr, "OBBigUInt_test: Divide returns NULL, TEST FAILED\n");
    exit(1);
  }

  printf("\n\n\nA\n");
  printBigUInt(a);
  printf("\n\nB\n");
  printBigUInt(b);
  printf("\n\nMult Result\n");
  printBigUInt(mult_result);
  printf("\n\nDiv Result\n");
  printBigUInt(div_result);
  printf("\n\n\n");

  if(compareBigUInts((obj *)a, (obj *)div_result) != OB_EQUAL_TO){
    fprintf(stderr, "OBBigUInt_test: Divided and multiply are not inverse "
                    "operations, TEST FAILED\n");
    exit(1);
  }
/*
  c = addBigUInts(mult_result, a);
  if(!c){
    fprintf(stderr, "OBBigUInt_test: Could not create secondary c, TEST "
                    "FAILED\n");
    exit(1);
  }

  mod_result = modBigUInts(c, b);
  if(!mod_result){
    fprintf(stderr, "OBBigUInt_test: Mod returns NULL, TEST FAILED\n");
    exit(1);
  }

  if(compareBigUInts((obj *)a, (obj *)mod_result) != OB_EQUAL_TO){
    fprintf(stderr, "OBBigUInt_test: Mod operation did not return expected "
                    "value, TEST FAILED\n");
    exit(1);
  }
  */

  printf("OBBigUInt_test: TESTS PASSED\n");
  return 0;
}
