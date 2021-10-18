/* Copyright (C) 2016,  Netronome Systems, Inc.  All rights reserved. */

#include <nfp.h>
#include <nfp_chipres.h>
#include <nfp/me.h>
#include <nfp/mem_bulk.h>
#include <nfp/cls.h>

#include <nfp6000/nfp_me.h>

#include <std/reg_utils.h>

#ifndef PIF_GLOBAL_NFD_DISABLED
#include <vnic/pci_out.h>
#include <vnic/utils/ordering.h>
#endif /* PIF_GLOBAL_NFD_DISABLED */

#include <blm/blm.h>

#include <pif_pkt.h>
#include <pif_common.h>
#include <pif_counters.h>

#include "config.h"
#include "pkt_clone.h"
#include "mcast.h"

#define CTM_ALLOC_ERR 0xffffffff


#ifdef DMA_CLONE
#include <assert.h>
#include <nfp.h>
#include <types.h>

#include <pktdma/pktdma.h>
#include <nfp6000/nfp_me.h>
#endif

/***************************************************************************
 * Packet clone                                                            *
 ***************************************************************************/

__import __shared __cls struct ctm_pkt_credits ctm_credits;  // configured in pkt_ops.c

#define PIF_8_BIT_XFR_LW     8
#define PIF_8_BIT_XFR_BYTES  ((PIF_8_BIT_XFR_LW) * 4)

#define PIF_32_BIT_XFR_LW     16
#define PIF_32_BIT_XFR_BYTES  ((PIF_32_BIT_XFR_LW) * 4)


#ifdef DMA_CLONE


__intrinsic void
mem_pe_dma_cmt_to_ctm(__add40 __ctm void *ctm_src_addr, __add40 __ctm void *ctm_dest_addr, size_t size, sync_t sync, SIGNAL *sig)
{
    struct nfp_mecsr_prev_alu alu_ind;
    struct nfp_mecsr_cmd_indirect_ref_0 csr_ind;
    unsigned int count = (size >> 6);
    uint32_t dest_addr = (uint32_t)ctm_dest_addr &  0x3ffff;

    ctassert(__is_ct_const(sync));
    ctassert(sync == sig_done || sync == ctx_swap);

    /* Set up PrevAlu for the indirect */
    alu_ind.__raw = 0;
    alu_ind.ov_len = 1;
    alu_ind.ove_data = 2;
    alu_ind.ov_bm_csr = 1;
    alu_ind.length = count - 1;
    alu_ind.data16 = (unsigned int)ctm_src_addr & 0x3ffff;

    /* Set up CSR for the indirect */
    csr_ind.__raw = 0;
    csr_ind.byte_mask = (0x02 << 6) | (__ISLAND & 0x3f);

    /* addr_hi is the CTM island ID (local).  addr_lo is the MU lo addr */
    if (sync == sig_done) {
        local_csr_write(local_csr_cmd_indirect_ref_0, csr_ind.__raw);
        __asm { alu[--, --, B, alu_ind.__raw] }
        __asm { mem[pe_dma_to_memory_buffer, --, dest_addr, 0, <<8, 1], sig_done[*sig], indirect_ref }
    } else {
        local_csr_write(local_csr_cmd_indirect_ref_0, csr_ind.__raw);
        __asm { alu[--, --, B, alu_ind.__raw] }
        __asm { mem[pe_dma_to_memory_buffer, --, dest_addr, 0, <<8, 1], ctx_swap[*sig], indirect_ref }
    }
}

#else

/*
 * Copy source CTM packet in 8 X 32-bit chunks to destination CTM packet.
 * Copy the length of the CTM packet minus the PKT_NBI_OFFSET and NBI_PKT_PREPEND_BYTES.
 */
