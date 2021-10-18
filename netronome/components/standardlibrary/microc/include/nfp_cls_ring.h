/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_CLS_RING_H__
#define __NFP_CLS_RING_H__

#include <nfp.h>
#include <stdint.h>
#include <nfp_override.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif


/** @file nfp_cls.h
 * @addtogroup cls_ring CLS Ring Intrinsics
 * @{
 */


/**
 * @name Ring functions for Cluster Local Scratch
 * @{
 */
/** This enumeration defines the state values that can be tested
 * with the cmd_cls_state_test() intrinsic.
 */
typedef enum
{
    cls_state_ring0_status  = 0x0, /**< Indicates status of CLS Ring 0. */
    cls_state_ring1_status  = 0x1, /**< Indicates status of CLS Ring 1. */
    cls_state_ring2_status  = 0x2, /**< Indicates status of CLS Ring 2. */
    cls_state_ring3_status  = 0x3, /**< Indicates status of CLS Ring 3. */
    cls_state_ring4_status  = 0x4, /**< Indicates status of CLS Ring 4. */
    cls_state_ring5_status  = 0x5, /**< Indicates status of CLS Ring 5. */
    cls_state_ring6_status  = 0x6, /**< Indicates status of CLS Ring 6. */
    cls_state_ring7_status  = 0x7, /**< Indicates status of CLS Ring 7. */
    cls_state_ring8_status  = 0x8, /**< Indicates status of CLS Ring 8. */
    cls_state_ring9_status  = 0x9, /**< Indicates status of CLS Ring 9. */
    cls_state_ring10_status = 0xa, /**< Indicates status of CLS Ring 10. */
    cls_state_ring11_status = 0xb, /**< Indicates status of CLS Ring 11. */
    cls_state_ring12_status = 0xc, /**< Indicates status of CLS Ring 12. */
    cls_state_ring13_status = 0xd, /**< Indicates status of CLS Ring 13. */
    cls_state_ring14_status = 0xe, /**< Indicates status of CLS Ring 14. */
    cls_state_ring15_status = 0x0f /**< Indicates status of CLS Ring 15. */
} cls_state_t;


/** Ring word size.
 */
typedef enum CLS_RING_SIZE
{
    CLS_RING_SIZE_32    =   0,    /**< 32 words.
                               * base address should be aligned to 128 byte boundary, full is >=24 words.
                               */
    CLS_RING_SIZE_64    =   1,    /**< 64 words.
                                * base address should be aligned to 256 byte boundary, full is >=48 words.
                                */
    CLS_RING_SIZE_128   =   2,    /**< 128 words.
                                * base address should be aligned to 512 byte boundary, full is >=96 words.
                                */
    CLS_RING_SIZE_256   =   3,    /**< 256 words.
                                * base address should be aligned to 1024 byte boundary, full is >=192 words.
                                */
    CLS_RING_SIZE_512   =   4,    /**< 512 words.
                                * base address should be aligned to 2048 byte boundary, full is >=384 words.
                                */
    CLS_RING_SIZE_1024  =   5     /**< 1024 words.
                                * base address should be aligned to 4096 byte boundary, full is >=768 words.
                                */
} CLS_RING_SIZE;




/** Ring events to generate system events on the event bus. The reported event
 *  has the source set to the ring number and the event specified to the
 * relevant event type (0=>not empty, 1=>not full, 8=>underflow, 9=>overflow)
 */
typedef enum CLS_RING_EVENT_REPORTS
{
    CLS_RING_NO_EVENTS        =   0x00,    /**< No events generated.    */
    CLS_RING_UNDERFLOW_EVENT  =   0x01,    /**< Generate event on event bus if ring underflows.
                                    *  If asserted then a system event is generated whenever a command is
                                    *  handled which attempts to underflow the ring (remove elements when
                                    *  the ring is empty). This is generated even though the ring operation is
                                    *  aborted.
                                    */
    CLS_RING_OVERFLOW_EVENT   =   0x02,    /**< Generate event on event bus if ring overflows.
                                    *  If asserted then a system event is generated whenever a command is
                                    *  handled which attempts to overflow the ring; this is generated even
                                    *  though the ring operation is aborted, to indicate to the system that data has
                                    *  probably been lost.
                                   */
    CLS_RING_NOT_EMPTY_EVENT  =   0x04,    /**< Generate event on event bus if ring become not empty.
                                    *  If asserted then a system event is generated whenever the ring goes from
                                    *  empty to not empty.
                                    */
    CLS_RING_NOT_FULL_EVENT   =   0x08,    /**< Generate event on event bus if ring become not full.
                                    *  If asserted then a system event is generated whenever the ring goes from
                                    *  full (i.e. >3/4 entries used) to not full (<3/4 entries used).
                                    */
} CLS_RING_EVENT_REPORTS;

