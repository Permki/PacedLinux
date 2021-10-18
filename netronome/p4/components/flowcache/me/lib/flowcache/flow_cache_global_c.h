/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache_global_c.h
 * @brief       Definitions, structures and macros used by flow_cache.
 */

#ifndef _FLOW_CACHE_GLOBAL_C_H_
#define _FLOW_CACHE_GLOBAL_C_H_

#include <nfp.h>
#include <stdint.h>
#include <stdlib.h>


#ifndef FC_ENABLE_OVERFLOW
#warning "Flow cache overflow disabled (FC_ENABLE_OVERFLOW)"
#endif

#define CDP_PLATFORM    1
#define SF_PLATFORM     2
#define FWA_PLATFORM    3
#define H_PLATFORM      4
#ifndef PLATFORM_PORT_SELECTION
#error "Platform was not defined!"
#endif

/*
 * Constants and sizes and ring needed by flow cache internally
 * FC_NUMBER_RING_ENTRIES is calculated as __log2 is not available during compile time in microC.
 * Defining FC_NUMBER_RING_ENTRIES the same as FC_OVERFLOW_ENTRIES/FC_LB_EMU and this must be power of 2
 * to initialise the ring.
 */
#if (SIMULATION)

    #define FC_EMU_BASE                 0x8000000
    #define FC_BUCKETS                  (16 * 1024 * FC_LB_EMU)
    #define FC_OVERFLOW_ENTRIES         (1024 * FC_LB_EMU)

#elif (PLATFORM_PORT_SELECTION == CDP_PLATFORM)

    #define ME_MHZ                      1200
    #define FC_EMU_BASE                 0x8000000
    #define FC_BUCKETS                  (2 * 1024 * 1024 * FC_LB_EMU)
    #define FC_OVERFLOW_ENTRIES         (1024 * 1024 * FC_LB_EMU)

#elif (PLATFORM_PORT_SELECTION == FWA_PLATFORM)

    #define ME_MHZ                      1200
    #define FC_EMU_BASE                 0x108000000
    #define FC_BUCKETS                  (8 * 1024 * 1024 * FC_LB_EMU)
    #define FC_OVERFLOW_ENTRIES         (8 * 1024 * 1024 * FC_LB_EMU)

#elif (PLATFORM_PORT_SELECTION == H_PLATFORM)

    #define ME_MHZ                      800
    #define FC_EMU_BASE                 0x8000000
    #define FC_BUCKETS                  (1 * 1024 * 1024 * FC_LB_EMU)
    #define FC_OVERFLOW_ENTRIES         (1 * 1024 * 1024 * FC_LB_EMU)

#else

    #define ME_MHZ                      1200
    #define FC_EMU_BASE                 0x8000000
    #define FC_BUCKETS                  (4 * 1024 * 1024 * FC_LB_EMU)
    #define FC_OVERFLOW_ENTRIES         (4 * 1024 * 1024 * FC_LB_EMU)         // free ring should be equal to overflow entries

#endif

#define FC_ENTRIES                      (FC_BUCKETS + FC_OVERFLOW_ENTRIES)

#define _FC_BUCKET_SIZE                  64
#define _FC_ENTRY_SIZE                  128
#define FC_PAYLOAD_SIZE                 256

#define FC_FREE_ENTRIES                 8                               // number of entries kept locally
#define FC_FREE_CACHE_ENTRIES           (FC_FREE_ENTRIES * FC_LB_EMU)   // in local memory we cache some free entries popped from the ring
#define FC_NUMBER_RING_ENTRIES          (FC_OVERFLOW_ENTRIES/FC_LB_EMU) // free ring should be equal to overflow entries

#define _FC_BUCKETS_ALLOC_SIZE          ((FC_BUCKETS / FC_LB_EMU) * _FC_BUCKET_SIZE)
#define _FC_ENTRIES_ALLOC_SIZE          ((FC_ENTRIES / FC_LB_EMU) * _FC_ENTRY_SIZE)
#define _FC_PAYLOADS_ALLOC_SIZE         ((FC_ENTRIES / FC_LB_EMU) * FC_PAYLOAD_SIZE)


#define _FC_BUCKETS_ADDR                FC_EMU_BASE
#define _FC_ENTRIES_ADDR                (_FC_BUCKETS_ADDR + _FC_BUCKETS_ALLOC_SIZE)
#define _FC_PAYLOADS_ADDR               (_FC_ENTRIES_ADDR + _FC_ENTRIES_ALLOC_SIZE)

