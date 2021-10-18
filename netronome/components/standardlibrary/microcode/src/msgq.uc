/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __MSGQ_UC__
#define __MSGQ_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <stdmac.uc>
#include <util.uc>

/** @file msgq.uc SRAM or Global Scratch Message Queue Macros
 * @addtogroup sg_msgq SRAM or Global Scratch Message Queues
 * @{
 *
 * @name SRAM or Global Scratch Message Queue Macros
 * @{
 *
 */

// API Quick Reference:
//
// msgq_init(in_base_addr, in_ring_id_or_index, MEM_TYPE, Q_SIZE, MSG_SIZE,
//           in_req_sig, in_wakeup_sigs, Q_OPTION)
//
// msgq_receive(out_data, in_base_addr, in_ring_id_or_index, MEM_TYPE, Q_SIZE, MSG_SIZE,
//             in_req_sig, in_wakeup_sigs, Q_OPTION)
//
// msgq_send(io_data, in_base_addr, in_ring_id_or_index, MEM_TYPE, Q_SIZE, MSG_SIZE,
//           in_req_sig, in_wakeup_sigs, Q_OPTION)



// cs base addresses
#ifndef CLUSTER_SCRATCH_RING_BASE_ADDR
    #define CLUSTER_SCRATCH_RING_BASE_ADDR 0x10000
#endif
#ifndef CLUSTER_SCRATCH_RING_PTRS_ADDR
    #define CLUSTER_SCRATCH_RING_PTRS_ADDR 0x10080
#endif


// field locations
// for scratch init, where ring size goes
#ifndef SCRATCH_RING_SIZE_FIELD
    #define SCRATCH_RING_SIZE_FIELD 30
#endif
//for sram ring init, where ring size goes
#ifndef SRAM_RING_SIZE_FIELD
    #define SRAM_RING_SIZE_FIELD 29
#endif
// for sram ring queue descriptor, where ring number goes
#ifndef SRAM_RING_NUM_FIELD
    #define SRAM_RING_NUM_FIELD 24
#endif
// for sram ring queue descriptor, where channel number is
#ifndef SRAM_CHAN_NUM_FIELD
    #define SRAM_CHAN_NUM_FIELD 30
#endif
//YDSDK519
// for cluster scratch init, where ring size goes
#ifndef CLUSTER_SCRATCH_RING_SIZE_FIELD
    #define CLUSTER_SCRATCH_RING_SIZE_FIELD 16
#endif


/// @cond INTERNAL_MACROS
/** Internal supporting macro that returns the minimum ring size required
 *  given message size and number of messages.
 *
 * @param out_min_ring_size  Minimum ring size in 32-bit longwords (GPR)
 * @param Q_SIZE             Constant number of messages
 * @param MSG_SIZE           Constant message size in 32-bit longwords
 * @param MEM_TYPE           Either SCRATCH or SRAM or CS
 *
 * @instruction_cnt 1-2
 *
 * @b Example:
 * @code
 * _CALC_MIN_RING_SIZE(out_ring, 100, 3, SCRATCH) will give 512 LW
 * @endcode
 *
 */
