/*
 * Copyright (C) 2008-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __HASH_UC__
#define __HASH_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif


#include <constants.uc>
#include <xbuf.uc>
#include <stdmac.uc>
#include <sram.uc>
#include <dram.uc>
#include <util.uc>

/** @file hash.uc Hash Operation Macros
 * @addtogroup shac HASH operation
 * @{
 *
 * @name HASH operation macros
 * @{
 *
 * Trie type (@c TRIE_TYPE) used by hash lookup macros is encoded as:
 * @code
 * 31     (8-bits)        24    (8-bits)       16     (8-bits)        8    (4-bits)     4   (4-bits)  0
 * ----------------------------------------------------------------------------------------------------
 * | PRIM_INDEX_START_BIT | PRIM_INDEX_BIT_LEN | COLL_INDEX_START_BIT | COLL_SHIFT_BITS | LOOKUP_TYPE |
 * ----------------------------------------------------------------------------------------------------
 * @endcode
 *
 * For use with CLS, this section creates a global absolute register:
 * @code @cls_hash_mask_idx_addr @endcode
 */

// API Quick Reference:
//
// hash_dual_lookup(out_index1, out_index2, in_key1, in_key2, KEY_SIZE, trie_base_addr, TRIE_TYPE, KEY_DATA_SD_BASE)
// hash_init(in_multiplier, MULTIPLIER_SIZE)
// hash_init_cls(INDEX, CLS_OFFSET, MULT_SELECT, SBOX_ENA, NUM_SBOX)
// hash_lookup(out_index, in_key, KEY_SIZE, trie_base_addr, TRIE_TYPE, KEY_DATA_SD_BASE)
// hash_translate(io_index, INDEX_SIZE, REQ_SIG, in_wait_sigs)



#define HW_HASH_48  1 /**< API identifier for 48 bit hash keys. */
#define HW_HASH_64  2 /**< API identifier for 64 bit hash keys. */
#define HW_HASH_128 3 /**< API identifier for 128 bit hash keys. */

#define CLS_HASH_M4  (1<<0)
#define CLS_HASH_M36 (1<<1)
#define CLS_HASH_M53 (1<<2)
#define CLS_HASH_M63 (1<<3)

#define CLS_HASH_BASE_ADDR          0x40000
#define CLS_HASH_MUL_REG_OFFSET     0x000
#define CLS_HASH_IDX0_64_REG_OFFSET 0x800

/// @cond INTERNAL_MACROS
#define _LOOKUP_TYPE_16_4          1 /**< Value for @c LOOKUP_TYPE in @c TRIE_TYPE */
#define _LOOKUP_TYPE_FOLD_16       2 /**< Value for @c LOOKUP_TYPE in @c TRIE_TYPE */

#define _PRIM_INDEX_START_BIT  0 /**< Encoded in the trie type */
#define _PRIM_INDEX_BIT_LEN   16 /**< Encoded in the trie type */
#define _COLL_INDEX_START_BIT 16 /**< Encoded in the trie type. Since primary
                                      index bit len is 16 we should start
                                      checking after the 16 bits */
#define _COLL_SHIFT_BITS       4 /**< Encoded in the trie type */
/// @endcond


#ifdef HASH_16_4
    #warning "HASH_16_4 is being redefined."
#endif

#ifdef HASH_FOLD_16
    #warning "HASH_FOLD_16 is being redefined."
#endif

/// @cond INTERNAL_MACROS
#define_eval HASH_16_4  ( (_PRIM_INDEX_START_BIT <<24) |  \
                          (_PRIM_INDEX_BIT_LEN <<16) |    \
                          (_COLL_INDEX_START_BIT<<8) |    \
                          (_COLL_SHIFT_BITS <<4) |        \
                          (_LOOKUP_TYPE_16_4) )

#define_eval HASH_FOLD_16 ( (_PRIM_INDEX_START_BIT <<24) |    \
                            (_PRIM_INDEX_BIT_LEN <<16) |      \
                            (_COLL_INDEX_START_BIT<<8) |      \
                            (_COLL_SHIFT_BITS <<4) |          \
                            (_LOOKUP_TYPE_FOLD_16) )
/// @endcond


/** Initialize the hash multiplier of the hardware hash translation unit.
 *
 * @b Example:
 * @code
 * xbuf_alloc($multiplier, 2, read_write)
 * move($multiplier[0], 0)
 * move($multiplier[1], 1)
 * hash_init($multiplier, HW_HASH_64)
 * xbuf_free($multiplier)
 * @endcode
 *
 * @param in_multiplier     Buffer of write transfer registers with the multiplier. For more
 *                          description of the multiplier, please refer to the Hash Unit section
 *                          of the @databook. Note that this parameter must be
 *                          supplied using the xbuf_alloc macro. (See Example Usage)
 * @param MULTIPLIER_SIZE   Size of multiplier: @ref HW_HASH_48, @ref HW_HASH_64 or @ref HW_HASH_128
 *
 * @note Not available for NFP6000. Use hash_init_cls instead.
 *
 * @instruction_cnt 2 to 4
 */
#macro hash_init(in_multiplier, MULTIPLIER_SIZE)
.begin
    #if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
        #error "hash_init: this macro is not supported on NFP6000. use hash_init_cls instead."
    #else

    .sig hash_init_sig
    #if (MULTIPLIER_SIZE == HW_HASH_48)
        cap[write, in_multiplier[/**/0/**/], HASH_MULTIPLIER_48_0], ctx_swap[hash_init_sig]
        cap[write, in_multiplier[/**/1/**/], HASH_MULTIPLIER_48_1], ctx_swap[hash_init_sig]

    #elif(MULTIPLIER_SIZE == HW_HASH_64)
        cap[write, in_multiplier[/**/0/**/], HASH_MULTIPLIER_64_0], ctx_swap[hash_init_sig]
        cap[write, in_multiplier[/**/1/**/], HASH_MULTIPLIER_64_1], ctx_swap[hash_init_sig]

    #elif(MULTIPLIER_SIZE == HW_HASH_128)
        cap[write, in_multiplier[/**/0/**/], HASH_MULTIPLIER_128_0], ctx_swap[hash_init_sig]
        cap[write, in_multiplier[/**/1/**/], HASH_MULTIPLIER_128_1], ctx_swap[hash_init_sig]
        cap[write, in_multiplier[/**/2/**/], HASH_MULTIPLIER_128_2], ctx_swap[hash_init_sig]
        cap[write, in_multiplier[/**/3/**/], HASH_MULTIPLIER_128_3], ctx_swap[hash_init_sig]
    #else
        #error "Invalid multiplier, valid multiplier sizes are HW_HASH_48, HW_HASH_64 or HW_HASH_128"
    #endif

    #endif
.end
#endm