/*
 * Verify that number of free entries on ring does not exceed the number of overflow entries
 */
#if FC_NUMBER_RING_ENTRIES > FC_OVERFLOW_ENTRIES
#error "FC_NUMBER_RING_ENTRIES cannot exceed FC_OVERFLOW_ENTRIES"
#endif

/*
 * It is essential for FC operation that rings in different EMU's have the same number,
 * This ring number is used by flow cache as well as flow cache timeout.
 */
#define FC_FREE_RING                    1



/*
 * Default timeout for flow cache entry
 */
#define _FC_DEFAULT_TIMEOUT             30


/*
 * Used to indicate invalid payload address - key mismatch
 */
#define _FC_INVALID_PAYLOAD_ADDR        ~0x00

/**
 *  Format of the BUCKETS
 *  Bit    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
 *  -----\ 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 *  Word  +---------------+---------------+---------------+---------------+
 *     0  | Lock 1        |                     Hash 1 (1:1)              |
 *        +---------------+---------------+---------------+---------------+
 *     1  | Lock 2        |                     Hash 2                    |
 *        +---------------+---------------+---------------+---------------+
 *     2  | Lock 3        |                     Hash 3                    |
 *        +---------------+---------------+---------------+---------------+
 *     3  | Lock 4        |                     Hash 4                    |
 *        +---------------+---------------+---------------+---------------+
 *     4  | Lock (ovf)    |   Timeout 1   |   Timeout 2   |   Timeout 3   |
 *        +---------------+---------------+---------------+---------------+
 *     5  |   Timeout 4   | Timeout (ovf) |          Timestamp 1          |
 *        +---------------+---------------+---------------+---------------+
 *     6  |          Timestamp 2          |          Timestamp 3          |
 *        +---------------+---------------+---------------+---------------+
 *     7  |          Timestamp 4          |        Timestamp (ovf)        |
 *        +---------------+---------------+---------------+---------------+
 *     8  |                 Bytes 1                 |G|    Packets 1    |N|
 *        +---------------+---------------+---------+-+---+-------------+-+
 *     9  |                 Bytes 2                 |G|    Packets 2    |N|
 *        +---------------+---------------+---------+-+---+-------------+-+
 *    10  |                 Bytes 3                 |G|    Packets 3    |N|
 *        +---------------+---------------+---------+-+---+-------------+-+
 *    11  |                 Bytes 4                 |G|    Packets 4    |N|
 *        +---------------+---------------+---------+-+---+-------------+-+
 *    12  |               Bytes (ovf)               |G|  Packets (ovf)  |N|
 *        +---------------+---------------+---------------+---------------+
 *    13  |                     Head 2                    |    Head 3     |
 *        +---------------+---------------+---------------+---------------+
 *    14  |        Head 3 (cont.)         |             Head 4            |
 *        +---------------+---------------+---------------+---------------+
 *    15  | Head 4 (cont.)|                   Head (ovf)                  |
 *        +---------------+---------------+---------------+---------------+
 *
 *  NOTE:   There is no Pointer 0, it is calculated from the address of the CAM
 *          bucket
 *
 * Need exclusive lock to change any fields except timestamp and packet/byte count.
 * Need exclusive lock for (Next), Hash, Head.
 *
 * Breakdown of the first 4 words of the bucket entry:
 *  Bit         3          3          2          2          2          2          2          2      2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
 *  -----\      1          0          9          8          7          6          5          4      3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 *  Word  +----------+----------+----------+----------+----------+----------+----------+-----------+---------------+---------------+---------------+
 *     0  |Write Lock|                            Read Lock                                        |                     Hash                      |
 *        +----------+----------+----------+----------+----------+----------+----------+-----------+---------------+---------------+---------------+
 */

#define FC_INVALID_VALUE                        0xffffffff

#define FC_CAM_HASH_1_wrd                       0
#define FC_CAM_HASH_2_wrd                       1
#define FC_CAM_HASH_3_wrd                       2
#define FC_CAM_HASH_4_wrd                       3

#define FC_OVF_LOCK_wrd                         4

#define FC_TIMEOUT_1_wrd                        4
#define FC_TIMEOUT_2_wrd                        4
#define FC_TIMEOUT_3_wrd                        4
#define FC_TIMEOUT_4_wrd                        5
#define FC_TIMEOUT_OFV_wrd                      5

