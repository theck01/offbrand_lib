
#include "../include/offbrand.h"
#include "../include/private/obj_private.h"

/* NOTES: NEED OBLOCK INIT METHOD (DECLARED IN OBLock_private, used in initBase
 * if threaded lib used
 * Wrap all internals (except returns) in OB_THREADED macro conditionals, so
 * locking commands do nothing if OB_THREADED is not defined */


int readLock(obj *to_read_lock){

#ifdef OB_THREADED
  
  if(!to_read_lock){
    fprintf(stderr, "Offbrand Lock Lib: Unexpected NULL argument passed to "
                    "readLock\n");
    return 1;
  }
  
  /* Lock the OBLock mutex to change shared lock data */
  if(pthread_mutex_lock(&((*to_read_lock)->lock.mutex))){
    perror("readLock pthread_mutex_lock\n");
    return 1;
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
    return 1;
  }

#endif

  return 0;
}


int readUnlock(obj *to_read_unlock){

#ifdef OB_THREADED
  
  uint8_t signal_write = 0; /* booleans to signal writes and reads at end of */
  uint8_t signal_read = 0;  /* readUnlock */
                                        

  if(!to_read_unlock){
    fprintf(stderr, "Offbrand Lock Lib: Unexpected NULL argument passed to "
                    "readUnlock\n");
    return 1;
  }

  /* Lock the OBLock mutex to change shared lock data */
  if(pthread_mutex_lock(&((*to_read_unlock)->lock.mutex))){
    perror("readUnlock pthread_mutex_lock\n");
    return 1;
  }

  /* check count for error */
  if((*to_read_unlock)->lock.reads_active == 0){
    fprintf(stderr, "Offbrand Lock Lib: Cannot readUnlock the provided obj, "
                    "there are no threads holding a read lock\n");
    return 1;
  }

  /* if thread was last thread with read lock */
  if(--((*to_read_unlock)->lock.reads_active) == 0){
      /* attempt to signal writes first, reads if no writes are waiting */
      if((*to_read_unlock)->lock.writes_waiting > 0) signal_write = 1;
      else if((*to_read_unlock)->lock.reads_waiting > 0) signal_read = 1;
  }

  
  if(pthread_mutex_unlock(&((*to_read_unlock)->lock.mutex))){
    perror("readUnlock pthread_mutex_unlock\n");
    return 1;
  }

  /* if should signal waiting threads */
  if(signal_write) pthread_cond_signal(&((*to_read_unlock)->lock.write_rdy));
  else if(signal_read) 
    pthread_cond_broadcast(&((*to_read_unlock)->lock.read_rdy));

#endif

  return 0;
}


int writeLock(obj *to_write_lock){

#ifdef OB_THREADED

  if(!to_write_lock){
    fprintf(stderr, "Offbrand Lock Lib: Unexpected NULL argument passed to "
                    "writeLock\n");
    return 1;
  }
  
  /* Lock the OBLock mutex to change shared lock data */
  if(pthread_mutex_lock(&((*to_write_lock)->lock.mutex))){
    perror("writeLock pthread_mutex_lock\n");
    return 1;
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
    return 1;
  }
 
#endif

  return 0;
}


int writeUnlock(obj *to_write_unlock){

#ifdef OB_THREADED

  uint8_t signal_read = 0;  /* Boolean indicating whether any threads waiting */
  uint8_t signal_write = 0; /* should be signlaed */

  if(!to_write_unlock){
    fprintf(stderr, "Offbrand Lock Lib: Unexpected NULL argument passed to "
                    "writeLock\n");
    return 1;
  }

  /* Lock the mutex to change shared lock data */
  if(pthread_mutex_lock(&((*to_write_unlock)->lock.mutex))){
    perror("writeUnlock pthread_mutex_lock\n");
    return 1;
  }

  if(!(*to_write_unlock)->lock.write_active == 0){
    fprintf(stderr, "Offbrand Lock Lib: Cannot writeUnlock the provided obj, "
                    "there are no threads holding a read lock\n");
    return 1;
  }

  (*to_write_unlock)->lock.write_active = 0;

  /* if reads are waiting flag that they should be signaled. If no reads then
   * check and flag if writes should be signaled */
  if((*to_write_unlock)->lock.reads_waiting) signal_read = 1;
  else if((*to_write_unlock)->lock.writes_waiting) signal_write = 1;

  if(pthread_mutex_unlock(&((*to_write_unlock)->lock.mutex))){
    perror("writeUnlock pthread_mutex_unlock\n");
    return 1;
  }

  /* if should signal waiting threads */
  if(signal_read) pthread_cond_broadcast(&((*to_read_unlock)->lock.read_rdy));
  else if(signal_write)
    pthread_cond_signal(&((*to_read_unlock)->lock.write_rdy));
  
#endif

  return 0;
}

int initLock(OBLock *to_init){

#ifdef OB_THREADED

#endif
  
  return 0;
}

