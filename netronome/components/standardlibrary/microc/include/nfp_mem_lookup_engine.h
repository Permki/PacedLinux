/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* NFP MEM LOAD_BALANCE intrinsic function definitions */

#ifndef __NFP_MEM_LOOKUP_ENGINE_H__
#define __NFP_MEM_LOOKUP_ENGINE_H__

#include <nfp.h>
#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif


/** @file nfp_mem_lookup_engine.h
 * @addtogroup mu_lookup MU Lookup Engine Intrinsics
 * @{
 */

/** @name Lookup Engine functions for MEM
 *  @{
 *
 * Lookup engine functions are available on internal memory or external memory. Different lookups are
 * available, refer to NFP-6xxx Programmer Reference Manual for a list.
 *
 * Below is a complete example with 32-bit direct lookup on island 28 (imem0).
 * The DLUT table is populated and then a lookup is performed of a specific index.
 *
 * @code
 *    // i28 imem is used for island and base address
 *    uint64_t                          island =  (uint64_t)LoadTimeConstant("__ADDR_I28_IMEM");
 *    __imem_n(0) __addr40 uint32_t     base_address[16];
 *
 *
 *    // Write to 32-bit DLUT table. See Table 9.66 "DLUT32 Memory Contents" in 6xxx databook
 *    {
 *        volatile __xwrite uint32_t wr_reg[8];
 *        uint32_t                                i;
 *        SIGNAL                                      sig;
 *
 *        // write in chunks of 128 bits for each data line
 *        // (result 0 to result 3/result 4 - result 7/..)
 *        for (i = 0; i < 4; i++)
 *        {
 *            wr_reg[i] = 0x11110000 + i;
 *        }
 *
 *        cmd_mem_write64_ptr40(
 *                (void *)wr_reg,
 *                __addr40 __mem void *)(&base_address[0]),
 *                2,
 *                ctx_swap,
 *                &sig
 *            );
 *
 *        for (i = 0; i < 4; i++)
 *        {
 *            wr_reg[i] = 0x22222222 + i;
 *        }
 *
 *        cmd_mem_write64_ptr40(
 *                (void *)wr_reg,
 *                __addr40 __mem void *)(&base_address[4]),
 *                2,
 *                ctx_swap,
 *                &sig
 *            );
 *
 *        for (i = 0; i < 4; i++)
 *        {
 *            wr_reg[i] = 0x33333333 + i;
 *        }
 *
 *        cmd_mem_write64_ptr40(
 *                (void *)wr_reg,
 *                __addr40 __mem void *)(&base_address[8]),
 *                2,
 *                ctx_swap,
 *                &sig
 *            );
 *
 *        for (i = 0; i < 4; i++)
 *        {
 *            wr_reg[i] = 0x44444444 + i;
 *        }
 *
 *        cmd_mem_write64_ptr40(
 *                (void *)wr_reg,
 *                __addr40 __mem void *)(&base_address[12]),
 *                2,
 *                ctx_swap,
 *                &sig
 *            );
 *
 *        // verify that all information is written before we do lookup.
 *        {
 *            __xread uint32_t  rd_reg[2];
 *            cmd_mem_read64_ptr40(
 *                    (void *)rd_reg,
 *                    __addr40 __mem void *)(&base_address[12]),
 *                    1,
 *                    ctx_swap,
 *                    &sig
 *                );
 *        }
 *    }
 *
 *    // Do the lookup on index 0x0a
 *    {
 *        volatile mem_lookup_dlut_small_t                small_table_desc;
 *        volatile __xwrite  uint32_t    lookup_index[4];
 *        mem_lookup_result_in_read_reg_t                 *read_result;
 *        SIGNAL_PAIR                                     sig_pair;
 *
 *        lookup_index[0] = 0x0a;
 *        lookup_index[1] = 0x00;
 *
 *        small_table_desc.value = 0;
 *        small_table_desc.start_bit_position = 0;
 *        small_table_desc.table_type = LOOKUP_DLUT_SMALL;
 *        small_table_desc.table_size = LOOKUP_DLUT_SMALL_SIZE_1K;
 *        small_table_desc.result = LOOKUP_32BIT_RESULT;
 *        small_table_desc.direct_lookup = 1;
 *        small_table_desc.internal_memory = 1;
 *
 *        // Refer to Table 9.64 "Small Direct Lookup Table (word address) -DLUT32" in 6xxx databook
 *        // Only lower bits are used for base address.
 *        small_table_desc.base_address = (uint32_t)base_address >> 12;
 *
 *        {
 *            uint64_t lookup_addr = (island | small_table_desc.value);
 *
 *            read_result = cmd_mem_lookup(
 *                (void *)&lookup_index[0],
 *                __addr40 __mem void *)(lookup_addr),
 *                1,
 *                sig_done,
 *                &sig_pair
 *            );
 *            wait_for_all(&sig_pair);
 *        }
 *        if (read_result->result != 0x33333335)
 *        {
 *            return 0;        // We have an error
 *        }
 *    }
 *    return 1;
 * @endcode
 */

