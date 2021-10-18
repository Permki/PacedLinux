/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* DRAM list extended functions */
#ifndef __NFP_MEM_LIST_H__
#define __NFP_MEM_LIST_H__

#include <nfp.h>
#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif


/** @file nfp_mem_list.h
 * @addtogroup mu_list MU List Intrinsics
 * @{
 */

/** Available list types.
 */
typedef enum MEM_LIST_TYPE
{
    /** Dequeue Segments and Count Packets.
     *
     * Segment-based linked list: each link includes SOP, EOP, a segment count, and a 24-bit long word address.
     * Type 0 allows multiple packets to be stored per buffer in a chain of buffers.
     * If this is the first entry on the list, the enqueue operation sets the tail and head pointers to the element being enqueued.
     * The segment count, EOP and SOP is set from the enqueue command. Queue count is set to one.
     * When there are already entries on the list, the enqueue operation writes the new entry at the tail pointer, updates
     * the tail pointer in the queue array to point to the new entry and increments the count in the queue array.
     * The dequeue operation decrements the segment count value and only removes the entry from the list when the
     * segment count is zero. The queue count is only decremented when the EOP bit is set.
     */
    TYPE_0,

    /** Dequeue Buffers and Count Packets.
     *
     * Buffer-based linked list counting packets: each link includes SOP, EOP, a 24-bit byte address plus 6 bits of user data.
     * If this is the first entry on the list, the enqueue operation sets the tail and head pointers to the buffer being enqueued.
     * The segment count, EOP and SOP is set from the enqueue command. Queue count is set to one.
     * When there are already entries on the list, the enqueue operation writes the new entry at the tail pointer, updates
     * the tail pointer in the queue array to point to the new entry and increments the count in the queue array.
     * The dequeue command always removes the entry from the head of the list, but only decrements the queue count
     * when the EOP bit is set.
     * Type 1 lists are typically used when packets are larger than a single buffer, allowing a packet to be stored across
     * a few buffers.
     * This type of list also provides a 6 bit field which can be used for user data which is unaltered between enqueue and
     * dequeue operations.
     */
    TYPE_1,

    /**  Dequeue Buffers and Count Buffers.
     *
     * 32-bit buffer-based linked list counting buffers: each link includes a 30-bit long word address.
     * If this is the first entry on the list, the enqueue operation sets the tail and head pointers to the buffer being enqueued.
     * The segment count, EOP and SOP is set from the enqueue command. The queue count is set to one.
     * When there are already entries on the list, the enqueue operation writes the new entry at the tail pointer in memory,
     * updates the tail pointer in the queue array to point to the new entry and increments the queue count in the queue
     * array by one.
     * The dequeue command always removes the entry from the head of the list, but only decrements the queue count
     * when the EOP bit for the entry was set on the enqueue command. The values for SOP and EOP in the link returned
     * from the dequeue command, are inverted from the values used for the enqueue command. The values in the queue
     * array is the same as the enqueue command.
     * Type two lists are typically used for single buffer per packet operations where all the buffers are in the same 4GB
     * window.
     *
     */
    TYPE_2,

    /**  Dequeue Buffers and Count Buffers.
     *
     * 24-bit buffer-based linked list counting buffers: each link includes a 24-bit word address.
     * If this is the first entry on the list, the enqueue operation sets the tail and head pointers to the buffer being enqueued.
     * The SOP and EOP is set from the enqueue command and the queue count is set from the segment count.
     * When there are already entries on the list, the enqueue operation writes the new entry at the tail pointer in memory,
     * updates the tail pointer in the queue array to point to the new entry and adds the segment count to the count in the
     * queue array. It is illegal to use a segment count of 0 for the enqueue command.
     * The dequeue command always removes the entry from the head of the list and decrements the queue count by one.
     * The values for SOP and EOP are the same as used for the enqueue command.
     * Type three lists are typically used for single buffer per packet operations with all the buffers in the same 64MB
     * window.
     */
    TYPE_3,
} MEM_LIST_TYPE;

/*
 * Type 00 => cell-based linked list: each link includes SOP, EOP, a segment count, and a 24-bit word address which
 * must have the bottom 2 bits clear.
 */
#ifdef __BIGENDIAN

