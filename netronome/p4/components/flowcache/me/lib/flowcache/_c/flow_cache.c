/*
 * Copyright (C) 2014-2017,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache.c
 * @brief       Do a lookup using the provided key, if not found then add it
 *              to the CAM.
 */

#ifndef _FLOW_CACHE_C_
#define _FLOW_CACHE_C_

#include <assert.h>
#include <nfp.h>
#include <stdint.h>

#include <nfp6000/nfp_me.h>
#include <nfp/me.h>
#include <nfp/mem_atomic.h>
#include <nfp/mem_bulk.h>
#include <nfp/mem_ring.h>
#include <nfp/mem_cam.h>
#include <mem_cam_add_inc_c.h>
#include <mem_atomic_indirect_c.h>
#include <cam_c.h>

#include "flow_cache_global_c.h"
#include "flow_cache_lock_c.h"
#include "flow_cache_c.h"
#include "flow_cache_timestamp_c.h"

#ifdef JENKINS_HASH
    #include "jenkins_hash_c.h"
#else
#ifdef HASHMAP_CAMPHASH
    #include "camp_hash.h"

    #define value_0 hi
    #define value_1 lo
    #endif
#endif

#ifndef JENKINS_HASH
#ifndef HASHMAP_CAMPHASH
    #error "Either HASHMAP_CAMPHASH or JENKINS_HASH must be defined!"
#endif
#endif
#ifdef JENKINS_HASH
#ifdef HASHMAP_CAMPHASH
    #error "Either HASHMAP_CAMPHASH or JENKINS_HASH must be defined!"
#endif
#endif

/*
 * RINGS emem_queues
 */
__asm .declare_resource emem0_queues global 1024

#if (FC_LB_EMU > 1)
    __asm .declare_resource emem1_queues global 1024

    #if (FC_LB_EMU > 2)
        __asm .declare_resource emem2_queues global 1024
    #endif
#endif


#ifdef FC_JOURNAL

    /*
    * Journalling rings for debug
    */

    #include "flow_cache_journal_c.h"

    #define _FC_PAYLOADS_ALLOC_SIZE ((FC_ENTRIES / FC_LB_EMU) * FC_PAYLOAD_SIZE)


#ifndef FC_JOURNAL_SIZE_LW
    #define FC_JOURNAL_SIZE_LW 4096
#endif

    __asm .alloc_mem fc_journal_ring_data_0 emem0 global FC_JOURNAL_SIZE_LW FC_JOURNAL_SIZE_LW
    __asm .alloc_resource fc_journal_ring_0 emem0_queues global 1
    __asm .init_mu_ring fc_journal_ring_0 fc_journal_ring_data_0 0


#if (FC_LB_EMU > 1)
    __asm .alloc_mem fc_journal_ring_data_1 emem1 global FC_JOURNAL_SIZE_LW FC_JOURNAL_SIZE_LW
    __asm .alloc_resource fc_journal_ring_1 emem1_queues global 1
    __asm .init_mu_ring fc_journal_ring_1 fc_journal_ring_data_1 0
#endif

#if (FC_LB_EMU > 2)
    __asm .alloc_mem fc_journal_ring_data_2 emem2 global FC_JOURNAL_SIZE_LW FC_JOURNAL_SIZE_LW
    __asm .alloc_resource fc_journal_ring_2 emem2_queues global 1
    __asm .init_mu_ring fc_journal_ring_2 fc_journal_ring_data_2 0
#endif

    __gpr uint32_t journal_ring_num;

#endif




/*
 * Setting up ring:
 * First setup _FC_EMU_x_FREE_BASE which contains numbered free entries (from 1..FC_NUMBER_RING_ENTRIES).
 * Then declare a ring on emuX with ring number 1 (FC_FREE_RING) and set the ring up with FC_EMU_x_FREE_BASE
 * and initialise ring with FC_NUMBER_RING_ENTRIES entries.
 */
__asm .alloc_mem _FC_EMU_0_FREE_BASE emem0 global (FC_NUMBER_RING_ENTRIES << 2) (FC_NUMBER_RING_ENTRIES << 2)
__asm .declare_resource FC_EMU_0_RINGS global 1 emem0_queues+FC_FREE_RING
__asm .alloc_resource _FC_FREE_RING_EMU_0 FC_EMU_0_RINGS global 1

// .init_mu_ring        <ring_num>              <ring_sym>              [count in bytes -one word less than ring size]
__asm .init_mu_ring     _FC_FREE_RING_EMU_0     _FC_EMU_0_FREE_BASE     ((FC_NUMBER_RING_ENTRIES - 1) << 2)

#if (FC_LB_EMU > 1)

    __asm .alloc_mem _FC_EMU_1_FREE_BASE emem1 global (FC_NUMBER_RING_ENTRIES << 2) (FC_NUMBER_RING_ENTRIES << 2)
    __asm .declare_resource FC_EMU_1_RINGS global 1 emem1_queues+FC_FREE_RING
    __asm .alloc_resource _FC_FREE_RING_EMU_1 FC_EMU_1_RINGS global 1

    // .init_mu_ring            <ring_num>              <ring_sym>              [count in bytes] --- THSDK-886
    __asm .init_mu_ring         _FC_FREE_RING_EMU_1     _FC_EMU_1_FREE_BASE     ((FC_NUMBER_RING_ENTRIES - 1) << 2)

    #if (FC_LB_EMU > 2)

        __asm .alloc_mem _FC_EMU_2_FREE_BASE emem2 global (FC_NUMBER_RING_ENTRIES << 2) (FC_NUMBER_RING_ENTRIES << 2)
        __asm .declare_resource FC_EMU_2_RINGS global 1 emem2_queues+FC_FREE_RING
        __asm .alloc_resource _FC_FREE_RING_EMU_2 FC_EMU_2_RINGS global 1

            // .init_mu_ring    <ring_num> [          <ring_sym>                [count in bytes]
        __asm .init_mu_ring     _FC_FREE_RING_EMU_2 _FC_EMU_2_FREE_BASE     ((FC_NUMBER_RING_ENTRIES - 1) << 2)

    #endif

#endif


/*
 * Cached locks
 */
__lmem __shared fc_cached_lock_entry_t fc_lock_cache_base[16];
__lmem __shared uint32_t fc_cached_lock_free_slot_list;

/*
 * Free cache contains indexes (to be converted to pointers) of free entries in entry table.
 * This is shared among all threads.
 * While the local cache is being populated from the ring in emem, set the free cache pointer to
 * 0xff000000.
 */
#define FC_BUSY_POPULATING_FREE_CACHE   0xff000000      // set when local cache is in the process of being updated

__lmem __shared uint32_t fc_free_cache_base[FC_LB_EMU][FC_FREE_ENTRIES];





/*
 * Statistics
 */

volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_first_entry_added;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_entry_added;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_exact_match;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_first_entry_exact_match;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_wait_lock1 = 0;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_wait_lock2 = 0;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_wait_lock3 = 0;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_wait_lock4 = 0;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_wait_lock5 = 0;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_wait_lock6 = 0;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_wait_lock7 = 0;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_allocate_error;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_version_updated;

#ifdef FC_JOURNAL
        volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_allocate_error1;
        volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_allocate_error2;
        volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_result_added_out_of_bounds;
#endif

volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_overflow;
volatile __declspec(i28.imem, shared, export) uint32_t fc_stats_ring_empty;




/*
 * result code for exact_lookup_learn
 */
typedef enum INTERNAL_LOOKUP_RESULT_CODE
{
    INTERNAL_LOOKUP_ERROR                =   0,       /**< Error on fc_lookup. */
    INTERNAL_LOOKUP_EXACT_MATCH          =   1,       /**< Exact entry found. */
    INTERNAL_LOOKUP_FIRST_ENTRY_ADDED    =   2,       /**< First entry in CAM added. */
    INTERNAL_LOOKUP_WAIT_FOR_LOCK        =   3,       /**< Waiting for lock. */
    INTERNAL_LOOKUP_ENTRY_ADDED          =   4,       /**< Entry in CAM added. */
    INTERNAL_LOOKUP_KEY_MISMATCH         =   5,       /**< Found entry but key mismatch. */
    INTERNAL_LOOKUP_ALLOCATE_ERROR       =   6,       /**< Ring with free entries empty or index error. */
    INTERNAL_LOOKUP_VERSION_MISMATCH     =   7,       /**< Found entry and key match but version not. */
    INTERNAL_LOOKUP_CAM_OVERFLOW         =   0xff     /**< Entry not added, overflow. */
} INTERNAL_LOOKUP_RESULT_CODE;


/*
 * Get the bucket mu from the hashed values
 */


/*
 * Set up first time which MU to use, either emem0, emem1 or emem2 depending
 * on number of emus in use.
 */
#if (FC_LB_EMU == 1)
    #define _FC_INITIALISE_MU_PTR(hash)   (0);
#endif

#if (FC_LB_EMU == 2)
    #define _FC_INITIALISE_MU_PTR(hash)   ((hash >> 24) & 0x1);
#endif

#if (FC_LB_EMU == 3)
    #define _FC_INITIALISE_MU_PTR(hash)   ((((hash >> 24) * 3) >> 8) & 3);
#endif



/*
 * Setup lock for return
 */
#define _FC_SET_LOCK(lock, cam, mu, xlock, bucket_addr)\
    lock.value = bucket_addr;                          \
    lock.cam_entry = cam;                              \
    lock.bucket_mu = mu;                               \
    lock.exclusive_lock = xlock;

/*
 * Setup lock with the exception of the CAM entry
 */
#define _FC_SET_PARTIAL_LOCK(lock,  mu, bucket_addr)    \
    lock.value = bucket_addr;                           \
    lock.bucket_mu = mu;




#if (_nfp_has_island("i24.mem") && !_nfp_has_island("i25.mem") && !_nfp_has_island("i26.mem"))
#define _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(number)                            \
{                                                                               \
        hi_addr = (unsigned int)((unsigned long long)LoadTimeConstant("__ADDR_I24_EMEM") >> 8);   \
}
#endif
#if (_nfp_has_island("i24.mem") && _nfp_has_island("i25.mem") && !_nfp_has_island("i26.mem"))
#define _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(number)                            \
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
#if  (_nfp_has_island("i24.mem") && _nfp_has_island("i25.mem") && _nfp_has_island("i26.mem"))
#define _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(number)                            \
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

/*
 * if in_version and rule the same; then no update required
 * if in_version > rule, then an update is required, taking wrapping into
 * consideration.
 * It is considered that wrapping occurred if the in_version and rule_version
 * differs more than 256. If it differs that much, the entry should have
 * timed out anyway.
 */
