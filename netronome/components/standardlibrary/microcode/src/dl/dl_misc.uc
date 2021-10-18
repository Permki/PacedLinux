/*
 * Copyright (C) 2009-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __DL_MISC_UC__
#define __DL_MISC_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file dl_misc.uc Dispatch Loop Miscellaneous Macros
 * @addtogroup dl_misc Dispatch Loop Miscellaneous
 * @{
 *
 * @name Dispatch Loop Miscellaneous Macros
 * @{
 *
 */

#define dl_set_exception               dl_exception_set
#define dl_set_exception_priority      dl_exception_set_priority
#define dl_set_exception_high_priority dl_exception_set_high_priority
#define dl_get_block_id                dl_exception_get_block_id
#define dl_get_exception_code          dl_exception_get_exception_code
#define dl_get_priority                dl_exception_get_priority

/** Set the exception code.
 *
 */
#macro dl_exception_set(block_id, exception_code)
.begin

    alu[dl_exception_reg, dl_exception_reg, OR, block_id, 0]
    alu[dl_exception_reg, dl_exception_reg, OR, exception_code, <<10]

.end
#endm

/** Set the exception code priority to Low (0) or High (1).
 */
#macro dl_exception_set_priority(priority)
    #if((!is_ct_const(priority)) || (priority < 0) || (priority > 1))
        #error  "Priority should be a either 0 or 1"
    #endif

    #if (priority == 1)
        alu[dl_exception_reg, dl_exception_reg, OR, 0x1, <<31]  //set bit
    #else
        alu[dl_exception_reg, dl_exception_reg, AND~, 0x1, <<31] // clear bit
    #endif
#endm

/** Set the exception code with high priority.
 */
#macro dl_exception_set_high_priority(block_id, exception_code)
.begin

    dl_set_exception_priority(1)
    dl_set_exception[block_id, exception_code]

.end
#endm


/** Get the block id / COM id.
 */
#macro dl_exception_get_block_id(block_id)
.begin

    .reg temp

    immed[temp, 0x3ff]
    alu[block_id, dl_exception_reg, AND, temp]

.end
#endm

/** Get the exception code.
 */
#macro dl_exception_get_exception_code(exception_code)
.begin

    alu[exception_code, 0xff, AND, dl_exception_reg, >>10]

.end
#endm

/** Get the priority.
 */
#macro dl_exception_get_priority(priority)
.begin

    alu[priority, --, b, dl_exception_reg, >>31]

.end
#endm

#ifndef DISABLE_PR5_REWORK
/** This macro calculates the number of csix-frames in the sop buffer.
 *
 * This recomputation of c-frames is required in the sop buffer as some
 * microblocks after the RX block might have updated the buffer size according
 * to the encapsulation the packet has.
 *
 * @note This algorithm gives count of c-frames one less than actual as required
 *       by the hardware.
 *
 * c-frames count = (buff_size + 8)/120 where divide is not a regular divide
 * but our special divide which yields 1 less for numbers that are integral multiples
 * of 120.
 *
 * @b Example:
 * @code
 * buf size            c-frame cnt returned
 * 230 = 112 + 118            1
 * 231 = 112 + 119            1
 * 232 = 112 + 120            1
 * 233 = 112 + 120 + 1        2
 * 234 = 112 + 120 + 2        2
 * @endcode
 *
 * @param sop_buf_handle   Buffer handle of first buffer in buffer chain
 *
 * @instruction_cnt Worst case: 12
 */
#macro compute_sop_buf_cframes(sop_buf_handle)
.begin

    .reg sop_buf_size, sop_cell_count, buf_handle_msb, temp1, temp2 offset_mod_8

    ld_field_w_clr[buf_handle_msb, 0001, sop_buf_handle, >>30]

    dl_meta_get_buffer_size(sop_buf_size)

    // The CSIX Tx, when transfering data from DRAM to TBUF,
    // accesses the DRAM at quadword boundary. As a result, there is
    // a slack of (buf_offset%8) bytes in TBUF before the payload starts
    // in case of SOP.EOP cframe. We need to account for this also.
    dl_meta_get_offset(offset_mod_8)
    alu[offset_mod_8, offset_mod_8, AND, 0x7]
    alu[sop_buf_size, sop_buf_size, +, offset_mod_8]

    // now we re-compute the cell size

    alu[sop_buf_size, sop_buf_size, +, 8]

    // now execute divide by 120
    // The algorithm we use to divide by 120 (remember this is integer division).
    // (x/120) = ((x<<4+x)<<4+x)>>15

    alu_shf[temp2, --, B, sop_buf_size, <<4]
    alu[temp1, temp2, +, sop_buf_size]
    alu_shf[temp1, --, B, temp1, <<4]
    alu[temp1, temp1, +, sop_buf_size]
    alu_shf[sop_cell_count, --, B, temp1, >>15]