/** Initialize the cls hash logic.
 *
 * @b Example:
 * @code
 * #define CLS_HASH_IDX  1
 * #define CLS_MUL_SEL   (CLS_HASH_M4 | CLS_HASH_M36 | CLS_HASH_M53 | CLS_HASH_M63 )
 * #define CLS_SB_ENA    0
 * #define CLS_NUM_SB    0
 * .global_mem hashmask cls 16 8
 * .init hashmask 0xffffffff, 0x0000ffff, 0x00000000, 0x00000000
 * hash_init_cls(CLS_HASH_IDX, hashmask, CLS_MUL_SEL, CLS_SB_ENA, CLS_NUM_SB)
 * @endcode
 *
 * @param INDEX          Cls Hash Index to use, valid values 0-7
 * @param in_mask        Cls global mem with initialized 128 bit mask value. For more
 *                       description of the mask, please refer to the CLS Hash Unit section
 *                       of the @databook.
 * @param MUL_SEL        Multiplier selects, logical or of CLS_HASH_M4, CLS_HASH_M36, CLS_HASH_M53,
 *                       CLS_HASH_M63 as required by user.  Refer to @databook for description
 * @param SB_ENA         SBOX Enable, valied values 1 or 0, refer to @databook for description
 * @param NUM_SB         Number of SBOX's to use, value values 0 through 15, refer to @databook for
 *                       description.
 *
 * @note Must be invoked on NFP6000 prior to using other hash macros.
 *
 * @instruction_cnt 2 to 3
 */

.reg global @cls_hash_mask_idx_addr

#macro hash_init_cls(INDEX, in_mask, MUL_SEL, SB_ENA, NUM_SB)
.begin
    .sig hash_init_sig
    .reg $xfr, hb_addr

    #ifdef _CLS_HASH_IDX
        #undef _CLS_HASH_IDX
    #endif

    #define_eval _CLS_HASH_IDX (INDEX & 7)
    #define _CLS_MASK_ADDR in_mask

    #define_eval _CLS_MUL_SEL (MUL_SEL & 0xf)
    #define_eval _CLS_SB_ENA ((SB_ENA & 1) << 8)
    #define_eval _CLS_NUM_SB ((NUM_SB & 0xf) << 4)
    #define _CLS_HASH_MUL_REG_VAL ( _CLS_MUL_SEL | _CLS_SB_ENA | _CLS_NUM_SB )

    move($xfr, _CLS_HASH_MUL_REG_VAL)
    move(hb_addr, CLS_HASH_BASE_ADDR)
    cls[write, $xfr, hb_addr, CLS_HASH_MUL_REG_OFFSET, 1], ctx_swap[hash_init_sig]

    immed32[@cls_hash_mask_idx_addr, in_mask]
    immed_b2[@cls_hash_mask_idx_addr, _CLS_HASH_IDX]

    #undef _CLS_MASK_ADDR
    #undef _CLS_MUL_SEL
    #undef _CLS_SB_ENA
    #undef _CLS_NUM_SB
    #undef _CLS_HASH_MUL_REG_VAL

.end
#endm

/** Translate a big index, up to 128 bits, using the hardware hash translation unit.
 *
 * @b Example:
 * @code
 * .sig sig1
 * hash_translate($big_index, HW_HASH_128, sig1, sig1)
 * @endcode
 *
 * @param io_index          Buffer of read/write tranfer registers:
 *                          @arg Output: buffer of read transfer registers with the
 *                                    translated index
 *                          @arg Input : buffer of write transfer registers with the
 *                                    index to be translated
 * @param INDEX_SIZE        Size of index: @ref HW_HASH_48, @ref HW_HASH_64 or @ref HW_HASH_128
 * @param REQ_SIG           Requested signal. See common section Signal Arguments.
 * @param in_wakeup_sigs    List of signals causing thread to swap/wakeup. See common
 *                          section Signal Arguments.
 *
 * @instruction_cnt 1 to 2
 */
#macro hash_translate(io_index, INDEX_SIZE, REQ_SIG, in_wakeup_sigs)
.begin

    #if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))

        #if !(defined (_CLS_HASH_IDX))
            #error "hash_translate: for nfp6000, must call cls_hash_init prior to using the hash unit."
        #endif

        .reg idx_reg_addr, mask_idx_addr
        #define_eval IDX_ADDR (CLS_HASH_BASE_ADDR + CLS_HASH_IDX0_64_REG_OFFSET + (_CLS_HASH_IDX << 3))
	move(idx_reg_addr, IDX_ADDR)
        #undef IDX_ADDR
        alu[mask_idx_addr, --, B, @cls_hash_mask_idx_addr]
        #if (INDEX_SIZE == HW_HASH_48)
            cls[hash_mask_clear, io_index, mask_idx_addr, 0, 2], ctx_swap[REQ_SIG]
            cls[read, io_index, idx_reg_addr, 0, 2], ctx_swap[REQ_SIG]
        #elif (INDEX_SIZE == HW_HASH_64)
            cls[hash_mask_clear, io_index, mask_idx_addr, 0, 2], ctx_swap[REQ_SIG]
            cls[read, io_index, idx_reg_addr, 0, 2], ctx_swap[REQ_SIG]
        #elif (INDEX_SIZE == HW_HASH_128)
            cls[hash_mask_clear, io_index, mask_idx_addr, 0, 4], ctx_swap[REQ_SIG]
            cls[read, io_index, idx_reg_addr, 0, 4], ctx_swap[REQ_SIG]
        #else
            #error "Invalid index_size, valid index sizes are HW_HASH_48, HW_HASH_64 or HW_HASH_128"
        #endif

    #else

        #if (INDEX_SIZE == HW_HASH_48)
            hash_48[io_index, 1], sig_done[REQ_SIG]
        #elif (INDEX_SIZE == HW_HASH_64)
            hash_64[io_index, 1], sig_done[REQ_SIG]
        #elif (INDEX_SIZE == HW_HASH_128)
            hash_128[io_index, 1], sig_done[REQ_SIG]
        #else
            #error "Invalid index_size, valid index sizes are HW_HASH_48, HW_HASH_64 or HW_HASH_128"
        #endif

        #if (!streq('in_wakeup_sigs', 'SIG_NONE'))
            ctx_arb[in_wakeup_sigs]
        #endif

    #endif

.end
#endm


/** Lookup a table entry using up to 128 bit index.
 *
 * Uses tables written by core Hash Table Database Manager.
 *
 * @b Example:
 * @code
 * hash_lookup(table_entry_index, $wide_index[0], 102, trie_addr, HASH16_4, 0x100)
 * @endcode
 *
 * @param out_index         Index of hash table entry if success. 0 if fail (no entry)
 * @param in_key            Buffer of write transfer registers with the starting index,
 *                          up to 128 bits
 * @param KEY_SIZE          Size of @p in_key in bits
 * @param trie_base_addr    Address of trie table, which is the sram primary base table
 * @param TRIE_TYPE         Index bits used to address the hash trie: @c HASH_16_4, @c HASH_FOLD_16
 *                          @arg @c HASH_16_4: First lookup uses 16 bits of index, subsequent lookups
 *                                             use 4 bits of index
 *                          @arg @c HASH_FOLD_16: First lookup XORs the initial index to reduce it by
 *                                                half, then performs a table lookup using 16 bits of
 *                                                half-index, with subsequent chain search until no collision
 * @param KEY_DATA_SD_BASE  GPR or CONST, list of available SDRAM space to be used for key/data storage
 *
 * @note If HASH_DONT_TRANSLATE_KEYS is defined, the macro will skip hash translation.
 *
 * @instruction_cnt 14 to 16 for @p TRIE_TYPE = @c HASH_16_4 ((1 dram access, 1 dram accesss) + 11*No of iterations (1 sram access/iteration))
 * @instruction_cnt 12 to 19 for @p TRIE_TYPE = @c HASH_FOLD_16 ((1 sram access) + 14*No of iteration (1 dram access/Iteration))
 */