#macro _CALC_MIN_RING_SIZE(out_min_ring_size, Q_SIZE, MSG_SIZE, MEM_TYPE)
    #ifdef _RING_SIZE
        #warning "_RING_SIZE is being redefined."
    #endif
    #define_eval _RING_SIZE (MSG_SIZE * Q_SIZE)
    #if (streq(MEM_TYPE, SCRATCH))
    // Valid ring sizes: 128, 256, 512, 1024 32-bit LW
        #if (_RING_SIZE <= 0x80)
            immed32(out_min_ring_size, 0x80)
        #elif (_RING_SIZE <= 0x100)
            immed32(out_min_ring_size, 0x100)
        #elif (_RING_SIZE <= 0x200)
            immed32(out_min_ring_size, 0x200)
        #else
            immed32(out_min_ring_size, 0x400)
        #endif
    #elif (streq(MEM_TYPE, CS))
    // Valid ring sizes: 32, 64, 128, 256, 512, 1024 32-bit LW
        #if (_RING_SIZE <= 0x20)
            immed32(out_min_ring_size, 0x20)
        #elif (_RING_SIZE <= 0x40)
            immed32(out_min_ring_size, 0x40)
        #elif (_RING_SIZE <= 0x80)
            immed32(out_min_ring_size, 0x80)
        #elif (_RING_SIZE <= 0x100)
            immed32(out_min_ring_size, 0x100)
        #elif (_RING_SIZE <= 0x200)
            immed32(out_min_ring_size, 0x200)
        #else
            immed32(out_min_ring_size, 0x400)
        #endif
    #elif (streq(MEM_TYPE, SRAM))
    // Valid ring sizes: 512, 1K, 2K, 4K, 8K, 16K, 32K, 64K 32-bit LW
        #if (_RING_SIZE <= 0x200)
            immed32(out_min_ring_size, 0x200)
        #elif (_RING_SIZE <= 0x400)
            immed32(out_min_ring_size, 0x400)
        #elif (_RING_SIZE <= 0x800)
            immed32(out_min_ring_size, 0x800)
        #elif (_RING_SIZE <= 0x1000)
            immed32(out_min_ring_size, 0x1000)
        #elif (_RING_SIZE <= 0x2000)
            immed32(out_min_ring_size, 0x2000)
        #elif (_RING_SIZE <= 0x4000)
            immed32(out_min_ring_size, 0x4000)
        #elif (_RING_SIZE <= 0x8000)
            immed32(out_min_ring_size, 0x8000)
        #else
            immed32(out_min_ring_size, 0x10000)
        #endif
    #endif
    #undef _RING_SIZE
#endm
/// @endcond


/** Initialize a msgq.
 *
 * The msgq may be in CS (cluster scratch), Scratch or SRAM, as specified by the @p MEM_TYPE argument.
 *
 * @param in_base_addr           Byte address of ring base (constant or GPR)
 * @param in_ring_id_or_index    Ring number (constant: 0-15 for scratch, 0-63 for sram)
 * @param MEM_TYPE               SRAM, SCRATCH, CS
 * @param Q_SIZE                 Number of entries in queue (constant)
 * @param MSG_SIZE               Message size in 32-bit longwords (constant)
 * @param in_req_sig             Requested signal.
 * @param in_wakeup_sigs         List of signals causing thread to swap/wakeup
 * @param Q_OPTION               queue_t. Directive for memory controller queue selection.
 *
 * @instruction_cnt 6-8 for scratch (3 CSR writes)
 * @instruction_cnt 11-20 for sram (1 sram write and 2 sram read ring operations)
 *
 * @b Example:
 * @code
 * #define_eval QMSG_RING_NUMBER 4
 * // 128 messages, 8 words each
 * msgq_init(msgq_ring_base_addr, QMSG_RING_NUMBER, CS, 128, 8, sig4, sig4, ___)
 * @endcode
 */
#macro msgq_init(in_base_addr, in_ring_id_or_index, MEM_TYPE, Q_SIZE, MSG_SIZE, \
        in_req_sig, in_wakeup_sigs, Q_OPTION)
