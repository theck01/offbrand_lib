
#ifndef OBJ_PRIVATE_H
#define OBJ_PRIVATE_H

struct obj_struct{
  ref_count_t references;
  dealloc_fptr dealloc;
};

#endif