__forceinline static int fc_check_version_update(
    uint32_t rule_version, uint32_t in_version)
{
    __gpr uint32_t diff = in_version - rule_version;

    if (diff == 0)
        return 0;       // fastpath
    else if (diff < 0x100)
        return 1;

    return 0;
}

/*
 * Once off initialisation for free rings.
 */

void fc_global_init()
{

    __mem __addr40 uint32_t *free_base_ptr0 = (__mem __addr40 uint32_t  *)__link_sym("_FC_EMU_0_FREE_BASE");
#if (FC_LB_EMU > 1)
    __mem __addr40 uint32_t *free_base_ptr1 = (__mem __addr40 uint32_t  *)__link_sym("_FC_EMU_1_FREE_BASE");
    #if (FC_LB_EMU > 2)
        __mem __addr40 uint32_t *free_base_ptr2 = (__mem __addr40 uint32_t  *)__link_sym("_FC_EMU_2_FREE_BASE");
    #endif
#endif

    uint32_t                     i, j;
    volatile __xwrite uint32_t   wr_free_entry_idx[32];
    SIGNAL_MASK                  sig_mask = 0;
    SIGNAL                       sig_1, sig_2, sig_3;

    /*
    * Initialise free ring
    */

#if (SIMULATION)
    i = FC_NUMBER_RING_ENTRIES - 32*2;
#else
    i = 1;
#endif


    while (i < FC_NUMBER_RING_ENTRIES)
    {
        for (j = 0; j < 32; j++)
        {
            wr_free_entry_idx[j] = i + j;
        }

        __mem_write32((__xwrite void *)wr_free_entry_idx, free_base_ptr0 + i, 32<<2, 32<<2, sig_done, &sig_1);
        sig_mask = __signals(&sig_1);
#if (FC_LB_EMU > 1)
            __mem_write32((__xwrite void *)wr_free_entry_idx, free_base_ptr1 + i, 32<<2, 32<<2, sig_done, &sig_2);
            sig_mask |= __signals(&sig_2);
    #if (FC_LB_EMU > 2)
            __mem_write32((__xwrite void *)wr_free_entry_idx, free_base_ptr2 + i, 32<<2, 32<<2, sig_done, &sig_3);
            sig_mask |= __signals(&sig_3);
    #endif
#endif

        wait_for_all(sig_mask);
        __implicit_read(&sig_1);
#if (FC_LB_EMU > 1)
        __implicit_read(&sig_2);
    #if (FC_LB_EMU > 2)
        __implicit_read(&sig_3);
    #endif
#endif
        i+=32;
    }

}


void fc_me_init()
{
    uint32_t i, j;

    for (j = 0; j < FC_FREE_ENTRIES; j++)
    {
        fc_free_cache_base[0][j] = 0x00;
    }
#if (FC_LB_EMU > 1)
        for (j = 0; j < FC_FREE_ENTRIES; j++)
        {
            fc_free_cache_base[1][j] = 0x00;
        }
    #if (FC_LB_EMU > 2)
        for (j = 0; j < FC_FREE_ENTRIES; j++)
        {
            fc_free_cache_base[2][j] = 0x00;
        }
    #endif
#endif

    /*
    * Initialise cached locks
    */
    cam_clear();
    fc_cached_lock_free_slot_list = 0xffff;

    /*
        * Initialise hash
        */
#ifndef JENKINS_HASH
#ifdef HASHMAP_CAMPHASH
    camp_hash_init();
#endif
#endif

#ifdef FC_JOURNAL

    journal_ring_num = __alloc_resource("fc_journal_ring_0 emem0_queues global 1");

    #if (FC_LB_EMU > 1)

        journal_ring_num = __alloc_resource("fc_journal_ring_1 emem1_queues global 1");
        journal_ring_num |= (1 << 10);      // change ring num to indicate emem1

        #if (FC_LB_EMU > 2)

            journal_ring_num = __alloc_resource("fc_journal_ring_2 emem2_queues global 1");
            journal_ring_num |= (2 << 10);      // change ring num to indicate emem2

        #endif

    #endif

#endif
}

/*
 * Pop 8 entries from ring for free cache entries
 */
static int fc_populate_free_cache(
    __lmem __shared uint32_t *free_cache_ptr,
    __gpr uint32_t ring_mu,
    __gpr uint32_t ring_number
     )
{
    __gpr uint32_t      i = 0;
    __gpr uint32_t      hi_addr;
    __xread uint32_t    rd_free_entries[FC_FREE_ENTRIES];

    *free_cache_ptr = FC_BUSY_POPULATING_FREE_CACHE;                    // 0xff000000

   _MEM_RING_CONVERT_RING_TO_HI_ADDRESS(ring_mu);

    /*
     *Check the error signal
     */
    if (mem_ring_pop(ring_number, hi_addr, rd_free_entries, FC_FREE_ENTRIES << 2) == -1)
    {
        /*
         * Error has occurred.
         * Clear the bit to indicate we are busy populating and return with 0
         */
        *free_cache_ptr = 0x00;
        mem_incr32((__mem __addr40 void *)&fc_stats_ring_empty);
        return 0;
    }

    /*
     * save the entries into free cache
     */
    for (i = 0; i < FC_FREE_ENTRIES; i++)
    {
        __lmem fc_free_cache_entry_t *entry = (__lmem fc_free_cache_entry_t *)(free_cache_ptr + i);

        entry->entry_address    = rd_free_entries[i];
        entry->offset = FC_FREE_ENTRIES;

#ifdef FC_JOURNAL
        if (rd_free_entries[i] > FC_NUMBER_RING_ENTRIES)
        {
            fc_journal_t        error_data;
            __gpr uint32_t      hi_addr;

            FC_SET_HI_ADDRESS(journal_ring_num >> 10);

            error_data.value = 0x00;
            error_data.error_code = JOURNAL_RING_ENTRY_OUT_OF_BOUND;
            error_data.param_1 = 0x01;
            error_data.param_2 = i;
                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);

            error_data.value = rd_free_entries[i];
                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
        }
#endif
    }

    return 1;
}


/*
* Extract index of next cached entry,
* the index is at an offset in the free_cache_ptr list.
* The cache_offset is saved in entry at offset 8, which is decremented until zero.
* When cache offset is zero, clear free_cache_ptr so that free entries will be populated
* from ring next time.
*/
 static __gpr uint32_t fc_get_entry_from_free_cache(
    __lmem __shared uint32_t *free_cache_ptr
     )
{
    __gpr uint32_t cache_offset;
    __gpr uint32_t cache_addr;
    __gpr uint32_t entry_idx;

    __lmem fc_free_cache_entry_t *lm_entry = (__lmem fc_free_cache_entry_t *)free_cache_ptr;

    lm_entry->offset--;
    cache_offset = lm_entry->offset;

    lm_entry = (__lmem fc_free_cache_entry_t *)((uint32_t)free_cache_ptr) + (cache_offset);
    entry_idx = lm_entry->entry_address;


    if (cache_offset == 0x00)
    {
        /*
         * clear so that next get entry will populate free cache
         */
        *free_cache_ptr = 0x00;
    }

#ifdef FC_JOURNAL
        if (entry_idx > FC_NUMBER_RING_ENTRIES)
        {
            fc_journal_t        error_data;
            __gpr uint32_t      hi_addr;

            FC_SET_HI_ADDRESS(journal_ring_num >> 10);

            error_data.value = 0x00;
            error_data.error_code = JOURNAL_RING_ENTRY_OUT_OF_BOUND;
            error_data.param_1 = 0x02;
            error_data.param_2 = *free_cache_ptr;
                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);

            error_data.value = entry_idx;
                mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
        }
#endif

    return entry_idx;
}


/*
 * Update statistics in CAM bucket and if overflow then update in entry table.
 * Host context stats are also updated.
 * Packet length statistics are incremented by pkt_len and number of packets are incremented by 1.
 * The timeout and timestamp of the bucket entry is also updated.
 */
__forceinline static void fc_update_bucket_ts_and_stats_first_entry(
    __mem __addr40 uint32_t             *bucket_ptr,
    __gpr uint32_t                      timeout
)
{
        SIGNAL                 sig_1, sig_2;
    __xwrite uint32_t       value[2];


    /*
    * CAM entry 0
    */
    value[0] = timeout << 16;
    mem_write_atomic_with_bytemask(
        &value[0],
        (__mem __addr40 uint32_t *)bucket_ptr + FC_TIMEOUT_1_wrd,
        1 << 2,
        1 << 2,
        sig_done,
        &sig_1,
        0x04);  // mask 0x00ff0000


    // bucket_ptr->timestamp1 = fc_get_timestamp(0);
    value[1] = fc_get_timestamp();

    mem_write_atomic_with_bytemask(
        &value[1],
        (__mem __addr40 uint32_t *)bucket_ptr + FC_TIMESTAMP_1_wrd,
        1 << 2,
        1 << 2,
        sig_done,
        &sig_2,
        0x03);  // mask 0x0000ffff

    wait_for_all(&sig_1, &sig_2);
    return;
}

/*
 * Update statistics in CAM bucket and if overflow then update in entry table.
 * Host context stats are also updated.
 * Packet length statistics are incremented by pkt_len and number of packets are incremented by 1.
 * The timeout and timestamp of the bucket entry is also updated.
 */