.begin

    #if ((IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) && \
          (streq(MEM_TYPE, SCRATCH) || streq(MEM_TYPE, CS )))
        #error "msgq_init: MEM_TYPE SCRATCH or CS not supported on NFP6000."
    #endif

    #ifdef _MSGQ_RING_SIZE
        #warning "_MSGQ_RING_SIZE is being redefined."
    #endif


    #if ((!is_ct_const(Q_SIZE)) || (!is_ct_const(MSG_SIZE)))
        #error "msgq_init -> Q_SIZE and MSG_SIZE must be immediates."

    #elif ((!streq(MEM_TYPE, SCRATCH)) && (!streq(MEM_TYPE, SRAM)) && ((!streq(MEM_TYPE, CS))))
        #error "msgq_send -> MEM_TYPE not supported."
    #elif (streq(MEM_TYPE, CS))

        // _MSGQ_RING_SIZE is in bytes
        #define_eval _MSGQ_RING_SIZE (Q_SIZE * MSG_SIZE * 4)

        .sig write_cs0 write_cs1
        .reg $cs_base_init $cs_head_tail_init msgq_ring_base_addr_gpr

        #if (is_ct_const(in_ring_id_or_index))
            #ifdef _SCR_RING_NUM
                #warning "_SCR_RING_NUM is being redefined."
            #endif
            #define_eval _SCR_RING_NUM in_ring_id_or_index
        #else
            #error "msgq_init -> in_ring_id_or_index must be a number"
        #endif

        // Input ring size is in bytes.  Must convert bytes to two bit encoded value
        #if (_MSGQ_RING_SIZE == 128)
            #define _ENCODED_RING_SIZE 0
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0x7f)
                        #error "msgq_init -> 128 byte CS ring must have base address with bits 6:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 256)
            #define _ENCODED_RING_SIZE 1
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0xff)
                    #error "msgq_init -> 256 byte CS ring must have base address with bits 7:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 512)
            #define _ENCODED_RING_SIZE 2
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0x1ff)
                    #error "msgq_init -> 512 byte CS ring must have base address with bits 8:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 1024)
            #define _ENCODED_RING_SIZE 3
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0x3ff)
                    #error "msgq_init -> 1024 byte CS ring must have base address with bits 9:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 2048)
            #define _ENCODED_RING_SIZE 4
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0x7ff)
                    #error "msgq_init -> 2048 byte CS ring must have base address with bits 10:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 4096)
            #define _ENCODED_RING_SIZE 5
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0xfff)
                    #error "msgq_init -> 4096 byte CS ring must have base address with bits 11:0 clear"
                #endif
            #endif
        #else
            #error "msgq_init -> Invalid immediate value input for (Q_SIZE * MSG_SIZE). \
                must be 128, 256, 512, 1024, 2048, or 4096 (bytes)"
        #endif

        // in_base_addr and MSGQ_RING_BASE_ADDR are byte addresses
        #if (is_rt_const(in_base_addr))
            move(msgq_ring_base_addr_gpr, in_base_addr)
            #define_eval MSGQ_RING_BASE_ADDR msgq_ring_base_addr_gpr
        #else   // in_base_addr is a gpr
            #ifdef MSGQ_RING_BASE_ADDR
                #warning "MSGQ_RING_BASE_ADDR is being redefined."
            #endif
            #define_eval MSGQ_RING_BASE_ADDR in_base_addr
        #endif

        alu_shf[$cs_base_init, MSGQ_RING_BASE_ADDR, OR, _ENCODED_RING_SIZE, <<SCRATCH_RING_SIZE_FIELD]
        immed[$cs_head_tail_init, 0x0000]
        // compute address map for the CS ring number
        move (cs_base_loc, (CLUSTER_SCRATCH_RING_BASE_ADDR+(_SCR_RING_NUM<<3)))
        move (cs_ptr_loc, (CLUSTER_SCRATCH_RING_PTR_ADDR+(_SCR_RING_NUM<<3)))
        cls[write, $cs_base_init, cs_base_loc, 0, 1], sig_done[write_cs0]
        cls[write, $cs_head_tail_init, cs_ptr_loc, 0, 1], sig_done[in_req_sig]

        #ifdef _WAKEUP_SIGS
            #warning "_WAKEUP_SIGS is being redefined."
        #endif
        #define_eval _WAKEUP_SIGS in_wakeup_sigs
        #if (!is_ct_const(_WAKEUP_SIGS))
            ctx_arb[write_cs0, in_req_sig], and
        #elif (_WAKEUP_SIGS > 0)
            ctx_arb[write_cs0, in_wakeup_sigs], and
        #else
            ctx_arb[write_cs0]
        #endif

        #undef _WAKEUP_SIGS

        #undef MSGQ_RING_BASE_ADDR


        #undef _ENCODED_RING_SIZE

        #ifdef _SCR_RING_NUM
            #undef _SCR_RING_NUM
        #endif



    #elif (streq(MEM_TYPE, SCRATCH))

        // _MSGQ_RING_SIZE is in bytes
        #define_eval _MSGQ_RING_SIZE (Q_SIZE * MSG_SIZE * 4)

        .sig write_cap_scratch0 write_cap_scratch1
        .reg $scratch_base_init $scratch_head_tail_init msgq_ring_base_addr_gpr

        #if (is_ct_const(in_ring_id_or_index))
            #ifdef _SCR_RING_NUM
                #warning "_SCR_RING_NUM is being redefined."
            #endif
            #define_eval _SCR_RING_NUM in_ring_id_or_index
        #else
            #error "msgq_init -> in_ring_id_or_index must be a number"
        #endif

        // Input ring size is in bytes.  Must convert bytes to two bit encoded value
        #if (_MSGQ_RING_SIZE == 512)
            #define _ENCODED_RING_SIZE 0
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0x1ff)
                        #error "msgq_init -> 512 byte scratch ring must have base address with bits 8:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 1024)
            #define _ENCODED_RING_SIZE 1
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0x3ff)
                    #error "msgq_init -> 1024 byte scratch ring must have base address with bits 9:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 2048)
            #define _ENCODED_RING_SIZE 2
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0x7ff)
                    #error "msgq_init -> 2048 byte scratch ring must have base address with bits 10:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 4096)
            #define _ENCODED_RING_SIZE 3
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0xfff)
                    #error "msgq_init -> 4096 byte scratch ring must have base address with bits 11:0 clear"
                #endif
            #endif
        #else
            #error "msgq_init -> Invalid immediate value input for (Q_SIZE * MSG_SIZE). \
                must be 512, 1024, 2048, or 4096 (bytes)"
        #endif

        // in_base_addr and MSGQ_RING_BASE_ADDR are byte addresses
        #if (is_rt_const(in_base_addr))
            move(msgq_ring_base_addr_gpr, in_base_addr)
            #define_eval MSGQ_RING_BASE_ADDR msgq_ring_base_addr_gpr
        #else   // in_base_addr is a gpr
            #ifdef MSGQ_RING_BASE_ADDR
                #warning "MSGQ_RING_BASE_ADDR is being redefined."
            #endif
            #define_eval MSGQ_RING_BASE_ADDR in_base_addr
        #endif

        alu_shf[$scratch_base_init, MSGQ_RING_BASE_ADDR, OR, _ENCODED_RING_SIZE, <<SCRATCH_RING_SIZE_FIELD]
        immed[$scratch_head_tail_init, 0x0000]
        cap[write, $scratch_base_init, scratch_ring_base_/**/_SCR_RING_NUM], sig_done[write_cap_scratch0]
        cap[write, $scratch_head_tail_init, scratch_ring_head_/**/_SCR_RING_NUM], sig_done[write_cap_scratch1]
        cap[write, $scratch_head_tail_init, scratch_ring_tail_/**/_SCR_RING_NUM], sig_done[in_req_sig]

        #ifdef _WAKEUP_SIGS
            #warning "_WAKEUP_SIGS is being redefined."
        #endif
        #define_eval _WAKEUP_SIGS in_wakeup_sigs
        #if (!is_ct_const(_WAKEUP_SIGS))
            ctx_arb[write_cap_scratch0, write_cap_scratch1, in_req_sig], and
        #elif (_WAKEUP_SIGS > 0)
            ctx_arb[write_cap_scratch0, write_cap_scratch1, in_req_sig], and
        #else
            ctx_arb[write_cap_scratch0, write_cap_scratch1], and
        #endif

        #undef _WAKEUP_SIGS

        #undef MSGQ_RING_BASE_ADDR

        #undef _ENCODED_RING_SIZE

        #ifdef _SCR_RING_NUM
            #undef _SCR_RING_NUM
        #endif

    #elif (streq(MEM_TYPE, SRAM))
        // _MSGQ_RING_SIZE is in 32-bit longwords
        #define_eval _MSGQ_RING_SIZE (Q_SIZE * MSG_SIZE)

        .sig sram_msgq_init
        .reg $sram_head_init $sram_tail_init $sram_count_init msgq_ring_base_addr _base_addr_gpr
        .xfer_order $sram_head_init $sram_tail_init $sram_count_init

        #if (is_ct_const(in_ring_id_or_index))
            #ifdef _SRAM_RING_NUM
                #warning "_SRAM_RING_NUM is being redefined."
            #endif
            #define_eval _SRAM_RING_NUM in_ring_id_or_index
        #else
            #error  "msgq_init -> in_ring_id_or_index must be a number"
        #endif

        // Input ring size is in longwords.  Must convert longwords to two-bit encoded value
        #if (_MSGQ_RING_SIZE == 512)
            #define _ENCODED_RING_SIZE 0
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0x7ff)
                        #error "msgq_init -> 512 byte sram ring must have base address with bits 10:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 1024)
            #define _ENCODED_RING_SIZE 1
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0xfff)
                    #error "msgq_init -> 1024 byte sram ring must have base address with bits 11:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 2048)
            #define _ENCODED_RING_SIZE 2
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0x1fff)
                    #error "msgq_init -> 2048 byte sram ring must have base address with bits 12:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 4096)
            #define _ENCODED_RING_SIZE 3
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0x3fff)
                    #error "msgq_init -> 4096 byte sram ring must have base address with bits 13:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 8192)
            #define _ENCODED_RING_SIZE 4
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0x3fff)
                    #error "msgq_init -> 8192 byte sram ring must have base address with bits 14:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 16384)
            #define _ENCODED_RING_SIZE 5
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0xffff)
                    #error "msgq_init -> 16384 byte sram ring must have base address with bits 15:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 32768)
            #define _ENCODED_RING_SIZE 6
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0x1ffff)
                    #error "msgq_init -> 32768 byte sram ring must have base address with bits 16:0 clear"
                #endif
            #endif
        #elif (_MSGQ_RING_SIZE == 65536)
            #define _ENCODED_RING_SIZE 7
            #if (is_ct_const(in_base_addr))
                #if (in_base_addr & 0x3ffff)
                    #error "msgq_init -> 65536 byte sram ring must have base address with bits 17:0 clear"
                #endif
            #endif
        #else
            #error "msgq_init -> Invalid immediate value input for (Q_SIZE * MSG_SIZE). \
                must be 512, 1024, 2048, 4096, 8192, 16384, 32768 or 65536 (longwords)"
        #endif

        #if (is_rt_const(in_base_addr))
            move(_base_addr_gpr, in_base_addr)
            #define_eval _BASE_ADDR _base_addr_gpr
        #else
            #ifdef _BASE_ADDR
                #warning "_BASE_ADDR is being redefined."
            #endif
            #define_eval _BASE_ADDR in_base_addr
        #endif

        // IXP2XXX format for initial head/tail: 31:29 size enclode, 23:0 longword address
        // msgq_ring_base_addr includes channel number in 31:30. Strip it off and convert to word address for queue desc.
        alu_shf_left(msgq_ring_base_addr, _BASE_ADDR, AND~, 0x3, SRAM_CHAN_NUM_FIELD)
        shf_right(msgq_ring_base_addr, msgq_ring_base_addr, 2)
        alu_shf[$sram_head_init, msgq_ring_base_addr, OR, _ENCODED_RING_SIZE, <<SRAM_RING_SIZE_FIELD]
        alu_shf[$sram_tail_init, msgq_ring_base_addr, OR, _ENCODED_RING_SIZE, <<SRAM_RING_SIZE_FIELD]
        immed[$sram_count_init, 0x0000]
        sram[write, $sram_head_init, _BASE_ADDR, 0, 3], ctx_swap[sram_msgq_init]
        // Read the data back so we can be sure it's in memeory
        sram[read, $sram_head_init, _BASE_ADDR, 0, 3], ctx_swap[sram_msgq_init]

        // IXP2XXX format for rd_qdesc*: 31:30 channel, 29:24 q number, 23:0 longword address
        // merge channel back into msgq_ring_base_addr
        ld_field[msgq_ring_base_addr, 1000, _BASE_ADDR]
        // insert ring number
        alu_shf_left(msgq_ring_base_addr, msgq_ring_base_addr, OR, _SRAM_RING_NUM, SRAM_RING_NUM_FIELD)
        sram[rd_qdesc_head, $sram_head_init, 0, msgq_ring_base_addr, 2], sig_done[in_req_sig]
        sram[rd_qdesc_other, --, 0, msgq_ring_base_addr]

        #ifdef _WAKEUP_SIGS
            #warning "_WAKEUP_SIGS is being redefined."
        #endif
        #define_eval _WAKEUP_SIGS in_wakeup_sigs
        #if (!is_ct_const(_WAKEUP_SIGS))
            ctx_arb[in_wakeup_sigs]
        #elif (_WAKEUP_SIGS > 0)
            ctx_arb[in_wakeup_sigs]
        #endif
        #undef _WAKEUP_SIGS

        #undef _BASE_ADDR

        #undef _ENCODED_RING_SIZE
        #ifdef _SRAM_RING_NUM
            #undef _SRAM_RING_NUM
        #endif
    #endif

    #ifdef _MSGQ_RING_SIZE
        #undef _MSGQ_RING_SIZE
    #endif

