/*
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 */

/* NFP definitions for use by intrinsic functions */

#ifndef __NFP_INTRINSIC_H__
#define __NFP_INTRINSIC_H__

#include <nfp.h>
#include <stdint.h>

#ifndef NFP_LIB_ANY_NFCC_VERSION
    #if (!defined(__NFP_TOOL_NFCC) ||                       \
        (__NFP_TOOL_NFCC < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFCC > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#define _INTRINSIC_BEGIN
#define _INTRINSIC_END
#define CT_QPERFINFO_INDIRECT_REF(fn)  \
        __ct_Qperfinfo(128, #fn "(): Size of data access cannot be determined at compile-time. __implicit_read/write may be needed to protect xfer buffer. Use of " #fn "_ind() is recommended instead.")

#define CT_ASSERT(expr) __ct_assert(expr, #expr)

#define _INTRINSIC_UNSUPPORTED_NFP3200_INDIRECT_REF() { const int32_t Unsupported_NFP3200_Indirect_Reference = 0; CT_ASSERT(Unsupported_NFP3200_Indirect_Reference); }

#define _INTRINSIC_NEED_40_BIT_THIRD_PARTY() { const int32_t Need_40_bit_Third_Party_Addressing_Mode_Enabled = 0;  CT_ASSERT(Need_40_bit_Third_Party_Addressing_Mode_Enabled); }


/*
 * Indirect reference macros for indirect words
 */

#define INTRINSIC_IND_ALU_COUNT_MAX 32

/*
 * Override fields macros  used internally by the standard library.
 */

// Override byte mask as data in PREV ALU
#define _INTRINSIC_OVERRIDE_BYTEMASK_AS_DATA_FIELD(ov_alu, ov_data)             \
{                                                                               \
    ov_alu = ov_alu | (((ov_data) & 0xff) << 16) | ovr_data_byte_mask_bit;      \
}

// Override length but mask/limit the length bits required by the operation
#define _INTRINSIC_OVERRIDE_LENGTH_FIELD(ov_alu, ov_len)                        \
{                                                                               \
    ov_alu = ov_alu | (((ov_len) & 0x1f) << 8) | ovr_length_bit;                \
}

// Override immediate 16 bits
#define _INTRINSIC_OVERRIDE_IMM_DATA_FIELD(ov_alu, ov_data)                     \
{                                                                               \
    ov_alu = ov_alu | ovr_data_16bit_imm_bit | (((ov_data) & 0xffff) << 16);    \
}

// Override xfer
#define _INTRINSIC_OVERRIDE_FULL_REF_FIELD(ov_alu, ov_data)                     \
{                                                                               \
    ov_alu = ov_alu | ovr_data_full_ref_bit | (((ov_data) & 0xffff) << 16);     \
}

// Override byte mask which has ovr bit set in PREV ALU result and data in indirect CSR
#define _INTRINSIC_OVERRIDE_BYTEMASK_IND_CSR_FIELD(ov_alu, ov_csr, ov_bm)       \
{                                                                               \
    ov_alu = ov_alu | ovr_byte_mask_bit;                                        \
    ov_csr = ov_csr | ((ov_bm) & 0xff);                                         \
}

// Override data master
#define _INTRINSIC_OVERRIDE_DATA_MASTER_FIELD(ov_alu, ov_csr, ov_master)        \
{                                                                               \
    ov_alu = ov_alu | ovr_data_master_bit;                                      \
    ov_csr = ov_csr | (((ov_master) & 0x3ff) << 20);                            \
}

// Override 14 bits - island+master+signal_master in CSR ind
#define _INTRINSIC_OVERRIDE_SIGNAL_ISLAND_AND_DATA_MASTER_FIELD(ov_alu, ov_csr, ov_master)     \
{                                                                                       \
    ov_alu = ov_alu | ovr_signal_and_data_master_bit;                                   \
    ov_csr = ov_csr | ((ov_master & 0x3fff) << 16);                                     \
}

/*
 * These macros are used internally by the standard library.
 * These macros initialises and set the override bits plus override fields
 */

/*
 * Override bytemask in prev ALU result
 */
#define _INTRINSIC_OVERRIDE_BYTEMASK_IN_ALU(ov_ind, bytemask)                              \
{                                                                                          \
    ov_ind.alu_ind.value = 0x00;                                                           \
    _INTRINSIC_OVERRIDE_BYTEMASK_AS_DATA_FIELD(ov_ind.alu_ind.value, bytemask);            \
}

#define _INTRINSIC_OVERRIDE_BYTEMASK _INTRINSIC_OVERRIDE_BYTEMASK_IN_ALU

/*
 * Override length in prev ALU result
 */
#define _INTRINSIC_OVERRIDE_LENGTH(ov_ind, length)                                          \
{                                                                                           \
    ov_ind.alu_ind.value = 0x00;                                                            \
    _INTRINSIC_OVERRIDE_LENGTH_FIELD(ov_ind.alu_ind.value, length);                         \
}

/*
 * Override length and bytemask in prev ALU result
 */
#define _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ov_ind, length, bytemask)            \
{                                                                                           \
    ov_ind.alu_ind.value = 0x00;                                                            \
    _INTRINSIC_OVERRIDE_BYTEMASK_AS_DATA_FIELD(ov_ind.alu_ind.value, bytemask);             \
    _INTRINSIC_OVERRIDE_LENGTH_FIELD(ov_ind.alu_ind.value, length);                         \
}

/*
 * Override immediate data in prev ALU result
 */
#define _INTRINSIC_OVERRIDE_DATA_IN_ALU(ov_ind, data)                                       \
{                                                                                           \
    ov_ind.alu_ind.value = 0x00;                                                            \
    _INTRINSIC_OVERRIDE_IMM_DATA_FIELD(ov_ind.alu_ind.value, data);                         \
}

/*
 * Override length and immediate data in prev ALU result
 */
#define _INTRINSIC_OVERRIDE_IMM_DATA_AND_LENGTH(ov_ind, data, length)                       \
{                                                                                           \
    ov_ind.alu_ind.value = 0x00;                                                            \
    _INTRINSIC_OVERRIDE_IMM_DATA_FIELD(ov_ind.alu_ind.value, data);                         \
    _INTRINSIC_OVERRIDE_LENGTH_FIELD(ov_ind.alu_ind.value, length);                         \
}

/*
 * Override full data ref in prev ALU result
 */
#define _INTRINSIC_OVERRIDE_FULL_DATA_REF(ov_ind, data)                                     \
{                                                                                           \
    ov_ind.alu_ind.value = 0x00;                                                            \
    _INTRINSIC_OVERRIDE_FULL_REF_FIELD(ov_ind.alu_ind.value, data);                         \
}


/* Count override */
#define _INTRINSIC_IND_ALU_COUNT_MIN_MAX(count, min_value, max_value)       \
    generic_ind_t ind;                                                      \
    if (__is_ct_const(count))                                               \
    {                                                                       \
        CT_ASSERT((count) >= (min_value) && (count) <= (max_value));        \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        RT_RANGE_ASSERT((count) >= (min_value) && (count) <= (max_value));  \
    }                                                                       \
    _INTRINSIC_OVERRIDE_LENGTH(ind, (count-1));                             \
    __asm { alu[--, --, B, ind.alu_ind.value] }


/* Count and Byte mask override */
#define _INTRINSIC_IND_ALU_COUNT_MIN_MAX_MASK_MAX(count, min_value, max_value, mask, max_mask) \
    generic_ind_t ind;                                                      \
    if (__is_ct_const(count))                                               \
    {                                                                       \
        CT_ASSERT((count) >= (min_value) && (count) <= (max_value));        \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        RT_RANGE_ASSERT((count) >= (min_value) && (count) <= (max_value));  \
    }                                                                       \
    if (__is_ct_const(mask))                                                \
    {                                                                       \
        CT_ASSERT((mask) <= (max_mask));                                    \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        RT_RANGE_ASSERT((mask) <= (max_mask));                              \
    }                                                                       \
    _INTRINSIC_OVERRIDE_LENGTH_AND_BYTEMASK_IN_ALU(ind, (count-1), mask);   \
    __asm { alu[--, --, B, ind.alu_ind.value] }


/* Byte mask override */
#define _INTRINSIC_IND_ALU_MASK_MAX(mask,max_mask)                          \
    generic_ind_t ind;                                                      \
    if (__is_ct_const(mask))                                                \
    {                                                                       \
        CT_ASSERT((mask) >= 0x1 &&  (mask) <= (max_mask));                  \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        RT_RANGE_ASSERT((mask) >= 0x1 &&  (mask) <= (max_mask));            \
    }                                                                       \
    _INTRINSIC_OVERRIDE_BYTEMASK_IN_ALU(ind, mask);                         \
    __asm { alu[--, --, B, ind.alu_ind.value] }


/* Convert 40 bit address to high and low address */
#define _INTRINSIC_CONVERT_HI_LO_ADDRESS(address)                           \
    hi_addr = (uint32_t)((uint64_t)address>>32);                            \
    hi_addr = (uint32_t)hi_addr<<24;                                        \
    low_addr = (uint32_t)address;


/**
 * @addtogroup misc Miscellaneous Intrinsics
 * @{
 */

/* External memory string constant defines */
#define i24_emem_base           "__ADDR_I24_EMEM"      // Type: EMEM, Island: 24
#define i25_emem_base           "__ADDR_I25_EMEM"      // Type: EMEM, Island: 25
#define i26_emem_base           "__ADDR_I26_EMEM"      // Type: EMEM, Island: 26

#define sleep(a)                _sleep(a)
#define set_timestamp(a)        _set_timestamp(a)
#define timestamp_start()       _timestamp_start()
#define timestamp_stop(a)       _timestamp_stop(a)
#define set_profile_count(a)    _set_profile_count(a)
#define profile_count_start()   _profile_count_start()
#define profile_count_stop(a)   _profile_count_stop(a)



/** Get the current Microengine number and island id.
 *  Also available for use LoadTimeConstant("__meid");
 *
 * @return The value of the currently executing microengine.
 *         (island_id << 4) | (me_num + 4)
 *
 * @see __LoadTimeConstant
 */
uint32_t _ME(void);


/** Get the current island/cluster id of the current Microengine.
 *  Also available for use is LoadTimeConstant("_island");
 *
 * @return The island/cluster id of the currently executing microengine
 *
 * @see __LoadTimeConstant
 */
uint32_t _island(void);


/** Sets the local_csr_profile_count field of the local_csr_t enum.
 *
 * @param profile_count Profile count to set.
 */
void _set_profile_count(uint32_t profile_count);


/** Uses local_csr_profile_count to measure time elapse in cycle between start and stop.
 *
 * The _profile_count_start() function returns a handle that is used by the _profile_count_stop()
 * function.
 *
 * @see _profile_count_stop
 */
uint32_t _profile_count_start(void);

/** This function use local_csr_profile_count to measure time elapse in cycle between start and stop.
 *
 * @param handle Handle returned by the _profile_count_start() function that is passed to the _profile_count_stop() function.
 *
 * The _profile_count_start() function returns a handle that is used by the _profile_count_stop()
 * function.
 *
 * @see _profile_count_start
 *
 */
uint32_t _profile_count_stop(uint32_t handle);


/** Assert if unsigned value is not between min_value and max_value. If all of these are
 * constants, a compile time assert is shown otherwise a runtime assert.
 *
 * @param value       Value to verify if within range
 * @param min_value   The minimum value of the range
 * @param max_value   The maximum value of the range
 *
 */
__intrinsic
void assert_range(uint32_t value, uint32_t min_value, uint32_t max_value);



/** Test the bit_pos in data word and return a 1 if set or 0 if clear.
 *
 * @param data      Data word to test
 * @param bit_pos   Bit position in data to test
 */
int32_t bit_test(
    uint32_t data,
    uint32_t bit_pos
);

/** This enumeration defines the state values that can be tested
 * with the inp_state_test() intrinsic.
 */
typedef enum
{
    inp_state_nn_empty          = 0x0, /**< NN_Ring in the neighbor microengine is empty.       */
    inp_state_nn_full           = 0x1, /**< NN_Ring in the neighbor microengine is full.        */

    inp_state_ctm_ring0_status  = 0x2, /**< Indicates if CTM Ring 0 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTM_RING_BASE_0 register.
                                        */
    inp_state_ctm_ring1_status  = 0x3, /**< Indicates if CTM Ring 1 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTMRING_BASE_1 register.
                                        */
    inp_state_ctm_ring2_status  = 0x4, /**< Indicates if CTM Ring 2 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTM_RING_BASE_2 register.
                                        */
    inp_state_ctm_ring3_status  = 0x5, /**< Indicates if CTM Ring 3 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTM_RING_BASE_3 register.
                                        */
    inp_state_ctm_ring4_status  = 0x6, /**< Indicates if CTM Ring 4 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTM_RING_BASE_4 register.
                                        */
    inp_state_ctm_ring5_status  = 0x7, /**< Indicates if CTM Ring 5 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTM_RING_BASE_5 register.
                                        */
    inp_state_ctm_ring6_status  = 0x8, /**< Indicates if CTM Ring 6 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTM_RING_BASE_6 register.
                                        */
    inp_state_ctm_ring7_status  = 0x9, /**< Indicates if CTM Ring 7 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTM_RING_BASE_7 register.
                                        */
    inp_state_ctm_ring8_status  = 0xA, /**< Indicates if CTM Ring 8 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTM_RING_BASE_8 register.
                                        */
    inp_state_ctm_ring9_status  = 0xB, /**< Indicates if CTM Ring 9 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTM_RING_BASE_9 register.
                                        */
    inp_state_ctm_ring10_status = 0xC, /**< Indicates if CTM Ring 10 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTM_RING_BASE_10 register.
                                        */
    inp_state_ctm_ring11_status = 0xD, /**< Indicates if CTM Ring 11 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTM_RING_BASE_11 register.
                                        */
    inp_state_ctm_ring12_status = 0xE, /**< Indicates if CTM Ring 12 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTM_RING_BASE_12 register.
                                        */
    inp_state_ctm_ring13_status = 0xF, /**< Indicates if CTM Ring 13 is full or empty.
                                            The Full Flag can be configured as an Empty Flag by
                                            the RING_STATUS_FLAG bit in the CTM_RING_BASE_13 register.
                                        */
    inp_state_ctm_ring0_full    = inp_state_ctm_ring0_status,   /**< Indicates if CTM Ring 0 is full.
                                                                    This assumes CTM_Ring_Base_0[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
    inp_state_ctm_ring1_full    = inp_state_ctm_ring1_status,   /**< Indicates if CTM Ring 1 is full.
                                                                    This assumes CTM_Ring_Base_1[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
    inp_state_ctm_ring2_full    = inp_state_ctm_ring2_status,   /**< Indicates if CTM Ring 2 is full.
                                                                    This assumes CTM_Ring_Base_2[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
    inp_state_ctm_ring3_full    = inp_state_ctm_ring3_status,   /**< Indicates if CTM Ring 3 is full.
                                                                    This assumes CTM_Ring_Base_3[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
    inp_state_ctm_ring4_full    = inp_state_ctm_ring4_status,   /**< Indicates if CTM Ring 4 is full.
                                                                    This assumes CTM_Ring_Base_4[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
    inp_state_ctm_ring5_full    = inp_state_ctm_ring5_status,   /**< Indicates if CTM Ring 5 is full.
                                                                    This assumes CTM_Ring_Base_5[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
    inp_state_ctm_ring6_full    = inp_state_ctm_ring6_status,   /**< Indicates if CTM Ring 6 is full.
                                                                    This assumes CTM_Ring_Base_6[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
    inp_state_ctm_ring7_full    = inp_state_ctm_ring7_status,   /**< Indicates if CTM Ring 7 is full.
                                                                    This assumes CTM_Ring_Base_7[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
    inp_state_ctm_ring8_full    = inp_state_ctm_ring8_status,   /**< Indicates if CTM Ring 8 is full.
                                                                    This assumes CTM_Ring_Base_8[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
    inp_state_ctm_ring9_full    = inp_state_ctm_ring9_status,   /**< Indicates if CTM Ring 9 is full.
                                                                    This assumes CTM_Ring_Base_9[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
    inp_state_ctm_ring10_full   = inp_state_ctm_ring10_status,  /**< Indicates if CTM Ring 10 is full.
                                                                    This assumes CTM_Ring_Base_10[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
    inp_state_ctm_ring11_full   = inp_state_ctm_ring11_status,  /**< Indicates if CTM Ring 11 is full.
                                                                    This assumes CTM_Ring_Base_11[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
    inp_state_ctm_ring12_full   = inp_state_ctm_ring12_status,  /**< Indicates if CTM Ring 12 is full.
                                                                    This assumes Scratch_Ring_Base_12[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
    inp_state_ctm_ring13_full   = inp_state_ctm_ring13_status   /**< Indicates if CTM Ring 13 is full.
                                                                    This assumes CTM_Ring_Base_13[Ring_Status_Flag]
                                                                    is configured to set the flag when the ring is full. */
} inp_state_t;



/** Tests the value of the specified input state name.
 *
 * @param state State to test
 *
 * This function tests the value of the specified state name and returns a 1 if the state is set or 0 if clear.
 * The argument state must be a constant literal as required by the microcode assembler; otherwise,
 * the compiler generates a runtime check, if possible, with loss of performance.
 */
int32_t inp_state_test(
    inp_state_t state
);



/** @name Next Neighbor intrinsics
 * @{
 */

/** Dequeue from next neighbor ring.
 *
 * @return Next neighbor ring
 *
 * This function returns the next neighbor ring indexed by NN_GET without post-incrementing NN_GET.
 *
 */
uint32_t nn_ring_dequeue(void);

/** Dequeue from and post-increment next neighbor ring.
 *
 * @return Next neighbor ring
 *
 * This function returns the next neighbor ring indexed by NN_GET, then post-increments NN_GET.
 *
 */
uint32_t nn_ring_dequeue_incr(void);

/** Enqueue to the next neighbor ring.
 *
 * @param val    Value to set next neighbor ring indexed by NN_PUT
 *
 * This function sets the next neighbor ring indexed by NN_PUT with val and post-increments NN_PUT.
 *
 */
void nn_ring_enqueue_incr(uint32_t val);
/**
 * @}
 */


/** @name Byte align intrinsics
 * @{
 */
/** Byte align block in Big-endian format.
 *
 * @param n_byte_align_oper Number of byte_align operations to perform on dest and src
 * @param dest              Address that stores the results of the alignment shift
 * @param src               Address that contains the pair of 32-bit elements to shift
 * @param shift_cnt         Number of bytes to shift
 *
 * This function sets local_csr BYTE_INDEX to shift_cnt, then performs n_byte_align_oper times
 * of consecutive byte_align_le operations on a pair of 32-bit elements in dest and src. Arguments
 * dest and src are addresses of Xfer/GPR 32-bit variables (or aggregates of 32-bit elements) that
 * must be enregisterized.
 *
 */
void byte_align_block_be(
    uint32_t n_byte_align_oper,
    void *dest,
    void *src,
    unsigned shift_cnt
);

/** Byte align block in Little-endian format.
 *
 * @param n_byte_align_oper Number of byte_align operations to perform on dest and src
 * @param dest              Address that stores the results of the alignment shift
 * @param src               Address that contains the pair of 32-bit elements to shift
 * @param shift_cnt         Number of bytes to shift
 *
 * This function sets local_csr BYTE_INDEX to shift_cnt, then performs n_byte_align_oper times
 * of consecutive byte_align_le operations on a pair of 32-bit elements in dest and src. Arguments
 * dest and src are addresses of Xfer/GPR 32-bit variables (or aggregates of 32-bit elements) that
 * must be enregisterized.
 *
 */
void byte_align_block_le(
    uint32_t n_byte_align_oper,
    void *dest,
    void *src,
    unsigned shift_cnt
);
/**
 * @}
 */


/** @name Other intrinsics
 * @{
 */
/** Find the first (least significant) bit set in data.
 *
 * @param data  Data to examine
 *
 * This function finds the first (least significant) bit set in data and returns its bit position.
 * If there are no bits set (i.e., the data argument is 0) then the return value is undefined.
 * Otherwise, the return value is in the range 0 through 31.
 *
 */
uint32_t ffs(
    uint32_t data
);


/** Halt the ME
 */
void halt();


/** Swap bytes in a long word.
 *
 *  @param lw Long word to swap bytes in
 */
uint32_t bswap(uint32_t lw);


/** Swap bits in each byte of a long word.
 *
 * @param lw Long word where bits in bytes are swapped
 */
uint32_t bitswap(uint32_t lw);

/* ToDo: load field */

/* ToDo: load field with clear */

/**
 * @}
 */


/** @name CAM intrinsics
 * @{
 */

#if __BIGENDIAN

/** This structure is used to capture the results of a CAM lookup.
 *
 * @see cam_lookup(), cam_read_state().
 */
typedef union
{
    struct
    {
        uint32_t zeros1         : 20;   /**< All zeros.                             */
        uint32_t state          : 4;    /**< CAM entry state.                       */
        uint32_t hit            : 1;    /**< hit (1) or miss (0).                   */
        uint32_t entry_num      : 4;    /**< CAM entry number.                      */
        uint32_t zeros2         : 3;    /**< All zeros.                             */
    };
    uint32_t value;                     /**< Accessor to all fields simultaneously  */
} cam_lookup_t;
#else
/* __LITTLEENDIAN */
typedef union
{
    struct
    {
        uint32_t zeros2         : 3;    /* all zeros */

        uint32_t entry_num      : 4;    /* CAM entry number */
        uint32_t hit            : 1;    /* hit (1) or miss (0) */
        uint32_t state          : 4;    /* CAM entry state */
        uint32_t zeros1         : 20;   /* all zeros */
    };
    uint32_t value;
} cam_lookup_t;

#endif /* __BIGENDIAN */

/** Write an entry in the CAM.
 *
 * @param entry_num CAM entry number to write
 * @param tag       Value to set for this CAM entry
 * @param cam_state State to set for this CAM entry
 *
 * Writes an entry in the CAM specified by the argument index with the value
 * specified by tag, and sets the state to the value specified in the argument state.
 * Argument state must be a constant literal specified directly in the intrinsics
 * argument list. Otherwise, the compiler may have to generate runtime checks for the possible 16
 * values, since the microcode only accepts a constant literal for the state.
 *
 */
void cam_write(
    uint32_t entry_num,             /* CAM entry number */
    uint32_t tag,                   /* tag to write */
    uint32_t cam_state              /* state to write */
);

/** Perform a CAM lookup.
 *
 * @param tag       The value to lookup in the CAM
 *
 * Perform a CAM lookup and return the hit/miss status, state, and entry number as
 * bitfields in the return value. In the event of a miss, the entry value is the
 * LRU (least recently used) entry (which is the suggested entry to replace) and state
 * bits are 0. On a CAM hit, this function has the side effect of marking the CAM entry as
 * MRU (most recently used).
 *
 */
cam_lookup_t cam_lookup(
    uint32_t tag                    /* tag to lookup */
);

/** Set the state for the entry in the CAM.
 *
 * @param entry_num       CAM entry whose state is set
 * @param cam_state       State to set for the CAM entry
 *
 * Set the state for the entry in the CAM specified by the entry_num argument to the value specified
 * in the argument state. Argument state must be a constant literal specified directly in the
 * intrinsics argument list. Otherwise, the compiler may have to generate runtime checks for the
 * possible 16 values, since the microcode only accepts a constant literal for the state.
 *
 */
void cam_write_state(
    uint32_t entry_num,             /* CAM entry number */
    uint32_t cam_state              /* state to write */
);

/** Read the tag associated with the CAM entry.
 *
 * @param entry_num       CAM entry whose tag is returned
 *
 * Read out the tag associated with the CAM entry specified by the entry_num argument.
 *
 */
uint32_t cam_read_tag(
    uint32_t entry_num              /* CAM entry number */
);

/** Read the state associated with the CAM entry.
 *
 * @param entry_num       CAM entry whose state is returned
 *
 * Read out the state associated with the CAM entry specified by the entry_num
 * argument and returns it in the state bitfield of the return value. All other
 * fields of the return value structure are set to 0.
 *
 */
cam_lookup_t cam_read_state(
    uint32_t entry_num              /* CAM entry number */
);

/** Clear all entries in the CAM.
 *
 */
void cam_clear(void);
/**
 * @}
 */


/** @name Multiplication intrinsics
 * @{
 */

/** Multiply 24-bit by 8-bit.
 *
 * @param x     24-bit int32_t to multiply
 * @param y     8-bit int32_t to multiply
 *
 * Returns the result of 24-bit x multiplied by 8-bit y.
 */
uint32_t multiply_24x8(
    uint32_t x,
    uint32_t y
);

/** Multiply 16-bit by 16-bit.
 *
 * @param x     16-bit int32_t to multiply
 * @param y     16-bit int32_t to multiply
 *
 * Returns the result of 16-bit x multiplied by 16-bit y.
 */
uint32_t multiply_16x16(
    uint32_t x,
    uint32_t y
);

/** Multiply 32-bit by 32-bit.
 *
 * @param x     32-bit int32_t to multiply
 * @param y     32-bit int32_t to multiply
 *
 * Returns the lower 32-bit result of 32-bit x multiplied by 32-bit y.
 *
 */
uint32_t multiply_32x32_lo(
    uint32_t x,
    uint32_t y
);

/** Multiply 32-bit by 32-bit.
 *
 * @param x     32-bit int32_t to multiply
 * @param y     32-bit int32_t to multiply
 *
 * Returns the higher 32-bit result of 32-bit x multiplied by 32-bit y.
 *
 */
uint32_t multiply_32x32_hi(
    uint32_t x,
    uint32_t y
);

/** Multiply 32-bit by 32-bit.
 *
 * @param x     32-bit int32_t to multiply
 * @param y     32-bit int32_t to multiply
 *
 * Returns the higher 32-bit result of 32-bit x multiplied by 32-bit y.
 */
uint64_t multiply_32x32(
    uint32_t x,
    uint32_t y
);
/**
 * @}
 */


/** @name Timer intrinsics
 * @{
 */

/** Sets the timestamp for local CSR enum.
 *
 * @param timestamp     Timestamp to set
 *
 * Sets both the local_csr_timestamp_low and local_csr_timestamp_high fields of the local_csr_t enum.
 */
void _set_timestamp(__int64 timestamp);

/** Start timestamp.
 *
 * This function uses local_csr_timestamp_low and timestamp_high to measure time elapse in 16-cycle intervals
 * between start and stop. The _timestamp_start() function returns a handle that is used by the _timestamp_stop()
 * function.
 *
 * @note This function returns a handle that is used by the _timestamp_stop() function.
 *
 */
__int64 _timestamp_start(void);

/** Stop timestamp.
 *
 * @param handle    Timestamp handle returned by the _timestamp_start() function
 *
 * This function returns the time elapse in 16-cycle intervals between start and stop.
 *
 */
__int64 _timestamp_stop(__int64 handle);

/** Sleep for a number of cycles.
 *
 * @param cycles Approximate number of cycles to sleep, resolution is 16 cycles and should be less than (1 << 20).
 *
 * @note The ME timers must be enabled before using the sleep function.
 *
 */
void _sleep(uint32_t cycles);
/**
 * @}
 */


/**
 * @}
 */


/** @addtogroup signal Synchronization Intrinsics
 *  @{
 */

/** Signal the same Microengine, same context.
 *
 * @param sig_no    Signal number to raise
 * @param ctx       Context in which the signal is to be raised
 *
 * Raise the specified signal number in the specified context of the same Microengine.
 * The context "ctx" must be running on the same Microengine.
 *
 */
void signal_same_ME(
    uint32_t sig_no,
    uint32_t ctx
);

/** Signal the same Microengine, next context.
 *
 * @param sig_no    Signal number to raise
 *
 * Raise the specified signal number in the next context number of the same Microengine.
 *
 */
void signal_same_ME_next_ctx(
    uint32_t sig_no
);

/** Signal the previous Microengine.
 *
 * @param sig_no    Signal number to raise
 * @param ctx       Context in which to raise the signal
 *
 * Raise the specified signal number in the specified context of the previous Microengine.
 *
 */
void signal_prev_ME(
    uint32_t sig_no,
    uint32_t ctx
);

/** Signal the previous Microengine, same context.
 *
 * @param sig_no    Signal number to raise
 *
 * Raise the specified signal number in the same context number of the previous Microengine.
 *
 */
void signal_prev_ME_this_ctx(
    uint32_t sig_no
);

/** Signal the next Microengine.
 *
 * @param sig_no    Signal number to raise
 * @param ctx       Context in which to raise the signal
 *
 * Raise the specified signal number in the specified context of the next Microengine.
 *
 */
void signal_next_ME(
    uint32_t sig_no,
    uint32_t ctx
);

/** Signal the next Microengine, same context.
 *
 * @param sig_no    Signal number to raise
 *
 * Raise the specified signal number in the same context number of the next Microengine.
 *
 */
void signal_next_ME_this_ctx(
    uint32_t sig_no
);
/**
 * @}
 */



/** Wait for signal.
 * This is a C macro that serves as a wrapper around the function call ctx_wait(voluntary). The
 * ctx_wait(voluntary) function call voluntarily swaps out the current context. The context is
 * ready to run again immediately.
 */
#define ctx_swap()  ctx_wait(voluntary)

/** Wait for signal.
 *
 * @param sig One of kill, voluntary or bpt.
 *
 * This function swaps out the current context and waits for the specified signal, bpt,
 * voluntary, kill, or no_load (where the signal mask is specified earlier in one of the local csr
 * CTX_WAKEUP_EVENTS). Please use the __wait_for_all(), __wait_for_all_single(), __wait_for_any(), __wait_for_any_single() or
 * signal_test() intrinsics to wait for signal variables.
 */
void ctx_wait(signal_t sig);




/** @addtogroup crc CRC Intrinsics
 * @{
 */

/** CRC bytes specifier.
 *
 * The bytes_specifier_t enumeration is used as an argument to the CRC functions and specifies one or more contiguous
 * bytes within a 32-bit word of big-endian or little-endian data. For example, the bytes_0_3 item in this enumeration
 * refers to bytes 0 through 3. When using the big-endian CRC functions, byte 0 refers to the most significant byte
 * and byte 3 refers to the least significant byte. When using the little-endian CRC functions, byte 0 refers to the least
 * significant byte and byte 3 refers to the most significant byte. This enumeration type specifies the bytes to be used
 * with CRC operations.
 */
typedef enum
{
    bytes_0_3,                  /**< BE: 0, 1, 2, 3      LE: 3, 2, 1, 0. */
    bytes_0_2,                  /**< BE: 0, 1, 2         LE:    2, 1, 0. */
    bytes_0_1,                  /**< BE: 0, 1            LE:       1, 0. */
    byte_0,                     /**< BE: 0               LE:          0. */
    bytes_1_3,                  /**< BE:    1, 2, 3      LE: 3, 2, 1   . */
    bytes_2_3,                  /**< BE:       2, 3      LE: 3, 2      . */
    byte_3                      /**< BE:          3      LE: 3         . */
} bytes_specifier_t;


/** 32-bit iSCSI CRC computation in Little-endian format.
 *
 * @param data      Data on which to perform the 32-bit iSCSI CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 32-bit iSCSI CRC computation on specified bytes in the data argument that
 * is assumed to be in Little-endian format and return the unmodified value of data.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
__intrinsic
uint32_t crc_iscsi_le(
    uint32_t data,
    bytes_specifier_t bspec
);


/** 32-bit iSCSI CRC computation in Big-endian format.
 *
 * @param data      Data on which to perform the 32-bit iSCSI CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 32-bit iSCSI CRC computation on specified bytes in the data argument that
 * is assumed to be in Big-endian format and return the unmodified value of data.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
__intrinsic
uint32_t crc_iscsi_be(
    uint32_t data,
    bytes_specifier_t bspec
);


/** CRC 10 computation in Little-endian format.
 *
 * @param data      Data on which to perform the CRC 10 computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a CRC-10 computation on specified bytes of the data argument that are
 * assumed to be in Little-endian format and return the unmodified value of data.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
__intrinsic
uint32_t crc_10_le(
    uint32_t data,
    bytes_specifier_t bspec
);


/** CRC 10 computation in Big-endian format.
 *
 * @param data      Data on which to perform the CRC 10 computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a CRC-10 computation on specified bytes of the data argument that are
 * assumed to be in Big-endian format and return the unmodified value of data.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
__intrinsic
uint32_t crc_10_be(
    uint32_t data,
    bytes_specifier_t bspec
);


/** CRC 5 computation in Little-endian format.
 *
 * @param data      Data on which to perform the CRC 5 computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a CRC-5 computation on specified bytes of the data argument that are
 * assumed to be in Little-endian format and return the unmodified value of data.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
__intrinsic
uint32_t crc_5_le(
    uint32_t data,
    bytes_specifier_t bspec
);


/** CRC 5 computation in Big-endian format.
 *
 * @param data      Data on which to perform the CRC 5 computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a CRC-5 computation on specified bytes of the data argument that are
 * assumed to be in Big-endian format and return the unmodified value of data.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
__intrinsic
uint32_t crc_5_be(
    uint32_t data,
    bytes_specifier_t bspec
);


/** 32-bit iSCSI CRC computation and bit swap in Little-endian format.
 *
 * @param data      Data on which to perform the 32-bit iSCSI CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 32-bit iSCSI CRC computation on specified bytes of the data argument that are
 * assumed to be in Little-endian format and return the unmodified value of data.
 * The bits in each byte are swapped before the computation begins.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
uint32_t crc_iscsi_le_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);


/** 32-bit iSCSI CRC computation and bit swap in Big-endian format.
 *
 * @param data      Data on which to perform the 32-bit iSCSI CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 32-bit iSCSI CRC computation on specified bytes of the data argument that are
 * assumed to be in Big-endian format and return the unmodified value of data.
 * The bits in each byte are swapped before the computation begins.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
uint32_t crc_iscsi_be_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);


/** CRC 10 computation and bit swap in Little-endian format.
 *
 * @param data      Data on which to perform the CRC 10 computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a CRC-10 computation on specified bytes of the data argument that are
 * assumed to be in Little-endian format and return the unmodified value of data.
 * The bits in each byte are swapped before the computation begins.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
uint32_t crc_10_le_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);


/** CRC 10 computation and bit swap in Big-endian format.
 *
 * @param data      Data on which to perform the CRC 10 computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a CRC-10 computation on specified bytes of the data argument that are
 * assumed to be in Big-endian format and return the unmodified value of data.
 * The bits in each byte are swapped before the computation begins.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
uint32_t crc_10_be_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);


/** CRC 5 computation and bit swap in Little-endian format.
 *
 * @param data      Data on which to perform the CRC 5 computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a CRC-5 computation on specified bytes of the data argument that are
 * assumed to be in Little-endian format and return the unmodified value of data.
 * The bits in each byte are swapped before the computation begins.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
uint32_t crc_5_le_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);



/** CRC 5 computation and bit swap in Big-endian format.
 *
 * @param data      Data on which to perform the CRC 5 computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a CRC-5 computation on specified bytes of the data argument that are
 * assumed to be in Big-endian format and return the unmodified value of data.
 * The bits in each byte are swapped before the computation begins.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
uint32_t crc_5_be_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);


/** Pop the number of "1" bits.
 *
 * @param data  Value on which to perform the operation
 *
 * This function returns the number of "1" bits in the given value "data".
 *
 */
uint32_t pop_count(
    uint32_t data
);


/** Rotate right a 32-bit word by specified number of bits.
 *
 * @param value     The 32-bit value to rotate to the right
 * @param shift     The number of bits to rotate to the right (1 - 31).
 *
 * @return         The 32-bit result of the value shifted to the right by "shift" bits.
 */
__intrinsic
uint32_t rotr(uint32_t value, int32_t shift);

/** Rotate left a 32-bit word by specified number of bits.
 *
 * @param value     The 32-bit value to rotate to the left
 * @param shift     The number of bits to rotate to the left (1 - 31).
 *
 * @return         The 32-bit result of the value shifted to the left by "shift" bits.
 */
__intrinsic
uint32_t rotl(uint32_t value, int32_t shift);


/** 16-bit CCITT CRC computation in Little-endian format.
 *
 * @param data      Data on which to perform the 16-bit CCITT CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 16-bit CCITT CRC computation on specified bytes in the data argument that
 * is assumed to be in Little-endian format and return the unmodified value of data.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
__intrinsic
uint32_t crc_16_le(
    uint32_t data,
    bytes_specifier_t bspec
);


/** 16-bit CCITT CRC computation in Big-endian format.
 *
 * @param data      Data on which to perform the 16-bit CCITT CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 16-bit CCITT CRC computation on specified bytes in the data argument that
 * is assumed to be in Big-endian format and return the unmodified value of data.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
__intrinsic
uint32_t crc_16_be(
    uint32_t data,
    bytes_specifier_t bspec
);


/** 32-bit iSCSI CRC computation in Little-endian format.
 *
 * @param data      Data on which to perform the 32-bit iSCSI CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a CRC-32 computation on specified bytes in the data argument that is
 * assumed to be in Little-endian format and return the unmodified value of data.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
__intrinsic
uint32_t crc_32_le(
    uint32_t data,
    bytes_specifier_t bspec
);


/** 32-bit iSCSI CRC computation in Big-endian format.
 *
 * @param data      Data on which to perform the 32-bit iSCSI CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a CRC-32 computation on specified bytes in the data argument that is
 * assumed to be in Big-endian format and return the unmodified value of data.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
__intrinsic
uint32_t crc_32_be(
    uint32_t data,
    bytes_specifier_t bspec
);


/** 16-bit CCITT CRC computation in Little-endian format.
 *
 * @param data      Data on which to perform the 16-bit CCITT CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 16-bit CCITT CRC computation on specified bytes in the data argument that
 * is assumed to be in Little-endian format and return the unmodified value of data.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
__intrinsic
uint32_t crc_ccitt_le(
    uint32_t data,
    bytes_specifier_t bspec
);


/** 16-bit CCITT CRC computation in Big-endian format.
 *
 * @param data      Data on which to perform the 16-bit CCITT CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 16-bit CCITT CRC computation on specified bytes in the data argument that
 * is assumed to be in Big-endian format and return the unmodified value of data.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
__intrinsic
uint32_t crc_ccitt_be(
    uint32_t data,
    bytes_specifier_t bspec
);


/* crc_none little-endian */

/** @cond */
uint32_t crc_none_le(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);

/* crc_none big-endian */

uint32_t crc_none_be(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);
/** @endcond */


/** 16-bit CCITT CRC computation and bit swap in Little-endian format.
 *
 * @param data      Data on which to perform the 16-bit CCITT CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 16-bit CCITT CRC computation on specified bytes of the data argument that are
 * assumed to be in Little-endian format and return the unmodified value of data.
 * The bits in each byte are swapped before the computation begins.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
uint32_t crc_16_le_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);


/** 16-bit CCITT CRC computation and bit swap in Big-endian format.
 *
 * @param data      Data on which to perform the 16-bit CCITT CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 16-bit CCITT CRC computation on specified bytes of the data argument that are
 * assumed to be in Big-endian format and return the unmodified value of data.
 * The bits in each byte are swapped before the computation begins.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
uint32_t crc_16_be_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);


/** 32-bit iSCSI CRC computation and bit swap in Little-endian format.
 *
 * @param data      Data on which to perform the 32-bit iSCSI CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 32-bit iSCSI CRC computation on specified bytes of the data argument that are
 * assumed to be in Little-endian format and return the unmodified value of data.
 * The bits in each byte are swapped before the computation begins.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
uint32_t crc_32_le_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);


/** 32-bit iSCSI CRC computation and bit swap in Big-endian format.
 *
 * @param data      Data on which to perform the 32-bit iSCSI CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 32-bit iSCSI CRC computation on specified bytes of the data argument that are
 * assumed to be in Big-endian format and return the unmodified value of data.
 * The bits in each byte are swapped before the computation begins.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
uint32_t crc_32_be_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);


/** 16-bit CCITT CRC computation and bit swap in Little-endian format.
 *
 * @param data      Data on which to perform the 16-bit CCITT CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 16-bit CCITT CRC computation on specified bytes of the data argument that are
 * assumed to be in Little-endian format and return the unmodified value of data.
 * The bits in each byte are swapped before the computation begins.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
uint32_t crc_ccitt_le_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);


/** 16-bit CCITT CRC computation and bit swap in Big-endian format.
 *
 * @param data      Data on which to perform the 16-bit CCITT CRC computation
 * @param bspec     Specified bytes in the data argument on which to perform the computation
 *
 * Perform a 16-bit CCITT CRC computation on specified bytes of the data argument that are
 * assumed to be in Big-endian format and return the unmodified value of data.
 * The bits in each byte are swapped before the computation begins.
 *
 * crc_write() can be used to initialize the CRC value and the result can be obtained from crc_read().
 * @see crc_read() and crc_write()
 */
uint32_t crc_ccitt_be_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);


/* crc_none little-endian, bit_swap */
/** @cond */
uint32_t crc_none_le_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);

