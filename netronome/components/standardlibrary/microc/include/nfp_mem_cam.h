/*
 * Copyright (C) 2008-2014Netronome Systems, Inc.  All rights reserved.
 */

/* DRAM CAM functions */

#ifndef __NFP_MEM_CAM_H__
#define __NFP_MEM_CAM_H__

#include <nfp.h>
#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif


/** @file nfp_mem_cam.h
 *  @addtogroup mu_cam MU CAM Intrinsics
 *  @{
 *
 *  @name CAM functions for Memory Unit CAM
 *  @{
 *
 * 8/16/24/32-bit CAM lookups can be done in 128/256/384 or 512-bit CAMs.
 */


/** 128-bit CAM container type. */
typedef struct mem_cam128_t
{
    uint32_t value[4]; /**< Storage for the CAM data. */
} mem_cam128_t;

/** 256-bit CAM container type. */
typedef struct mem_cam256_t
{
    uint32_t value[8]; /**< Storage for the CAM data. */
} mem_cam256_t;

/** 384-bit CAM container type. */
typedef struct mem_cam384_t
{
    uint32_t value[12]; /**< Storage for the CAM data. */
} mem_cam384_t;

/** 512-bit CAM container type. */
typedef struct mem_cam512_t
{
    uint32_t value[16]; /**< Storage for the CAM data. */
} mem_cam512_t;


/* CAM container type aligned in 32-bit addressed MEM. */
typedef __addr32 __align16 mem_cam128_t mem_cam128_in_mem_t;
typedef __addr32 __align32 mem_cam256_t mem_cam256_in_mem_t;
typedef __addr32 __align64 mem_cam384_t mem_cam384_in_mem_t;
typedef __addr32 __align64 mem_cam512_t mem_cam512_in_mem_t;

/* CAM container pointer type aligned in 40-bit addressed MEM. */
/* 40-bit pointer to CAM container */
typedef __addr40 __mem __align16 mem_cam128_t *mem_cam128_ptr40_t;
typedef __addr40 __mem __align32 mem_cam256_t *mem_cam256_ptr40_t;
typedef __addr40 __mem __align64 mem_cam384_t *mem_cam384_ptr40_t;
typedef __addr40 __mem __align64 mem_cam512_t *mem_cam512_ptr40_t;


#ifdef __BIGENDIAN
/** Input type for 8-bit CAM lookups.
 *
 * This search field of this structure needs to be populated before performing a CAM lookup.
 */
typedef union mem_cam_lookup8_in_t
{
    struct
    {
        uint32_t reserved1              : 24;   /**< Reserved */
        uint32_t search                 : 8;    /**< 8-bit value to search for in the CAM */
    };
    uint32_t value[2];                          /**< Accessor to entire structure */
} mem_cam_lookup8_in_t;


/** Output type of 8-bit CAM lookups.
 *
 * The match-bit fields indicate which CAM entries matched the lookup.
 */
typedef union mem_cam_lookup8_out_t
{
    struct
    {
        uint32_t match_bitf_low;                /**< Lower 32-bits of 64-bit field with ones indicating that a corresponding entry matched */
        uint32_t match_bitf_high;               /**< Upper 32-bits of 64-bit field with ones indicating that a corresponding entry matched */
    };
    uint32_t value[2];                          /**< Accessor to entire structure */
} mem_cam_lookup8_out_t;


/** Output type of 8-bit CAM lookup and add operations.
 *
 * The match-bit fields indicate which CAM entries matched the lookup.
 */
typedef union mem_cam_lookup8_add_out_t
{
    struct
    {
        uint32_t reserved               : 24;   /**< Reserved */
        uint32_t added                  : 1;    /**< When set, this field indicates that an entry was added.
                                                     *   @note This field is also set on a CAM miss when it is full.
                                                     */
        uint32_t first_match            : 7;    /**< First matched entry number, or on a CAM miss where the entry was added.
                                                     *   @note All-bits in this field are set on a CAM miss when it is full.
                                                     */

        uint32_t match_bitf;                    /**< Bitfield of matching entries */
    };
    uint32_t value[2];                          /**< Accessor to entire structure */
} mem_cam_lookup8_add_out_t;

#else /* __LITTLEENDIAN */