static void fc_update_bucket_ts_and_stats(
    __mem __addr40 uint32_t             *bucket_ptr,
    __gpr uint32_t                      cam_idx,
    __gpr uint32_t                      timeout
)
{
    SIGNAL              sig_1,
                        sig_2;
    __xwrite uint32_t   value[2];
    __gpr uint32_t      timestamp = fc_get_timestamp();

    switch (cam_idx)
    {
        case CAM_ENTRY_SECOND:
        {
            //  bucket_ptr->timeout2 = timeout;
            value[0] = timeout << 8;
            mem_write_atomic_with_bytemask(
                &value[0],
                (__mem __addr40 uint32_t *)bucket_ptr + FC_TIMEOUT_2_wrd,
                1 << 2,
                1 << 2,
                sig_done,
                &sig_1,
                0x02);  // mask 0x0000ff00


            // bucket_ptr->timestamp2 = fc_get_timestamp(0);
            value[1] = (timestamp << 16);
            mem_write_atomic_with_bytemask(
                &value[1],
                (__mem __addr40 uint32_t *)bucket_ptr + FC_TIMESTAMP_2_wrd,
                1 << 2,
                1 << 2,
                sig_done,
                &sig_2,
                0x0c);  // mask 0xffff0000

            wait_for_all(&sig_1, &sig_2);
        }
        break;

        case CAM_ENTRY_THIRD:
        {
            //  bucket_ptr->timeout3 = timeout
            value[0] = timeout;
            mem_write_atomic_with_bytemask(
                &value[0],
                (__mem __addr40 uint32_t *)bucket_ptr + FC_TIMEOUT_3_wrd,
                1 << 2,
                1 << 2,
                sig_done,
                &sig_1,
                0x01);  // mask 0x000000ff

            // bucket_ptr->timestamp3 = fc_get_timestamp(0)
            value[1] = timestamp;
            mem_write_atomic_with_bytemask(
                &value[1],
                (__mem __addr40 uint32_t *)bucket_ptr + FC_TIMESTAMP_3_wrd,
                1 << 2,
                1 << 2,
                sig_done,
                &sig_2,
                0x03);  // mask 0x0000ffff


            wait_for_all(&sig_1, &sig_2);
        }
        break;

        case CAM_ENTRY_FOURTH:
        {
            //  bucket_ptr->timeout4 = timeout;
            value[0] = (timeout << 24);

            mem_write_atomic_with_bytemask(
                &value[0],
                (__mem __addr40 uint32_t *)bucket_ptr + FC_TIMEOUT_4_wrd,
                1 << 2,
                1 << 2,
                sig_done,
                &sig_1,
                0x08);  // mask 0xff000000

            // bucket_ptr->timestamp4 = fc_get_timestamp(0);
            value[1] = (timestamp << 16);
            mem_write_atomic_with_bytemask(
                &value[1],
                (__mem __addr40 uint32_t *)bucket_ptr + FC_TIMESTAMP_4_wrd,
                1 << 2,
                1 << 2,
                sig_done,
                &sig_2,
                0x0c);  // mask 0xffff0000

            wait_for_all(&sig_1, &sig_2);

        }
        break;

        case CAM_ENTRY_OVERFLOW:
        {
            //  bucket_ptr->timeout_overflow = timeout;
            value[0] = (timeout << 16);

            mem_write_atomic_with_bytemask(
                &value[0],
                (__mem __addr40 uint32_t *)bucket_ptr + FC_TIMEOUT_OFV_wrd,
                1 << 2,
                1 << 2,
                sig_done,
                &sig_1,
                0x04);  // mask 0x00ff0000

            // bucket_ptr->timestamp_overflow = fc_get_timestamp(0);
            value[1] = timestamp;
            mem_write_atomic_with_bytemask(
                &value[1],
                (__mem __addr40 uint32_t *)bucket_ptr + FC_TIMESTAMP_OVF_wrd,
                1 << 2,
                1 << 2,
                sig_done,
                &sig_2,
                0x03);  // mask 0x0000ffff

            wait_for_all(&sig_1, &sig_2);

        }
        break;
    }
}




/*
 * Update statistics in entry table.
 * The correct entry_ptr has to be passed in.
 * Host context stats are also updated.
 */
__forceinline extern void fc_update_entry_ts_and_stats(
    __declspec(mem, addr40, aligned(32)) uint32_t               *entry_ptr,
    __gpr uint32_t                                              timeout
)
{
    __xwrite        uint32_t expiry;

    /*
    * Write 2 bytes timestamp and 1 byte timeout into entry
    */
    expiry = (fc_get_timestamp() << 16) | (timeout << 8);
    mem_write8(&expiry, (__mem __addr40 uint8_t *)entry_ptr + (_FC_ENTRY_TIMESTAMP_wrd << 2) + 2, 3);

    return;
}

/*
 * Update statistics in entry table.
 * The correct entry_ptr has to be passed in.
 * Host context stats are also updated.
 */
__forceinline extern void fc_update_entry_ts_and_stats_version(
    __declspec(mem, addr40, aligned(32)) uint32_t *entry_ptr,
    __gpr uint32_t timeout,
    __gpr uint32_t version
)
{
    __xwrite    uint32_t wr_expiry = (fc_get_timestamp() << 16) | (timeout << 8);
    __xwrite    uint32_t wr_version = version;
    SIGNAL      sig1, sig2;

    /* Write 4B version and then 2B timestamp plus 1B timeout */
    __mem_write8(&wr_version,
                 (__mem __addr40 uint8_t *)entry_ptr + (_FC_ENTRY_VERSION_wrd << 2),
                 4, 4, sig_done, &sig1);
    __mem_write8(&wr_expiry,
                 (__mem __addr40 uint8_t *)entry_ptr + (_FC_ENTRY_TIMESTAMP_wrd << 2) + 2,
                 3, 3, sig_done, &sig2);

    wait_for_all(&sig1, &sig2);
    return;
}

/*
 * Set pointer in bucket table to point to entry in entry table.
 * Word align the xfer to the appropriate position of the pointer (3 byte in size).
 */
void fc_write_entry_ptr_in_bucket(
    __mem __addr40 fc_cam_bucket_t      *cam_bucket_ptr,
    __gpr uint32_t                      cam_index,
    __gpr uint32_t                      entry_idx
)
{
    __xwrite uint32_t   wr_cam_head[2];
    SIGNAL              sig_1, sig_2;


    /*
    * Write the entry_idx aka head_ptr to the CAM bucket table
    */
    switch (cam_index)
    {
        case CAM_ENTRY_SECOND:

            /*
             * cam_bucket_ptr->head2 = entry_idx;
             */
            wr_cam_head[0] = (entry_idx << 8);
            mem_write_atomic_with_bytemask(
                &wr_cam_head[0],
                &cam_bucket_ptr->value[FC_HEAD_2_wrd],
                1 << 2,
                1 << 2,
                ctx_swap,
                &sig_1,
                0x0e);  // mask 0xffffff00
            break;


        case CAM_ENTRY_THIRD:

            /*
             * cam_bucket_ptr->head3_hi = entry_idx >> 16;
             * cam_bucket_ptr->head3_lo = entry_idx & 0xffff;
             */
            wr_cam_head[0] = ((entry_idx >> 16) & 0xff);
            mem_write_atomic_with_bytemask(
                &wr_cam_head[0],
                &cam_bucket_ptr->value[FC_HEAD_3_HI_wrd],
                1 << 2,
                1 << 2,
                sig_done,
                &sig_1,
                0x01);  // mask 0x000000ff

            wr_cam_head[1] = ((entry_idx & 0xffff) << 16);
            mem_write_atomic_with_bytemask(
                &wr_cam_head[1],
                &cam_bucket_ptr->value[FC_HEAD_3_LO_wrd],
                1 << 2,
                1 << 2,
                sig_done,
                &sig_2,
                0x0c);  // mask 0xffff0000

            wait_for_all(&sig_1, &sig_2);
            break;


        case CAM_ENTRY_FOURTH:

            /*
             * cam_bucket_ptr->head4_hi = entry_idx >> 8;
             * cam_bucket_ptr->head4_lo = entry_idx & 0xff;
             */
            wr_cam_head[0] = ((entry_idx >> 8) & 0xffff);
            mem_write_atomic_with_bytemask(
                &wr_cam_head[0],
                &cam_bucket_ptr->value[FC_HEAD_4_HI_wrd],
                1 << 2,
                1 << 2,
                sig_done,
                &sig_1,
                0x03);  // mask 0x0000ffff

            wr_cam_head[1] = ((entry_idx & 0xff) << 24);
            mem_write_atomic_with_bytemask(
                &wr_cam_head[1],
                &cam_bucket_ptr->value[FC_HEAD_4_LO_wrd],
                1 << 2,
                1 << 2,
                sig_done,
                &sig_2,
                0x08);  // 0xff000000

            wait_for_all(&sig_1, &sig_2);
            break;


        case CAM_ENTRY_OVERFLOW:

            wr_cam_head[0] = entry_idx;
            mem_write_atomic_with_bytemask(
                &wr_cam_head[0],
                &cam_bucket_ptr->value[FC_HEAD_OVF_wrd],
                1 << 2,
                1 << 2,
                ctx_swap,
                &sig_2,
                0x07);  // 0x00ffffff
            break;
    }

    return;
}


/*
 * Free cache in local memory is an array, use entry_mu (0, 1, 2) to refer to cache.
 * Check if there are free entries in the cache, if it is being populated, wait.
 * If the cache is empty, populate it.
 * Take an entry from the cache and use it.
 */
__forceinline static __gpr uint32_t fc_allocate_entry(
    __lmem __shared uint32_t    *free_cache_ptr,
    __gpr uint32_t              entry_mu,
    __gpr uint32_t              ring_number
)
{
    /*
     * The cache is empty, populate it.
     * If no entries available then 0 will be returned.
     */
    if (*free_cache_ptr == 0x00)
    {
        if (fc_populate_free_cache(free_cache_ptr, entry_mu, ring_number) == 0)
        {
            return 0;
        }
    }

    /*
     * The cache is in the process of begin populated. Wait.
     */
    while ((*free_cache_ptr & FC_BUSY_POPULATING_FREE_CACHE) == FC_BUSY_POPULATING_FREE_CACHE)
    {
        __asm ctx_arb[voluntary]
    }

    /*
     * Cache has been populated successfully
     */
    if (*free_cache_ptr != 0x00)
    {
        __gpr uint32_t entry_idx = 0;

        entry_idx = fc_get_entry_from_free_cache(free_cache_ptr);
        return entry_idx;

    }
    else
    {
        return 0;
    }
}



/* fc_write_entry:
 *
 * Write an entry after it was added to the CAM bucket:
 * Write the key into the entry table
 */
static void  fc_write_entry(
        __lmem uint32_t *in_key_addr,
        __gpr uint32_t in_key_len,
        __gpr uint32_t in_version,
#ifdef FC_ENABLE_OVERFLOW
        __gpr uint32_t in_hash,
#endif
        __gpr uint32_t in_timeout,
        __gpr uint32_t in_timestamp,
        __mem __addr40 fc_entry_t *entry_ptr
      )
{
    __xwrite uint32_t  entry_xfer[FC_MAX_BASIC_ENTRY_LEN_LW];
    __gpr fc_entry_t   entry;
    __gpr uint32_t     i;
    SIGNAL             sig;

    entry_xfer[0] = 0x00;
    entry_xfer[1] = in_version;


#ifdef FC_ENABLE_OVERFLOW
    entry.hash_hi = (in_hash >> 16);
    entry.hash_lo = (in_hash & 0xffff);

#else

    entry.value[2] = 0x00;

#endif
    entry.timestamp = in_timestamp;
    entry.timeout = in_timeout;
    entry.next = 0x00;

    entry_xfer[2] = entry.value[2];
    entry_xfer[3] = entry.value[3];
    entry_xfer[4] = entry.value[4];

    for (i = _FC_ENTRY_KEY_wrd; i < in_key_len + _FC_ENTRY_KEY_wrd; i++)
    {
        entry_xfer[i] = in_key_addr[i - _FC_ENTRY_KEY_wrd];
    }

    mem_write32(entry_xfer, entry_ptr, (in_key_len + _FC_ENTRY_KEY_wrd) << 2);

    return;
}