/** Direct lookup small table sizes defined by the number of entries.
 *  The actual size of the table is dependent on 24-bit or 32-bit result
 *  (see MEM_LOOKUP_RESULT) and the number of entries
 *      - 24 bit result has table size of 3 * number of entries.
 *      - 32 bit result has table size of 4 * number of entries.
 *  Small tables must be 4K byte aligned.
 */
typedef enum MEM_LOOKUP_DLUT_SMALL_SIZE
{
    LOOKUP_DLUT_SMALL_SIZE_1K          = 0x00,      /**< 1K table entries.   */
    LOOKUP_DLUT_SMALL_SIZE_2K          = 0x01,      /**< 2K table entries.   */
    LOOKUP_DLUT_SMALL_SIZE_4K          = 0x02,      /**< 4K table entries.   */
    LOOKUP_DLUT_SMALL_SIZE_8K          = 0x03,      /**< 8K table entries.   */
    LOOKUP_DLUT_SMALL_SIZE_16K         = 0x04,      /**< 16K table entries.  */
    LOOKUP_DLUT_SMALL_SIZE_32K         = 0x05,      /**< 32K table entries.  */
    LOOKUP_DLUT_SMALL_SIZE_64K         = 0x06,      /**< 64K table entries.  */
    LOOKUP_DLUT_SMALL_SIZE_128K        = 0x07       /**< 128K table entries. */

}  MEM_LOOKUP_DLUT_SMALL_SIZE;


/** Direct lookup large table sizes defined by the number of entries.
 *  The actual size of the table is dependent on 24-bit or 32-bit result
 *  (see MEM_LOOKUP_RESULT) and the number of entries
 *      - 24 bit result has table size of 3 * number of entries.
 *      - 32 bit result has table size of 4 * number of entries.
 *  Large tables must be 256K byte aligned.
 */
typedef enum MEM_LOOKUP_DLUT_LARGE_SIZE
{
    LOOKUP_DLUT_LARGE_SIZE_64K         = 0x00,      /**< 64K table entries.  */
    LOOKUP_DLUT_LARGE_SIZE_128K        = 0x01,      /**< 128K table entries. */
    LOOKUP_DLUT_LARGE_SIZE_256K        = 0x02,      /**< 256K table entries. */
    LOOKUP_DLUT_LARGE_SIZE_512K        = 0x03,      /**< 512K table entries. */
    LOOKUP_DLUT_LARGE_SIZE_1M          = 0x04,      /**< 1M table entries.   */
    LOOKUP_DLUT_LARGE_SIZE_2M          = 0x05,      /**< 2M table entries.   */
    LOOKUP_DLUT_LARGE_SIZE_4M          = 0x06,      /**< 4M table entries.   */
    LOOKUP_DLUT_LARGE_SIZE_8M          = 0x07       /**< 8M table entries.   */

}  MEM_LOOKUP_DLUT_LARGE_SIZE;


/** Lookup result bits */
typedef enum MEM_LOOKUP_RESULT
{
    LOOKUP_24BIT_RESULT                 = 0x00,      /**< 24 bit result.  */
    LOOKUP_32BIT_RESULT                 = 0x01       /**< 32 bit result. */
}  MEM_LOOKUP_RESULT;

/** Lookup result bits */
typedef enum MEM_LOOKUP_DLUT_TYPE
{
    LOOKUP_DLUT_SMALL                 = 0x00,      /**< Small table - 1K to 128K.  */
    LOOKUP_DLUT_LARGE                 = 0x01       /**< Large table - 64K to 8M. */
}   MEM_LOOKUP_DLUT_TYPE;



