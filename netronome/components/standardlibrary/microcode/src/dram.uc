/*
 * Copyright (C) 2008-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __DRAM_UC__
#define __DRAM_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

#include <constants.uc>
#include <stdmac.uc>
#include <util.uc>

/** @file dram.uc DRAM Access Macros
 * @addtogroup dram DRAM Access
 * @{
 *
 * @name DRAM Access Macros
 * @{
 * DRAM memory specific Access Macros
 */

// API Quick Reference:
//
// dram_mask_write(in_data, in_dram_addr, in_addr_offset, in_byte_mask, REQ_SIG,
//                 wakeup_sigs, in_reserved)
// dram_rbuf_read(in_dram_addr, in_dram_addr_offset, in_rbuf_addr, in_rbuf_addr_offset,
//                in_qw_count, REQ_SIG, in_wakeup_sigs, in_reserved)
// dram_read(out_data, in_dram_addr, in_addr_offset, in_qw_count, REQ_SIG,
//           in_wakeup_sigs, in_reserved)
// dram_tbuf_write(in_dram_addr, in_dram_addr_offset, in_tbuf_addr, in_tbuf_addr_offset,
//                 in_qw_count, IN_REQ_SIG, IN_WAKEUP_SIGS, IN_in_reserved)
// dram_write(in_data, in_dram_addr, in_addr_offset, in_qw_count, REQ_SIG,
//            in_wakeup_sigs, in_reserved)


/** Write bytes selected by @p in_byte_mask to a DRAM quadword.
 *
 * @param in_data           Transfer register containing data to be written to DRAM
 * @param in_dram_addr      DRAM address. Register or constant. Added to @p in_addr_offset to form
 *                          DRAM address used in transfer.
 * @param in_addr_offset    DRAM address. Register or constant. Added to @p in_dram_addr to form
 *                          DRAM address used in transfer.
 * @param in_byte_mask      Register or constant containing an 8 bit mask that indicates which
 *                          bytes to write. The bits in the mask correspond to bytes, left to right.
 *                          For example, 0x80 specifies the leftmost byte, and 0x1 specifies the
 *                          rightmost byte.
 * @param REQ_SIG           Requested signal.
 * @param in_wakeup_sigs    List of signals causing thread to swap/wakeup.
 * @param in_reserved       Reserved for future use. Pass: as the value of this parameter.
 *
 * @note
 * @b Limitations: Input data must be in transfer registers.
 *
 * @condition_codes Data dependent: assume they are clobbered.
 *
 * @instruction_cnt 3 to 8
 */
#macro dram_mask_write(in_data, in_dram_addr, in_addr_offset, in_byte_mask, REQ_SIG, in_wakeup_sigs, in_reserved)
    _dram_mask_write(in_data, in_dram_addr, in_addr_offset, in_byte_mask, REQ_SIG, in_wakeup_sigs, --)
#endm

/** Write bytes selected by @p in_byte_mask to a DRAM quadword - 40-bit addressing version.
 *
 * @param in_data           Transfer register containing data to be written to DRAM
 * @param in_addr_1         Address specification - see note.
 * @param in_addr_2         Address specification - see note.
 * @param in_addr_3         Address specification - see note.
 * @param in_byte_mask      Register or constant containing an 8 bit mask that indicates which
 *                          bytes to write. The bits in the mask correspond to bytes, left to right.
 *                          For example, 0x80 specifies the leftmost byte, and 0x1 specifies the
 *                          rightmost byte.
 * @param REQ_SIG           Requested signal.
 * @param in_wakeup_sigs    List of signals causing thread to swap/wakeup.
 * @param in_reserved       Reserved for future use. Pass: as the value of this parameter.
 *
 * @note
 * @b Limitations: Input data must be in transfer registers.\n
 * <b>Address specification</b> takes the form "reg_or_const1, <<8, reg_or_const2" or "reg_or_const1, reg_or_const2, <<8".
 * @condition_codes Data dependent: assume they are clobbered.
 *
 * @instruction_cnt 3 to 8
 */
#macro dram_mask_write(in_data, in_addr_1, in_addr_2, in_addr_3, in_byte_mask, REQ_SIG, in_wakeup_sigs, in_reserved)
    #if (!strstr(in_data, $))
        #error "dram_mask_write -> in_data must be a transfer register"
    #endif

    #if (streq('in_addr_2', '<<8'))
        _dram_mask_write(in_data, in_addr_1, in_addr_3, in_byte_mask, REQ_SIG, in_wakeup_sigs, A<<8)
    #elif (streq('in_addr_3', '<<8'))
        _dram_mask_write(in_data, in_addr_1, in_addr_2, in_byte_mask, REQ_SIG, in_wakeup_sigs, B<<8)
    #else
        _dram_mask_write(in_data, in_addr_1, in_addr_2, in_byte_mask, REQ_SIG, in_wakeup_sigs, --)
    #endif

#endm

/// @cond INTERNAL_MACROS
//Internal macro
#macro _dram_mask_write(in_data, in_dram_addr, in_addr_offset, in_byte_mask, REQ_SIG, in_wakeup_sigs, in_addr_shift)
    #if (!strstr(in_data, $))
        #error "dram_mask_write -> in_data must be a transfer register"
    #endif
    _GET_ADDRESSES(_MEM_ADDR0, _MEM_ADDR1, in_dram_addr, in_addr_offset, DRAM, in_addr_shift)
    .begin

        .reg indirect
        #if (defined(__NFP_INDIRECT_REF_FORMAT_V1))
            alu[indirect, --, b, in_byte_mask]
            alu[--, indirect, OR, 0x4, <<28]
        #elif (defined(__NFP_INDIRECT_REF_FORMAT_V2))
            alu[indirect, --, b, in_byte_mask]
            alu[--, (6<<3), OR, indirect, <<16]
        #else
            alu[indirect, --, b, 1, <<_DRAM_INDIRECT_REF_BYTE_MASK_OV_OFFSET] // set OV bit for byte mask
            alu[--, indirect, OR, in_byte_mask, <<_DRAM_INDIRECT_REF_BYTE_MASK_OFFSET]
        #endif

        #if (streq('in_addr_shift', 'A<<8'))
            mem[write, in_data, _MEM_ADDR0, <<8, _MEM_ADDR1, 1], indirect_ref, sig_done[REQ_SIG]
        #elif (streq('in_addr_shift', 'B<<8'))
            mem[write, in_data, _MEM_ADDR0, _MEM_ADDR1, <<8, 1], indirect_ref, sig_done[REQ_SIG]
        #else
            mem[write, in_data, _MEM_ADDR0, _MEM_ADDR1, 1], indirect_ref, sig_done[REQ_SIG]
        #endif

    .end

    #if (!streq('in_wakeup_sigs', 'SIG_NONE'))
        ctx_arb[in_wakeup_sigs], all
    #endif

    _FREE_ADDRESSES()
#endm
/// @endcond


/** Copy @p in_qw_count quadwords from RBUF to DRAM.
 *
 * RBUF is the interface buffer for data
 * received from the network.
 *
 * @param in_dram_addr          DRAM address. Register or constant. Added to @p in_dram_addr_offset to form
 *                              DRAM address used in transfer.
 * @param in_dram_addr_offset   DRAM address. Register or constant. Added to @p in_dram_addr to form
 *                              DRAM address used in transfer.
 * @param in_rbuf_addr          RBUF address. Register or constant. Added to @p in_rbuf_addr_offset to
 *                              form RBUF address used in transfer.
 * @param in_rbuf_addr_offset   RBUF address. Register or constant. Added to @p in_rbuf_addr to form
 *                              RBUF address used in transfer.
 * @param in_qw_count           Register or constant. Number of quadwords to transfer from RBUF to DRAM
 * @param REQ_SIG               Requested signal.
 * @param in_wakeup_sigs        List of signals causing thread to swap/wakeup.
 * @param in_reserved           Reserved for future use. Pass: as the value of this parameter.
 *
 * @note
 * @b Limitations: Granularity of transfer count is quadwords.
 *
 * @condition_codes Data dependent: assume they are clobbered.
 * @instruction_cnt 2 to 10
 * @ixp
 */