#define FC_TIMESTAMP_1_wrd                      5
#define FC_TIMESTAMP_2_wrd                      6
#define FC_TIMESTAMP_3_wrd                      6
#define FC_TIMESTAMP_4_wrd                      7
#define FC_TIMESTAMP_OVF_wrd                    7

#define FC_STATS_1_wrd                          8
#define FC_STATS_2_wrd                          9
#define FC_STATS_3_wrd                          10
#define FC_STATS_4_wrd                          11
#define FC_STATS_OVF_wrd                        12

#define _FC_BUCKET_STATS_NEXT_lsb               0

#define FC_HEAD_wrd                             13
#define FC_HEAD_2_wrd                           13
#define FC_HEAD_3_HI_wrd                        13
#define FC_HEAD_3_LO_wrd                        14
#define FC_HEAD_4_HI_wrd                        14
#define FC_HEAD_4_LO_wrd                        15
#define FC_HEAD_OVF_wrd                         15
/*
 * Typedef for CAM entry
 */
typedef enum FC_CAM_ENTRIES {
    CAM_ENTRY_FIRST = 0,
    CAM_ENTRY_SECOND = 1,
    CAM_ENTRY_THIRD = 2,
    CAM_ENTRY_FOURTH = 3,
    CAM_ENTRY_OVERFLOW = 4
};


typedef union  fc_cam_lock_hash_t
{
    struct
    {
        uint32_t lock       :8;
        uint32_t hash       :24;
    };
    uint32_t value;
} fc_cam_lock_hash_t;


/*
 * Typedef for fc_statistics
 */
typedef union  fc_statistics_t
{
    struct
    {
        uint32_t bytes          : 21;
        uint32_t guard          : 1;
        uint32_t packets        : 9;
        uint32_t next           : 1;
    };
    uint32_t value;
} fc_statistics_t;


typedef union fc_cam_bucket_t
{
    struct
    {
        /* First word */
        fc_cam_lock_hash_t       cam_entry1;

        /* Second word */
        fc_cam_lock_hash_t       cam_entry2;

        /* Third word */
        fc_cam_lock_hash_t       cam_entry3;

        /* Fourth word */
        fc_cam_lock_hash_t       cam_entry4;

        /* Fifth word */
        uint32_t lock_overflow          :8;
        uint32_t timeout1               :8;
        uint32_t timeout2               :8;
        uint32_t timeout3               :8;

        /* Sixth word */
        uint32_t timeout4               :8;
        uint32_t timeout_overflow       :8;
        uint32_t timestamp1             :16;

        /* Seventh word */
        uint32_t timestamp2             :16;
        uint32_t timestamp3             :16;

        /* Eighth word */
        uint32_t timestamp4             :16;
        uint32_t timestamp_overflow     :16;

        /* Ninth to thirteenth word */
        fc_statistics_t stats[5];

        /* Fourteenth word */
        uint32_t head2                  :24;
        uint32_t head3_hi               :8;

        /* Fifteenth word */
        uint32_t head3_lo               :16;
        uint32_t head4_hi               :16;

        /* Sixteenth word */
        uint32_t head4_lo               :8;
        uint32_t head_overflow          :24;
    };
    uint32_t value[16];
}fc_cam_bucket_t;



/*
 * Extract head pointer from CAM bucket depending on the index returned on the CAM lookup
 * Return the entry_addr (head) from the xfer registers cam_bucket_xfer
 */
#define FC_EXTRACT_HEAD_PTR_FROM_CAM_BUCKET_XFERS(cam_bucket_xfer, index, entry_addr)   \
    if (index == CAM_ENTRY_SECOND)                                                      \
    {                                                                                   \
        entry_addr = cam_bucket_xfer.head2;                                             \
    }                                                                                   \
    else if (index == CAM_ENTRY_THIRD)                                                  \
    {                                                                                   \
        entry_addr = (cam_bucket_xfer.head3_hi << 16) | cam_bucket_xfer.head3_lo;       \
    }                                                                                   \
    else if (index == CAM_ENTRY_FOURTH)                                                 \
    {                                                                                   \
        entry_addr = (cam_bucket_xfer.head4_hi << 8) | cam_bucket_xfer.head4_lo;        \
    }                                                                                   \
    else if (index == CAM_ENTRY_OVERFLOW)                                               \
    {                                                                                   \
        entry_addr = cam_bucket_xfer.head_overflow;                                     \
    }


/*
 * Key length in entry. The version is part of the key (first 32-bit word).
 */

#ifndef FC_MAX_KEY_LEN_LW
#define FC_MAX_KEY_LEN_LW 23
#warning "FC_MAX_KEY_LEN_LW is not configured, setting it to the maximum of 23"
#endif