/** List type 0 descriptor type. */
typedef union mem_list_desc_t0
{
    struct
    {
        /*
         * First list buffer descriptor longword
         */
        uint32_t seg_cnt        : 6;    /**< Segment count. */
        uint32_t head_ptr       : 24;   /**< Word address of first buffer. */
        uint32_t eop            : 1;    /**< End Of Packet indication. */
        uint32_t sop            : 1;    /**< Start Of Packet indication. */
        /*
         * Second list buffer descriptor longword
         */
        uint32_t tail_ptr       : 30;   /**< Word address of last(tail) buffer of the list. */
        uint32_t q_type         : 2;    /**< Set to TYPE_0. @see TYPE_0. */
        /*
         * Third list buffer descriptor longword
         */
        uint32_t q_loc          : 2;    /**< Locality specification of list. */
        uint32_t reserved_1     : 4;    /**< Reserved. */
        uint32_t q_page         : 2;    /**< Page of list storage. */
        uint32_t q_count        : 24;   /**< Number of entries in list. */
        /*
         * Fourth longword
         */
        uint32_t reserved_2;            /**< Reserved. */
    };
    uint32_t value[4];                  /**< Accessor to entire structure. */
} mem_list_desc_t0;

/** List type 0 link type. */
typedef union mem_list_link_t0
{
    struct
    {
        uint32_t seg_cnt        : 6;    /**< Segment count. */
        uint32_t next_ptr       : 24;   /**< Pointer to next list entry. */
        uint32_t eop            : 1;    /**< End Of Packet indication. */
        uint32_t sop            : 1;    /**< Start Of Packet indication. */
    };
    uint32_t value;                     /**< Accessor to entire structure. */
} mem_list_link_t0;

/** List type 0 link descriptor format in memory. */
typedef union mem_list_link_mem_t0
{
    struct
    {
        uint32_t eop            : 1;    /**< End Of Packet indication. */
        uint32_t sop            : 1;    /**< Start Of Packet indication. */
        uint32_t seg_cnt        : 6;    /**< Segment count. */
        uint32_t next_ptr       : 24;   /**< Pointer to next list entry. */
    };
    uint32_t value;                     /**< Accessor to entire structure. */
} mem_list_link_mem_t0;

#else /* __LITTLEENDIAN */

/* See BE description above */
typedef union mem_list_desc_t0
{
    struct
    {
        uint32_t sop            : 1;
        uint32_t eop            : 1;
        uint32_t head_ptr       : 24;
        uint32_t seg_cnt        : 6;
        uint32_t q_type         : 2;
        uint32_t tail_ptr       : 30;
        uint32_t q_count        : 24;
        uint32_t q_page         : 2;
        uint32_t reserved_1     : 4;
        uint32_t q_loc          : 2;
        uint32_t reserved_2;
    };
    uint32_t value[4];
} mem_list_desc_t0;

typedef union mem_list_link_t0
{
    struct
    {
        uint32_t sop            : 1;
        uint32_t eop            : 1;
        uint32_t next_ptr       : 24;
        uint32_t seg_cnt        : 6;
    };
    uint32_t value;
} mem_list_link_t0;

typedef union mem_list_link_mem_t0
{
    struct
    {
        uint32_t next_ptr       : 24;
        uint32_t seg_cnt        : 6;
        uint32_t sop            : 1;
        uint32_t eop            : 1;
    };
    uint32_t value;
} mem_list_link_mem_t0;
#endif /* __BIGENDIAN */

/** List type 0 descriptor aligned in memory. */
typedef __addr40 __align16 mem_list_desc_t0      mem_list_desc_in_mem_t0;

/** List type 0 link descriptor aligned in memory. */
typedef __addr40 __align16 mem_list_link_mem_t0  mem_list_link_in_mem_t0;

#ifdef __BIGENDIAN
/** List type 1 descriptor type.
 *
 * Buffer-based linked list counting packets: each link includes SOP, EOP and a 24-bit word address.
 */
typedef union mem_list_desc_t1
{
    struct
    {
        /*
         * First list buffer descriptor longword
         */
        uint32_t user_data      : 6;    /**< 6 - bits of user data. */
        uint32_t head_ptr       : 24;   /**< Word address of first buffer. */
        uint32_t eop            : 1;    /**< End Of Packet indication. */
        uint32_t sop            : 1;    /**< Start Of Packet indication. */
        /*
         * Second list buffer descriptor longword
         */
        uint32_t tail_ptr       : 30;   /**< Word address of last(tail) buffer of the list. */
        uint32_t q_type         : 2;    /**< Set to TYPE_1. @see TYPE_1. */

        /*
         * Third list buffer descriptor longword
         */
        uint32_t q_loc          : 2;    /**< Locality specification of list. */
        uint32_t reserved_1     : 4;    /**< Reserved. */
        uint32_t q_page         : 2;    /**< Page of list storage. */
        uint32_t q_count        : 24;   /**< Number of entries in list. */

        /*
         * Fourth longword
         */
        uint32_t reserved_2;            /**< Reserved. */
    };
    uint32_t value[4];                  /**< Accessor to entire descriptor structure. */
} mem_list_desc_t1;

