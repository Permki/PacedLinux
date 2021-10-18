/*
 * Copyright (C) 2014,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          lib/dcfl/an.h
 * @brief         Aggregation network 'crossproduct'.
 *
 */
#ifndef _DCFL__AN_H_
#define _DCFL__AN_H_

#if defined(__NFP_LANG_MICROC)
#include <nfp.h>
#include <nfp/me.h>
#include <nfp6000/nfp_me.h>
#include <nfp/mem_bulk.h>
#include <stdint.h>
#include <types.h>
#endif

#define DCFL_CONFIG_AN_LW     8
#if defined(__NFP_LANG_MICROC)
struct dcfl_config_an {
    union {
        struct {
            uint32_t base[DCFL_CONFIG_AN_LW];
        };
        uint32_t values[DCFL_CONFIG_AN_LW];
    };
};
#else
/* placeholder for assembler bitfield definitions */
#endif

/**
 * Aggregation network (number of nodes per block = m):
 * Each offset of each stage must start in a new block.
 *
 *      3                   2                   1
 *    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 0 | label1                        | metalabel1 (relative pointer) |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 1 | label2                        | metalabel2 (relative pointer) |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *m-1| pointer to extra entries                                      |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * n | labeln                        | metalabeln (relative pointer) |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   etc..
 */

/* Value to test whether priority is uninitialised.
 * No rule should have this priority */
#define DCFL_MAGIC_PRIORITY         ((1<<16) - 1)

#define DCFL_AN_ENTRY_LW            1
#if defined(__NFP_LANG_MICROC)
struct dcfl_an_entry {
    union {
        struct {
            uint32_t label:16;          /* DCFL label  */
            uint32_t mlabel:16;         /* DCFL meta-label */
        };
        struct {
            uint32_t prio_even:16; /* Priority for rules with even AN offset */
            uint32_t prio_odd:16;  /* Priority for rules with odd AN offset */
        };
        uint32_t values[DCFL_AN_ENTRY_LW];
    };
};
#else
#define DCFL_LABEL_IN_AN_bf         0, 31, 16
#define DCFL_METALABEL_IN_AN_bf     0, 15, 0
#define DCFL_PRIORITY_ODD_bf        0, 15, 0
#define DCFL_PRIORITY_EVEN_bf       0, 31, 16
#endif /* defined(__NFP_LANG_MICROC) */

#if defined(__NFP_LANG_MICROC)

/** Algorithmic lookup table sizes in cache lines where 16 bytes (128-bit) per cache line */
typedef enum MEM_LOOKUP_ALUT_SIZE
{
    LOOKUP_ALUT_SIZE_1          = 0x00,      /**< 1 128-bit cache line.  */
    LOOKUP_ALUT_SIZE_2          = 0x01,      /**< 2 128-bit cache lines. */
    LOOKUP_ALUT_SIZE_3          = 0x02,      /**< 3 128-bit cache lines. */
    LOOKUP_ALUT_SIZE_4          = 0x03       /**< 4 128-bit cache lines. */
}  MEM_LOOKUP_ALUT_SIZE;

/** Command for algorithmic lookup table */
typedef union mem_lookup_alut_t
{
    struct
    {
        uint32_t internal_memory            : 1;    /**< If internal memory is used. */
        uint32_t direct_lookup              : 1;    /**< 0 = not a direct lookup. */
        uint32_t lookup_type                : 1;    /**< 0 = algorithmic lookup. */
        MEM_LOOKUP_ALUT_SIZE table_size     : 2;    /**< Specified table size. */
        uint32_t table_number               : 3;    /**< Table number 0-7 (algorithmic table location CSR)
                                                        address bits 32:28 and IMEM or EMEM select. */
        uint32_t address_bits               : 24;   /**< Address bits 27:4. */
    };
    uint32_t value;                                 /**< Accessor to entire descriptor structure. */
} mem_lookup_alut_t;

typedef struct  mem_lookup_result_t
{
    uint32_t result;
} mem_lookup_result_t;

typedef __xread mem_lookup_result_t  mem_lookup_result_in_read_reg_t;

/**
 * Perform a DCFL Aggregation Network lookup
 *
 *
 */
int dcfl_an_lkp(__lmem struct dcfl_label_set *labels_in1,
                        uint32_t nlabel1,
                        __lmem struct dcfl_label_set *labels_in2,
                        uint32_t nlabel2,
                        __lmem struct dcfl_label_set *labels_out,
                        uint32_t stage_base, uint32_t stage_base_hi,
                        uint32_t alut_table
           );
/**
 * Perform a DCFL Aggregation Network 'crossproduct
 *
 *
 */
int dcfl_an_final(__lmem struct dcfl_label_set *labels_in1,
                        uint32_t nlabel1,
                        __lmem struct dcfl_label_set *labels_in2,
                        uint32_t nlabel2,
                        __lmem struct dcfl_label_set *labels_out,
                        uint32_t stage_base, uint32_t stage_base_hi
           );

#endif /* defined(__NFP_LANG_MICROC) */

#endif /* !_DCFL__AN_H_ */