static void copy_ctm_packet(uint32_t source_ctm_pkt_num, uint32_t dest_ctm_pkt_num, uint32_t ctm_src_off, uint32_t ctm_src_byte_len)
{
    __mem __addr40 uint32_t     *source_ctm_ptr;
    __mem __addr40 uint32_t     *dest_ctm_ptr;
    __gpr uint32_t              chunk;
    __gpr uint32_t              i;
    SIGNAL                      sig;

    /* Copy 32-bit words whole words for bulk of packet */
    {
        __xread uint32_t        rd_buffer[PIF_32_BIT_XFR_LW];
        __xwrite uint32_t       wr_buffer[PIF_32_BIT_XFR_LW];
        __gpr uint32_t          to_copy_words = (ctm_src_byte_len/4);

        chunk = PIF_32_BIT_XFR_LW;
        while (to_copy_words)
        {
            if (to_copy_words < PIF_32_BIT_XFR_LW)
                chunk = to_copy_words;

            source_ctm_ptr = pkt_ctm_ptr40(0, source_ctm_pkt_num, ctm_src_off);
            dest_ctm_ptr = pkt_ctm_ptr40(0, dest_ctm_pkt_num, ctm_src_off);

            __mem_read32(rd_buffer, source_ctm_ptr, chunk << 2, PIF_32_BIT_XFR_BYTES, ctx_swap, &sig);
            reg_cp(wr_buffer, rd_buffer, sizeof(rd_buffer));
            __mem_write32(wr_buffer, dest_ctm_ptr, chunk << 2, PIF_32_BIT_XFR_BYTES, ctx_swap, &sig);

            to_copy_words -= chunk;
            ctm_src_off += (chunk << 2);
        }
    }

    /* Copy 8-bits for remainder of packet */
    {
        __xread uint32_t        rd_buffer[PIF_8_BIT_XFR_LW];
        __xwrite uint32_t       wr_buffer[PIF_8_BIT_XFR_LW];
        __gpr uint32_t          to_copy_bytes = (ctm_src_byte_len - ((ctm_src_byte_len/4) * 4));

        chunk = PIF_8_BIT_XFR_BYTES;

        while (to_copy_bytes)
        {
            if (to_copy_bytes < PIF_8_BIT_XFR_BYTES)
                chunk = to_copy_bytes;

            source_ctm_ptr = pkt_ctm_ptr40(0, source_ctm_pkt_num, ctm_src_off);
            dest_ctm_ptr = pkt_ctm_ptr40(0, dest_ctm_pkt_num, ctm_src_off);

            __mem_read8(rd_buffer, source_ctm_ptr, chunk, PIF_8_BIT_XFR_BYTES, ctx_swap, &sig);
            reg_cp(wr_buffer, rd_buffer, sizeof(rd_buffer));
            __mem_write8(wr_buffer, dest_ctm_ptr, chunk, PIF_8_BIT_XFR_BYTES, ctx_swap, &sig);

            to_copy_bytes -= chunk;
            ctm_src_off += chunk;
        }
    }

    return;
}


/*
 * Copy MU buffer packet in 64-bit chunks for byte_length.
 */
static void copy_mu_buffer(__mem __addr40 uint32_t *source_mu_ptr, __mem __addr40 uint32_t *dest_mu_ptr, uint32_t byte_length)
{
    __mem __addr40 uint32_t     *source_ptr;
    __mem __addr40 uint32_t     *dest_ptr;
    SIGNAL                      sig;
    __gpr uint32_t              chunk;
    __gpr uint32_t              offset = 0;

    /* Copy 32-bit words whole words for bulk of packet */
    {
        __xread uint32_t        rd_buffer[PIF_32_BIT_XFR_LW];
        __xwrite uint32_t       wr_buffer[PIF_32_BIT_XFR_LW];
        __gpr uint32_t          to_copy_words = (byte_length/4);

        chunk = PIF_32_BIT_XFR_LW;
        while (to_copy_words)
        {
            if (to_copy_words < PIF_32_BIT_XFR_LW)
                chunk = to_copy_words;

            source_ptr = (__mem __addr40 uint32_t *)((uint64_t)source_mu_ptr + offset);
            dest_ptr = (__mem __addr40 uint32_t *)((uint64_t)dest_mu_ptr + offset);

            __mem_read32(rd_buffer, source_ptr, chunk << 2, PIF_32_BIT_XFR_BYTES, ctx_swap, &sig);
            reg_cp(wr_buffer, rd_buffer, sizeof(rd_buffer));
            __mem_write32(wr_buffer, dest_ptr, chunk << 2, PIF_32_BIT_XFR_BYTES, ctx_swap, &sig);

            to_copy_words -= chunk;
            offset += (chunk << 2);
        }
    }

    /* Copy 8-bits for remainder of packet */
    {
        __xread uint32_t        rd_buffer[PIF_8_BIT_XFR_LW];
        __xwrite uint32_t       wr_buffer[PIF_8_BIT_XFR_LW];
        __gpr uint32_t          to_copy_bytes = (byte_length - ((byte_length/4) * 4));

        chunk = PIF_8_BIT_XFR_BYTES;
        while (to_copy_bytes)
        {
            if (to_copy_bytes < PIF_8_BIT_XFR_BYTES)
                chunk = to_copy_bytes;

            source_ptr = (__mem __addr40 uint32_t *)((uint64_t)source_mu_ptr + offset);
            dest_ptr = (__mem __addr40 uint32_t *)((uint64_t)dest_mu_ptr + offset);

            __mem_read8(rd_buffer, source_ptr, chunk, PIF_8_BIT_XFR_BYTES, ctx_swap, &sig);
            reg_cp(wr_buffer, rd_buffer, sizeof(rd_buffer));
            __mem_write8(wr_buffer, dest_ptr, chunk, PIF_8_BIT_XFR_BYTES, ctx_swap, &sig);

            to_copy_bytes -= chunk;
            offset += chunk;
        }
    }

    return;
}