#macro dram_rbuf_read(in_dram_addr, in_dram_addr_offset, in_rbuf_addr, in_rbuf_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, in_reserved)
    #if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
        #error "Rbuf is not available on NFP6000."
    #else
        #ifndef __NFP_INDIRECT_REF_FORMAT_IXP
            #error "This macro is currently only available in IXP indirect reference format mode."
        #endif
    #endif
    _dram_xbuf_xfr(rbuf_rd, in_dram_addr, in_dram_addr_offset, in_rbuf_addr, in_rbuf_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, in_reserved)
#endm


/** Read @p in_qw_count quadwords from DRAM.
 *
 * @param out_data          Transfer register that will contain read data
 * @param in_dram_addr      DRAM address. Register or constant. Added to @p in_addr_offset to form
 *                          DRAM address used in transfer.
 * @param in_addr_offset    DRAM address. Register or constant. Added to @p in_dram_addr to form
 *                          DRAM address used in transfer.
 * @param in_qw_count       Register or constant. Number of quadwords to read. The maximum quadword
 *                          count is 16.
 * @param REQ_SIG           Requested signal.
 * @param in_wakeup_sigs    List of signals causing thread to swap/wakeup.
 * @param in_reserved       Reserved for future use. Pass: as the value of this parameter.
 *
 * @note
 * @b Limitations: Granularity of transfer count is quadwords.
 *
 * @condition_codes Data dependent: assume they are clobbered.
 * @instruction_cnt 1 to 7 (indirect read (count in GPR) and 8<=count<=15)
 */
#macro dram_read(out_data, in_dram_addr, in_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, in_reserved)
    _dram_op_32_40(read, out_data, in_dram_addr, in_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, in_reserved)
#endm

/** Read @p in_qw_count quadwords from DRAM.
 *
 * @param out_data          Transfer register that will contain read data
 * @param in_dram_addr      DRAM address. Register or constant. Added to @p in_addr_offset to form
 *                          DRAM address used in transfer.
 * @param in_addr_offset    DRAM address. Register or constant. Added to @p in_dram_addr to form
 *                          DRAM address used in transfer.
 * @param in_qw_count       Register or constant. Number of quadwords to read. The maximum quadword
 *                          count is 16.
 * @param REQ_SIG           Requested signal.
 * @param in_wakeup_sigs    List of signals causing thread to swap/wakeup.
 * @note
 * @b Limitations: Granularity of transfer count is quadwords.
 *
 * @condition_codes Data dependent: assume they are clobbered.
 * @instruction_cnt 1 to 7 (indirect read (count in GPR) and 8<=count<=15)
 */
#macro dram_read(out_data, in_dram_addr, in_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs)
    _dram_op_32_40(read, out_data, in_dram_addr, in_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, --)
#endm

/** Read @p in_qw_count quadwords from DRAM - 40-bit addressing version.
 *
 * @param out_data          Transfer register that will contain read data
 * @param in_addr_1         Address specification - see note.
 * @param in_addr_2         Address specification - see note.
 * @param in_addr_3         Address specification - see note.
 * @param in_qw_count       Register or constant. Number of quadwords to read. The maximum quadword
 *                          count is 16.
 * @param REQ_SIG           Requested signal.
 * @param in_wakeup_sigs    List of signals causing thread to swap/wakeup.
 * @param in_reserved       Reserved for future use. Pass: as the value of this parameter.
 *
 * @note
 * @b Limitations: Granularity of transfer count is quadwords.\n
 * <b>Address specification</b> takes the form "reg_or_const1, <<8, reg_or_const2" or "reg_or_const1, reg_or_const2, <<8".
 * @condition_codes Data dependent: assume they are clobbered.
 * @instruction_cnt 1 to 7 (indirect read (count in GPR) and 8<=count<=15)
 */
#macro dram_read(out_data, in_addr_1, in_addr_2, in_addr_3, in_qw_count, REQ_SIG, in_wakeup_sigs, in_reserved)
    _dram_op_32_40(read, out_data, in_addr_1, in_addr_2, in_addr_3, in_qw_count, REQ_SIG, in_wakeup_sigs)
#endm

/** Copy in_qw_count quadwords from DRAM address to TBUF.
 *
 *  (@p in_dram_addr + @p in_dram_addr_offset) to the TBUF address.
 *  (@p in_tbuf_addr + @p in_tbuf_addr_offset).
 *
 * @param in_dram_addr          DRAM address. Register or constant. Added to @p in_dram_addr_offset to form
 *                              DRAM address used in transfer.
 * @param in_dram_addr_offset   DRAM address. Register or constant. Added to @p in_dram_addr to form
 *                              DRAM address used in transfer.
 * @param in_tbuf_addr          TBUF address. Register or constant. Added to @p in_tbuf_addr_offset to
 *                              form TBUF address used in transfer.
 * @param in_tbuf_addr_offset   TBUF address. Register or constant. Added to @p in_tbuf_addr to form
 *                              TBUF address used in transfer.
 * @param in_qw_count           Register or constant. Number of quadwords to write.
 * @param REQ_SIG               Requested signal.
 * @param in_wakeup_sigs        List of signals causing thread to swap/wakeup.
 * @param in_reserved           Reserved for future use. Pass: as the value of this parameter.
 *
 * @note
 * @b Limitations: Granularity of transfer count is quadwords.
 *
 * @condition_codes Data dependent: assume they are clobbered.
 * @instruction_cnt 2 to 10
 * @ixp
 */
#macro dram_tbuf_write(in_dram_addr, in_dram_addr_offset, in_tbuf_addr, in_tbuf_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, in_reserved)
    #if (IS_NFPTYPE(__NFP6000) || IS_NFPTYPE(__NFP3800))
        #error "Tbuf is not available on NFP6000."
    #else
        #ifndef __NFP_INDIRECT_REF_FORMAT_IXP
            #error "This macro is currently only available in IXP indirect reference format mode."
        #endif
    #endif

    _dram_xbuf_xfr(tbuf_wr, in_dram_addr, in_dram_addr_offset, in_tbuf_addr, in_tbuf_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, in_reserved)
#endm


/** Write in_qw_count quadwords to DRAM memory - 40-bit addressing version.
 *
 * @param in_data           Transfer register containing data to write
 * @param in_addr_1         Address specification - see note.
 * @param in_addr_2         Address specification - see note.
 * @param in_addr_3         Address specification - see note.
 * @param in_qw_count       Register or constant. Number of quadwords to write. The maximum
 *                          quadword count is 16.
 * @param REQ_SIG           Requested signal.
 * @param in_wakeup_sigs    List of signals causing thread to swap/wakeup.
 *
 * @note
 * @b Limitations: Granularity of transfer count is quadwords.\n
 * <b>Address specification</b> takes the form "reg_or_const1, <<8, reg_or_const2" or "reg_or_const1, reg_or_const2, <<8".
 * @condition_codes Data dependent: assume they are clobbered.
 * @instruction_cnt 1 to 7 (indirect write (count in GPR) and 8<=count<=15)
 */
#macro dram_write(in_data, in_addr_1, in_addr_2,        in_addr_3, in_qw_count, REQ_SIG, in_wakeup_sigs)
    _dram_op_32_40(write, in_data, in_addr_1, in_addr_2, in_addr_3, in_qw_count, REQ_SIG, in_wakeup_sigs)
#endm

/** Write in_qw_count quadwords to DRAM memory.
 *
 * @param in_data           Transfer register containing data to write
 * @param in_dram_addr      DRAM address. Register or constant. Added to in_addr_offset to form
 *                          DRAM address used in transfer.
 * @param in_addr_offset    DRAM address. Register or constant. Added to in_dram_addr to form
 *                          DRAM address used in transfer.
 * @param in_qw_count       Register or constant. Number of quadwords to write. The maximum
 *                          quadword count is 16.
 * @param REQ_SIG           Requested signal.
 * @param in_wakeup_sigs    List of signals causing thread to swap/wakeup.
 *
 * @note
 * @b Limitations: Granularity of transfer count is quadwords.
 *
 * @condition_codes Data dependent: assume they are clobbered.
 * @instruction_cnt 1 to 7 (indirect write (count in GPR) and 8<=count<=15)
 */
