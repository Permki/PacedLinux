/*
 * Copyright (C) 2011-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __PUSH_COUNTERS_UC__
#define __PUSH_COUNTERS_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <push_counters.h>
#include <constants.uc>

/** @file push_counters.uc Push Counter Macros
 * @addtogroup push_counters Push Counters
 * @{
 *
 * @name Push Counter Macros
 * @{
 *
 * This is a counter framework that supports pushing local copies to a faster
 * memory and then later pushing the counter updates to a global memory.
 * Example: incrementing stats in cluster scratch, but then updating DRAM as
 * the master copies.  "Push counters" may saturate on overflow or underflow
 * depending on the local and global memory choices.  The framework assumes
 * that counters can be updated by adding (modulo the counter width) local
 * copies atomically to the global copy.
 *
 * This framework can just as easily be used without caching local copies.
 * See push_counters.h for the macros to declare counter sets.  The
 * framework represents a counter set by a #define token ending in _PCSD.  It
 * represents a counter by a #define token that refers to a value that a
 * PUSH_COUNTER() or PUSH_COUNTER64() macro generates.
 *
 * Global operations (pctr_reset_global() and pctr_push()) will become empty
 * macros if the _PCSD defines a global memory location of 'NONE'.   MAXX is
 * the maximum number of transfer registers to use at a time while modifying
 * memory in bulk.  pctr_*() macros that take a 'sig' value will not wait on
 * the signal.  The caller must ensure that the signal returns before reusing
 * the corresponding xfer argument.
 *
 * For the pctr_add/sub*() macros, the amount argument 'amt' must generally
 * either be a small (<= 127) constant or stored in a register.  This is due
 * to the instruction requirements for the fast arithmetic operations.  If
 * the counter is 64-bit, instructions can be saved if the constant is <= 7.
 *
 * For each of the above macros there is a related macro that has the exact
 * same arguments and whose name is the name of the macro suffixed by _dbg.
 * For example, there is a pctr_incr_dbg() macro that corresponds to the
 * pctr_incr() macro.  These macros behave exactly the same way as their
 * base counterparts except that if PCTR_DEBUG is not #defined when the
 * macro expands, the macro will produce and empty string.  This makes it
 * easy to conditionally compile in and remove macros from microcode.
 * Debug macros can reside in the same counter set or in their own.
 *
 */

// API Quick Reference:
//
// pctr_reset_global(CTRSET_PCSD, MAXX) - reset global counter mem (to 0).
// pctr_reset_local(CTRSET_PCSD, MAXX) - reset local counter mem (to 0).
// pctr_set(COUNTER, val, xfer, sig) - atomically set a counter.
// pctr_incr(COUNTER) - atomically increment a counter.
// pctr_decr(COUNTER) - atomically decrement a counter.
// pctr_add(COUNTER, amt, xfer, sig) - atomically add to a counter.
// pctr_sub(COUNTER, amt, xfer, sig) - atomically subtract from a counter.
// pctr_add_fast(COUNTER, amt) - fast counter add (no xfer regs or signals).
// pctr_sub_fast(COUNTER, amt) - fast counter sub (no xfer regs or signals).
// pctr_push(CTRSET_PCSD, MAXX) - push local copies of counters to global.

/// @cond INTERNAL_MACROS
/* Basically immed32(), but rewritten to avoid dependency */
#macro _pctr_load_addr(reg, VAL)
    #define     _VAL    0
    #define_eval _VAL   VAL
    #if (!is_ct_const(VAL))
        #error "Invalid address (not a number):" VAL
    #endif
    immed[reg, (VAL & 0xFFFF)]
    #if (VAL > 0xFFFF)
        immed_w1[reg, ((VAL >> 16) & 0xFFFF)]
    #endif
    #undef _VAL
#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _pctr_validate_descriptor(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64)
    #if (( IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) && \
            !( streq(G_MEM, mem) || streq(G_MEM, none) ))
        #error "_pctr_validate_descriptor: only G_MEM type mem or none is supported on NFP6000."
    #endif

    #if (( IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) && \
            !( streq(L_MEM, cls) || streq(L_MEM, sram) ))
        #error "_pctr_validate_descriptor: only L_MEM type cls or sram is supported on NFP6000."
    #endif

    #if ((NUM32 + NUM64 < 1) || (NUM32 < 0) || (NUM64 < 0))
        #error "Invalid numbers of counters."
    #endif

    #if (NUM32 + NUM64 * 2 >= 16384)
        #error "Too many counters: " NUM32 32-bit, NUM64 64-bit
    #endif

    #if ((NUM64 > 0) && ((NUM32 % 2) != 0))
        #error "NUM32 is not an even # when there are 64-bit counters"
    #endif

    #if (streq(L_MEM, 'dram') || streq(L_MEM, 'mem') || streq(L_MEM, 'dr'))
        #error "L_MEM of dram not supported"
    #endif

    #if (NUM64 > 0)
        #if (streq(L_MEM, 'scratch') || streq(L_MEM, 'gs') || streq(L_MEM, 'sr') || \
            streq(L_MEM, 'sram') || streq(G_MEM, 'scratch') || \
            streq(G_MEM, 'gs') || streq(G_MEM, 'sr') || streq(G_MEM, 'sram'))
            #error "Global scratch and SRAM local memories don't support 64-bit counters"
        #endif
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _pctr_validate_index(NUM32, NUM64, IDX, IS64)
    #if (IS64)
        #if (IDX >= NUM64)
            #error "Counter out of range: " IDX > NUM64
        #endif
    #else
        #if (IDX >= NUM32)
            #error "Counter out of range: " IDX > NUM32
        #endif
    #endif
