
/*
 * Offbrand Library with pthread support
 * 
 * NOTE: The locking methods declared below are intended to benefit the creation
 * of threadsafe classes. These locking methods are called internally in all
 * Offbrand classes if this header file is included in a build. Thread safety is
 * the default on all Offbrand builtins, locking methods do not (and should not
 * for performance reasons) need be called on Offbrand builtins directly.
 */

#ifndef OFFBRAND_THREADED_H
#define OFFBRAND_THREADED_H

#include <pthread.h>
#include "offbrand.h"

/* PUBLIC THREADSAFETY METHODS */

/* lock an obj for reading. Multiple threads can obtain a read lock on the same
 * obj at the same time. A call to readUnlock is required when read operations
 * have been completed to allow for write operations to occur. A readLock can be
 * obtained recursively by the same thread. Blocks if another object has 
 * obtained a write lock on that obj, resuming when a read lock can be obtained.
 * Returns 0 if successfully locked, prints system error and returns 1 on
 * failure */
int readLock(obj *to_read_lock);

/* unlock an obj that previously obtained a read lock. readUnlock must be called
 * once for each readLock call to fully unlock the obj. Calling readUnlock from 
 * a thread that does not have at least one pending read lock results in 
 * undefined behavior. Returns 0 on success, prints system error and returns 1
 * on failure */
int readUnlock(obj *to_read_unlock);

/* lock an obj for writing. Only one thread may obtain a write lock on an obj at
 * any given time. A call to writeUnlock is required when write operations have
 * been completed to allow other operations to occur on the obj. A writeLock
 * cannot be obtained recursively by the same thread. Blocks if another write
 * lock or any read locks are held on the obj until a write lock can be obtained
 * Returns 0 if successfully locked, prints system error and returns 1 on
 * failure */
int writeLock(obj *to_write_lock);

/* unlock an obj that previously obtained a write lock. writeUnlock must be 
 * called once for each readLock call to fully unlock the obj. Calling
 * writeUnlock from a thread that does not have an active write lock results in
 * undefined behavior. Returns 0 if successfully locked, prints system error and
 * returns 1 on failure */
int writeUnlock(obj *to_write_unlock);

#endif