#ifdef __BIGENDIAN

/** Command for direct lookup small table */
typedef union mem_lookup_dlut_small_t
{
    struct
    {
        uint32_t internal_memory                : 1;    /**< If internal memory is used. */
        uint32_t direct_lookup                  : 1;    /**< 1 = direct lookup. */
        uint32_t base_address                   : 16;   /**< Base address at which the first result is located. */
        uint32_t reserved_2                     : 1;    /**< Reserved. */
        MEM_LOOKUP_RESULT result                    : 1;    /**< 0 = 24 bits, 1 = 32 bit. */
        uint32_t reserved_1                     : 1;    /**< Reserved. */
        MEM_LOOKUP_DLUT_SMALL_SIZE table_size       : 3;    /**< Specified table entry size. */
        uint32_t table_type                     : 1;    /**< 0 = small table type: 1K to 128K. */
        uint32_t start_bit_position             : 7;    /**< Starting bit position, bit 0 to bit 127. */
    };
    uint32_t value;                                     /**< Accessor to entire descriptor structure. */
} mem_lookup_dlut_small_t;

#else /* __LITTLEENDIAN */

typedef union mem_lookup_dlut_small_t
{
    struct
    {
        uint32_t start_bit_position                 : 7;
        uint32_t type                               : 1;
        MEM_LOOKUP_DLUT_SMALL_SIZE  table_size          : 3;
        uint32_t reserved_1                         : 1;
        MEM_LOOKUP_RESULT result                        : 1;
        uint32_t reserved_2                         : 1;
        uint32_t base_address                       : 16;
        uint32_t direct_lookup                      : 1;
        uint32_t internal_memory                    : 1;
    };
    uint32_t value;
} mem_lookup_dlut_small_t;

#endif



#ifdef __BIGENDIAN

/** Command for direct lookup large table */
typedef union mem_lookup_dlut_large_t
{
    struct
    {
        uint32_t internal_memory                    : 1;    /**< If internal memory is used. */
        uint32_t direct_lookup                      : 1;    /**< 1 = direct lookup. */
        uint32_t base_address                       : 15;   /**< Base address at which the first result is located. */
        uint32_t reserved_2                         : 2;    /**< Reserved. */
        MEM_LOOKUP_RESULT result                        : 1;    /**< 0 = 24 bits, 1 = 32 bits. */
        uint32_t reserved_1                         : 1;    /**< Reserved. */
        MEM_LOOKUP_DLUT_LARGE_SIZE table_size           : 3;    /**< Specified table size. */
        uint32_t table_type                         : 1;    /**< 1 = large table size. 64K to 8M. */
        uint32_t start_bit_position                 : 7;    /**< Starting bit position, bit 0 to bit 127. */
    };
    uint32_t value;                                         /**< Accessor to entire descriptor structure. */
} mem_lookup_dlut_large_t;

#else /* __LITTLEENDIAN */

typedef union mem_lookup_dlut_large_t
{
    struct
    {
        uint32_t start_bit_position                 : 7;
        uint32_t type                               : 1;
        MEM_LOOKUP_DLUT_LARGE_SIZE table_size           : 3;
        uint32_t reserved_1                         : 1;
        MEM_LOOKUP_RESULT result                        : 1;
        uint32_t reserved_2                         : 1;
        uint32_t base_address                       : 16;
        uint32_t direct_lookup                      : 1;
        uint32_t internal_memory                    : 1;
    };
    uint32_t value;
} mem_lookup_dlut_large_t;

#endif



#ifdef __BIGENDIAN

/** Command for recursive 32-bit direct lookup small table */
typedef union mem_lookup_dlut_small_recursive_t
{
    struct
    {
        uint32_t further_lookup_required        : 1;    /**< If further lookup is required. */
        uint32_t direct_lookup                  : 1;    /**< 1 = direct lookup. */
        uint32_t base_address                   : 16;   /**< Base address at which the first result is located. */
        uint32_t reserved_1                     : 1;    /**< Reserved. */
        MEM_LOOKUP_RESULT result                    : 1;    /**< 0 = 24 bits, 1 = 32 bits. */
        uint32_t mem_location                   : 1;    /**< Location where 0 = IMEM and 1 = EMEM. */
        MEM_LOOKUP_DLUT_SMALL_SIZE table_size       : 3;    /**< Specified table size. */
        uint32_t table_type                     : 1;    /**< 0 = small table type: 1K to 128K. */
        uint32_t start_bit_position             : 7;    /**< Starting bit position, bit 0 to bit 127. */
    };
    uint32_t value;                                     /**< Accessor to entire descriptor structure. */
} mem_lookup_dlut_small_recursive_t;