#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _pctr_op(MLOC, OP, xfer, addr1, addr2, LEN, sig)
    #define _REFCNT     LEN
    #if ((streq(MLOC, 'dram') || streq(MLOC, 'mem') || streq(MLOC, 'dr')) && \
        (streq(OP, 'read') || streq(OP, 'write')))

        #define_eval _REFCNT    (LEN / 2)

        #if (defined(__NFP_INDIRECT_REF_FORMAT_V1) || defined(__NFP_INDIRECT_REF_FORMAT_V2))
            #if (((_REFCNT - 1) > 0x1F) || (_REFCNT < 1))
                #error "LEN invalid, must be 1 <= LEN <= 64"
            #endif
        #else
            #if (((_REFCNT - 1) > 0xF) || (_REFCNT < 1))
                #error "LEN invalid, must be 1 <= LEN <= 32"
            #endif
        #endif

    #else
        #define_eval _REFCNT    LEN

        #if (defined(__NFP_INDIRECT_REF_FORMAT_V1) || defined(__NFP_INDIRECT_REF_FORMAT_V2))
            #if (((_REFCNT - 1) > 0x1F) || (_REFCNT < 1))
                #error "LEN invalid, must be 1 <= LEN <= 32"
            #endif
        #else
            #if (((_REFCNT - 1) > 0xF) || (_REFCNT < 1))
                #error "LEN invalid, must be 1 <= LEN <= 16"
            #endif
        #endif

    #endif
    #if (_REFCNT > 8)
        #if (defined(__NFP_INDIRECT_REF_FORMAT_V1))
            .begin
                .reg _ind_ref
                immed[_ind_ref, (_REFCNT-1)]
                alu[--, _ind_ref, OR, 0x2, <<28]
                MLOC[OP, xfer, addr1, addr2, max_/**/_REFCNT], sig_done[sig], indirect_ref
            .end
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_V2))
            .begin
                .reg _ind_ref
                immed[_ind_ref, (_REFCNT-1)]
                alu[--, (1<<7), OR, _ind_ref, <<8]
                MLOC[OP, xfer, addr1, addr2, max_/**/_REFCNT], sig_done[sig], indirect_ref
            .end
        #else
            alu[--, --, B, (0x10|(_REFCNT-1)), <<21]
            MLOC[OP, xfer, addr1, addr2, max_/**/_REFCNT], sig_done[sig], indirect_ref
        #endif
    #else
        /* For scratch/sram operations, there is no reference count field */
        #if ((streq(MLOC, 'gs') || streq(MLOC, 'scratch') || \
            streq(MLOC, 'sr') || streq(MLOC, 'sram')) && \
            !streq(OP, 'read') && !streq(OP, 'write'))
            MLOC[OP, xfer, addr1, addr2], sig_done[sig]
        #else
            MLOC[OP, xfer, addr1, addr2, _REFCNT], sig_done[sig]
        #endif
    #endif
    #undef _REFCNT
#endm
/// @endcond


/// @cond INTERNAL_MACROS
#macro _pctr_reset_mem(MLOC, BASE, LEN, MAXX)
.begin

    #if ((MAXX < 1) || (MAXX > 16))
        #error  "MAXX must be between 1 and 16"
    #endif
    #if (MAXX < LEN)
        #define_eval _PCTR_DECLEN       MAXX
    #else
        #define_eval _PCTR_DECLEN       LEN
    #endif

    #if (streq(MLOC, 'dram') || streq(MLOC, 'mem'))
        #if (defined(__NFP_INDIRECT_REF_FORMAT_V1) || defined(__NFP_INDIRECT_REF_FORMAT_V2))
            #define _XFER   $x
        #else
            #define _XFER   $$x
        #endif
    #else
        #define _XFER   $x
    #endif

    .reg addr
    .reg write _XFER[_PCTR_DECLEN]
    .xfer_order _XFER
    .sig wrdone

    #define     _PCTR_LOOP      0
    #while (_PCTR_LOOP < _PCTR_DECLEN)
        immed[_XFER[_PCTR_LOOP], 0]
        #define_eval _PCTR_LOOP (_PCTR_LOOP + 1)
    #endloop
    #undef _PCTR_LOOP

    immed[addr, (BASE & 0xFFFF)]
    immed_w1[addr, ((BASE >> 16) & 0xFFFF)]

    #if (MAXX < LEN)
        .reg ctr
        immed[ctr, (LEN / MAXX)]
        .while (ctr > 0)
            _pctr_op(MLOC, write, _XFER[0], addr, 0, MAXX, wrdone)
            alu[addr, addr, +, (MAXX * 4)]
            alu[ctr, ctr, -, 1]
            ctx_arb[wrdone]
        .endw

        #define_eval _PCTR_REM  (LEN % MAXX)
    #else
        #define_eval _PCTR_REM  LEN
    #endif

    #if (_PCTR_REM > 0)
        _pctr_op(MLOC, write, _XFER[0], addr, 0, _PCTR_REM, wrdone)
        ctx_arb[wrdone]
    #endif

    #undef _XFER
    #undef _PCTR_REM