/* See BE description above */
typedef union mem_cam_lookup8_in_t
{
    struct
    {
        uint32_t search                 : 8;
        uint32_t reserved1              : 24;

        uint32_t padding;
    };
    uint32_t value[2];
} mem_cam_lookup8_in_t;

typedef union mem_cam_lookup8_out_t
{
    struct
    {
        uint32_t match_bitf_low;
        uint32_t match_bitf_high;
    };
    uint32_t value[2];
} mem_cam_lookup8_out_t;

typedef union mem_cam_lookup8_add_out_t
{
    struct
    {
        uint32_t first_match            : 7;
        uint32_t added                  : 1;
        uint32_t reserved               : 24;

        uint32_t match_bitf;
    };
    uint32_t value[2];
} mem_cam_lookup8_add_out_t;
#endif /* __BIGENDIAN */

/** Type for mem_cam_lookup8_out_t in read registers. */
typedef __xread mem_cam_lookup8_out_t         mem_cam_lookup8_out_in_read_reg_t;

/** Type for mem_cam_lookup8_add_out_t in read registers. */
typedef __xread mem_cam_lookup8_add_out_t     mem_cam_lookup8_add_out_in_read_reg_t;


#ifdef __BIGENDIAN
/** Input type for 16-bit CAM lookups.
 *
 * This search field of this structure needs to be populated before performing a CAM lookup.
 */
typedef union mem_cam_lookup16_in_t
{
    struct
    {
        uint32_t reserved1              : 16;   /**< Reserved */
        uint32_t search                 : 16;   /**< 16-bit value to search for in the CAM */

        uint32_t padding;                       /**< Additional word to ensure read data is same size as write data */
    };
    uint32_t value[2];                          /**< Accessor to entire structure */
} mem_cam_lookup16_in_t;

/** Output type of 16-bit CAM lookups.
 *
 * The match-bit fields indicate which CAM entries matched the lookup.
 */
typedef union mem_cam_lookup16_out_t
{
    struct
    {
        uint32_t reserved               : 24;   /**< Reserved */
        uint32_t first_match            : 8;    /**< 0xff - when not found */

        uint32_t match_bitf;                    /**< Bitfield of matching entries */
    };
    uint32_t value[2];                          /**< Accessor to entire structure */
} mem_cam_lookup16_out_t;

/** Output type of 16-bit CAM lookup and add operations.
 *
 * The match-bit fields indicate which CAM entries matched the lookup.
 */
typedef union mem_cam_lookup16_add_out_t
{
    struct
    {
        uint32_t reserved               : 24;   /**< Reserved */
        uint32_t added                  : 1;    /**< When set, this field indicates that an entry was added.
                                                     *   @note This field is also set on a CAM miss when it is full. */
        uint32_t first_match            : 7;    /**< First matched entry number, or on a CAM miss where the entry was added.
                                                     *   @note All-bits in this field are set on a CAM miss when it is full. */

        uint32_t match_bitf;                    /**< Bitfield of matching entries */
    };
    uint32_t value[2];                          /**< Accessor to entire structure */
} mem_cam_lookup16_add_out_t;

#else /* __LITTLEENDIAN */

/* See BE description above */
typedef union mem_cam_lookup16_in_t
{
    struct
    {
        uint32_t search                 : 16;
        uint32_t reserved1              : 16;

        uint32_t padding;
    };
    uint32_t value[2];
} mem_cam_lookup16_in_t;

typedef union mem_cam_lookup16_out_t
{
    struct
    {
        uint32_t first_match            : 8;
        uint32_t reserved               : 24;

        uint32_t match_bitf;
    };
    uint32_t value[2];
} mem_cam_lookup16_out_t;

typedef union mem_cam_lookup16_add_out_t
{
    struct
    {
        uint32_t first_match            : 7;
        uint32_t added                  : 1;

        uint32_t reserved               : 24;

        uint32_t match_bitf;
    };
    uint32_t value[2];
} mem_cam_lookup16_add_out_t;

#endif /* __BIGENDIAN */

/** Type for mem_cam_lookup16_out_t in read registers. */
typedef __xread mem_cam_lookup16_out_t        mem_cam_lookup16_out_in_read_reg_t;
/** Type for mem_cam_lookup16_out_t in read registers. */
typedef __xread mem_cam_lookup16_add_out_t    mem_cam_lookup16_add_out_in_read_reg_t;