.end
#endm


/** Get a message from a msgq.
 *
 * The msgq may be in Cluster Scratch, Scratch or SRAM, as specified by the MEM_TYPE argument.
 * Hardware-supported rings are used.
 *
 * @param out_data              Resulting status:
 *                              @arg >0 = out_data contains a new message from the msgq
 *                              @arg 0 = no message was available
 * @param in_base_addr          Byte address of ring (constant or GPR)
 * @param in_ring_id_or_index   Ring number (constant: 0-15 for scratch, 0-63 for sram)
 * @param MEM_TYPE              SRAM, SCRATCH
 * @param Q_SIZE                Number of entries in queue (constant)
 * @param MSG_SIZE              Constant message size in 32-bit words. Maximum message size is 8.
 * @param in_req_sig            Requested signal. See common section "Signal Arguments."
 * @param in_wakeup_sigs        List of signals causing thread to swap/wakeup
 * @param Q_OPTION              queue_t. Directive for memory controller queue selection.
 *
 * @note The address part of "in_base_addr" is ignored but bits [31:30] are
 *       required to indicate SRAM channel. The implementation just passes "in_base_addr"
 *       instead of extracting bits [31:30] to avoid cycle overhead.
 *
 * @instruction_cnt 2 - 7 instructions (1 ring operation for sram or scratch or Cs)
 */