#if FC_MAX_KEY_LEN_LW > 23
#error "FC_MAX_KEY_LEN_LW is limited to a maximum of 23"
#endif
#define FC_MAX_BASIC_ENTRY_LEN_LW     (FC_MAX_KEY_LEN_LW + _FC_ENTRY_KEY_wrd)
#define FC_MAX_READ_XFER_COUNT         FC_MAX_BASIC_ENTRY_LEN_LW


/**
 *  Format of the ENTRIES
 *  Bit    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
 *  -----\ 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 *  Word  +---------------+---------------+---------------+---------------+
 *     0  |                             Unused                            |
 *        +---------------+---------------+---------------+---------------+
 *     1  |                            Version                            |
 *        +---------------+---------------+---------------+---------------+
 *     2  |                 Host Context                  |     Hash      |
 *        +---------------+---------------+---------------+---------------+
 *     3  |             Hash              |           Timestamp           |
 *        +---------------+---------------+---------------+---------------+
 *     4  |    Timeout    |                   Next                        |
 *        +---------------+---------------+---------------+---------------+
 *     5  |                             Key 0                             |
 *        +---------------+---------------+---------------+---------------+
 *     6  |                             Key 1                             |
 *        +---------------+---------------+---------------+---------------+
 *     7  |                             Key 2                             |
 *        +---------------+---------------+---------------+---------------+
 *     8  |                             Key 3                             |
 *        +---------------+---------------+---------------+---------------+
 *     9  |                             Key 4                             |
 *        +---------------+---------------+---------------+---------------+
 *    10  |                             Key 5                             |
 *        +---------------+---------------+---------------+---------------+
 *    11  |                             Key 6                             |
 *        +---------------+---------------+---------------+---------------+
 *    12  |                             Key 7                             |
 *        +---------------+---------------+---------------+---------------+
 *    13  |                             Key 8                             |
 *        +---------------+---------------+---------------+---------------+
 *    14  |                             Key 9                             |
 *        +---------------+---------------+---------------+---------------+
 *    15  |                             Key 10                            |
 *        +---------------+---------------+---------------+---------------+
 *    16  |                             Key 11                            |
 *        +---------------+---------------+---------------+---------------+
 *    17  |                             Key 12                            |
 *        +---------------+---------------+---------------+---------------+
 *    18  |                             Key 13                            |
 *        +---------------+---------------+---------------+---------------+
 *    19  |                             Key 14                            |
 *        +---------------+---------------+---------------+---------------+
 *    20  |                             Key 15                            |
 *        +---------------+---------------+---------------+---------------+
 *    21  |                             Key 16                            |
 *        +---------------+---------------+---------------+---------------+
 *    22  |                             Key 17                            |
 *        +---------------+---------------+---------------+---------------+
 *    23  |                             Key 18                            |
 *        +---------------+---------------+---------------+---------------+
 *    24  |                             Key 19                            |
 *        +---------------+---------------+---------------+---------------+
 *    25  |                             Key 20                            |
 *        +---------------+---------------+---------------+---------------+
 *    26  |                             Key 21                            |
 *        +---------------+---------------+---------------+---------------+
 *    27  |                             Key 22                            |
 *        +---------------+---------------+---------------+---------------+
 *    28  |                      Packet Count (high)   -- ATOMIC ACCESS   |
 *        +---------------+---------------+---------------+---------------+
 *    29  |                      Packet Count (low)    -- ATOMIC ACCESS   |
 *        +---------------+---------------+---------------+---------------+
 *    30  |                        Byte Count (high)   -- ATOMIC ACCESS   |
 *        +---------------+---------------+---------------+---------------+
 *    31  |                        Byte Count (low)    -- ATOMIC ACCESS   |
 *        +---------------+---------------+---------------+---------------+
 *
 *
 * Need exclusive lock to change any fields except timeout, timestamp and packet/byte count
 */


    #define _FC_ENTRY_VERSION_wrd               1
    #define _FC_ENTRY_HASH_HIGH_wrd             2
    #define _FC_ENTRY_HASH_LOW_wrd              3
    #define _FC_ENTRY_TIMESTAMP_wrd             3
    #define _FC_ENTRY_TIMEOUT_wrd               4
    #define _FC_ENTRY_NEXT_wrd                  4
    #define _FC_ENTRY_KEY_wrd                   5
    #define _FC_ENTRY_PACKETS_HIGH_wrd          29
    #define _FC_ENTRY_PACKETS_LOW_wrd           28
    #define _FC_ENTRY_BYTES_HIGH_wrd            31
    #define _FC_ENTRY_BYTES_LOW_wrd             30