#macro dram_write(in_data, in_dram_addr, in_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs)
    _dram_op_32_40(write, in_data, in_dram_addr, in_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, --)
#endm

/// @cond INTERNAL_MACROS
// Internal macro
#macro _dram_op_32_40(in_mem_op, xfer_reg, in_p1, in_p2, in_p3, in_p4, in_p5, in_p6)

    #if (streq('in_p2', '<<8'))
        _dram_op(in_mem_op, xfer_reg, in_p1, in_p3, in_p4, in_p5, in_p6, A<<8)
    #elif (streq('in_p3', '<<8'))
        _dram_op(in_mem_op, xfer_reg, in_p1, in_p2, in_p4, in_p5, in_p6, B<<8)
    #else
        _dram_op(in_mem_op, xfer_reg, in_p1, in_p2, in_p3, in_p4, in_p5, in_p6)
    #endif

#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro to perform DRAM operation.
 *
 * @param in_mem_op         DRAM memory operation
 * @param xfer_reg          Transfer register containing data to write
 * @param in_dram_addr      DRAM address. Register or constant. Added to in_addr_offset to form
 *                          DRAM address used in transfer.
 * @param in_addr_offset    DRAM address. Register or constant. Added to in_dram_addr to form
 *                          DRAM address used in transfer.
 * @param in_qw_count       Register or constant. Number of quadwords for in_mem_op. The maximum
 *                          quadword count is 16.
 * @param REQ_SIG           Requested signal.
 * @param in_wakeup_sigs    List of signals causing thread to swap/wakeup.
 * @param in_addr_shift     A<<8 or B<<8 to use 40-bit addressing mode, else '--'.
 *
 * @note
 * @b Limitations: Granularity of transfer count is quadwords.
 *
 * @condition_codes Data dependent: assume they are clobbered.
 */
#macro _dram_op(in_mem_op, xfer_reg, in_dram_addr, in_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, in_addr_shift)
.begin
    _GET_ADDRESSES(_MEM_ADDR0, _MEM_ADDR1, in_dram_addr, in_addr_offset, DRAM, in_addr_shift)

    #ifdef _QW_CNT
        #warning "_QW_CNT is being redefined."
    #endif
    #ifdef _REF_CNT_FIELD
        #warning "_REF_CNT_FIELD is being redefined."
    #endif

    // Process in_qw_count
    #if (is_ct_const(in_qw_count) || isimport(in_qw_count))
        #ifdef _act_in_qw_count
            #warning "_act_in_qw_count is being redefined."
        #endif

        #if (MEM_WD_COUNT_MIN_1 == TRUE)
            #define_eval _act_in_qw_count (in_qw_count + 1)
            #if (_act_in_qw_count < 1)
                #error "in_qw_count must be >= 0"
            #endif
            #if (defined(__NFP_INDIRECT_REF_FORMAT_V1) || defined(__NFP_INDIRECT_FORMAT_NFP6000))
                #if (_act_in_qw_count > 32) // 4ctx mode
                    #error "in_qw_count must be <= 31"
                #endif
            #else
                #if (_act_in_qw_count > 16) // 4ctx mode
                    #error "in_qw_count must be <= 15"
                #endif
            #endif
        #else
            #define_eval _act_in_qw_count in_qw_count
            #if (_act_in_qw_count < 1)
                #error "in_qw_count must be >= 1"
            #endif
            #if (defined(__NFP_INDIRECT_REF_FORMAT_V1) || defined(__NFP_INDIRECT_FORMAT_NFP6000))
                #if (_act_in_qw_count > 32) // 4ctx mode
                    #error "in_qw_count must be <= 32"
                #endif
            #else
                #if (_act_in_qw_count > 16) // 4ctx mode
                    #error "in_qw_count must be <= 16"
                #endif
            #endif
        #endif


        #if ( (_act_in_qw_count >= 1) && (_act_in_qw_count <= 8) )
            #define_eval _QW_CNT _act_in_qw_count
        #else

            #define _QW_CNT max // dummy param used in instruction
            // then we must use indirect
            #define INDIRECT_REQUIRED
            #if (defined(__NFP_INDIRECT_REF_FORMAT_V1))
                .begin

                    .reg indirect
                    alu[indirect, --, b, (_act_in_qw_count-1)]
                    alu[--, indirect, OR, 0x2, <<28]

                .end
            #elif (defined(__NFP_INDIRECT_REF_FORMAT_V2))
                .begin

                    .reg indirect
                    alu[indirect, --, b, (_act_in_qw_count-1)]
                    alu[--, (1<<7), OR, indirect, <<8]

                .end
            #else
                #define_eval _REF_CNT_FIELD ((_act_in_qw_count-1) + (1<<(_REF_CNT_OV_OFFSET - _REF_CNT_OFFSET)))
                alu[--, --, b, _REF_CNT_FIELD, <<_REF_CNT_OFFSET]
                #undef _REF_CNT_FIELD
            #endif

        #endif

        #undef _act_in_qw_count // clean up name space
    #else // in_qw_count is a gpr
        .begin

            .reg indirect

            #define _QW_CNT max // dummy param in instruction
            #define INDIRECT_REQUIRED

            #if (defined(__NFP_INDIRECT_REF_FORMAT_V1))
                #if (MEM_WD_COUNT_MIN_1 != TRUE)
                    alu[indirect, in_qw_count, -, 1]
                    alu[--, indirect, OR, 0x2, <<28]
                #else
                    alu[--, in_qw_count, OR, 0x2, <<28]
                #endif
            #elif (defined(__NFP_INDIRECT_REF_FORMAT_V2))
                #if (MEM_WD_COUNT_MIN_1 != TRUE)
                    alu[indirect, in_qw_count, -, 1]
                    alu[--, (1<<7), OR, indirect, <<8]
                #else
                    alu[--, (1<<7), OR, indirect, <<8]
                #endif
            #else
                #if (MEM_WD_COUNT_MIN_1 != TRUE)
                    alu[indirect, in_qw_count, -, 1]
                    alu[indirect, indirect, OR, 1, <<(_REF_CNT_OV_OFFSET - _REF_CNT_OFFSET)]
                    alu[--, --, b, indirect, <<_REF_CNT_OV_OFFSET]
                #else
                    alu[indirect, in_qw_count, OR, 1, <<(_REF_CNT_OV_OFFSET - _REF_CNT_OFFSET)]
                    alu[--, --, b, indirect, <<_REF_CNT_OV_OFFSET]
                #endif
            #endif

        .end
    #endif // is_ct_const(in_qw_count)

    #ifdef INDIRECT_REQUIRED
        #if (streq('in_addr_shift', 'A<<8'))
            mem[in_mem_op, xfer_reg, _MEM_ADDR0, <<8, _MEM_ADDR1, _QW_CNT], indirect_ref, sig_done[REQ_SIG]
        #elif (streq('in_addr_shift', 'B<<8'))
            mem[in_mem_op, xfer_reg, _MEM_ADDR0, _MEM_ADDR1, <<8, _QW_CNT], indirect_ref, sig_done[REQ_SIG]
        #else
            mem[in_mem_op, xfer_reg, _MEM_ADDR0, _MEM_ADDR1, _QW_CNT], indirect_ref, sig_done[REQ_SIG]
        #endif
        #undef INDIRECT_REQUIRED
    #else
        #if (streq('in_addr_shift', 'A<<8'))
            mem[in_mem_op, xfer_reg, _MEM_ADDR0, <<8, _MEM_ADDR1, _QW_CNT], sig_done[REQ_SIG]
        #elif (streq('in_addr_shift', 'B<<8'))
            mem[in_mem_op, xfer_reg, _MEM_ADDR0, _MEM_ADDR1, <<8, _QW_CNT], sig_done[REQ_SIG]
        #else
            mem[in_mem_op, xfer_reg, _MEM_ADDR0, _MEM_ADDR1, _QW_CNT], sig_done[REQ_SIG]
        #endif
    #endif

    #if (!streq('in_wakeup_sigs', 'SIG_NONE'))
        ctx_arb[in_wakeup_sigs]
    #endif

    #undef _QW_CNT

    _FREE_ADDRESSES()

.end
#endm
/// @endcond