/*
 * lookup 24
 */
#ifdef __BIGENDIAN
/** Input type for 24-bit CAM lookups.
 *
 * @note 24-bit CAM entries occupy 32-bits in MEM with 8-bits being available for user data
 *       or a count which is typically used with the 24-bit CAM add_inc functions.
 *
 * This search field of this structure needs to be populated before performing a CAM lookup.
 */
typedef union mem_cam_lookup24_in_t
{
    struct
    {
        uint32_t reserved1              : 8;    /**< Reserved */
        uint32_t search                 : 24;   /**< 24-bit value to search for in the CAM */
    };
    uint32_t value;                             /**< Accessor to entire structure */
} mem_cam_lookup24_in_t;

/** Input type for 24-bit CAM lookup and add operations.
 *
 * This search field of this structure needs to be populated before performing a CAM lookup.
 * The upper 8-bits of the CAM entry is set from the high_byte field in this structure when a
 * new entry is added on a CAM miss.
 *
 * @note 24-bit CAM entries occupy 32-bits in MEM with 8-bits being available for user data
 *       or a count which is typically used with the 24-bit CAM add_inc functions.
 */
typedef union mem_cam_lookup24_add_in_t
{
    struct
    {
        uint32_t high_byte              : 8;    /**< Value to set upper 8-bits in CAM to when entry is added  */
        uint32_t search                 : 24;   /**< 24-bit value to search for in the CAM */
    };
    uint32_t value;                             /**< Accessor to entire structure */
} mem_cam_lookup24_add_in_t;


/** Input type for 24-bit CAM lookup, add and inc operations.
 *
 * @see mem_cam_lookup24_add_in_t
 */
typedef mem_cam_lookup24_add_in_t mem_cam_lookup24_add_inc_in_t;

/** Output type of 24-bit CAM lookups.
 *
 * The match-bit fields indicate which CAM entries matched the lookup.
 */
typedef union mem_cam_lookup24_out_t
{
    struct
    {
        uint32_t match_bitf             : 16;   /**< Bits are set on CAM entries matching */
        uint32_t match_high_byte        : 8;    /**< Upper 8-bits of matched CAM entry */
        uint32_t first_match            : 8;    /**< Entry number of first match lookup - 0xff when not found */
    };
    uint32_t value;                             /**< Accessor to entire structure */
} mem_cam_lookup24_out_t;

/** Output type of 24-bit CAM lookup and add operations.
 *
 * The match-bit fields indicate which CAM entries matched the lookup.
 */
typedef union mem_cam_lookup24_add_out_t
{
    struct
    {
        uint32_t match_bitf             : 16;   /**< Bits are set on matching entries */
        uint32_t match_high_byte        : 8;    /**< Upper 8-bits of matched CAM entry is returned in this field */
        uint32_t added                  : 1;    /**< When set, this field indicates that an entry was added.
                                                     *   @note This field is also set on a CAM miss when it is full. */
        uint32_t first_match            : 7;    /**< First matched entry number, or on a CAM miss where the entry was added.
                                                     *   @note All-bits in this field are set on a CAM miss when it is full. */
    };
    uint32_t value;                             /**< Accessor to entire structure */
} mem_cam_lookup24_add_out_t;

/** Output type of 24-bit CAM lookup, add and increment operations.
 *
 * The match-bit fields indicate which CAM entries matched the lookup.
 */
typedef union mem_cam_lookup24_add_inc_out_t
{
    struct
    {
        uint32_t match_bitf             : 16;   /**< Bits are set on matching entries */
        uint32_t count                  : 8;    /**< Upper 8-bits of the matched CAM entry is returned in this field.
                                                         The upper 8-bits is incremented in memory after the CAM operation. */
        uint32_t added                  : 1;    /**< When set, this field indicates that an entry was added.
                                                     *   @note This field is also set on a CAM miss when it is full. */
        uint32_t first_match            : 7;    /**< First matched entry number, or on a CAM miss where the entry was added.
                                                     *   @note All-bits in this field are set on a CAM miss when it is full. */
    };
    uint32_t value;                             /**< Accessor to entire structure */
} mem_cam_lookup24_add_inc_out_t;