#else /* __LITTLEENDIAN */

typedef union mem_lookup_dlut_small_recursive_t
{
    struct
    {
        uint32_t start_bit_position                 : 7;
        uint32_t table_type                         : 1;
        MEM_LOOKUP_DLUT_SMALL_SIZE table_size           : 3;
        uint32_t mem_location                       : 1;
        MEM_LOOKUP_RESULT result                        : 1;
        uint32_t reserved_1                         : 1;
        uint32_t base_address                       : 16;
        uint32_t direct_lookup                      : 1;
        uint32_t further_lookup_required            : 1;
    };
    uint32_t value;
} mem_lookup_dlut_small_recursive_t;

#endif



#ifdef __BIGENDIAN

/** Command for recursive 32-bit direct lookup large table */
typedef union mem_lookup_dlut_large_recursive_t
{
    struct
    {
        uint32_t further_lookup_required                        : 1;    /**< If further lookup is required. */
        uint32_t direct_lookup                                  : 1;    /**< 1 = direct lookup. */
        uint32_t base_address                                   : 15;   /**< Base address at which the first result is located. */
        uint32_t reserved_1                                     : 2;    /**< Reserved. */
        MEM_LOOKUP_RESULT result                                    : 1;    /**< 0 = 24 bits, 1 = 32 bits. */
        uint32_t mem_location                                   : 1;    /**< Location where 0 = IMEM and 1 = EMEM. */
        MEM_LOOKUP_DLUT_LARGE_SIZE table_size                       : 3;    /**< Specified table size. */
        uint32_t table_type                                     : 1;    /**< 1 = large table size. 64K to 8M. */
        uint32_t start_bit_position                             : 7;    /**< Starting bit position, bit 0 to bit 127. */
    };
    uint32_t value;                                 /**< Accessor to entire descriptor structure. */
} mem_lookup_dlut_large_recursive_t;

#else /* __LITTLEENDIAN */

typedef union mem_lookup_dlut_large_recursive_t
{
    struct
    {
        uint32_t start_bit_position                 : 7;
        uint32_t type                               : 1;
        MEM_LOOKUP_DLUT_LARGE_SIZE table_size           : 3;
        uint32_t mem_location                       : 1;
        MEM_LOOKUP_RESULT result                        : 1;
        uint32_t reserved_1                         : 1;
        uint32_t base_address                       : 16;
        uint32_t direct_lookup                      : 1;
        uint32_t further_lookup_required            : 1;
    };
    uint32_t value;
} mem_lookup_dlut_large_recursive_t;

#endif




typedef struct  mem_lookup_result_t
{
    uint32_t result;
} mem_lookup_result_t;

typedef __xread mem_lookup_result_t  mem_lookup_result_in_read_reg_t;





/*
 *
 * ALUT
 *
 */



/** Algorithmic lookup table sizes in cache lines where 16 bytes (128-bit) per cache line */
typedef enum MEM_LOOKUP_ALUT_SIZE
{
    LOOKUP_ALUT_SIZE_1          = 0x00,      /**< 1 128-bit cache line.  */
    LOOKUP_ALUT_SIZE_2          = 0x01,      /**< 2 128-bit cache lines. */
    LOOKUP_ALUT_SIZE_3          = 0x02,      /**< 3 128-bit cache lines. */
    LOOKUP_ALUT_SIZE_4          = 0x03       /**< 4 128-bit cache lines. */
}  MEM_LOOKUP_ALUT_SIZE;


/** Algorithmic lookup table sizes in cache lines where 16 bytes (128-bit) per cache line.
 *  algorithmic lookup tables include: ALUT, PMM (prefix match with mask), TCAM (ternary content addressable memory table),
 *  SPLIT and multi-bit tables.
 */
