/*
 * Copyright (C) 2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __CAM_SHARING_UC__
#define __CAM_SHARING_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <constants.uc>
#include <stdmac.uc>

/** @file cam_sharing.uc Microengine CAM Sharing Operation Macros
 * @addtogroup me_cam_share Microengine CAM Sharing Operation
 * @{
 *
 * @name Microengine CAM Sharing Operation Macros
 * @{
 *
 * This API supports:
 *  - CAM sharing across different microblocks running on same ME.
 *  - The original LRU CAM implementation (without CAM sharing).
 *
 * To enable CAM sharing support, define @b @c CAM_SHARED before including this
 * file.
 *
 * For most macros a virtual parameter,
 * called the <B>CAM handle</B>, is used to
 * refer to a specific group of macro parameters. It is composed of:
 * @arg @b BID      Unique Microblock Block ID. 4 bits by default.
 *                  @see BID_BIT_SZ
 * @arg @b COUNT    Number of CAM entries. 4 bits.
 * @arg @b FIRST    Number of first CAM entry. 4 bits.
 *
 * For example:
 * @code
 * #define RX_CAM_HANDLE   1, 8, 0
 * @endcode
 *
 * However, the <B>CAM handle</B> is not used if CAM sharing is not enabled.
 * The relevant macro parameters must still be passed though.
 *
 * This section creates the following global absolute registers:
 * @code
 * @camsharing_bit_vector
 * @camsharing_ref_cnt1
 * @camsharing_ref_cnt2
 * @endcode
 *
 * @cond INTERNAL_DEFINES
 * @arg @c camsharing_bit_vector The bit vector is used to find the
 *                               free CAM entry for the microblock.
 * @arg @c camsharing_ref_cnt1 Holds reference count for cam entries 0 through 7.
 * @arg @c camsharing_ref_cnt2 Holds reference count for cam entries 8 through 15.
 *
 * Each cam entry has 4 bits assigned for reference count.
 * By using registers instead of Local Memory to keep reference counts,
 * CAM usage (shared or not shared) becomes transparent to the user.
 * Otherwise for reference counts in LM case, the user will be required to
 * pass LM handle, LM base. The number of instructions are almost comparable
 * in both the cases.
 * @endcond
 */

// API Quick Reference:
//
// cam_init()
// cam_entry_read_state(out_state_val, in_entry_num, in_bid, in_num_entries, \
//      in_first_entry)
// cam_entry_write_state(in_entry_num, in_state_val, in_bid, in_num_entries, \
//      in_first_entry)
// cam_entry_read_tag(out_tag, in_entry_num, in_bid, in_num_entries, \
//      in_first_entry)
// cam_clearall(in_bid, in_num_entries, in_first_entry)
// cam_entry_write(in_entry_num, in_key, in_state_val, in_bid, in_num_entries, \
//      in_first_entry)
// cam_exit_using_entry(in_entry_num, in_bid, in_num_entries, \
//      in_first_entry)
// cam_entry_lookup(out_lookup_result, in_key, in_bid, in_num_entries, \
//      in_first_entry)
// cam_entry_lookup_with_lm(out_lookup_result, in_key, in_lm_handle,
//      in_lm_start_addr, in_bid, in_num_entries, in_first_entry)



#ifdef CAM_SHARED

    #ifndef SRC_KEY_BIT_SZ
        /** Bit size of the key part of the CAM tag. This plus @ref BID_BIT_SZ must
         * not exceed 32 bits.
         */
        #define SRC_KEY_BIT_SZ      28
    #endif
    #ifndef BID_BIT_SZ
        /** Bit size of the block ID part of the CAM tag.
         * This plus @ref SRC_KEY_BIT_SZ must not exceed 32 bits.
         */
        #define BID_BIT_SZ          4
    #endif

    #if ( (SRC_KEY_BIT_SZ + BID_BIT_SZ) > 32)
        #error "CAM sharing: (SRC_KEY_BIT_SZ + BID_BIT_SZ) > 32"
    #endif
    // Bit vector for keeping track of free cam entries per microblock.
    // Each bit represents one CAM entry. The CAM enty is considered free
    // when the bit is set.
    .reg @camsharing_bit_vector

    // Registers to hold reference count for all 16 CAM entries.
    .reg @camsharing_ref_cnt1
    .reg @camsharing_ref_cnt2

#endif


/** Initializes CAM variables and clears all CAM entries, must be called before
 *  using any other macros of this API.
 *
 * It is assumed that after calling this macro all CAM entries will
 * be initialized to unique values prior to doing CAM lookups.
 */
#macro cam_init()
.begin