/// @cond INTERNAL_MACROS
/** Internal macro used for data transfers from RBUF to DRAM and from DRAM to TBUF.
 *
 * @param in_operation          DRAM memory operation (read/write)
 * @param in_dram_addr          DRAM address. Register or constant. Added to in_dram_addr_offset to
 *                              form DRAM address used in transfer.
 * @param in_dram_addr_offset   DRAM address. Register or constant. Added to in_dram_addr to form
 *                              DRAM address used in transfer.
 * @param in_buf_addr           RBUF/TBUF address. Register or constant. Added to in_buf_addr_offset to
 *                              form rbuf address used in transfer.
 * @param in_buf_addr_offset    RBUF/TBUF address. Register or constant. Added to in_buf_addr to form
 *                              RBUF/TBUF address used in transfer.
 * @param in_qw_count           Register or constant. Number of quadwords to transfer from RBUF to DRAM
 *                              or from DRAM to TBUF.
 * @param REQ_SIG               Requested signal.
 * @param in_wakeup_sigs        List of signals causing thread to swap/wakeup.
 * @param in_reserved           Reserved for future use. Pass: as the value of this parameter.
 *
 * @note
 * @b Limitations: Granularity of transfer count is quadwords.
 *
 * @condition_codes Data dependent: assume they are clobbered.
 * @ixp
 */
#macro _dram_xbuf_xfr(in_operation, in_dram_addr, in_dram_addr_offset, in_buf_addr, in_buf_addr_offset, in_qw_count, REQ_SIG, in_wakeup_sigs, in_reserved)
.begin

    .reg indirect
    .set indirect
    .use indirect

    #if (IS_NFPTYPE(__NFP6000) || IS_NFP_TYPE(__NFP3800))
        #error "Rbufs/Tbufs are not available on NFP6000."
    #else
        #ifndef __NFP_INDIRECT_REF_FORMAT_IXP
            #error "This macro is currently only available in IXP indirect reference format mode."
        #endif
    #endif

    _GET_ADDRESSES(_MEM_ADDR0, _MEM_ADDR1, in_dram_addr, in_dram_addr_offset, DRAM)

    #define _INDIRECT_NOT_SET_YET // use this to minimize the number of emitted instructions

    // Process in_buf_addr and in_buf_addr_offset
    // Try to save a few cycles
    #if ( (is_ct_const(in_buf_addr) || isimport(in_buf_addr)) && (is_ct_const(in_buf_addr_offset) || isimport(in_buf_addr_offset)) ) // if both immediates:
        #ifdef _TBUF_ADDR
            #warning "_TBUF_ADDR is being redefined."
        #endif

        #define_eval _TBUF_ADDR (in_buf_addr + in_buf_addr_offset)

        #if (_TBUF_ADDR != 0) // then we must shift it into indirect
            #if (_TBUF_ADDR <= MAX_IMMEDIATE) // save alu if number is small enough
                #if ((_TBUF_ADDR*2) <= MAX_IMMEDIATE)
                    // We can set _TBUF_ADDR and set OV bit at the same time
                    #ifdef _TBUF_ADDR_FIELD
                        #warning "_TBUF_ADDR_FIELD is being redefined."
                    #endif
                    #define_eval _TBUF_ADDR_FIELD ( (_TBUF_ADDR*2) + 1 )
                    alu[indirect, --, b, _TBUF_ADDR_FIELD, <<__TBUF_ADDR_OV_OFFSET]
                    #undef _INDIRECT_NOT_SET_YET // we set it, so clear flag
                    #undef _TBUF_ADDR_FIELD
                #else // need extra alu because value > MAX_IMMEDIATE
                    alu[indirect, --, b, 1, <<__TBUF_ADDR_OV_OFFSET]
                    alu[indirect, indirect, OR, _TBUF_ADDR, <<__TBUF_ADDR_OFFSET]
                    #undef _INDIRECT_NOT_SET_YET
                #endif // (_TBUF_ADDR*2) <= MAX_IMMEDIATE
            #else // _TBUF_ADDR > MAX_IMMEDIATE
                .begin

                    .reg tmp_gpr
                    move(tmp_gpr, _TBUF_ADDR)
                    // Set ov bit and set addr field with same instruction
                    alu[indirect, _TBUF_ADDR_OV_VALUE, OR, tmp_gpr, <<__TBUF_ADDR_OFFSET]
                    #undef _INDIRECT_NOT_SET_YET

                .end
            #endif
        #endif // _TBUF_ADDR != 0
        #undef _TBUF_ADDR

    #else // they are not both immediates:

        #ifdef _TBUF_ADDR
            #warning "_TBUF_ADDR is being redefined."
        #endif

        // check if either one is an immediate
        #if (is_ct_const(in_buf_addr) || isimport(in_buf_addr))
            // in_buf_addr is immediate
            // in_buf_addr_offset is a gpr

            #define_eval _TBUF_ADDR in_buf_addr
            #if (_TBUF_ADDR <= MAX_IMMEDIATE)
                #if (_TBUF_ADDR != 0)
                    .begin

                        .reg tmp_tbuf_addr
                        alu[tmp_tbuf_addr, in_buf_addr_offset, +, _TBUF_ADDR]

                        #ifdef _INDIRECT_NOT_SET_YET
                            alu[indirect, _TBUF_ADDR_OV_VALUE, OR, tmp_tbuf_addr, <<__TBUF_ADDR_OFFSET]
                            #undef _INDIRECT_NOT_SET_YET
                        #else // indirect has already been set:
                            alu[indirect, indirect, OR, tmp_tbuf_addr, <<__TBUF_ADDR_OFFSET]
                        #endif

                    .end
                #else // in_buf_addr == 0:
                    #ifdef _INDIRECT_NOT_SET_YET
                        alu[indirect, _TBUF_ADDR_OV_VALUE, OR, in_buf_addr_offset, <<__TBUF_ADDR_OFFSET]
                        #undef _INDIRECT_NOT_SET_YET
                    #else // indirect has already been set:
                        alu[indirect, indirect, OR, in_buf_addr_offset, <<__TBUF_ADDR_OFFSET]
                    #endif
                #endif // in_buf_addr != 0
            #else // in_buf_addr > MAX_IMMEDIATE:
                .begin

                    .reg tmp_tbuf_addr tmp_immed_holder
                    move(tmp_tbuf_addr, in_buf_addr)
                    alu[tmp_tbuf_addr, in_buf_addr_offset, +, tmp_tbuf_addr]
                    #ifdef _INDIRECT_NOT_SET_YET
                        alu[indirect, _TBUF_ADDR_OV_VALUE, OR, tmp_tbuf_addr, <<__TBUF_ADDR_OFFSET]
                        #undef _INDIRECT_NOT_SET_YET
                    #else // indirect has already been set:
                        alu[indirect, indirect, OR, tmp_tbuf_addr, <<__TBUF_ADDR_OFFSET]
                    #endif

                .end
            #endif // in_buf_addr <= MAX_IMMEDIATE

        #elif (is_ct_const(in_buf_addr_offset) || isimport(in_buf_addr_offset))
            // in_buf_addr is gpr
            // in_buf_addr_offset is immediate

            #define_eval _TBUF_ADDR in_buf_addr_offset
            #if (_TBUF_ADDR <= MAX_IMMEDIATE)
                #if (_TBUF_ADDR != 0)
                    .begin

                        .reg tmp_tbuf_addr
                        alu[tmp_tbuf_addr, in_buf_addr, +, _TBUF_ADDR]
                        #ifdef _INDIRECT_NOT_SET_YET
                            alu[indirect, _TBUF_ADDR_OV_VALUE, OR, tmp_tbuf_addr, <<__TBUF_ADDR_OFFSET]
                            #undef _INDIRECT_NOT_SET_YET
                        #else // indirect has already been set:
                            alu[indirect, indirect, OR, tmp_tbuf_addr, <<__TBUF_ADDR_OFFSET]
                        #endif // indirect not set yet

                    .end
                #else // in_buf_addr_offset == 0
                    #ifdef _INDIRECT_NOT_SET_YET
                        alu[indirect, _TBUF_ADDR_OV_VALUE, OR, in_buf_addr, <<__TBUF_ADDR_OFFSET]
                        #undef _INDIRECT_NOT_SET_YET
                    #else // indirect has already been set:
                        alu[indirect, indirect, OR, in_buf_addr, <<__TBUF_ADDR_OFFSET]
                    #endif
                #endif // in_buf_addr_offset != 0
            #else // in_buf_addr_offset > MAX_IMMEDIATE:
                .begin

                    .reg tmp_tbuf_addr, tmp_immed_holder
                    move(tmp_immed_holder, in_buf_addr_offset)
                    alu[tmp_tbuf_addr, tmp_immed_holder, +, in_buf_addr]
                    #ifdef _INDIRECT_NOT_SET_YET
                        alu[indirect, _TBUF_ADDR_OV_VALUE, OR, tmp_tbuf_addr, <<__TBUF_ADDR_OFFSET]
                        #undef _INDIRECT_NOT_SET_YET
                    #else // indirect has already been set:
                        alu[indirect, indirect, OR, tmp_tbuf_addr, <<__TBUF_ADDR_OFFSET]
                    #endif // indirect not set yet

                .end
            #endif // in_buf_addr_offset <= MAX_IMMEDIATE
        #else // both are gprs
            .begin

                .reg tmp_tbuf_addr
                alu[tmp_tbuf_addr, in_buf_addr, +, in_buf_addr_offset]
                #ifdef _INDIRECT_NOT_SET_YET
                    alu[indirect, _TBUF_ADDR_OV_VALUE, OR, tmp_tbuf_addr, <<__TBUF_ADDR_OFFSET]
                    #undef _INDIRECT_NOT_SET_YET
                #else // indirect has already been set
                    alu[indirect, indirect, OR, tmp_tbuf_addr, <<__TBUF_ADDR_OFFSET]
                #endif // _INDIRECT_NOT_SET_YET

            .end
        #endif // either is an immediate
        #ifdef _TBUF_ADDR
            #undef _TBUF_ADDR
        #endif
    #endif // both are immediates

    #ifdef _QW_CNT
        #warning "_QW_CNT is being redefined."
    #endif

    // Process in_qw_count
    #if (is_ct_const(in_qw_count) || isimport(in_qw_count))
        #ifdef _act_in_qw_count
            #warning "_act_in_qw_count is being redefined."
        #endif

        #if (MEM_WD_COUNT_MIN_1 == TRUE)
            #define_eval _act_in_qw_count (in_qw_count + 1)
        #else
            #define_eval _act_in_qw_count in_qw_count
        #endif

        #if ( (_act_in_qw_count >= 1) && (_act_in_qw_count <= 8) )
            #define_eval _QW_CNT _act_in_qw_count
        #elif ( (_act_in_qw_count >= 9) && (_act_in_qw_count <= 16) )
            #define _QW_CNT MAX // dummy param used in instruction
            // then we must use indirect
            #ifdef _INDIRECT_NOT_SET_YET
                alu[indirect, --, b, 1, <<__TBUF_ADDR_OV_OFFSET]
                #undef _INDIRECT_NOT_SET_YET
            #endif
            #ifdef _REF_CNT_FIELD
                #warning "_REF_CNT_FIELD is being redefined."
            #endif
            #define_eval _REF_CNT_FIELD ((_act_in_qw_count-1) + (1<<(_REF_CNT_OV_OFFSET-_REF_CNT_OFFSET)))
            alu[indirect, indirect, OR, _REF_CNT_FIELD, <<_REF_CNT_OFFSET]
            #undef _REF_CNT_FIELD
        #else // in_qw_count out of range:
            #error "in_qw_count must be >= 1 and <= 16"
        #endif
        #undef _act_in_qw_count
    #else // in_qw_count is a gpr
        #define _QW_CNT max // dummy param used in instruction
        #ifdef _INDIRECT_NOT_SET_YET
            .begin

                #ifdef ___ind_qw_count
                    #warning "___ind_qw_count is being redefined"
                #endif

                #if (MEM_WD_COUNT_MIN_1 != TRUE)
                    .reg cnt_m_1
                    alu[cnt_m_1, in_qw_count, -, 1]
                    #define_eval ___ind_qw_count cnt_m_1
                #else
                    #define_eval ___ind_qw_count in_qw_count
                #endif

                alu[indirect, _TBUF_ADDR_OV_VALUE, OR, ___ind_qw_count, <<_REF_CNT_OFFSET] // shift in qw count
                #undef ___ind_qw_count

            .end
            #undef _INDIRECT_NOT_SET_YET
        #else // indirect has already been set:
            .begin

                #ifdef ___ind_qw_count
                    #warning "___ind_qw_count is being redefined"
                #endif

                #if (MEM_WD_COUNT_MIN_1 != TRUE)
                    .reg cnt_m_1
                    alu[cnt_m_1, in_qw_count, -, 1]
                    #define_eval ___ind_qw_count cnt_m_1
                #else
                    #define_eval ___ind_qw_count in_qw_count
                #endif
                alu[indirect, indirect, OR, ___ind_qw_count, <<_REF_CNT_OFFSET] // shift in qw count
                #undef ___ind_qw_count

            .end
        #endif // _INDIRECT_NOT_SET_YET
        // set indirect ref for dram[tbuf_wr...] instruction
        alu[indirect, indirect, OR, 1, <<_REF_CNT_OV_OFFSET] // set ref cnt override bit
    #endif

    #ifdef _INDIRECT_NOT_SET_YET
        alu[--, --, b, 1, <<__TBUF_ADDR_OV_OFFSET] // set TBUF byte address override bit
        #undef _INDIRECT_NOT_SET_YET
    #endif

    #ifdef _INDIRECT_NOT_SET_YET
        alu[--, --, b, 0]
    #endif

    // This is IXP indref format, use IXP style command for MSF xfers.
    dram[in_operation, --, _MEM_ADDR0, _MEM_ADDR1, _QW_CNT], indirect_ref, sig_done[REQ_SIG]
    #if (!streq('in_wakeup_sigs', 'SIG_NONE'))
        ctx_arb[in_wakeup_sigs]
    #endif

    #ifdef _INDIRECT_NOT_SET_YET
        #undef _INDIRECT_NOT_SET_YET
    #endif

    _FREE_ADDRESSES()

    #undef _QW_CNT