typedef union fc_entry_t
{
    struct
    {
        /* Word 0 */
        uint32_t unused;

        /* Word 1 */
        uint32_t version;

        /* Word 2 */
        uint32_t host_context   :24;
        uint32_t hash_hi        :8;

       /* Word 3 */
        uint32_t hash_lo        :16;
        uint32_t timestamp      :16;

       /* Word 4 */
        uint32_t timeout        :8;
        uint32_t next           :24;

        /* Word 5 - 27 */
        uint32_t key[23];

        /* Word 28 - 29 */
        uint64_t packet_count;

        /* Word 30 - 31 */
        uint64_t byte_count;
    };
    uint32_t value[32];
} fc_entry_t;


/*
 *  Format of the CACHED LOCK BASE ENTRIES
 *  Bit    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
 *  -----\ 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 *  Word  +---------------+---------------+---------------+---------------+
 *     0  |Usage ctr|R|B|CAMov ctr|CAM 4 ctr|CAM 3 ctr|CAM 2 ctr|CAM 1 ctr|
 *        +---------------+---------------+---------------+---------------+
 */
typedef union fc_cached_lock_entry_t
{
    struct
    {
        uint32_t       usage_counter            : 5;
        uint32_t       reserved                 : 1;
        uint32_t       busy_bit                 : 1;
        uint32_t       cam_entry_ovf_counter    : 5;
        uint32_t       cam_entry_4_counter      : 5;
        uint32_t       cam_entry_3_counter      : 5;
        uint32_t       cam_entry_2_counter      : 5;
        uint32_t       cam_entry_1_counter      : 5;
    };
    uint32_t value;
} fc_cached_lock_entry_t;

/*
* Macro implements:     Number of bits to shift by to get to cam_entry_X_counter
*/
#ifndef FC_CACHED_LOCK_CAM_COUNTER_SHIFT
    #define FC_CACHED_LOCK_CAM_COUNTER_SHIFT(cam_index) \
        ((cam_index) | ((cam_index) << 2))
#endif


/*
* Macro implements:     fc_lock_cache_base[lock_slot_idx].cam_entry_X_counter++;
* This uses about 4 instructions less than using a bit field.
*/
#ifndef FC_CACHED_LOCK_INCREMENT_CAM_ENTRY_COUNTER
    #define FC_CACHED_LOCK_INCREMENT_CAM_ENTRY_COUNTER(lock_idx, cam_index) \
        fc_lock_cache_base[lock_idx].value += (1 << FC_CACHED_LOCK_CAM_COUNTER_SHIFT(cam_index));
#endif

/*
* Macro implements:     fc_lock_cache_base[lock_slot_idx].cam_entry_X_counter--;
* This uses about 4 instructions less than using a bit field.
*/
#ifndef FC_CACHED_LOCK_DECREMENT_CAM_ENTRY_COUNTER
    #define FC_CACHED_LOCK_DECREMENT_CAM_ENTRY_COUNTER(lock_idx, cam_index) \
        fc_lock_cache_base[lock_idx].value -= (1 << FC_CACHED_LOCK_CAM_COUNTER_SHIFT(cam_index));
#endif

/*
* Macro implements:     fc_lock_cache_base[lock_slot_idx].cam_entry_X_counter
* This uses about 4 instructions less than using a bit field.
*/
#ifndef FC_CACHED_LOCK_GET_CAM_COUNTER
    #define FC_CACHED_LOCK_GET_CAM_COUNTER(lock_idx, cam_index) \
        ((fc_lock_cache_base[lock_idx].value >> FC_CACHED_LOCK_CAM_COUNTER_SHIFT(cam_index)) & 0x1f)
#endif




/*
* Macro implements:     fc_lock_cache_base[lock_slot_idx].usage_counter++;
* This uses about 4 instructions less than using a bit field.
*/
#ifndef FC_CACHED_LOCK_INCREMENT_USAGE_COUNTER
    #define FC_CACHED_LOCK_INCREMENT_USAGE_COUNTER(lock_idx) \
        fc_lock_cache_base[lock_idx].value += (1 << 27);
#endif

