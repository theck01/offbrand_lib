
#include "../include/offbrand_threaded.h"
#include "../include/private/obj_Private.h"

void initLock(OBLock *to_init){

  if(pthread_mutex_init(&(to_init->mutex), NULL) || 
     pthread_cond_init(&(to_init->read_rdy), NULL) ||
     pthread_cond_init(&(to_init->write_rdy), NULL)){
    perror("initLock");
    exit(1);
  }

  return;
}

void readLock(obj *to_read_lock){

  assert(to_read_lock != NULL);

  /* Lock the OBLock mutex to change shared lock data */
  if(pthread_mutex_lock(&((*to_read_lock)->lock.mutex))){
    perror("readLock pthread_mutex_lock\n");
    exit(1);
  }

  /* Wait (block) if a write is currently happening, or if a write is waiting to
   * happen (new reads yeild to one write before obtaining lock) */
  if((*to_read_lock)->lock.write_active > 0 || 
     (*to_read_lock)->lock.writes_waiting > 0){
    
    /* mark as waiting */
    (*to_read_lock)->lock.reads_waiting++;

    /* make sure that when signal is resumed there are no writes happening.
     * Threads waiting on writes no longer matter */
    while((*to_read_lock)->lock.write_active){
      pthread_cond_wait(&((*to_read_lock)->lock.read_rdy),
                        &((*to_read_lock)->lock.mutex));
    }

    /* read is ready, thread is no longer waiting on read */
    (*to_read_lock)->lock.reads_waiting--;
  }

  /* thread now has active read access */
  (*to_read_lock)->lock.reads_active++;

  if(pthread_mutex_unlock(&((*to_read_lock)->lock.mutex))){
    perror("readLock pthread_mutex_unlock\n");
    exit(1);
  }

  return; 
}


void readUnlock(obj *to_read_unlock){

  uint8_t signal_write = 0; /* booleans to signal writes and reads at end of */
  uint8_t signal_read = 0;  /* readUnlock */

  assert(to_read_unlock != NULL);

  /* Lock the OBLock mutex to change shared lock data */
  if(pthread_mutex_lock(&((*to_read_unlock)->lock.mutex))){
    perror("readUnlock pthread_mutex_lock\n");
    exit(1);
  }

  /* check count for error */
  assert((*to_read_unlock)->lock.reads_active != 0);

  /* if thread was last thread with read lock */
  if(--((*to_read_unlock)->lock.reads_active) == 0){
      /* attempt to signal writes first, reads if no writes are waiting */
      if((*to_read_unlock)->lock.writes_waiting > 0) signal_write = 1;
      else if((*to_read_unlock)->lock.reads_waiting > 0) signal_read = 1;
  }

  
  if(pthread_mutex_unlock(&((*to_read_unlock)->lock.mutex))){
    perror("readUnlock pthread_mutex_unlock\n");
    exit(1);
  }

  /* if should signal waiting threads */
  if(signal_write) pthread_cond_signal(&((*to_read_unlock)->lock.write_rdy));
  else if(signal_read) 
    pthread_cond_broadcast(&((*to_read_unlock)->lock.read_rdy));

  return;
}


void writeLock(obj *to_write_lock){

  assert(to_write_lock != NULL); 

  /* Lock the OBLock mutex to change shared lock data */
  if(pthread_mutex_lock(&((*to_write_lock)->lock.mutex))){
    perror("writeLock pthread_mutex_lock\n");
    exit(1);
  }

  /* increment waiting before check */
  (*to_write_lock)->lock.writes_waiting++;

  /* check for active threads. if none are active immediatly obtain write 
   * lock */
  while((*to_write_lock)->lock.write_active > 0 || 
        (*to_write_lock)->lock.reads_active > 0 ){
    /* wait for write ready signal. BLOCKS */
    pthread_cond_wait(&((*to_write_lock)->lock.write_rdy),
                      &((*to_write_lock)->lock.mutex));
  }

  /* update counts */
  (*to_write_lock)->lock.writes_waiting--;
  (*to_write_lock)->lock.write_active = 1;

  /* unlock the OBLock mutex to change shared lock data */
  if(pthread_mutex_unlock(&((*to_write_lock)->lock.mutex))){
    perror("writeLock pthread_mutex_unlock\n");
    exit(1);
  }
 
  return;
}


void writeUnlock(obj *to_write_unlock){

  uint8_t signal_read = 0;  /* Boolean indicating whether any threads waiting */
  uint8_t signal_write = 0; /* should be signlaed */

  assert(to_write_unlock != NULL);

  /* Lock the mutex to change shared lock data */
  if(pthread_mutex_lock(&((*to_write_unlock)->lock.mutex))){
    perror("writeUnlock pthread_mutex_lock\n");
    exit(1);
  }

  assert((*to_write_unlock)->lock.write_active != 0);

  (*to_write_unlock)->lock.write_active = 0;

  /* if reads are waiting flag that they should be signaled. If no reads then
   * check and flag if writes should be signaled */
  if((*to_write_unlock)->lock.reads_waiting) signal_read = 1;
  else if((*to_write_unlock)->lock.writes_waiting) signal_write = 1;

  if(pthread_mutex_unlock(&((*to_write_unlock)->lock.mutex))){
    perror("writeUnlock pthread_mutex_unlock\n");
    exit(1);
  }

  /* if should signal waiting threads */
  if(signal_read) pthread_cond_broadcast(&((*to_write_unlock)->lock.read_rdy));
  else if(signal_write)
    pthread_cond_signal(&((*to_write_unlock)->lock.write_rdy));
  
  return;
}


void deallocLock(OBLock *to_dealloc){

  if(pthread_mutex_destroy(&(to_dealloc->mutex)) ||
     pthread_cond_destroy(&(to_dealloc->read_rdy)) ||
     pthread_cond_destroy(&(to_dealloc->write_rdy))){
    perror("deallocLock");
    exit(1);
  }

  return;
}