.end
#endm
/// @endcond


/** Alias to @ref ddr_add64_immed_init.
 *
 * @param indirect_ref_reg  GPR to be initialized
 * @param en_64_bit         Constant Boolean value. @n
 *                          @arg 0: Perform 32-bit Add operations
 *                          @arg 1: Perform 64-bit Add operations
 * @param ref_cnt           Reference count. Constant. @n
 *                          Valid values: 0, 1, 2, 3.
 * @param byte_mask_dm_dr   Not used, must be 1.
 *
 * @deprecated
 */
#macro ddr_add64_immed_init(indirect_ref_reg, en_64_bit, ref_cnt, byte_mask_dm_dr)
    #if (byte_mask_dm_dr != 1)
        #error "byte_mask_dm_dr must be 1."
    #endif

    #warning "This overloaded version of ddr_add64_immed_init is deprecated due to removal of parameter byte_mask_dm_dr."
    ddr_add64_immed_init(indirect_ref_reg, en_64_bit, ref_cnt)
#endm

/** Initialize the indirect reference register for @ref ddr_add64_immed.
 *
 * Using a static register for indirect reference, saves few instructions for every immed add operation.
 *
 * @param indirect_ref_reg  GPR to be initialized
 * @param en_64_bit         Constant Boolean value. @n
 *                          @arg 0: Perform 32-bit Add operations
 *                          @arg 1: Perform 64-bit Add operations
 * @param ref_cnt           Reference count. Constant. @n
 *                          Valid values: 0, 1, 2, 3.
 */
