/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __MUTEXLV_H__
#define __MUTEXLV_H__

#include <nfp.h>
#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file mutexlv.h
 * @addtogroup mutex Mutex/Semaphore Functions
 * @{
 *
 * Mutual exclusion locks (mutexes) prevent multiple threads from
 * simultaneously executing critical sections of code which access shared
 * data (that is, mutexes are used to serialize the execution of threads).
 * All mutexes must be global. A successful call for a mutex lock via
 * MUTEXLV_lock() will cause another thread that is also trying to lock
 * the same mutex to block until the owner thread unlocks it via or
 * mutex_unlock(). Threads within the same micro engine can share mutexes.
 *
 *
 * The MUTEXLV uses the micro C construct
 * typedef volatile __shared __gpr unsigned int MUTEXLV;
 * which is a shared general purpose register (across threads in a ME)
 *
 * If the compiler cannot allocate the gp_reg object in a register, it will
 * report an error and die most heinously.
 *
 * The MUTEXLV are implemented via macros, as they must manipulate
 * the register object directly.
 *
 * A single MUTEXLV object is capable of 32 mutex(s), each referred to with
 * a user specified unique id (MUTEXID) from [ 0 .. 31 ] , not necessarily
 * a constant.  There is no range checking on the required id.
 *
 * Usage
 * @code
 *  MUTEXLV lock= 0;
 *  f(MUTEXID handle)
 *  {
 *      ...
 *      MUTEXLV_lock(lock, handle)
 *
 *      // lock data/code access to only one ME local thread
 *
 *      MUTEXLV_unlock(lock, handle)
 *      ...
 *  }
 * @endcode
 *
 * If the handle value is reused, it is the responsibility of the user
 * to use the correct barrier synchronization (semaphore).
 */
typedef unsigned int MUTEXID;

/**
 * MUTEXLV types are stored in gp_reg's.
 */
typedef volatile __shared __gpr unsigned int MUTEXLV;


#define MUTEX_BSET(x,i) ((x) |=  (1<<(i)))
#define MUTEX_BTST(x,i) ((x) &   (1<<(i)))
#define MUTEX_BCLR(x,i) ((x) &= ~(1<<(i)))

/** @name MUTEXLV Macro Functions
 * @{
 */

/** Initialize all id's to zero
 *
 * @param m     mutex object to be initialized
 *
 * @note The mutex must be initialized only once (either globally,
 *  or with some semaphore)
 */
#define MUTEXLV_init(m) \
    m = 0

/** Clear the specific [mutex, id] in mutex.
 *
 * @param m     Mutex object
 * @param h     Handle to specific mutex id
 *
 */
#define MUTEXLV_destroy(m,h) \
    MUTEX_BCLR(m, h)

/** Test and block a mutex for a lock.
 *
 * @param m     Mutex object
 * @param h     Handle to specific mutex id
 *
 * If busy, the current context is swapped out;
 * If free, the [mutex,id] is set and ERRCODE_EOK is returned in supplied
 * argument. There is no sense of ctx() ownership or recursion. If the
 * same thread tries to reacquire the lock (that it already owns), it
 * too will block.
 */
#define MUTEXLV_lock(m,h) \
    while(MUTEX_BTST(m,h)) { ctx_wait(voluntary);  }  \
    MUTEX_BSET(m,h)

/** Unlock the [mutex, id].
 *
 * @param m     Mutex object
 * @param h     Handle to specific mutex id
 *
 * There is no sense of ctx() ownership
 * (i.e. a different thread may unlock the object)
 */
#define MUTEXLV_unlock(m,h) \
    MUTEX_BCLR(m, h)

/** Attempt to acquire the [mutex, id] (when free), but don't block.
 *
 * @param m     Mutex object
 * @param h     Handle to specific mutex id
 * @param err   Specific error code returned.
 *
 * This function acquires a mutex when it is free. When the mutex is locked
 * it does not block but returns ERRCODE_EBUSY.
 *
 * Values returned in err are:
 * @li ERRCODE_EBUSY lock is busy
 * @li ERRCODE_EOK   lock has been acquired
 */
#define MUTEXLV_trylock(m,h,err)    \
    if (MUTEX_BTST(m,h))            \
    {   err= ERRCODE_EBUSY; }       \
    else                            \
    {   err=  ERRCODE_EOK;          \
        MUTEX_BSET(m,h);            \
    }

/** This tests but does not acquire the mutex
 *
 * @param m     Mutex object
 * @param h     Handle to specific mutex id
 * @param err   Specific error code returned.
 *
 * This function tests a mutex but does not acquire it.
 *
 * Values returned in err are:
 * @li ERRCODE_EBUSY mutex is locked
 * @li ERRCODE_EOK   mutex is free (i.e. may be acquired)
 */
#define MUTEXLV_testlock(m,h,err)  \
    err= MUTEX_BTST(m,h) ? ERRCODE_EBUSY:ERRCODE_EOK

/** @}
 * @}
 */

extern MUTEXLV comm_ready;

#endif /* __MUTEXLV_H__ */