/*
 * Setup the payload address for non-first entry.
 * Lookup the entry address where stored in CAM, then read the entry (key) and verify if keys match.
 * Then setup the payload address using the entry address and payload size.
 * payload_base_addr already specified.
 * returns 0, INTERNAL_LOOKUP_VERSION_MISMATCH or INTERNAL_LOOKUP_KEY_MISMATCH
 */
static __gpr uint32_t fc_lookup_payload_addr_with_base_addr(
    __mem __addr40 uint32_t     *entry_ptr,
    __gpr uint32_t              in_key_len,
    __lmem uint32_t             *in_key_addr,
    __gpr uint32_t              in_version
)
{
    SIGNAL              sig;
    __gpr uint64_t      payload_addr;
    __gpr uint32_t      i;
    __xread uint32_t    rd_key[FC_MAX_KEY_LEN_LW + _FC_ENTRY_KEY_wrd];

    /* The entry idx is the same index in the entry table as well as the payload table. */
    __mem_read32(rd_key, entry_ptr, (in_key_len + _FC_ENTRY_KEY_wrd) << 2, (in_key_len+ _FC_ENTRY_KEY_wrd) << 2, sig_done, &sig);
    __wait_for_all(&sig);

    for (i = 0; i < in_key_len; i++)
    {
        if (in_key_addr[i] != rd_key[i  + _FC_ENTRY_KEY_wrd])
        {
            return INTERNAL_LOOKUP_KEY_MISMATCH;
        }
    }

    if (fc_check_version_update(rd_key[_FC_ENTRY_VERSION_wrd], in_version))
        return INTERNAL_LOOKUP_VERSION_MISMATCH;

    return 0;
}