/** List type 1 link type. */
typedef union mem_list_link_t1
{
    struct
    {
        uint32_t user_data      : 6;    /**< 6 - bits of user data. */
        uint32_t next_ptr       : 24;   /**< Word address of next list entry. */
        uint32_t eop            : 1;    /**< End Of Packet indication. */
        uint32_t sop            : 1;    /**< Start Of Packet indication. */
    };
    uint32_t value;                     /**< Accessor to entire structure. */
} mem_list_link_t1;

/** List type 1 link descriptor format in memory.
 * This type is typically used when a linked list is construct in memory using a ME.
 */
typedef union mem_list_link_mem_t1
{
    struct
    {
        uint32_t eop            : 1;    /**< End Of Packet indication. */
        uint32_t sop            : 1;    /**< Start Of Packet indication. */
        uint32_t user_data      : 6;    /**< 6 - bits of user data. */
        uint32_t next_ptr       : 24;   /**< Word address of next list entry. */
    };
    uint32_t value;                     /**< Accessor to entire structure. */
} mem_list_link_mem_t1;
#else /* __LITTLEENDIAN */

/* See BE description above */
typedef union mem_list_desc_t1
{
    struct
    {
        uint32_t sop            : 1;
        uint32_t eop            : 1;
        uint32_t head_ptr       : 24;
        uint32_t user_data      : 6;
        uint32_t q_type         : 2;
        uint32_t tail_ptr       : 30;
        uint32_t q_count        : 24;
        uint32_t q_page         : 2;
        uint32_t reserved_1     : 4;
        uint32_t q_loc          : 2;
        uint32_t reserved_2;
    };
    uint32_t value[4];
} mem_list_desc_t1;

typedef union mem_list_link_t1
{
    struct
    {
        uint32_t sop            : 1;
        uint32_t eop            : 1;
        uint32_t next_ptr       : 24;
        uint32_t user_data      : 6;
    };
    uint32_t value;
} mem_list_link_t1;

typedef union mem_list_link_mem_t1
{
    struct
    {
        uint32_t next_ptr       : 24;
        uint32_t user_data      : 6;
        uint32_t sop            : 1;
        uint32_t eop            : 1;
    };
    uint32_t value;
} mem_list_link_mem_t1;
#endif /* __BIGENDIAN */

/** List type 1 descriptor aligned in memory.
 */
typedef __addr40 __align16 mem_list_desc_t1      mem_list_desc_in_mem_t1;

/** List type 1 link aligned in memory.
 */
typedef __addr40 __align16 mem_list_link_mem_t1  mem_list_link_in_mem_t1;

#ifdef __BIGENDIAN
/** List type 2 descriptor type.
 *
 * 32-bit buffer based linked list counting buffers.
 */
typedef union mem_list_desc_t2
{
    struct
    {
        /*
         * First list buffer descriptor longword
         */
        uint32_t head_ptr       : 30;   /**< Word address of first buffer. */
        uint32_t eop            : 1;    /**< End Of Packet indication. */
        uint32_t sop            : 1;    /**< Start Of Packet indication. */
        /*
         * Second list buffer descriptor longword
         */
        uint32_t tail_ptr       : 30;   /**< Word address of last(tail) buffer of the list. */
        uint32_t q_type         : 2;    /**< Set to TYPE_2. @see TYPE_2. */

        /*
         * Third list buffer descriptor longword
         */
        uint32_t q_loc          : 2;    /**< Locality specification of list. */
        uint32_t reserved_1     : 4;    /**< Reserved. */
        uint32_t q_page         : 2;    /**< Page of list storage. */
        uint32_t q_count        : 24;   /**< Number of entries in list. */

        /*
         * Fourth longword
         */
        uint32_t reserved_2;            /**< Reserved. */
    };
    uint32_t value[4];                  /**< Accessor to entire descriptor structure. */
} mem_list_desc_t2;

/** List type 2 link type. */
typedef union mem_list_link_t2
{
    struct
    {
        uint32_t next_ptr       : 30;   /**< Word address of next list entry. */
        uint32_t eop            : 1;    /**< End Of Packet indication. */
        uint32_t sop            : 1;    /**< Start Of Packet indication. */
    };
    uint32_t value;                     /**< Accessor to entire structure. */
} mem_list_link_t2;