#macro hash_lookup(out_index, in_key, KEY_SIZE, trie_base_addr, TRIE_TYPE, KEY_DATA_SD_BASE)
.begin

    #ifdef _PRIM_INDEX_START_BIT
        #warning "_PRIM_INDEX_START_BIT is being redefined"
    #endif

    #ifdef _PRIM_INDEX_BIT_LEN
        #warning "_PRIM_INDEX_BIT_LEN is being redefined"
    #endif

    #ifdef _COLL_INDEX_START_BIT
        #warning "_COLL_INDEX_START_BIT is being redefined"
    #endif

    #ifdef _COLL_SHIFT_BITS
        #warning "_COLL_SHIFT_BITS is being redefined"
    #endif

    #ifdef _LOOKUP_TYPE
        #warning "_LOOKUP_TYPE is being redefined"
    #endif

    #define_eval _PRIM_INDEX_START_BIT  ((TRIE_TYPE >> 24) & 0xFF)
    #define_eval _PRIM_INDEX_BIT_LEN    ((TRIE_TYPE >> 16) & 0xFF)
    #define_eval _COLL_INDEX_START_BIT  ((TRIE_TYPE >> 8) & 0xFF)
    #define_eval _COLL_SHIFT_BITS       ((TRIE_TYPE >> 4) & 0xF)
    #define_eval _LOOKUP_TYPE           ((TRIE_TYPE) & 0xF)

    #if (_LOOKUP_TYPE == _LOOKUP_TYPE_16_4)
        _hash_lookup_16_4(out_index, in_key, KEY_SIZE, trie_base_addr, \
                            _PRIM_INDEX_START_BIT, _PRIM_INDEX_BIT_LEN, \
                            _COLL_INDEX_START_BIT, _COLL_SHIFT_BITS, \
                            KEY_DATA_SD_BASE)
    #elif (_LOOKUP_TYPE == _LOOKUP_TYPE_FOLD_16)
        _hash_lookup_fold_16 (out_index, in_key, KEY_SIZE, trie_base_addr,
                                _PRIM_INDEX_BIT_LEN, KEY_DATA_SD_BASE)
    #else
        #error "Invalid Trie types, valid values are HASH_16_4 & HASH_FOLD_16"
    #endif

    // clean up name space

    #undef _PRIM_INDEX_START_BIT
    #undef _PRIM_INDEX_BIT_LEN
    #undef _COLL_INDEX_START_BIT
    #undef _COLL_SHIFT_BITS
    #undef _LOOKUP_TYPE

.end
#endm


/** Lookup two table entries in parallel using up to 128 bit index each.
 *
 * The reads of the trie structure are done in parallel. This utilizes trie
 * structure and tables written by core Hash Table Database Manager.
 *
 * @b Example:
 * @code
 * hash_dual_lookup(table_entry_index1, table_entry_index2, $wide_index1[0], wide_index2[0], \
      102, trie_addr, HASH16_4, 0)
 * @endcode
 *
 * @param out_index1        Corresponding index of hash table entry if success. 0 if fail (no entry).
 * @param out_index2        Corresponding index of hash table entry if success. 0 if fail (no entry).
 * @param in_key1           Corresponding buffer of write transfer registers with the starting index,
 *                          up to 128 bits
 * @param in_key2           Corresponding buffer of write transfer registers with the starting index,
 *                          up to 128 bits
 * @param KEY_SIZE          Size of @p in_key1 in bits, @p in_key2 must be the same size
 * @param trie_base_addr    Address of trie table, which is the sram primary base table
 * @param TRIE_TYPE         Index bits used to address the hash trie: @c HASH_16_4, @c HASH_FOLD_16
 *                          @arg @c HASH_16_4: First lookup uses 16 bits of index, subsequent lookups
 *                                          use 4 bits of index
 *                          @arg @c HASH_FOLD_16: First lookup XORs the initial index to reduce it by
 *                                             half, then performs a table lookup using 16 bits of
 *                                             half-index, with subsequent chain search until no collision
 * @param KEY_DATA_SD_BASE  GPR or CONST, list of available SDRAM space to be used for key/data storage
 *
 * @note If HASH_DONT_TRANSLATE_KEYS is defined the macro will skip hash translation.
 *
 * @instruction_cnt 32 to 37 for @p TRIE_TYPE = @c HASH_16_4 ((2 sram access, 2 dram access) + 22*No of iterations (2 sram access/iteration))
 * @instruction_cnt 32 to 40 for @p TRIE_TYPE = @c HASH_FOLD_16 ((2 sram access) + 28*No of iteration (1 dram access/Iteration))
 */
#macro hash_dual_lookup(out_index1, out_index2, in_key1, in_key2, KEY_SIZE, trie_base_addr, TRIE_TYPE, KEY_DATA_SD_BASE)
.begin

    #ifdef _PRIM_INDEX_START_BIT
        #warning "_PRIM_INDEX_START_BIT is being redefined"
    #endif

    #ifdef _PRIM_INDEX_BIT_LEN
        #warning "_PRIM_INDEX_BIT_LEN is being redefined"
    #endif

    #ifdef _COLL_INDEX_START_BIT
        #warning "_COLL_INDEX_START_BIT is being redefined"
    #endif

    #ifdef _COLL_SHIFT_BITS
        #warning "_COLL_SHIFT_BITS is being redefined"
    #endif

    #ifdef _LOOKUP_TYPE
        #warning "_LOOKUP_TYPE is being redefined"
    #endif

    #define_eval _PRIM_INDEX_START_BIT  ((TRIE_TYPE >> 24) & 0xFF)
    #define_eval _PRIM_INDEX_BIT_LEN    ((TRIE_TYPE >> 16) & 0xFF)
    #define_eval _COLL_INDEX_START_BIT  ((TRIE_TYPE >> 8) & 0xFF)
    #define_eval _COLL_SHIFT_BITS       ((TRIE_TYPE >> 4) & 0xF)
    #define_eval _LOOKUP_TYPE           ((TRIE_TYPE) & 0xF)



    #if (_LOOKUP_TYPE == _LOOKUP_TYPE_16_4)
        _hash_dual_lookup_16_4(out_index1, out_index2, in_key1, in_key2, KEY_SIZE, trie_base_addr, \
                            _PRIM_INDEX_START_BIT, _PRIM_INDEX_BIT_LEN, \
                            _COLL_INDEX_START_BIT, _COLL_SHIFT_BITS, \
                            KEY_DATA_SD_BASE)
    #elif (_LOOKUP_TYPE == _LOOKUP_TYPE_FOLD_16)
        _hash_dual_lookup_fold_16 (out_index1, out_index2, in_key1, in_key2, KEY_SIZE, trie_base_addr, \
                            _PRIM_INDEX_START_BIT, _PRIM_INDEX_BIT_LEN, \
                            _COLL_INDEX_START_BIT, _COLL_SHIFT_BITS, \
                            KEY_DATA_SD_BASE)
    #else
        #error "Invalid Trie types, valid values are HASH_16_4 & HASH_FOLD_16"
    #endif

    // clean up name space
    #undef _PRIM_INDEX_START_BIT
    #undef _PRIM_INDEX_BIT_LEN
    #undef _COLL_INDEX_START_BIT
    #undef _COLL_SHIFT_BITS
    #undef _LOOKUP_TYPE