__gpr fc_lookup_result_t fc_exact_lookup_and_learn(
    __lmem uint32_t *in_key_addr,
    __gpr uint32_t in_key_len,
    __gpr uint32_t in_pkt_len,
    __gpr uint32_t in_version,
    __gpr uint32_t in_timeout,
    __mem __addr40 uint32_t *bucket_base_ptr0,
#if (FC_LB_EMU > 1)
    __mem __addr40 uint32_t *bucket_base_ptr1,
#if (FC_LB_EMU > 2)
    __mem __addr40 uint32_t *bucket_base_ptr2,
#endif
#endif

    __mem __addr40 uint32_t *entry_base_ptr0,
#if (FC_LB_EMU > 1)
    __mem __addr40 uint32_t *entry_base_ptr1,
#if (FC_LB_EMU > 2)
    __mem __addr40 uint32_t *entry_base_ptr2,
#endif
#endif

    __mem __addr40 uint32_t *payload_base_ptr0
#if (FC_LB_EMU > 1)
    , __mem __addr40 uint32_t *payload_base_ptr1
#if (FC_LB_EMU > 2)
    , __mem __addr40 uint32_t *payload_base_ptr2
#endif
#endif
)
{
    __gpr uint32_t                                      bucket_mu;
    __gpr uint32_t                                      entry_mu;
    __gpr uint32_t                                      payload_mu;
    __gpr uint32_t                                      bucket_addr;
    __gpr fc_lookup_result_t                            result;
    __gpr INTERNAL_LOOKUP_RESULT_CODE                   result_code = INTERNAL_LOOKUP_ERROR;
    __xrw struct mem_cam_add_inc_24bit                  xrw_cam_lookup;

    __gpr uint32_t                                      cam_lookup_value;
    __declspec(mem, addr40, aligned(32)) uint32_t       *bucket_base_ptr;
    __declspec(mem, addr40, aligned(32)) uint32_t       *entry_base_ptr;
    __mem __addr40 uint32_t                             *payload_base_ptr;

    /*
     * Do the hash where the key is saved at in_key_addr for length in_key_len. Two words
     * of hash is returned.
     * Use hash_value.value_0 to calculate which MU to use (emem0, emem1 or emem2)?
     * Use hash_value.value_1 to map to an address in the hash bucket (CAM).
     * Use hash_value.value_0 as lookup key in the CAM table.
     */
    {
        __gpr hash_result_t        hash_value;

#ifdef JENKINS_HASH
        hash_value = hash_jenkins(in_key_addr, in_key_len);
#else
#ifdef HASHMAP_CAMPHASH
        hash_value = camp_hash(in_key_addr, in_key_len, FC_MAX_KEY_LEN_LW);
#endif
#endif


        /*
        * Calculate bucket_mu using calculated hash. Should be emem0, emem1 or emem2
        */
        bucket_mu = _FC_INITIALISE_MU_PTR(hash_value.value_0);


    /*
     * Set bucket pointer
     */
    FC_SELECT_MU_PTR(
                bucket_mu,
                bucket_base_ptr0,
#if FC_LB_EMU > 1
                bucket_base_ptr1,
#if FC_LB_EMU > 2
                bucket_base_ptr2,
#endif
#endif
                bucket_base_ptr);

        /*
        * Calculate hash_value.value_0:
        * need zeros in upper 8 bits of hash_value.value_0 (reserved for lock)
        * ensure hash_value.value_0 will not clash with special CAM entries that have bit 0x100 set
        */
        hash_value.value_0 = (hash_value.value_0 & 0x00ffffff);

        // ensure hash_value[0] is not zero
        if (hash_value.value_0 == 0x00)
        {
            hash_value.value_0 = 0x01;
        }


        // effectively mask the bits that will be used in lookup by shifting in zeroes and leaving the appropriate number of bits in the MSBs
        // move hash bits back into LSB as the index and multiply it by 64 bytes to get offset
        bucket_addr = hash_value.value_1 << (32 - __log2(FC_BUCKETS/FC_LB_EMU));
        bucket_addr = rotl32(bucket_addr, __log2(FC_BUCKETS/FC_LB_EMU) + 6);

        // preload exclusive lock for new insertions
        cam_lookup_value = hash_value.value_0 | FC_CAM_BUCKET_XLOCK | FC_CAM_BUCKET_SHARED_LOCK;
    }


    /*
     * Do a lookup in CAM, if entry not found, add the entry to the first empty slot.
     */
    {
        __gpr struct mem_cam_add_inc_24bit                      lookup_result;
        SIGNAL_PAIR                                             cam_sig_pair;
        SIGNAL                                                  read_sig;
        __gpr uint32_t                                          overflow_hash;
        __gpr cam_lookup_t                                      lock_slot;
        __gpr uint32_t                                          lock_slot_idx;
        __mem __addr40 uint32_t                                 *entry_ptr;
        __gpr uint32_t                                          flag = 0;
        __gpr uint64_t                                          bucket_ptr = (uint64_t)bucket_base_ptr + bucket_addr;

        lock_slot = cam_lookup(bucket_addr | (1 << 2) | bucket_mu);
        xrw_cam_lookup.search.value =  cam_lookup_value;

        if (!lock_slot.hit)
        {

            /*
             * We have not accessed this CAM bucket at all,
             * allocate cache lock slot
             */

            /*
            * Do CAM lookup add inc, no other thread is accessing slot
            */
            __mem_cam128_lookup24_add_inc(
                &xrw_cam_lookup,
                (__mem __addr40 __align16 void *)(bucket_ptr),
                sig_done,
                &cam_sig_pair
            );

            /*
            * Set the entry address pointer to the correct MU.
            */
            FC_SET_MU_ADDRESS_PTR(
                    FC_LB_EMU, bucket_mu, entry_mu,
                    entry_base_ptr0,
#if FC_LB_EMU > 1
                    entry_base_ptr1,
#if FC_LB_EMU > 2
                    entry_base_ptr2,
#endif
#endif
                    entry_base_ptr);


            {
                /*
                * Pre-read the entry table to check the key- preload for 1:1 case - we hope to be lucky
                */
               __xread uint32_t                rd_key[FC_MAX_KEY_LEN_LW + _FC_ENTRY_KEY_wrd];

                entry_ptr = (__mem __addr40 uint32_t *)FC_SET_FIRST_ENTRY_ADDRESS(entry_base_ptr, bucket_addr);

                __mem_read32(
                        rd_key,
                        entry_ptr,
                        (in_key_len  + _FC_ENTRY_KEY_wrd) << 2,
                        (in_key_len  + _FC_ENTRY_KEY_wrd) << 2,
                        sig_done,
                        &read_sig
                    );

                /*
                * set the payload MU and payload addr
                */
                {
                    FC_SET_MU_ADDRESS_PTR(
                        FC_LB_EMU, entry_mu, payload_mu,
                        payload_base_ptr0,
#if FC_LB_EMU > 1
                        payload_base_ptr1,
#if FC_LB_EMU > 2
                        payload_base_ptr2,
#endif
#endif
                        payload_base_ptr);

                    /*
                    * setup payload_addr if this is first match or first addition (while we wait for signal)
                    */
                    result.payload_address = FC_SET_PAYLOAD_ADDRESS(payload_base_ptr, bucket_addr);
                }

                /*
                * allocate cached lock and write to it
                * Set busy bit.
                */
                lock_slot_idx = ffs(fc_cached_lock_free_slot_list);
                fc_cached_lock_free_slot_list &= ~(0x01 << lock_slot_idx);

                cam_write(lock_slot_idx, bucket_addr | (1 << 2) | bucket_mu, 0);
                fc_lock_cache_base[lock_slot_idx].busy_bit = 1;
                FC_CACHED_LOCK_INCREMENT_USAGE_COUNTER(lock_slot_idx);

                /*
                 * Setup lock with bucket mu, address
                 */
                _FC_SET_PARTIAL_LOCK(result.lock, bucket_mu, bucket_addr);

                /*
                * Wait for:
                * CAM return
                */
                wait_for_all( &cam_sig_pair, &read_sig);

                /*
                * setup lock with CAM entry found/added
                */
                result.lock.cam_entry = xrw_cam_lookup.result.match;
                lookup_result.result = xrw_cam_lookup.result; // copy to GPR

#ifdef FC_JOURNAL
                if (xrw_cam_lookup.search.value != 0xff && result.lock.cam_entry > CAM_ENTRY_FOURTH)
                {
                    /*
                    * This is very bad, we have released an exclusive lock and shared lock but there was not one set !!!!
                    */
                    fc_journal_t error_data;
                    __gpr uint32_t          hi_addr;
                    FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                    error_data.value = 0x00;
                    error_data.error_code = JOURNAL_CAM_ENTRY_OUT_OF_BOUNDS;
                    error_data.param_1 = 0x01;
                    error_data.param_3 = result.lock.cam_entry;
                        mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);

                    error_data.value = xrw_cam_lookup.search.value;
                        mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                }
#endif
                /*
                 * Check results
                 */
                if ((xrw_cam_lookup.search.value & 0x80ff) == 0)
                {
                    /* Above test for:
                     * (xrw_cam_lookup.result.added == 0 && xrw_cam_lookup.result.match == 0
                     * && !FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK_IN_LOOKUP(xrw_cam_lookup.result.data)
                     * BUT above is only one branch
                     */

                    __gpr uint32_t i;

                    /*
                    * Our lucky guess that the lookup entry is at 0 was correct.
                    * (d) - entry found - not locked and first entry (prefetched)
                    * We have set the global shared lock, no exlusive lock, increment cache shared lock.
                    */
                    result_code = INTERNAL_LOOKUP_EXACT_MATCH;

                    FC_CACHED_LOCK_INCREMENT_CAM_ENTRY_COUNTER(lock_slot_idx, 0);
                    fc_lock_cache_base[lock_slot_idx].busy_bit = 0;

                    /*
                    * Compare the entry from the entry table with the key stored in LM
                    * It has to be the same otherwise it is a key mismatch
                    */

                    for (i = 0; i < in_key_len; i++)
                    {
                        if (in_key_addr[i] != rd_key[i + _FC_ENTRY_KEY_wrd])
                        {
                            result_code = INTERNAL_LOOKUP_KEY_MISMATCH;
                            break;
                        }
                    }

                    if (result_code != INTERNAL_LOOKUP_KEY_MISMATCH)
                    {
                        /*
                        * Compare version
                        */
                        if (fc_check_version_update(
                            rd_key[_FC_ENTRY_VERSION_wrd], in_version))
                            result_code = INTERNAL_LOOKUP_VERSION_MISMATCH;
                        else
                        {
                            __critical_path();

                            /*
                            * Update stats of first entry
                            */
                            fc_update_bucket_ts_and_stats_first_entry(
                                (__mem __addr40 __declspec(aligned(32)) uint32_t *)bucket_ptr,
                                in_timeout
                            );

                            mem_incr32((__mem __addr40 void *)&fc_stats_first_entry_exact_match);
                            result.result_code = FC_LOOKUP_EXACT_MATCH;
                            return result;
                        }
                    }
                }
                else if ((xrw_cam_lookup.search.value & 0x8080) == 0x8000)
                {
                    /*
                    * (c) another thread holds the exclusive lock: we did not add an entry hence exclusive lock not ours
                    * Subtract shared lock.
                    * Clear busy bit.
                    * We will have to retry.
                    */
                    __mem __addr40 uint32_t     *cam_entry_ptr = (__mem __addr40 uint32_t *)((uint64_t)bucket_ptr + (xrw_cam_lookup.result.match << 2));
                    __xwrite uint32_t           lock_xfer  = FC_CAM_BUCKET_SHARED_LOCK;

                    mem_sub32(
                        (__xwrite void *)&lock_xfer,
                        cam_entry_ptr,
                        1 << 2
                    );
                    fc_lock_cache_base[lock_slot_idx].busy_bit = 0;
                    flag = 1;
                    mem_incr32((__mem __addr40 void *)&fc_stats_wait_lock1);

                } // if exclusive lock

#ifndef FC_ENABLE_OVERFLOW

                else if (xrw_cam_lookup.search.value == 0xff)
                {
                    /*
                    * Overflow on lookup (CAM full)
                    * (f) - overflow  -> overflow_on_lookup#
                    */

                    /*
                     * Clear busy bit
                     */
                    fc_lock_cache_base[lock_slot_idx].busy_bit = 0;

                    /*
                    * Decrement usage counter which would have been set
                    */
                    FC_CACHED_LOCK_DECREMENT_USAGE_COUNTER(lock_slot_idx);

                    if (fc_lock_cache_base[lock_slot_idx].usage_counter == 0)
                    {
                        /*
                        * Last user, put it back onto free fc_cached_lock_free_slot_list
                        */
                        cam_write(lock_slot_idx, 0, 0);
                        fc_cached_lock_free_slot_list |= (1 << lock_slot_idx);
                    }

                    mem_incr32((__mem __addr40 void *)&fc_stats_overflow);

                    _FC_SET_PARTIAL_LOCK(result.lock, 0x00, 0x00);        // clear partial lock

                    result.result_code = FC_LOOKUP_CAM_OVERFLOW;
                    return result;
                }
#endif
            }
        }
        else
        {
            /*
            * We have a CAM hit, check for shared locks and so forth
            * Increment the usage counter
            */
            lock_slot_idx = lock_slot.entry_num;
            FC_CACHED_LOCK_INCREMENT_USAGE_COUNTER(lock_slot_idx);
            flag = 1;
        }

        do
        {
            xrw_cam_lookup.search.value =  cam_lookup_value;

            while (flag)
            {
                /*
                * Spin here until exclusive lock has been cleared.
                * Or busy lock has been cleared.
                */

                SIGNAL_PAIR lookup_sig_pair;

                /*
                * Do mem_cam_add_24bit, don't increment shared lock when entry found
                */
                __mem_cam128_lookup24_add(
                    (__xrw struct mem_cam_24bit *)&xrw_cam_lookup,
                    (__mem __addr40 __align16 void *)(bucket_ptr),
                    sig_done,
                    &lookup_sig_pair
                );

                /*
                 * Setup lock with bucket mu, address
                 */
                _FC_SET_PARTIAL_LOCK(result.lock, bucket_mu, bucket_addr);


                FC_SET_MU_ADDRESS_PTR(
                    FC_LB_EMU, bucket_mu, entry_mu,
                    entry_base_ptr0,
#if FC_LB_EMU > 1
                    entry_base_ptr1,
#if FC_LB_EMU > 2
                    entry_base_ptr2,
#endif
#endif
                    entry_base_ptr);

                entry_ptr = (__mem __addr40 uint32_t *)FC_SET_FIRST_ENTRY_ADDRESS(entry_base_ptr, bucket_addr);

                /*
                * set the payload MU and payload addr
                */
                FC_SET_MU_ADDRESS_PTR(
                    FC_LB_EMU, entry_mu, payload_mu,
                    payload_base_ptr0,
#if FC_LB_EMU > 1
                    payload_base_ptr1,
#if FC_LB_EMU > 2
                    payload_base_ptr2,
#endif
#endif
                    payload_base_ptr);


                /*
                * setup payload_addr if this is first match or first addition (while we wait for signal)
                */
                result.payload_address = FC_SET_PAYLOAD_ADDRESS(payload_base_ptr, bucket_addr);

                /*
                 * wait for CAM lookup
                 */
                wait_for_all(&lookup_sig_pair);



                lookup_result.result = xrw_cam_lookup.result; // copy to GPR

                /*
                * setup lock with CAM entry found
                */
                result.lock.cam_entry = xrw_cam_lookup.result.match;


#ifdef FC_JOURNAL

                if (xrw_cam_lookup.search.value != 0xff && result.lock.cam_entry > CAM_ENTRY_FOURTH)
                {
                    fc_journal_t error_data;
                    __gpr uint32_t          hi_addr;
                    FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                    error_data.value = 0x00;
                    error_data.error_code = JOURNAL_CAM_ENTRY_OUT_OF_BOUNDS;
                    error_data.param_1 = 0x02;
                    error_data.param_3 = result.lock.cam_entry;
                        mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);

                    error_data.value = xrw_cam_lookup.search.value;
                        mem_ring_journal_fast((journal_ring_num & 0x3ff), hi_addr, error_data.value);
                }
#endif

                if (xrw_cam_lookup.search.value == 0xff)
                {
                    /*
                    * Overflow on lookup (CAM full)
                    * (f) - overflow  -> overflow_on_lookup#
                    */
#ifdef FC_ENABLE_OVERFLOW

                    /*
                     * Entry not found and CAM full ...
                     * Check overflow list or add overflow if first
                     * Set busy bit if not already set otherwise retry lookup
                     */
                    if (fc_lock_cache_base[lock_slot_idx].busy_bit == 0)
                    {
                        fc_lock_cache_base[lock_slot_idx].busy_bit = 1;
                        break;
                    }
                    else
                    {
                        sleep(FC_BACKOFF);
                        continue;
                    }
#else
                    /*
                    * Decrement usage counter which would have been incremented
                    */
                    FC_CACHED_LOCK_DECREMENT_USAGE_COUNTER(lock_slot_idx);

                    if (fc_lock_cache_base[lock_slot_idx].usage_counter == 0)
                    {
                        /*
                        * Last user, put it back onto free fc_cached_lock_free_slot_list
                        */
                        cam_write(lock_slot_idx, 0, 0);
                        fc_cached_lock_free_slot_list |= (1 << lock_slot_idx);
                    }

                    mem_incr32((__mem __addr40 void *)&fc_stats_overflow);
                    _FC_SET_PARTIAL_LOCK(result.lock, 0x00, 0x00);        // clear partial lock
                    result.result_code = FC_LOOKUP_CAM_OVERFLOW;
                    return result;
#endif

                }
                else if (xrw_cam_lookup.result.added)
                {
                    /*
                    * We have added an entry (we have an exclusive and shared lock)
                    * Set the busy bit
                    */
                    while (fc_lock_cache_base[lock_slot_idx].busy_bit)
                    {
                        sleep(FC_BACKOFF);
                    }
                    fc_lock_cache_base[lock_slot_idx].busy_bit = 1;
                    break;
                }
                else if (!FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK_IN_LOOKUP(xrw_cam_lookup.result.data)
                    && (fc_lock_cache_base[lock_slot_idx].busy_bit == 0))
                {
                    /*
                    * Entry found
                    * Exclusive lock not set and busy bit clear
                    */
                    __mem __addr40 fc_cam_bucket_t *cam_bucket_ptr = (__mem __addr40 fc_cam_bucket_t *)bucket_ptr;
                    __mem __addr40 uint32_t *cam_entry_ptr = &cam_bucket_ptr->value[xrw_cam_lookup.result.match];

                    fc_lock_cache_base[lock_slot_idx].busy_bit = 1;

                    if (FC_CACHED_LOCK_GET_CAM_COUNTER(lock_slot_idx, xrw_cam_lookup.result.match) == 0)
                    {
                        /*
                        * No cached shared locks yet, acquire the global shared lock.
                        */
                        __xrw uint32_t  lock_xfer  = FC_CAM_BUCKET_SHARED_LOCK;

                        mem_test_add(
                            &lock_xfer,
                            cam_entry_ptr,
                            1 << 2
                        );

                        if (FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK(lock_xfer)
                            || (((lock_xfer << 8) >> 8) != ((cam_lookup_value << 8) >> 8)))
                        {
                            /*
                            * Another thread has exclusive lock,
                            * OR the hash value has changed.
                            * Subtract our global shared lock and clear busy bit.
                            * We will have to do another lookup.
                            */
                            mem_sub32(
                                (__xwrite void *)&lock_xfer,
                                cam_entry_ptr,
                                1 << 2
                            );

                            fc_lock_cache_base[lock_slot_idx].busy_bit = 0;

                            /*
                            * Lookup again,
                            */
                            sleep(FC_BACKOFF);
                            continue;
                        }

                        /*
                        * We have acquired the global shared lock.
                        */

                    }
                    else
                    {
                        /*
                        * Existing cached shared locks, verify global lock
                        */
                        __xread fc_cam_lock_hash_t      rd_lock;

                        mem_read_atomic(&rd_lock.value, cam_entry_ptr, 1 << 2);

                        if (FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK(rd_lock.value)
                            || (rd_lock.hash != ((cam_lookup_value << 8) >> 8))
                        )
                        {
                            /*
                            * There is an exclusive lock on the CAM entry, try again.
                            * OR
                            * Hash value different, retry
                            */
                            fc_lock_cache_base[lock_slot_idx].busy_bit = 0;

                            /*
                            * Do lookup again
                            */
                            sleep(FC_BACKOFF);
                            continue;
                        }
                    }


                    if (xrw_cam_lookup.result.match == 0)
                    {
                        /*
                        * lookup matched first entry - the case of adding first entry was handled earlier
                        */
                        __xread uint32_t                rd_key[FC_MAX_KEY_LEN_LW  + _FC_ENTRY_KEY_wrd];
                        __gpr uint32_t                  i;

                        __mem_read32(
                            rd_key,
                            entry_ptr,
                            (in_key_len  + _FC_ENTRY_KEY_wrd) << 2,
                            (in_key_len  + _FC_ENTRY_KEY_wrd) << 2,
                            sig_done,
                            &read_sig
                        );

                        wait_for_all(&read_sig);


                        result_code = INTERNAL_LOOKUP_EXACT_MATCH;
                        entry_ptr = (__declspec(mem, addr40, aligned(32)) uint32_t *)FC_SET_FIRST_ENTRY_ADDRESS(entry_base_ptr, bucket_addr);

                        FC_CACHED_LOCK_INCREMENT_CAM_ENTRY_COUNTER(lock_slot_idx, 0);
                        fc_lock_cache_base[lock_slot_idx].busy_bit = 0;


                        /*
                        * Compare the entry from the entry table with the key stored in LM
                        * It has to be the same otherwise it is a key mismatch
                        */
                        for (i = 0; i < in_key_len; i++)
                        {
                            if (in_key_addr[i] != rd_key[i + _FC_ENTRY_KEY_wrd])
                            {
                                result_code = INTERNAL_LOOKUP_KEY_MISMATCH;
                                break;
                            }
                        }

                        if (result_code != INTERNAL_LOOKUP_KEY_MISMATCH)
                        {
                            /*
                            * Compare version
                            */
                            if (fc_check_version_update(
                                    rd_key[_FC_ENTRY_VERSION_wrd], in_version))
                                result_code = INTERNAL_LOOKUP_VERSION_MISMATCH;
                            else
                            {
                                __critical_path();

                                /*
                                * Update stats of first entry
                                */
                                fc_update_bucket_ts_and_stats_first_entry(
                                    (__mem __addr40 __declspec(aligned(32)) uint32_t *)bucket_ptr,
                                    in_timeout
                                );

                                mem_incr32((__mem __addr40 void *)&fc_stats_first_entry_exact_match);
                                result.result_code = FC_LOOKUP_EXACT_MATCH;
                                return result;
                            }
                        }
                    }
                    break;
                }
                else
                {
                    /*
                    * Lookup again - busy bit was set or exclusive lock on CAM entry
                    */
                    sleep(FC_BACKOFF);
                    continue;
                }

            } // while

            flag = 0;

            /*
            * Process result from CAM lookup
            */

            /*
            * Lookup in CAM results:
            * a - entry added - first entry (1-1 mapping)                 -> fc_write_entry
            * b - entry added - not first entry                           -> allocate_entry
            * c - entry found - locked                                    -> release_shared_retry_lookup (handled in while (flag) loop
            * d - entry found - not locked and first entry (prefetched)   -> correct_entry_preloaded - this has been handled just after lookup
            * e - entry found - not locked and not first entry            -> normal flow
            * f - overflow                                                -> overflow_on_lookup
            */


            if ((lookup_result.search.value & 0xff) == 0x80)
            {
                /*
                * FAST PATH first entry added,
                * (a) entry added - first entry (1-1 mapping)
                */
                __mem __addr40 fc_entry_t *entry_ptr = (__mem __addr40 fc_entry_t *)FC_SET_FIRST_ENTRY_ADDRESS(entry_base_ptr, bucket_addr);
                fc_lock_cache_base[lock_slot_idx].busy_bit = 0;

                /*
                * Entry 0 added, write entry
                * a) - entry added - first entry   -> fc_write_entry
                */
                fc_write_entry(
                        in_key_addr,
                        in_key_len,
                        in_version,
#ifdef FC_ENABLE_OVERFLOW
                        0,      // hash
#endif
                        0,      // timeout
                        0,      // timestamp
                        entry_ptr
                );

                fc_update_bucket_ts_and_stats_first_entry(
                        (__mem __addr40 __declspec(aligned(32)) uint32_t *)bucket_ptr,
                        in_timeout
                    );
                result.lock.exclusive_lock = 1;
                result.result_code = FC_LOOKUP_ENTRY_ADDED;
                mem_incr32((__mem __addr40 void *)&fc_stats_first_entry_added);
                return result;
            }

#ifdef FC_ENABLE_OVERFLOW
            else if (lookup_result.search.value  == 0xff)
            {
                /*
                 * f - overflow      -> overflow_on_lookup
                 */
                SIGNAL                              sig;
                __xread uint32_t                    rd_head_ovf;
                __mem __addr40 fc_cam_bucket_t      *cam_bucket_ptr = (__mem __addr40 fc_cam_bucket_t *)bucket_ptr;
                __xrw uint32_t                      lock_xfer  = FC_CAM_BUCKET_SHARED_LOCK;
                __mem __addr40 uint32_t             *cam_entry_ptr = &cam_bucket_ptr->value[CAM_ENTRY_OVERFLOW];
                __gpr uint32_t                      entry_idx;

                /*
                * Overflow on lookup (CAM full)
                * (f) - overflow                                                -> overflow_on_lookup#
                */
                /*
                * Get shared lock and then exclusive lock
                */
                _FC_SET_LOCK(result.lock, CAM_ENTRY_OVERFLOW, bucket_mu, 0x00, bucket_addr);

                if (FC_CACHED_LOCK_GET_CAM_COUNTER(lock_slot_idx, CAM_ENTRY_OVERFLOW) == 0)
                {
                    /*
                    * No cached shared lock as yet, try to get global lock - shared code at bottom
                    */

                    /*
                     * First check if exclusive lock set; and then acquire shared lock.
                     * See THSDK-3362
                     */
                    mem_read_atomic(
                        &lock_xfer,
                        cam_entry_ptr,
                        1 << 2
                    );

                    if (!FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK(lock_xfer))
                    {
                        lock_xfer  = FC_CAM_BUCKET_SHARED_LOCK;
                        mem_test_add(
                            &lock_xfer,
                            cam_entry_ptr,
                            1 << 2
                        );

                        /*
                        * Another thread has exclusive lock,
                        * Subtract our global shared lock
                        */
                        if (FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK(lock_xfer))
                            mem_sub32(
                                (__xwrite void *)&lock_xfer,
                                cam_entry_ptr,
                                1 << 2
                            );
                    }

                    if (FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK(lock_xfer))
                    {
                        /*
                        * Another thread has exclusive lock,
                        * Clear busy bit.
                        */
                        mem_incr32((__mem __addr40 void *)&fc_stats_wait_lock2);

                        // back off, wait for exclusive lock to disappear
                        sleep(FC_TIMEOUT_LOCK_BACKOFF);
                        fc_lock_cache_base[lock_slot_idx].busy_bit = 0;
                        flag = 1;
                        result_code = INTERNAL_LOOKUP_WAIT_FOR_LOCK;
                        continue;
                    }

                    /*
                    * No exclusive lock, we have the global shared lock
                    */
                }
                else
                {
                    /*
                    * Existing cached shared locks, verify global lock
                    */
                    mem_read_atomic(&lock_xfer, cam_entry_ptr, 1 << 2);

                    if (FC_CAM_BUCKET_IS_EXCLUSIVE_LOCK(lock_xfer))
                    {
                        /*
                        * There is an exclusive lock on the CAM entry, try again.
                        */
                        fc_lock_cache_base[lock_slot_idx].busy_bit = 0;
                        result_code = INTERNAL_LOOKUP_WAIT_FOR_LOCK;
                        flag = 1;
                        mem_incr32((__mem __addr40 void *)&fc_stats_wait_lock3);
                        continue;
                    }
                }

                /*
                * We now have a shared lock
                */
                FC_CACHED_LOCK_INCREMENT_CAM_ENTRY_COUNTER(lock_slot_idx, CAM_ENTRY_OVERFLOW);
                fc_lock_cache_base[lock_slot_idx].busy_bit = 0;
                mem_read_atomic(&rd_head_ovf, (__mem __addr40 uint32_t *)&cam_bucket_ptr->value[FC_HEAD_OVF_wrd], 1 << 2);

                lookup_result.result.match = CAM_ENTRY_OVERFLOW;
                overflow_hash = ((cam_lookup_value << 8) >> 8);
                entry_idx = ((rd_head_ovf << 8) >> 8);

                if (entry_idx == 0x00)
                {
                    /*
                    * If overflow head empty, force into entry added code
                    */
                    result.lock = fc_lock_exclusive_with_bucket_addr(result.lock, bucket_base_ptr);

                    if (!result.lock.exclusive_lock)
                    {
                        /*
                        * Failure
                        * Shared lock would have been deleted too, do lookup again.
                        * Another entry with the same hash as the one we have
                        * now might have been added.
                        */
                        result_code = INTERNAL_LOOKUP_WAIT_FOR_LOCK;
                        flag = 1;
                        mem_incr32((__mem __addr40 void *)&fc_stats_wait_lock4);
                        continue;
                    }

                    lookup_result.result.added = 1;
                    cam_lookup_value = (FC_CAM_BUCKET_XLOCK | FC_CAM_BUCKET_SHARED_LOCK); // to subtract if free entry not found
                }
                else
                {

                    SIGNAL  sig;

                    /*
                    * Overflow head is not empty, search list for match
                    */

                    lookup_result.result.added = 0;

                    /*
                    * Set entry_ptr
                    */
                    entry_ptr = (__mem __addr40 uint32_t *)FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, entry_idx);

                    result_code = fc_lookup_payload_addr_with_base_addr(
                                    entry_ptr,
                                    in_key_len,
                                    in_key_addr,
                                    in_version
                                );

                    /*
                    * Don't count stats if key mismatch
                    */
                    if (result_code == 0)
                    {
                        fc_update_bucket_ts_and_stats(
                            (__mem __addr40 uint32_t *)cam_bucket_ptr,
                            lookup_result.result.match,
                            in_timeout
                        );
#ifdef FC_JOURNAL
                    if (!(
                        ((uint32_t)result.payload_address >= (uint32_t)payload_base_ptr0
                        && (uint32_t)result.payload_address <= ((uint32_t)payload_base_ptr0 + _FC_PAYLOADS_ALLOC_SIZE ))
                        ))
                        {

                            fc_journal_t error_data;
                            __gpr uint32_t          hi_addr;

                            FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                            error_data.value = ((uint64_t)result.payload_address >> 32);
                            error_data.error_code = JOURNAL_PAYLOAD_OUT_OF_BOUNDS;
                            error_data.param_1 = 0x01;
                            mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);

                            error_data.value = (uint32_t)result.payload_address;
                            mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);

                            error_data.value = entry_idx;
                            mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);

                        }