/*
* Macro implements:     fc_lock_cache_base[lock_slot_idx].usage_counter--;
* This uses about 4 instructions less than using a bit field.
*/
#ifndef FC_CACHED_LOCK_DECREMENT_USAGE_COUNTER
    #define FC_CACHED_LOCK_DECREMENT_USAGE_COUNTER(lock_idx) \
        fc_lock_cache_base[lock_idx].value -= (1 << 27);
#endif






/*
 * Value to use for no timeout.
 * Used in timeout of CAM entries. 0 indicate no timeout.
 */
#ifndef FC_NO_TIMEOUT
#define FC_NO_TIMEOUT                   0
#endif


/*
 * Number of times the timeout thread retries to get a lock.
 * Two (2) is enough as the timestamp will probably change if another
 * thread is holding the lock.
 */
#ifndef FC_TIMEOUT_LOCK_RETRY
#define FC_TIMEOUT_LOCK_RETRY           2
#endif

/*
 * Retry to get lock infinitely
 */
#ifndef FC_LOCK_RETRY_FOREVER
#define FC_LOCK_RETRY_FOREVER           0
#endif

/*
 * Backoff timeout
 */
#ifndef FC_TIMEOUT_LOCK_BACKOFF
#define FC_TIMEOUT_LOCK_BACKOFF         100
#endif

/*
 * Flow cache backoff
 */
#ifndef FC_BACKOFF
#define FC_BACKOFF                      100
#endif



#ifndef __fc_addr40
#define __fc_addr40                     __declspec(mem addr40, shared, import)
#endif

#define FC_EXTRACT_HASH(hash_hi_word, hash_lo_word) (((hash_hi_word) << 16) | (hash_lo_word))



/*
 * Calculaton of entry and payload pointer using entry_idx or for 1:1 case
 *
 * 1:1 entry:
 *      entryPtr = (bucket_address << (log2(FC_ENTRY_SIZE) - log2(FC_BUCKET_SIZE))
 *      payloadPtr = (bucket_address << (log2(FC_PAYLOAD_SIZE) - log2(FC_BUCKET_SIZE))
 *
 * Linked entry
 *      entryPtr = (((FC_BUCKETS / FC_LB_EMU) - 1) + entry_idx) << log2(FC_ENTRY_SIZE)
 *      payloadPtr = (((FC_BUCKETS / FC_LB_EMU) - 1) + entry_idx) << log2(FC_PAYLOAD_SIZE)
 */

/*
 * Set the entry address with base and MU of the first entry (1:1 mapping)
 */
#ifndef FC_SET_FIRST_ENTRY_ADDRESS
    #define FC_SET_FIRST_ENTRY_ADDRESS(base, address) ((uint64_t)base + ((uint64_t)(address) << (__log2(_FC_ENTRY_SIZE) - __log2(_FC_BUCKET_SIZE))))
#endif

/*
 * Set the entry address with base and MU of entry,
 * Also add the offset to skip the first "fixed" entries (1:1 head entries)
 */
#define FC_TIMEOUT_LB_EMU FC_LB_EMU

#ifndef FC_SET_LINKED_ENTRY_ADDRESS
    #define FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, entry_index) \
        ((uint64_t)entry_base_ptr + ((FC_BUCKETS/FC_LB_EMU - 1 + entry_index) << (__log2(_FC_ENTRY_SIZE))))
#endif


/*
 * Setup payload address
 *   use for 1:1 entry (cam_entry1): payload_addr = FC_SET_PAYLOAD_ADDRESS(payload_base_ptr, bucket_addr);
 *   use for other entries: payload_addr = FC_SET_LINKED_PAYLOAD_ADDRESS(payload_base_ptr, entry_idx);
 */

#ifndef FC_SET_PAYLOAD_ADDRESS
    #define FC_SET_PAYLOAD_ADDRESS(base, address) ((uint64_t)base + ((uint64_t)(address) << (__log2(FC_PAYLOAD_SIZE) - __log2(_FC_BUCKET_SIZE))))
#endif



#ifndef FC_SET_LINKED_PAYLOAD_ADDRESS
    #define FC_SET_LINKED_PAYLOAD_ADDRESS(base, entry_index) \
    ((uint64_t)base + ((FC_BUCKETS/FC_LB_EMU - 1 + entry_index) << (__log2(FC_PAYLOAD_SIZE))))
#endif




/*
 * Using current (previous) MU set up next MU to use, either emem0, emem1 or emem2 depending
 * on number of emus in use.
 */
#if (FC_LB_EMU == 1)
    #ifndef FC_NEXT_MEMORY_UNIT
    #define FC_NEXT_MEMORY_UNIT(number_emu, previous_mu, next_mu)           \
        next_mu = previous_mu;
    #endif