.end
#endm


/// @cond INTERNAL_MACROS
/** Internal macro to perform HASH look up table entry using up to 128 bit index.
 *
 * Uses tables written by core Hash Table Database Manager.
 *
 * @param out_index              Index of hash table entry if success. 0 if fail (no entry)
 * @param in_key                 Buffer of write transfer registers with the starting index,
 *                               up to 128 bits
 * @param KEY_SIZE               Size of @p in_key in bits
 * @param trie_base_addr         Address of trie table, which is the sram primary base table
 * @param _PRIM_INDEX_START_BIT  GPR or CONST, primary table index starting-bit position
 * @param _PRIM_INDEX_BIT_LEN    GPR or CONST, primary table index bit length
 * @param _COLL_INDEX_START_BIT  GPR or CONST, collision table index starting-bit position
 * @param _COLL_SHIFT_BITS       GPR or CONST, number of bits to step the collision table index after a collision
 * @param KEY_DATA_SD_BASE       GPR or CONST, list of available SDRAM space to be used for key/data storage
 *
 * @instruction_cnt 14 to 16 (1 dram access, 1 sram access) + 11*No of iterations (1 sram access/iteration)
 */
#macro _hash_lookup_16_4(out_index, in_key, KEY_SIZE, trie_base_addr, \
                        _PRIM_INDEX_START_BIT, _PRIM_INDEX_BIT_LEN, \
                        _COLL_INDEX_START_BIT, _COLL_SHIFT_BITS, \
                        KEY_DATA_SD_BASE)
.begin

    // Issue warnings if the names that we use are already defined
    #ifdef _INDEX_SIZE
        #warning  "_INDEX_SIZE is being redefined"
    #endif

    #ifdef _LOOKUP_READ_SIZE
        #warning "_LOOKUP_READ_SIZE is being redefined"
    #endif

    #ifdef _D_LOOKUP_READ_SIZE
        #warning "_D_LOOKUP_READ_SIZE is being redefined"
    #endif

    #ifdef _COLL_INDEX_BIT_LEN
        #warning "COLL_INDEX_BIT_LEN is being redefined"
    #endif

    #if (KEY_SIZE <= 48)
        #define_eval _INDEX_SIZE               HW_HASH_48
        #define_eval _LOOKUP_READ_SIZE         2
        #define_eval _D_LOOKUP_READ_SIZE       1
        #define_eval _COLL_INDEX_BIT_LEN       48

    #elif (KEY_SIZE <= 64)
        #define_eval _INDEX_SIZE               HW_HASH_64
        #define_eval _LOOKUP_READ_SIZE         2
        #define_eval _D_LOOKUP_READ_SIZE       1
        #define_eval _COLL_INDEX_BIT_LEN       64

    #elif (KEY_SIZE <=128)
        #define_eval _INDEX_SIZE               HW_HASH_128
        #define_eval _LOOKUP_READ_SIZE         4
        #define_eval _D_LOOKUP_READ_SIZE       2
        #define_eval _COLL_INDEX_BIT_LEN       128

    #else
        #error "Specified key size too large, valid values are <= 128"
    #endif

    // we skip hash translation when HASH_DONT_TRANSLATE_KEYS is defined
    #ifndef HASH_DONT_TRANSLATE_KEYS
        .sig hash_lookup_sig

        hash_translate(in_key[0], _INDEX_SIZE, hash_lookup_sig, hash_lookup_sig)
    #endif  // HASH_DONT_TRANSLATE_KEYS

    .reg $table_val
    _hash_read_prim_table($table_val, in_key, trie_base_addr, _PRIM_INDEX_START_BIT, _PRIM_INDEX_BIT_LEN)
    _hash_resolve_16_4(out_index, $table_val, in_key, KEY_SIZE, _COLL_INDEX_START_BIT, _COLL_INDEX_BIT_LEN, _COLL_SHIFT_BITS, KEY_DATA_SD_BASE)

    // Clean up our defines
    #undef _INDEX_SIZE
    #undef _LOOKUP_READ_SIZE
    #undef _D_LOOKUP_READ_SIZE
    #undef _COLL_INDEX_BIT_LEN

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to define _LOOKUP_READ_SIZE_LW depending on the KEY_SIZE
 *
 * @param KEY_SIZE  Size of the key
 */
#macro _DEFINE_LOOKUP_READ_SIZE(KEY_SIZE)
    #if (KEY_SIZE <= 48)
        #define_eval _LOOKUP_READ_SIZE_LW 2
    #elif (KEY_SIZE <= 64)
        #define_eval _LOOKUP_READ_SIZE_LW 2
    #elif (KEY_SIZE <=128)
        #define_eval _LOOKUP_READ_SIZE_LW 4
    #else
        #error "Specified key size too large, valid values are <= 128"
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to perform a parallel search of the primary table for the specified key.
 *
 * @param out_value             GPR or SRAM transfer register
 * @param in_key                Transfer registers upper lower 32 bits of the search key
 * @param prim_table_base       GPR or CONST, SRAM base of the primary table
 * @param prim_index_start_bit  GPR or CONST, primary table index starting-bit position
 * @param prim_index_bit_len    GPR or CONST, primary table index bit length
 *
 * @instruction_cnt 2 to 6 (1 sram access)
 */
#macro _hash_read_prim_table(out_value, in_key, prim_table_base, prim_index_start_bit, prim_index_bit_len)
.begin

    .reg _hdbm_index tmp_index
    .sig read_prim_table_sig
    .reg start_bit key_lo key_hi
    #ifdef _HASH_INDEX_IS_0_16
        #error "_HASH_INDEX_IS_0_16 is being redefined"
    #endif

    // Optimize code for prim_index_start_bit=0 and prim_index_bit_len=16
    #if (is_ct_const(prim_index_start_bit) || isimport(prim_index_start_bit))
        #if (prim_index_start_bit == 0)
            #if (is_ct_const(prim_index_bit_len) || isimport(prim_index_bit_len))
                #if (prim_index_bit_len == 16)
                    bytefield_extract(_hdbm_index, in_key[0], 2, 3, 0)

                    #define _HASH_INDEX_IS_0_16    1
                #endif
            #endif
        #endif
    #endif

    #ifndef _HASH_INDEX_IS_0_16
        // get the index from the key based on the starting bit position and the length
        move(start_bit, prim_index_start_bit)
        move(key_lo, in_key[0])
        move(key_hi, in_key[1])
        _get_index_field(_hdbm_index, key_lo, key_hi, start_bit, prim_index_bit_len)
    #else
        #undef _HASH_INDEX_IS_0_16 // clean up name space
    #endif

    shf_left(_hdbm_index, _hdbm_index, 2)  // get byte offset

    // read the primary table
    sram_read(out_value, prim_table_base, _hdbm_index, 1, read_prim_table_sig, read_prim_table_sig, ___)

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to perform a parallel search of the primary table for the specified key
 *  in BIG Endian.
 *
 * @param out_value1            GPR or SRAM transfer register
 * @param out_value2            GPR or SRAM transfer register
 * @param in_key1               Transfer registers the search key
 * @param in_key2               Transfer registers the search key
 * @param prim_table_base       GPR or CONST, SRAM base of the primary table
 * @param prim_index_start_bit  GPR or CONST, primary table index starting-bit position
 * @param prim_index_bit_len    GPR or CONST, primary table index bit length
 *
 * @instruction_cnt 17 to 20 (2 sram access)
 */