typedef enum MEM_LOOKUP_ALUT_COMMANDS
{
    LOOKUP_ALUT_COMMAND_24_3_4          = 0x80,     /**< ALUT with index of 3-bits and a result of 24-bits with 4 possible results. */
    LOOKUP_ALUT_COMMAND_24_4_7          = 0x81,     /**< ALUT with index of 4-bits and a result of 24-bits with 7 possible results. */
    LOOKUP_ALUT_COMMAND_24_5_4          = 0x82,     /**< ALUT with index of 5-bits and a result of 24-bits with 4 possible results. */
    LOOKUP_ALUT_COMMAND_24_5_8          = 0x83,     /**< ALUT with index of 5-bits and a result of 24-bits with 8 possible results. */
    LOOKUP_ALUT_COMMAND_24_5_14         = 0x84,     /**< ALUT with index of 5-bits and a result of 24-bits with 14 possible results.*/
    LOOKUP_ALUT_COMMAND_24_6_2          = 0x85,     /**< ALUT with index of 6-bits and a result of 24-bits with 2 possible results. */
    LOOKUP_ALUT_COMMAND_24_6_4          = 0x86,     /**< ALUT with index of 6-bits and a result of 24-bits with 4 possible results. */
    LOOKUP_ALUT_COMMAND_32_5_2          = 0x90,     /**< ALUT with index of 5-bits and a result of 32-bits with 2 possible results. */
    LOOKUP_ALUT_COMMAND_32_5_4          = 0x91,     /**< ALUT with index of 5-bits and a result of 32-bits with 4 possible results. */
    LOOKUP_ALUT_COMMAND_32_5_8          = 0x92,     /**< ALUT with index of 5-bits and a result of 32-bits with 8 possible results. */
    LOOKUP_ALUT_COMMAND_32_5_11         = 0x93,     /**< ALUT with index of 5-bits and a result of 32-bits with 11 possible results.*/
    LOOKUP_ALUT_COMMAND_32_7_2          = 0x94,     /**< ALUT with index of 7-bits and a result of 32-bits with 2 possible results. */
    LOOKUP_ALUT_COMMAND_32_7_4          = 0x95,     /**< ALUT with index of 7-bits and a result of 32-bits with 4 possible results. */

    LOOKUP_PMM_COMMAND_32_12_4          = 0xa0,     /**< PMM with index of 12-bits and a result of 32 bits with 4 possible results. */
    LOOKUP_PMM_COMMAND_32_12_7          = 0xa1,     /**< PMM with index of 12-bits and a result of 32 bits with 7 possible results. */
    LOOKUP_PMM_COMMAND_32_12_9          = 0xa2,     /**< PMM with index of 12-bits and a result of 32 bits with 9 possible results. */

    LOOKUP_TCAM_COMMAND_32_8_4          = 0xb0,     /**< TCAM with index of 8-bits and a result of 32 bits with 4 possible results. */
    LOOKUP_TCAM_COMMAND_32_8_8          = 0xb1,     /**< TCAM with index of 8-bits and a result of 32 bits with 8 possible results. */
    LOOKUP_TCAM_COMMAND_32_8_10         = 0xb2,     /**< TCAM with index of 8-bits and a result of 32 bits with 10 possible results.*/
    LOOKUP_TCAM_COMMAND_32_12_4         = 0xb3,     /**< TCAM with index of 12-bits and a result of 32 bits with 4 possible results.*/
    LOOKUP_TCAM_COMMAND_32_12_8         = 0xb4,     /**< TCAM with index of 12-bits and a result of 32 bits with 8 possible results.*/

    LOOKUP_SPLIT_COMMAND_32_8_6         = 0xc0,     /**< SPLIT with index of 8-bits and a result of 32 bits with 6 possible results. */
    LOOKUP_SPLIT_COMMAND_32_16_5        = 0xc1,     /**< SPLIT with index of 16-bits and a result of 32 bits with 5 possible results.*/
    LOOKUP_SPLIT_COMMAND_32_24_9        = 0xc2,     /**< SPLIT with index of 24-bits and a result of 32 bits with 9 possible results.*/
    LOOKUP_SPLIT_COMMAND_32_32_8        = 0xc3,     /**< SPLIT with index of 32-bits and a result of 32 bits with 8 possible results.*/

    LOOKUP_MULTIBIT_COMMAND             = 0xd0      /**< Multi-bit command. */
}  MEM_LOOKUP_ALUT_COMMANDS;



#ifdef __BIGENDIAN