/** List type 2 link descriptor format in memory.
 * This type is typically used when a linked list is construct in memory using a ME.
 */
typedef union mem_list_link_mem_t2
{
    struct
    {
        uint32_t next_ptr       : 30;   /**< Word address of next list entry. */
        uint32_t eop            : 1;    /**< End Of Packet indication. */
        uint32_t sop            : 1;    /**< Start Of Packet indication. */
    };
    uint32_t value;                     /**< Accessor to entire structure. */
} mem_list_link_mem_t2;
#else /* __LITTLEENDIAN */

/* See BE description above */
typedef struct
{
    uint32_t sop                : 1;
    uint32_t eop                : 1;
    uint32_t head_ptr           : 30;
    uint32_t q_type             : 2;
    uint32_t tail_ptr           : 30;
    uint32_t q_count            : 24;
    uint32_t q_page             : 2;
    uint32_t reserved_1         : 4;
    uint32_t q_loc              : 2;
    uint32_t reserved_2;
} mem_list_desc_t2;

typedef union mem_list_link_t2
{
    struct
    {
        uint32_t sop            : 1;
        uint32_t eop            : 1;
        uint32_t next_ptr       : 30;
    };
    uint32_t value;
} mem_list_link_t2;

typedef union mem_list_link_mem_t2
{
    struct
    {
        uint32_t sop            : 1;
        uint32_t eop            : 1;
        uint32_t next_ptr       : 30;
    };
    uint32_t value;
} mem_list_link_mem_t2;
#endif /* __BIGENDIAN */

/** List type 2 descriptor aligned in memory. */
typedef __addr40 __align16 mem_list_desc_t2      mem_list_desc_in_mem_t2;

/** List type 2 link aligned in memory.
 * This type is typically used when a linked list is construct in memory using a ME
 */
typedef __addr40 __align16 mem_list_link_mem_t2  mem_list_link_in_mem_t2;

#ifdef __BIGENDIAN
/** List Type 3 descriptor type.
 * 24-bit buffer-based linked list counting buffers.
 */
typedef union mem_list_desc_t3
{
    struct
    {
        /*
         * First list buffer descriptor longword
         */
        uint32_t seg_cnt        : 6;    /**< Segment count. */
        uint32_t head_ptr       : 24;   /**< Word address of first buffer. */
        uint32_t eop            : 1;    /**< End Of Packet indication. */
        uint32_t sop            : 1;    /**< Start Of Packet indication. */
        /*
         * Second list buffer descriptor longword
         */
        uint32_t tail_ptr       : 30;   /**< Word address of last(tail) buffer of the list. */
        uint32_t q_type         : 2;    /**< Set to TYPE_3. @see TYPE_3. */

        /*
         * Third list buffer descriptor longword
         */
        uint32_t q_loc          : 2;    /**< Locality specification of list. */
        uint32_t reserved_1     : 4;    /**< Reserved. */
        uint32_t q_page         : 2;    /**< Page of list storage. */
        uint32_t q_count        : 24;   /**< Number of entries in list. */

        /*
         * Fourth longword
         */
        uint32_t reserved_2;            /**< Reserved. */
    };
    uint32_t value[4];                  /**< Accessor to entire descriptor structure. */
} mem_list_desc_t3;

/** List type 3 link type. */
typedef union mem_list_link_t3
{
    struct
    {
        uint32_t seg_cnt        : 6;    /**< Segment count. */
        uint32_t next_ptr       : 24;   /**< Word address of next list entry. */
        uint32_t eop            : 1;    /**< End Of Packet indication. */
        uint32_t sop            : 1;    /**< Start Of Packet indication. */
    };
    uint32_t value;                     /**< Accessor to entire structure. */
} mem_list_link_t3;

/** List type 3 link descriptor format in memory.
 * This type is typically used when a linked list is construct in memory using a ME.
 */
typedef union mem_list_link_mem_t3
{
    struct
    {
        uint32_t eop            : 1;    /**< End Of Packet indication. */
        uint32_t sop            : 1;    /**< Start Of Packet indication. */
        uint32_t seg_cnt        : 6;    /**< Segment count. */
        uint32_t next_ptr       : 24;   /**< Word address of next list entry. */
    };
    uint32_t value;                     /**< Accessor to entire structure. */
} mem_list_link_mem_t3;
#else /* __LITTLEENDIAN */