.end
#endm
/// @endcond


#macro pctr_reset_global(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, MAXX)
    _pctr_validate_descriptor(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64)
    #if (streq(L_MEM, 'dram') || streq(L_MEM, 'mem') || streq(L_MEM, 'dr'))
        #error "L_MEM of dram not supported"
    #endif
    #if (!streq(G_MEM, 'NONE'))
        #define _TLEN       0
        #define_eval _TLEN (NUM32 + NUM64 * 2)
        _pctr_reset_mem(G_MEM, G_BASE, _TLEN, MAXX)
        #undef _TLEN
    #endif
#endm


#macro pctr_reset_global_dbg(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, MAXX)
    #ifdef PCTR_DEBUG
        pctr_reset_global(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, MAXX)
    #endif
#endm


#macro pctr_reset_local(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, MAXX)
    _pctr_validate_descriptor(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64)
    #if (streq(L_MEM, 'dram') || streq(L_MEM, 'mem') || streq(L_MEM, 'dr'))
        #error "L_MEM of dram not supported"
    #endif
    #define _TLEN       0
    #define_eval _TLEN (NUM32 + NUM64 * 2)
    _pctr_reset_mem(L_MEM, L_BASE, _TLEN, MAXX)
    #undef _TLEN
#endm


#macro pctr_reset_local_dbg(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, MAXX)
    #ifdef PCTR_DEBUG
        pctr_reset_local(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, MAXX)
    #endif
#endm


#macro pctr_set(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, val, xfer, sig)
    _pctr_validate_descriptor(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64)
    _pctr_validate_index(NUM32, NUM64, IDX, IS64)
    .begin

        #define _IDX        0
        #define _XFER       0
        #if (IS64)
            #define_eval _IDX        (NUM32 + (IDX * 2))
            alu[xfer[0], --, B, val[0]]
            alu[xfer[1], --, B, val[1]]
            #define_eval _XFER xfer[0]
        #else
            #define_eval _IDX        IDX
            alu[xfer, --, B, val]
            #define_eval _XFER xfer
        #endif

        .reg addr
        _pctr_load_addr(addr, (L_BASE + (_IDX * 4)))
        #if (IS64)
            #if (streq(L_MEM, 'dram') || streq(L_MEM, 'mem') || streq(L_MEM, 'dr'))
                L_MEM[write, _XFER, addr, 0, 1], sig_done[sig]
            #else
                L_MEM[write, _XFER, addr, 0, 2], sig_done[sig]
            #endif
        #else
            L_MEM[write, _XFER, addr, 0, 1], sig_done[sig]
        #endif
        #undef _IDX
        #undef _XFER

    .end
#endm


#macro pctr_set_dbg(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, val, xfer, sig)
    #ifdef PCTR_DEBUG
        pctr_set(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, val, xfer, sig)
    #endif
#endm


#macro pctr_incr(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64)
    _pctr_validate_descriptor(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64)
    _pctr_validate_index(NUM32, NUM64, IDX, IS64)
    .begin

        #define _IDX        0
        #if (IS64)
            #define_eval _IDX        (NUM32 + (IDX * 2))
        #else
            #define_eval _IDX        IDX
        #endif

        .reg addr
        _pctr_load_addr(addr, (L_BASE + (_IDX * 4)))
        #if (IS64)
            L_MEM[add64_imm, --, 0, addr, 1]
        #elif (streq(L_MEM, 'scratch') || streq(L_MEM, 'gs') || \
            streq(L_MEM, 'sram') || streq(L_MEM, 'sr'))
            L_MEM[incr, --, addr, 0]
        #else /* cls or mem */
            L_MEM[add_imm, --, 0, addr, 1]
        #endif
        #undef _IDX

    .end
#endm


#macro pctr_incr_dbg(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64)
    #ifdef PCTR_DEBUG
        pctr_incr(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64)
    #endif
#endm


#macro pctr_decr(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64)
    _pctr_validate_descriptor(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64)
    _pctr_validate_index(NUM32, NUM64, IDX, IS64)
    .begin

        #define _IDX        0
        #if (IS64)
            #define_eval _IDX        (NUM32 + (IDX * 2))
        #else
            #define_eval _IDX        IDX
        #endif

        .reg addr
        _pctr_load_addr(addr, (L_BASE + (_IDX * 4)))
        #if (IS64)
            L_MEM[sub64_imm, --, 0, addr, 1]
        #elif (streq(L_MEM, 'scratch') || streq(L_MEM, 'gs') || \
            streq(L_MEM, 'sram') || streq(L_MEM, 'sr'))
            L_MEM[decr, --, addr, 0]
        #else /* cls or mem */
            L_MEM[sub_imm, --, 0, addr, 1]
        #endif
        #undef _IDX

    .end