#endif

struct pkt_clone_destination pkt_clone(uint32_t source_ctm_pkt_num, enum PKT_CTM_SIZE ctm_size, __mem __addr40 uint32_t *source_mu_ptr,
                                            uint32_t blq, uint32_t pkt_off, uint32_t pkt_len, uint32_t packet_split)
{

    struct pkt_clone_destination destination;

    /*
    * Poll for a CTM packet until one is returned.
    * Set alloc_internal to acquire ctm packet and buffer credits, before allocating CTM buffer.
    */

    destination.ctm_pkt_num = pkt_ctm_alloc(&ctm_credits, 0, ctm_size, 1, 0);

    while (destination.ctm_pkt_num == CTM_ALLOC_ERR)
    {
        sleep(BACKOFF_SLEEP);
        PIF_COUNT(ERROR_WAIT_CTM_PKT_ALLOC);
        destination.ctm_pkt_num = pkt_ctm_alloc(&ctm_credits, 0, ctm_size, 1, 0);
    }

    /*
     * Poll for MU buffer until one is returned.
     */
    {
        __xread blm_buf_handle_t    buf;

        while (blm_buf_alloc(&buf, blq) != 0)
        {
            sleep(BACKOFF_SLEEP);
            PIF_COUNT(ERROR_WAIT_BLM_BUF_ALLOC);
        }

        destination.mu_ptr = (uint64_t) blm_buf_handle2ptr(buf);
    }

#ifdef DMA_CLONE
    /*
     * Depending on packet length, copy only CTM portion or copy full CTM buffer and portion residing in MU buffer
     */
    {
        __mem __addr40 uint8_t  *dest_ctm_ptr, *dest_mu_ptr, *src_dma_mu_ptr;
        uint32_t                ctm_buffer_size = (256 << ctm_size);        // maximum CTM size
        int32_t                 mu_bytes_to_copy;
        uint32_t                xfer_len_bytes, mu_offset = 0;
        uint32_t                ctm_bytes_to_copy = ((pkt_len + pkt_off) + (64-1)) & (~(64-1));
        __xread pkt_status_t    pkt_status_dest;

        pkt_status_read(__ISLAND, destination.ctm_pkt_num, &pkt_status_dest);
        dest_ctm_ptr = pkt_status_dest.ctm_addr_div256 * 256;

        if (packet_split) {
            /*
            * Copy from existing MU buffer to new CTM
            * then copy from new CTM to new MU buffer
            */
            mu_bytes_to_copy = pkt_len - (ctm_buffer_size - pkt_off);
            dest_mu_ptr = (__mem __addr40 uint8_t *)destination.mu_ptr;
            mu_offset = ctm_buffer_size;
            src_dma_mu_ptr = (__mem __addr40 uint8_t *) source_mu_ptr;

            while (mu_bytes_to_copy > 0) {

                if (mu_bytes_to_copy > ctm_buffer_size)
                    xfer_len_bytes = ctm_buffer_size;
                else {
                    xfer_len_bytes = mu_bytes_to_copy;
                    xfer_len_bytes = (xfer_len_bytes + (64-1)) & (~(64-1));
                }

                pktdma_mu_to_ctm(dest_ctm_ptr, src_dma_mu_ptr + mu_offset, xfer_len_bytes);
                pktdma_ctm_to_mu(dest_mu_ptr + mu_offset, dest_ctm_ptr, xfer_len_bytes);

                mu_bytes_to_copy -= xfer_len_bytes;
                mu_offset += xfer_len_bytes;
            }

            ctm_bytes_to_copy = ctm_buffer_size;
        }

        /*
         * Copy CTM section from existing to new
         */
        {
            __mem __addr40 uint32_t     *source_ctm_ptr;
            __xread pkt_status_t        pkt_status_src;
             SIGNAL                      sig1;

            pkt_status_read(__ISLAND, source_ctm_pkt_num, &pkt_status_src);
            source_ctm_ptr = pkt_status_src.ctm_addr_div256 * 256;
            mem_pe_dma_cmt_to_ctm(source_ctm_ptr, dest_ctm_ptr, ctm_bytes_to_copy, sig_done, &sig1);
            wait_for_all(&sig1);

            return destination;
        }
    }
#else
    /*
     * Depending on packet length, copy only CTM portion or copy full CTM buffer and portion residing in MU buffer
     */
    {
        __gpr uint32_t  ctm_buffer_size = (256 << ctm_size);        // maximum CTM size

        if (!packet_split)
        {
            /*
             * CTM buffer size is bigger than packet size, only copy the CTM buffer
             * and no need to copy MU buffer as there is nothing there.
             */
            copy_ctm_packet(source_ctm_pkt_num, destination.ctm_pkt_num, pkt_off, pkt_len);

            return destination;
        }
        else
        {
             __gpr uint32_t  mu_len = 0;

            /*
             * Packet length is bigger than CTM buffer size, copy full CTM buffer size - pkt_off
             * and then copy rest of packet into MU buffer. Start of data in MU buffer is size of CTM buffer.
             */
            mu_len = pkt_len - (ctm_buffer_size - pkt_off);

            copy_mu_buffer(
                (__mem __addr40 uint32_t *)((uint64_t)source_mu_ptr + ctm_buffer_size),
                (__mem __addr40 uint32_t *)((uint64_t)destination.mu_ptr + ctm_buffer_size),
                mu_len
            );
            copy_ctm_packet(source_ctm_pkt_num, destination.ctm_pkt_num, pkt_off, ctm_buffer_size - pkt_off);

            return destination;
        }
    }
#endif
    return destination;
}