/* See BE description above */
typedef union mem_list_desc_t3
{
    struct
    {
        uint32_t sop            : 1;
        uint32_t eop            : 1;
        uint32_t head_ptr       : 24;
        uint32_t seg_cnt        : 6;
        uint32_t q_type         : 2;
        uint32_t tail_ptr       : 30;
        uint32_t q_count        : 24;
        uint32_t q_page         : 2;
        uint32_t reserved_1     : 4;
        uint32_t q_loc          : 2;
        uint32_t reserved_2;
    };
    uint32_t value[4];
} mem_list_desc_t3;

typedef union mem_list_link_t3
{
    struct
    {
        uint32_t sop            : 1;
        uint32_t eop            : 1;
        uint32_t next_ptr       : 24;
        uint32_t seg_cnt        : 6;
    };
    uint32_t value;
} mem_list_link_t3;

typedef union mem_list_link_mem_t3
{
    struct
    {
        uint32_t next_ptr       : 24;
        uint32_t seg_cnt        : 6;
        uint32_t sop            : 1;
        uint32_t eop            : 1;
    };
    uint32_t value;
} mem_list_link_mem_t3;
#endif /* __BIGENDIAN */


/** List type 3 descriptor aligned in memory. */
typedef __addr40 __align16 mem_list_desc_t3      mem_list_desc_in_mem_t3;

/** List type 3 link aligned in memory. */
typedef __addr40 __align16 mem_list_link_mem_t3  mem_list_link_in_mem_t3;


/** @name List functions for MEM
 * @{
 */

/** Read MEM linked list queue descriptor for type 0 lists from MEM into queue array.
 *
 * @param q_array_number        queue array number in which to load. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param mem_list_desc_in_mem  40 bit pointer to list descriptor in external memory including the MU island. The pointer must be 16 byte aligned.
 *
 */
__intrinsic
void cmd_mem_list_read_desc_t0(
    uint32_t q_array_number,
    mem_list_desc_in_mem_t0 *mem_list_desc_in_mem
);

/** Read MEM linked list queue descriptor for type 1 lists from MEM into queue array.
 *
 * @param q_array_number        queue array number in which to load. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param mem_list_desc_in_mem  40 bit pointer to list descriptor in external memory including the MU island. The pointer must be 16 byte aligned.
 *
 *
 */
__intrinsic
void cmd_mem_list_read_desc_t1(
    uint32_t q_array_number,
    mem_list_desc_in_mem_t1 *mem_list_desc_in_mem
);

/** Read MEM linked list queue descriptor for type 2 lists from MEM into queue array.
 *
 * @param q_array_number        queue array number in which to load. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param mem_list_desc_in_mem  40 bit pointer to list descriptor in external memory including the MU island. The pointer must be 16 byte aligned.
 *
 *
 */
__intrinsic
void cmd_mem_list_read_desc_t2(
    uint32_t q_array_number,
    mem_list_desc_in_mem_t2 *mem_list_desc_in_mem
);

/** Read MEM linked list queue descriptor for type 3 lists from MEM into queue array.
 *
 * @param q_array_number        queue array number in which to load. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param mem_list_desc_in_mem  40 bit pointer to list descriptor in external memory including the MU island. The pointer must be 16 byte aligned.
 *
 *
 */
__intrinsic
void cmd_mem_list_read_desc_t3(
    uint32_t q_array_number,
    mem_list_desc_in_mem_t3 *mem_list_desc_in_mem
);