#macro msgq_receive(out_data, in_base_addr, in_ring_id_or_index, MEM_TYPE, Q_SIZE, MSG_SIZE, \
        in_req_sig, in_wakeup_sigs, Q_OPTION)
.begin

    #if ((IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) && \
          (streq(MEM_TYPE, SCRATCH) || streq(MEM_TYPE, CS )))
        #error "msgq_receive: MEM_TYPE SCRATCH or CS not supported on NFP6000."
    #endif

    #if ((!is_ct_const(Q_SIZE)) || (!is_ct_const(MSG_SIZE)))
        #error "msgq_receive -> Q_SIZE and MSG_SIZE must be immediates."

    #elif ((!streq(MEM_TYPE, SCRATCH)) && (!streq(MEM_TYPE, SRAM)))
        #error "msgq_send -> MEM_TYPE not supported."

    #else
        #if (is_ct_const(in_ring_id_or_index))
            #ifdef _RING_NUM
                #warning "_RING_NUM is being redefined."
            #endif
            #define_eval _RING_NUM in_ring_id_or_index
        #else
            #error  "msgq_receive -> in_ring_id_or_index must be a number"
        #endif

        #ifdef _RING_NUM_X4
            #warning "_RING_NUM_X4 is being redefined."
        #endif
        #define_eval _RING_NUM_X4 (_RING_NUM << 2)

        _DEF_MEM_TOKENS(in_req_sig, in_wakeup_sigs, Q_OPTION, single_sig_op)

        .reg msgq_ring_base_addr

        // src1+src2 has the following format:
        // SRAM:
        //      [31:30] SRAM channel select
        //      [29:8]  ignored
        //      [7:2]   Q_array entry
        //      [1:0]   ignored
        // SCRATCH:
        //      [5:2]   scratch ring encoding
        //      other bits are ignored
        // CS:
        //      [5:2]   CS ring encoding
        //      other bits are ignored
        // last 8 bits of in_base_addr always have to be 0 (address alignment)
        //  so it's safe to OR with _RING_NUM_X4
        alu_op(msgq_ring_base_addr, in_base_addr, OR, _RING_NUM_X4)
        MEM_TYPE/**/[get, out_data, msgq_ring_base_addr, 0, MSG_SIZE], _MEM_TOKENS

        #ifdef _HW_SIG_EXPR
            #undef _HW_SIG_EXPR
            ctx_arb[in_wakeup_sigs]
        #endif

        #undef _RING_NUM
        #undef _RING_NUM_X4
    #endif

