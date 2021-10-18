/*
 * Copyright (C) 2013 Netronome Systems, Inc.  All rights reserved.
 */

/* DRAM TCAM functions */
#ifndef __NFP_MEM_TCAM_H__
#define __NFP_MEM_TCAM_H__

#include <nfp.h>
#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file nfp_mem_tcam.h
 *  @addtogroup mu_tcam MU TCAM Intrinsics
 *  @{
 */

/** @name TCAM types and functions
 *  @{
 *   A TCAM entry number refers to both the value and the mask at the same index,
 *   while a word index refers simply to an N bit word at an offset in the TCAM
 *   which can be either an entry value or an entry mask. For example, a 128
 *   bit TCAM used with 8 bit access (sets and lookups) will have 8 entries, but
 *   16 words of 8 bits each.
 *
 *   Note that 24 bit TCAM lookups operate on 32 bit words, but
 *   compare only the lower 24 bits. Other TCAM lookups operate on word
 *   boundaries matching the lookup bit size.
 */

/** 128 bit TCAM container type. */
typedef struct mem_tcam128_t
{
    uint32_t value[4]; /**< Storage for the TCAM data. */
} mem_tcam128_t;

/** 256 bit TCAM container type. */
typedef struct mem_tcam256_t
{
    uint32_t value[8]; /**< Storage for the TCAM data. */
} mem_tcam256_t;

/** 384 bit TCAM container type. */
typedef struct mem_tcam384_t
{
    uint32_t value[12]; /**< Storage for the TCAM data. */
} mem_tcam384_t;

/** 512 bit TCAM container type. */
typedef struct mem_tcam512_t
{
    uint32_t value[16]; /**< Storage for the TCAM data. */
} mem_tcam512_t;

/* TCAM container type aligned in 32-bit addressed MEM. */
typedef __addr32 __align16 mem_tcam128_t mem_tcam128_in_mem_t;
typedef __addr32 __align32 mem_tcam256_t mem_tcam256_in_mem_t;
typedef __addr32 __align64 mem_tcam384_t mem_tcam384_in_mem_t;
typedef __addr32 __align64 mem_tcam512_t mem_tcam512_in_mem_t;

/* TCAM container pointer type aligned in 40-bit addressed MEM. */
typedef __addr40 __mem __align16 mem_tcam128_t *mem_tcam128_ptr40_t;
typedef __addr40 __mem __align32 mem_tcam256_t *mem_tcam256_ptr40_t;
typedef __addr40 __mem __align64 mem_tcam384_t *mem_tcam384_ptr40_t;
typedef __addr40 __mem __align64 mem_tcam512_t *mem_tcam512_ptr40_t;

/*
 * Lookup 8
 */
#ifdef __BIGENDIAN

/** Input type for 8 bit TCAM lookups.
 *
 * The search field of this structure needs to be populated before performing a TCAM lookup.
 */
typedef union mem_tcam_lookup8_in_t
{
    struct
    {
        uint32_t reserved1  :   24;     /**< Reserved */
        uint32_t search     :   8;      /**< 8 bit value to search for in the TCAM */

        uint32_t padding;               /**< Additional word to ensure read data is same size as write data */
    };
    uint32_t value[2];                  /**< Accessor to entire structure */
} mem_tcam_lookup8_in_t;

/** Output type of 8 bit TCAM lookups.
 *
 * The match bit fields indicate which TCAM entries matched the lookup.
 */
typedef union mem_tcam_lookup8_out_t
{
    struct
    {
        uint32_t reserved    : 24;      /**< Reserved */
        uint32_t first_match : 8;       /**< First matched 8 bit word index. 0xFF when no match was found */

        uint32_t match_bitf;            /**< 32 bit field with ones indicating that a corresponding entry matched */
    };
    uint32_t value[2];                  /**< Accessor to entire structure */
} mem_tcam_lookup8_out_t;

#else /* __LITTLEENDIAN */
typedef union mem_tcam_lookup8_in_t
{
    struct
    {
        uint32_t search     :   8;
        uint32_t reserved1  :   24;

        uint32_t padding;
    };
    uint32_t value[2];
} mem_tcam_lookup8_in_t;

typedef union mem_tcam_lookup8_out_t
{
    struct
    {
        uint32_t first_match            : 8;
        uint32_t reserved               : 24;

        uint32_t match_bitf;
    };
    uint32_t value[2];
} mem_tcam_lookup8_out_t;

#endif /* __BIGENDIAN */

/** Type for mem_tcam_lookup8_out_t in read registers. */
typedef __xread mem_tcam_lookup8_out_t         mem_tcam_lookup8_out_in_read_reg_t;

/*
 * lookup 16
 */
#ifdef __BIGENDIAN
/** Input type for 16 bit TCAM lookups.
 *
 * The search field of this structure needs to be populated before performing a TCAM lookup.
 */
typedef union mem_tcam_lookup16_in_t
{
    struct
    {
        uint32_t reserved1              : 16;   /**< Reserved */
        uint32_t search                 : 16;   /**< 16 bit value to search for in the TCAM. */
    };
    uint32_t value;                             /**< Accessor to entire structure */
} mem_tcam_lookup16_in_t;

/** Output type of 16 bit TCAM lookups.
 *
 * The match bit field indicates which TCAM entries matched the lookup.
 */
typedef union mem_tcam_lookup16_out_t
{
    struct
    {
        uint32_t match_bitf             : 16;   /**< 16 bit field with ones indicating that a corresponding entry matched. */
        uint32_t reserved               : 8;    /**< Reserved */
        uint32_t first_match            : 8;    /**< First matched 16 bit word index. Equal to 0xFF when no match was found */
    };
    uint32_t value;                             /**< Accessor to entire structure */
} mem_tcam_lookup16_out_t;

#else /* __LITTLEENDIAN */
typedef union mem_tcam_lookup16_in_t
{
    struct
    {
        uint32_t search                 : 16;
        uint32_t reserved1              : 16;
    };
    uint32_t value;
} mem_tcam_lookup16_in_t;

typedef union mem_tcam_lookup16_out_t
{
    struct
    {
        uint32_t first_match            : 8;
        uint32_t reserved               : 8;
        uint32_t match_bitf             : 16;
    };
    uint32_t value;
} mem_tcam_lookup16_out_t;

#endif /* __BIGENDIAN */

/** Type for mem_tcam_lookup16_out_t in read registers. */
typedef __xread mem_tcam_lookup16_out_t        mem_tcam_lookup16_out_in_read_reg_t;

/*
 * lookup 24
 */
#ifdef __BIGENDIAN
/** Input type for 24 bit TCAM lookups.
 *
 * The search field of this structure needs to be populated before performing a TCAM lookup.
 */
typedef union mem_tcam_lookup24_in_t
{
    struct
    {
        uint32_t reserved1              : 8;    /**< Reserved */
        uint32_t search                 : 24;   /**< 24 bit value to search for in the TCAM */
    };
    uint32_t value;                             /**< Accessor to entire structure */
} mem_tcam_lookup24_in_t;

/** Output type of 24 bit TCAM lookup and add operations.
 *
 * These match bit fields indicate which TCAM entries matched the lookup.
 */
typedef union mem_tcam_lookup24_out_t
{
    struct
    {
        uint32_t reserved               : 8;    /**< Reserved */
        uint32_t match_bitf             : 8;    /**< 8 bit field with ones indicating that a corresponding entry matched */
        uint32_t match_high_byte        : 8;    /**< Upper 8 bits of the matched TCAM entry is returned in this field */
        uint32_t first_match            : 8;    /**< First matched 32 bit word index. Equal to 0xFF when no match was found */
    };
    uint32_t value;
} mem_tcam_lookup24_out_t;

#else /* __LITTLEENDIAN */
typedef union mem_tcam_lookup24_in_t
{
    struct
    {
        uint32_t search                 : 24;
        uint32_t reserved1              : 8;
    };
    uint32_t value;
} mem_tcam_lookup24_in_t;

typedef union mem_tcam_lookup24_out_t
{
    struct
    {
        uint32_t first_match            : 8;   /* Word index of first match lookup - 0xff when not found */
        uint32_t match_high_byte        : 8;
        uint32_t match_bitf             : 8;   /* Bits are set on matching entries */
        uint32_t reserved               : 8;
    };
    uint32_t value;
} mem_tcam_lookup24_out_t;

#endif /* __BIGENDIAN */

/** Type for mem_tcam_lookup24_out_t in read registers. */
typedef __xread mem_tcam_lookup24_out_t        mem_tcam_lookup24_out_in_read_reg_t;

/*
 * lookup 32
 */
#ifdef __BIGENDIAN
/** Input type for 32 bit TCAM lookups.
 *
 * The search field of this structure needs to be populated before performing a TCAM lookup.
 */
typedef struct mem_tcam_lookup32_in_t
{
    uint32_t search;                            /**< 32 bit value to search for in the TCAM */
} mem_tcam_lookup32_in_t;

/** Output type of 32 bit TCAM lookup and add operations.
 *
 * The match bit field indicates which TCAM entries matched the lookup.
 */
typedef union mem_tcam_lookup32_out_t
{
    struct
    {
        uint32_t reserved               : 8;    /**< Reserved */
        uint32_t match_bitf             : 8;    /**< 8 bit field with ones indicating that a corresponding entry matched */
        uint32_t match_high_byte        : 8;    /**< Upper 8 bits of the matched TCAM entry is returned in this field */
        uint32_t first_match            : 8;    /**< First matched 32 bit word index. Equal to 0xFF when no match was found */
    };
    uint32_t value;
} mem_tcam_lookup32_out_t;

#else /* __LITTLEENDIAN */
typedef struct mem_tcam_lookup32_in_t
{
    uint32_t search;
} mem_tcam_lookup32_in_t;

typedef union mem_tcam_lookup32_out_t
{
    struct
    {
        uint32_t first_match            : 8;    /* Word index of first match lookup - 0xff when not found */
        uint32_t match_high_byte        : 8;
        uint32_t match_bitf             : 8;   /* Bits are set on matching entries */
        uint32_t reserved               : 8;
    };
    uint32_t value;
} mem_tcam_lookup32_out_t;

#endif /* __BIGENDIAN */

/** Type for mem_tcam_lookup32_out_t in read registers. */
typedef __xread mem_tcam_lookup32_out_t        mem_tcam_lookup32_out_in_read_reg_t;

/** Initialize a 128 bit TCAM in memory.
 *
 * All words are initialized to 0. This means a TCAM lookup on a newly
 * initialized TCAM will match all entries, because the masks are all 0.
 *
 * @param tcam   Pointer to the TCAM in MEM
 */
__intrinsic
void cmd_mem_tcam128_init_ptr32(
    __mem mem_tcam128_in_mem_t *tcam
);

/** Initialize a 128 bit TCAM in 40-bit addressed MEM.
 *
 * All words are initialized to 0. This means a TCAM lookup on a newly
 * initialized TCAM will match all entries, because the masks are all 0.
 *
 * @param tcam   Pointer to the TCAM in MEM
 */
__intrinsic
void cmd_mem_tcam128_init_ptr40(
    mem_tcam128_ptr40_t tcam
);

/** Initialize a 256 bit TCAM in memory.
 *
 * All words are initialized to 0. This means a TCAM lookup on a newly
 * initialized TCAM will match all entries, because the masks are all 0.
 *
 * @param tcam   Pointer to the TCAM in MEM
 */
__intrinsic
void cmd_mem_tcam256_init_ptr32(
    __mem mem_tcam256_in_mem_t *tcam
);

/** Initialize a 256 bit TCAM in 40-bit addressed MEM.
 *
 * All words are initialized to 0. This means a TCAM lookup on a newly
 * initialized TCAM will match all entries, because the masks are all 0.
 *
 * @param tcam   Pointer to the TCAM in MEM
 */
__intrinsic
void cmd_mem_tcam256_init_ptr40(
    mem_tcam256_ptr40_t tcam
);

/** Initialize a 384 bit TCAM in memory.
 *
 * All words are initialized to 0. This means a TCAM lookup on a newly
 * initialized TCAM will match all entries, because the masks are all 0.
 *
 * @param tcam   Pointer to the TCAM in MEM
 */
__intrinsic
void cmd_mem_tcam384_init_ptr32(
    __mem mem_tcam384_in_mem_t *tcam
);

/** Initialize a 384 bit TCAM in 40-bit addressed MEM.
 *
 * All words are initialized to 0. This means a TCAM lookup on a newly
 * initialized TCAM will match all entries, because the masks are all 0.
 *
 * @param tcam   Pointer to the TCAM in MEM
 */
__intrinsic
void cmd_mem_tcam384_init_ptr40(
    mem_tcam384_ptr40_t tcam
);

/** Initialize a 512 bit TCAM in memory.
 *
 * All words are initialized to 0. This means a TCAM lookup on a newly
 * initialized TCAM will match all entries, because the masks are all 0.
 *
 * @param tcam   Pointer to the TCAM in MEM
 */
__intrinsic
void cmd_mem_tcam512_init_ptr32(
    __mem mem_tcam512_in_mem_t *tcam
);

/** Initialize a 512 bit TCAM in 40-bit addressed MEM.
 *
 * All words are initialized to 0. This means a TCAM lookup on a newly
 * initialized TCAM will match all entries, because the masks are all 0.
 *
 * @param tcam   Pointer to the TCAM in MEM
 */
__intrinsic
void cmd_mem_tcam512_init_ptr40(
    mem_tcam512_ptr40_t tcam
);

/** Set an 8 bit word in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 3 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam128_set8_word_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit word in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 3 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam128_set8_word_ptr40(
    mem_tcam128_ptr40_t tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam128_set8_entry_value_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam128_set8_entry_value_ptr40(
    mem_tcam128_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry mask in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam128_set8_entry_mask_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry mask in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam128_set8_entry_mask_ptr40(
    mem_tcam128_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value and mask in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam128_set8_entry_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value and mask in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam128_set8_entry_ptr40(
    mem_tcam128_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit word in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 3 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 *
 */
__intrinsic
void mem_tcam256_set8_word_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit word in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 3 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 *
 */
__intrinsic
void mem_tcam256_set8_word_ptr40(
    mem_tcam256_ptr40_t tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam256_set8_entry_value_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam256_set8_entry_value_ptr40(
    mem_tcam256_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry mask in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam256_set8_entry_mask_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry mask in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam256_set8_entry_mask_ptr40(
    mem_tcam256_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value and mask in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam256_set8_entry_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value and mask in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam256_set8_entry_ptr40(
    mem_tcam256_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit word in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 3 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 47.
 *
 *
 */
__intrinsic
void mem_tcam384_set8_word_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit word in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 3 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 47.
 *
 *
 */
__intrinsic
void mem_tcam384_set8_word_ptr40(
    mem_tcam384_ptr40_t tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 23.
 *
 *
 */
__intrinsic
void mem_tcam384_set8_entry_value_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 23.
 *
 *
 */
__intrinsic
void mem_tcam384_set8_entry_value_ptr40(
    mem_tcam384_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry mask in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 23.
 *
 *
 */
__intrinsic
void mem_tcam384_set8_entry_mask_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry mask in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 23.
 *
 *
 */
__intrinsic
void mem_tcam384_set8_entry_mask_ptr40(
    mem_tcam384_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value and mask in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 23.
 *
 *
 */
__intrinsic
void mem_tcam384_set8_entry_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value and mask in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 23.
 *
 *
 */
__intrinsic
void mem_tcam384_set8_entry_ptr40(
    mem_tcam384_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit word in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 3 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 63.
 *
 *
 */
__intrinsic
void mem_tcam512_set8_word_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit word in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 3 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 63.
 *
 *
 */
__intrinsic
void mem_tcam512_set8_word_ptr40(
    mem_tcam512_ptr40_t tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 *
 */
__intrinsic
void mem_tcam512_set8_entry_value_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 *
 */
__intrinsic
void mem_tcam512_set8_entry_value_ptr40(
    mem_tcam512_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry mask in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 *
 */
__intrinsic
void mem_tcam512_set8_entry_mask_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry mask in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 *
 */
__intrinsic
void mem_tcam512_set8_entry_mask_ptr40(
    mem_tcam512_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value and mask in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 *
 */
__intrinsic
void mem_tcam512_set8_entry_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set an 8 bit entry value and mask in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 *
 */
__intrinsic
void mem_tcam512_set8_entry_ptr40(
    mem_tcam512_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit word in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 2 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam128_set16_word_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit word in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 2 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam128_set16_word_ptr40(
    mem_tcam128_ptr40_t tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam128_set16_entry_value_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam128_set16_entry_value_ptr40(
    mem_tcam128_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry mask in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam128_set16_entry_mask_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry mask in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam128_set16_entry_mask_ptr40(
    mem_tcam128_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value and mask in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam128_set16_entry_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value and mask in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam128_set16_entry_ptr40(
    mem_tcam128_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit word in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 2 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam256_set16_word_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit word in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 2 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam256_set16_word_ptr40(
    mem_tcam256_ptr40_t tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam256_set16_entry_value_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam256_set16_entry_value_ptr40(
    mem_tcam256_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry mask in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam256_set16_entry_mask_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry mask in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam256_set16_entry_mask_ptr40(
    mem_tcam256_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value and mask in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam256_set16_entry_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value and mask in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam256_set16_entry_ptr40(
    mem_tcam256_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit word in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 2 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 23.
 *
 *
 */
__intrinsic
void mem_tcam384_set16_word_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit word in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 2 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 23.
 *
 *
 */
__intrinsic
void mem_tcam384_set16_word_ptr40(
    mem_tcam384_ptr40_t tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 *
 */
__intrinsic
void mem_tcam384_set16_entry_value_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 *
 */
__intrinsic
void mem_tcam384_set16_entry_value_ptr40(
    mem_tcam384_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry mask in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 *
 */
__intrinsic
void mem_tcam384_set16_entry_mask_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry mask in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 *
 */
__intrinsic
void mem_tcam384_set16_entry_mask_ptr40(
    mem_tcam384_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value and mask in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 *
 */
__intrinsic
void mem_tcam384_set16_entry_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value and mask in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 *
 */
__intrinsic
void mem_tcam384_set16_entry_ptr40(
    mem_tcam384_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit word in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 2 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 *
 */
__intrinsic
void mem_tcam512_set16_word_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit word in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 2 is
 *                          the first entry mask, 1 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 *
 */
__intrinsic
void mem_tcam512_set16_word_ptr40(
    mem_tcam512_ptr40_t tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam512_set16_entry_value_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam512_set16_entry_value_ptr40(
    mem_tcam512_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry mask in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam512_set16_entry_mask_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry mask in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam512_set16_entry_mask_ptr40(
    mem_tcam512_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value and mask in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam512_set16_entry_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16 bit entry value and mask in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry, 2 is the third entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam512_set16_entry_ptr40(
    mem_tcam512_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Alias for @ref mem_tcam128_set32_word_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam128_set24_word_ptr32            mem_tcam128_set32_word_ptr32

/** Alias for @ref mem_tcam128_set32_entry_value_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam128_set24_entry_value_ptr32     mem_tcam128_set32_entry_value_ptr32
#define mem_tcam128_set24_entry_value_ptr40     mem_tcam128_set32_entry_value_ptr40

/** Alias for @ref mem_tcam128_set32_entry_mask_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam128_set24_entry_mask_ptr32    mem_tcam128_set32_entry_mask_ptr32
#define mem_tcam128_set24_entry_mask_ptr40      mem_tcam128_set32_entry_mask_ptr40

/** Alias for @ref mem_tcam128_set32_entry_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam128_set24_entry_ptr32       mem_tcam128_set32_entry_ptr32
#define mem_tcam128_set24_entry_ptr40       mem_tcam128_set32_entry_ptr40

/** Alias for @ref mem_tcam256_set32_word_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam256_set24_word_ptr32        mem_tcam256_set32_word_ptr32
#define mem_tcam256_set24_word_ptr40        mem_tcam256_set32_word_ptr40

/** Alias for @ref mem_tcam256_set32_entry_value_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam256_set24_entry_value_ptr32     mem_tcam256_set32_entry_value_ptr32
#define mem_tcam256_set24_entry_value_ptr40     mem_tcam256_set32_entry_value_ptr40

/** Alias for @ref mem_tcam256_set32_entry_mask_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam256_set24_entry_mask_ptr32  mem_tcam256_set32_entry_mask_ptr32
#define mem_tcam256_set24_entry_mask_ptr40  mem_tcam256_set32_entry_mask_ptr40

/** Alias for @ref mem_tcam256_set32_entry_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam256_set24_entry_ptr32    mem_tcam256_set32_entry_ptr32
#define mem_tcam256_set24_entry_ptr40   mem_tcam256_set32_entry_ptr40

/** Alias for @ref mem_tcam384_set32_word_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam384_set24_word_ptr32    mem_tcam384_set32_word_ptr32
#define mem_tcam384_set24_word_ptr40    mem_tcam384_set32_word_ptr40

/** Alias for @ref mem_tcam384_set32_entry_value_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam384_set24_entry_value_ptr32     mem_tcam384_set32_entry_value_ptr32
#define mem_tcam384_set24_entry_value_ptr40     mem_tcam384_set32_entry_value_ptr40

/** Alias for @ref mem_tcam384_set32_entry_mask_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam384_set24_entry_mask_ptr32  mem_tcam384_set32_entry_mask_ptr32
#define mem_tcam384_set24_entry_mask_ptr40  mem_tcam384_set32_entry_mask_ptr40

/** Alias for @ref mem_tcam384_set32_entry_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam384_set24_entry_ptr32   mem_tcam384_set32_entry_ptr32
#define mem_tcam384_set24_entry_ptr40   mem_tcam384_set32_entry_ptr40

/** Alias for @ref mem_tcam512_set32_word_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam512_set24_word_ptr32    mem_tcam512_set32_word_ptr32
#define mem_tcam512_set24_word_ptr40    mem_tcam512_set32_word_ptr40

/** Alias for @ref mem_tcam512_set32_entry_value_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam512_set24_entry_value_ptr32     mem_tcam512_set32_entry_value_ptr32
#define mem_tcam512_set24_entry_value_ptr40     mem_tcam512_set32_entry_value_ptr40

/** Alias for @ref mem_tcam512_set32_entry_mask_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam512_set24_entry_mask_ptr32  mem_tcam512_set32_entry_mask_ptr32
#define mem_tcam512_set24_entry_mask_ptr40  mem_tcam512_set32_entry_mask_ptr40

/** Alias for @ref mem_tcam512_set32_entry_ptr32.
 *
 * @note
 * A 24 bit word TCAM is the same as a 32 bit word TCAM.
 * The difference is seen when lookups are performed.
 * For a 24 bit word TCAM, the full 32 bit word can be set (the highest 8 bits
 * are not masked off). When a 24 bit lookup is performed, the highest
 * 8 bits are @b not compared, but will be
 * returned for the first match.
 */
#define mem_tcam512_set24_entry_ptr32   mem_tcam512_set32_entry_ptr32
#define mem_tcam512_set24_entry_ptr40   mem_tcam512_set32_entry_ptr40

/** Set a 32 bit word in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 1 is
 *                          the first entry mask, 2 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam128_set32_word_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit word in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 1 is
 *                          the first entry mask, 2 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam128_set32_word_ptr40(
    mem_tcam128_ptr40_t tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 1.
 *
 *
 */
__intrinsic
void mem_tcam128_set32_entry_value_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 1.
 *
 *
 */
__intrinsic
void mem_tcam128_set32_entry_value_ptr40(
    mem_tcam128_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry mask in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 1.
 *
 *
 */
__intrinsic
void mem_tcam128_set32_entry_mask_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry mask in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 1.
 *
 *
 */
__intrinsic
void mem_tcam128_set32_entry_mask_ptr40(
    mem_tcam128_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value and mask in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 1.
 *
 *
 */
__intrinsic
void mem_tcam128_set32_entry_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value and mask in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 1.
 *
 *
 */
__intrinsic
void mem_tcam128_set32_entry_ptr40(
    mem_tcam128_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit word in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 1 is
 *                          the first entry mask, 2 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam256_set32_word_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit word in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 1 is
 *                          the first entry mask, 2 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam256_set32_word_ptr40(
    mem_tcam256_ptr40_t tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam256_set32_entry_value_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam256_set32_entry_value_ptr40(
    mem_tcam256_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry mask in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam256_set32_entry_mask_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry mask in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam256_set32_entry_mask_ptr40(
    mem_tcam256_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value and mask in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam256_set32_entry_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value and mask in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 *
 */
__intrinsic
void mem_tcam256_set32_entry_ptr40(
    mem_tcam256_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit word in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 1 is
 *                          the first entry mask, 2 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 *
 */
__intrinsic
void mem_tcam384_set32_word_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit word in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 1 is
 *                          the first entry mask, 2 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 *
 */
__intrinsic
void mem_tcam384_set32_word_ptr40(
    mem_tcam384_ptr40_t tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);


/** Set a 32 bit entry value in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 5.
 *
 *
 */
__intrinsic
void mem_tcam384_set32_entry_value_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 5.
 *
 *
 */
__intrinsic
void mem_tcam384_set32_entry_value_ptr40(
    mem_tcam384_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry mask in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 5.
 *
 *
 */
__intrinsic
void mem_tcam384_set32_entry_mask_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry mask in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 5.
 *
 *
 */
__intrinsic
void mem_tcam384_set32_entry_mask_ptr40(
    mem_tcam384_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value and mask in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 5.
 *
 *
 */
__intrinsic
void mem_tcam384_set32_entry_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value and mask in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 5.
 *
 *
 */
__intrinsic
void mem_tcam384_set32_entry_ptr40(
    mem_tcam384_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit word in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 1 is
 *                          the first entry mask, 2 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam512_set32_word_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit word in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param index             TCAM word to set (0 is the first entry value, 1 is
 *                          the first entry mask, 2 is the second entry value)
 * @param value             Value to set the TCAM word to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 *
 */
__intrinsic
void mem_tcam512_set32_word_ptr40(
    mem_tcam512_ptr40_t tcam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam512_set32_entry_value_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam512_set32_entry_value_ptr40(
    mem_tcam512_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry mask in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam512_set32_entry_mask_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry mask in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam512_set32_entry_mask_ptr40(
    mem_tcam512_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value and mask in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam512_set32_entry_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32 bit entry value and mask in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param entry_index       Number of TCAM entry to set (0 is the first entry,
 *                          1 is the second entry)
 * @param value             Value to set the TCAM entry value to
 * @param mask              Value to set the TCAM entry mask to
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 *
 */
__intrinsic
void mem_tcam512_set32_entry_ptr40(
    mem_tcam512_ptr40_t tcam,
    uint32_t entry_index,
    uint32_t value,
    uint32_t mask,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Perform an 8 bit lookup in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_tcam_lookup8_in_t, mem_tcam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup8_out_in_read_reg_t *mem_tcam128_lookup8_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    __xwrite mem_tcam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform an 8 bit lookup in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_tcam_lookup8_in_t, mem_tcam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup8_out_in_read_reg_t *mem_tcam128_lookup8_ptr40(
    mem_tcam128_ptr40_t tcam,
    __xwrite mem_tcam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform an 8 bit lookup in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_tcam_lookup8_in_t, mem_tcam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup8_out_in_read_reg_t *mem_tcam256_lookup8_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    __xwrite mem_tcam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform an 8 bit lookup in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_tcam_lookup8_in_t, mem_tcam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup8_out_in_read_reg_t *mem_tcam256_lookup8_ptr40(
    mem_tcam256_ptr40_t tcam,
    __xwrite mem_tcam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform an 8 bit lookup in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_tcam_lookup8_in_t, mem_tcam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup8_out_in_read_reg_t *mem_tcam384_lookup8_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    __xwrite mem_tcam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform an 8 bit lookup in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_tcam_lookup8_in_t, mem_tcam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup8_out_in_read_reg_t *mem_tcam384_lookup8_ptr40(
    mem_tcam384_ptr40_t tcam,
    __xwrite mem_tcam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform an 8 bit lookup in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_tcam_lookup8_in_t, mem_tcam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup8_out_in_read_reg_t *mem_tcam512_lookup8_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    __xwrite mem_tcam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform an 8 bit lookup in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_tcam_lookup8_in_t, mem_tcam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup8_out_in_read_reg_t *mem_tcam512_lookup8_ptr40(
    mem_tcam512_ptr40_t tcam,
    __xwrite mem_tcam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16 bit lookup in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup16_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup16_in_t, mem_tcam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup16_out_in_read_reg_t *mem_tcam128_lookup16_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    __xwrite mem_tcam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16 bit lookup in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup16_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup16_in_t, mem_tcam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup16_out_in_read_reg_t *mem_tcam128_lookup16_ptr40(
    mem_tcam128_ptr40_t tcam,
    __xwrite mem_tcam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16 bit lookup in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup16_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup16_in_t, mem_tcam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup16_out_in_read_reg_t *mem_tcam256_lookup16_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    __xwrite mem_tcam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16 bit lookup in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup16_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup16_in_t, mem_tcam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup16_out_in_read_reg_t *mem_tcam256_lookup16_ptr40(
    mem_tcam256_ptr40_t tcam,
    __xwrite mem_tcam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16 bit lookup in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup16_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup16_in_t, mem_tcam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup16_out_in_read_reg_t *mem_tcam384_lookup16_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    __xwrite mem_tcam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16 bit lookup in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup16_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup16_in_t, mem_tcam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup16_out_in_read_reg_t *mem_tcam384_lookup16_ptr40(
    mem_tcam384_ptr40_t tcam,
    __xwrite mem_tcam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16 bit lookup in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup16_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup16_in_t, mem_tcam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup16_out_in_read_reg_t *mem_tcam512_lookup16_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    __xwrite mem_tcam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16 bit lookup in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup16_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup16_in_t, mem_tcam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup16_out_in_read_reg_t *mem_tcam512_lookup16_ptr40(
    mem_tcam512_ptr40_t tcam,
    __xwrite mem_tcam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24 bit lookup in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup24_in_t, mem_tcam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup24_out_in_read_reg_t *mem_tcam128_lookup24_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    __xwrite mem_tcam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24 bit lookup in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup24_in_t, mem_tcam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup24_out_in_read_reg_t *mem_tcam128_lookup24_ptr40(
    mem_tcam128_ptr40_t tcam,
    __xwrite mem_tcam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24 bit lookup in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup24_in_t, mem_tcam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup24_out_in_read_reg_t *mem_tcam256_lookup24_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    __xwrite mem_tcam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24 bit lookup in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup24_in_t, mem_tcam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup24_out_in_read_reg_t *mem_tcam256_lookup24_ptr40(
    mem_tcam256_ptr40_t tcam,
    __xwrite mem_tcam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24 bit lookup in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup24_in_t, mem_tcam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup24_out_in_read_reg_t *mem_tcam384_lookup24_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    __xwrite mem_tcam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24 bit lookup in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup24_in_t, mem_tcam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup24_out_in_read_reg_t *mem_tcam384_lookup24_ptr40(
    mem_tcam384_ptr40_t tcam,
    __xwrite mem_tcam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24 bit lookup in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup24_in_t, mem_tcam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup24_out_in_read_reg_t *mem_tcam512_lookup24_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    __xwrite mem_tcam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24 bit lookup in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup24_in_t, mem_tcam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup24_out_in_read_reg_t *mem_tcam512_lookup24_ptr40(
    mem_tcam512_ptr40_t tcam,
    __xwrite mem_tcam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32 bit lookup in a 128 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup32_in_t, mem_tcam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup32_out_in_read_reg_t *mem_tcam128_lookup32_ptr32(
    __mem mem_tcam128_in_mem_t *tcam,
    __xwrite mem_tcam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32 bit lookup in a 128 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup32_in_t, mem_tcam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup32_out_in_read_reg_t *mem_tcam128_lookup32_ptr40(
    mem_tcam128_ptr40_t tcam,
    __xwrite mem_tcam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32 bit lookup in a 256 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup32_in_t, mem_tcam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup32_out_in_read_reg_t *mem_tcam256_lookup32_ptr32(
    __mem mem_tcam256_in_mem_t *tcam,
    __xwrite mem_tcam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32 bit lookup in a 256 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup32_in_t, mem_tcam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup32_out_in_read_reg_t *mem_tcam256_lookup32_ptr40(
    mem_tcam256_ptr40_t tcam,
    __xwrite mem_tcam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32 bit lookup in a 384 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup32_in_t, mem_tcam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup32_out_in_read_reg_t *mem_tcam384_lookup32_ptr32(
    __mem mem_tcam384_in_mem_t *tcam,
    __xwrite mem_tcam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32 bit lookup in a 384 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup32_in_t, mem_tcam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup32_out_in_read_reg_t *mem_tcam384_lookup32_ptr40(
    mem_tcam384_ptr40_t tcam,
    __xwrite mem_tcam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32 bit lookup in a 512 bit TCAM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup32_in_t, mem_tcam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup32_out_in_read_reg_t *mem_tcam512_lookup32_ptr32(
    __mem mem_tcam512_in_mem_t *tcam,
    __xwrite mem_tcam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32 bit lookup in a 512 bit TCAM in 40-bit addressed MEM.
 *
 * @param tcam              Pointer to the TCAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_tcam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_tcam_lookup32_in_t, mem_tcam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_tcam_lookup32_out_in_read_reg_t *mem_tcam512_lookup32_ptr40(
    mem_tcam512_ptr40_t tcam,
    __xwrite mem_tcam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Convert a TCAM 8 bit word index to an entry index.
 *
 * @param word_idx          A word index, as typically returned in a lookup result's first_match field
 * @return                  An entry index which can be used with other entry indexed TCAM functions
 *
 * @note                    This function does not perform range checking.
 * @see                     mem_tcam_lookup8_out_t
 */
__intrinsic
uint32_t mem_tcam_word_to_entry_index8(uint32_t word_idx);

/** Convert a TCAM 8 bit entry value index to a word index.
 *
 * @param entry_idx         A entry index
 * @return                  A word index to the word containing the value of entry number entry_idx.
 *
 * @note                    This function does not perform range checking.
 */
__intrinsic
uint32_t mem_tcam_entry_value_to_word_index8(uint32_t entry_idx);

/** Convert a TCAM 8 bit entry mask index to a word index.
 *
 * @param entry_idx         A entry index
 * @return                  A word index to the word containing the mask of entry number entry_idx.
 *
 * @note                    This function does not perform range checking.
 */
__intrinsic
uint32_t mem_tcam_entry_mask_to_word_index8(uint32_t entry_idx);


/** Convert a TCAM 16 bit word index to an entry index.
 *
 * @param word_idx          A word index, as typically returned in a lookup result's first_match field
 * @return                  An entry index which can be used with other entry indexed TCAM functions
 *
 * @note                    This function does not perform range checking.
 * @see                     mem_tcam_lookup16_out_t
 */
__intrinsic
uint32_t mem_tcam_word_to_entry_index16(uint32_t word_idx);

/** Convert a TCAM 16 bit entry value index to a word index.
 *
 * @param entry_idx         A entry index
 * @return                  A word index to the word containing the value of entry number entry_idx.
 *
 * @note                    This function does not perform range checking.
 */
__intrinsic
uint32_t mem_tcam_entry_value_to_word_index16(uint32_t entry_idx);

/** Convert a TCAM 16 bit entry mask index to a word index.
 *
 * @param entry_idx         A entry index
 * @return                  A word index to the word containing the mask of entry number entry_idx.
 *
 * @note                    This function does not perform range checking.
 */
__intrinsic
uint32_t mem_tcam_entry_mask_to_word_index16(uint32_t entry_idx);


/** Alias for @ref mem_tcam_word_to_entry_index32
 *
 * @see mem_tcam128_set24_word_ptr32
 */
 #define mem_tcam_word_to_entry_index24 mem_tcam_word_to_entry_index32

/** Alias for @ref mem_tcam_entry_value_to_word_index32
 *
 * @see mem_tcam128_set24_word_ptr32
 */
#define mem_tcam_entry_value_to_word_index24 mem_tcam_entry_value_to_word_index32

/** Alias for @ref mem_tcam_entry_mask_to_word_index32
 *
 * @see mem_tcam128_set24_word_ptr32
 */
#define mem_tcam_entry_mask_to_word_index24 mem_tcam_entry_mask_to_word_index32


/** Convert a TCAM 32 bit word index to an entry index.
 *
 * @param word_idx          A word index, as typically returned in a lookup result's first_match field
 * @return                  An entry index which can be used with other entry indexed TCAM functions
 *
 * @note                    This function does not perform range checking.
 * @see                     mem_tcam_lookup32_out_t
 */
__intrinsic
uint32_t mem_tcam_word_to_entry_index32(uint32_t word_idx);

/** Convert a TCAM 32 bit entry value index to a word index.
 *
 * @param entry_idx         A entry index
 * @return                  A word index to the word containing the value of entry number entry_idx.
 *
 * @note                    This function does not perform range checking.
 */
__intrinsic
uint32_t mem_tcam_entry_value_to_word_index32(uint32_t entry_idx);

/** Convert a TCAM 32 bit entry mask index to a word index.
 *
 * @param entry_idx         A entry index
 * @return                  A word index to the word containing the mask of entry number entry_idx.
 *
 * @note                    This function does not perform range checking.
 */
__intrinsic
uint32_t mem_tcam_entry_mask_to_word_index32(uint32_t entry_idx);

/** @}
 * @}
 */

#endif /* __NFP_MEM_TCAM_H__ */