#endm


#macro pctr_decr_dbg(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64)
    #ifdef PCTR_DEBUG
        pctr_decr(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64)
    #endif
#endm


#macro pctr_add(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, amt, xfer, sig)
    _pctr_validate_descriptor(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64)
    _pctr_validate_index(NUM32, NUM64, IDX, IS64)
    .begin

        #define _IDX        0
        #define _XFER       0
        #if (IS64)
            #define_eval _IDX        (NUM32 + (IDX * 2))
            immed[xfer[1], 0]
            alu[xfer[0], --, B, amt]
            #define_eval _XFER xfer[0]
        #else
            #define_eval _IDX        IDX
            alu[xfer, --, B, amt]
            #define_eval _XFER xfer
        #endif

        .reg addr
        _pctr_load_addr(addr, (L_BASE + (_IDX * 4)))
        #if (IS64)
            L_MEM[add64, _XFER, addr, 0, 2], sig_done[sig]
        #elif (streq(L_MEM, 'scratch') || streq(L_MEM, 'gs') || \
            streq(L_MEM, 'sram') || streq(L_MEM, 'sr'))
            L_MEM[add, _XFER, addr, 0], sig_done[sig]
        #else /* cls or mem */
            L_MEM[add, _XFER, addr, 0, 1], sig_done[sig]
        #endif
        #undef _IDX
        #undef _XFER

    .end
#endm


#macro pctr_add_dbg(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, amt, xfer, sig)
    #ifdef PCTR_DEBUG
        pctr_add(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, amt, xfer, sig)
    #endif
#endm


#macro pctr_sub(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, amt, xfer, sig)
    _pctr_validate_descriptor(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64)
    _pctr_validate_index(NUM32, NUM64, IDX, IS64)
    .begin

        #define _IDX        0
        #define _XFER       0
        #if (IS64)
            #define_eval _IDX        (NUM32 + (IDX * 2))
            immed[xfer[1], 0]
            alu[xfer[0], --, B, amt]
            #define_eval _XFER xfer[0]
        #else
            #define_eval _IDX        IDX
            alu[xfer, --, B, amt]
            #define_eval _XFER xfer
        #endif

        .reg addr
        _pctr_load_addr(addr, (L_BASE + (_IDX * 4)))
        #if (IS64)
            L_MEM[sub64, _XFER, addr, 0, 2], sig_done[sig]
        #elif (streq(L_MEM, 'scratch') || streq(L_MEM, 'gs') || \
            streq(L_MEM, 'sram') || streq(L_MEM, 'sr'))
            L_MEM[sub, xfer, addr, 0], sig_done[sig]
        #else /* cls or mem */
            L_MEM[sub, _XFER, addr, 0, 1], sig_done[sig]
        #endif
        #undef _IDX
        #undef _XFER

    .end
#endm


#macro pctr_sub_dbg(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, amt, xfer, sig)
    #ifdef PCTR_DEBUG
        pctr_sub(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, amt, xfer, sig)
    #endif
#endm