#endif

#if (FC_LB_EMU == 2)
    #ifndef FC_NEXT_MEMORY_UNIT
    #define FC_NEXT_MEMORY_UNIT(number_emu, previous_mu, next_mu)           \
        next_mu = ((previous_mu + 1) & 0x1);
    #endif
#endif


#if (FC_LB_EMU == 3)
    #ifndef FC_NEXT_MEMORY_UNIT
    #define FC_NEXT_MEMORY_UNIT(number_emu, previous_mu, next_mu)           \
        next_mu = (9 >> (previous_mu << 1)) & 0x3;
    #endif
#endif


/*
 * Depending on number of external memories on the chip
 */
#if (FC_LB_EMU == 1)

    /*
    * Select the MU pointer by using the bucket.
    */
    #ifndef FC_SELECT_MU_PTR
    #define FC_SELECT_MU_PTR(bucket, ptr1, mu_ptr)                      \
    {                                                                   \
        (mu_ptr) = (ptr1);                                              \
    }
    #endif

    /*
    * Given the previous MU, use the load balancing algorithm in FC_NEXT_MEMORY_UNIT to
    * select the next MU. Then select the correct base_ptr dependent on that MU
    */
    #ifndef FC_SET_MU_ADDRESS_PTR
    #define FC_SET_MU_ADDRESS_PTR(number_emu, previous_mu, base_mu, base_ptr0, base_ptr)        \
    {                                                                                           \
        FC_NEXT_MEMORY_UNIT(number_emu, previous_mu, base_mu);                                  \
        FC_SELECT_MU_PTR(base_mu, base_ptr0, base_ptr);                                         \
    }
    #endif

#endif

#if (FC_LB_EMU == 2)

    /*
    * Select the MU pointer by using the bucket.
    */
    #ifndef FC_SELECT_MU_PTR
    #define FC_SELECT_MU_PTR(bucket, ptr1, ptr2, mu_ptr)                \
    {                                                                   \
        if (bucket == 0) (mu_ptr) = (ptr1);                             \
        else if (bucket == 1)  (mu_ptr) = (ptr2);                       \
    }
    #endif

    /*
    * Given the previous MU, use the load balancing algorithm in FC_NEXT_MEMORY_UNIT to
    * select the next MU. Then select the correct base_ptr dependent on that MU
    */
    #ifndef FC_SET_MU_ADDRESS_PTR
    #define FC_SET_MU_ADDRESS_PTR(number_emu, previous_mu, base_mu, base_ptr0, base_ptr1, base_ptr)     \
    {                                                                                                   \
        FC_NEXT_MEMORY_UNIT(number_emu, previous_mu, base_mu);                                          \
        FC_SELECT_MU_PTR(base_mu, base_ptr0, base_ptr1, base_ptr);                                      \
    }
    #endif

#endif

#if (FC_LB_EMU == 3)

    /*
    * Select the MU pointer by using the bucket.
    */
    #ifndef FC_SELECT_MU_PTR
    #define FC_SELECT_MU_PTR(bucket, ptr1, ptr2, ptr3, mu_ptr)          \
    {                                                                   \
        if (bucket == 0) (mu_ptr) = (ptr1);                             \
        else if (bucket == 1)  (mu_ptr) = (ptr2);                       \
        else if (bucket == 2) (mu_ptr) = (ptr3);                        \
    }
    #endif

    /*
    * Given the previous MU, use the load balancing algorithm in FC_NEXT_MEMORY_UNIT to
    * select the next MU. Then select the correct base_ptr dependent on that MU
    */
    #ifndef FC_SET_MU_ADDRESS_PTR
    #define FC_SET_MU_ADDRESS_PTR(number_emu, previous_mu, base_mu, base_ptr0, base_ptr1, base_ptr2, base_ptr)  \
    {                                                                                                           \
        FC_NEXT_MEMORY_UNIT(number_emu, previous_mu, base_mu);                                                  \
        FC_SELECT_MU_PTR(base_mu, base_ptr0, base_ptr1, base_ptr2, base_ptr);                                   \
    }
    #endif

#endif


/*
 * Set low locality for some atomics
 */
#ifndef FC_SET_ADDRESS_TO_LOW_LOCALITY
    #define FC_SET_ADDRESS_TO_LOW_LOCALITY(address)  ((uint64_t)address | ((uint64_t)0x40 << 32))
#endif