/** Command for algorithmic lookup table */
typedef union mem_lookup_alut_t
{
    struct
    {
        uint32_t internal_memory            : 1;    /**< If internal memory is used. */
        uint32_t direct_lookup              : 1;    /**< 0 = not a direct lookup. */
        uint32_t lookup_type                : 1;    /**< 0 = algorithmic lookup. */
        MEM_LOOKUP_ALUT_SIZE table_size         : 2;    /**< Specified table size. */
        uint32_t table_number               : 3;    /**< Table number 0-7 (algorithmic table location CSR)
                                                             contains address bits 32:28 and IMEM or EMEM select. */
        uint32_t address_bits               : 24;   /**< Address bits 27:4. */
    };
    uint32_t value;                                 /**< Accessor to entire descriptor structure. */
} mem_lookup_alut_t;

#else /* __LITTLEENDIAN */

typedef union mem_lookup_alut_t
{
    struct
    {
        uint32_t address_bits               : 24;
        uint32_t table_number               : 3;
        MEM_LOOKUP_ALUT_SIZE table_size         : 2;
        uint32_t lookup_type                : 1;
        uint32_t direct_lookup              : 1;
        uint32_t internal_memory            : 1;
    };
    uint32_t value;
} mem_lookup_alut_t;

#endif



/*
 *
 * HASH tables
 *
 */



/** Hash lookup table sizes. */
typedef enum MEM_LOOKUP_HASH_TABLE_SIZE
{
    LOOKUP_HASH_TABLE_SIZE_1          = 0x00,       /**< 1K table size.  */
    LOOKUP_HASH_TABLE_SIZE_2          = 0x01,       /**< 2K table size.  */
    LOOKUP_HASH_TABLE_SIZE_3          = 0x02,       /**< 4K table size.  */
    LOOKUP_HASH_TABLE_SIZE_4          = 0x03,       /**< 8K table size.  */
    LOOKUP_HASH_TABLE_SIZE_5          = 0x04,       /**< 16K table size. */
    LOOKUP_HASH_TABLE_SIZE_6          = 0x05,       /**< 32K table size. */
    LOOKUP_HASH_TABLE_SIZE_7          = 0x06,       /**< 64K table size. */
    LOOKUP_HASH_TABLE_SIZE_8          = 0x07        /**< 128K table size.*/
}  MEM_LOOKUP_HASH_TABLE_SIZE;


/** Starting bit position for hash table lookup. */
typedef enum MEM_LOOKUP_HASH_STARTING_BIT
{
    LOOKUP_HASH_BIT_0   = 0x00,         /**< Starting bit position at bit 0. */
    LOOKUP_HASH_BIT_32  = 0x01,         /**< Starting bit position at bit 32. */
    LOOKUP_HASH_BIT_64  = 0x02,         /**< Starting bit position at bit 64. */
    LOOKUP_HASH_BIT_96  = 0x03          /**< Starting bit position at bit 96. */
}  MEM_LOOKUP_HASH_STARTING_BIT;