#else /* __LITTLEENDIAN */

/* See BE description above */
typedef union mem_cam_lookup24_in_t
{
    struct
    {
        uint32_t search                 : 24;
        uint32_t reserved1              : 8;
    };
    uint32_t value;
} mem_cam_lookup24_in_t;

typedef union mem_cam_lookup24_add_in_t
{
    struct
    {
        uint32_t search                 : 24;
        uint32_t high_byte              : 8;
    };
    uint32_t value;
} mem_cam_lookup24_add_in_t;

typedef mem_cam_lookup24_add_in_t mem_cam_lookup24_add_inc_in_t;

typedef union mem_cam_lookup24_out_t
{
    struct
    {
        uint32_t first_match            : 8;
        uint32_t match_high_byte        : 8;
        uint32_t match_bitf             : 16;
    };
    uint32_t value;
} mem_cam_lookup24_out_t;

typedef union mem_cam_lookup24_add_out_t
{
    struct
    {
        uint32_t first_match            : 7;
        uint32_t added                  : 1;
        uint32_t match_high_byte        : 8;
        uint32_t match_bitf             : 16;
    };
    uint32_t value;
} mem_cam_lookup24_add_out_t;

typedef union mem_cam_lookup24_add_inc_out_t
{
    struct
    {
        uint32_t first_match            : 7;
        uint32_t added                  : 1;
        uint32_t count                  : 8;
        uint32_t match_bitf             : 16;
    };
    uint32_t value;
} mem_cam_lookup24_add_inc_out_t;

#endif /* __BIGENDIAN */

/** Type for mem_cam_lookup24_out_t in read registers. */
typedef __xread mem_cam_lookup24_out_t            mem_cam_lookup24_out_in_read_reg_t;

/** Type for mem_cam_lookup24_add_out_t in read registers. */
typedef __xread mem_cam_lookup24_add_out_t        mem_cam_lookup24_add_out_in_read_reg_t;

/** Type for mem_cam_lookup24_add_inc_out_t in read registers. */
typedef __xread mem_cam_lookup24_add_inc_out_t    mem_cam_lookup24_add_inc_out_in_read_reg_t;

/*
 * lookup 32
 */
#ifdef __BIGENDIAN
/** Input type for 32-bit CAM lookups.
 *
 * This search field of this structure needs to be populated before performing a CAM lookup.
 */
typedef struct mem_cam_lookup32_in_t
{
    uint32_t search;                            /**< Value to search for */
} mem_cam_lookup32_in_t;

/** Output type of 32-bit CAM lookups.
 *
 * The match-bit fields indicate which CAM entries matched the lookup.
 */
typedef union mem_cam_lookup32_out_t
{
    struct
    {
        uint32_t match_bitf             : 16;   /**< Bits are set on matching entries */
        uint32_t match_high_byte        : 8;    /**< Upper 8-bits of the matched CAM entry is returned in this field */
        uint32_t first_match            : 8;    /**< First CAM entry that matched or 0xff - when not found */
    };
    uint32_t value;                             /**< Accessor to entire structure */
} mem_cam_lookup32_out_t;

/** Output type of 32-bit CAM lookup and add operations.
 *
 * The match-bit fields indicate which CAM entries matched the lookup.
 */
typedef union mem_cam_lookup32_add_out_t
{
    struct
    {
        uint32_t match_bitf             : 16;   /**< Bits are set on matching entries */
        uint32_t match_high_byte        : 8;    /**< Upper 8-bits of the matched CAM entry is returned in this field. */
        uint32_t added                  : 1;    /**< When set, this field indicates that an entry was added.
                                                     *   @note This field is also set on a CAM miss when it is full. */
        uint32_t first_match            : 7;    /**< First matched entry number, or on a CAM miss where the entry was added.
                                                     *   @note All-bits in this field are set on a CAM miss when it is full. */        };
    uint32_t value;                             /**< Accessor to entire structure */
} mem_cam_lookup32_add_out_t;

#else /* __LITTLEENDIAN */

/* See BE description above */
typedef struct
{
    uint32_t search;
} mem_cam_lookup32_in_t;