/** Tests the value of the specified CLS state name.
 *
 * @param state State to test
 *
 * This function tests the value of the specified state name and returns a 1 if the state is set or 0 if clear.
 * The argument state must be a constant literal as required by the microcode assembler; otherwise,
 * the compiler generates a runtime check, if possible, with loss of performance.
 */
int32_t cmd_cls_state_test(
    cls_state_t state
);

/** Put data on Cluster Local Scratch memory ring.
 *
 * @param data     Data to put onto ring
 * @param address  Ring address to put data into where the ring number is specified in address[5:2]
 * @param count    Number of 32-bit words to put into ring
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Write 32-bit words to tail of ring and update tail pointer.
 * If there are not enough empty slots in the ring, no words are added
 * and the tail pointer is not updated.
 * This will raise an overflow event if the ring is configured to deliver one.
 *
 */
__intrinsic
void cmd_cls_ring_put_ptr32(
    __xwrite void *data,
    volatile void __addr32 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Put data on Cluster Local Scratch memory ring.
 *
 * @param data     Data to put onto ring
 * @param address  40 bit ring address to put data into where upper 6 bits indicate the island.
 *                  The ring number is specified in address[5:2]
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to put into ring
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Write 40-bit words to tail of ring and update tail pointer.
 * If there are not enough empty slots in the ring, no words are added
 * and the tail pointer is not updated.
 * This will raise an overflow event if the ring is configured to deliver one.
 *
 */
__intrinsic
void cmd_cls_ring_put_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Add data on Cluster Local Scratch memory ring.
 *
 * @param data     Data to add onto ring
 * @param address  Ring address to add data into where the ring number is specified in address[5:2]
 * @param count    Number of 32-bit words to add into ring
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Journal adds data into specified ring in Cluster Local Scratch memory.
 * A ring journal command ignores any full indication.
 * The consumer of a journal will start at the tail pointer and read backwards
 * from there through time, up to the count of the journal.
 */
__intrinsic
void cmd_cls_ring_journal_ptr32(
    __xwrite void *data,
    volatile void __addr32 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Add data on Cluster Local Scratch memory ring.
 *
 * @param data     Data to add onto ring
 * @param address  40 bit ring address to put data into where upper 6 bits indicate the island.
 *                  The ring number is specified in address[5:2]
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to add into ring
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Journal adds data into specified ring in Cluster Local Scratch memory.
 * A ring journal command ignores any full indication.
 * The consumer of a journal will start at the tail pointer and read backwards
 * from there through time, up to the count of the journal.
 */
__intrinsic
void cmd_cls_ring_journal_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read data from Cluster Local Scratch memory ring and put in transfer register.
 *
 * @param data     Data read from ring
 * @param address  Ring address to read data from where the ring number is specified in address[5:2]
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Read 32-bit words from head of ring and update head pointer.
 * If there are not enough entries in the ring, no valid data is returned
 * and the head pointer is not updated.
 * This will raise an underflow event if the ring is configured to deliver one.
 */
__intrinsic
void cmd_cls_ring_get_ptr32(
    __xread void *data,
    volatile void __addr32 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read data from Cluster Local Scratch memory ring and put in transfer register.
 *
 * @param data     Data read from ring
 * @param address  40 bit ring address to read data from where upper 6 bits indicate the island.
 *                  The ring number is specified in address[5:2]
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Read 32-bit words from head of ring and update head pointer.
 * If there are not enough entries in the ring, no valid data is returned
 * and the head pointer is not updated.
 * This will raise an underflow event if the ring is configured to deliver one.
 */
__intrinsic
void cmd_cls_ring_get_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Remove data from Cluster Local Scratch memory ring and put in transfer register.
 *
 * @param data     Data read from ring
 * @param address  Ring address to read data from where the ring number is specified in address[5:2]
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Read 32-bit words from tail of ring and update tail pointer.
 * If there are not enough entries in the ring, no valid data is returned
 * and the tail pointer is not updated.
 * This will raise an underflow event if the ring is configured to deliver one.
 * @note A single pop operation will return the words in the order they were added.
 *       If the ring contains the words (A, B, C, D, E), then a single pop operation of 3 words
 *       will return the values (C, D, E) - in that order.
 *
 */
__intrinsic
void cmd_cls_ring_pop_ptr32(
    __xread void *data,
    volatile void __addr32 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Remove data from Cluster Local Scratch memory ring and put in transfer register.
 *
 * @param data     Data read from ring
 * @param address  40 bit ring address to read data from where upper 6 bits indicate the island.
 *                  The ring number is specified in address[5:2]
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * Read 32-bit words from tail of ring and update tail pointer.
 * If there are not enough entries in the ring, no valid data is returned
 * and the tail pointer is not updated.
 * This will raise an underflow event if the ring is configured to deliver one.
 * @note A single pop operation will return the words in the order they were added.
 *       If the ring contains the words (A, B, C, D, E), then a single pop operation of 3 words
 *       will return the values (C, D, E) - in that order.
 *
 */
__intrinsic
void cmd_cls_ring_pop_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Read data from Cluster Local Scratch memory ring in a safe way and put in transfer register.
 *
 * @param data     Data read from ring
 * @param address  Ring address to read data from where the ring number is specified in address[5:2]
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * The ring get safe attempts to remove the count of 32-bit words from the ring.
 * It will permit any number of entries to be removed up to the desired length.
 * If there are fewer entries on the ring, then the ring will be emptied,
 * and zero will be returned for each entry that was not available from the ring.
 */
__intrinsic
void cmd_cls_ring_get_safe_ptr32(
    __xread void *data,
    volatile void __addr32 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read data from Cluster Local Scratch memory ring in a safe way and put in transfer register.
 *
 * @param data     Data read from ring
 * @param address  40 bit ring address to read data from where upper 6 bits indicate the island.
 *                  The ring number is specified in address[5:2]
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * The ring get safe attempts to remove the count of 32-bit words from the ring.
 * It will permit any number of entries to be removed up to the desired length.
 * If there are fewer entries on the ring, then the ring will be emptied,
 * and zero will be returned for each entry that was not available from the ring.
 */
__intrinsic
void cmd_cls_ring_get_safe_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Remove data from Cluster Local Scratch memory ring in a safe way and put in transfer register.
 *
 * @param data     Data read from ring
 * @param address  Ring address to read data from where the ring number is specified in address[5:2]
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * The ring pop attempts to remove the count of 32-bit words from the ring.
 * It will permit any number of entries to be removed up to the desired length.
 * If there are fewer entries on the ring, then the ring will be emptied,
 * and zero will be returned for each entry that was not available from the ring.
 * @note A single pop operation will return the words in the order they were added.
 *       If the ring contains the words (A, B, C, D, E), then two pop operations of 3 words
 *       will return the values (C, D, E) and (A, B, 0) - in that order.
 *
 */
__intrinsic
void cmd_cls_ring_pop_safe_ptr32(
    __xread void *data,
    volatile void __addr32 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Remove data from Cluster Local Scratch memory ring in a safe way and put in transfer register.
 *
 * @param data     Data read from ring
 * @param address  40 bit ring address to read data from where upper 6 bits indicate the island.
 *                  The ring number is specified in address[5:2]
 *                  See 6xxx databook for recommended addressing mode.
 * @param count    Number of 32-bit words to read
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 * The ring pop attempts to remove the count of 32-bit words from the ring.
 * It will permit any number of entries to be removed up to the desired length.
 * If there are fewer entries on the ring, then the ring will be emptied,
 * and zero will be returned for each entry that was not available from the ring.
 * @note A single pop operation will return the words in the order they were added.
 *       If the ring contains the words (A, B, C, D, E), then two pop operations of 3 words
 *       will return the values (C, D, E) and (A, B, 0) - in that order.
 *
 */
__intrinsic
void cmd_cls_ring_pop_safe_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Initialise a Cluster Local Scratch memory ring
 *
 * @param ring_number   Ring number to initialise (0 to 15)
 * @param base_address  Base address of ring which could be anywhere in CLS. The base address
 *                       should also be aligned to either 128, 256, 512, 1024 or 2048 byte boundary
 *                       as specified in enum CLS_RING_SIZE.
 * @param size          Size of ring (0 to 5) of type enum CLS_RING_SIZE.
 * @param report_events List of events of enum CLS_RING_EVENT_REPORTS which should be generated
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Setup a ring of specified ring number, size and base address in Cluster Local Scratch memory.
 *
 */
__intrinsic
void cmd_cls_ring_init_ptr32(
    uint32_t ring_number,
    volatile void __addr32 __cls *base_address,
    enum CLS_RING_SIZE size,
    uint32_t report_events,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Initialise a Cluster Local Scratch memory ring
 *
 * @param ring_number   Ring number to initialise (0 to 15)
 * @param base_address  40 bit pointer base address of ring which could be anywhere in CLS. The base address
 *                       should also be aligned to either 128, 256, 512, 1024 or 2048 byte boundary
 *                       as specified in enum CLS_RING_SIZE.  The top bits of the address must be set to
 *                       the island as in 6xxx databook for recommended addressing mode for higher 8 bits
 *                       of address parameter.
 * @param size          Size of ring (0 to 5) of type enum CLS_RING_SIZE.
 * @param report_events List of events of enum CLS_RING_EVENT_REPORTS which should be generated
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Setup a ring of specified ring number, size and base address in Cluster Local Scratch memory.
 *
 * The following example shows how to create a ring in cluster local scratch on i34.
 * @code
 *
 * {
 *    SIGNAL                              sig;
 *    uint32_t                        ring_index = 15;
 *
 *    __addr40 __cls void *ring_address = (__addr40 __cls void *)
 *              (LoadTimeConstant("__ADDR_I34_CLS") | (ring_index << 2));
 *    __declspec(i34.cls, addr40, aligned(32*sizeof(uint32_t))) uint32_t ring_base[32];
 *
 *    // initialise ring
 *    {
 *        cmd_cls_ring_init_ptr40(
 *            ring_index,
 *            ring_base,
 *            CLS_RING_SIZE_32,
 *            0,
 *            sig_done,
 *            &sig
 *            );
 *
 *        __wait_for_all(&sig);
 *    }
 *
 *    // write four 32-bit words to the ring
 *    {
 *        __xwrite uint32_t  write_data[4];
 *
 *        write_data[0] = 0x11223344;
 *        write_data[1] = 0xaabbccdd;
 *        write_data[2] = 0x55667788;
 *        write_data[3] = 0x9900aabb;
 *        cmd_cls_ring_put_ptr40((void*)write_data, ring_address, 4, ctx_swap, &sig);
 *    }
 *
 *    // read one 32-bit word from the tail of the ring
 *    {
 *        __xread uint32_t   read_data;
 *
 *        // pop from tail
 *        cmd_cls_ring_pop_ptr40((void*)&read_data, ring_address, 1, ctx_swap, &sig);
 *
 *        if (read_data != 0x9900aabb)
 *        {
 *            return 0;         // We have an error
 *        }
 *    }
 *    // read one 32-bit word from the head of the ring
 *    {
 *        __xread uint32_t   read_data;
 *
 *        // get from head
 *        cmd_cls_ring_get_ptr40((void*)&read_data, ring_address, 1, ctx_swap, &sig);
 *
 *        if (read_data != 0x11223344)
 *        {
 *            return 0;         // We have an error
 *        }
 *    }
 *
 *    return 1;
 * }
 * @endcode
 */
__intrinsic
void cmd_cls_ring_init_ptr40(
    uint32_t ring_number,
    volatile void __addr40 __cls *base_address,
    enum CLS_RING_SIZE size,
    uint32_t report_events,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Write data on Cluster Local Scratch memory ring, starting at the specified offset
 *
 * @param data          Data to write to ring
 * @param ring_number   Ring number to write data to (0 to 15)
 * @param count         Number of 32-bit words to put into ring (1 to 32)
 * @param offset        Offset from base for ring to write; will be bounded by the ring size
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Write to a specified offset within a ring, bounded by the ring size
 * in Cluster Local Scratch memory.
 * This is used to add entries at an offset beyond the current tail pointer without updating
 * the tail, and to later update the tail pointer without adding any entries. This can be used
 * to, e.g., reorder the packets in a queue.
 *
 */
__intrinsic
void cmd_cls_ring_write_ptr32(
    __xwrite void *data,
    uint32_t ring_number,
    uint32_t count,
    uint32_t offset,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write data on Cluster Local Scratch memory ring, starting at the specified offset
 *
 * @param data          Data to write to ring
 * @param ring_number   Ring number to write data to (0 to 15)
 * @param count         Number of 32-bit words to put into ring (1 to 32)
 * @param offset        Offset from base for ring to write; will be bounded by the ring size
 * @param island_id     Island id indicating CLS locality. See 6xxx databook for Local Scratch
 *                       recommended addressing mode.
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Write to a specified offset within a ring, bounded by the ring size
 * in Cluster Local Scratch memory specified by the island_id.
 * This is used to add entries at an offset beyond the current tail pointer without updating
 * the tail, and to later update the tail pointer without adding any entries. This can be used
 * to, e.g., reorder the packets in a queue.
 *
 */
__intrinsic
void cmd_cls_ring_write_ptr40(
    __xwrite void *data,
    uint32_t ring_number,
    uint32_t count,
    uint32_t offset,
    uint32_t island_id,
    sync_t sync,
    SIGNAL *sig_ptr
);




/** Read data on Cluster Local Scratch memory ring, starting at the specified offset
 *
 * @param data          Data read from ring
 * @param ring_number   Ring number to read data from (0 to 15)
 * @param count         Number of 32-bit words to read (1 to 16)
 * @param offset        Offset from base for ring to read; will be bounded by the ring size
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Read from a specified offset within a ring, bounded by the ring size
 * in Cluster Local Scratch memory.
 *
 */
__intrinsic
void cmd_cls_ring_read_ptr32(
    __xread void *data,
    uint32_t ring_number,
    uint32_t count,
    uint32_t offset,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read data on Cluster Local Scratch memory ring, starting at the specified offset
 *
 * @param data          Data read from ring
 * @param ring_number   Ring number to read data from (0 to 15)
 * @param count         Number of 32-bit words to read (1 to 16)
 * @param offset        Offset from base for ring to read; will be bounded by the ring size
 * @param island_id     Island id indicating CLS locality. See 6xxx databook for Local Scratch
 *                       recommended addressing mode.
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Read from a specified offset within a ring, bounded by the ring size
 * in Cluster Local Scratch memory.
 *
 */
__intrinsic
void cmd_cls_ring_read_ptr40(
    __xread void *data,
    uint32_t ring_number,
    uint32_t count,
    uint32_t offset,
    uint32_t island_id,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Lock a Cluster Local Scratch memory ring for reordering
 *
 * @param ring_number       Ring number to read data from (0 to 15)
 * @param sequence_number   Sequence number to reorder on.
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * Permits a ring to be locked for reordering. Compare sequence number with ring head pointer
 * on a specified ring. If match is found then push signal only, otherwise write the
 * signal information to the ring at offset specified by sequence number.
 *
 */
__intrinsic
void cmd_cls_ring_ordered_lock_ptr32(
    uint32_t ring_number,
    uint32_t sequence_number,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Lock a Cluster Local Scratch memory ring for reordering
 *
 * @param ring_number       Ring number to read data from (0 to 15)
 * @param sequence_number   Sequence number to reorder on.
 * @param island_id         Island id indicating CLS locality. See 6xxx databook for Local Scratch
 *                          recommended addressing mode.
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * Permits a ring to be locked for reordering. Compare sequence number with ring head pointer
 * on a specified ring. If match is found then push signal only, otherwise write the
 * signal information to the ring at offset specified by sequence number.
 *
 */
__intrinsic
void cmd_cls_ring_ordered_lock_ptr40(
    uint32_t ring_number,
    uint32_t sequence_number,
    uint32_t island_id,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Unlock a Cluster Local Scratch memory ring after reordering
 *
 * @param ring_number       Ring number to read data from (0 to 15)
 * @param sequence_number   Sequence number
 *
 * Unlocks a ring after reordering. Increment the head pointer on the specified ring.
 * Read the ring at the new head pointer and overwrite with zero, if the data was
 * non-zero then push to the signal data that was read.
 *
 * @ind_6000
 */
__intrinsic
void cmd_cls_ring_ordered_unlock_ptr32(
    uint32_t ring_number,
    uint32_t sequence_number
);


/** Unlock a Cluster Local Scratch memory ring after reordering
 *
 * @param ring_number       Ring number to read data from (0 to 15)
 * @param sequence_number   Sequence number
 * @param island_id         Island id indicating CLS locality. See 6xxx databook for Local Scratch
 *                           recommended addressing mode.
 *
 * Unlocks a ring after reordering. Increment the head pointer on the specified ring.
 * Read the ring at the new head pointer and overwrite with zero, if the data was
 * non-zero then push to the signal data that was read.
 *
 * @ind_6000
 */
__intrinsic
void cmd_cls_ring_ordered_unlock_ptr40(
    uint32_t ring_number,
    uint32_t sequence_number,
    uint32_t island_id
);

/** Add a thread to a Cluster Local Scratch memory ring (queue)
 *
 * @param data          Work received from ring
 * @param ring_number   Ring number to read data from (0 to 15)
 * @param count         Number of 32-bit words to read (1 to 16)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Add a thread to the work queue. Adding a thread to a queue that contains
 * work will get the first work and deliver it to the thread. If there are no
 * work on the queue, the thread is added to the ring.
 *
 */
__intrinsic
void cmd_cls_ring_workq_add_thread_ptr32(
    __xread void *data,
    uint32_t ring_number,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Add a thread to a Cluster Local Scratch memory ring (queue)
 *
 * @param data          Work received from ring
 * @param ring_number   Ring number to read data from (0 to 15)
 * @param count         Number of 32-bit words to read (1 to 16)
 * @param island_id     Island id indicating CLS locality. See 6xxx databook for Local Scratch
 *                       recommended addressing mode.
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Add a thread to the work queue. Adding a thread to a queue that contains
 * work will get the first work and deliver it to the thread. If there are no
 * work on the queue, the thread is added to the ring.
 *
 */
__intrinsic
void cmd_cls_ring_workq_add_thread_ptr40(
    __xread void *data,
    uint32_t ring_number,
    uint32_t count,
    uint32_t island_id,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Add work to a Cluster Local Scratch memory ring (queue)
 *
 * @param data          Work written to ring
 * @param ring_number   Ring number to read data from (0 to 15)
 * @param count         Number of 32-bit words to read (1 to 16)
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Adding work to a queue that contains threads will get the first thread and
 * deliver the work to it. If there are no threads on the ring, the work is written
 * to the ring.
 *
 */
__intrinsic
void cmd_cls_ring_workq_add_work_ptr32(
    __xwrite void *data,
    uint32_t ring_number,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Add work to a Cluster Local Scratch memory ring (queue)
 *
 * @param data          Work written to ring
 * @param ring_number   Ring number to read data from (0 to 15)
 * @param count         Number of 32-bit words to read (1 to 16)
 * @param island_id     Island id indicating CLS locality. See 6xxx databook for Local Scratch
 *                       recommended addressing mode.
 * @param sync          Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr       Signal to raise upon completion
 *
 * Adding work to a queue that contains threads will get the first thread and
 * deliver the work to it. If there are no threads on the ring, the work is written
 * to the ring.
 *
 */
__intrinsic
void cmd_cls_ring_workq_add_work_ptr40(
    __xwrite void *data,
    uint32_t ring_number,
    uint32_t count,
    uint32_t island_id,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write data to tail of ring in Cluster Local Scratch memory in indirect mode.
 * If ring full nothing added and pointer not updated
 *
 * @param data     Data to write  to tail of ring
 * @param address  40 bit Cluster Local Scratch address
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn   Maximum number of 32-bit words to put into ring
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */

__intrinsic
void cmd_cls_ring_put_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to tail of ring in Cluster Local Scratch memory in indirect mode.
 * If ring full nothing added and pointer not updated
 *
 * @param data     Data to write  to tail of ring
 * @param address  32 bit Cluster Local Scratch address
 * @param max_nn   Maximum number of 32-bit words to put into ring
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_cls_ring_put_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Write data to tail of ring in Cluster Local Scratch memory in indirect mode.
 * Always update pointer, no concept of overflow
 *
 * @param data     Data to write  to tail of ring
 * @param address  40 bit Cluster Local Scratch address
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn   Maximum number of 32-bit words to put into ring
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */

__intrinsic
void cmd_cls_ring_journal_ind_ptr40(
    __xwrite void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Write data to tail of ring in Cluster Local Scratch memory in indirect mode.
 * Always update pointer, no concept of overflow
 *
 * @param data     Data to write  to tail of ring
 * @param address  32 bit Cluster Local Scratch address
 * @param max_nn   Maximum number of 32-bit words to put into ring
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 */
__intrinsic
void cmd_cls_ring_journal_ind_ptr32(
    __xwrite void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from Cluster Local Scratch memory head of ring and put into transfer register, with indirect mode
 * Empty then underflow event, data invalid
 *
 * @param data     Data read from ring
 * @param address  40 bit address
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn   Maximum number of 32-bit words to read
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_cls_ring_get_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** Read data from Cluster Local Scratch memory head of ring and put into transfer register in, with indirect mode
 * Empty then underflow event, data invalid
 *
 * @param data     Data read from ring
 * @param address  Cluster Local Scratch 32 bit address
 * @param max_nn   Maximum number of 32-bit words to read
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_cls_ring_get_ind_ptr32(
    __xread void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from Cluster Local Scratch memory tail of ring and put into transfer register in
 * indirect mode.
 *
 * @param data     Data read from ring
 * @param address  40 bit address
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn   Maximum number of 32-bit words to read
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_cls_ring_pop_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from Cluster Local Scratch memory tail of ring and put into transfer register in
 * indirect mode.
 *
 * @param data     Data read from ring
 * @param address  Cluster Local Scratch 32 bit address
 * @param max_nn   Maximum number of 32-bit words to read
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_cls_ring_pop_ind_ptr32(
    __xread void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from Cluster Local Scratch memory head of ring and put into transfer register, with indirect mode
 * Empty then no underflow event, data will be zero
 *
 * @param data     Data read from ring
 * @param address  40 bit address
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn   Maximum number of 32-bit words to read
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_cls_ring_get_safe_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from Cluster Local Scratch memory head of ring and put into transfer register in, with indirect mode
 * Empty then no underflow event, data will be zero
 *
 * @param data     Data read from ring
 * @param address  Cluster Local Scratch 32 bit address
 * @param max_nn   Maximum number of 32-bit words to read
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_cls_ring_get_safe_ind_ptr32(
    __xread void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );




/** Read data from Cluster Local Scratch memory tail of ring and put into transfer register, with indirect mode
 * Empty then no underflow event, data will be zero
 *
 * @param data     Data read from ring
 * @param address  40 bit address
 *                 See 6xxx databook for recommended addressing mode.
 * @param max_nn   Maximum number of 32-bit words to read
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */

__intrinsic
void cmd_cls_ring_pop_safe_ind_ptr40(
    __xread void *data,
    volatile void __addr40 __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );


/** Read data from Cluster Local Scratch memory tail of ring and put into transfer register, with indirect mode
 * Empty then no underflow event, data will be zero
 *
 * @param data     Data read from ring
 * @param address  32 bit address
 * @param max_nn   Maximum number of 32-bit words to read
 * @param ind      Indirect word
 * @param sync     Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr  Signal to raise upon completion
 *
 */
__intrinsic
void cmd_cls_ring_pop_safe_ind_ptr32(
    __xread void *data,
    volatile void __cls *address,
    uint32_t max_nn,
    generic_ind_t ind,
    sync_t sync,
    SIGNAL *sig_ptr
    );

/** @}
 * @}
 */

#endif /* __NFP_CLS_RING_H__ */