/** Push MEM queue descriptor from queue array into mem_list_desc_t0 residing in transfer registers.
 *
 * @param q_array_number   queue array number in which to push. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param xfer              Pointer to read xfer registers where the descriptor will be written
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 */
__intrinsic
void cmd_mem_list_push_desc_t0(
    uint32_t q_array_number,
    __xread mem_list_desc_t0 *xfer,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Push MEM queue descriptor from queue array into mem_list_desc_t1 residing in transfer registers.
 *
 * @param q_array_number   queue array number in which to push. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param xfer              Pointer to read xfer registers where the descriptor will be written
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 */
__intrinsic
void cmd_mem_list_push_desc_t1(
    uint32_t q_array_number,
    __xread mem_list_desc_t1 *xfer,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Push MEM queue descriptor from queue array into mem_list_desc_t2 residing in transfer registers.
 *
 * @param q_array_number   queue array number in which to push. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param xfer              Pointer to read xfer registers where the descriptor will be written
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 */
__intrinsic
void cmd_mem_list_push_desc_t2(
    uint32_t q_array_number,
    __xread mem_list_desc_t2 *xfer,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Push MEM queue descriptor from queue array into mem_list_desc_t3 residing in transfer registers.
 *
 * @param q_array_number   queue array number in which to push. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param xfer              Pointer to read xfer registers where the descriptor will be written
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 */
__intrinsic
void cmd_mem_list_push_desc_t3(
    uint32_t q_array_number,
    __xread mem_list_desc_t3 *xfer,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Write MEM queue descriptor from queue array into MEM for a type 0 list.
 *
 * @param q_array_number        queue array number to write to memory. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param mem_list_desc_in_mem  40 bit pointer to descriptor in external memory including the MU island where the descriptor will be written
 *
 */
__intrinsic
void cmd_mem_list_write_desc_t0(
    uint32_t q_array_number,
    mem_list_desc_in_mem_t0 *mem_list_desc_in_mem
);

/** Write MEM queue descriptor from queue array into MEM for a type 1 list.
 *
 * @param q_array_number        queue array number to write to memory. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param mem_list_desc_in_mem  40 bit pointer to descriptor in external memory including the MU island where the descriptor will be written
 *
 */
__intrinsic
void cmd_mem_list_write_desc_t1(
    uint32_t q_array_number,
    mem_list_desc_in_mem_t1 *mem_list_desc_in_mem
);

/** Write MEM queue descriptor from queue array into MEM for a type 2 list.
 *
 * @param q_array_number        queue array number in to write to memory. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param mem_list_desc_in_mem  40 bit pointer to descriptor in external memory including the MU island where the descriptor will be written.
 *
 *
 */
__intrinsic
void cmd_mem_list_write_desc_t2(
    uint32_t q_array_number,
    mem_list_desc_in_mem_t2 *mem_list_desc_in_mem
);

/** Write MEM queue descriptor from queue array into MEM for a type 3 list.
 *
 * @param q_array_number        queue array number to write to memory. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param mem_list_desc_in_mem  40 bit pointer to descriptor in external memory including the MU island where the descriptor will be written.
 *
 */
__intrinsic
void cmd_mem_list_write_desc_t3(
    uint32_t q_array_number,
    mem_list_desc_in_mem_t3 *mem_list_desc_in_mem
);

/** Add an entry to the tail of type 0 list.
 *
 * @param q_array_number        queue array number in which to enqueue entry. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param sop                   start of packet indication
 * @param eop                   end of packet indication
 * @param seg_cnt               segment count
 * @param next_ptr              40 bit pointer to next list descriptor in external memory including the MU island. The pointer must be 16 byte aligned.
 *
 * If the list is empty the head and tail pointers are set to point to the next_ptr parameter and
 * the q_count in the Q array to 1.
 *
 * If there is an entry on the list then the new entry is linked to the tail before
 * the tail is set to the next parameter. The q_count in the Q array is incremented.
 *
 *
 */
__intrinsic
void cmd_mem_list_enqueue_t0(uint32_t q_array_number, uint32_t sop, uint32_t eop, uint32_t seg_cnt,
                            mem_list_link_in_mem_t0 *next_ptr
                        );

/** Add an entry to the tail of a type 1 list.
 *
 * @param q_array_number        queue array number in which to enqueue entry. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param sop                   start of packet indication
 * @param eop                   end of packet indication
 * @param user_data             User Data (6 bits)
 * @param next_ptr              40 bit pointer to next list descriptor in external memory including the MU island. The pointer must be 16 byte aligned.

 * If the list is empty the head an tail pointers are set to point to the next_ptr parameter and
 * the q_count in the Q array to 1. The sop, eop and user_data fields is set from the call.
 *
 * If there is already an entry on the list then the new entry is linked to the tail before
 * the tail is set to the next parameter. The q_count in the Q array is incremented.
 *
 */
__intrinsic
void cmd_mem_list_enqueue_t1(uint32_t q_array_number, uint32_t sop, uint32_t eop, uint32_t user_data,
                            mem_list_link_in_mem_t1 *next_ptr
                        );

/** Add an entry to the tail of a type 2 list.
 *
 * @param q_array_number        queue array number in which to enqueue entry. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param sop                   start of packet indication
 * @param eop                   end of packet indication
 * @param next_ptr              40 bit pointer to next list descriptor in external memory including the MU island. The pointer must be 16 byte aligned.
 *
 * If the list is empty the head an tail pointers are set to point to the next_ptr parameter and
 * the q_count in the Q array to 1. sop and eop is set from the call.
 *
 * If there is already an entry on the list then the new entry is linked to the tail before
 * the tail is set to the next parameter. The q_count in the Q array is incremented.
 *
 *
 */
__intrinsic
void cmd_mem_list_enqueue_t2(uint32_t q_array_number, uint32_t sop, uint32_t eop, mem_list_link_in_mem_t2 *next_ptr);


/** Add an entry to the tail of a type 3 list.
 *
 * @param q_array_number        queue array number in which to enqueue entry. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param sop                   start of packet indication
 * @param eop                   end of packet indication
 * @param seg_cnt               segment count
 * @param next_ptr              40 bit pointer to next list descriptor in external memory including the MU island. The pointer must be 16 byte aligned.
 *
 * If the list is empty the head and tail pointers are set to point to the next_ptr parameter and
 * the q_count in the Q array to 1.
 *
 * If there is an entry on the list then the new entry is linked to the tail before
 * the tail is set to the next parameter. The q_count is set from the seg_cnt which must have a
 * value between 1 and 63.
 *
 *
 */
__intrinsic
void cmd_mem_list_enqueue_t3(uint32_t q_array_number, uint32_t sop, uint32_t eop, uint32_t seg_cnt,
                            mem_list_link_in_mem_t3 *next_ptr
                        );


/** Update the tail pointer of a type 0 list
 *
 * @param q_array_number        queue array number in which to enqueue entry. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param tail                  40 bit pointer to the tail of the list in external memory including the MU island. The pointer must be 16 byte aligned.
 *
 * This function is typically used when a list of packets is constructed in memory and
 * after being enqueued the tail pointer in the q_array needs to be updated.
 *
 *
 */
__intrinsic
void cmd_mem_list_enqueue_tail_t0(uint32_t q_array_number, mem_list_link_in_mem_t0 *tail);


/** Update the tail pointer of a type 1 list
 *
 * @param q_array_number        queue array number in which to enqueue entry. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param tail                  40 bit pointer to the tail of the list in external memory including the MU island. The pointer must be 16 byte aligned.
 *
 * This function is typically used when a list of packets is constructed in memory and
 * after being enqueued the tail pointer in the q_array needs to be updated.
 *
 */
__intrinsic
void cmd_mem_list_enqueue_tail_t1(uint32_t q_array_number, mem_list_link_in_mem_t1 *tail);


/** Update the tail pointer of a type 2 list
 *
 * @param q_array_number        queue array number in which to enqueue entry. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param tail                  40 bit pointer to the tail of the list in external memory including the MU island. The pointer must be 16 byte aligned.
 *
 * This function is used to set the tail pointer in the q_array.
 *
 */
__intrinsic
void cmd_mem_list_enqueue_tail_t2(uint32_t q_array_number, mem_list_link_in_mem_t2 *tail);


/** Update the tail pointer of a type 3 list
 *
 * @param q_array_number        queue array number in which to enqueue entry. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param tail                  40 bit pointer to the tail of the list in external memory including the MU island. The pointer must be 16 byte aligned.
 *
 * This function is typically used when a list of packets is constructed in memory and
 * after being enqueued the tail pointer in the q_array needs to be updated.
 *
 *
 */
__intrinsic
void cmd_mem_list_enqueue_tail_t3(uint32_t q_array_number, mem_list_link_in_mem_t3 *tail);


/** Remove an entry from the head of a type 0 list.
 *
 * @param mem_list_link         Pointer to read xfer registers where dequeued entry will be written.
 * @param q_array_number        queue array number to dequeue from. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param sync                  Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr               Signal to raise upon completion
 *
 * This function decrements the segment count value and only removes the entry
 * from the list when the segment count is zero. The queue count is only decremented
 * when the EOP bit is set.
 */
__intrinsic
void
cmd_mem_list_dequeue_t0(__xread mem_list_link_t0 *mem_list_link,
                        uint32_t q_array_number,
                        sync_t sync,
                        SIGNAL *sig_ptr
                    );


/** Remove an entry from the head of a type 1 list.
 *
 * @param mem_list_link         Pointer to read xfer registers where dequeued entry will be written.
 * @param q_array_number        queue array number to dequeue from. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param sync                  Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr               Signal to raise upon completion
 *
 * This function always removes the entry from the head of the list, but only
 * decrements the queue count when the EOP bit is set.
 */
__intrinsic
void
cmd_mem_list_dequeue_t1(__xread mem_list_link_t1 *mem_list_link,
                        uint32_t q_array_number,
                        sync_t sync,
                        SIGNAL *sig_ptr
                    );


/** Remove an entry from the head of a type 2 list.
 *
 * @param mem_list_link         Pointer to read xfer registers where dequeued entry will be written.
 * @param q_array_number        queue array number to dequeue from. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param sync                  Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr               Signal to raise upon completion
 *
 * The dequeue command always removes the entry from the head of the list, but only
 * decrements the queue count when the EOP bit for the entry was set on the enqueue
 * command. The values for SOP and EOP in the link returned from the dequeue command,
 * are inverted from the values used for the enqueue command.
 */
__intrinsic
void
cmd_mem_list_dequeue_t2(__xread mem_list_link_t2 *mem_list_link,
                        uint32_t q_array_number,
                        sync_t sync,
                        SIGNAL *sig_ptr
                    );


/** Remove an entry from the head of a type 3 list.
 *
 * @param mem_list_link         Pointer to read xfer registers where dequeued entry will be written.
 * @param q_array_number        queue array number to dequeue from. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param sync                  Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr               Signal to raise upon completion
 *
 * This function always removes the entry from the head of the list and
 * decrements the queue count by one. The values for SOP and EOP are the same
 * as used for the enqueue command.
 */
__intrinsic
void
cmd_mem_list_dequeue_t3(__xread mem_list_link_t3 *mem_list_link,
                        uint32_t q_array_number,
                        sync_t sync,
                        SIGNAL *sig_ptr
                    );



/** Initialize a memory list
 *
 * @param q_array_number        queue array number to use for list. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param type                  List type
 *
 * The following example shows how to create a list of type 3 in emem0 (i24). Note the setup of the q_array_number
 * to indicate that the list is in emem0.
 *
 * @code
 *
 *    // buf1 is on emem0 (i24)
 *    __emem_n(0) __addr40 mem_list_link_in_mem_t3  buf1;
 *   SIGNAL                                                sig;
 *   __xread  mem_list_desc_t3                descriptor;
 *
 *   // list is on emem0 (i24)
 *   uint32_t                                          q_array_number = 1023;
 *
 *    cmd_mem_list_init(q_array_number,TYPE_3);
 *
 *   // enqueue the entry and verify list count and list type
 *   {
 *        uint32_t    sop = 0;
 *        uint32_t    eop = 1;
 *        uint32_t    cell_count = 1;
 *
 *        cmd_mem_list_enqueue_t3(q_array_number, sop, eop, cell_count, &buf1);
 *        cmd_mem_list_push_desc_t3(q_array_number, &descriptor, ctx_swap, &sig);
 *
 *        if (descriptor.q_count != 1)
 *        {
 *            return 0;         // We have an error
 *        }
 *        if (descriptor.q_type != TYPE_3)
 *        {
 *            return 0;         // We have an error
 *        }
 *    }
 *
 *    // dequeue the entry and verify sop and eop
 *    {
 *        __xread mem_list_link_t3 result;
 *
 *        cmd_mem_list_dequeue_t3(&result,q_array_number,sig_done,&sig);
 *        wait_for_all(&sig);
 *
 *        if (result.sop != 0 || result.eop != 1)
 *       {
 *            return 0;         // We have an error
 *       }
 *
 *        cmd_mem_list_push_desc_t3(q_array_number, &descriptor, ctx_swap, &sig);
 *
 *        if (descriptor.q_count != 0)
 *        {
 *            return 0;         // We have an error
 *        }
 *     }
 *    return 1;
 * @endcode
 *
 * The following code can be used to create a list on any emem island:
 * @code
 * uint32_t emem_island = 1; // 0 = emem0, 1 = emem1, 2 = emem2
 * uint32_t q_array_number = emem_island << 10 | 1023; // list with q_array_nr 1023 on emem1
 * @endcode
 *
 */
__intrinsic
void cmd_mem_list_init(uint32_t q_array_number, enum MEM_LIST_TYPE type);

/** Initialize a memory list specifying the page and locality of the list
 *
 * @param q_array_number        queue array number to use for list. The range of the array number identifies the external memory to use.
 * - 0 - 1023:    emem0 or island 24,
 * - 1024 - 2047: emem1 or island 25,
 * - 2048 - 3071: emem2 or island 26
 * @param type                  List type
 * @param loc                   Locality specification of the list. Please
 *                              refer to the PRM for detail
 * @param page                  4G page in which the list needs to reside.
 *
 */
__intrinsic
void cmd_mem_list_init_with_loc_and_page(uint32_t q_array_number, enum MEM_LIST_TYPE type, uint32_t loc, uint32_t page);

/** @}
 * @}
 */

#endif /* __NFP_MEM_LIST_H__ */