typedef union mem_cam_lookup32_out_t
{
    struct
    {
        uint32_t first_match            : 8;
        uint32_t match_high_byte        : 8;
        uint32_t match_bitf             : 16;
    };
    uint32_t value;
} mem_cam_lookup32_out_t;

typedef union mem_cam_lookup32_add_out_t
{
    struct
    {
        uint32_t first_match            : 7;
        uint32_t added                  : 1;
        uint32_t match_high_byte        : 8;
        uint32_t match_bitf             : 16;
    };
    uint32_t value;
} mem_cam_lookup32_add_out_t;

#endif /* __BIGENDIAN */

/** Type for mem_cam_lookup32_out_t in read registers. */
typedef __xread mem_cam_lookup32_out_t        mem_cam_lookup32_out_in_read_reg_t;

/** Type for mem_cam_lookup32_add_out_t in read registers. */
typedef __xread mem_cam_lookup32_add_out_t    mem_cam_lookup32_add_out_in_read_reg_t;




/** Initialize a 128-bit CAM in 32-bit addressed memory.
 *
 * @param cam   Pointer to the CAM in MEM
 */
__intrinsic
void cmd_mem_cam128_init_ptr32(
    __mem mem_cam128_in_mem_t *cam
);

/** Initialize a 128-bit CAM in 40-bit addressed MEM.
 *
 * @param cam   Pointer to the CAM in MEM
 */
__intrinsic
void cmd_mem_cam128_init_ptr40(
    mem_cam128_ptr40_t cam
);

/** Initialize a 256-bit CAM in in 32-bit addressed memory.
 *
 * @param cam   Pointer to the CAM in MEM
 */
__intrinsic
void cmd_mem_cam256_init_ptr32(
    __mem mem_cam256_in_mem_t *cam
);

/** Initialize a 256-bit CAM in 40-bit addressed MEM.
 *
 * @param cam   Pointer to the CAM in MEM
 */
__intrinsic
void cmd_mem_cam256_init_ptr40(
    mem_cam256_ptr40_t cam
);

/** Initialize a 384-bit CAM in in 32-bit addressed memory.
 *
 * @param cam   Pointer to the CAM in MEM
 */
__intrinsic
void cmd_mem_cam384_init_ptr32(
    __mem mem_cam384_in_mem_t *cam
);

/** Initialize a 384-bit CAM in 40-bit addressed MEM.
 *
 * @param cam   Pointer to the CAM in MEM
 */
__intrinsic
void cmd_mem_cam384_init_ptr40(
    mem_cam384_ptr40_t cam
);

/** Initialize a 512-bit CAM in in 32-bit addressed memory.
 *
 * @param cam   Pointer to the CAM in MEM
 */
__intrinsic
void cmd_mem_cam512_init_ptr32(
    __mem mem_cam512_in_mem_t *cam
);

/** Initialize a 512-bit CAM in in 40-bit addressed memory.
 *
 * @param cam   Pointer to the CAM in MEM
 */
__intrinsic
void cmd_mem_cam512_init_ptr40(
    mem_cam512_ptr40_t cam
);

/** Set a 8-bit entry in a 128-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15
 *
 */
__intrinsic
void mem_cam128_set8_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 8-bit entry in a 128-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15
 *
 */
