
#ifndef REF_PRIVATE_H
#define REF_PRIVATE_H

struct obj_struct{
  ref_count_t references;
  destroy_fptr dealloc;
};

#endif
