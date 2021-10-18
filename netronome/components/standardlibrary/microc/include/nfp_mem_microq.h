/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* mem microq functions */

#ifndef __NFP_MEM_MICROQ_H__
#define __NFP_MEM_MICROQ_H__

#include <nfp.h>
#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file nfp_mem_microq.h
 *  @addtogroup mu_microq MU MicroQ Intrinsics
 *  @{
 */

/** @name Micro Queue Intrinsics for MEM.
 *  @{
 *
 * The micro queue intrinsic functions make provision for 128 or 256 bit queues.
 *
 * Micro queues can be configured to store 16 or 32 bit words. A 128 bit micro queue can queue 6, 16 bit
 * words or 3, 32 bit words and a 256 bit micro queue can contain 14, 16 bit words and 7, 32 bit words.
 *
 * Data is queued (at the tail) using the put function. Data can be read from head of the queue using the get function or
 * from the tail using the pop function.
 *
 * Initialize functions are available to set up micro queues before first use.
 */

/** Micro queue entry sizes.
 *
 * 0 - 16 bit
 * 1 - 32 bit
 */
typedef enum MICROQ_ENTRY_SIZE
{
    MICROQ_ENTRY_SIZE_16,   /**< 16 bit */
    MICROQ_ENTRY_SIZE_32,   /**< 32 bit */
} MICROQ_ENTRY_SIZE;

#ifdef __BIGENDIAN
/** Micro queue descriptor.
 *
 * The descriptor is the first word in a micro queue structure and contains the configuration
 * and state of the micro queue.
 */
typedef union mem_microq_desc_t
{
    struct
    {
        uint32_t reserved_1     : 4;    /**< Reserved.                                          */

        uint32_t event_source   : 12;   /**< Event source number to include in event.           */
        uint32_t reserved_2     : 6;    /**< Reserved.                                          */

        uint32_t generate_event : 1;    /**< Generate event on under or overflow.               */
        uint32_t entry_size     : 1;    /**< One of MICROQ_ENTRY_SIZE. @see MICROQ_ENTRY_SIZE.  */
        uint32_t overflow       : 1;    /**< Set on overflow.                                   */
        uint32_t underflow      : 1;    /**< Set on underflow.                                  */
        uint32_t reserved_3     : 1;    /**< Reserved.                                          */

        uint32_t locked         : 1;    /**< Used for queue lock queues.
                                             *   When this bit is set, it indicates that
                                             *   the lock has been given out.
                                             */
        uint32_t num_elements   : 4;    /**< 0-3 or 0-6 or 0-14 depending on entry_size
                                                 and if it is quarter or half cache line.            */
    };
    uint32_t value;                     /**< Accessor to entire structure.                      */
} mem_microq_desc_t;

#else
/* __LITTLEENDIAN */

/* See BE description above */
typedef struct
{
    union
    {
        struct
        {
            uint32_t num_elements   : 4;
            uint32_t locked         : 1;
            uint32_t reserved_3     : 1;
            uint32_t underflow      : 1;
            uint32_t overflow       : 1;

            uint32_t entry_size     : 1;
            uint32_t generate_event : 1;
            uint32_t reserved_2     : 6;
            uint32_t event_source   : 12;

            uint32_t reserved_1     : 4;
        };
        uint32_t value;
    };
} mem_microq_desc_t;
#endif

/** 128 bit micro queue container type.
 *  The first word is the queue descriptor @see mem_microq_desc_t
 */
typedef struct mem_microq128_t
{
    mem_microq_desc_t desc;                 /**< Micro queue descriptor */
    uint32_t q[3];                      /**< 3 words for queue contents */
} mem_microq128_t;

/** 256 bit micro queue container type.
 *  The first word is the queue descriptor @see mem_microq_desc_t
 */
typedef struct mem_microq256_t
{
    mem_microq_desc_t desc;                 /**< Micro queue descriptor */
    uint32_t q[7];                      /**< 7 words for queue contents */
} mem_microq256_t;

/** 128-bit microqueue aligned in 32-bit addressed memory. */
typedef volatile __addr32 __align16 mem_microq128_t mem_microq128_in_mem_t;

/** 128-bit microqueue aligned in 40-bit addressed memory. */
typedef volatile __addr40 __mem __align16 mem_microq128_t *mem_microq128_ptr40_t;

/** 256-bit microqueue aligned in 32-bit addressed memory. */
typedef volatile __addr32 __align32 mem_microq256_t mem_microq256_in_mem_t;

/** 256-bit microqueue aligned in memory 40-bit addressed memory. */
typedef volatile __addr40 __mem __align32 mem_microq256_t *mem_microq256_ptr40_t;


/** Add data to the tail of a 128-bit micro queue in 32-bit addressed MEM.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param xfer              Transfer register containing the data
 * @param sync              Type of synchronization to use
 * @param sig_ptr           Signal to raise upon completion
 *
 * If there are not enough space, the data is discarded and depending on the micro queue
 * configuration an event is presented on the event bus. 16 bits or 32 bits of the
 * transfer register is written to the micro queue based on the configuration
 * of the micro queue.
 */
__intrinsic
void cmd_mem_microq128_put_ptr32(
    __mem mem_microq128_in_mem_t *microq,
    __xwrite void *xfer,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Add data to the tail of a 128-bit micro queue in 40-bit addressed MEM.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param xfer              Transfer register containing the data
 * @param sync              Type of synchronization to use
 * @param sig_ptr           Signal to raise upon completion
 *
 * If there are not enough space, the data is discarded and depending on the micro queue
 * configuration an event is presented on the event bus. 16 bits or 32 bits of the
 * transfer register is written to the micro queue based on the configuration
 * of the micro queue.
 */
__intrinsic
void cmd_mem_microq128_put_ptr40(
    mem_microq128_ptr40_t microq,
    __xwrite void *xfer,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Add data to the tail of a 256-bit micro queue in 32-bit addressed MEM.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param xfer              Transfer register containing the data
 * @param sync              Type of synchronization to use
 * @param sig_ptr           Signal to raised upon completion
 *
 * If there are not enough space, the data is discarded and depending on the micro queue
 * configuration an event is presented on the event bus. 16 bits or 32 bits of the
 * transfer register is written to the micro queue based on the configuration
 * of the micro queue.
 */
__intrinsic
void cmd_mem_microq256_put_ptr32(
    __mem mem_microq256_in_mem_t *microq,
    __xwrite void *xfer,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Add data to the tail of a 256-bit micro queue in 40-bit addressed MEM .
 *
 * @param microq            Pointer to micro queue in MEM
 * @param xfer              Transfer register containing the data
 * @param sync              Type of synchronization to use
 * @param sig_ptr           Signal to raised upon completion
 *
 * If there are not enough space, the data is discarded and depending on the micro queue
 * configuration an event is presented on the event bus. 16 bits or 32 bits of the
 * transfer register is written to the micro queue based on the configuration
 * of the micro queue.
 */
__intrinsic
void cmd_mem_microq256_put_ptr40(
    mem_microq256_ptr40_t microq,
    __xwrite void *xfer,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Remove data from the head of a 128-bit micro queue in 32-bit addressed MEM and place data in transfer register.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param xfer              Transfer register receiving the data
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Even signal is raised upon completion. Odd signal is raised on underflow.
 *
 * If there are not enough entries in the micro queue, a zero long word is returned and an odd signal will
 * also be delivered.
 */
__intrinsic
void cmd_mem_microq128_get_ptr32(
    __mem mem_microq128_in_mem_t *microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Remove data from the head of a 128-bit micro queue in 40-bit addressed MEM and place data in transfer register.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param xfer              Transfer register receiving the data
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Even signal is raised upon completion. Odd signal is raised on underflow.
 *
 * If there are not enough entries in the micro queue, a zero long word is returned and an odd signal will
 * also be delivered.
 */
__intrinsic
void cmd_mem_microq128_get_ptr40(
    mem_microq128_ptr40_t microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);


/** Remove data from the head of a 128-bit micro queue in 32-bit addressed MEM and place data in transfer register.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param xfer              Transfer register receiving the data
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Even signal is raised upon completion. Odd signal is raised on underflow.
 *
 * If there are not enough entries in the micro queue, a zero long word is returned and an odd signal will
 * also be delivered.
 */
__intrinsic
void cmd_mem_microq256_get_ptr32(
    __mem mem_microq256_in_mem_t *microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Remove data from the head of a 128-bit micro queue in 40-bit addressed MEM and place data in transfer register.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param xfer              Transfer register receiving the data
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Even signal is raised upon completion. Odd signal is raised on underflow.
 *
 * If there are not enough entries in the micro queue, a zero long word is returned and an odd signal will
 * also be delivered.
 */
__intrinsic
void cmd_mem_microq256_get_ptr40(
    mem_microq256_ptr40_t microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);


/** Remove data from the tail of a 128-bit micro queue in 32-bit addressed MEM and place data in transfer register.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param xfer              Transfer register receiving the data
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Even signal is raised upon completion. Odd signal is raised on underflow.
 *
 * If there are not enough entries in the micro queue, a zero long word is returned and an odd signal will
 * also be delivered.
 */
__intrinsic
void cmd_mem_microq128_pop_ptr32(
    __mem mem_microq128_in_mem_t *microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Remove data from the tail of a 128-bit micro queue in 40-bit addressed MEM and place data in transfer register.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param xfer              Transfer register receiving the data
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Even signal is raised upon completion. Odd signal is raised on underflow.
 *
 * If there are not enough entries in the micro queue, a zero long word is returned and an odd signal will
 * also be delivered.
 */
__intrinsic
void cmd_mem_microq128_pop_ptr40(
    mem_microq128_ptr40_t microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);


/** Remove data from the tail of a 256-bit micro queue in 32-bit addressed MEM and place data in transfer register.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param xfer              Transfer register receiving the data
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Even signal is raised upon completion. Odd signal is raised on underflow.
 *
 * If there are not enough entries in the micro queue, a zero long word is returned and an odd signal will
 * also be delivered.
 */
__intrinsic
void cmd_mem_microq256_pop_ptr32(
    __mem mem_microq256_in_mem_t *microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Remove data from the tail of a 256-bit micro queue in 40-bit addressed MEM and place data in transfer register.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param xfer              Transfer register receiving the data
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Even signal is raised upon completion. Odd signal is raised on underflow.
 *
 * If there are not enough entries in the micro queue, a zero long word is returned and an odd signal will
 * also be delivered.
 */
__intrinsic
void cmd_mem_microq256_pop_ptr40(
    mem_microq256_ptr40_t microq,
    __xread void *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);


/** Initialize a 128-bit micro queue in 32-bit addressed MEM.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param entry_size        Size of entries in the microqueue, 1 = 32-bit 0 = 16-bit
 * @param generate_event    Enable event on overflow or underflow
 * @param event_source      Event to present on event bus if generate_event is 1
 *
 * The header of the micro queue is written and then read back to ensure the
 * micro queue is configured when this function returns.
 */
void cmd_mem_microq128_init_ptr32(
    __mem mem_microq128_in_mem_t *microq,
    enum MICROQ_ENTRY_SIZE entry_size, /* 0 = 16-bit, 1 = 32-bit              */
    uint32_t generate_event,       /* Generate event on under or overflow */
    uint32_t event_source
);

/** Initialize a 128-bit micro queue in 40-bit addressed MEM.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param entry_size        Size of entries in the microqueue, 1 = 32-bit 0 = 16-bit
 * @param generate_event    Enable event on overflow or underflow
 * @param event_source      Event to present on event bus if generate_event is 1
 *
 * The header of the micro queue is written and then read back to ensure the
 * micro queue is configured when this function returns.
 *
 * The following example shows how to create a 128-bit microq in emem0 (i24).
 * @code
 *
 *     volatile __emem_n(0) __addr40 __align16 mem_microq128_t  mq128_ptr40;
 *    __xread uint32_t                                      read_data;
 *    __xwrite uint32_t                                     write_data;
 *    uint32_t                                              counter;
 *    int32_t                                                       i;
 *    SIGNAL                                                    sig;
 *    SIGNAL_PAIR                                               sig_pair;
 *
 *    cmd_mem_microq128_init_ptr40(&mq128_ptr40, MICROQ_ENTRY_SIZE_16, 0, 0);
 *
 *    write_data = 0x10001000;
 *    cmd_mem_microq128_put_ptr40(&mq128_ptr40, &write_data, ctx_swap, &sig);
 *
 *    write_data = 0x20002000;
 *    cmd_mem_microq128_put_ptr40(&mq128_ptr40, &write_data, ctx_swap, &sig);
 *
 *    // Verify the number of elements
 *    if (mq128_ptr40.desc.num_elements != 2)
 *    {
 *        return 0;         // We have an error
 *    }
 *
 *    cmd_mem_microq128_pop_ptr40(&mq128_ptr40, &read_data, sig_done, &sig_pair);
 *   wait_for_all_single(&sig_pair.even);
 *
 *    if (signal_test(&sig_pair.odd))
 *    {
 *        return 0;         // We have an error
 *    }
 *
 *    if (read_data != (0x20002000 & 0xffff))
 *    {
 *        return 0;         // We have an error
 *   }
 *
 *    return 1;
 *
 * @endcode
 */
void cmd_mem_microq128_init_ptr40(
    mem_microq128_ptr40_t microq,
    enum MICROQ_ENTRY_SIZE entry_size, /* 0 = 16-bit, 1 = 32-bit              */
    uint32_t generate_event,       /* Generate event on under or overflow */
    uint32_t event_source
);

/** Initialize a 256-bit micro queue in 32-bit addressed MEM.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param entry_size        Size of entries in the microqueue, 1 = 32-bit 0 = 16-bit
 * @param generate_event    Enable event on overflow or underflow
 * @param event_source      Event to present on event bus if generate_event is 1
 *
 * The header of the micro queue is written and then read back to ensure the
 * micro queue is configured when this function returns.
 */
void cmd_mem_microq256_init_ptr32(
    __mem mem_microq256_in_mem_t *microq,
    enum MICROQ_ENTRY_SIZE entry_size, /* 0 = 16-bit, 1 = 32-bit              */
    uint32_t generate_event,       /* Generate event on under or overflow */
    uint32_t event_source
);

/** Initialize a 256-bit micro queue in 40-bit addressed MEM.
 *
 * @param microq            Pointer to micro queue in MEM
 * @param entry_size        Size of entries in the microqueue, 1 = 32-bit 0 = 16-bit
 * @param generate_event    Enable event on overflow or underflow
 * @param event_source      Event to present on event bus if generate_event is 1
 *
 * The header of the micro queue is written and then read back to ensure the
 * micro queue is configured when this function returns.
 */
void cmd_mem_microq256_init_ptr40(
    mem_microq256_ptr40_t microq,
    enum MICROQ_ENTRY_SIZE entry_size, /* 0 = 16-bit, 1 = 32-bit              */
    uint32_t generate_event,       /* Generate event on under or overflow */
    uint32_t event_source
);

/** @}
 * @}
 */

#endif /* __NFP_MEM_MICROQ_H__ */