__intrinsic
void mem_cam128_set8_ptr40(
    mem_cam128_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 8-bit entry in a 256-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 */
__intrinsic
void mem_cam256_set8_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 8-bit entry in a 256-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 */
__intrinsic
void mem_cam256_set8_ptr40(
    mem_cam256_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 8-bit entry in a 384-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 47.
 *
 */
__intrinsic
void mem_cam384_set8_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 8-bit entry in a 384-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 47.
 *
 */
__intrinsic
void mem_cam384_set8_ptr40(
    mem_cam384_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 8-bit entry in a 512-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 63.
 *
 */
__intrinsic
void mem_cam512_set8_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 8-bit entry in a 512-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 63.
 *
 */
__intrinsic
void mem_cam512_set8_ptr40(
    mem_cam512_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16-bit entry in a 128-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 */
__intrinsic
void mem_cam128_set16_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16-bit entry in a 128-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 */
__intrinsic
void mem_cam128_set16_ptr40(
    mem_cam128_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16-bit entry in a 256-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 */
__intrinsic
void mem_cam256_set16_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16-bit entry in a 256-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 */
__intrinsic
void mem_cam256_set16_ptr40(
    mem_cam256_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16-bit entry in a 384-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 23.
 *
 */
__intrinsic
void mem_cam384_set16_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16-bit entry in a 384-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 23.
 *
 */
__intrinsic
void mem_cam384_set16_ptr40(
    mem_cam384_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16-bit entry in a 512-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 */
__intrinsic
void mem_cam512_set16_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 16-bit entry in a 512-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 31.
 *
 */
__intrinsic
void mem_cam512_set16_ptr40(
    mem_cam512_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 24-bit entry in a 128-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 * 24-bit CAM entries occupy 32-bits in MEM with 8-bits being available for user
 * data or a count which is typically used with the 24-bit CAM add_inc functions.
 * @see mem_cam128_lookup24_add_inc_ptr32
 *
 */
__intrinsic
void mem_cam128_set24_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 24-bit entry in a 128-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 * 24-bit CAM entries occupy 32-bits in MEM with 8-bits being available for user
 * data or a count which is typically used with the 24-bit CAM add_inc functions.
 * @see mem_cam128_lookup24_add_inc_ptr40
 *
 */
__intrinsic
void mem_cam128_set24_ptr40(
    mem_cam128_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 24-bit entry in a 256-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 */
__intrinsic
void mem_cam256_set24_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 24-bit entry in a 256-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 */
__intrinsic
void mem_cam256_set24_ptr40(
    mem_cam256_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 24-bit entry in a 384-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 */
__intrinsic
void mem_cam384_set24_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 24-bit entry in a 384-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 */
__intrinsic
void mem_cam384_set24_ptr40(
    mem_cam384_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 24-bit entry in a 512-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 */
__intrinsic
void mem_cam512_set24_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 24-bit entry in a 512-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 */
__intrinsic
void mem_cam512_set24_ptr40(
    mem_cam512_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32-bit entry in a 128-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 */
__intrinsic
void mem_cam128_set32_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32-bit entry in a 128-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 3.
 *
 */
__intrinsic
void mem_cam128_set32_ptr40(
    mem_cam128_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32-bit entry in a 256-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 */
__intrinsic
void mem_cam256_set32_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32-bit entry in a 256-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 7.
 *
 */
__intrinsic
void mem_cam256_set32_ptr40(
    mem_cam256_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32-bit entry in a 384-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 */
__intrinsic
void mem_cam384_set32_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32-bit entry in a 384-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 11.
 *
 */
__intrinsic
void mem_cam384_set32_ptr40(
    mem_cam384_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32-bit entry in a 512-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 */
__intrinsic
void mem_cam512_set32_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Set a 32-bit entry in a 512-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param index             CAM entry to set (0 is the first entry)
 * @param value             Value of the CAM entry
 * @param sync              Type of synchronization to use (sig_done or ctx_swap)
 * @param sig_ptr           Signal to raise upon completion
 *
 * @note The maximum index is 15.
 *
 */
__intrinsic
void mem_cam512_set32_ptr40(
    mem_cam512_ptr40_t cam,
    uint32_t index,
    uint32_t value,
    sync_t sync,
    SIGNAL *sig_ptr
);

/** Perform a 8-bit lookup in a 128-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_out_in_read_reg_t *mem_cam128_lookup8_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 128-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_out_in_read_reg_t *mem_cam128_lookup8_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 256-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_out_in_read_reg_t *mem_cam256_lookup8_ptr32(
   __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 256-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_out_in_read_reg_t *mem_cam256_lookup8_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 384-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_out_in_read_reg_t *mem_cam384_lookup8_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 384-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_out_in_read_reg_t *mem_cam384_lookup8_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 512-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_out_in_read_reg_t *mem_cam512_lookup8_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 512-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_out structure occupying
 *                          two read transfer registers.
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_out_in_read_reg_t *mem_cam512_lookup8_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 128-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_cam_lookup8_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_add_out_in_read_reg_t *mem_cam128_lookup8_add_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 128-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup
 *
 * @note                    The result is returned in a mem_cam_lookup8_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_add_out_in_read_reg_t *mem_cam128_lookup8_add_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 256-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_add_out_in_read_reg_t *mem_cam256_lookup8_add_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 256-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_add_out_in_read_reg_t *mem_cam256_lookup8_add_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 384-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_add_out_in_read_reg_t *mem_cam384_lookup8_add_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 384-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_add_out_in_read_reg_t *mem_cam384_lookup8_add_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 512-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_add_out_in_read_reg_t *mem_cam512_lookup8_add_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 8-bit lookup in a 512-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup8_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup8_in_t, mem_cam_lookup8_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup8_add_out_in_read_reg_t *mem_cam512_lookup8_add_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup8_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 128-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_out_in_read_reg_t *mem_cam128_lookup16_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 128-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_out_in_read_reg_t *mem_cam128_lookup16_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 256-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_out_in_read_reg_t *mem_cam256_lookup16_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 256-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_out_in_read_reg_t *mem_cam256_lookup16_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 384-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_out_in_read_reg_t *mem_cam384_lookup16_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 384-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_out_in_read_reg_t *mem_cam384_lookup16_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 512-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_out_in_read_reg_t *mem_cam512_lookup16_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 512-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_out_in_read_reg_t *mem_cam512_lookup16_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 128-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_add_out_in_read_reg_t *mem_cam128_lookup16_add_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 128-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_add_out_in_read_reg_t *mem_cam128_lookup16_add_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 256-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_add_out_in_read_reg_t *mem_cam256_lookup16_add_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 256-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_add_out_in_read_reg_t *mem_cam256_lookup16_add_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 384-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_add_out_in_read_reg_t *mem_cam384_lookup16_add_ptr32(
   __mem  mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 384-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_add_out_in_read_reg_t *mem_cam384_lookup16_add_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 512-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_add_out_in_read_reg_t *mem_cam512_lookup16_add_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 16-bit lookup in a 512-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup16_add_out structure occupying
 *                          two read transfer registers.
 *
 * @see                     mem_cam_lookup16_in_t, mem_cam_lookup16_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup16_add_out_in_read_reg_t *mem_cam512_lookup16_add_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup16_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 128-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_out_in_read_reg_t *mem_cam128_lookup24_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 128-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_out_in_read_reg_t *mem_cam128_lookup24_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 256-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_out_in_read_reg_t *mem_cam256_lookup24_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 256-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_out_in_read_reg_t *mem_cam256_lookup24_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 384-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_out_in_read_reg_t *mem_cam384_lookup24_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 384-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_out_in_read_reg_t *mem_cam384_lookup24_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 512-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_out_in_read_reg_t *mem_cam512_lookup24_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 512-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_out_in_read_reg_t *mem_cam512_lookup24_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup24_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 128-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_out_in_read_reg_t *mem_cam128_lookup24_add_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 128-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_out_in_read_reg_t *mem_cam128_lookup24_add_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 256-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_out_in_read_reg_t *mem_cam256_lookup24_add_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 256-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_out_in_read_reg_t *mem_cam256_lookup24_add_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 384-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_out_in_read_reg_t *mem_cam384_lookup24_add_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 384-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_out_in_read_reg_t *mem_cam384_lookup24_add_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 512-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_out_in_read_reg_t *mem_cam512_lookup24_add_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 512-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_out_in_read_reg_t *mem_cam512_lookup24_add_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 128-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * When an entry is found the upper 8-bits of the entry (count) is incremented in a non saturating way. The
 * pre-incremented value of count is returned in the count field of the mem_cam_lookup24_add_out_t structure.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_inc_out_in_read_reg_t *mem_cam128_lookup24_add_inc_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 128-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * When an entry is found the upper 8-bits of the entry (count) is incremented in a non saturating way. The
 * pre-incremented value of count is returned in the count field of the mem_cam_lookup24_add_out_t structure.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_inc_out_in_read_reg_t *mem_cam128_lookup24_add_inc_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 256-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * When an entry is found the upper 8-bits of the entry (count) is incremented in a non saturating way. The
 * pre-incremented value of count is returned in the count field of the mem_cam_lookup24_add_out_t structure.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_inc_out_in_read_reg_t *mem_cam256_lookup24_add_inc_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 256-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * When an entry is found the upper 8-bits of the entry (count) is incremented in a non saturating way. The
 * pre-incremented value of count is returned in the count field of the mem_cam_lookup24_add_out_t structure.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_inc_out_in_read_reg_t *mem_cam256_lookup24_add_inc_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 384-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * When an entry is found the upper 8-bits of the entry (count) is incremented in a non saturating way. The
 * pre-incremented value of count is returned in the count field of the mem_cam_lookup24_add_out_t structure.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_inc_out_in_read_reg_t *mem_cam384_lookup24_add_inc_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 384-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * When an entry is found the upper 8-bits of the entry (count) is incremented in a non saturating way. The
 * pre-incremented value of count is returned in the count field of the mem_cam_lookup24_add_out_t structure.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_inc_out_in_read_reg_t *mem_cam384_lookup24_add_inc_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 512-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * When an entry is found the upper 8-bits of the entry (count) is incremented in a non saturating way. The
 * pre-incremented value of count is returned in the count field of the mem_cam_lookup24_add_out_t structure.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_inc_out_in_read_reg_t *mem_cam512_lookup24_add_inc_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 24-bit lookup in a 512-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * The result is returned in a mem_cam_lookup24_add_out structure in a read xfer register.
 *
 * When an entry is found the upper 8-bits of the entry (count) is incremented in a non saturating way. The
 * pre-incremented value of count is returned in the count field of the mem_cam_lookup24_add_out_t structure.
 *
 * @see                     mem_cam_lookup24_in_t, mem_cam_lookup24_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup24_add_inc_out_in_read_reg_t *mem_cam512_lookup24_add_inc_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup24_add_inc_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 128-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_out_in_read_reg_t *mem_cam128_lookup32_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 128-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_out_in_read_reg_t *mem_cam128_lookup32_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 256-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_out_in_read_reg_t *mem_cam256_lookup32_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 256-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_out_in_read_reg_t *mem_cam256_lookup32_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 384-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_out_in_read_reg_t *mem_cam384_lookup32_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 384-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_out_in_read_reg_t *mem_cam384_lookup32_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 512-bit CAM in 32-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_out_in_read_reg_t *mem_cam512_lookup32_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 512-bit CAM in 40-bit addressed MEM.
 *
 * @param cam               Pointer to the CAM structure in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_out structure in one read transfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_out_in_read_reg_t *mem_cam512_lookup32_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 128-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_add_out_in_read_reg_t *mem_cam128_lookup32_add_ptr32(
    __mem mem_cam128_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 128-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_add_out_in_read_reg_t *mem_cam128_lookup32_add_ptr40(
    mem_cam128_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 256-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_add_out_in_read_reg_t *mem_cam256_lookup32_add_ptr32(
    __mem mem_cam256_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 256-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_add_out_in_read_reg_t *mem_cam256_lookup32_add_ptr40(
    mem_cam256_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 384-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_add_out_in_read_reg_t *mem_cam384_lookup32_add_ptr32(
    __mem mem_cam384_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 384-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_add_out_in_read_reg_t *mem_cam384_lookup32_add_ptr40(
    mem_cam384_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 512-bit CAM in 32-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_add_out_in_read_reg_t *mem_cam512_lookup32_add_ptr32(
    __mem mem_cam512_in_mem_t *cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** Perform a 32-bit lookup in a 512-bit CAM in 40-bit addressed MEM and if a match is not found add the entry to
 * the first empty slot.
 *
 * @param cam               Pointer to the CAM in MEM
 * @param xfer              Pointer to write xfer register containing the data to lookup
 * @param sync              Type of synchronization to use (must be sig_done)
 * @param sig_pair_ptr      Signal pair to raise upon completion
 * @return                  Pointer to the first read transfer register containing the result of the lookup.
 *
 * @note                    The result is returned in a mem_cam_lookup32_add_out structure in a read xfer register.
 *
 * @see                     mem_cam_lookup32_in_t, mem_cam_lookup32_add_out_t
 *
 *
 */
__intrinsic
mem_cam_lookup32_add_out_in_read_reg_t *mem_cam512_lookup32_add_ptr40(
    mem_cam512_ptr40_t cam,
    __xwrite mem_cam_lookup32_in_t *xfer,
    sync_t sync,
    SIGNAL_PAIR *sig_pair_ptr
);

/** @}
 * @}
 */


#endif /* __NFP_MEM_CAM_H__ */