set_cell_count#:
    alu_shf[buf_handle_msb, sop_cell_count, OR, buf_handle_msb, <<6]
    ld_field[dl_buf_handle, 1000, buf_handle_msb, <<24]

.end
#endm // compute_sop_buf_cframes()

/** Initialize the Next Neighbour Mode (NN mode) for the producer.
 *
 * For the producer, NN Registers are written from this ME. [bit 20 = 1]
 */
#macro nn_prod_init()
.begin

    .reg csr_data

    local_csr_rd[CTX_ENABLES]
    immed[csr_data, 0]

    // For the source, NN Registers are wrriten: from This ME. [bit 20 = 1]

    bits_set(csr_data, 20, 1)                       ; [20] = 1

    local_csr_wr[CTX_ENABLES, csr_data]

.end
#endm

/** Initialize the Next Neighbour Mode (NN mode) for the consumer.
 *
 * For the consumer, NN Registers are written from previous ME. [bit 20 = 0]
// We do some one time initialization in consumer???
// (so producer need not initialize them) Should it be producer???? TBD
 */
#macro nn_cons_init()
.begin

    .reg csr_data

    local_csr_rd[CTX_ENABLES]
    immed[csr_data, 0]

    //  For the sink, NN Registers are wrriten: from previous ME. [bit 20 = 0]

    bits_clr(csr_data, 20, 1)                       ; [20] = 0

    // Tell when to trigger ring empty. i.e when nn_get == nn_put (0 entires valid)
    // We have kept this and above instruction (alu_shf) separate for clarity.
    // we can always merge them when the need arises.

    bits_clr(csr_data, 18, 3)                       ; [19:18] = 0
    local_csr_wr[CTX_ENABLES, csr_data]

    local_csr_wr[NN_GET, 0]
    local_csr_wr[NN_PUT, 0]

.end
#endm

/** This macro writes the two longwords of exception and triggers interrupt to activate RM ISR for
 * exception packets.
 *
 * @param in_buffer_handle
 * @param in_exception_code
 *
 // Constant    : nil
 * @instruction_cnt 4
// Branches    : 0
 */
