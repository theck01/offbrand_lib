
#ifndef OBOBJ_PRIVATE_H
#define OBOBJ_PRIVATE_H

struct OBObj_struct{
  ref_count_t references;
  destroy_fptr dealloc;
};

#endif
