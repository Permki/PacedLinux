/* Copyright (C) 2016,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __PKT_CLONE_H__
#define __PKT_CLONE_H__

#include <nfp.h>
#include <nfp_chipres.h>

/* add include for PKT_CTM_SIZE enum */
#include <pkt/pkt.h>

struct pkt_clone_destination
{
    uint32_t            ctm_pkt_num;
    uint64_t            mu_ptr;
} pkt_clone_destination;


/** pkt_clone
 *
 * Allocate a CTM packet and allocate a buffer from blm.
 * Copy the source CTM packet (using packet number) to the destination CTM packet.
 * Copy the source MU buffer to the destination MU buffer.
 *
 * @param source_ctm_pkt_num            Source CTM packet number
 * @param size                          PKT_CTM_SIZE
 * @param source_mu_ptr                 Source MU pointer
 * @param blq                           BLM queue to use
 * @param pkt_off                       Packet offset to copy from
 * @param pkt_len                       Total packet length in bytes including PKT_MAC_PREPEND_BYTES and PKT_NBI_OFFSET
 *
 */
struct pkt_clone_destination pkt_clone(uint32_t source_ctm_pkt_num, enum PKT_CTM_SIZE ctm_size, __mem __addr40 uint32_t *source_mu_ptr,
                                       uint32_t blq, uint32_t pkt_off, uint32_t pkt_len, uint32_t packet_split);


/** pkt_info_save_global_state
 *
 * Save pif_pkt_info_global from NN registers to CLS
 *
 * @param cls_ptr            Pointer to where pif_pkt_info should be saved.
 */
void save_pkt_info(__cls void *cls_ptr);



/** pkt_info_retrieve_global_state
 *
 * Retrieve pif_pkt_info_global from CLS to NN registers
 *
 * @param cls_ptr            Pointer to where pif_pkt_info is saved.
 */
void retrieve_pkt_info(__cls void *cls_ptr);


/** parrep_save
 *
 * Save parrep from local memory to CLS
 *
 * @param parrep             Local memory to copy from.
 * @param cls_ptr            Pointer (16 byte aligned) in CLS where parsed info is saved to.
 * @param word_length        Length of parsed header in 32-bit words.
 */
void save_parrep(__lmem uint32_t *parrep, __cls uint32_t *cls_ptr, uint32_t word_length);


/** parrep_retrieve
 *
 * Retrieve parrep from CLS in local memory
 *
 * @param parrep             Local memory to save to.
 * @param cls_ptr            Pointer (16-byte aligned) in CLS where parsed info is retrieved from.
 * @param word_length        Length of parsed header in 32-bit words.
 */
void retrieve_parrep(__lmem uint32_t *parrep, __cls uint32_t *cls_ptr, uint32_t word_length);



#endif