#endif
                        mem_incr32((__mem __addr40 void *)&fc_stats_exact_match);
                        result.payload_address = FC_SET_LINKED_PAYLOAD_ADDRESS(
                                                payload_base_ptr, entry_idx);
                        result.result_code = FC_LOOKUP_EXACT_MATCH;
                        return result;
                    }

                    if (result_code == INTERNAL_LOOKUP_VERSION_MISMATCH)
                    {
                        result.payload_address = FC_SET_LINKED_PAYLOAD_ADDRESS(
                                                payload_base_ptr, entry_idx);
                    }

                }
            }
#endif

            if ((lookup_result.search.value & 0x80) == 0x80)
            {
                /*
                * (b) entry added - Not first entry, allocate entry (OVERFLOW with empty head also added here)
                */

                __gpr uint32_t                      entry_idx = 0;
                __mem __addr40 fc_cam_bucket_t      *cam_bucket_ptr = (__mem __addr40 fc_cam_bucket_t *)(bucket_ptr);

                if (lookup_result.result.match < CAM_ENTRY_OVERFLOW)
                    fc_lock_cache_base[lock_slot_idx].busy_bit = 0;


                entry_idx = fc_allocate_entry(
                    fc_free_cache_base[entry_mu],
                    entry_mu,
                    FC_FREE_RING
                );

                if (entry_idx == 0)
                {
                    SIGNAL                  sig;
                    __xwrite uint32_t       cam_lookup_key = cam_lookup_value;

                    result.result_code = FC_LOOKUP_ALLOCATE_ERROR;
                    result.payload_address = 0x00;

#ifdef FC_JOURNAL

#ifdef FC_ENABLE_OVERFLOW
                    if (lookup_result.result.match > CAM_ENTRY_OVERFLOW)
#else
                    if (lookup_result.result.match > CAM_ENTRY_FOURTH)
#endif
                    {

                        fc_journal_t            error_data;
                        __gpr uint32_t          hi_addr;

                        FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                        error_data.value = 0x00;
                        error_data.error_code = JOURNAL_CAM_ENTRY_OUT_OF_BOUNDS;
                        error_data.param_1 = 0x03;
                        error_data.param_3 = lookup_result.result.match;
                            mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);

                        error_data.value = cam_lookup_value;
                            mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);

                    }
