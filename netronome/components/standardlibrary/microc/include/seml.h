/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __SEML_H__
#define __SEML_H__

#include <errcode.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif


/** @file seml.h
 * @addtogroup mutex Mutex/Semaphore Functions
 * @{
 */

typedef unsigned int SEMVALUE;

/** Semaphore structure. */
struct SEML
{
    unsigned barrier :1;    /**< Indication that barrier is set. */
    unsigned init :1;       /**< Semaphore is initialized. */
    unsigned :14;
    unsigned initval :8;    /**< Initial value of semaphore. */
    unsigned val :8;        /**< Current value of semaphore. */
};

typedef volatile __shared __gpr struct SEML SEML;

#define SEML_f_barrier(x) x.barrier
#define SEML_f_init(x) x.init
#define SEML_f_initval(x) x.initval
#define SEML_f_val(x) x.val

#define SEML_init_list(v) { 0, 0, v, v }

/** @name SEML Macro Functions
 * @{
 */

/** Initialize an unnamed semaphore.
 *
 *  @param s    Semaphore object
 *  @param v    Initial value of the semaphore counter (0-255)
 *
 *  This macro statically initializes an unnamed semaphore. The
 *  initial value of the semaphore is set to 'value'.
 *
 *  The semaphore may also be initialized globally by
 *  @code
 *  SEML sem = SEML_init_list(value)
 *  @endcode
 */
#define SEML_init(s, v)             \
    if (SEML_f_init(s) == 0)        \
    {                               \
        SEML_f_init(s) = 1;         \
        SEML_f_barrier(s)= 0;       \
        SEML_f_initval(s)= v;       \
        SEML_f_val(s)= v;           \
    }

/** Destroy a semaphore.
 *
 * @param s     Semaphore object
 *
 * This function destroys a semaphore.
 */
#define SEML_destroy(s)             \
    SEML_f_init(s)= 0;              \
    SEML_f_initval(s)= 0

/** Return the value of a semaphore.
 *
 * @param s     Semaphore object
 */
#define SEML_getvalue(s) SEML_f_val(s)

/** Increment a semaphore and wake up a thread.
 *
 * @param s     Semaphore object
 *
 * This macro will increment a semaphore. If there are waiting threads,
 * one is unblocked; otherwise, the semaphore value is incremented
 * by one.
 *
 * @see SEML_wait
 */
#define SEML_post(s) SEML_f_val(s)++

/** Decrement a semaphore and wake up a thread.
 *
 * @param s     Semaphore object
 *
 * This macro will decrement a semaphore. If there are waiting threads,
 * one is unblocked; otherwise, the semaphore value is decremented by one.
 *
 * @see SEML_wait
 */
#define SEML_dec(s) SEML_f_val(s)--

/** Set the barrier in the semaphore if the value is
 *  equal to the value specified.
 *
 * @param s     Semaphore object.
 * @param n     Value when to set the barrier.
 */
#define SEML_set_barrier_at(s,n)    \
    if (SEML_f_val(s)==n) { SEML_f_barrier(s) = 1; }

/** Clear the barrier in a semaphore if the value is
 *  equal to the value specified.
 *
 * @param s     Semaphore object
 * @param n     Value when to clear the barrier.
 */
#define SEML_clr_barrier_at(s,n)    \
    if (SEML_f_val(s)==n) { SEML_f_barrier(s) = 0; }

/** Wait on a semaphore if its value is equal or less than zero and
 * the barrier is not set.
 *
 * @param s  Semaphore object
 *
 * If the semaphore value is greater than zero, the value is decreased
 * by one. If the semaphore value is less than or equal zero (and the barrier
 * is not set), the calling thread is then blocked until it can successfully
 * decrease the value.
 */
#define SEML_wait(s)                                    \
    while(SEML_f_val(s)<=0 && SEML_f_barrier(s)==0)     \
    {  ctx_wait(voluntary);  }                          \
    SEML_dec(s)

/** Test a semaphore and if its value is greater than zero, decrement it.
 *
 * @param s     Semaphore object
 * @param err   ERRCODE_EOK or ERRCODE_AGAIN when semaphore is locked
 *
 * Similar to SEML_wait except that if the semaphore value is zero or,
 * less then this macro returns immediately with the error EAGAIN
 */
#define SEML_trywait(s, err)                            \
    if (SEML_f_val(s)<=0) {  err = ERRCODE_AGAIN;    }  \
    else { err = ERRCODE_EOK; SEML_dec(s); }

/** Wait on a semaphore if its value is greater that zero and the
 *  barrier is not set.
 *
 * @param s     Semaphore object
 *
 * This macro is performs the dual of SEML_wait. It is used to
 * provide a synchronization point for threads to join by waiting on
 * a semaphore and other threads to get through the while loop and
 * when everyone is thru, clear the barrier.
 *
 * The steps followed are:
 * Wait on the semaphore if its value is greater than zero and the barrier
 * is not set. If its value is 0 or less, fall through and set then the barrier
 * when its value is zero.
 *
 * Then increment the value of the semaphore.
 *
 * When the semaphore value is the same as its initial value, clear
 * the barrier.
 *
 */
#define SEML_barrier(s)                                 \
    while(SEML_f_val(s)>0 && SEML_f_barrier(s)==0)      \
    {  ctx_wait(voluntary);  }                          \
    if (SEML_f_val(s)==0)                               \
    { SEML_f_barrier(s)= 1; }                           \
    SEML_post(s);                                       \
    if (SEML_f_val(s)==SEML_f_initval(s))               \
    { SEML_f_barrier(s)= 0; }

/** Try a barrier without blocking.
 *
 * @param s     Semaphore object
 * @param err   ERRCODE_EOK or ERRCODE_AGAIN when semaphore is locked
 *
 * Similar to SEML_barrier except that if the semaphore value is less
 * than or equal zero, this function then increments the value.
 */
#define SEML_trybarrier(s, err)                         \
    if   (SEML_f_val(s)>0) {  err = ERRCODE_AGAIN;    } \
    else { err = ERRCODE_EOK; SEML_post(s); }

/** @}
 * @}
 */

#endif /* __SEML_H__ */_