#macro pctr_add_fast(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, amt)
    _pctr_validate_descriptor(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64)
    _pctr_validate_index(NUM32, NUM64, IDX, IS64)
    #if (streq(L_MEM, 'scratch') || streq(L_MEM, 'gs'))
        #error "Global scratch and SRAM can't do fast adds"
    #endif
    #if (streq(L_MEM, 'dram') || streq(L_MEM, 'mem') || streq(L_MEM, 'dr'))
        #error "L_MEM of dram not supported"
    #endif

    .begin

        #define _IDX        0
        #if (IS64)
            #define_eval _IDX        (NUM32 + (IDX * 2))
        #else
            #define_eval _IDX        IDX
        #endif

        .reg addr
        _pctr_load_addr(addr, (L_BASE + (_IDX * 4)))

        #if (IS64)
            #if (is_ct_const(amt) && (amt <= 7) && \
                  !(IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
                L_MEM[add64_imm, --, 0, addr, amt]
            #else
                #if (defined(__NFP_INDIRECT_REF_FORMAT_IXP))
                    #error "This part of the macro is currently only available in NFP indirect reference format mode."
                #endif
                .reg _pctr_tmp
                #if ( defined(__NFP_INDIRECT_REF_FORMAT_V2))
                    #if is_ct_const(amt)
                        alu[_pctr_tmp, --, B, amt, <<16]
                        alu[--, _pctr_tmp, OR, 2, <<3]
                    #else
                        alu[--, (2<<3), OR, amt, <<16]
                    #endif
                #else
                    alu[_pctr_tmp, --, B, amt, <<5]
                    alu[--, _pctr_tmp, OR, 8, <<28]
                #endif
                L_MEM[add64_imm, --, 0, addr, 0], indirect_ref
            #endif
        #elif (streq(L_MEM, 'sram') || streq(L_MEM, 'sr'))
            .reg _np
            #if ( defined(__NFP_INDIRECT_REF_FORMAT_V2)
                    .reg _msk, _tmp
                    immed[_msk, 0xFF]
                    alu[_np, _msk, AND, amt]
                    alu[_np, --, B, _np, <<16]
                    immed[_msk, 0x700]
                    alu[_tmp, _msk, AND, amt]
                    alu[_np, _nfp, OR, _tmp]
                    alu[_np, _np, OR, 6, <<3]
                    alu[_np, _np, OR, 8, <<8]
            #elif (defined(__NFP_INDIRECT_REF_FORMAT_V1))
                #if (is_ct_const(amt))
                    immed[_np, ((0x800) | (amt & 0x7FF))]
                    alu[_np, _np, OR, 0x3, <<28]
                #else
                    .reg _msk
                    immed[_msk, 0x7FF]
                    alu[_np, _msk, AND, amt]
                    alu[_np, _np, OR, 1, <<(8+3)]
                    alu[_np, _np, OR, 0x3, <<28]
                #endif
            #else
                #if (is_ct_const(amt))
                    alu[_np, --, B, (amt & 0xFF), <<12]
                    alu[--, _np, OR, (0x31 | ((amt >> 7) & 0xF)), <<20]
                #else
                    .reg _hi
                    alu[_np, amt, and, 0xFF]
                    alu[_hi, 0x31, OR, amt, >>7]
                    alu[_np, _np, OR, _hi, <<8]
                    alu[_np, --, B, _np, <<12]
                #endif
            #endif
            sram[add, --, addr, 0], no_pull, indirect_ref
        #else /* cls */
            #if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
                #if (is_ct_const(amt) && (amt <= 7))
                    L_MEM[add_imm, --, addr, 0, amt]
                #else
                    .reg _pctr_tmp
                    #if ( defined(__NFP_INDIRECT_REF_FORMAT_V2))
                        #if (is_ct_const(amt))
                            alu[_pctr_tmp, --, B, amt, <<16]
                            alu[--, _pctr_tmp, OR, 2, <<3]
                        #else
                            alu[--, (2<<3), OR, amt, <<16]
                        #endif
                    #else
                        alu[_pctr_tmp, --, B, amt, <<5]
                        alu[--, _pctr_tmp, OR, 8, <<28]
                    #endif
                    L_MEM[add_imm, --, addr, 0, 0], indirect_ref
                #endif
            #else
                #if (is_ct_const(amt) && (amt <= 7))
                    L_MEM[add_imm, --, addr, 0, amt]
                #else
                    .reg _pctr_tmp
                    alu[_pctr_tmp, --, B, amt, <<5]
                    alu[--, _pctr_tmp, OR, 8, <<28]
                    L_MEM[add_imm, --, addr, 0, 1], indirect_ref
               #endif
            #endif
        #endif

        #undef _IDX

    .end
#endm


#macro pctr_add_fast_dbg(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, amt)
    #ifdef PCTR_DEBUG
        pctr_add_fast(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, amt)
    #endif
#endm


#macro pctr_sub_fast(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, amt)
    _pctr_validate_descriptor(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64)
    _pctr_validate_index(NUM32, NUM64, IDX, IS64)
    #if (streq(L_MEM, 'scratch') || streq(L_MEM, 'gs') || \
        streq(L_MEM, 'sram') || streq(L_MEM, 'sr') || streq(L_MEM, 'qdr'))
        #error "Global scratch and SRAM can't do fast subtracts"
    #endif
    #if (streq(L_MEM, 'dram') || streq(L_MEM, 'mem') || streq(L_MEM, 'dr'))
        #error "L_MEM of dram not supported"
    #endif

    .begin

        #define _IDX        0
        #if (IS64)
            #define_eval _IDX        (NUM32 + (IDX * 2))
        #else
            #define_eval _IDX        IDX
        #endif

        .reg addr
        _pctr_load_addr(addr, (L_BASE + (_IDX * 4)))

        #if (IS64)
            #if (is_ct_const(amt) && (amt <= 7) && \
                  !(IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)))
                L_MEM[sub64_imm, --, 0, addr, amt]
            #else
                #if (defined(__NFP_INDIRECT_REF_FORMAT_IXP))
                    #error "This part of the macro is currently only available in NFP indirect reference format mode."
                #endif
                .reg _pctr_tmp
                #if ( defined(__NFP_INDIRECT_REF_FORMAT_V2))
                    #if (is_ct_const(amt))
                        alu[_pctr_tmp, --, B, amt, <<16]
                        alu[--, _pctr_tmp, OR, 2, <<3]
                    #else
                        alu[--, (2<<3), OR, amt, <<16]
                    #endif
                #else
                    alu[_pctr_tmp, --, B, amt, <<5]
                    alu[--, _pctr_tmp, OR, 8, <<28]
                #endif
                L_MEM[sub64_imm, --, addr, 0, 0], indirect_ref
            #endif
        #else /* cls */
            #if ((IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)))
                #if (is_ct_const(amt) && (amt <= 7))
                    L_MEM[sub_imm, --, addr, 0, amt]
                #else
                    .reg _pctr_tmp
                    #if ( defined(__NFP_INDIRECT_REF_FORMAT_V2))
                        #if (is_ct_const(amt))
                            alu[_pctr_tmp, --, B, amt, <<16]
                            alu[--, _pctr_tmp, OR, 2, <<3]
                        #else
                            alu[--, (2<<3), OR, amt, <<16]
                        #endif
                    #else
                        alu[_pctr_tmp, --, B, amt, <<5]
                        alu[--, _pctr_tmp, OR, 8, <<28]
                    #endif
                    L_MEM[sub_imm, --, addr, 0, 0], indirect_ref
                #endif
            #else
                #if (is_ct_const(amt) && (amt <= 7))
                    L_MEM[sub_imm, --, addr, 0, amt]
                #else
                    .reg _pctr_tmp
                    alu[_pctr_tmp, --, B, amt, <<5]
                    alu[--, _pctr_tmp, OR, 8, <<28]
                    L_MEM[sub_imm, --, addr, 0, 1], indirect_ref
               #endif
            #endif
        #endif

        #undef _IDX

    .end