#endif

                    while (fc_lock_cache_base[lock_slot_idx].busy_bit)
                        sleep(FC_BACKOFF);

                    /*
                        * Decrement usage counter which would have been set
                        */
                    FC_CACHED_LOCK_DECREMENT_USAGE_COUNTER(lock_slot_idx);

                    if (fc_lock_cache_base[lock_slot_idx].usage_counter == 0)
                    {
                        /*
                        * Last user, put it back onto free fc_cached_lock_free_slot_list
                        */
                        cam_write(lock_slot_idx, 0, 0);
                        fc_cached_lock_free_slot_list |= (1 << lock_slot_idx);
                    }
                    /*
                    * Subtract what we have added to the CAM bucket
                    */
                    mem_sub32((__xwrite void*)&cam_lookup_key,
                        &cam_bucket_ptr->value[lookup_result.result.match],
                        1 << 2
                    );

                    mem_incr32((__mem __addr40 void *)&fc_stats_allocate_error);
#ifdef FC_JOURNAL
                    mem_incr32((__mem __addr40 void *)&fc_stats_allocate_error1);
#endif

                    return result;
                }

                /*
                * write allocated pointer in bucket
                */
                fc_write_entry_ptr_in_bucket(cam_bucket_ptr, lookup_result.result.match, entry_idx);


                {
                    __mem __addr40 fc_entry_t       *entry_ptr = (__mem __addr40 fc_entry_t *)FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, entry_idx);


                    fc_write_entry(
                            in_key_addr,
                            in_key_len,
                            in_version,
#ifdef FC_ENABLE_OVERFLOW
                            overflow_hash,
#endif
                            0,      // timeout
                            0,      // timestamp
                            entry_ptr
                        );
                    fc_update_bucket_ts_and_stats(
                            (__mem __addr40 __declspec(aligned(32)) uint32_t *)bucket_ptr,
                            lookup_result.result.match,
                            in_timeout
                        );
                    /*
                    * setup payload_addr
                    */

                    result.payload_address = FC_SET_LINKED_PAYLOAD_ADDRESS(payload_base_ptr, entry_idx);
                    result.lock.exclusive_lock = 1;

#ifdef FC_JOURNAL
                if (!(
                    ((uint32_t)result.payload_address >= (uint32_t)payload_base_ptr0
                    && (uint32_t)result.payload_address <= ((uint32_t)payload_base_ptr0 + _FC_PAYLOADS_ALLOC_SIZE ))
                    ))
                    {

                        fc_journal_t            error_data;
                        __gpr uint32_t          hi_addr;

                        FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                        error_data.value = ((uint64_t)result.payload_address >> 32);
                        error_data.error_code = JOURNAL_PAYLOAD_OUT_OF_BOUNDS;
                        error_data.param_1 = 0x04;
                            mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);

                        error_data.value = (uint32_t)result.payload_address;
                            mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);

                        error_data.value = entry_idx;
                            mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);

                    }
#endif
                }

                mem_incr32((__mem __addr40 void *)&fc_stats_entry_added);
                result.result_code = FC_LOOKUP_ENTRY_ADDED;
                return result;
            } // else



            /*
             * Fall through here when lookup_result.result.added == 0
             */

            /*
            * We have a key mismatch, so either find another match or add
            * this entry in the entry table.
            * Use the "next" field of the entry to find the next entry. If this is 0,
            * then we are at the end of the list and a new entry can be added.
            */


            if (result_code != INTERNAL_LOOKUP_KEY_MISMATCH
                && result_code != INTERNAL_LOOKUP_VERSION_MISMATCH)
            {
                /*
                * (e) We have found the entry but it is not the first
                */

                __mem __addr40 fc_cam_bucket_t  *cam_bucket_ptr = (__mem __addr40 fc_cam_bucket_t *)bucket_ptr;
                __gpr uint32_t                  entry_idx;
                __xread  fc_cam_bucket_t        rd_cam_head;
                SIGNAL                          sig;

                /*
                * Increment the appropriate CAM entry counter with 1
                * i.e. fc_lock_cache_base[lock_slot_idx].cam_entry_2_counter++;
                */
                FC_CACHED_LOCK_INCREMENT_CAM_ENTRY_COUNTER(lock_slot_idx, lookup_result.result.match);
                fc_lock_cache_base[lock_slot_idx].busy_bit = 0;

                /*
                * Read the head pointer (entry idx) from the CAM bucket.
                */
                __mem_read_atomic(
                    &rd_cam_head.value[FC_HEAD_wrd],
                    (__mem __addr40 uint32_t *)&cam_bucket_ptr->value[FC_HEAD_wrd],
                    3 << 2,
                    3 <<2,
                    sig_done,
                    &sig
                    );
                wait_for_all(&sig);

                FC_EXTRACT_HEAD_PTR_FROM_CAM_BUCKET_XFERS(rd_cam_head, lookup_result.result.match, entry_idx);

                /*
                * Set entry_ptr
                */
                entry_ptr = (__mem __addr40 uint32_t *)FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, entry_idx);

                result_code = fc_lookup_payload_addr_with_base_addr(
                                entry_ptr,
                                in_key_len,
                                in_key_addr,
                                in_version
                            );
                if (result_code == 0)
                {
                    fc_update_bucket_ts_and_stats(
                        (__mem __addr40 uint32_t *)bucket_ptr,
                        lookup_result.result.match,
                        in_timeout
                    );
#ifdef FC_JOURNAL
                if (!(
                    ((uint32_t)result.payload_address >= (uint32_t)payload_base_ptr0
                    && (uint32_t)result.payload_address <= ((uint32_t)payload_base_ptr0 + _FC_PAYLOADS_ALLOC_SIZE ))
                    ))
                {

                    fc_journal_t error_data;
                    __gpr uint32_t          hi_addr;

                    FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                    error_data.value = ((uint64_t)result.payload_address >> 32);
                    error_data.error_code = JOURNAL_PAYLOAD_OUT_OF_BOUNDS;
                    error_data.param_1 = 0x02;
                        mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);

                    error_data.value = (uint32_t)result.payload_address;
                        mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);

                    error_data.value = entry_idx;
                        mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);

                }