#macro ddr_add64_immed_init(indirect_ref_reg, en_64_bit, ref_cnt)
.begin

    #if (is_ct_const(en_64_bit))
        #if ( (en_64_bit < 0) || (en_64_bit > 1) )
            #error " Value must be boolean (0: 32-bit Adds, 1: 64-bit Adds)."
        #endif
    #else
        #error "Value must be constant."
    #endif

    #if (is_ct_const(ref_cnt))
        #if ( (ref_cnt < 0) || (ref_cnt > 3) )
            #error " Reference count must be in range 0-3."
        #endif
    #else
        #error "Reference count field must be constant."
    #endif

    #define_eval _IND_LENGTH ((en_64_bit << 2) | ref_cnt)

    #ifdef __NFP_INDIRECT_REF_FORMAT_IXP
        #define_eval _IND ( (_IND_LENGTH << 1) | 0x21 )
        alu[indirect_ref_reg, --, B, _IND, <<20]
        #undef _IND
    #elif (defined(__NFP_INDIRECT_REF_FORMAT_V2))
        alu[indirect_ref_reg, indirect_ref_reg, or, 2, <<3]
    #else
        // 14-bit immediate data
        #define_eval _IND ( (_IND_LENGTH) | (1 << 3) )
        alu[indirect_ref_reg, --, B, 1, <<28] // Encoding Field
        alu[indirect_ref_reg, indirect_ref_reg, OR, _IND]
        #undef _IND
    #endif

    #undef _IND_LENGTH

.end
#endm


/// @cond INTERNAL_MACROS
#macro _ddr_add64_immed_cmd(memcmd, indirect_ref_reg, val, addr, offset)
.begin

    #if (is_ct_const(val))
        #if (val > 0x7F)
            #ifdef __NFP_INDIRECT_REF_FORMAT_IXP
                #error "Only 7-bit immediate data is supported in IXP indirect reference mode."
            #endif
            .reg val_reg
            immed[val_reg, val]
            #define ADD_VALUE   val_reg
        #else
            #define_eval ADD_VALUE   val
        #endif
    #else
        #define_eval ADD_VALUE   val
    #endif

    #if (is_ct_const(offset))
        #if (offset & 0x7)
            #error "Offset must be 8-byte aligned."
        #endif
    #endif

    #if (is_ct_const(addr) && is_ct_const(offset))
        .reg addr_reg
        immed32(addr_reg, (addr+offset))
        #define_eval BASE_ADDR   addr_reg
        #define_eval ADDR_OFFSET 0
    #elif (is_ct_const(addr) && !is_ct_const(offset))
        .reg addr_reg
        #if (defined(__NFP_INDIRECT_REF_FORMAT_IXP) || defined(__NFP_THIRD_PARTY_ADDRESSING_32_BIT))
            immed32(addr_reg, addr)
            #define_eval BASE_ADDR   addr_reg
            #define_eval ADDR_OFFSET offset
        #else
            // NFP-indref && 40-bit-3rd-party
            immed32(addr_reg, addr)
            alu[addr_reg, addr_reg, +, offset]
            #define_eval BASE_ADDR   addr_reg
            #define_eval ADDR_OFFSET 0
        #endif
    #elif (!is_ct_const(addr) && is_ct_const(offset))
        #if (defined(__NFP_INDIRECT_REF_FORMAT_IXP) || defined(__NFP_THIRD_PARTY_ADDRESSING_32_BIT))
            #define_eval BASE_ADDR   addr
            #define_eval ADDR_OFFSET offset
        #else
            // NFP-indref && 40-bit-3rd-party
            #if (offset == 0)
                #define_eval BASE_ADDR   addr
                #define_eval ADDR_OFFSET 0
            #else
                .reg addr_reg
                alu[addr_reg, addr, +, offset]
                #define_eval BASE_ADDR   addr_reg
                #define_eval ADDR_OFFSET 0
            #endif
        #endif
    #else // (!is_ct_const(addr) && !is_ct_const(offset))
        #if (defined(__NFP_INDIRECT_REF_FORMAT_IXP) || defined(__NFP_THIRD_PARTY_ADDRESSING_32_BIT))
            #define_eval BASE_ADDR   addr
            #define_eval ADDR_OFFSET offset
        #else
            // NFP-indref && 40-bit-3rd-party
            .reg addr_reg
            alu[addr_reg, addr, +, offset]
            #define_eval BASE_ADDR   addr_reg
            #define_eval ADDR_OFFSET 0
        #endif
    #endif

    #ifdef __NFP_INDIRECT_REF_FORMAT_IXP
        alu[--, indirect_ref_reg, or, ADD_VALUE, <<12]
        mem[memcmd, --, BASE_ADDR, ADDR_OFFSET], indirect_ref
    #elif __NFP_INDIRECT_REF_FORMAT_V2
        alu[--, indirect_ref_reg, or, ADD_VALUE, <<16]
        mem[memcmd, --, BASE_ADDR, ADDR_OFFSET], indirect_ref, force_third_party
    #else
        // bits [18:5] is immediate data
        alu[--, indirect_ref_reg, or, ADD_VALUE, <<5]
        mem[memcmd, --, BASE_ADDR, ADDR_OFFSET], indirect_ref, force_third_party
    #endif

    #undef BASE_ADDR
    #undef ADDR_OFFSET

.end
#endm
/// @endcond

/** Given indirect reference register, value, address and offset, do immed add.
 *
 * In NFP indirect reference mode, 14-bit values are supported.
 * In IXP indirect reference mode, only 7-bit values are supported.
 *
 * @param indirect_ref_reg  Indirect reference register initialized using ddr_add64_immed_init macro
 * @param val               Value to be added - constant or GPR
 * @param addr              DRAM address where add to be performed
 * @param offset            Offset from 'addr', where add is to be performed.
 *                          must by 8-byte aligned.
 */
#macro ddr_add64_immed(indirect_ref_reg, val, addr, offset)
    _ddr_add64_immed_cmd(add64_imm, indirect_ref_reg, val, addr, offset)
#endm


/** Given indirect reference register, value, address and offset, do immed add (saturates at max value).
 *
 * In NFP indirect reference mode, 14-bit values are supported.
 * In IXP indirect reference mode, only 7-bit values are supported.
 *
 * @param indirect_ref_reg  Indirect reference register initialized using ddr_add64_immed_init macro
 * @param val               Value to be added - constant or GPR
 * @param addr              DRAM address where add to be performed
 * @param offset            Offset from 'addr', where add to be performed.
 *                          must by 8-byte aligned.
 */
#macro ddr_add64_immed_sat(indirect_ref_reg, val, addr, offset)
    _ddr_add64_immed_cmd(addsat64_imm, indirect_ref_reg, val, addr, offset)
#endm


/** Compare a region of DRAM memory with a specified pattern.
 *
 * Comparison is done on data from @p in_cur_dram_addr (labelled @c A) to
 * data at @p in_src_dram_addr (labelled @c B).
 *
 * @param in_cur_addr_1         Address specification - see note.
 * @param in_cur_addr_2         Address specification - see note.
 * @param in_cur_addr_3         Address specification - see note.
 * @param in_src_addr_1         Address specification - see note.
 * @param in_src_addr_2         Address specification - see note.
 * @param in_src_addr_3         Address specification - see note.
 * @param in_cur_len            Number of bytes to compare
 * @param ret_val               Set to the first address in @c A where a mismatch was found otherwise unmodified
 *
 * @note
 * <b>Address specification</b> takes the form "reg_or_const1, <<8, reg_or_const2" or "reg_or_const1, reg_or_const2, <<8".
 */