.end
#endm


/** Send a message to a msgq.
 *
 * The msgq may be in Cluster Scratch, Scratch or SRAM, as specified by the MEM_TYPE argument.
 * Hardware-supported rings are used.
 *
 * @param io_data               Input: write xfer reg, data to be sent @n
 *                              Output (SRAM only): read xfer reg @n
 *                              Resulting status: @n
 *                              @arg bit 31 = 1: Message was successfully sent to the msgq
 *                              @arg bit 31 = 0: msgq is full. Message not sent.
 * @param in_base_addr            byte address of ring (constant or GPR)
 * @param in_ring_id_or_index   Ring number (constant: 0-15 for scratch, 0-63 for sram)
 * @param MEM_TYPE              SRAM, SCRATCH, CS
 * @param Q_SIZE                Constant number of entries in queue
 * @param MSG_SIZE              Constant message size in 32-bit words. Maximum message size is 8.
 * @param in_req_sig            Requested signal. See common section "Signal Arguments."
 * @param in_wakeup_sigs        List of signals causing thread to swap/wakeup
 * @param Q_OPTION              queue_t. Directive for memory controller queue selection.
 *
 * @instruction_cnt 2 - 7 instructions (1 ring operation for sram or scratch)
 *
 * @note The address part of "in_base_addr" is ignored but bits [31:30] are
 *       required to indicate SRAM channel. The implementation just passes "in_base_addr"
 *       instead of extracting bits [31:30] to avoid cycle overhead.
 *
 * @b Example:
 * @code
 * #define RING_ID 2
 * xbuf_alloc($msg, 2, read_write)
 * move($msg0, 0xa)
 * move($msg1, 0xb)
 * msgq_send($msg0, base_addr, RING_ID, SRAM, 128, 8, MY_SIG, MY_SIG, ___)
 * @endcode
 */