#endm


#macro pctr_sub_fast_dbg(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, amt)
    #ifdef PCTR_DEBUG
        pctr_sub_fast(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, IDX, IS64, amt)
    #endif
#endm


#macro pctr_push(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64, MAXX)
    _pctr_validate_descriptor(NAME, G_MEM, G_BASE, L_MEM, L_BASE, NUM32, NUM64)

    #if (streq(G_MEM, 'dram'))
        #error "G_MEM cannot be 'dram'. Use 'mem' instead."
    #endif

    #if (streq(L_MEM, 'cls'))
        #define_eval _PCTR_MAXSIG 15
    #else
        #define_eval _PCTR_MAXSIG 7
    #endif

    #if (!streq(G_MEM, 'NONE'))
    .begin

        #if ((MAXX < 1) || (MAXX > 16))
            #error  "MAXX must be between 1 and 16"
        #endif
        #if ((NUM64 > 0) && ((MAXX % 2) != 0))
            #define_eval MAXX       (MAXX - 1)
        #endif
        #if ((MAXX < 2) && (NUM64 > 0))
            #error "MAXX < 2, the there are 64-bit counters"
        #endif
        #if (NUM32 > NUM64*2)
            #if (MAXX > NUM32)
                #define_eval _PCTR_DECLEN       NUM32
            #else
                #define_eval _PCTR_DECLEN       MAXX
            #endif
        #else /* NUM32 > NUM64 */
            #if (MAXX > NUM64*2)
                #define_eval _PCTR_DECLEN       NUM64*2
            #else
                #define_eval _PCTR_DECLEN       MAXX
            #endif
        #endif /* NUM32 > NUM64*2 */

        #if (streq(G_MEM, 'mem'))
            #if (defined(__NFP_INDIRECT_REF_FORMAT_V1))
                #define _OX     $$out
            #else
                #define _OX     $out
            #endif
            #define _IX     $in

            .reg write _OX[_PCTR_DECLEN]
            .xfer_order _OX
            .reg _IX[_PCTR_DECLEN]
            .xfer_order _IX
        #else
            #define _OX     $x
            #define _IX     $x
            .reg _IX[_PCTR_DECLEN]
            .xfer_order _IX
        #endif

        .reg iaddr oaddr

        immed[iaddr, (L_BASE & 0xFFFF)]
        immed_w1[iaddr, ((L_BASE >> 16) & 0xFFFF)]
        immed[oaddr, (G_BASE & 0xFFFF)]
        immed_w1[oaddr, ((G_BASE >> 16) & 0xFFFF)]


        /* --------------- First update 32-bit counters ------------------ */
        #if (NUM32 > 0)
            /* we may max out due to lack fo signals */
            #if (_PCTR_DECLEN >= _PCTR_MAXSIG)
                #define_eval _PCTR_BULK32_LEN _PCTR_MAXSIG
            #else
                #define_eval _PCTR_BULK32_LEN _PCTR_DECLEN
            #endif
            .begin

                /* UGH: Define a list of all signals */
                #define _PCTR_LOOP      0
                #define _SIGLIST ''
                #define _SIGTMP ''
                #while (_PCTR_LOOP < _PCTR_BULK32_LEN)
                    .sig iodone_/**/_PCTR_LOOP
                    #if (_PCTR_LOOP == 0)
                        #define_eval _SIGTMP        'iodone_/**/_PCTR_LOOP'
                        #define_eval _SIGLIST       '_SIGTMP'
                    #else
                        #define_eval _SIGTMP        '_SIGLIST,iodone_/**/_PCTR_LOOP'
                        #define_eval _SIGLIST       '_SIGTMP'
                    #endif
                    #define_eval _PCTR_LOOP (_PCTR_LOOP + 1)
                #endloop
                #undef _PCTR_LOOP

                #if (_PCTR_BULK32_LEN <= NUM32)
                    .reg ctr
                    immed[ctr, (NUM32 / _PCTR_BULK32_LEN)]
                    .while (ctr > 0)
                        alu[ctr, ctr, -, 1]

                        /* unroll a bunch of writes */
                        #define _PCTR_LOOP      0
                        #while (_PCTR_LOOP < _PCTR_BULK32_LEN)
                            immed[_IX[_PCTR_LOOP], 0xFFFFFFFF]
                            _pctr_op(L_MEM, test_and_clr, _IX[_PCTR_LOOP],
                                     iaddr, 0, 1, iodone_/**/_PCTR_LOOP)
                            alu[iaddr, iaddr, +, 4]
                            #define_eval _PCTR_LOOP (_PCTR_LOOP + 1)
                        #endloop
                        #undef _PCTR_LOOP
                        ctx_arb[_SIGLIST], ALL

                        /* unroll a bunch of writes */
                        #define _PCTR_LOOP  0
                        #while (_PCTR_LOOP < _PCTR_BULK32_LEN)
                            /* XXX If the prevous op was a test and set, _IX should be set! */
                            .set _IX[_PCTR_LOOP]
                            alu[_OX[_PCTR_LOOP], --, B, _IX[_PCTR_LOOP]]
                            _pctr_op(G_MEM, add, _OX[_PCTR_LOOP], oaddr, 0, 1,
                                iodone_/**/_PCTR_LOOP)
                            alu[oaddr, oaddr, +, 4]
                            #define_eval _PCTR_LOOP (_PCTR_LOOP + 1)
                        #endloop
                        #undef _PCTR_LOOP
                        ctx_arb[_SIGLIST], ALL

                    .endw
                #endif
                #define_eval _PCTR_REM      (NUM32 % _PCTR_BULK32_LEN)

                #if (_PCTR_REM > 0)
                    #define     _PCTR_LOOP      0
                    #define_eval _SIGLIST ''
                    #while (_PCTR_LOOP < _PCTR_REM)
                        #if (_PCTR_LOOP == 0)
                            #define_eval _SIGTMP        'iodone_/**/_PCTR_LOOP'
                            #define_eval _SIGLIST       '_SIGTMP'
                        #else
                            #define_eval _SIGTMP        '_SIGLIST,iodone_/**/_PCTR_LOOP'
                            #define_eval _SIGLIST       '_SIGTMP'
                        #endif
                        #define_eval _PCTR_LOOP (_PCTR_LOOP + 1)
                    #endloop
                    #undef _PCTR_LOOP

                    /* unroll a bunch of writes */
                    #define     _PCTR_LOOP      0
                    #while (_PCTR_LOOP < _PCTR_REM)
                        immed[_IX[_PCTR_LOOP], 0xFFFFFFFF]
                        _pctr_op(L_MEM, test_and_clr, _IX[_PCTR_LOOP],
                                 iaddr, 0, 1, iodone_/**/_PCTR_LOOP)
                        alu[iaddr, iaddr, +, 4]
                        #define_eval _PCTR_LOOP (_PCTR_LOOP + 1)
                    #endloop
                    #undef _PCTR_LOOP
                    ctx_arb[_SIGLIST], ALL

                    #define     _PCTR_LOOP      0
                    #while (_PCTR_LOOP < _PCTR_REM)
                        /* XXX If the prevous op was a test and set, _IX should be set! */
                        .set _IX[_PCTR_LOOP]
                        alu[_OX[_PCTR_LOOP], --, B, _IX[_PCTR_LOOP]]
                        _pctr_op(G_MEM, add, _OX[_PCTR_LOOP], oaddr, 0, 1,
                                iodone_/**/_PCTR_LOOP)
                        alu[oaddr, oaddr, +, 4]
                        #define_eval _PCTR_LOOP (_PCTR_LOOP + 1)
                    #endloop
                    #undef _PCTR_LOOP
                    ctx_arb[_SIGLIST], ALL
                #endif

            .end
            #undef _PCTR_BULK32_LEN
        #endif /* (NUM32 > 0) */

        /* --------------- Next, update 64-bit counters ------------------- */
        #if (NUM64 > 0)
            /* Now we have to regard the number of registers as half */
            #if ((_PCTR_DECLEN / 2) >= _PCTR_MAXSIG)
                #define_eval _PCTR_BULK64_LEN _PCTR_MAXSIG
            #else
                #define_eval _PCTR_BULK64_LEN (_PCTR_DECLEN / 2)
            #endif
            #if (_PCTR_BULK64_LEN  < 1)
                #error "insufficent xfer registers to atomically update 64-bit counters"
            #endif

            #define _LI64_0
            #define _LI64_1

            .begin
                /* UGH: Define a list of all signals */
                #define_eval _PCTR_LOOP      0
                #while (_PCTR_LOOP < _PCTR_BULK64_LEN )
                    .sig iodone_/**/_PCTR_LOOP
                    #if (_PCTR_LOOP == 0)
                        #define_eval _SIGTMP        'iodone_/**/_PCTR_LOOP'
                        #define_eval _SIGLIST       '_SIGTMP'
                    #else
                        #define_eval _SIGTMP        '_SIGLIST,iodone_/**/_PCTR_LOOP'
                        #define_eval _SIGLIST       '_SIGTMP'
                    #endif
                    #define_eval _PCTR_LOOP (_PCTR_LOOP + 1)
                #endloop
                #undef _PCTR_LOOP

                #if (_PCTR_BULK64_LEN  <= NUM64)
                    .reg ctr
                    immed[ctr, (NUM64 / _PCTR_BULK64_LEN)]
                    .while (ctr > 0)
                        alu[ctr, ctr, -, 1]

                        /* unroll a bunch of writes */
                        #define _PCTR_LOOP      0
                        #while (_PCTR_LOOP < _PCTR_BULK64_LEN)
                            #define_eval _LI64_0    (_PCTR_LOOP * 2)
                            #define_eval _LI64_1    (_PCTR_LOOP * 2 + 1)
                            immed[_IX[_LI64_0], 0xFFFFFFFF]
                            immed[_IX[_LI64_1], 0xFFFFFFFF]
                            _pctr_op(L_MEM, test_and_clr, _IX[_LI64_0],
                                     iaddr, 0, 2, iodone_/**/_PCTR_LOOP)
                            alu[iaddr, iaddr, +, 8]
                            #define_eval _PCTR_LOOP (_PCTR_LOOP + 1)
                        #endloop
                        #undef _PCTR_LOOP
                        ctx_arb[_SIGLIST], ALL

                        /* unroll a bunch of writes */
                        #define _PCTR_LOOP  0
                        #while (_PCTR_LOOP < _PCTR_BULK64_LEN)
                            #define_eval _LI64_0    (_PCTR_LOOP * 2)
                            #define_eval _LI64_1    (_PCTR_LOOP * 2 + 1)
                            /* XXX If the prevous op was a test and set, _IX should be set! */
                            .set _IX[_LI64_0]
                            .set _IX[_LI64_1]
                            alu[_OX[_LI64_0], --, B, _IX[_LI64_0]]
                            alu[_OX[_LI64_1], --, B, _IX[_LI64_1]]
                            _pctr_op(G_MEM, add64, _OX[_LI64_0], oaddr, 0, 1,
                                iodone_/**/_PCTR_LOOP)
                            alu[oaddr, oaddr, +, 8]
                            #define_eval _PCTR_LOOP (_PCTR_LOOP + 1)
                        #endloop
                        #undef _PCTR_LOOP
                        ctx_arb[_SIGLIST], ALL

                    .endw
                #endif
                #define_eval _PCTR_REM  (NUM64 % _PCTR_BULK64_LEN)

                #if (_PCTR_REM > 0)
                    #define     _PCTR_LOOP      0
                    #define_eval _SIGLIST ''
                    #while (_PCTR_LOOP < _PCTR_REM)
                        #if (_PCTR_LOOP == 0)
                            #define_eval _SIGTMP        'iodone_/**/_PCTR_LOOP'
                            #define_eval _SIGLIST       '_SIGTMP'
                        #else
                            #define_eval _SIGTMP        '_SIGLIST,iodone_/**/_PCTR_LOOP'
                            #define_eval _SIGLIST       '_SIGTMP'
                        #endif
                        #define_eval _PCTR_LOOP (_PCTR_LOOP + 1)
                    #endloop
                    #undef _PCTR_LOOP

                    /* unroll a bunch of writes */
                    #define _PCTR_LOOP      0
                    #while (_PCTR_LOOP < _PCTR_REM)
                        #define_eval _LI64_0    (_PCTR_LOOP * 2)
                        #define_eval _LI64_1    (_PCTR_LOOP * 2 + 1)
                        immed[_IX[_LI64_0], 0xFFFFFFFF]
                        immed[_IX[_LI64_1], 0xFFFFFFFF]
                        _pctr_op(L_MEM, test_and_clr, _IX[_LI64_0],
                                 iaddr, 0, 2, iodone_/**/_PCTR_LOOP)
                        alu[iaddr, iaddr, +, 8]
                        #define_eval _PCTR_LOOP (_PCTR_LOOP + 1)
                    #endloop
                    #undef _PCTR_LOOP
                    ctx_arb[_SIGLIST], ALL

                    #define     _PCTR_LOOP      0
                    #while (_PCTR_LOOP < _PCTR_REM)
                        #define_eval _LI64_0    (_PCTR_LOOP * 2)
                        #define_eval _LI64_1    (_PCTR_LOOP * 2 + 1)
                        /* XXX If the prevous op was a test and set, _IX should be set! */
                        .set _IX[_LI64_0]
                        alu[_OX[_LI64_0], --, B, _IX[_LI64_0]]
                        /* XXX If the prevous op was a test and set, _IX should be set! */
                        .set _IX[_LI64_1]
                        alu[_OX[_LI64_1], --, B, _IX[_LI64_1]]
                        _pctr_op(G_MEM, add64, _OX[_LI64_0], oaddr, 0, 2,
                                 iodone_/**/_PCTR_LOOP)
                        alu[oaddr, oaddr, +, 8]
                        #define_eval _PCTR_LOOP (_PCTR_LOOP + 1)
                    #endloop
                    #undef _PCTR_LOOP
                    ctx_arb[_SIGLIST], ALL
                #endif


            .end

            #undef _LI64_0
            #undef _LI64_1
            #undef _PCTR_BULK64_LEN
        #endif /* (NUM64 > 0 ) */

        #undef _OX
        #undef _IX
        #undef _PCTR_REM
        #undef _SIGLIST
        #undef _SIGTMP

    .end
    #endif
#endm


/** @}
 * @}
 */

#endif /* __PUSH_COUNTERS_UC__ */
