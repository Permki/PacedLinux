/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* NFP MEM LOAD_BALANCE intrinsic function definitions */

#ifndef __NFP_MEM_LOAD_BALANCE_ENGINE_H__
#define __NFP_MEM_LOAD_BALANCE_ENGINE_H__

#include <nfp.h>
#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file nfp_mem_load_balance_engine.h
 * @addtogroup mu_lb MU LB (Load Balance) Engine Intrinsics
 * @{
 */

/** Location reference for HashAddr or StatAddr */
typedef enum MEM_LB_LOCATION
{
    LB_LOCATION_LOCAL_MEM               = 0x00,     /**< Local memory.   */
    LB_LOCATION_DCACHE                  = 0x01      /**< DCache.   */
}  MEM_LB_LOCATION;

#ifdef __BIGENDIAN

/** Load balance descriptor detail */
typedef union mem_lb_desc_t
{
    struct
    {
        /* first 32 bit word */
        uint32_t hash_base_addr_low     : 13;   /**< Upper 13 bits of 18 bit hash base address. */
        uint32_t stat_addr_ref          : 1;    /**< Location reference for StatAddr (0-local mem,1-DCache). */
        uint32_t stat_base_addr         : 18;   /**< Statistics base address. */

        /* second 32 bit word */
        uint32_t hash_seed_low          : 13;   /**< Lower 13 bits of 24 bit hash seed value. */
        uint32_t boundaries             : 3;    /**< Number of boundaries in the hash bucket. */
        uint32_t index_bits             : 5;    /**< Number of bits of hash to use as address offset. */
        uint32_t compare_bits           : 5;    /**< Number of bits of hash to use for compare. */
        uint32_t hash_addr_ref          : 1;    /**< Location reference for HashAddr (0-local mem,1-DCache). */
        uint32_t hash_base_addr_high    : 5;    /**< Upper 5 bits of 18 bit hash base address. */

        /* third 32 bit word */
        uint32_t reserved_1             : 21;   /**< Reserved. */
        uint32_t hash_seed_high         : 11;   /**< Upper 11 bits of 24 bit hash seed value. */

        /* fourth 32 bit word */
        uint32_t reserved_2             : 32;   /**< Reserved. */

        /* fifth 32 bit word */
        uint32_t user_data_1            : 32;   /**< User data first 32-bit word. */

        /* sixth 32 bit word */
        uint32_t user_data_2            : 32;   /**< User data second 32-bit word. */
    };
    uint32_t value[6];                          /**< Accessor to entire descriptor structure. */
} mem_lb_desc_t;

#else /* __LITTLEENDIAN */

typedef union mem_lb_desc_t
{
    struct
    {
        /* first 32 bit word */
        uint32_t stat_base_addr         : 18;
        uint32_t stat_addr_ref          : 1;
        uint32_t hash_base_addr_low     : 13;

        /* second 32 bit word */
        uint32_t hash_base_addr_high    : 5;
        uint32_t hash_addr_ref          : 1;
        uint32_t compare_bits           : 5;
        uint32_t index_bits             : 5;
        uint32_t boundaries             : 3;
        uint32_t hash_seed_low          : 13;

        /* third 32 bit word */
        uint32_t hash_seed_high         : 11;
        uint32_t reserved_1             : 21;

        /* fourth 32 bit word */
        uint32_t reserved_2             : 32;

        /* fifth 32 bit word */
        uint32_t user_data_1            : 32;

        /* sixth 32 bit word */
        uint32_t user_data_2            : 32;
    };
    uint32_t value[6];
} mem_lb_desc_t;

#endif

/**  descriptor in write_reg
 */
typedef __xwrite mem_lb_desc_t    mem_lb_desc_in_write_reg_t;

/**  descriptor in write_reg
 */
typedef __xread mem_lb_desc_t    mem_lb_desc_in_read_reg_t;


#ifdef __BIGENDIAN

/** Lower 32 bit of read statistic command address of local stat array. */
typedef union mem_lb_local_stats_address_format_t
{
    struct
    {
        uint32_t reserved_2             : 19;    /**< Reserved. */
        uint32_t statistic_address      : 10;   /**< Statistic address (64b address).  */
        uint32_t reserved_1             : 3;    /**< Reserved. */
    };
    uint32_t value;                             /**< Accessor to entire descriptor structure. */
} mem_lb_local_stats_address_format_t;

#else /* __LITTLEENDIAN */

typedef union mem_lb_local_stats_address_format_t
{
    struct
    {
        uint32_t reserved_1             : 19;
        uint32_t statistic_address      : 10;
        uint32_t reserved_2             : 3;
    };
    uint32_t value;
} mem_lb_local_stats_address_format_t;

#endif


#ifdef __BIGENDIAN

/** Lower 32 bit of read statistic command address of dcache stat array. */
typedef union mem_lb_dcache_stats_address_format_t
{
    struct
    {
        uint32_t reserved_2             : 10;    /**< Reserved. */
        uint32_t statistic_address      : 19;   /**< Statistic address (64b address).  */
        uint32_t reserved_1             : 3;    /**< Reserved. */
    };
    uint32_t value;                             /**< Accessor to entire descriptor structure. */
} mem_lb_dcache_stats_address_format_t;

#else /* __LITTLEENDIAN */

typedef union mem_lb_dcache_stats_address_format_t
{
    struct
    {
        uint32_t reserved_1             : 10;
        uint32_t statistic_address      : 19;
        uint32_t reserved_2             : 3;
    };
    uint32_t value;
} mem_lb_dcache_stats_address_format_t;

#endif



#ifdef __BIGENDIAN

/** Lower 32 bit of local descriptor address detail.  */
typedef union mem_lb_descriptor_address_format_t
{
    struct
    {
        uint32_t reserved               : 26;  /**< Reserved. */
        uint32_t array_entry            : 6;   /**< Descriptor array entry location.  */
    };
    uint32_t value;                           /**< Accessor to entire descriptor structure. */
} mem_lb_descriptor_address_format_t;

#else /* __LITTLEENDIAN */

typedef union mem_lb_descriptor_address_format_t
{
    struct
    {
        uint32_t array_entry            : 6;
        uint32_t reserved               : 26;
    };
    uint32_t value;
} mem_lb_descriptor_address_format_t;

#endif


#ifdef __BIGENDIAN

/** Lower 32 bit of Local ID table address detail. This is used when writing to the local id table. */
typedef union mem_lb_id_table_address_format_t
{
    struct
    {
        uint32_t reserved_2             : 21;   /**< Reserved. */
        uint32_t id_table_address       : 8;    /**< Local ID table array address.  */
        uint32_t reserved_1             : 3;    /**< Reserved. */
    };
    uint32_t value;                           /**< Accessor to entire id table address structure. */
} mem_lb_id_table_address_format_t;

#else /* __LITTLEENDIAN */

typedef union mem_lb_id_table_address_format_t
{
    struct
    {
        uint32_t reserved_1             : 3;
        uint32_t id_table_address       : 8;
        uint32_t reserved_2             : 21;
    };
    uint32_t value;
} mem_lb_id_table_address_format_t;

#endif



#ifdef __BIGENDIAN

/** Lower 32 bit of Local Hash Bucket address detail. */
typedef union mem_lb_bucket_local_address_format_t
{
    struct
    {
        uint32_t reserved_2             : 19;   /**< Reserved. */
        uint32_t bucket_address         : 9;    /**< Local hash bucket address in hash table array.  */
        uint32_t reserved_1             : 4;    /**< Reserved. */
    };
    uint32_t value;                            /**< Accessor to entire bucket address structure. */
} mem_lb_bucket_local_address_format_t;

#else /* __LITTLEENDIAN */

typedef union mem_lb_bucket_local_address_format_t
{
    struct
    {
        uint32_t reserved_1             : 4;
        uint32_t bucket_address         : 9;
        uint32_t reserved_2             : 19;
    };
    uint32_t value;
} mem_lb_bucket_local_address_format_t;

#endif


#ifdef __BIGENDIAN

/** Lower 32 bit of Dcache Hash Bucket address detail. */
typedef union mem_lb_bucket_dcache_address_format_t
{
    struct
    {
        uint32_t reserved_2             : 10;   /**< Reserved. */
        uint32_t bucket_address         : 18;   /**< Dcache hash bucket address in hash table array.  */
        uint32_t reserved_1             : 4;    /**< Reserved. */
    };
    uint32_t value;                            /**< Accessor to entire bucket address structure. */
} mem_lb_bucket_dcache_address_format_t;

#else /* __LITTLEENDIAN */

typedef union mem_lb_bucket_dcache_address_format_t
{
    struct
    {
        uint32_t reserved_1             : 4;
        uint32_t bucket_address         : 18;
        uint32_t reserved_2             : 10;
    };
    uint32_t value;
} mem_lb_bucket_dcache_address_format_t;

#endif






/*
 * Lookup command and results unions and types
 *
 */


#ifdef __BIGENDIAN

/** Lookup data detail. */
typedef union mem_lb_lookup_command_format_t
{
    struct
    {
        /*
        * First lookup data 32 bit word
        */
        uint32_t reserved_1             : 8;    /**< Reserved. */
        uint32_t hash_input             : 24;   /**< Initial hash input.  */

        /*
        * Second lookup data 32 bit word
        */
        uint32_t reserved_2             : 16;   /**< Reserved. */
        uint32_t stat_add_value         : 16;   /**< Value to add to statistic.  */

        uint32_t padding[2];                    /**< Padding required for xfer to be used together with result lookup. */
    };
    uint32_t value[4];                          /**< Accessor to entire lookup detail structure. */
} mem_lb_lookup_command_format_t;

#else /* __LITTLEENDIAN */

typedef union mem_lb_lookup_command_format_t
{
    struct
    {
        uint32_t hash_input             : 24;
        uint32_t reserved_1             : 8;

        uint32_t stat_add_value         : 16;
        uint32_t reserved_2             : 16;
    };
    uint32_t value[2];
} mem_lb_lookup_command_format_t;

#endif

/**  lookup command in write_reg
 */
typedef __xwrite mem_lb_lookup_command_format_t    mem_lb_lookup_command_format_in_write_reg_t;


#ifdef __BIGENDIAN

/** Lookup data bundle_id result detail. The bundle id and user data are returned. */
typedef union mem_lb_lookup_bundle_id_result_t
{
    struct
    {
        /*
        * First 64 bit word
        */
        uint32_t reserved_1             : 24;   /**< Reserved. */
        uint32_t bundle_id              : 8;    /**< Bundle id. */
        uint32_t reserved_2             : 32;   /**< Reserved. */

        /*
         * Second 64 bit word
         */
        uint32_t user_data_1            : 32;   /**< User_data_1. */
        uint32_t user_data_2            : 32;   /**< User data_2. */

     };
    uint32_t value[4];                       /**< Accessor to entire lookup detail structure. */
} mem_lb_lookup_bundle_id_result_t;

#else /* __LITTLEENDIAN */

typedef union mem_lb_lookup_bundle_id_result_t
{
    struct
    {
        uint32_t reserved_2             : 32;
        uint32_t bundle_id              : 8;
        uint32_t reserved_1             : 24;

        uint32_t user_data_2            : 32;
        uint32_t user_data_1            : 32;
    };
    uint32_t value[4];
} mem_lb_lookup_bundle_id_result_t;

#endif

/**  lookup result in read_reg
 */
typedef __xread mem_lb_lookup_bundle_id_result_t    mem_lb_lookup_bundle_id_result_in_read_reg_t;





#ifdef __BIGENDIAN

/** Lookup direct result detail. The lookup result and user data are returned. */
typedef union mem_lb_lookup_direct_result_t
{
    struct
    {
        /*
        * First 64 bit word
        */
        uint32_t lo_result              : 32;   /**< Bottom 32 bits of id table lookup result. */
        uint32_t hi_result              : 32;   /**< Top 32 bits of id table lookup result. */

        /*
         * Second 64 bit word
         */
        uint32_t user_data_1            : 32;   /**< User_data_1. */
        uint32_t user_data_2            : 32;   /**< User data_2. */

     };
    uint32_t value[4];                          /**< Accessor to entire lookup detail structure. */
} mem_lb_lookup_direct_result_t;

#else /* __LITTLEENDIAN */

typedef union mem_lb_lookup_direct_result_t
{
    struct
    {
        uint32_t hi_result              : 32;
        uint32_t lo_result              : 32;

        uint32_t user_data_2            : 32;
        uint32_t user_data_1            : 32;
    };
    uint32_t value[4];
} mem_lb_lookup_direct_result_t;

#endif

/**  lookup result in read_reg
 */
typedef __xread mem_lb_lookup_direct_result_t    mem_lb_lookup_direct_result_in_read_reg_t;



/** Helper macro to extract the hash_seed_high (higher 11 bits) from 24-bit hash seed as needed for
 * load balance descriptor data (mem_lb_desc_t).
 */
#define MEM_LB_HASH_SEED_HIGH(seed)  ((seed >> 13) & 0x7ff);


/** Helper macro to extract the hash_seed_low (lower 13 bits) from 24-bit hash seed as needed for
 * load balance descriptor data (mem_lb_desc_t).
 */
#define MEM_LB_HASH_SEED_LOW(seed)  (seed & 0x1fff);


/** Helper macro to extract the hash_base_addr_high (higher 5 bits) from 18-bit hash base address as needed for
 * load balance descriptor data (mem_lb_desc_t).
 */
#define MEM_LB_HASH_BASE_ADDR_HIGH(addr)  ((addr >> 13) & 0x1f);


/** Helper macro to extract the hash_base_addr_low (lower 13 bits) from 18-bit hash base address as needed for
 * load balance descriptor data (mem_lb_desc_t).
 */
#define MEM_LB_HASH_BASE_ADDR_LOW(addr)  (addr & 0x1fff);


/** Helper macro to concatenate hash base address from load balance descriptor data (mem_lb_desc_t).
 * The hash_base_addr_high and hash_base_addr_low fields are concatenated for the hash base address.
 */
#define MEM_LB_HASH_BASE_ADDR_FROM_DESC(desc) (desc.hash_base_addr_high << 13) | desc.hash_base_addr_low;


/** Helper macro to concatenate hash seed from load balance descriptor data (mem_lb_desc_t).
 * The hash_seed_high and hash_seed_low fields are concatenated for the hash seed.
 */
#define MEM_LB_HASH_SEED_FROM_DESC(desc)  (desc.hash_seed_high << 13) | desc.hash_seed_low;


/** Helper macro to concatenate hash base address from load balance descriptor data (mem_lb_desc_t).
 *  The hash_base_addr_high and hash_base_addr_low fields are concatenated for the hash base address.
 */
#define MEM_LB_HASH_BASE_ADDR_FROM_DESC(desc) (desc.hash_base_addr_high << 13) | desc.hash_base_addr_low;





/** Write descriptor data into the Local Descriptor Array.
 *
 * @param xfer              Transfer registers containing descriptor data.
 * @param address           40 bit pointer with array entry location (mem_lb_descriptor_address_format_t) in lower 32 bits
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param count             Length in 64 bit words to write, calculated as count * 3 (valid values 1- 4).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap).
 * @param sig_ptr           Signal to raise upon completion.
 * *
 * Load descriptor data into the Local Descriptor Array. The array supports up to 64 Descriptors.
 * Descriptors must be loaded into the Local Descriptor Array prior to doing lookups.
 */
__intrinsic
void cmd_mem_lb_write_desc(
    mem_lb_desc_in_write_reg_t *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read descriptor data from the Local Descriptor Array.
 *
 * @param xfer              Transfer registers containing descriptor data
 * @param address           40 bit pointer with array entry location (mem_lb_descriptor_address_format_t) in lower 32 bits
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param count             Length in 64 bit words to read, calculated as count * 3 (valid values 1- 4).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * Read descriptor data from the Local Descriptor Array. The array supports up to 64 Descriptors.
 * Descriptors must be loaded into the Local Descriptor Array prior to doing lookups.
 */
__intrinsic
void cmd_mem_lb_read_desc(
    mem_lb_desc_in_read_reg_t *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Write entries to the Local ID Table Array.
 *
 * @param xfer              Transfer registers containing entries to write.
 * @param address           40 bit pointer with array entry location (mem_lb_id_table_address_format_t) in lower 32 bits.
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param count             Length in 64 bit words to write (valid values 1 - 16).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * Write entries to the Local ID Table Array. The array supports up to 256 entries.
 */
__intrinsic
void cmd_mem_lb_write_id_table(
    __xwrite void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read entries from the Local ID Table Array.
 *
 * @param xfer              Transfer registers containing entries read.
 * @param address           40 bit pointer with array entry location (mem_lb_id_table_address_format_t) in lower 32 bits.
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param count             Length in 64 bit words to read (valid values 1 - 16).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap).
 * @param sig_ptr           Signal to raise upon completion.
 *
 * Read entries from the Local ID Table Array. The array supports up to 256 entries.
 */
__intrinsic
void cmd_mem_lb_read_id_table(
    __xread void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);



/** Write entries to the Local Hash Table Bucket Array.
 *
 * @param xfer              Transfer registers containing entries to write.
 * @param address           40 bit pointer with bucket address (mem_lb_bucket_local_address_format_t) in lower 32 bits.
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param count             Length in 64 bit words to write, calculated as count * 2 (valid values 1 - 8).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap).
 * @param sig_ptr           Signal to raise upon completion.
 *
 *
 * Write entries to the Local Hash Table Bucket Array. The array supports up to 512 entries
 * with each entry 128 bits in width.
 */
__intrinsic
void cmd_mem_lb_bucket_write_local(
    __xwrite void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read entries from the Local Hash Table Array.
 *
 * @param xfer              Transfer registers containing descriptor data.
 * @param address           40 bit pointer with bucket address (mem_lb_bucket_local_address_format_t) in lower 32 bits.
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param count             Length in 64 bit words to read calculated as count * 2 (valid values 1 - 8).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap).
 * @param sig_ptr           Signal to raise upon completion.
 *
 *
 * Read entries from the Local Hash Table Bucket Array. The array supports up to 512 entries
 * with each entry 128 bits in width.
 */
__intrinsic
void cmd_mem_lb_bucket_read_local(
    __xread void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);




/** Write entries to the DCache Hash Table Bucket Array.
 *
 * @param xfer              Transfer registers containing entries to write.
 * @param address           40 bit pointer with bucket address ( mem_lb_bucket_dcache_address_format_t) in lower 32 bits.
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param count             Length in 64 bit words to read calculated as count * 2 (valid values 1 - 8).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap).
 * @param sig_ptr           Signal to raise upon completion.
 *
 * Write entries to the DCache Hash Table Bucket Array. The array supports up to 512 entries
 * with each entry 128 bits in width.
 */
__intrinsic
void cmd_mem_lb_bucket_write_dcache(
    __xwrite void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read entries from the DCache Hash Table Bucket Array.
 *
 * @param xfer              Transfer registers containing descriptor data.
 * @param address           40 bit pointer with bucket address (mem_lb_bucket_dcache_address_format_t) in lower 32 bits.
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param count             Length in 64 bit words to read calculated as count * 2 (valid values 1 - 8).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap).
 * @param sig_ptr           Signal to raise upon completion.
 *
 * Read entries from the DCache Hash Table Bucket Array. The array supports up to 512 entries
 * with each entry 128 bits in width.
 */
__intrinsic
void cmd_mem_lb_bucket_read_dcache(
    __xread void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Read statistics data from the Local Stats Table Array. Also clear the statistics after read.
 *
 * @param xfer              Transfer registers containing statistics data.
 * @param address           40 bit pointer with bucket address (mem_lb_bucket_local_address_format_t) in lower 32 bits.
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param count             Length in 64 bit words to read (valid values 1 - 16).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 *
 * Read and return stats data from the Local Stats Table Array. Write zeros into same location to clear stats data.
 */
__intrinsic
void cmd_mem_lb_stats_read_and_clear_local(
    __xread void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Read statistics data from the Local Stats Table Array
 *
 * @param xfer              Transfer registers containing statistics data.
 * @param address           40 bit pointer with bucket address (mem_lb_bucket_local_address_format_t) in lower 32 bits.
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param count             Length in 64 bit words to read (valid values 1 - 16).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion.
 *
 * Read and return stats data from the Local Stats Table Array.
 */
__intrinsic
void cmd_mem_lb_stats_read_local(
    __xread void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Read statistics data from the designated location in DCache. Also clear the statistics after read.
 *
 * @param xfer              Transfer registers containing statistics data  (mem_lb_read_stats_address_format_t).
 * @param address           40 bit pointer with bucket address (mem_lb_dcache_stats_address_format_t) in lower 32 bits.
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param count             Length in 64 bit words to read (valid values 1 - 16).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap).
 * @param sig_ptr           Signal to raise upon completion.
 *
 * Read and return stats data from DCache. Write zeros into same location to clear stats data.
 */
__intrinsic
void cmd_mem_lb_stats_read_and_clear_dcache(
    __xread void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Read statistics data from the designated location in DCache.
 *
 * @param xfer              Transfer registers containing statistics data (mem_lb_read_stats_address_format_t).
 * @param address           40 bit pointer with bucket address (mem_lb_dcache_stats_address_format_t) in lower 32 bits.
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param count             Length in 64 bit words to read (valid values 1 - 16).
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion.
 *
 *
 * Read and return stats data from DCache.
 */
__intrinsic
void cmd_mem_lb_stats_read_dcache(
    __xread void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Lookup bundle id using has input and update statistics.
 *
 * @param xfer              Transfer registers containing one 64 bit word of hash input and statistics
 * @param address           40 bit pointer with bucket address (mem_lb_descriptor_address_format_t) in lower 32 bits.
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup (two 64-bit words).
 *
 * Perform a comparison lookup on 24-bit initial hash input and return bundleId. Add 16-bit stat input data
 * to the statistics counter. Pull one 64-bit word of Hash and Stat, push two 64-bit words as result.
 * This is also called indirect lookup.
 *
 * Below is an example of bundle id lookup on MU island 29 (imem1).
 * Steps:
 * 1. Initialise 0x0f to load balancing hash multiply CSR. Hash SBox is left at default.
 * 2. Initialise descriptor array (3) to Boundaries=3, CompareBits=24.
 * 3. Write to the hash bucket table.
 * 4. Do lookup with hash input.
 *
 * @code
 *    SIGNAL              sig;
 *    uint64_t  mu_island = (uint64_t)LoadTimeConstant("__ADDR_I29_IMEM");
 *    uint32_t        descriptor_array = 3;
 *    uint32_t        count = 1;
 *    uint32_t        write_stat_base_addr = 0x100;
 *    uint32_t        write_hash_base_addr = 0x00;
 *    uint32_t        write_user_data_1 = 0x9abcdeff;
 *    uint32_t        write_user_data_2 = 0x12345678;
 *    uint32_t        write_hash_seed = 0xaaaaaa;
 *
 *    // Write 0x0f to load balancing hash multiply CSR
 *    {
 *         cluster_target_xpb_address_format_t    xpb_command;
 *         __xwrite  uint32_t    wr_xfer;
 *         xpb_command.value = 0;
 *         xpb_command.device = 0x20;
 *         xpb_command.target_island = 29;
 *         xpb_command.global_xpb = 1;
 *
 *        wr_xfer = 0x0f;
 *        cmd_cluster_target_xpb_write((void *)&wr_xfer, &xpb_command, 1, ctx_swap, &sig);
 *    }
 *
 *    // Initialise load balancing descriptor array 3
 *    cmd_mem_lb_init
 *    (
 *        descriptor_array,
 *        mu_island >> 32,
 *        write_stat_base_addr,
 *        LB_LOCATION_LOCAL_MEM,
 *        write_hash_base_addr,
 *        LB_LOCATION_LOCAL_MEM,
 *        3,      // boundaries
 *        0,      // index_bits
 *       24,     // compare_bits
 *        write_hash_seed,
 *        ((uint64_t)write_user_data_1 << 32) | write_user_data_2,
 *        count
 *    );
 *
 *    // Write hash bucket
 *    // Refer to Table 9.51 "HashBucket - Boundaries=3, CompareBits=24" in 6xxx databook
 *    {
 *        mem_lb_bucket_local_address_format_t        hb_addr;
 *        volatile __xwrite uint32_t write_xfer[4];
 *        __xread uint32_t           read_xfer[4];
 *        uint32_t                                hb_entry = 0;
 *        uint32_t                                bnd0 = 0x900000,
 *                                                    bnd1 = 0xa00000,
 *                                                    bnd2 = 0xb00000;
 *
 *        write_xfer[3] = bnd2 >> 16;
 *        write_xfer[2] = (bnd2 << 16) | (bnd1 >> 8);
 *        write_xfer[1] = (bnd1 << 24) | bnd0;
 *        write_xfer[0] = (3 << 24) | (2 << 16) | (1 << 8) | 0; // regions
 *
 *        hb_addr.value = 0;
 *        hb_addr.bucket_address = hb_entry;
 *
 *        {
 *            uint64_t addr = mu_island |
 *                      (uint64_t)(write_hash_base_addr | hb_addr.value);
 *
 *            cmd_mem_lb_bucket_write_local(
 *                (void *)write_xfer,
 *                (__addr40 __mem void *) addr,
 *                1,
 *                ctx_swap,
 *                &sig
 *                );
 *            cmd_mem_lb_bucket_read_local(
 *                (void *)read_xfer,
 *                (__addr40 __mem void *) addr,
 *                1,
 *                ctx_swap,
 *                &sig
 *            );
 *        }
 *    }
 *
 *    // Do lookup
 *    {
 *        SIGNAL_PAIR                                  sig_pair;
 *        mem_lb_lookup_bundle_id_result_in_read_reg_t *result;
 *        mem_lb_lookup_command_format_in_write_reg_t  write_data;
 *        uint64_t                           addr = (mu_island | descriptor_array);
 *
 *        write_data.value[0] = 0;
 *        write_data.value[1] = 0;
 *        write_data.stat_add_value = 0x2233;
 *        write_data.hash_input = 0x200000;
 *
 *        result = cmd_mem_lb_lookup_bundle_id(
 *            &write_data,
 *            (__addr40 __mem void *)addr,
 *            sig_done,
 *            &sig_pair
 *        );
 *        wait_for_all_single(&sig_pair.odd);
 *
 *        if (result->user_data_1 != write_user_data_1)
 *        {
 *            return 0;        // error;
 *        }
 *
 *        if (result->user_data_2 != write_user_data_2)
 *        {
 *            return 0;        // error
 *        }
 *
 *        // bundle id must be 0
 *        if (result->bundle_id != 0)
 *        {
 *            return 0;        // error
 *        }
 *    }
 *
 *    return 1;
 *
 * @endcode
 */
__intrinsic
mem_lb_lookup_bundle_id_result_in_read_reg_t *cmd_mem_lb_lookup_bundle_id(
                mem_lb_lookup_command_format_in_write_reg_t *xfer,
                volatile void __addr40 __mem *address,
                sync_t sync,
                SIGNAL_PAIR *sig_pair_ptr
);


/** Lookup DCache using has input and update statistics.
 *
 * @param xfer              Transfer registers containing one 64 bit word of hash and statistics
 * @param address           40 bit pointer with bucket address (mem_lb_descriptor_address_format_t) in lower 32 bits.
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup (two 64-bit words).
 *
 * Perform a comparison lookup on 24-bit initial hash input, use the bundleId to address the Id table and
 * use the the Id table result to address the designated location in DCache that becomes the DCache lookup result.
 * Add 16-bit stat input data to the statistics counter. Pull one 64-bit word of Hash and Stat, push two 64-bit words as result.
 * This is also called direct DCache lookup.
 */
__intrinsic
mem_lb_lookup_direct_result_in_read_reg_t *cmd_mem_lb_lookup_dcache(
                mem_lb_lookup_command_format_in_write_reg_t *xfer,
                volatile void __addr40 __mem *address,
                sync_t sync,
                SIGNAL_PAIR *sig_pair_ptr
);


/** Lookup IdTable using has input and update statistics.
 *
 * @param xfer              Transfer registers containing one 64 bit word of hash and statistics
 * @param address           40 bit pointer with bucket address (mem_lb_descriptor_address_format_t) in lower 32 bits.
 *                           See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup (two 64-bit words).
 *
 * Perform a comparison lookup on 24-bit initial hash input, use the bundleId to address the Id table and
 * return the Id table result. Add 16-bit stat input data to the statistics counter.
 * Pull one 64-bit word of Hash and Stat, push two 64-bit words as result.
 * This is also called direct Id table lookup.
 */
__intrinsic
mem_lb_lookup_direct_result_in_read_reg_t  *cmd_mem_lb_lookup_id_table(
                mem_lb_lookup_command_format_in_write_reg_t *xfer,
                volatile void __addr40 __mem *address,
                sync_t sync,
                SIGNAL_PAIR *sig_pair_ptr
);

/** Initialise the descriptor array prior to doing lookups.
 *
 * @param descriptor_array_number       Descriptor array entry location (0 - 63).
 * @param mu_island                     8 high bits of 40-bit address indicating the mu_island.
 *                                      See 6xxx databook for recommended addressing mode for higher 8 bits of address parameter.
 * @param stat_base_addr                Statistics base address
 * @param stat_addr_ref                 Location reference for statitsics address (LB_LOCATION_LOCAL_MEM or LB_LOCATION_DCACHE)
 * @param hash_base_addr                Hash base address
 * @param hash_addr_ref                 Location reference for hash address (LB_LOCATION_LOCAL_MEM or LB_LOCATION_DCACHE)
 * @param boundaries                    Boundary value that is used to divide the hash into regions. The number of boundaries in a hash
 *                                      bucket is determined by the descriptor value. Hash is compared to boundary value in order to
 *                                      determine region indirection index
 * @param index_bits                    Number of bits of hash to use as address offset
 * @param compare_bits                  Number of bits of hash to use for compare
 * @param hash_seed                     Hash seed value
 * @param user_data                     User specific data
 * @param count                         Length in 64 bit words to write, calculated as count * 3 (valid values 1- 4)
 *
 * @see cmd_mem_lb_write_desc()
 *
 * A valid descriptor must be loaded to the Descriptor Array prior to issuing any descriptor-
 * dependent commands (eg. LBLookup). cmd_mem_lb_write_desc() is used to initialise a location in the descriptor array.
 * The descriptor array supports 64 descriptors.
 * Boundaries, index_bits and compare_bits have only a few supported configurations. See Supported Descriptor Parameters
 * Table in 6xxx databook.
 *
 * An example of cmd_mem_lb_init() is given below. The descriptor is stored in array number 4.
 * cmd_mem_lb_read_desc() is used to read back the descriptor and verify the stored values.
 *
 * @code
 *    uint64_t island = (uint64_t)LoadTimeConstant("__ADDR_I28_IMEM");
 *    mem_lb_descriptor_address_format_t          descriptor;
 *    uint32_t                                descriptor_array = 5;
 *    uint32_t                                count = 1;
 *
 *    uint32_t write_stat_base_addr = 0x100;
 *    uint32_t write_hash_base_addr = 0x00;
 *    uint32_t write_user_data_1 = 0x9abcdeff;
 *    uint32_t write_user_data_2 = 0x12345678;
 *    uint32_t write_hash_seed = 0xaaaaaa;
 *
 *    descriptor.value = 0;
 *    descriptor.array_entry = descriptor_array;
 *
 *    cmd_mem_lb_init
 *    (
 *        descriptor.array_entry,
 *        island >> 32,
 *        write_stat_base_addr,
 *        0,
 *        write_hash_base_addr,
 *        0,
 *        3,      // boundaries
 *        0,      // index_bits
 *        24,     // compare_bits
 *        write_hash_seed,
 *        ((uint64_t)write_user_data_1 << 32) | write_user_data_2,
 *        count
 *    );
 *
 *    // Verify by reading back the descriptor
 *    {
 *        uint64_t address = ((uint64_t) island | descriptor.value);
 *        SIGNAL             sig;
 *        mem_lb_desc_in_read_reg_t       lb_read_xfer_desc;
 *
 *        cmd_mem_lb_read_desc(
 *            (void *)&lb_read_xfer_desc.value[0],
 *            (__addr40 __mem void *)address,
 *            count,
 *            ctx_swap,
 *            &sig
 *        );
 *
 *        // Verify user data
 *        if (lb_read_xfer_desc.user_data_2  != write_user_data_2)
 *        {
 *            return 0;        // We have an error
 *        }
 *
 *        if (lb_read_xfer_desc.user_data_1  != write_user_data_1)
 *        {
 *            return 0;        // We have an error
 *        }
 *
 *        // Verify hash seed
 *        {
 *            uint32_t read_hash_seed = MEM_LB_HASH_SEED_FROM_DESC(lb_read_xfer_desc);
 *
 *            if (read_hash_seed != write_hash_seed)
 *            {
 *                return 0;        // We have an error
 *            }
 *        }
 *
 *        // Verify statistics address
 *        {
 *            uint32_t read_stat_base_addr= lb_read_xfer_desc.stat_base_addr;
 *            uint32_t read_stat_addr_ref = lb_read_xfer_desc.stat_addr_ref;
 *
 *            if (read_stat_base_addr != write_stat_base_addr)
 *            {
 *                return 0;        // We have an error
 *            }
 *
 *            if (read_stat_addr_ref != 0)
 *            {
 *                return 0;        // We have an error
 *            }
 *        }
 *
 *        // Verify hash address
 *        {
 *            uint32_t read_hash_addr_ref = lb_read_xfer_desc.hash_addr_ref;
 *            uint32_t read_hash_base_addr = MEM_LB_HASH_BASE_ADDR_FROM_DESC(lb_read_xfer_desc);
 *
 *            if (read_hash_base_addr != write_hash_base_addr)
 *            {
 *                return 0;        // We have an error
 *            }
 *
 *            if (read_hash_addr_ref != 0)
 *            {
 *                return 0;        // We have an error
 *            }
 *        }
 *    }
 *
 *    return 1;
 * @endcode
 */
__intrinsic
void cmd_mem_lb_init(
    uint32_t descriptor_array_number,
    uint32_t mu_island,
    uint32_t stat_base_addr,
    enum MEM_LB_LOCATION stat_addr_ref,
    uint32_t hash_base_addr,
    enum MEM_LB_LOCATION hash_addr_ref,
    uint32_t boundaries,
    uint32_t index_bits,
    uint32_t compare_bits,
    uint32_t hash_seed,
    uint64_t user_data,
    uint32_t count
);
/**
 *  @}
 * @}
 */

#endif /* __NFP_MEM_LOAD_BALANCE_ENGINE_H__ */