#endif
                    mem_incr32((__mem __addr40 void *)&fc_stats_exact_match);
                    result.payload_address = FC_SET_LINKED_PAYLOAD_ADDRESS(
                                                payload_base_ptr, entry_idx);
                    result.result_code = FC_LOOKUP_EXACT_MATCH;
                    return result;
                } // if !key/version mismatch

                result.payload_address = FC_SET_LINKED_PAYLOAD_ADDRESS(
                                                payload_base_ptr, entry_idx);

            } // if match but not first


            if (result_code == INTERNAL_LOOKUP_VERSION_MISMATCH)
            {
                /* Lock the bucket entry */
                if (!result.lock.exclusive_lock)
                    result.lock = fc_lock_exclusive_with_bucket_addr(
                                                result.lock, bucket_base_ptr);

                if (!result.lock.exclusive_lock)
                {
                    /*
                    * We could not get exclusive lock, had to delete our
                    * shared lock.
                    * CAM entry might be deleted by exclusive lock held by
                    * another thread.
                    * Give up and do lookup again.
                    */
                    result_code = INTERNAL_LOOKUP_WAIT_FOR_LOCK;
                    flag = 1;
                    mem_incr32((__mem __addr40 void *)&fc_stats_wait_lock5);
                }
                else
                {
                    SIGNAL             sig;
                    __xwrite uint32_t  write_xfer;
                    __xread uint32_t   read_xfer;
                    __gpr fc_entry_t   entry;
                    __mem __addr40 uint32_t *mem_ptr = (__mem __addr40 uint32_t *)entry_ptr + 1;

                    /* While waiting for exclusive lock, the version might have
                     * changed. Check version again. */
                    __mem_read32(&read_xfer, mem_ptr, 1 << 2, 1 << 2, sig_done,
                                 &sig);
                    __wait_for_all(&sig);

                    if (fc_check_version_update(read_xfer, in_version))
                    {
                        __critical_path();
                        write_xfer = in_version;
                        // write it
                        __mem_write32(&write_xfer, mem_ptr, 1 << 2, 1 << 2,
                                      sig_done, &sig);

                        /* Update bucket stats of first entry */
                        fc_update_bucket_ts_and_stats(
                            (__mem __addr40 uint32_t *)bucket_ptr,
                            lookup_result.result.match,
                            in_timeout
                        );
                        __wait_for_all(&sig);

                        mem_incr32((__mem __addr40 void *)&fc_stats_version_updated);
                        result.result_code = FC_LOOKUP_ENTRY_ADDED;
                    }
                    else
                    {
                        /* Return with exclusive lock intact, the app will
                         * release it */
                        mem_incr32((__mem __addr40 void *)&fc_stats_exact_match);
                        result.result_code = FC_LOOKUP_EXACT_MATCH;
                    }
                    return result;
                }
            }

            while (result_code == INTERNAL_LOOKUP_KEY_MISMATCH)
            {
                __gpr uint32_t                  next_idx;
                __mem __addr40 fc_entry_t       *previous_entry_ptr = (__mem __addr40 fc_entry_t *)entry_ptr;
                __mem __addr40 fc_cam_bucket_t  *cam_bucket_ptr = (__mem __addr40 fc_cam_bucket_t *)(bucket_ptr);

                /*
                * Find the next linked entry
                */
                next_idx = previous_entry_ptr->next;

                if (next_idx == 0)
                {
                    /*
                    * If next is null, then allocate an entry and link into list
                    */

                    /*
                    * Lock the bucket entry
                    */
                    if (!result.lock.exclusive_lock)
                    {
                        result.lock = fc_lock_exclusive_with_bucket_addr(result.lock, bucket_base_ptr);
                    }

                    if (!result.lock.exclusive_lock)
                    {
                        /*
                        * We could not get exclusive lock, had to delete our shared lock.
                        * CAM entry might be deleted by exclusive lock held by another thread has been released.
                        * Give up.
                        * Do lookup again.
                        */
                        result_code = INTERNAL_LOOKUP_WAIT_FOR_LOCK;
                        flag = 1;
                        mem_incr32((__mem __addr40 void *)&fc_stats_wait_lock6);
                        break;
                    }

                    /*
                    * Allocate entry from free ring
                    */
                    next_idx = fc_allocate_entry(
                        fc_free_cache_base[entry_mu],
                        entry_mu,
                        FC_FREE_RING
                    );

                    if (next_idx == 0)
                    {
                        /*
                        * Could not allocate
                        */
                        fc_lock_release_with_bucket_addr(result.lock, (__mem __addr40 uint32_t *)bucket_base_ptr);

                        /*
                        * Free list empty
                        */
                        mem_incr32((__mem __addr40 void *)&fc_stats_allocate_error);
#ifdef FC_JOURNAL
                        mem_incr32((__mem __addr40 void *)&fc_stats_allocate_error2);
#endif
                        result.result_code = FC_LOOKUP_ALLOCATE_ERROR;
                        return result;
                    }


                    /*
                    * Set Next bit in bucket stats to indicate that the overflow list has elements beyond the
                    * head (linked list). Use the index of the lookup result as the index to the stats.
                    */
                    mem_bitset_imm(1, (__mem __addr40 void *)&cam_bucket_ptr->stats[lookup_result.result.match]);

                    /*
                    * Set pointer in the Next field of the entry
                    */
                    previous_entry_ptr->next = next_idx;

                    /*
                    * write entry with mismatched key into linked list of entry table
                    */
                    {
                        __mem __addr40 fc_entry_t      *entry_ptr = (__mem __addr40 fc_entry_t *)FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_idx);

                        /*
                        * Write entry (key and version etc) to entry table and update entry statistics
                        */
                        fc_write_entry(
                                in_key_addr,
                                in_key_len,
                                in_version,
#ifdef FC_ENABLE_OVERFLOW
                                overflow_hash,          // hash
#endif
                                in_timeout,
                                fc_get_timestamp(),
                                entry_ptr
                            );

                        /*
                        * setup payload_addr
                        */
                        result.payload_address = FC_SET_LINKED_PAYLOAD_ADDRESS(payload_base_ptr, next_idx);
                    }

#ifdef FC_JOURNAL
                    if (!(
                        ((uint32_t)result.payload_address >= (uint32_t)payload_base_ptr0
                        && (uint32_t)result.payload_address <= ((uint32_t)payload_base_ptr0 + _FC_PAYLOADS_ALLOC_SIZE ))
                        ))
                    {
                        fc_journal_t    error_data;
                        __gpr uint32_t  hi_addr;

                        FC_SET_HI_ADDRESS(journal_ring_num >> 10);

                        error_data.value = ((uint64_t)result.payload_address >> 32);
                        error_data.error_code = JOURNAL_PAYLOAD_OUT_OF_BOUNDS;
                        error_data.param_1 = 0x03;
                            mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);

                        error_data.value = (uint32_t)result.payload_address;
                            mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);

                        error_data.value = (uint32_t)entry_ptr;
                            mem_ring_journal_fast(journal_ring_num & 0x3ff, hi_addr, error_data.value);
                    }
#endif

                    mem_incr32((__mem __addr40 void *)&fc_stats_entry_added);
                    result.result_code = FC_LOOKUP_ENTRY_ADDED;
                    return result;
                }
                else
                {
                    /*
                    * Not end of list, find next linked entry
                    */
                    entry_ptr = (__mem __addr40 uint32_t *)FC_SET_LINKED_ENTRY_ADDRESS(entry_base_ptr, next_idx);

                    /*
                    * Read the key of  (entry idx) from the CAM bucket.
                    */
                    result_code = fc_lookup_payload_addr_with_base_addr(
                                    entry_ptr,
                                    in_key_len,
                                    in_key_addr,
                                    in_version
                                );

                    if (result_code == 0)
                    {
                        /* We have a match, update entry statistics */
                        fc_update_entry_ts_and_stats(entry_ptr, in_timeout);
                        mem_incr32((__mem __addr40 void *)&fc_stats_exact_match);
                        result.payload_address = FC_SET_LINKED_PAYLOAD_ADDRESS(
                                                payload_base_ptr, next_idx);
                        result.result_code = FC_LOOKUP_EXACT_MATCH;
                        return result;
                    }

                    if (result_code == INTERNAL_LOOKUP_VERSION_MISMATCH)
                    {
                        result.payload_address = FC_SET_LINKED_PAYLOAD_ADDRESS(
                                                payload_base_ptr, next_idx);
                        /* Lock the bucket entry */
                        if (!result.lock.exclusive_lock)
                            result.lock = fc_lock_exclusive_with_bucket_addr(
                                                        result.lock,
                                                        bucket_base_ptr);

                        if (!result.lock.exclusive_lock)
                        {
                            /*
                            * We could not get exclusive lock, had to delete our
                            * shared lock.
                            * CAM entry might be deleted by exclusive lock held by
                            * another thread.
                            * Give up and do lookup again.
                            */
                            result_code = INTERNAL_LOOKUP_WAIT_FOR_LOCK;
                            flag = 1;
                            mem_incr32((__mem __addr40 void *)&fc_stats_wait_lock7);
                        }
                        else
                        {
                            SIGNAL sig;
                            __xread uint32_t   read_xfer;
                            __gpr fc_entry_t   entry;
                            __mem __addr40 uint32_t *mem_ptr = (__mem __addr40 uint32_t *)entry_ptr + 1;

                            /* While waiting for exclusive lock, the version
                             * might havechanged. Check version again.*/
                            __mem_read32(&read_xfer, mem_ptr, 1 << 2, 1 << 2,
                                         sig_done, &sig);
                            __wait_for_all(&sig);

                            /* If version greater than one in entry,
                             * overwrite it with new version */
                            if (fc_check_version_update(read_xfer, in_version))
                            {
                                /* We have a match, update entry statistics */
                                __critical_path();
                                fc_update_entry_ts_and_stats_version(entry_ptr,
                                                                     in_timeout,
                                                                     in_version);
                                mem_incr32((__mem __addr40 void *)&fc_stats_version_updated);
                                result.result_code = FC_LOOKUP_ENTRY_ADDED;
                            }
                            else
                            {
                                /* Return with exclusive lock intact, the app will
                                * release it */
                                /* We have a match, update entry statistics */
                                fc_update_entry_ts_and_stats(entry_ptr,
                                                             in_timeout);
                                mem_incr32((__mem __addr40 void *)&fc_stats_exact_match);
                                result.result_code = FC_LOOKUP_EXACT_MATCH;
                            }
                            return result;
                        }
                    }

                    /*
                    * Continue searching.
                    */
                } // else
            } // while

        } while (flag);
    }

    result.result_code = FC_LOOKUP_ERROR;
    return result;
}



#endif