#if (FC_LB_EMU == 1)
#define FC_SET_HI_ADDRESS(number)                                               \
{                                                                               \
        hi_addr = (unsigned int)((unsigned long long)LoadTimeConstant("__ADDR_I24_EMEM") >> 8);   \
}
#endif
#if (FC_LB_EMU == 2)
#define FC_SET_HI_ADDRESS(number)                                               \
{                                                                               \
    switch (number)                                                             \
    {                                                                           \
    case 0:                                                                     \
        hi_addr = (unsigned int)((unsigned long long)LoadTimeConstant("__ADDR_I24_EMEM") >> 8);   \
        break;                                                                  \
    case 1:                                                                     \
        hi_addr = (unsigned int)((unsigned long long)LoadTimeConstant("__ADDR_I25_EMEM") >> 8);   \
        break;                                                                  \
    }                                                                           \
}
#endif
#if (FC_LB_EMU == 3)
#define FC_SET_HI_ADDRESS(number)                                               \
{                                                                               \
    switch (number)                                                             \
    {                                                                           \
    case 0:                                                                     \
        hi_addr = (unsigned int)((unsigned long long)LoadTimeConstant("__ADDR_I24_EMEM") >> 8);   \
        break;                                                                  \
    case 1:                                                                     \
        hi_addr = (unsigned int)((unsigned long long)LoadTimeConstant("__ADDR_I25_EMEM") >> 8);   \
        break;                                                                  \
    case 2:                                                                     \
        hi_addr = (unsigned int)((unsigned long long)LoadTimeConstant("__ADDR_I26_EMEM") >> 8);   \
        break;                                                                  \
    }                                                                           \
}
#endif



/**
 *  Type for lock when adding or releasing fc entry.
 * It contains the bucket address and cam index
 */
typedef union fc_lock_t
{
    struct
    {
        uint32_t bucket_address     : 26;           /**< Bucket address.            */
        uint32_t exclusive_lock     : 1;            /**< Lock.                      */
        uint32_t cam_entry          : 3;            /**< Index in CAM bucket.       */
        uint32_t bucket_mu          : 2;            /**< MU used for bucket.        */
    };
    uint32_t value;                                 /**< Accessor to entire structure. */
} fc_lock_t;


/**
 *  Used for free cache
 */
typedef union fc_free_cache_entry_t
{
    struct
    {
        uint32_t entry_address      : 24;           /**< Entry index.                   */
        uint32_t offset             : 8;            /**< Entry offset in free cache (next to use)  */
    };
    uint32_t value;                                 /**< Accessor to entire structure. */
} fc_free_cache_entry_t;




/** Flow cache Lookup result code
 */
typedef enum FC_LOOKUP_RESULT_CODE
{
    FC_LOOKUP_ERROR                =   0,       /**< Error on fc_lookup. */
    FC_LOOKUP_EXACT_MATCH          =   1,       /**< Exact entry found. */
    FC_LOOKUP_ENTRY_ADDED          =   2,       /**< Entry in CAM added. */
    FC_LOOKUP_ALLOCATE_ERROR       =   6,       /**< Ring with free entries empty or index error. */
    FC_LOOKUP_CAM_OVERFLOW         =   0xff     /**< Entry not added, overflow. */
} FC_LOOKUP_RESULT_CODE;



/** Flow cache remove result code
 */
typedef enum FC_REMOVE_RESULT_CODE
{
    FC_REMOVE_ERROR                     =   0,       /**< Error on fc remove. */
    FC_REMOVE_WAIT_FOR_LOCK             =   1,       /**< Waiting for lock. */
    FC_REMOVE_ENTRY_NOT_FOUND           =   2,       /**< Entry not found. */
    FC_REMOVE_EXACT_MATCH               =   3,       /**< Entry removed. */
    FC_REMOVE_EXACT_MATCH_LOCK_CLEARED  =   4,       /**< Entry removed and hash+lock cleared (was only entry in the list) */
    FC_REMOVE_EXCLUSIVE_LOCK_EXP        =   5        /**< Expected an exclusive lock but it was not set. */
} FC_REMOVE_RESULT_CODE;


/** Flow cache lookup result with return code, lock and payload address
 */
typedef struct fc_lookup_result_t
{
    uint64_t                    payload_address;        /**< Payload address. */
    fc_lock_t                   lock;                   /**< Lock. */
    enum FC_LOOKUP_RESULT_CODE  result_code;            /**< Result of lookup. */
} fc_lookup_result_t;



#endif