#macro dl_exception_send(in_dl_buf_handle)
.begin

    .reg    priority

    /* check for priority */
    dl_exception_get_priority(priority)
    br_bclr[priority, 0, low_priority#]

    dl_exception_write_ring(in_dl_buf_handle, dl_exception_reg, EXCEPTION_RING_OUT_1, XSCALE_INT_B)
    br[end#]

low_priority#:

    dl_exception_write_ring(in_dl_buf_handle, dl_exception_reg, EXCEPTION_RING_OUT_0, XSCALE_INT_A)
    br[end#]

end#:

.end
#endm


/** This macro writes the two longwords of exception and triggers interrupt to activate RM ISR for
 * exception packets.
 *
 * @param in_buffer_handle
 * @param in_exception_code
// Constant    : nil
 * @instruction_cnt 4
// Branches    : 0
 */
#macro dl_exception_write_ring(in_buffer_handle, in_exception_code, IN_RING_NUM, IN_INTERRUPT)
.begin

    .reg $excep_lw0, $excep_lw1, ring
    .xfer_order $excep_lw0, $excep_lw1
    .sig scratch_wr_done


    // set the ring number

    alu[ring, --, b, IN_RING_NUM, <<2]

    // set the exception longwords

    alu[$excep_lw0, --, b, in_buffer_handle]
    alu[$excep_lw1, --, b, in_exception_code]

    // check if transmit ring is full

    #define_eval RING_NUM IN_RING_NUM

    scr_write#:

    br_inp_state[SCR_RING/**/RING_NUM/**/_STATUS, scratch_ring_full#]

    // write the two longwords to the exception ring

    scratch[put, $excep_lw0, ring, 0, 2], sig_done[scratch_wr_done]
    ctx_arb[scratch_wr_done]

    // trigger the interrupt

    cap[fast_wr, 0, IN_INTERRUPT]

    // take three  cycles for the CSR to be updated
    // using two defer cycles
    nop

    br[end#], defer[2]
    nop
    nop

scratch_ring_full#:
    //  Drop the packet. Two cases to handle. Single buffer or a bufffer chain.
    //  It is determined by looking at the dl_eop_buf_handle

    alu[--, dl_eop_buf_handle, -, IX_NULL]                  ; if dl_eop_buf_handle == IX_NULL
    beq[drop_single_buffer_1#]                              ; then it is single buffer.

    dl_drop_buffer_chain[dl_buf_handle, dl_eop_buf_handle]  ; otherwise, buffer chain.
    br[end#]

drop_single_buffer_1#:
    dl_drop_buffer[dl_buf_handle]

end#:
    #undef RING_NUM

.end
#endm

/** This macro writes the two longwords of exception and triggers interrupt to activate RM ISR for
 * exception packets.
 *
 * In the current implementation all packets go to queue manager and it is assumed
 * that all packet processing has been completed at the core.
 *
 * @param in_buffer_handle
 * @param in_exception_code
 *
// Constant    : nil
 * @instruction_cnt 4
// Branches    : 0
 */
#macro dl_get_exception_pkt()
    #if (IPV4_TO_QM_SCR_RING > 11)
    #error          "For Ring Number Greater than 11, we cannot use the SCR_Ring#_status input state. Check IPV4_TO_QM_SCR_RING"
    #else
    #define_eval    DL_IPV4_QM_RING_FULL    SCR_Ring/**/IPV4_TO_QM_SCR_RING /**/_status
    #endif


    .begin

        .reg ring, temp
        .reg queue_number
        .reg $excep_ctrl_lw0, $excep_ctrl_lw1, $excep_ctrl_lw2
        .xfer_order $excep_ctrl_lw0, $excep_ctrl_lw1, $excep_ctrl_lw2
        .reg $excep_data_lw0, $excep_data_lw1, $excep_data_lw2
        .xfer_order $excep_data_lw0, $excep_data_lw1, $excep_data_lw2
        .sig scratch_rd_done, meta_read_signal

        // set the ring number

        alu[ring, --, b, EXCEPTION_RING_IN_0, <<2]

        // read one longword from exception ring 1

        scratch[get, $excep_ctrl_lw0, ring, 0, 3], sig_done[scratch_rd_done]

        ctx_arb[scratch_rd_done]

        .if ( $excep_ctrl_lw0 != 0 )

            move(dl_buf_handle, $excep_ctrl_lw0)               ; LW0 from ring is the current buffer handle (containign SOP)

            // allocate 6 read xfer registers for reading in the meta data

            xbuf_alloc($dl_meta, 7, read)

            //read meta data

            dl_meta_load_cache($excep_ctrl_lw0, $dl_meta, meta_read_signal, 1, 6)

            ctx_arb[meta_read_signal]

            //  Move the meta data from xfer registers to GPR.

            dl_meta_init_cache(0, $dl_meta1, $dl_meta2, $dl_meta3, $dl_meta4, $dl_meta5, $dl_meta6, 0)

            // set the eop handle

            move(dl_eop_buf_handle, $excep_ctrl_lw1)

            //  set lw0, lw1 and lw2 accroding to themsg format we want.
            //  OC48 msg format is different from OC192.

            #ifdef NEW_MSG_FORMAT
                .reg    packet_size
                .reg    sop_buf_size
                .reg    mop_eop_buf_size

                move($excep_data_lw1, dl_buf_handle)
                move($excep_data_lw2, dl_eop_buf_handle)
                dl_meta_get_fabric_port(queue_number)              ; LW0 on the ring is Queue number.

                ; The 2 LSB bits will be used to indicate DSCP bits for Diffserv
                ; in the future
                alu[queue_number, --, B, queue_number, <<2]

                ; Compute number of bytes in the packet outside the sop buffer.
                ; This value is passed over the outgoing scratch ring and is
                ; used by the statistics ME to compute the number of cells in
                ; all mop buffers of the packet.

                dl_meta_get_buffer_size(sop_buf_size)
                dl_meta_get_packet_size(packet_size)
                alu[mop_eop_buf_size, packet_size, -, sop_buf_size]

                alu[$excep_data_lw0, queue_number, or, mop_eop_buf_size, <<16]

            #else
                //Write three long words on scratch ring

                move($excep_data_lw0, dl_buf_handle)
                move($excep_data_lw1, dl_eop_buf_handle)
                dl_meta_get_fabric_port(queue_number)              ; LW2 on the ring is Queue number for Queue Manager
                alu_shf[queue_number, --, B, queue_number, <<4]        ; fabric * 16 is the actual queue number
                alu_shf[$excep_data_lw2, queue_number, OR, 1, <<31]        ; The msb is always set to 1, to prevent producing a value of 0 on to the ring.
            #endif

            alu_shf[ring, --, b, IPV4_TO_QM_SCR_RING, <<2]

            // check for ring full condition. if full then keep trying

        check_ring#:
            br_inp_state[DL_IPV4_QM_RING_FULL, ring_full#]

            scratch[put, $excep_data_lw0, 0, ring, 3], sig_done[scratch_rd_done]
            ctx_arb[scratch_rd_done]

            br[end#]

            // if the ring is full go back and retry to write to scratch ring
        ring_full#:
            br[check_ring#]

        end#:

            xbuf_free($dl_meta)
        .endif


    .end
    read_again#:

    #undef  DL_IPV4_QM_RING_FULL    // cleanup namespace
#endm


#endif      // DISABLE_PR5_REWORK


/** @}
 * @}
 */

#endif /* __DL_MISC_UC__ */