#macro msgq_send(io_data, in_base_addr, in_ring_id_or_index, MEM_TYPE, Q_SIZE, MSG_SIZE, \
        in_req_sig, in_wakeup_sigs, Q_OPTION)
.begin

    #if ((IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800)) && \
          (streq(MEM_TYPE, SCRATCH) || streq(MEM_TYPE, CS )))
        #error "msgq_send: MEM_TYPE SCRATCH or CS not supported on NFP6000."
    #endif

    #if ((!is_ct_const(Q_SIZE)) || (!is_ct_const(MSG_SIZE)))
        #error "msgq_send -> Q_SIZE and MSG_SIZE must be immediates."

    #elif ((!streq(MEM_TYPE, SCRATCH)) && (!streq(MEM_TYPE, SRAM)))
        #error "msgq_send -> MEM_TYPE not supported."

    #else
        #if (is_ct_const(in_ring_id_or_index))
            #ifdef _RING_NUM
                #warning "_RING_NUM is being redefined."
            #endif
            #define_eval _RING_NUM in_ring_id_or_index
        #else
            #error  "msgq_send -> in_ring_id_or_index must be a number"
        #endif

        #ifdef _RING_NUM_X4
            #warning "_RING_NUM_X4 is being redefined."
        #endif
        #define_eval _RING_NUM_X4 (_RING_NUM << 2)

        #ifdef _TMP_OP_TYPE
            #warning "_TMP_OP_TYPE is being redefined."
        #endif
        #if (streq(MEM_TYPE, SRAM))
            #define_eval _TMP_OP_TYPE dual_sig_op
        #else
            #define_eval _TMP_OP_TYPE single_sig_op
        #endif
        _DEF_MEM_TOKENS(in_req_sig, in_wakeup_sigs, Q_OPTION, _TMP_OP_TYPE)

        .reg msgq_ring_base_addr

        // src1+src2 has the following format:
        // SRAM:
        //      [31:30] SRAM channel select
        //      [29:8]  ignored
        //      [7:2]   Q_array entry
        //      [1:0]   ignored
        // SCRATCH:
        //      [5:2]   scratch ring encoding
        //      other bits are ignored
        // CS:
        //      [5:2]   cluster scratch ring encoding
        //      other bits are ignored

        // last 8 bits of in_base_addr always have to be 0 (address alignment)
        //  so it's safe to OR with _RING_NUM_X4
        alu_op(msgq_ring_base_addr, in_base_addr, OR, _RING_NUM_X4)
        MEM_TYPE/**/[put, io_data, msgq_ring_base_addr, 0, MSG_SIZE], _MEM_TOKENS

        #ifdef _HW_SIG_EXPR
            #undef _HW_SIG_EXPR
            ctx_arb[in_wakeup_sigs]
        #endif

        #undef _RING_NUM
        #undef _RING_NUM_X4
        #undef _TMP_OP_TYPE
    #endif

.end
#endm


/** @}
 * @}
 */

#endif /* __MSGQ_UC__ */