#macro dram_memcmp(in_cur_addr_1, in_cur_addr_2, in_cur_addr_3, in_src_addr_1, in_src_addr_2, in_src_addr_3, in_cur_len, ret_val)
    //Both shift addresses can be -- but they must be in the last position
    #if (streq('in_cur_addr_2', '--') && streq('in_src_addr_2', '--'))
        #error "If both address are not shifted, -- must be specified after the address and offset"
    #endif
    #if (streq('in_cur_addr_2', '<<8'))
        #if (streq('in_src_addr_2', '<<8'))
            _dram_memcmp_32_40(in_cur_addr_1, in_cur_addr_3, in_src_addr_1, in_src_addr_3, in_cur_len, ret_val, A<<8, A<<8)
        #elif (streq('in_src_addr_3', '<<8'))
            _dram_memcmp_32_40(in_cur_addr_1, in_cur_addr_3, in_src_addr_1, in_src_addr_2, in_cur_len, ret_val, A<<8, B<<8)
        #else
            #if (streq('in_src_addr_3', '--'))
                _dram_memcmp_32_40(in_cur_addr_1, in_cur_addr_3, in_src_addr_1, in_src_addr_2, in_cur_len, ret_val, A<<8, --)
            #else
                #error "in_src_addr_3 must be either <<8 or --"
            #endif
        #endif
    #elif (streq('in_cur_addr_3', '<<8'))
        #if (streq('in_src_addr_2', '<<8'))
            _dram_memcmp_32_40(in_cur_addr_1, in_cur_addr_2, in_src_addr_1, in_src_addr_3, in_cur_len, ret_val, B<<8, A<<8)
        #elif (streq('in_src_addr_3', '<<8'))
            _dram_memcmp_32_40(in_cur_addr_1, in_cur_addr_2, in_src_addr_1, in_src_addr_2, in_cur_len, ret_val, B<<8, B<<8)
        #else
            #if (streq('in_src_addr_3', '--'))
                _dram_memcmp_32_40(in_cur_addr_1, in_cur_addr_2, in_src_addr_1, in_src_addr_2, in_cur_len, ret_val, B<<8, --)
            #else
                #error "in_src_addr_3 must be either <<8 or --"
            #endif
        #endif
    #else
        #if (streq('in_cur_addr_3', '--'))
            #if (streq('in_src_addr_2', '<<8'))
                _dram_memcmp_32_40(in_cur_addr_1, in_cur_addr_2, in_src_addr_1, in_src_addr_3, in_cur_len, ret_val, --, A<<8)
            #elif (streq('in_src_addr_3', '<<8'))
                _dram_memcmp_32_40(in_cur_addr_1, in_cur_addr_2, in_src_addr_1, in_src_addr_2, in_cur_len, ret_val, --, B<<8)
            #elif (streq('in_src_addr_3', '--'))
                _dram_memcmp_32_40(in_cur_addr_1, in_cur_addr_2, in_src_addr_1, in_src_addr_2, in_cur_len, ret_val, --, --)
            #else
                #error "in_src_addr_3 must be either <<8 or --"
            #endif
        #else
                #error "in_cur_addr_3 must be either <<8 or --"
        #endif
    #endif
#endm

//Internal macro
/// @cond INTERNAL_MACROS
#macro _dram_memcmp_32_40(in_cur_dram_addr, in_cur_dram_offset, in_src_dram_addr, in_src_dram_offset, in_cur_len, ret_val, in_cur_shift, in_src_shift)
.begin

    .reg base_addr, cur_dram_addr, expect_dram_addr, cur_len
    .reg base_src

    #if (defined(__NFP_INDIRECT_REF_FORMAT_IXP))
        #error "This macro is currently only available in NFP indirect reference format mode."
    #else
        #define _DEST_XFER   $dest_in
    #endif


    .reg _DEST_XFER[16]
    .reg $exp_in[16]

    .xfer_order _DEST_XFER
    .xfer_order $exp_in
    .sig dram_sig, dram_sig2

    move(cur_dram_addr, in_cur_dram_addr)
    move(expect_dram_addr, in_src_dram_addr)
    move(cur_len, in_cur_len)

    .reg nw_passed, nw_failed, dmask
    alu[nw_passed, --, B, 0]
    alu[nw_failed, --, B, 0]

    .while (cur_len > 0)

        // Compare "in_cur_len" bytes at "in_cur_dram_addr" to "in_src_dram_addr"
        .reg temp, e_val, d_val

        #if (streq('in_cur_shift', 'A<<8'))
            mem[read, _DEST_XFER[0], cur_dram_addr, <<8, in_cur_dram_offset, 8], sig_done[dram_sig2]
        #elif (streq('in_cur_shift', 'B<<8'))
            mem[read, _DEST_XFER[0], cur_dram_addr, in_cur_dram_offset, <<8, 8], sig_done[dram_sig2]
        #else
            mem[read, _DEST_XFER[0], cur_dram_addr, in_cur_dram_offset, 8], sig_done[dram_sig2]
        #endif

        #if (streq('in_src_shift', 'A<<8'))
            mem[read, $exp_in[0], expect_dram_addr, <<8, in_src_dram_offset, 8], sig_done[dram_sig]
        #elif (streq('in_src_shift', 'B<<8'))
            mem[read, $exp_in[0], expect_dram_addr, in_src_dram_offset, <<8, 8], sig_done[dram_sig]
        #else
            mem[read, $exp_in[0], expect_dram_addr, in_src_dram_offset, 8], sig_done[dram_sig]
        #endif

        ctx_arb[dram_sig, dram_sig2]

        #define_eval    _COUNT  0

        #while  (_COUNT < 16)

        .if (cur_len > 0)
            alu[d_val, --, B, _DEST_XFER[_COUNT]]
            alu[e_val, --, B, $exp_in[_COUNT]]
            move(dmask, 0xFFFFFFFF)

            .if (cur_len < 4)

                .begin

                    .reg rem
                    // Writing last word - only write bytes based on packet length
                    alu[rem, 4, -, cur_len]
                    // Calculate number of bits to shift rem bytes * 8 bits
                    alu_shf[rem, --, B, rem, <<3]
                    // Use alu_shf indirect to zero part of mask for the last long word
                    alu[--, rem, OR, 0]
                    alu_shf[dmask, --, B, dmask, <<indirect]

                .end

            .endif

            // if there is an error, temp will be non-zero
            alu[d_val, d_val, AND, dmask]
            alu[e_val, e_val, AND, dmask]
            alu[temp, d_val, xor, e_val]
            .if (temp != 0)
                alu[nw_failed, nw_failed, +, 1]
            .else
                alu[nw_passed, nw_passed, +, 1]
            .endif

            // Capture dram_addr where first error was seen
            .if (nw_failed == 1)
                alu[ret_val, --, B, cur_dram_addr]
            .endif

            alu[cur_len, cur_len, -, 4]

        .endif

        #define_eval    _COUNT  (_COUNT + 1)

        #endloop        /* _COUNT */

        #undef _COUNT

        //We can only increment addresses that are used as offsets (not shifted addresses)
        #if (streq('in_cur_shift', 'A<<8'))
            alu[in_cur_dram_offset, in_cur_dram_offset, +, 64]
        #else
            alu[cur_dram_addr, cur_dram_addr, +, 64]
        #endif

        #if (streq('in_src_shift', 'A<<8'))
            alu[in_src_dram_offset, in_src_dram_offset, +, 64]
        #else
            alu[expect_dram_addr, expect_dram_addr, +, 64]
        #endif

    .endw
    //
    // display number of words passed and number of words that failed
    // log msg : upper two bytes number of words failed
    //           lower two bytes number of words passed

    alu_shf[nw_passed, nw_passed, OR, nw_failed, <<16]

.end
#endm
/// @endcond

/** Compare a region of DRAM memory with a specified pattern.
 *
 * Comparison is done on data from @p in_cur_dram_addr (labelled @c A) to
 * data at @p in_src_dram_addr (labelled @c B).
 *
 * @param in_cur_dram_addr      Byte address to start comparing from (@c A)
 * @param in_cur_dram_offset    Offset added to in_cur_dram_addr to determine start address
 * @param in_src_dram_addr      Byte address to start comparing against (@c B)
 * @param in_src_dram_offset    Offset added to in_src_dram_addr to determine start address
 * @param in_cur_len            Number of bytes to compare
 * @param ret_val               Set to the first address in @c A where a mismatch was found or unmodified
 */
#macro dram_memcmp(in_cur_dram_addr, in_cur_dram_offset, in_src_dram_addr, in_src_dram_offset, in_cur_len, ret_val)
    #if (streq('in_cur_dram_offset', '--'))
        //We need the following for compatability
        #define IN_CUR_DRAM_OFFSET 0
    #else
        #define_eval IN_CUR_DRAM_OFFSET in_cur_dram_offset
    #endif

    #if (streq('in_src_dram_offset', '--'))
        //We need the following for compatability
        #define IN_SRC_DRAM_OFFSET 0
    #else
        #define_eval IN_SRC_DRAM_OFFSET in_src_dram_offset
    #endif
    _dram_memcmp_32_40(in_cur_dram_addr, IN_CUR_DRAM_OFFSET, in_src_dram_addr, IN_SRC_DRAM_OFFSET, in_cur_len, ret_val, --, --)