/* crc_none big-endian, bit_swap */

uint32_t crc_none_be_bit_swap(
    uint32_t data,
    bytes_specifier_t bspec             /* byte specifier */
);
/** @endcond */


/** Read CRC remainder accumulated so far.
 *
 */
uint32_t crc_read(void);


/** Write the CRC remainder.
 *
 * @param residue   Value to initialize the CRC remainder
 *
 */
void crc_write(
    uint32_t residue
);

/**
 * @}
 */








/*
 * Lightweight runtime asserts for intrinsic functions - the context is suspended
 * and mailbox1 contains the address where the problem occurred. mailbox 0
 * is set to zero.
 *
 * Excluding the condition check the assert uses about 4 instructions. It does
 * not use any SRAM as assert() in util.c as it only records the program
 * counter. The list file can be used to determine the location in source
 * code where the assert failed.
 *
 * Run time asserts are only done when _INTRINSIC_RT_ASSERT is defined
 */
#ifdef _INTRINSIC_RT_ASSERT
__noinline
__intrinsic
void ___rt_assert(void* addr)
{
    local_csr_write(local_csr_mailbox1,(uint32_t)addr);
    local_csr_write(local_csr_mailbox0,1);
    __asm ctx_arb[bpt]
}

/*
 * Needs to be intrinsic to use from intrinsics
 */
__intrinsic
__forceinline
void __rt_assert(void)
{
    __gpr uint32_t reg;
    __asm
    {
        load_addr[reg,label]
        label:
    }
    ___rt_assert((void*)reg);
}


/* RT_ALIGN_ASSERT is used for pointer alignment asserts */
#define RT_ALIGN_ASSERT(exp) RT_ASSERT(exp)

/* RT_RANGE_ASSERT is used for value and count asserts */
#define RT_RANGE_ASSERT(exp) RT_ASSERT(exp)

/* RT_ASSERT is for asserts not falling in the above two categories */
#define RT_ASSERT(exp) (void)( (exp) || (__rt_assert(),0) )

#else /* ! _INTRINSIC_RT_ASSERT */

#define RT_ALIGN_ASSERT(exp)
#define RT_RANGE_ASSERT(exp)
#define RT_ASSERT(exp)

#endif

#endif /* __NFP_INTRINSIC_H__ */
