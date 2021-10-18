/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved. */

#include <stdint.h>
#include <nfp.h>
#include <app_counters.h>

#include <nfp/me.h>
#include <nfp/mem_bulk.h>
#include <nfp/mem_ring.h>
#include <nfp/mem_atomic.h>
#include <nfp6000/nfp_me.h>

#ifndef PIF_GLOBAL_NFD_DISABLED
#include <vnic/shared/nfd_cfg.h>
#include <vnic/pci_in.h>
#include <vnic/pci_out.h>
#endif /* PIF_GLOBAL_NFD_DISABLED */

#ifdef PIF_USE_GRO
#include <gro_cli.h>
#endif

#include <system_init_c.h>
#include <pkt_meta.h>

/***************************************************************************
 * Globals                                                                 *
 ***************************************************************************/

PIF_PKT_INFO_TYPE struct pif_pkt_info pif_pkt_info_global;

volatile __export __emem char pif_version[64] = PIF_VERSION;

/* make sure only half the CTM is used for packet data */
/* TODO move the code below */
__asm .init_csr xpb:CTMXpbMap.MuPacketReg.MUPEMemConfig 1
__asm .alloc_mem PKT_RSVD ctm+0x0 island 0x20000 reserved


/* XXX: parrep is a hack so the pif runtime goodies can find the app MEs */
__volatile __lmem uint32_t parrep[1] = {0};

/* entry point for managedc function */
extern int mc_main(void);

/* optional initialization calls for managedc */
#ifdef MC_INIT
extern int mc_init(void);
extern int mc_init_master(void);
#endif

/***************************************************************************
 * Helpers                                                                 *
 ***************************************************************************/

#define PACKET_ERROR_SUCCESS   0
#define PACKET_ERROR_PKTMETA   1
#define PACKET_ERROR_PARREP    2
#define PACKET_ERROR_FLOWCACHE 3
#define PACKET_ERROR_INGRESS   4
#define PACKET_ERROR_EGRESS    5
#define PACKET_ERROR_DEPARSE   6
void handle_error_packet(int reason)
{
    __nnr struct pif_pkt_info *pkt_state = &pif_pkt_info_global;

#if 0
    local_csr_write(local_csr_mailbox0, 0xffff0000 | reason);
#ifdef DEBUG
    /*
     * Break the simulation here
     */
    __asm ctx_arb[bpt]
#endif
#endif

    pktio_tx_drop(); // drop packet
    return;
}

/***************************************************************************
 * Main                                                                    *
 ***************************************************************************/

void main(void)
{
    __volatile __shared __gpr unsigned int ready_ctxs;

    if (ctx() == 0) {
        ready_ctxs = __nctx_mode();

        pktio_rx_init();
        pktio_tx_init();

#ifdef MC_INIT
        mc_init();
        if (__ME() == MC_MASTER_ME)
            mc_init_master();
#endif
    }
    system_init_wait_done();

    while (1) {
        unsigned int mb0;
        mb0 = local_csr_read(local_csr_mailbox0);
        if (mb0 != 0x0) {
            ready_ctxs--;
#if (PIF_APP_NUM_NBI_CTX > 0)
            if (ctx() == 0)
                break;
#endif
#if (PIF_APP_NUM_PCIE_CTX > 0)
            if (ctx() == PIF_APP_NUM_NBI_CTX)
                break;
#endif
            /* mailbox0 == 2 means use at most a single thread for packet
             * from NBI and/or PCIe/NFD. */
            if (mb0 == 2)
                __asm ctx_arb[kill];

            break;
        }
        __asm ctx_arb[voluntary]
    }

    while (ready_ctxs)
        __asm ctx_arb[voluntary];

    parrep[0] = local_csr_read(local_csr_mailbox0);
    local_csr_write(local_csr_mailbox0, 0);

    while (1) {
        if (ctx() < PIF_APP_NUM_NBI_CTX) {
            if (pktio_rx_wire() != 0) {
                handle_error_packet(PACKET_ERROR_PKTMETA);
                PIF_COUNT(ERROR_PKTMETA);
                continue;
            }
            PIF_COUNT(RX);
        }
#ifndef PIF_GLOBAL_NFD_DISABLED
        else {
            if (pktio_rx_host() != 0) {
                handle_error_packet(PACKET_ERROR_PKTMETA);
                PIF_COUNT(ERROR_NFDMETA);
                continue;
            }
            PIF_COUNT(RX_NFD);
        }
#endif
        if (mc_main() != 0) {
            pktio_tx_drop();
            PIF_COUNT(DROP);
            continue;
        }

        {
            int ret_code = pktio_tx();

            if (PKT_PORT_TYPE_of(pif_pkt_info_global.p_dst) == PKT_PTYPE_WIRE) {
                if (ret_code == 0) {
                    PIF_COUNT(TX);
                } else if (ret_code == -1) {
                    PIF_COUNT(DROP_PORT_PAUSED);
                } else {
                    PIF_COUNT(DROP_INVALID_PKT_MOD_SCRIPT);
                }
            }
#ifndef PIF_GLOBAL_NFD_DISABLED
            else {
                if (ret_code == 0) {
                    PIF_COUNT(TX_NFD);
                } else {
                    PIF_COUNT(DROP_NFD_NO_CREDITS);
                }
            }
#endif
        }
    }
}