#macro _hash_bi_read_prim_table(out_value1, out_value2, in_key1, in_key2, prim_table_base, prim_index_start_bit, prim_index_bit_len)
.begin

    .reg _hdbm_index1 _hdbm_index2 key_lo key_hi tmp_data
    .sig read_prim_table_sig1 read_prim_table_sig2
    .reg start_bit
    //get the index from the key based on the starting bit position and the length
    move(start_bit, prim_index_start_bit)
    move(key_lo, in_key1[0])
    move(key_hi, in_key1[1])
    _get_index_field(_hdbm_index1, key_lo, key_hi, start_bit, prim_index_bit_len)

    move(key_lo, in_key2[0])
    move(key_hi, in_key2[1])
    _get_index_field(_hdbm_index2, key_lo, key_hi, start_bit, prim_index_bit_len)

    shf_left(_hdbm_index1, _hdbm_index1, 2) // get byte offset

    shf_left(_hdbm_index2, _hdbm_index2, 2) // get byte offset

    // read the primary table
    sram_read(out_value1, prim_table_base, _hdbm_index1, 1, read_prim_table_sig1, ___, ___)
    sram_read(out_value2, prim_table_base, _hdbm_index2, 1, read_prim_table_sig2, \
        signals(read_prim_table_sig1, read_prim_table_sig2), ___)

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to determine the status of the value read from the primary table and return the data if
 *  there was not conflict and, if there was a conflict, then search the secondary table for the specified key.
 *
 * @param out_index                 GPR contains the user defined index for the hash, 0 implies no match
 * @param table_val                 SRAM transfer register, the value read from the primary table
 * @param in_key                    Transfer registers, upper lower 32 bits of the search key
 * @param KEY_SIZE                  CONST indicating the size of in_key parameter
 * @param coll_index_start_bit      GPR or CONST, collision table index starting-bit position
 * @param coll_index_bit_len        GPR or CONST, collision table index bit length
 * @param coll_shift_bits           GPR or CONST, number of bits to step the collision table index after a collision
 * @param key_data_freelist_base    GPR or CONST, list of available SDRAM space to be used for key/data storage
 *
 * @instruction_cnt 12 to 14 ((1 dram access) + 11*No of iterations (1 sram access/iteration))
 */
#macro _hash_resolve_16_4(out_index, $table_val, in_key, KEY_SIZE, coll_index_start_bit, coll_index_bit_len, coll_shift_bits, key_data_freelist_base)
.begin

    .reg fwd_entry _hdbm_index table_base col_sht_amt

    _DEFINE_LOOKUP_READ_SIZE(KEY_SIZE)
    move(col_sht_amt, 0)