/***************************************************************************
 * Packet info state                                                       *
 ***************************************************************************/

extern __forceinline void save_pkt_info(__cls void *cls_ptr)
{
    __xwrite struct pif_pkt_combined wr_pkt_info;

    reg_cp(&wr_pkt_info.pkt, (void *)&pif_pkt_info_global, sizeof(pif_pkt_info_global));
    reg_cp(&wr_pkt_info.specific, (void *)&pif_pkt_info_spec, sizeof(pif_pkt_info_spec));
    cls_write((__xwrite void *)&wr_pkt_info, (__cls void *)cls_ptr, sizeof(wr_pkt_info));
    return;
}

extern __forceinline void retrieve_pkt_info(__cls void *cls_ptr)
{
    __xread struct pif_pkt_combined rd_pkt_info;

    cls_read((__xread void *)&rd_pkt_info, (__cls void *)cls_ptr, sizeof(rd_pkt_info));

    reg_cp((void *)&pif_pkt_info_global, &rd_pkt_info.pkt, sizeof(pif_pkt_info_global));
    reg_cp((void *)&pif_pkt_info_spec, &rd_pkt_info.specific, sizeof(pif_pkt_info_spec));
    return;
}


/***************************************************************************
 * Parsed header                                                           *
 ***************************************************************************/

extern __forceinline void save_parrep(__lmem uint32_t *parrep, __cls uint32_t *cls_ptr, uint32_t word_length)
{
    __xwrite uint32_t   wr_buffer[4];
    __gpr uint32_t     i;

    /*
     * Copy in chunks of 4 32-bit words
     */
    for (i = 0; i < (word_length >> 2); i++)
    {
        reg_cp(wr_buffer,(void *)parrep, sizeof(wr_buffer));
        cls_write((__xwrite void *)wr_buffer, (__cls void *)cls_ptr, sizeof(wr_buffer));
        cls_ptr += 4;
        parrep += 4;
    }

    /*
     * Copy one 32-bit word at a time for the rest of the word_length
     */
    for (i = 0; i < (word_length - ((word_length >> 2) << 2)); i++)
    {
        reg_cp(wr_buffer,(void *)parrep, 1 << 2);
        cls_write((__xwrite void *)wr_buffer, (__cls void *)cls_ptr, 1 << 2);
        cls_ptr++;
        parrep++;
    }

    return;
}

extern __forceinline void retrieve_parrep(__lmem uint32_t *parrep, __cls uint32_t *cls_ptr, uint32_t word_length)
{
    __xread uint32_t   rd_buffer[4];
    __gpr uint32_t     i;

    /*
     * Copy in chunks of 4 32-bit words
     */
    for (i = 0; i < (word_length >> 2); i++)
    {
        cls_read((__xread void *)rd_buffer, (__cls void *)cls_ptr, sizeof(rd_buffer));
        reg_cp((void *)parrep, rd_buffer, sizeof(rd_buffer));
        cls_ptr += 4;
        parrep += 4;
    }

    /*
     * Copy one 32-bit word at a time for the rest of the word_length
     */
    for (i = 0; i < (word_length - ((word_length >> 2) << 2)); i++)
    {
        cls_read((__xread void *)rd_buffer, (__cls void *)cls_ptr, 1 << 2);
        reg_cp((void *)parrep, rd_buffer, 1 << 2);
        cls_ptr++;
        parrep++;
    }

    return;
}



