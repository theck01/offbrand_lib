
#ifndef OBLOCK_PRIVATE_H
#define OBLOCK_PRIVATE_H

#include <pthread.h>

struct OBLock_struct{
  pthread_mutex_t mutex;
  uint32_t reads_waiting;
  uint32_t reads_active;
  uint32_t writes_waiting;
  uint8_t write_active;
  pthread_cond_t read_rdy;
  pthread_cond_t write_rdy;
};

/* PRIVATE METHOD */
void initLock(OBLock *to_init);
void deallocLock(OBLock *to__dealloc);

#endif