#ifdef CAM_SHARED

    // Initialize all 16 cam entries as free.
    immed32(@camsharing_bit_vector, 0xffff)

    // Initialize cam entries ref_cnt to zero.
    alu[@camsharing_ref_cnt1, --, B, 0x0]
    alu[@camsharing_ref_cnt2, --, B, 0x0]

#endif

    // Clear all CAM entries (its also clears tags and state value bits)
    cam_clear

.end
#endm


/** Reads the state value for the CAM entry (the block specific entry
 *  if CAM is shared).
 *
 * @param out_state_val     The state value of the CAM entry is placed in bits
 *                          11 to 8, other bits are set to 0.
 * @param in_entry_num      CAM entry number. May not exceed the total number of
 *                          CAM entries. It is not range checked.
 * @param in_bid            Microblock Block ID.
 * @param in_num_entries    Number of CAM entries for this microblock.
 * @param in_first_entry    First CAM entry number of this microblock.
 *
 * <b>Example:</b>
 * @code
 * cam_entry_read_state(cam_state, cam_entry, RX_CAM_HANDLE)
 * @endcode
 *
 */
#macro cam_entry_read_state(out_state_val, in_entry_num, in_bid, in_num_entries, \
                            in_first_entry)
.begin

#ifdef CAM_SHARED

    .reg abs_entry_num // absolute cam entry number [0 thru 15]

    // Calculate the absolute entry number in the range of 0 thru 15 from the
    // microblock specific entry number.
    __cam_calc_abs_entry(abs_entry_num, in_entry_num, in_first_entry)

    // Read the state
    cam_read_state[out_state_val, abs_entry_num]

#else

    // CAM not shared.

    // Read the state
    cam_read_state[out_state_val,  in_entry_num]

#endif

.end
#endm


/** Writes the state value for the CAM entry (the block specific entry
 *  if CAM is shared).
 *
 * @param in_entry_num      CAM entry number. May not exceed the total number of
 *                          CAM entries. It is not range checked.
 * @param in_state_val      The state value to write to the CAM entry.
 * @param in_bid            Microblock Block ID.
 * @param in_num_entries    Number of CAM entries for this microblock.
 * @param in_first_entry    First CAM entry number of this microblock.
 *
 * <b>Example:</b>
 * @code
 * cam_entry_write_state(cam_entry, CAM_STATE_VALID, RX_CAM_HANDLE)
 * @endcode
 *
 */
#macro cam_entry_write_state(in_entry_num, in_state_val, in_bid, in_num_entries, \
                             in_first_entry)
.begin

#ifdef CAM_SHARED

    .reg abs_entry_num // absolute cam entry number [0 thru 15]

    // Calculate the absolute entry number in the range of 0 thru 15 from the
    // microblock specific entry number.
    __cam_calc_abs_entry(abs_entry_num, in_entry_num, in_first_entry)

    // Write the state
    cam_write_state[abs_entry_num, in_state_val]

#else

    // CAM not shared.

    // Write the state
    cam_write_state[in_entry_num, in_state_val]

#endif

.end
#endm


/** Reads the tag for the CAM entry (the block specific entry
 *  if CAM is shared).
 *
 * @param out_tag           The tag of the CAM entry is returned here.
 * @param in_entry_num      CAM entry number. May not exceed the total number of
 *                          CAM entries. It is not range checked.
 * @param in_bid            Microblock Block ID.
 * @param in_num_entries    Number of CAM entries for this microblock.
 * @param in_first_entry    First CAM entry number of this microblock.
 *
 * <b>Example:</b>
 * @code
 * cam_entry_read_tag(cam_tag, cam_entry, RX_CAM_HANDLE)
 * @endcode
 *
 */
#macro cam_entry_read_tag(out_tag, in_entry_num, in_bid, in_num_entries, \
                          in_first_entry)
.begin

#ifdef CAM_SHARED

    .reg abs_entry_num // absolute cam entry number [0 thru 15]

    // Calculate the absolute entry number in the range of 0 thru 15 from the
    // microblock specific entry number.
    __cam_calc_abs_entry(abs_entry_num, in_entry_num, in_first_entry)

    // Read the cam tag
    cam_read_tag[out_tag, abs_entry_num]

    // Mask the upper bits equal to BID_BIT_SZ of out_tag

#define_eval BID_MASK  ( (1 << BID_BIT_SZ) - 1)

    alu_shf[out_tag, out_tag, AND~, BID_MASK, <<SRC_KEY_BIT_SZ]

#undef BID_MASK



#else

    // CAM not shared

    // Read the cam tag
    cam_read_tag[out_tag, in_entry_num]

