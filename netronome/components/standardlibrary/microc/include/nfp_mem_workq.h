/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_MEM_WORKQ_H__
#define __NFP_MEM_WORKQ_H__

#include <nfp.h>
#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif


/** @file nfp_mem_workq.h
 * @addtogroup MU_WorkQ MU WorkQ Intrinsics
 * @{
 */

/** @name Work Queue functions for MEM
 *  @{
 *
 * Work queues enable work to be scheduled among a pool of threads. Threads are added
 * to the work queue using the cmd_mem_workq_add_thread() intrinsic. The call will only return
 * when work is available or becomes available.
 *
 * Work is added to a work queue using cmd_mem_workq_add_work_imm() or cmd_mem_workq_add_work().
 * These calls will never block and overflow will occur when more work is added than
 * is consumed.
 *
 * Work queues are based on type 2 rings and the cmd_mem_ring_init() function can be used to
 * configure a new work queue before adding work or threads.
 *
 * Work can consist of 1 to 16 long words and should be the same size when added as when
 * consumed by cmd_mem_workq_add_thread().
 *
 * Below is an example of workq functions:
 *
 * @code
 *   // ring head and tail on i25 or emem1
 *   __emem_n(1) __addr40 __align(512*sizeof(uint32_t)) uint32_t mem_workq[512];
 *
 *   // place workq on emem1 with ring number 4
 *   uint32_t       mu_island = 1;
 *   uint32_t       ring_number =  (mu_island << 10) | 4;
 *   SIGNAL             signal;
 *
 *   cmd_mem_ring_init(ring_number, RING_SIZE_512, mem_workq, mem_workq, 0);
 *
 *   // Add work
 *   {
 *       __xwrite uint32_t          write_register;
 *       write_register = 0x01;
 *       cmd_mem_workq_add_work(ring_number, &write_register, 1, sig_done, &signal);
 *       __wait_for_all(&signal);
 *   }
 *
 *   // Add more work
 *   {
 *       uint32_t work = 0x123;
 *       cmd_mem_workq_add_work_imm(ring_number, work);
 *   }
 *
 *   {
 *       __xread uint32_t           read_register;
 *
 *       // Add thread, should get back 0x01
 *       cmd_mem_workq_add_thread(ring_number, &read_register, 1, ctx_swap, &signal);
 *
 *       if (read_register != 0x01)
 *       {
 *           return 0;        // We have an error
 *       }
 *
 *       // Add thread, should get back 0x123
 *       cmd_mem_workq_add_thread(ring_number, &read_register, 1, ctx_swap, &signal);
 *
 *       if (read_register != 0x123)
 *       {
 *           return 0;        // We have an error
 *       }
 *   }
 *
 *   return 1;
 *
 * @endcode
 */

/** Add work specified in transfer register(s) and place on the work queue.
 *
 * @param ring_number       MEM ring number to get data from (0-1023)
 * @param xfer              Transfer register(s) containing the work
 * @param count             Number of 32-bit words specifying the work (1 - 16)
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion.
 *
 * @note    No work queue overflow checking is performed.
 *
 */
__intrinsic
void cmd_mem_workq_add_work(
    uint32_t ring_number,
    __xwrite void *xfer,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Add a single long word describing work on the work queue.
 *
 * @param ring_number       MEM ring number to put get data from (0-1023)
 * @param work_longword     32-bit word describing the work
 *
 * @note    No work queue overflow checking is performed and no signals are
 *          used.
 *
 *
 */
__intrinsic
void
cmd_mem_workq_add_work_imm(
    uint32_t ring_number,
    uint32_t work_longword
);

/** Fetch work from a work queue into transfer register(s).
 *
 * @param ring_number       MEM ring number to get data from (0-1023)
 * @param xfer              Transfer register(s) receiving the work
 * @param count             Number of 32-bit words to get (1 - 16)
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note    This call will block until there is work on the queue.
 *
 */
__intrinsic
void cmd_mem_workq_add_thread(
    uint32_t ring_number,
    __xread uint32_t *xfer,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** @}
 * @}
 */

#endif /* __NFP_MEM_WORKQ_H__ */
