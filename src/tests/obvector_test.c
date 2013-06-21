/**
 * @file obvector_test.c
 * @brief obvector Unit Tests
 * @author theck
 */

#include "../../include/offbrand.h"
#include "../../include/obvector.h"
#include "../../include/private/obvector_private.h" /* For testing purposes
                                                       only */
#include "../../include/obtest.h"

/** main unit testing routine */
int main(){

  uint32_t i, id;
  obtest *tests[6], *singleton, *tmp;
  obvector *main_vec, *copy_vec;

  main_vec = obvector_new(1);
  singleton = obtest_new(7);

  for(i=0; i<6; i++){
    tests[i] = obtest_new(i);
    obvector_store_at_index(main_vec, (obj *)tests[i], i);

    if(ob_reference_count((obj *)tests[i]) != 2){
      fprintf(stderr, "obvector_test: vector did not properly ob_retain element "
                      "TEST FAILED\n");
      exit(1);
    }

    ob_release((obj *)tests[i]); /* vector should maintain only references */
  }

  if(obvector_length(main_vec) != 6){
    fprintf(stderr, "obvector_test: vector size is off, TEST FAILED\n");
    exit(1);
  }

  for(i=0; i<6; i++){
    if(obtest_id((obtest *)obvector_obj_at_index(main_vec,i)) != i){
      fprintf(stderr, "obvector_test: vector does not contain added contents "
                      "in expected order\nTEST FAILED\n");
      printf("%d\n", i);
      exit(1);
    }
  }

  obvector_store_at_index(main_vec, (obj *)singleton, 12);
  if(obvector_length(main_vec) != 13){
    fprintf(stderr, "obvector_test: store did not properly extend length "
                    "TEST FAILED\n");
    exit(1);
  }


  obvector_store_at_index(main_vec, NULL, 12);
  if(obvector_length(main_vec) != 6){
    fprintf(stderr, "obvector_test: remove did not properly get rid of element "
                    "TEST FAILED\n");
    exit(1);
  }

  if(ob_reference_count((obj *)singleton) != 1){
    fprintf(stderr, "obvector_test: vector did not ob_release element on removal "
                    "TEST FAILED\n");
    exit(1);
  }

  tmp = (obtest *)obvector_obj_at_index(main_vec, 3);
  ob_retain((obj *)tmp);

  obvector_store_at_index(main_vec, (obj *)singleton, 3);
  if(ob_reference_count((obj *)tmp) != 1){
    fprintf(stderr, "obvector_test: vector did not ob_release element on "
                    "replacement, TEST FAILED\n");
    exit(1);
  }

  if(obvector_length(main_vec) != 6 ||
     obtest_id((obtest *)obvector_obj_at_index(main_vec, 3)) !=
     obtest_id(singleton)){
    fprintf(stderr, "obvector_test: element was not added properly to vector "
                    "at index\nTEST FAILED\n");
    exit(1);
  }

  obvector_store_at_index(main_vec, (obj *)tmp, 3);

  /* reduce reference count to 1, so only vector holds reference to obj */
  ob_release((obj *)tmp);

  if(obvector_find_obj(main_vec, (obj *)singleton)){
    fprintf(stderr, "obvector_test: vector did not replace element correctly, "
                    "or was not found\nTEST FAILED\n");
    exit(1);
  }

  obvector_store_at_index(main_vec, (obj *)singleton, 10);
  if(obvector_length(main_vec) != 11){
    fprintf(stderr, "obvector_test: store did not properly extend length "
                    "TEST FAILED\n");
    exit(1);
  }

  obvector_sort(main_vec, OB_LEAST_TO_GREATEST);
  if(obvector_length(main_vec) != 7){
    fprintf(stderr, "obvector_test: sort did not properly remove NULLs "
                    "TEST FAILED\n");
    fprintf(stderr, "obvector_test: vector size == %d\n",
                    obvector_length(main_vec));
    for(i=0; i<obvector_length(main_vec); i++){
      if(obvector_obj_at_index(main_vec, i)) printf("object\n");
      else printf("NULL\n");
    }
    exit(1);
  }

  id = 0;
  for(i=0; i<obvector_length(main_vec); i++){
    if(id > obtest_id((obtest *)obvector_obj_at_index(main_vec, i))){
      fprintf(stderr, "obvector_test: vector was not sorted least to "
                      "greatest, TEST FAILED\n");
      exit(1);
    }
    id = obtest_id((obtest *)obvector_obj_at_index(main_vec, i));
  }

  obvector_store_at_index(main_vec, NULL, 6);
  obvector_store_at_index(main_vec, (obj *)singleton, 10);

  obvector_sort(main_vec, OB_GREATEST_TO_LEAST);
  if(obvector_length(main_vec) != 7){
    fprintf(stderr, "obvector_test: second sort did not properly remove NULLs "
                    "TEST FAILED\n");
    exit(1);
  }

  id = 10;
  for(i=0; i<obvector_length(main_vec); i++){
    if(id < obtest_id((obtest *)obvector_obj_at_index(main_vec, i))){
      fprintf(stderr, "obvector_test: vector was not sorted greatest to "
                      "least, TEST FAILED\n");
      exit(1);
    }
    id = obtest_id((obtest *)obvector_obj_at_index(main_vec, i));
  }

  copy_vec = obvector_copy(main_vec);

  assert(ob_compare((obj *)copy_vec, (obj *)main_vec) == OB_EQUAL_TO);
  assert(ob_hash((obj *)copy_vec) == ob_hash((obj *)main_vec));

  ob_release((obj *)singleton); /* ob_release singleton once so it is only referenced
                                by containing vectors */

  for(i=0; i<obvector_length(main_vec); i++){
    if(ob_reference_count((obj *)(obtest *)obvector_obj_at_index(main_vec, i)) != 2){
      fprintf(stderr, "obvector_test: vector contents not ob_retained on copy, "
                      "TEST FAILED\n");
      exit(1);
    }
  }

  /* get obj at index and ob_retain it so it isnt deallocated */
  tmp = (obtest *)obvector_obj_at_index(main_vec, 3);
  ob_retain((obj *)tmp);

  obvector_store_at_index(main_vec, NULL, 3);

  assert(ob_compare((obj *)copy_vec, (obj *)main_vec) == OB_NOT_EQUAL);
  assert(ob_hash((obj *)copy_vec) != ob_hash((obj *)main_vec));

  if(obvector_find_obj(main_vec, (obj *)tmp)){
    fprintf(stderr, "obvector_test: Item was not removed properly from vector"
                    "index, TEST FAILED\n");
    exit(1);
  }

  obvector_store_at_index(main_vec, (obj *)tmp, 3);
  assert(obvector_length(main_vec) == 7);
  obvector_concat(main_vec, copy_vec);
  assert(obvector_length(main_vec) == 14);

  for(i=0; i<obvector_length(main_vec)/2; i++){
    if(obtest_id((obtest *)obvector_obj_at_index(main_vec,i)) !=
       obtest_id((obtest *)obvector_obj_at_index(main_vec,i+7))){
      fprintf(stderr, "obvector_test: Copy of vector not appended properly, "
                      "TEST FAILED\n");
      exit(1);
    }
  }

  ob_release((obj *)main_vec);
  ob_release((obj *)copy_vec);

  if(ob_reference_count((obj *)tmp) != 1){
    fprintf(stderr, "obvector_test: ob_releases of obvector container did not "
                    "properly manage\ncontained obtest reference count, TEST "
                    "FAILED\n");
    exit(1);
  }

  ob_release((obj *)tmp);

  printf("obvector: TESTS PASSED\n");
  return 0;
}