#endif

.end
#endm


/** In CAM sharing mode, clear CAM entries for the specified microblock,
 *  otherwise clear all CAM entries.
 *
 * @param in_bid            Microblock Block ID.
 * @param in_num_entries    Number of CAM entries for this microblock.
 * @param in_first_entry    First CAM entry number of this microblock.
 *
 * @note
 * In CAM sharing mode the CAM tags and state bits of entries are not cleared.
 */
#macro cam_clearall(in_bid, in_num_entries, in_first_entry)
.begin

#ifdef CAM_SHARED

    .reg bv_mask // used to mask off other bits and keep just
                        // microblock specific bit vector.

    // Clear the CAM entries for the specific microblock.
    // (ie set the bits of @camsharing_bit_vector)

#define_eval BIT_MASK   ( ( (1 << in_num_entries) - 1 )  << in_first_entry)

    immed32(bv_mask, BIT_MASK)
    alu[@camsharing_bit_vector, @camsharing_bit_vector, OR, bv_mask]

#undef BIT_MASK

#else

    // CAM not shared

    // Clear all CAM entries
    cam_clear

#endif

.end
#endm


/** Write the tag for the specified CAM entry of the specific microblock.
 *
 * @param in_entry_num      CAM entry number. May not exceed the total number of
 *                          CAM entries. It is not range checked.
 * @param in_key            Source key with which CAM lookup was done.
 * @param in_state_val      State value of CAM entry.
 * @param in_bid            Microblock Block ID.
 * @param in_num_entries    Number of CAM entries for this microblock.
 * @param in_first_entry    First CAM entry number of this microblock.
 *
 * <b>Example:</b>
 * @code
 * cam_entry_write(cam_entry, src_key, CAM_STATE_VALID, RX_CAM_HANDLE)
 * @endcode
 */
#macro cam_entry_write(in_entry_num, in_key, in_state_val, in_bid, in_num_entries, \
                       in_first_entry)
.begin

#ifdef CAM_SHARED

    .reg cam_tag // unique cam_tag (bid + key)
    .reg abs_entry_num // absolute cam entry number [0 thru 15]

    // Create unique cam tag with the combination of bid and key.
    __cam_create_tag(cam_tag, in_key, in_bid, in_num_entries, in_first_entry)

    // Calculate the absolute entry number in the range of 0 thru 15 from the
    // microblock specific entry number.
    __cam_calc_abs_entry(abs_entry_num, in_entry_num, in_first_entry)

    cam_write[abs_entry_num, cam_tag, in_state_val]

#else

    // CAM not shared

    // Write the tag and state for the cam entry
    cam_write[in_entry_num, in_key, in_state_val]


#endif

.end
#endm


/** Decrement the reference count for the specified CAM entry to indicate that
 *  a thread is done with the entry.
 *
 * When the reference count is zero, the entry is marked as free.
 *
 * @param in_entry_num      CAM entry number. May not exceed the total number of
 *                          CAM entries. It is not range checked.
 * @param in_bid            Microblock Block ID.
 * @param in_num_entries    Number of CAM entries for this microblock.
 * @param in_first_entry    First CAM entry number of this microblock.
 *
 */
#macro cam_exit_using_entry(in_entry_num, in_bid, in_num_entries, \
                            in_first_entry)
.begin

