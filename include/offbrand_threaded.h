
/*
 * THREAD SYNCHRONIZATION LIBRARY
 * 
 * The locking methods declared below are intended to benefit the creation
 * of threadsafe classes. These locking methods are called internally in all
 * Offbrand classes. Thread safety is the default on all Offbrand builtins,
 * locking methods do not (and should not) need be called on Offbrand builtins
 * directly. This convention should be followed when creating any Offbrand
 * compatible classes.
 *
 * Methods do nothing but return 0 if OB_THREADED is undefined (threading is not
 * desired) so that program resources are not wasted locking and unlocking objs
 * for the single thread. if OB_THREADED is defined then methods perform 
 * described function*
 */

#ifndef OFFBRAND_THREADED_H
#define OFFBRAND_THREADED_H

#include <pthread.h>
#include "offbrand.h"

/* lock an obj for reading. Multiple threads can obtain a read lock on the same
 * obj at the same time. A call to readUnlock is required when read operations
 * have been completed to allow for write operations to occur. A readLock can be
 * obtained recursively by the same thread. Blocks if another object has 
 * obtained a write lock on that obj, resuming when a read lock can be obtained.
 * Returns 0 if successfully locked, prints system error and returns 1 on
 * failure */
void readLock(obj *to_read_lock);

/* unlock an obj that previously obtained a read lock. readUnlock must be called
 * once for each readLock call to fully unlock the obj. Calling readUnlock from 
 * a thread that does not have at least one pending read lock results in 
 * undefined behavior. Returns 0 on success, prints system error and returns 1
 * on failure */
void readUnlock(obj *to_read_unlock);

/* lock an obj for writing. Only one thread may obtain a write lock on an obj at
 * any given time. A call to writeUnlock is required when write operations have
 * been completed to allow other operations to occur on the obj. A writeLock
 * cannot be obtained recursively by the same thread. Blocks if another write
 * lock or any read locks are held on the obj until a write lock can be obtained
 * Returns 0 if successfully locked, prints system error and returns 1 on
 * failure */
void writeLock(obj *to_write_lock);

/* unlock an obj that previously obtained a write lock. writeUnlock must be 
 * called once for each readLock call to fully unlock the obj. Calling
 * writeUnlock from a thread that does not have an active write lock results in
 * undefined behavior. Returns 0 if successfully locked, prints system error and
 * returns 1 on failure */
void writeUnlock(obj *to_write_unlock);

#endif