top_of_loop#:
    alu[--, --, B, $table_val]
    // if 0, entry not found
    beq[match_not_found#], defer[1]

    // if collision bit set, add the entry to collision table
    // entry_or_collision case:
    alu[--, $table_val, AND, 0x1]
    beq[entry_and_no_collision#], defer[3]

    // collision and existing entry
    _get_entry_index(table_base, $table_val)   ;get collision table reference
    .reg start_bit key_lo key_hi
    add(start_bit, coll_index_start_bit, col_sht_amt)
    alu[--, start_bit, -, coll_index_bit_len]
    bgt[match_not_found#], defer[2]

    add(col_sht_amt, coll_shift_bits, col_sht_amt)    //incr shft amt to avoid doing mult

    #if (KEY_SIZE <= 64)
        move(key_lo, in_key[0])
        move(key_hi, in_key[1])
    #else
        // for 128-bit KEY_SIZE support
        alu[--, start_bit, -, 64]
        bpl[load_high_key_index#], defer[2]
        move(key_lo, in_key[0])
        move(key_hi, in_key[1])
        br[next_read#]

        load_high_key_index#:
            // we use the higher 64-bit key when we cross 64-bits
            move(key_lo, in_key[2])
            move(key_hi, in_key[3])
            // also roll the start_bit to zero index if it crosses 64
            sub(start_bit, start_bit, 64)
    #endif // KEY_SIZE<64

next_read#:
    _get_index_field(_hdbm_index, key_lo, key_hi, start_bit, coll_shift_bits)
    shf_left(_hdbm_index, _hdbm_index, 2) // get byte offset

    .sig hdbm_resolve_sig
    // read the table
    sram_read($table_val, table_base, _hdbm_index, 1, hdbm_resolve_sig, hdbm_resolve_sig, ___)  ; lookup hash table
    br[top_of_loop#]

// no collision and existing entry,
entry_and_no_collision#:
    _get_entry_index(fwd_entry, $table_val)        ;get collision table reference

    #if (KEY_SIZE <= 64)
        // Read 1 longword of data in addition to key (2lw for key+ 1lw for data)
        #define_eval KEY_DATA_READ_SIZE_LW  4
        #define_eval KEY_DATA_READ_SIZE_QW  2
        // Key size - 2 long words
        #define_eval KEY_SIZE_LW    2
    #else
        // Read 1 longword of data in addition to key (4lw for key + 1lw for data)
        #define_eval KEY_DATA_READ_SIZE_LW  6
        #define_eval KEY_DATA_READ_SIZE_QW  3
        // Key size - 4 long words
        #define_eval KEY_SIZE_LW    4

    #endif

    #if (defined(__NFP_INDIRECT_REF_FORMAT_V1))
        #define _DXFER   $xkey_data
    #else
        #define _DXFER   $$xkey_data
    #endif

    xbuf_alloc(_DXFER, KEY_DATA_READ_SIZE_LW, read)

    // read and compare keys
    add(fwd_entry, fwd_entry, key_data_freelist_base)
    // NOTE: let us not subtract 1, we say zero is an invalid entry
    //sub(fwd_entry, fwd_entry, 1)

    dram_read(_DXFER[0], fwd_entry, 0, KEY_DATA_READ_SIZE_QW, hdbm_resolve_sig, hdbm_resolve_sig, ___) ;read entry key

    #ifdef _i
        #warning  "_i is being redefined"
    #endif

    #define_eval _i 0
    .reg temp_in_key
    #while (_i < KEY_SIZE_LW)
        move(temp_in_key, _DXFER[/**/_i/**/])
        comp(temp_in_key, in_key[/**/_i/**/])
        bne[match_not_found#]
        #define_eval _i _i+1
    #endloop

    // copy the data portion (32-bit?) on a successful match
    move(out_index, _DXFER[/**/_i/**/])

    // additional dummy data
    // #define_eval _i  _i+1
    // move(out_data_dummy, _DXFER[/**/_i/**/])
    br[exit_hash_resolve_16_4#]

match_not_found#:
    // Return 0 if no match found
    move(out_index, 0)

exit_hash_resolve_16_4#:
    xbuf_free(_DXFER)

    // clean up the name space

    #ifdef _i
        #undef _i
    #endif

    #ifdef _DXFER
        #undef _DXFER
    #endif
.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to search the database for the specified keys and return their status and associated
 *  data if the key was found.
 *
 * @param out_index1                GPR contains the user defined index for the hash, 0 implies no match
 * @param out_index2                GPR contains the user defined index for the hash, 0 implies no match
 * @param in_key1                   Transfer register, the search key
 * @param in_key2                   Transfer register, the search key
 * @param KEY_SIZE                  CONST indicating the size of in_key parameter
 * @param trie_base_addr            Address of trie table, which is the sram primary base table
 * @param _PRIM_INDEX_START_BIT     GPR or CONST, primary table index starting-bit position
 * @param _PRIM_INDEX_BIT_LEN       GPR or CONST, primary table index bit length
 * @param _COLL_INDEX_START_BIT     GPR or CONST, collision table index starting-bit position
 * @param _COLL_SHIFT_BITS          GPR or CONST, number of bits to step the collision table index after a collision
 * @param KEY_DATA_SD_BASE          GPR or CONST, list of available SDRAM space to be used for key/data storage
 *
 * @note No hash is performed on the key.
 *
 * @instruction_cnt 40 to 45 ((2 sram access, 2 dram access) + 22*No of iterations (2 sram access/iteration))
 */
#macro _hash_dual_lookup_16_4(out_index1, out_index2, in_key1, in_key2, KEY_SIZE, trie_base_addr, \
                                _PRIM_INDEX_START_BIT, _PRIM_INDEX_BIT_LEN, \
                                _COLL_INDEX_START_BIT, _COLL_SHIFT_BITS, \
                                KEY_DATA_SD_BASE)
.begin

    // Issue warnings if the names that we use are already defined
    #ifdef _INDEX_SIZE
        #warning  "_INDEX_SIZE is being redefined"
    #endif

    #ifdef _LOOKUP_READ_SIZE
        #warning "_LOOKUP_READ_SIZE is being redefined"
    #endif

    #ifdef _D_LOOKUP_READ_SIZE
        #warning "_D_LOOKUP_READ_SIZE is being  redefined"
    #endif

    #ifdef _COLL_INDEX_BIT_LEN
        #warning "COLL_INDEX_BIT_LEN is being redefined"
    #endif

    #if (KEY_SIZE <= 48)
        #define_eval _INDEX_SIZE           HW_HASH_48
        #define_eval _LOOKUP_READ_SIZE     2
        #define_eval _D_LOOKUP_READ_SIZE   1
        #define_eval _COLL_INDEX_BIT_LEN   48

    #elif (KEY_SIZE <= 64)
        #define_eval _INDEX_SIZE           HW_HASH_64
        #define_eval _LOOKUP_READ_SIZE     2
        #define_eval _D_LOOKUP_READ_SIZE   1
        #define_eval _COLL_INDEX_BIT_LEN   64

    #elif (KEY_SIZE <=128)
        #define_eval _INDEX_SIZE           HW_HASH_128
        #define_eval _LOOKUP_READ_SIZE     4
        #define_eval _D_LOOKUP_READ_SIZE   2
        #define_eval _COLL_INDEX_BIT_LEN   128

    #else
        #error "Specified key size too large, valid values are <= 128"
    #endif


    // we skip hash translation when HASH_DONT_TRANSLATE_KEYS is defined
    #ifndef HASH_DONT_TRANSLATE_KEYS
        .sig hash_lookup_sig1 hash_lookup_sig2

        // Optimization scope combine the two hash into one call
        hash_translate(in_key1[0], _INDEX_SIZE, hash_lookup_sig1, ___)
        hash_translate(in_key2[0], _INDEX_SIZE, hash_lookup_sig2, signals(hash_lookup_sig1, hash_lookup_sig2))
    #endif //HASH_DONT_TRANSLATE_KEYS

    .reg $table_val1 $table_val2
    _hash_bi_read_prim_table($table_val1, $table_val2, in_key1, in_key2, trie_base_addr, _PRIM_INDEX_START_BIT, _PRIM_INDEX_BIT_LEN)
    _hash_resolve_16_4(out_index1, $table_val1, in_key1, KEY_SIZE, _COLL_INDEX_START_BIT, _COLL_INDEX_BIT_LEN, _COLL_SHIFT_BITS, KEY_DATA_SD_BASE)
    _hash_resolve_16_4(out_index2, $table_val2, in_key2, KEY_SIZE, _COLL_INDEX_START_BIT, _COLL_INDEX_BIT_LEN, _COLL_SHIFT_BITS, KEY_DATA_SD_BASE)

    // Clean up our defines
    #undef _INDEX_SIZE
    #undef _LOOKUP_READ_SIZE
    #undef _D_LOOKUP_READ_SIZE
    #undef _COLL_INDEX_BIT_LEN

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to lookup two table entries in parallel using up to 128 bit index each. The reads of the trie
 *  structure are done in parallel. This utilizes trie structure and tables written by core
 *  Hash Table Database Manager.
 *
 * @param out_index1                GPR contains the user defined index for the hash, 0 implies no match
 * @param out_index2                GPR contains the user defined index for the hash, 0 implies no match
 * @param in_key1                   Transfer register, the search key
 * @param in_key2                   Transfer register, the search key
 * @param KEY_SIZE                  CONST indicating the size of in_key parameter
 * @param trie_base_addr            Address of trie table, which is the sram primary base table
 * @param _PRIM_INDEX_START_BIT     GPR or CONST, primary table index starting-bit position
 * @param _PRIM_INDEX_BIT_LEN       GPR or CONST, primary table index bit length
 * @param _COLL_INDEX_START_BIT     GPR or CONST, collision table index starting-bit position
 * @param _COLL_SHIFT_BITS          GPR or CONST, number of bits to step the collision table index after a collision
 * @param KEY_DATA_SD_BASE          GPR or CONST, list of available SDRAM space to be used for key/data storage
 *
 */
#macro _hash_dual_lookup_fold_16(out_index1, out_index2, in_key1, in_key2, KEY_SIZE, trie_base_addr, \
                                    _PRIM_INDEX_START_BIT, _PRIM_INDEX_BIT_LEN, \
                                    _COLL_INDEX_START_BIT, _COLL_SHIFT_BITS, \
                                    KEY_DATA_SD_BASE)
.begin

    // we skip hash translation when HASH_DONT_TRANSLATE_KEYS is defined
    #ifndef HASH_DONT_TRANSLATE_KEYS
        .sig hash_lookup_sig1 hash_lookup_sig2

        #if (KEY_SIZE <= 48)
            #define_eval _INDEX_SIZE HW_HASH_48
        #elif (KEY_SIZE <= 64)
            #define_eval _INDEX_SIZE HW_HASH_64
        #elif (KEY_SIZE <=128)
            #define_eval _INDEX_SIZE HW_HASH_128
        #else
            #error "Specified key size too large, valid values are <= 128"
    #endif

    // Optimization scope combine the two hash into one call
    hash_translate(in_key1[0], _INDEX_SIZE, hash_lookup_sig1, ___)
    hash_translate(in_key2[0], _INDEX_SIZE, hash_lookup_sig2, \
                    signals(hash_lookup_sig1, hash_lookup_sig2))

    #endif // HASH_DONT_TRANSLATE_KEYS

    // now call
    _hash_resolve_fold_16(out_index1, in_key1, KEY_SIZE, trie_base_addr, KEY_DATA_SD_BASE)
    _hash_resolve_fold_16(out_index2, in_key2, KEY_SIZE, trie_base_addr, KEY_DATA_SD_BASE)

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to extract the index-entry bits @<31:2@> from the a 32-bit word.
 *
 * @b Example:
 * @code
 * _get_entry_index(index, reg)
 * @endcode
 *
 * @param out_index     Resulting index or entry value
 * @param in_value      A register containing the input value
 *
 * @instruction_cnt 1 to 3
 */
#macro _get_entry_index(out_index, in_value)
    alu_op(out_index, in_value, AND, 0xfffffffc)
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to obtain a 1 thru 32 bit-field from a quad-word.
 *
 * @b Example:
 * @code
 * _get_index_field(index, word_lo, word_hi, 0, 16) // extracts the lower 16 bits
 * @endcode
 *
 * @param out_index     The resulting index field
 * @param in_word_lo    The lower 32 bits of the 64-bit value
 * @param in_word_hi    The upper 32 bits of the 64-bit value
 * @param in_start_bit  Starting bit position of the field
 * @param in_field_len  Number of bit in the field
 *
 * @note No checks are made to insure that the arguments are of reasonable values.
 *
 * @instruction_cnt 5 to 8
 */
#macro _get_index_field(out_index, in_word_lo, in_word_hi, in_start_bit, in_field_len)
.begin

    // Issue warnings if the names that we use are already defined
    #ifdef _BITS_IN_WORD
        #warning  "_BITS_IN_WORD is being redefined"
    #endif

    #ifdef _LAST_BIT_IN_WORD
        #warning  "_LAST_BIT_IN_WORD is being redefined"
    #endif


    #define_eval _BITS_IN_WORD      32                  // number of bits in the word
    #define_eval _LAST_BIT_IN_WORD  31                  // the last bit the word

    // if(start > _LAST_BIT_IN_WORD)
    sub(--, _LAST_BIT_IN_WORD, in_start_bit)
    bcc[wrd_hi#]

    // start <= _LAST_BIT_IN_WORD
    alu[--, in_start_bit, OR, 0]
    dbl_shf[out_index, in_word_hi, in_word_lo, >>indirect]
    br[trim_to_len#]

wrd_hi#:
    // start > _LAST_BIT_IN_WORD
    shf_right(out_index, in_word_hi, in_start_bit)

trim_to_len#:
    #if((is_ct_const(in_field_len)) || isimport(in_field_len))
        #define_eval _hdbm_tmp (_BITS_IN_WORD - in_field_len)
        shf_left(out_index, out_index, _hdbm_tmp)      //out_index <<= (_BITS_IN_WORD - _field_len)
        shf_right(out_index, out_index, _hdbm_tmp)     //out_index <<= (_BITS_IN_WORD - _field_len)
    #else
        .reg _hdbm_temp
        sub(_hdbm_temp, _BITS_IN_WORD, in_field_len)
        shf_left(out_index, out_index, _hdbm_temp)   //out_index <<= (_BITS_IN_WORD - _field_len)
        shf_right(out_index, out_index, _hdbm_temp)    //out_index <<= (_BITS_IN_WORD - _field_len)
    #endif

    #undef _BITS_IN_WORD
    #undef _LAST_BIT_IN_WORD

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to look up of folded index.
 *
 * SRAM will contain the folded index. DRAM will
 * contain the key, next chain (for collision) and user data.
 *
 * @param out_index             The resulting index field
 * @param in_key                Transfer registers, upper lower 32 bits of the search key
 * @param KEY_SIZE              CONST indicating the size of in_key parameter
 * @param trie_base_addr        Address of trie table, which is the sram primary base table
 * @param _PRIM_INDEX_BIT_LEN   GPR or CONST, primary table index bit length
 * @param KEY_DATA_SD_BASE      GPR or CONST, list of available SDRAM space to be used for key/data storage
 *
 * @note For complete memory view, see the description at _hash_resolve_fold_16().
 *
 * @instruction_cnt 12 to 19 instruction + 14*No Of Iteration (1 dram access/Iteration)
 */
#macro _hash_lookup_fold_16(out_index, in_key, KEY_SIZE, trie_base_addr, _PRIM_INDEX_BIT_LEN, KEY_DATA_SD_BASE)
.begin

    #if (_PRIM_INDEX_BIT_LEN != 16)
        #error "Not supported for values for PRIM_INDEX_BIT_LEN other than 16"
    #endif

    // Issue warnings if the names that we use are already defined
    #ifdef _INDEX_SIZE
        #warning  "_INDEX_SIZE is being redefined"
    #endif

    #ifdef _LOOKUP_READ_SIZE
        #warning "_LOOKUP_READ_SIZE is being redefined"
    #endif

    #ifdef _D_LOOKUP_READ_SIZE
        #warning "_D_LOOKUP_READ_SIZE is being redefined"
    #endif

    #if (KEY_SIZE <= 48)
        #define_eval _INDEX_SIZE HW_HASH_48
        #define_eval _LOOKUP_READ_SIZE 2
        #define_eval _D_LOOKUP_READ_SIZE 1
    #elif (KEY_SIZE <= 64)
        #define_eval _INDEX_SIZE HW_HASH_64
        #define_eval _LOOKUP_READ_SIZE 2
        #define_eval _D_LOOKUP_READ_SIZE 1
    #elif (KEY_SIZE <=128)
        #define_eval _INDEX_SIZE HW_HASH_128
        #define_eval _LOOKUP_READ_SIZE 4
        #define_eval _D_LOOKUP_READ_SIZE 2
    #else
        #error "Specified key size too large, valid values are <= 128"
    #endif

    // we skip hash translation when HASH_DONT_TRANSLATE_KEYS is defined
    #ifndef HASH_DONT_TRANSLATE_KEYS
        .sig hash_lookup_sig
        hash_translate(in_key[0], _INDEX_SIZE, hash_lookup_sig, hash_lookup_sig)
    #endif // HASH_DONT_TRANSLATE_KEYS

    _hash_resolve_fold_16(out_index, in_key, KEY_SIZE, trie_base_addr, KEY_DATA_SD_BASE)

    // Clean up our defines
    #undef _INDEX_SIZE
    #undef _LOOKUP_READ_SIZE
    #undef _D_LOOKUP_READ_SIZE

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to resolve folded index.
 *
 *
 * @param out_index         The resulting index field
 * @param in_key            Transfer registers, upper lower 32 bits of the search key
 * @param KEY_SIZE          CONST indicating the size of in_key parameter
 * @param trie_base_addr    Address of trie table, which is the sram primary base table
 * @param KEY_DATA_SD_BASE  GPR or CONST, list of available SDRAM space to be used for key/data storage
 *
 * @note SRAM will contain the folded index.
 *       For example if hashed folded value is 0x1234
 * @code
 *           ------------
 *          |            |===> (SRAM_TRIE BASE)
 *           ------------
 *          |            |
 *           ------------
 *          | Dram index |===> (SRAM_TRIE BASE+ 0x1234)
 *           ------------
 *          |            |
 *           ------------
 * @endcode
 *       DRAM will contain the key, next chain (for collision) and user data
 *
 *       For 48/64-bit keys, this will how the dram will look like
 * @code
 *           -----------
 *          |  key0     |===> (Dram index + KEY_DATA_SD_BASE)
 *           -----------
 *          |  key1     |
 *           -----------
 *          |  next     |===> For collision will point to next chained dram index
 *           -----------
 *          | user_data |
 *           -----------
 * @endcode
 *
 *       For 128-bit keys, this will how the dram will look like
 * @code
 *           -----------
 *          |  key00    |===> (Dram index + KEY_DATA_SD_BASE)
 *           -----------
 *          |  key01    |
 *           -----------
 *          |  key10    |
 *           -----------
 *          |  key11    |
 *           -----------
 *          |  next     |===> For collision will point to next chained dram index
 *           -----------
 *          | user_data |
 *           -----------
 * @endcode
 *
 * @instruction_cnt 11 to 17 ((1 sram access)+ 14*No Of Iteration (1 dram access/iteration))
 */
#macro _hash_resolve_fold_16(out_index, in_key, KEY_SIZE, trie_base_addr, KEY_DATA_SD_BASE)
.begin

    .reg tmp_filter1 tmp_filter2

    #if (KEY_SIZE <= 64)
        // Read 1 longword of data in addition to key (2lw for key+ 1lw for data)
        #define_eval KEY_DATA_READ_SIZE_LW  4
        #define_eval KEY_DATA_READ_SIZE_QW  2
        // Key size - 2 long words
        #define_eval KEY_SIZE_LW    2
    #else
        // Read 1 longword of data in addition to key (4lw for key + 1lw for data)
        #define_eval KEY_DATA_READ_SIZE_LW  6
        #define_eval KEY_DATA_READ_SIZE_QW  3
        // Key size - 4 long words
        #define_eval KEY_SIZE_LW    4
    #endif

    // Fold KEY_SIZE bits of hash result to the size of 16 (=PRIM_INDX_BIT_LEN)
    move(tmp_filter1, in_key[0])

    #ifdef _i
        #warning  "_i is being redefined"
    #endif

    #define_eval _i     1

    #while (_i < KEY_SIZE_LW)
        alu[tmp_filter1, in_key[/**/_i/**/], XOR, tmp_filter1]
        #define_eval _i _i+1
    #endloop


// Fold 32bit result until the index is 16 (=PRIM_INDX_BIT_LEN) long
//.reg folded_bits mask

    // Optimized for a 16bit table
    shf_right(tmp_filter2, tmp_filter1, 16)
    alu[tmp_filter1, tmp_filter1, XOR, tmp_filter2]
    ld_field_w_clr[tmp_filter2, 0011, tmp_filter1]

/*
// Generic folding mechanism - use this when PRIM_INDX_BIT_LEN is != 16
//  for (i = (32 - 1); i >= fieldLen; i--) {
//      index  = index ^ (index >> i)
//      index <<= (32 - i)
//      index >>= (32 - i)
//  }

    sub(folded_bits, 32, 1)
    .while (folded_bits >= PRIM_INDX_BIT_LEN)
        shf_right(tmp_filter2, tmp_filter1, folded_bits)
        alu[tmp_filter1, tmp_filter1, XOR, tmp_filter2]
        sub(mask, 32, folded_bits)
        shf_left(tmp_filter1, tmp_filter1, mask)
        shf_right(tmp_filter1, tmp_filter1, mask)
        sub(folded_bits, folded_bits, 1)
    .endw
*/
// fold_bits mask
    .reg $table_value
    .sig hash_lookup_sig

    shf_left(tmp_filter2, tmp_filter2, 2) // get byte offset

    // Lookup in SRAM primary table
    move(tmp_filter1, trie_base_addr) // set tmp_filter1 to PRIM_TABLE_BASE
    sram_read($table_value, tmp_filter1, tmp_filter2, 1, hash_lookup_sig, hash_lookup_sig, ___) ; lookup in Primary table

    // Get index without collision bit
    alu_op(tmp_filter1, $table_value, AND, 0xfffffffc)
    beq[return_error_no_match#], defer[1]

    #if (defined(__NFP_INDIRECT_REF_FORMAT_V1))
        #define _DXFER   $xkey_data
    #else
        #define _DXFER   $$xkey_data
    #endif

    xbuf_alloc(_DXFER, KEY_DATA_READ_SIZE_LW, read)

    #if (KEY_SIZE <= 64)
        // Read 1 longword of data in addition to key (2lw for key+ 1lw for data)
        #define_eval KEY_DATA_READ_SIZE_LW  4
        #define_eval KEY_DATA_READ_SIZE_QW  2
        // Key size - 2 long words
        #define_eval KEY_SIZE_LW    2
    #else
        // Read 1 longword of data in addition to key (4lw for key + 1lw for data)
        #define_eval KEY_DATA_READ_SIZE_LW  6
        #define_eval KEY_DATA_READ_SIZE_QW  3
        // Key size - 4 long words
        #define_eval KEY_SIZE_LW    4

    #endif
    .sig hdbm_resolve_sig
    .reg temp_in_key

    // Note: This loop is terminated when a match is found or no match
continue_hash_resolve_fold_16#:

    add(tmp_filter1, tmp_filter1, KEY_DATA_SD_BASE)

    // NOTE: we do not subtract 1, meaning zero is an invalid entry
    // sub(tmp_filter1, tmp_filter1, 1)

    // Read KEY_SIZE bits of keydata and "next" pointer chained due to collision (burst of KEY_DATA_READ_QW quad words)
    dram_read(_DXFER[0], tmp_filter1, 0, KEY_DATA_READ_SIZE_QW, hdbm_resolve_sig, hdbm_resolve_sig, ___) ;read entry key

    #ifdef _DEFER_VAL
        #warning "_DEFER_VAL is being redefined"
    #endif

    #define_eval _i 0
    #while (_i < KEY_SIZE_LW)
        move(temp_in_key, _DXFER[/**/_i/**/])
        comp(temp_in_key, in_key[/**/_i/**/])

        #if (_i == (KEY_SIZE_LW-1))
            #define_eval _DEFER_VAL  1
        #else
            #define_eval _DEFER_VAL  2
        #endif

        bne[match_not_found#], defer[/**/_DEFER_VAL/**/]
        #define_eval _i _i+1
    #endloop

    // Increment the index since the user data resides after the <next> field
    #define_eval _i _i+1
    // copy the data portion (32-bit) on a successful match
    move(out_index, _DXFER[/**/_i/**/])

    br[exit_hash_resolve_fold_16#]

match_not_found#:
    // If comparison is not true check if the "next" field has a valid value
    // And valid "next" value present lookup the chained SDRAM entry otherwise
    // error out
    alu[tmp_filter1, --, B, _DXFER[/**/KEY_SIZE_LW/**/]]
    bne[continue_hash_resolve_fold_16#], defer[1]

return_error_no_match#:
    // If no match, return 0
    move(out_index, 0)

exit_hash_resolve_fold_16#:

    // clean up the name space

    #ifdef _i
        #undef _i
    #endif

    #ifdef _DEFER_VAL
        #undef _DEFER_VAL
    #endif

    xbuf_free(_DXFER)

    #ifdef _DXFER
        #undef _DXFER
    #endif

.end
#endm
/// @endcond


/** @}
 * @}
 */

#endif /* __HASH_UC__ */