/** Hash command to use for lookup */
typedef enum MEM_LOOKUP_HASH_COMMANDS
{
    LOOKUP_HASH_COMMAND_CAMR_32_16              = 0x00,     /**< 16 bytes operation. */
    LOOKUP_HASH_COMMAND_CAMR_32_64              = 0x01,     /**< 64 bytes operation. */
    LOOKUP_HASH_COMMAND_CAMR_48_64              = 0x05,     /**< 64 bytes operation. */
    LOOKUP_HASH_COMMAND_CAMR_64_16              = 0x08,     /**< 16 bytes operation. */
    LOOKUP_HASH_COMMAND_CAMR_64_64              = 0x09,     /**< 64 bytes operation. */

    LOOKUP_HASH_COMMAND_CAM_32_16               = 0x10,     /**< 16 bytes operation. */
    LOOKUP_HASH_COMMAND_CAM_32_64               = 0x11,     /**< 64 bytes operation. */
    LOOKUP_HASH_COMMAND_CAM_48_16               = 0x14,     /**< 16 bytes operation. */
    LOOKUP_HASH_COMMAND_CAM_48_64               = 0x15,     /**< 64 bytes operation. */
    LOOKUP_HASH_COMMAND_CAM_64_16               = 0x18,     /**< 16 bytes operation. */
    LOOKUP_HASH_COMMAND_CAM_64_64               = 0x19,     /**< 64 bytes operation. */
    LOOKUP_HASH_COMMAND_CAM_128_16              = 0x1c,     /**< 16 bytes operation. */
    LOOKUP_HASH_COMMAND_CAM_128_64              = 0x1d,     /**< 64 bytes operation. */

    LOOKUP_HASH_COMMAND_LHASHR_16_28_16_2   = 0x20,     /**< 16 bytes bucket size with 2 bucket search. */
    LOOKUP_HASH_COMMAND_LHASHR_16_28_16_4   = 0x28,     /**< 16 bytes bucket size with 4 bucket search. */
    LOOKUP_HASH_COMMAND_LHASHR_16_28_16_7   = 0x2c,     /**< 16 bytes bucket size with 7 bucket search. */

    LOOKUP_HASH_COMMAND_LHASHR_16_28_64_2   = 0x21,     /**< 64 bytes bucket size with 2 bucket search. */
    LOOKUP_HASH_COMMAND_LHASHR_16_28_64_4   = 0x29,     /**< 64 bytes bucket size with 4 bucket search. */
    LOOKUP_HASH_COMMAND_LHASHR_16_28_64_7   = 0x2d,     /**< 64 bytes bucket size with 7 bucket search. */

    LOOKUP_HASH_COMMAND_LHASHR_48_60_16_2   = 0x22,     /**< 16 bytes bucket size with 2 bucket search. */
    LOOKUP_HASH_COMMAND_LHASHR_48_60_16_4   = 0x2a,     /**< 16 bytes bucket size with 4 bucket search. */
    LOOKUP_HASH_COMMAND_LHASHR_48_60_16_7   = 0x2e,     /**< 16 bytes bucket size with 7 bucket search. */

    LOOKUP_HASH_COMMAND_LHASHR_48_60_64_2   = 0x23,     /**< 64 bytes bucket size with 2 bucket search. */
    LOOKUP_HASH_COMMAND_LHASHR_48_60_64_4   = 0x2b,     /**< 64 bytes bucket size with 4 bucket search. */
    LOOKUP_HASH_COMMAND_LHASHR_48_60_64_7   = 0x2f,     /**< 64 bytes bucket size with 7 bucket search. */


    LOOKUP_HASH_COMMAND_LHASH_16_28_16_2   = 0x30,     /**< 16 bytes bucket size with 2 bucket search. */
    LOOKUP_HASH_COMMAND_LHASH_16_28_16_4   = 0x38,     /**< 16 bytes bucket size with 4 bucket search. */
    LOOKUP_HASH_COMMAND_LHASH_16_28_16_7   = 0x3c,     /**< 16 bytes bucket size with 7 bucket search. */

    LOOKUP_HASH_COMMAND_LHASH_16_28_64_2   = 0x31,     /**< 64 bytes bucket size with 2 bucket search. */
    LOOKUP_HASH_COMMAND_LHASH_16_28_64_4   = 0x39,     /**< 64 bytes bucket size with 4 bucket search. */
    LOOKUP_HASH_COMMAND_LHASH_16_28_64_7   = 0x3d,     /**< 64 bytes bucket size with 7 bucket search. */

    LOOKUP_HASH_COMMAND_LHASH_48_60_16_2   = 0x32,     /**< 16 bytes bucket size with 2 bucket search. */
    LOOKUP_HASH_COMMAND_LHASH_48_60_16_4   = 0x3a,     /**< 16 bytes bucket size with 4 bucket search. */
    LOOKUP_HASH_COMMAND_LHASH_48_60_16_7   = 0x3e,     /**< 16 bytes bucket size with 7 bucket search. */

    LOOKUP_HASH_COMMAND_LHASH_48_60_64_2   = 0x33,     /**< 64 bytes bucket size with 2 bucket search. */
    LOOKUP_HASH_COMMAND_LHASH_48_60_64_4   = 0x3b,     /**< 64 bytes bucket size with 4 bucket search. */
    LOOKUP_HASH_COMMAND_LHASH_48_60_64_7   = 0x3f      /**< 64 bytes bucket size with 7 bucket search. */
}  MEM_LOOKUP_HASH_COMMANDS;

#ifdef __BIGENDIAN