#ifdef CAM_SHARED

    .reg abs_entry_num  // absolute cam entry number [0 thru 15]
    .reg ref_cnt_pos    // the bit position of reference count for entry
    .reg ref_cnt        // reference count for cam entry
    .reg one_shifted    // 1 left-shifted by ref_cnt_pos
    .reg temp           // temp variable

    // Calculate the absolute entry number in the range of 0 thru 15 from the
    // microblock specific entry number.
    __cam_calc_abs_entry(abs_entry_num, in_entry_num, in_first_entry)

    // Check which @reg to update ref_cnt.
    br_bset[abs_entry_num, 3, ENTRY_GT_8#], defer[1]
        // Calculate the ref_cnt position
        alu[ref_cnt_pos, 0x1C, AND, abs_entry_num, <<2]

    // Use @camsharing_ref_cnt1 (for entries 0 thru 7)

    // Check if ref_cnt is zero
    alu[temp, ref_cnt_pos, B, @camsharing_ref_cnt1]
    alu[ref_cnt, 0xf, AND, temp, >>indirect]
    alu[ref_cnt, ref_cnt, -, 1]
    bne[END#], defer[3]
        // Decrement the ref_cnt
        // Left-shift 1 acc to ref_cnt_pos and subtract it from @camsharing_ref_cnt1.
        alu[temp, ref_cnt_pos, B, @camsharing_ref_cnt1]
        alu[one_shifted, --, B, 1, <<indirect]
        alu[@camsharing_ref_cnt1, temp, -, one_shifted]


    // Here CAM Entry is free.

    // Mark the entry as free (set the bit) in @camsharing_bit_vector.
    alu[temp, abs_entry_num, B, @camsharing_bit_vector]
    alu_shf[temp, temp, OR, 1, <<indirect]

    br[END#],defer[1]
        alu[@camsharing_bit_vector, --, B, temp]

ENTRY_GT_8#:

    // Use @camsharing_ref_cnt2 (for entries 8 thru 15)

    // Check if ref_cnt is zero
    alu[temp, ref_cnt_pos, B, @camsharing_ref_cnt2]
    alu[ref_cnt, 0xf, AND, temp, >>indirect]
    alu[ref_cnt, ref_cnt, -, 1]
    bne[END#], defer[3]
        // Decrement the ref_cnt
        // Left-shift 1 acc to ref_cnt_pos and subtract it from @camsharing_ref_cnt2.
        alu[temp, ref_cnt_pos, B, @camsharing_ref_cnt2]
        alu[one_shifted, --, B, 1, <<indirect]
        alu[@camsharing_ref_cnt2, temp, -, one_shifted]

    // Here CAM Entry is free.

    // Mark the entry as free (set the bit) in @camsharing_bit_vector.
    alu[temp, abs_entry_num, B, @camsharing_bit_vector]
    alu_shf[temp, temp, OR, 1, <<indirect]
    alu[@camsharing_bit_vector, --, B, temp]

END#:

#else

    // nothing to do

#endif

.end
#endm


/** Perform a CAM lookup and increment the reference count in CAM sharing mode.
 *
 * The following algorithm is used to increment the reference count:
 * @verbatim
 Say the @camsharing_ref_cnt1 has:        0011 ...  0100 0011 0011 0001
 To increment ref_cnt for entry 1, add:   0000 ...  0000 0000 0001 0000
 Result:                                  0011 ...  0100 0011 0100 0001 @endverbatim
 *
 * @param out_lookup_result The lookup result is returned here. For CAM sharing
 *                          mode the result has tje block specific entry number.
 * @param in_key            Source key with which CAM lookup should be done.
 * @param in_bid            Microblock Block ID.
 * @param in_num_entries    Number of CAM entries for this microblock.
 * @param in_first_entry    First CAM entry number of this microblock.
 *
 * <b>Example:</b>
 * @code
 * cam_entry_lookup(lookup_result, src_key, RX_CAM_HANDLE)
 * @endcode
 */
#macro cam_entry_lookup(out_lookup_result, in_key, in_bid, in_num_entries, \
                        in_first_entry)
.begin

#ifdef CAM_SHARED

    // Miss and Hit code paths are separated out to fill the defer slots effectively.

    // Steps for CAM Miss
    //--------------------
    //   a) Get microblock bit_vector and do ffs to get free entry within the block
    //   b) Update the lookup result with block specific entry number.
    //   c) Increment ref_cnt by 1
    //   d) Clear entry bit in @camsharing_bit_vector

    // Steps for CAM Hit
    //--------------------
    // Two cases to handle here
    //  1) - CAM entry is free
    //  2) - CAM entry is being used
    //
    // Common steps for both cases
    //  a) Update the lookup result with block specific entry number
    //  b) Increment ref_cont by 1
    //
    // Case (1) requires 1 more thing
    //  c) Clear entry bit in @camsharing_bit_vector
    // But here we have combined the case(1) and case(2)

    .reg cam_tag // unique cam_tag (bid + key)

    // Create unique cam tag with the combination of bid and key.
    __cam_create_tag(cam_tag, in_key, in_bid, in_num_entries, in_first_entry)

    // Do CAM lookup based on cam_tag.
    cam_lookup[out_lookup_result, cam_tag]

    // Check for CAM Hit/Miss
    br_bset[out_lookup_result, 7, CAM_HIT#]

CAM_MISS#:

    .begin

    .reg bit_vector     // microblock bit vector
    .reg abs_entry_num  // absolute cam entry number [0 thru 15]
    .reg blk_entry_num  // block specific CAM entry number
    .reg ref_cnt_pos    // the bit position of reference count for entry
    .reg one_shifted    // 1 left-shifted by ref_cnt_pos
    .reg temp           // temp variable
    .reg temp2          // temp2 variable


    // Get the microblock bitvector and zero out rest of the bits from
    // camsharing_bit_vector.
#define_eval BIT_MASK ( ( (1 << in_num_entries) - 1 )  << in_first_entry)

    immed32(bit_vector, BIT_MASK)
    alu[bit_vector, @camsharing_bit_vector, AND, bit_vector]

#undef BIT_MASK

    // Get the first free entry in the microblock (here absolute entry num)
    // We will always have atleast one entry free, if
    // num of cam entries assigned to  microblock >= num of block threads.
    // But in any case check is provided.
    ffs[abs_entry_num, bit_vector]
    beq[NO_FREE_ENTRY#]


INCR_REF_CNT#:

    // Check which @reg to update for ref_cnt increment.
    br_bset[abs_entry_num, 3, ENTRY_GT_8#], defer[3]
        // Calculate the ref_cnt position
        alu[ref_cnt_pos, 0x1C, AND, abs_entry_num, <<2]
        // Update lookup result with miss bit(value 0), block specific cam entry.
        // out_lookup_result = state(11:8) + miss/hit (7) + entry(6:3)
        __cam_calc_blk_entry(blk_entry_num, abs_entry_num, in_first_entry)
        ld_field[out_lookup_result, 0001, blk_entry_num, <<3]

    // Use @camsharing_ref_cnt1 (for entries 0 thru 7)

    // Increment the ref_cnt.
    // Left-shift 1 acc to ref_cnt_pos and add it to @camsharing_ref_cnt1. (See below)
    alu[temp, ref_cnt_pos, B, @camsharing_ref_cnt1]
    alu[one_shifted, --, B, 1, <<indirect]

    // Mark the entry as used (i.e. clear the bit) in @camsharing_bit_vector
    alu[temp2, abs_entry_num, B, @camsharing_bit_vector]
    alu_shf[temp2,  temp2, AND~, 1, <<indirect]

    br[END#],defer[2]
        alu[@camsharing_bit_vector, --, b, temp2]
        alu[@camsharing_ref_cnt1, temp, +, one_shifted]



ENTRY_GT_8#:

    // Use @camsharing_ref_cnt2 (for entries 8 thru 15)

    // Increment the ref_cnt.
    // Left-shift 1 acc to ref_cnt_pos and add it to @camsharing_ref_cnt2. (See below)
    alu[temp, ref_cnt_pos, B, @camsharing_ref_cnt2]
    alu[one_shifted, --, B, 1, <<indirect]

    // Mark the entry as used (i.e. clear the bit) in @camsharing_bit_vector
    alu[temp2, abs_entry_num, B, @camsharing_bit_vector]
    alu_shf[temp2,  temp2, AND~, 1, <<indirect]

    br[END#], defer[2]
        alu[@camsharing_bit_vector, --, b, temp2]
        alu[@camsharing_ref_cnt2, temp, +, one_shifted]


NO_FREE_ENTRY#:

    // Ideally this wont happen if num of cam entries >= num of block threads.

    // Here, you dont have any CAM entry free for block, so first entry of
    // the block is returned. Hence LRU implementation is messed up.
    // So user should take care that above condition is satisfied.

    br[INCR_REF_CNT#], defer[2]
        alu[temp, --, B, 0x0]
        __cam_calc_abs_entry(abs_entry_num, temp, in_first_entry)

    .end // end of CAM Miss

CAM_HIT#:

    // CAM HIT

    .begin

    .reg abs_entry_num  // absolute cam entry number [0 thru 15]
    .reg ref_cnt_pos    // the bit position of reference count for entry
    .reg one_shifted    // 1 left-shifted by ref_cnt_pos
    .reg temp           // temp variable


    // Get the absolute entry number from the lookup result ([0 thru 15])
    alu[abs_entry_num, 0xf, AND, out_lookup_result, >>3]

    // Check which @reg to increment the ref_cnt.
    br_bset[abs_entry_num, 3, ENTRY_GT_8_2#], defer[3]
        // Calculate the ref_cnt position
        alu[ref_cnt_pos, 0x1C, AND, abs_entry_num, <<2]

        // Load block specific entry num & hit-bit in lookup_result.(see also below)
        __cam_calc_blk_entry(temp, abs_entry_num, in_first_entry)
        ld_field[out_lookup_result, 0001, temp, <<3]

    // Use @camsharing_ref_cnt1 (for entries 0 thru 7)

    // Increment the ref_cnt.
    // Left-shift 1 acc to ref_cnt_pos and add it to @camsharing_ref_cnt1.
    alu[temp, ref_cnt_pos, B, @camsharing_ref_cnt1]
    alu[one_shifted, --, B, 1, <<indirect]

    br[UPD_ENTRY#],defer[2]
        alu[@camsharing_ref_cnt1, temp, +, one_shifted]
        alu_shf[out_lookup_result, out_lookup_result, OR, 1, <<7] // Set hit bit

ENTRY_GT_8_2#:

    // Use @camsharing_ref_cnt2 (for entries 8 thru 15)

    // Increment the ref_cnt.
    // Left-shift 1 acc to ref_cnt_pos and add it to @camsharing_ref_cnt2.
    alu[temp, ref_cnt_pos, B, @camsharing_ref_cnt2]
    alu[one_shifted, --, B, 1, <<indirect]
    alu[@camsharing_ref_cnt2, temp, +, one_shifted]

    alu_shf[out_lookup_result, out_lookup_result, OR, 1, <<7] // Set hit bit

UPD_ENTRY#:


    // Mark the entry as used (clear the bit) in @camsharing_bit_vector
    alu[temp, abs_entry_num, B, @camsharing_bit_vector]
    alu_shf[temp, temp, AND~, 1, <<indirect]
    alu[@camsharing_bit_vector, --, B, temp]

    .end

END#:

#else

    // CAM not shared

    cam_lookup[out_lookup_result, in_key]

#endif

.end
#endm


/** Perform a CAM lookup, increment the reference count in CAM sharing mode and
 *  set the given LM handle to point to corresponding data in Local Memory.
 *
 * The following algorithm is used to increment the reference count:
 * @verbatim
 Say the @camsharing_ref_cnt1 has:        0011 ...  0100 0011 0011 0001
 To increment ref_cnt for entry 1, add:   0000 ...  0000 0000 0001 0000
 Result:                                  0011 ...  0100 0011 0100 0001 @endverbatim
 *
 * @param out_lookup_result The lookup result is returned here. For CAM sharing
 *                          mode the result has tje block specific entry number.
 * @param in_lm_handle      LM handle 0 or 1 which will be set to point to
 *                          the data structure in LM according to the lookup result.
 * @param in_lm_start_addr  2-bit LM address where the data structure is located.
 *                          ( 0, 1, 2, 3  represents 0, 1024, 2048, 3072 bytes in LM)
 * @param in_key            Source key with which CAM lookup should be done.
 * @param in_bid            Microblock Block ID.
 * @param in_num_entries    Number of CAM entries for this microblock.
 * @param in_first_entry    First CAM entry number of this microblock.
 *
 * <b>Example:</b>
 * @code
 * cam_entry_lookup_with_lm(lookup_result, src_key, RXC_LM_HANDLE, RXC_INFO_LM_BASE, \
 *     RX_CAM_HANDLE)
 * @endcode
 */
#macro cam_entry_lookup_with_lm(out_lookup_result, in_key, in_lm_handle, in_lm_start_addr, \
                                in_bid, in_num_entries, in_first_entry)
.begin

#ifdef CAM_SHARED

    // Miss and Hit code paths are separated out to fill the defer slots effectively.

    // Steps for CAM Miss
    //--------------------
    //   a) Get microblock bit_vector and do ffs to get free entry within the block
    //   b) Update the lookup result with block specific entry number.
    //   c) Increment ref_cnt by 1
    //   d) Clear entry bit in @camsharing_bit_vector

    // Steps for CAM Hit
    //--------------------
    // Two cases to handle here
    //  1) - CAM entry is free
    //  2) - CAM entry is being used
    //
    // Common steps for both cases
    //  a) Update the lookup result with block specific entry number
    //  b) Increment ref_cont by 1
    //
    // Case (1) requires 1 more thing
    //  c) Clear entry bit in @camsharing_bit_vector
    // But here we have combined the case(1) and case(2)


    .reg cam_tag // unique cam_tag (bid + key)

    // Create unique cam tag with the combination of bid and key.
    __cam_create_tag(cam_tag, in_key, in_bid, in_num_entries, in_first_entry)

    // Do CAM lookup based on cam_tag.
    cam_lookup[out_lookup_result, cam_tag]

    // Check for CAM Hit/Miss
    br_bset[out_lookup_result, 7, CAM_HIT#]

CAM_MISS#:

    // CAM Miss

    .begin

    .reg bit_vector     // temp CAM bit vector
    .reg abs_entry_num  // absolute cam entry number [0 thru 15]
    .reg blk_entry_num  // block specific CAM entry number
    .reg ref_cnt_pos    // the bit position of reference count for entry
    .reg one_shifted    // 1 left-shifted by ref_cnt_pos
    .reg temp           // temp variable

    // Get the microblock bitvector and zero out rest of the bits from
    // camsharing_bit_vector.
#define_eval BIT_MASK ( ( (1 << in_num_entries) - 1 )  << in_first_entry)

    immed32(bit_vector, BIT_MASK)
    alu[bit_vector, @camsharing_bit_vector, AND, bit_vector]

#undef BIT_MASK

    // Get the first free entry in the microblock (here absolute entry num)
    // We will always have atleast one entry free, if
    // num of cam entries assigned to  microblock >= num of block threads.
    // But in any case check is provided.
    ffs[abs_entry_num, bit_vector]
    beq[NO_FREE_ENTRY#]


INCR_REF_CNT#:

    // Calculate the ref_cnt position
    alu[ref_cnt_pos, 0x1C, AND, abs_entry_num, <<2]

    // Check which @reg to update for ref_cnt increment.
    br_bset[abs_entry_num, 3, ENTRY_GT_8#], defer[3]
        // Load the result of the lookup into LM_Handle 0 or 1 (will be used by microblock).
        // LM_Addr[5:0]   = 0
        // LM_Addr[9:6]   = cam entry
        // LM_Addr[11:10] = a 2-bit value for LM start addr
        alu[temp, --, B, abs_entry_num, <<6]
        alu[temp, temp, OR, in_lm_start_addr, <<10]
        local_csr_wr[active_lm_addr_/**/in_lm_handle, temp]


    // Use @camsharing_ref_cnt1 (for entries 0 thru 7)

    // Increment the ref_cnt.
    // Left-shift 1 acc to ref_cnt_pos and add it to @camsharing_ref_cnt1.
    alu[temp, ref_cnt_pos, B, @camsharing_ref_cnt1]
    alu[one_shifted, --, B, 1, <<indirect]

    br[MARK_ENTRY#],defer[2]
        alu[@camsharing_ref_cnt1, temp, +, one_shifted]
        // Calc block specific cam entry.
        __cam_calc_blk_entry(blk_entry_num, abs_entry_num, in_first_entry)

ENTRY_GT_8#:

    // Use @camsharing_ref_cnt2 (for entries 8 thru 15)

    // Increment the ref_cnt.
    // Left-shift 1 acc to ref_cnt_pos and add it to @camsharing_ref_cnt2.
    alu[temp, ref_cnt_pos, B, @camsharing_ref_cnt2]
    alu[one_shifted, --, B, 1, <<indirect]
    alu[@camsharing_ref_cnt2, temp, +, one_shifted]

    // Calc block specific cam entry.
    __cam_calc_blk_entry(blk_entry_num, abs_entry_num, in_first_entry)


MARK_ENTRY#:

    // Mark the entry as used (i.e. clear the bit) in @camsharing_bit_vector
    alu[temp, abs_entry_num, B, @camsharing_bit_vector]
    alu_shf[temp,  temp, AND~, 1, <<indirect]

    br[END#], defer[2]
    alu[@camsharing_bit_vector, --, b, temp]
    // Update lookup result with miss bit(value 0), block specific cam entry.
    // out_lookup_result = state(11:8) + miss/hit (7) + entry(6:3)
    ld_field[out_lookup_result, 0001, blk_entry_num, <<3]


NO_FREE_ENTRY#:

    // Ideally this wont happen if num of cam entries >= num of block threads.

    // Here, you dont have any CAM entry free for block, so first entry of
    // the block is returned. Hence LRU implementation is messed up.
    // So user should take care that above condition is satisfied.

    br[INCR_REF_CNT#], defer[2]
        alu[temp, --, B, 0x0]
        __cam_calc_abs_entry(abs_entry_num, temp, in_first_entry)

    .end // end of CAM Miss

CAM_HIT#:

    // CAM Hit

    .begin

    .reg abs_entry_num  // absolute cam entry number [0 thru 15]
    .reg ref_cnt_pos    // the bit position of reference count for entry
    .reg one_shifted    // 1 left-shifted by ref_cnt_pos
    .reg lm_addr        // addr to load in in_lm_handle
    .reg temp           // temp variable


    // Get the absolute entry number from the lookup result ([0 thru 15])
    alu[abs_entry_num, 0xf, AND, out_lookup_result, >>3]

    // Check which @reg to increment the ref_cnt.
    br_bset[abs_entry_num, 3, ENTRY_GT_8_2#], defer[3]
        // Calculate the ref_cnt position
        alu[ref_cnt_pos, 0x1C, AND, abs_entry_num, <<2]

        // Load the result of the lookup into LM_Handle 0 or 1 (will be used by microblock).
        // LM_Addr[5:0]   = 0
        // LM_Addr[9:6]   = cam entry
        // LM_Addr[11:10] = a 2-bit value for LM start addr
        alu[lm_addr, --, B, abs_entry_num, <<6]
        alu[lm_addr, lm_addr, OR, in_lm_start_addr, <<10]



    // Use @camsharing_ref_cnt1 (for entries 0 thru 7)

    // Increment the ref_cnt.
    // Left-shift 1 acc to ref_cnt_pos and add it to @camsharing_ref_cnt1.
    alu[temp, ref_cnt_pos, B, @camsharing_ref_cnt1]
    alu[one_shifted, --, B, 1, <<indirect]

    br[UPD_ENTRY#],defer[2]
        alu[@camsharing_ref_cnt1, temp, +, one_shifted]
        local_csr_wr[active_lm_addr_/**/in_lm_handle, lm_addr]

ENTRY_GT_8_2#:

    // Use @camsharing_ref_cnt2 (for entries 8 thru 15)

    // Increment the ref_cnt.
    // Left-shift 1 acc to ref_cnt_pos and add it to @camsharing_ref_cnt2.
    alu[temp, ref_cnt_pos, B, @camsharing_ref_cnt2]
    alu[one_shifted, --, B, 1, <<indirect]
    alu[@camsharing_ref_cnt2, temp, +, one_shifted]

    local_csr_wr[active_lm_addr_/**/in_lm_handle, lm_addr]

UPD_ENTRY#:

    // Load block specific entry number and hit-bit in lookup_result.
    alu[temp, abs_entry_num, -, in_first_entry]
    ld_field[out_lookup_result, 0001, temp, <<3]
    alu_shf[out_lookup_result, out_lookup_result, OR, 1, <<7]


    // Mark the entry as used (clear the bit) in @camsharing_bit_vector
    alu[temp, abs_entry_num, B, @camsharing_bit_vector]
    alu_shf[temp, temp, AND~, 1, <<indirect]
    alu[@camsharing_bit_vector, --, B, temp]

    .end

END#:

#else

    // CAM not shared

    cam_lookup[out_lookup_result, in_key], lm_addr/**/in_lm_handle[/**/in_lm_start_addr]

#endif

.end
#endm


/// @cond INTERNAL_MACROS

/** Create unique CAM tag with the combination of @p in_bid and @p in_src_key.
 *
 * @param out_cam_tag       The resulting tag is returned here.
 * @param in_src_key        Source key.
 * @param in_bid            Microblock Block ID.
 * @param in_num_entries    Number of CAM entries for this microblock.
 * @param in_first_entry    First CAM entry number of this microblock.
 *
 */
#macro __cam_create_tag(out_cam_tag, in_src_key, in_bid, in_num_entries, \
                        in_first_entry)
.begin

#ifdef CAM_SHARED

    // Mask the upper bits equal to BID_BIT_SZ of in_src_key

#define_eval BID_MASK  ( (1 << BID_BIT_SZ) - 1)

    alu_shf[out_cam_tag, in_src_key, AND~, BID_MASK, <<SRC_KEY_BIT_SZ]

#undef BID_MASK

    // cam_tag = bid + src_key
    alu_shf[out_cam_tag, out_cam_tag, OR, in_bid, <<SRC_KEY_BIT_SZ]

#else

    // CAM not shared

    alu[out_cam_tag, --, B, in_src_key]

#endif

.end
#endm


/** Convert the @p in_blk_entry_num of a specific microblock into the absolute
 *  CAM entry number in range of 0 through 15.
 *
 * @param out_entry_num     The resulting entry number is returned here.
 * @param in_blk_entry_num  Relative entry number for block.
 * @param in_first_entry    First entry for the block.
 *
 */
#macro __cam_calc_abs_entry(out_entry_num, in_blk_entry_num, in_first_entry)
.begin

    // Calculate the absolute entry number in the range of 0 thru 15.
    alu[out_entry_num, in_first_entry, +, in_blk_entry_num]

.end
#endm


/** Convert the absolute CAM entry number from range 0 through 15 into a microblock
 *  specific CAM entry number.
 *
 * @param out_blk_entry_num The resulting entry number is returned here.
 * @param in_abs_entry_num  Absolute entry number.
 * @param in_first_entry    First entry for the block.
 *
 */
#macro __cam_calc_blk_entry(out_blk_entry_num, in_abs_entry_num, in_first_entry)
.begin

    // Calculate the block specific entry number.
    alu[out_blk_entry_num, in_abs_entry_num, -, in_first_entry]

.end
#endm

/// @endcond

/** @}
 * @}
 */


#endif /* __CAM_SHARING_UC__ */