#endm

/** Fill a region of DRAM memory with a specified pattern.
 *  Compatibility version, which does not use an offset parameter
 *
 * @param in_dram_addr Address to start memory fill from
 * @param in_len       Number of bytes to set. Must be a multiple of @p CHUNK_SIZE.
 * @param lw_pattern   32-bit pattern to fill memory region with
 * @param CHUNK_SIZE   Chunk size, a multiple of 8 bytes from 8 to 64. Must be a constant.
 *
 */
#macro dram_memset(in_dram_addr, in_len, lw_pattern, CHUNK_SIZE)
    _dram_memset_32_40(in_dram_addr, 0, in_len, lw_pattern, CHUNK_SIZE, --)
#endm

/** Fill a region of DRAM memory with a specified pattern.
 *  Compatibility version, which does not use an offset parameter
 *
 * @param in_dram_addr Address to start memory fill from
 * @param in_addr_offset Added to in_dram_addr to form the DRAM address used in transfer
 * @param in_len       Number of bytes to set. Must be a multiple of @p CHUNK_SIZE.
 * @param lw_pattern   32-bit pattern to fill memory region with
 * @param CHUNK_SIZE   Chunk size, a multiple of 8 bytes from 8 to 64. Must be a constant.
 *
 */
#macro dram_memset(in_dram_addr, in_addr_offset, in_len, lw_pattern, CHUNK_SIZE)
    _dram_memset_32_40(in_dram_addr, in_addr_offset, in_len, lw_pattern, CHUNK_SIZE, --)
#endm

/** Fill a region of DRAM memory with a specified pattern.
 *
 * @param in_addr_1         Address specification - see note.
 * @param in_addr_2         Address specification - see note.
 * @param in_addr_3         Address specification - see note.
 * @param in_len       Number of bytes to set. Must be a multiple of @p CHUNK_SIZE.
 * @param lw_pattern   32-bit pattern to fill memory region with
 * @param CHUNK_SIZE   Chunk size, a multiple of 8 bytes from 8 to 64. Must be a constant.
 *
 * @note
 * <b>Address specification</b> takes the form "reg_or_const1, <<8, reg_or_const2" or "reg_or_const1, reg_or_const2, <<8".
 */
#macro dram_memset(in_addr_1, in_addr_2, in_addr_3, in_len, lw_pattern, CHUNK_SIZE)
    _dram_memset_32_40(in_addr_1, in_addr_2, in_addr_3, in_len, lw_pattern, CHUNK_SIZE)
#endm

//Internal macro for both 32 and 40 bit memset
/// @cond INTERNAL_MACROS
#macro _dram_memset_32_40(in_p1, in_p2, in_p3, in_p4, in_p5, in_p6)
    #if (streq('in_p2', '<<8'))
        _dram_memset(in_p1, in_p3, in_p4, in_p5, in_p6, A<<8)
    #elif (streq('in_p3', '<<8'))
        _dram_memset(in_p1, in_p2, in_p4, in_p5, in_p6, B<<8)
    #else
        _dram_memset(in_p1, in_p2, in_p3, in_p4, in_p5, in_p6)
    #endif
#endm
/// @endcond

/// @cond INTERNAL_MACROS
#macro _dram_memset(in_dram_addr, in_addr_offset, in_len, lw_pattern, CHUNK_SIZE, in_addr_shift)
.begin

    #if (!is_ct_const(CHUNK_SIZE))
        #error "dram_memset: Chunk size must be constant."
    #endif

    #if ( (CHUNK_SIZE <= 0) || (CHUNK_SIZE > 64) )
        #error "dram_memset: Chunk Size must be in multiples of 8 bytes ranging 8-64 :"[CHUNK_SIZE]
    #endif

    #if (CHUNK_SIZE & 0x7)
        #error "dram_memset: Chunk Size must be in multiples of 8 bytes:" [CHUNK_SIZE]
    #endif

    //Verify that in_len is a multiple of CHUNK_SIZE
    #if ((in_len % CHUNK_SIZE) != 0)
        #define_eval CHUNK_REPR CHUNK_SIZE
        #define_eval in_len_repr in_len
        #error "in_len("in_len_repr") is not a multiple of chunk size("CHUNK_REPR")"
    #endif

    #if ( defined(__NFP_INDIRECT_REF_FORMAT_V1) || defined(__NFP_INDIRECT_REF_FORMAT_V2))
        #define _DEST_XFER   $dest_in
    #else
        #define _DEST_XFER   $$dest_in
    #endif

    #define_eval _NUM_XFERS  (CHUNK_SIZE / 4)

    #define_eval _CHUNK_REF_CNT (CHUNK_SIZE / 8)

    .reg _DEST_XFER[_NUM_XFERS]

    .xfer_order _DEST_XFER
    .sig dram_sig

    #if (is_ct_const(in_dram_addr))
        .reg cur_dram_addr
        move(cur_dram_addr, in_dram_addr)
        #define_eval _DRAM_BASE_ADDR_    cur_dram_addr
    #else
        #define_eval _DRAM_BASE_ADDR_    in_dram_addr
    #endif

    #if (is_ct_const(in_addr_offset) && \
        ((in_addr_offset > 0x7F) || \
        streq('in_addr_shift', 'A<<8') || \
        streq('in_addr_shift', 'B<<8')) )
        .reg cur_addr_offset
        move(cur_addr_offset, in_addr_offset)
        #define_eval _DRAM_ADDR_OFFSET_    cur_addr_offset
    #else
        #define_eval _DRAM_ADDR_OFFSET_    in_addr_offset
    #endif

    #if (is_ct_const(in_len))
        .reg cur_len
        move(cur_len, in_len)
        #define_eval _REM_BYTES_    cur_len
    #else
        #define_eval _REM_BYTES_    in_len
    #endif

    #if (is_ct_const(lw_pattern))
        .reg cur_pattern
        move(cur_pattern, lw_pattern)
        #define_eval _PATTERN_   cur_pattern
    #else
        #define_eval _PATTERN_   lw_pattern
    #endif

    #define_eval    _COUNT  0

    #while  (_COUNT < _NUM_XFERS)
        alu[_DEST_XFER[_COUNT], --, B, _PATTERN_]
        #define_eval    _COUNT  (_COUNT + 1)
    #endloop        /* _COUNT */

    #undef _COUNT

    .while (_REM_BYTES_ > 0)

        //
        // init dram to long word pattern specified
        //
        #if (streq('in_addr_shift', 'A<<8'))
            mem[write, _DEST_XFER[0], _DRAM_BASE_ADDR_, <<8, _DRAM_ADDR_OFFSET_, _CHUNK_REF_CNT], sig_done[dram_sig]
            alu[_DRAM_ADDR_OFFSET_, _DRAM_ADDR_OFFSET_, +, CHUNK_SIZE]
        #elif (streq('in_addr_shift', 'B<<8'))
            mem[write, _DEST_XFER[0], _DRAM_BASE_ADDR_, _DRAM_ADDR_OFFSET_, <<8, _CHUNK_REF_CNT], sig_done[dram_sig]
            alu[_DRAM_ADDR_OFFSET_, _DRAM_ADDR_OFFSET_, +, CHUNK_SIZE]
        #else
            mem[write, _DEST_XFER[0], _DRAM_BASE_ADDR_, _DRAM_ADDR_OFFSET_, _CHUNK_REF_CNT], sig_done[dram_sig]
            alu[_DRAM_BASE_ADDR_, _DRAM_BASE_ADDR_, +, CHUNK_SIZE]
        #endif

        ctx_arb[dram_sig]

        alu[_REM_BYTES_, _REM_BYTES_, -, CHUNK_SIZE]

    .endw

    #undef _DRAM_BASE_ADDR_
    #undef _DRAM_ADDR_OFFSET_
    #undef _REM_BYTES_
    #undef _PATTERN_
    #undef _NUM_XFERS
    #undef _CHUNK_REF_CNT
    #undef _DEST_XFER

.end
#endm
/// @endcond


/** @}
 * @}
 */

#endif /* __DRAM_UC__ */