/** Command for hash lookup table */
typedef union mem_lookup_hash_table_t
{
    struct
    {
        uint32_t internal_memory                            : 1;    /**< If internal memory is used: 1 = IMEM 0=EMEM. */
        uint32_t direct_lookup                              : 1;    /**< 0 = not a direct lookup. */
        uint32_t hash_lookup                                : 1;    /**< 1 = hash lookup. */
        uint32_t base_address                               : 17;   /**< 17-bit base address. */
        uint32_t reserved_1                                 : 1;    /**< Reserved. */
        MEM_LOOKUP_HASH_TABLE_SIZE table_size                   : 3;    /**< Specified table size. */
        uint32_t hash_command                               : 6;    /**< 6-bit hash command. */
        MEM_LOOKUP_HASH_STARTING_BIT start_bit_position         : 2;    /**< Starting bit position. */
    };
    uint32_t value;                                                 /**< Accessor to entire descriptor structure. */
} mem_lookup_hash_table_t;

#else /* __LITTLEENDIAN */

typedef union mem_lookup_hash_table_t
{
    struct
    {
        MEM_LOOKUP_HASH_STARTING_BIT start_bit_position             : 2;
        uint32_t hash_command                                   : 6;
        MEM_LOOKUP_HASH_TABLE_SIZE table_size                       : 3;
        uint32_t reserved_1                                     : 1;
        uint32_t base_address                                   : 17;
        uint32_t hash_lookup                                    : 1;
        uint32_t direct_lookup                                  : 1;
        uint32_t internal_memory                                : 1;
    };
    uint32_t value;
} mem_lookup_hash_table_t;

#endif

#ifdef __BIGENDIAN

/** Command for recursive hash lookup table */
typedef union mem_lookup_recursive_hash_table_t
{
    struct
    {
        uint32_t further_lookup_required                    : 1;    /**< 1 = further lookup required. */
        uint32_t direct_lookup                              : 1;    /**< 0 = not a direct lookup. */
        uint32_t hash_lookup                                : 1;    /**< 1 = hash lookup. */
        uint32_t base_address                               : 17;   /**< 17-bit base address. */
        uint32_t mem_location                               : 1;    /**<  Location where 0 = IMEM and 1 = EMEM. */
        MEM_LOOKUP_HASH_TABLE_SIZE table_size                   : 3;    /**< Specified table size. */
        uint32_t hash_command                               : 6;    /**< 6-bit hash command. */
        MEM_LOOKUP_HASH_STARTING_BIT start_bit_position         : 2;    /**< Starting bit position. */
    };
    uint32_t value;                                 /**< Accessor to entire descriptor structure. */
} mem_lookup_recursive_hash_table_t;

#else /* __LITTLEENDIAN */

typedef union mem_lookup_recursive_hash_table_t
{
    struct
    {
        MEM_LOOKUP_HASH_STARTING_BIT start_bit_position         : 2;
        uint32_t hash_command                               : 6;
        MEM_LOOKUP_HASH_TABLE_SIZE table_size                   : 3;
        uint32_t mem_location                               : 1;
        uint32_t base_address                               : 17;
        uint32_t hash_lookup                                : 1;
        uint32_t direct_lookup                              : 1;
        uint32_t further_lookup_required                    : 1;
    };
    uint32_t value;
} mem_lookup_recursive_hash_table_t;

#endif




/** Do lookup
 *
 * @param xfer              Transfer registers containing index to lookup.
 * @param address           40 bit pointer with lower 32 bits setup as specfic command address for table,
 * - mem_lookup_dlut_small_t or mem_lookup_dlut_large_t for DLUT tables,
 * - mem_lookup_alut_t for ALUT tables,
 * - mem_lookup_recursive_hash_table_t for HASH tables.
 * @param count             Length in 64-bit words to read, (valid values 1 - 2).
 * @param sync              Type of synchronization to use (can only be sig_done).
 * @param sig_pair_ptr      Signal to raise upon completion.
 *
 *
 * Load descriptor data into the Local Descriptor Array. The array supports up to 64 Descriptors.
 * Descriptors must be loaded into the Local Descriptor Array prior to doing lookups.
 */
__intrinsic
mem_lookup_result_in_read_reg_t *cmd_mem_lookup(
    __xwrite void *xfer,
    volatile void __addr40 __mem *address,
    uint32_t count,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);


/**
 *  @}
 * @}
 */

#endif /* __NFP_MEM_LOOKUP_ENGINE_H__ */
