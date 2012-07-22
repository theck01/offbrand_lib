##LOCKING MECHANISM DESIGN OUTLINE

The locking mechanism will consist of the following parts:

  - A 'lock' mutex. Must be locked when locking mechanim state changes.
  - A 'waiting to read' count. This integer count keeps track of the number of
    threads waiting to be notified that reading operations can occur.
  - A 'currently reading' count. This integer counts the number of threads that
    currently have read-only access to the protected data.
  - A 'waiting to write' count. This integer counts the number of threads
    waiting to perform write operations on the protected data.
  - A 'currently writing flag. This uint8_t indicates whether the data is
    currently granting write access to a single thread.
  - A 'allow read operation(s)' condition, which signals threads intending to
    read that access may be allowed.
  - A 'allow a write operation' condition, which signals a thread intending to
    write that access to data may be allowed.

Lock initially grants access on a first come first served basis, read or write.
After the first operation occurs, the lock attempts to ceed data control to an
opposing operation (read -> write, or write -> read) by signaling the condition
variable associated with that operation. If no variables are waiting on the
opposite operation then lock ceeds data control to threads waiting on the same
operation type as just executed as well. This way reads and writes get equal 
turns accessing data when lock contention occurs.

When a read condition variable is signaled all reading threads are notified,
allowing all threads waiting on a read operation to gain access. When a write
condition variable is signaled only one write thread is notified (selected in an
undefined way by the operating system), because only one write thread may change
the data at any time.

All read accessing threads are given access to the data as soon as possible,
unless one or more threads are waiting to write. They then wait for one write
operation to occur before being granted read access.

##LOCK API:

readLock(obj *) - Attempts to lock the given object so that a read operation
  may occur. Multiple readLocks can be granted at the same time. May also have
  a flag that indicates whether blocking should occur, or if the call should
  return with a indication that lock failed in contention.
readUnlock(obj *) - Unlocks the given object, decrementing the current reading
  count and possibly signaling that a write operation may occur if no other
  threads are reading.
writeLock(obj *) - Attempts to lock the given object so that the thread may
  perform a write operation on the data. May also have a flag that indicates
  whether blocking should occur, or if the call should return with a indication
  that lock failed in contention.
writeUnlock(obj *) - Unlocks the given object, reseting the 'writing' flag and
  signaling to any waiting read threads that access should be granted.

