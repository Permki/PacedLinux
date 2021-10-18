/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved. */

#include <stdint.h>
#include <nfp.h>
#include <pif_common.h>
#include <pif_counters.h>

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

#include <blm/blm.h>

#ifdef PIF_USE_GRO
#include <gro_cli.h>
#endif

#ifndef PIF_GLOBAL_FLOWCACHE_DISABLED
#include <flow_cache_c.h>
#include <flow_cache_lock_c.h>
#include <fc.h>
#endif /* PIF_GLOBAL_FLOWCACHE_DISABLED */

#include <hashmap.h>

#include <pif_pkt_duplication.h>
#include <macflush/macflushuser.h>

#include "port_config.h"
#include "system_init_c.h"
#include "pkt_clone.h"
#include "mcast.h"

#ifdef  CFLOW_TSTAMP
#include "cflow_tstamp_user.h"
#include "cflow_tstamp.h"
#endif

/***************************************************************************
 * Defines                                                                 *
 ***************************************************************************/

struct fc_control {
    union {
        struct {
            int16_t ingress_len;
            int16_t egress_len;
        };
        uint32_t _raw[0];
    };
};

/* duplicated pktio */
struct pktio_handle {
    union {
        struct {
            uint8_t ph_isl;
            uint8_t pad;
            uint16_t ph_pnum;
        };
        uint32_t __raw;
    };
};

/* special encoding for control lengths */
#define PIF_FC_CACHE_INITIAL -1
#define PIF_FC_CACHE_BYPASS  -2

/* always 1LW of control info */
#define PIF_FC_PAYLOAD_OFF 1


/* the BLM EMU ring to use (0 - 3) for MU buffer allocation  */
#define BLM_BLQ_0         0

/* drop packet */
#define PIF_EGRESS_DROP  0xffff

#ifdef FC_PAYLOAD_SIZE
#define ACTION_BUFFER_SIZE FC_PAYLOAD_SIZE
#else
#define ACTION_BUFFER_SIZE 256
#endif


#ifdef PIF_PLUGIN_INIT
void pif_plugin_init_master();
void pif_plugin_init();
#endif

volatile __export __emem uint32_t pif_qos_supported = 1;

/***************************************************************************
 * Globals                                                                 *
 ***************************************************************************/

volatile __export __emem char pif_version[64] = PIF_VERSION;

#ifdef DEBUG
volatile __export __emem uint32_t pif_debug_enabled = 1;
#endif

/*
 * Packet metadata
 */
PIF_PKT_INFO_TYPE struct pif_pkt_info   pif_pkt_info_global;    // global pkt metadata
__cls struct pif_pkt_combined           pif_pkt_info_copy;      // copy of metadata in CLS used by cloning/mcast

/*
 * Parsed header
 */
volatile __lmem uint32_t                parrep[PIF_PARREP_LEN_LW + PIF_DEBUG_EXTRA_PARREP] = {0};
__cls  uint32_t                         parrep_copy[PIF_PARREP_LEN_LW];

/*
 * Flow cache key
 */
#ifndef PIF_GLOBAL_FLOWCACHE_DISABLED
__lmem uint32_t                         flowkey[FC_MAX_KEY_LEN_LW];
#else
__lmem uint32_t                         flowkey[11];
#endif /* PIF_GLOBAL_FLOWCACHE_DISABLED */

__nnr uint32_t pif_flowcache_bypass;

/*
 * Miscellaneous
 */
__nnr struct pif_pkt_duplication_type   pkt_dup_state; // current packet to be cloned/mcast
__nnr struct pif_pkt_specific           pif_pkt_info_spec;

#ifndef PIF_GLOBAL_TRAFFIC_CLASS_DISABLED
#define TRAFFIC_CLASS_PER_PORT 64
/* TODO: this can be shrunk */
volatile __export __mem __addr40 uint32_t pif_traffic_class_map[TRAFFIC_CLASS_PER_PORT * 64 * 2];
#endif

/* make sure only half the CTM is used for packet data */
/* TODO move the code below */
__asm .init_csr xpb:CTMXpbMap.MuPacketReg.MUPEMemConfig 1
__asm .alloc_mem PKT_RSVD ctm+0x0 island 0x20000 reserved

PORT_CONFIG_DECLARE;

#if PIF_APP_REDUCED_THREADS == 1 && !defined(PIF_GLOBAL_NFD_DISABLED) && !defined(NO_MULTI_RX)
#define MULTI_RX

/***************************************************************************
 * 4 CTX multi source receive                                              *
 ***************************************************************************/

__nnr int multi_rx_ctm_pkt_no;
SIGNAL multi_rx_nfd_sig;
SIGNAL multi_rx_nbi_sig;

__xread struct nfd_in_pkt_desc multi_rx_nfd_rxd;
__xread struct pktio_nbi_meta multi_rx_nbi_rxd;

#define MULTI_RX_STATE_NFD_PENDING (1 << 31)
#define MULTI_RX_STATE_NBI_PENDING (1 << 30)

/* this bit is chosen to not clash with MULTI_RX_SIG_* */
#define MULTI_RX_STATE_LAST_BIT (1 << 2)
#define MULTI_RX_STATE_LAST_NBI (1 << 2)
#define MULTI_RX_STATE_LAST_NFD (0 << 2)

#define MULTI_RX_SIG_NFD (1 << 0)
#define MULTI_RX_SIG_NBI (1 << 1)
#define MULTI_RX_SIG_BOTH (MULTI_RX_SIG_NFD | MULTI_RX_SIG_NBI)

__gpr unsigned int multi_rx_state = 0;

__intrinsic int multi_rx_wait_signals(SIGNAL *sig_nfd,
                                      SIGNAL *sig_nbi,
                                      unsigned int state)
{
    int ret;

    while (1) {
        if ((state & MULTI_RX_STATE_LAST_BIT) == MULTI_RX_STATE_LAST_NBI) {
            if (signal_test(sig_nfd)) {
                ret = MULTI_RX_SIG_NFD;
                break;
            }
            if (signal_test(sig_nbi)) {
                ret = MULTI_RX_SIG_NBI;
                break;
            }
        } else {
            if (signal_test(sig_nbi)) {
                ret = MULTI_RX_SIG_NBI;
                break;
            }
            if (signal_test(sig_nfd)) {
                ret = MULTI_RX_SIG_NFD;
                break;
            }
        }
        wait_for_any(sig_nfd, sig_nbi);
    }
    return ret;
}

#endif /* PIF_APP_REDUCED_THREADS == 1 && !defined(PIF_GLOBAL_NFD_DISABLED) */

/***************************************************************************
 * Forwarding                                                              *
 ***************************************************************************/

/* The #NBI and #port are encoded in P4 standard_metadata.egress_port */
__forceinline int forward(__lmem uint32_t *parrep)
{
    SIGNAL sig_mac_paused;
    uint64_t port_bit;
    __xread uint64_t xfr_port_bitmap;
#ifndef PIF_GLOBAL_TRAFFIC_CLASS_DISABLED
    SIGNAL sig_traffic_class_xlate;
    __xread uint32_t xfr_traffic_class_queue;
    __mem __addr40 uint32_t *tc_map;
    unsigned int channel;
#endif
    unsigned int port_spec;
    unsigned int port, nbi;

    port_spec = pif_global_get_stdmd_egress_port(parrep);

    /* egress_port == -1 : drop */
    if (port_spec == PIF_EGRESS_DROP) {
        PIF_COUNT(DROP_META);
        return 1;
    }

    pif_global_set_pktmd_nic_vlan(parrep);

    if (PIF_PORT_SPEC_TYPE_of(port_spec) == PIF_PORT_SPEC_TYPE_PHYSICAL) {
        nbi = PIF_PKT_PSPEC_NBI_of(port_spec);
        port = PIF_PKT_PSPEC_PORT_of(port_spec);

        port_bit = __mac_is_port_paused(nbi, port, &xfr_port_bitmap, &sig_mac_paused);
#ifndef PIF_GLOBAL_TRAFFIC_CLASS_DISABLED
        channel = PIF_PORT_SPEC_CHAN_of(port_spec);
        tc_map = (__mem __addr40 uint32_t *)pif_traffic_class_map +
                 (nbi * 64 + port) * TRAFFIC_CLASS_PER_PORT
                 + channel;
        __mem_read8(&xfr_traffic_class_queue,
                    tc_map,
                    sizeof(pif_traffic_class_map[0]),
                    sizeof(pif_traffic_class_map[0]),
                    sig_done,
                    &sig_traffic_class_xlate);
        wait_for_all(&sig_mac_paused, &sig_traffic_class_xlate);
        channel = xfr_traffic_class_queue;

        pif_pkt_info_global.p_dst = PKT_SET_PORT(PKT_PTYPE_WIRE,
                PIF_PKT_PSPEC_NBI_of(port_spec),
                PIF_PKT_PSPEC_CHAN_of(port, channel));
#else /* ! PIF_GLOBAL_TRAFFIC_CLASS_DISABLED */
        wait_for_all(&sig_mac_paused);
        pif_pkt_info_global.p_dst = PKT_SET_PORT(PKT_PTYPE_WIRE,
                                                 nbi,
                                                 PIF_PKT_PSPEC_CHAN_of(port, 0));
#endif /* PIF_GLOBAL_TRAFFIC_CLASS_DISABLED */

        if (xfr_port_bitmap & port_bit) { /* paused ! */
            PIF_COUNT(DROP_PORT_PAUSED);
            return 1;
        }

        return 0;
    } else if (PIF_PORT_SPEC_TYPE_of(port_spec) == PIF_PORT_SPEC_TYPE_HOST) {
        unsigned port = PIF_PORT_SPEC_PORT_of(port_spec);
        pif_pkt_info_global.p_dst = PKT_SET_PORT(PKT_PTYPE_HOST,
                                                 PIF_PORT_SPEC_HOST_PORT_ISL_of(port),
                                                 PIF_PORT_SPEC_HOST_PORT_VF_of(port));
        return 0;
#ifndef PIF_GLOBAL_WQ_DISABLED
    } else if (PIF_PORT_SPEC_TYPE_of(port_spec) == PIF_PORT_SPEC_TYPE_LOGICAL) {
        port = PIF_PORT_SPEC_PORT_of(port_spec);
        pif_pkt_info_global.p_dst = PKT_SET_PORT(PKT_PTYPE_WQ,
                                                 0, /* no support for other than emem0 for now */
                                                 PIF_PORT_SPEC_WQ_RINGNUM_of(port));
        return 0;
#endif /* PIF_GLOBAL_WQ_DISABLED */
    } else {

        /* logical port - for future use, drop packet for now */
        /* multicast port - we dont support recursive mcast groups (mcast in mcast)- drop packet */
        pif_global_set_stdmd_egress_port(parrep, PIF_EGRESS_DROP);
        PIF_COUNT(DROP_META);
        return 1;
    }

    return 0;
}


/***************************************************************************
 * Helpers                                                                 *
 ***************************************************************************/

/* load control info into flow cache */
__forceinline void load_fc_control(__mem __addr40 uint32_t *buf, int ig_len, int eg_len)
{
    struct fc_control control_buf;
    __xwrite struct fc_control control;

    control_buf.ingress_len = ig_len;
    control_buf.egress_len = eg_len;
    control = control_buf;

    mem_write32(&control, buf, sizeof(control));
}

#define PACKET_ERROR_SUCCESS   0
#define PACKET_ERROR_PKTMETA   1
#define PACKET_ERROR_PARREP    2
#define PACKET_ERROR_FLOWCACHE 3
#define PACKET_ERROR_INGRESS   4
#define PACKET_ERROR_EGRESS    5
#define PACKET_ERROR_DEPARSE   6
void handle_error_packet(int reason)
{

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
    PIF_COUNT(DROP);
    return;
}

/***************************************************************************
 * Main                                                                    *
 ***************************************************************************/

volatile __export __mem __addr40 uint32_t flowkey_copy[256];

void main(void)
{
    /* action buffer in the case where no flow cache lookup is performed */
    __mem __addr40 uint32_t action_buffer[ACTION_BUFFER_SIZE];
    __volatile __shared __gpr unsigned int ready_ctxs;
    __emem uint32_t *act_buf;

    if (ctx() == 0)
        ready_ctxs = __nctx_mode();

    if (ctx() == 0) {
#ifndef PIF_GLOBAL_FLOWCACHE_DISABLED
        fc_me_init();
#endif /* PIF_GLOBAL_FLOWCACHE_DISABLED */
        camp_hash_init();
        pktio_rx_init();
        pktio_tx_init();
#ifdef PIF_PLUGIN_INIT
        if (__ME() == PIF_APP_MASTER_ME)
            pif_plugin_init_master();
#endif
    }
#ifdef PIF_PLUGIN_INIT
    pif_plugin_init();
#endif

    /* All other contexts need to wait for sys init */
    system_init_wait_done();

    while (1) {
        unsigned int mb0;
        mb0 = local_csr_read(local_csr_mailbox0);
        if (mb0 != 0x0) {
            ready_ctxs--;
#ifdef MULTI_RX
            if (ctx() == 0)
                break;
#else
#if (PIF_APP_NUM_NBI_CTX > 0)
            if (ctx() == 0)
                break;
#endif
#if (PIF_APP_NUM_PCIE_CTX > 0)
            if (ctx() == PIF_APP_NUM_NBI_CTX)
                break;
#endif
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

    local_csr_write(local_csr_mailbox0, 0);

    pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;

    while (1) {
        __gpr unsigned int flowkey_len = 0;
        __gpr int exit_node;
#ifndef PIF_GLOBAL_FLOWCACHE_DISABLED
        __gpr fc_lookup_result_t result;
#endif /* PIF_GLOBAL_FLOWCACHE_DISABLED */
        __gpr int ingress_cached, egress_cached;
        __gpr struct replication_information rep_info;

        pif_flowcache_bypass = 0;

        /*
         * For a multicast packet and clone to egress, jump to egress processing.
         * For a resubmit/recirculate/clone to ingress packet, jump to parsing.
         */

        if (!PIF_DUP_EGRESS_ACTION(pkt_dup_state)) {
            if (pkt_dup_state.__raw == PIF_PKT_INSTANCE_TYPE_NORMAL) {

                __critical_path();
                PIF_DEBUG_SET_STATE(PIF_DEBUG_STATE_WAITING, 0x999);

                /*
                * Receive next packet.
                */
#ifdef MULTI_RX
                {
                    int to_process;
                    int multi_rx_ret;

                    if (!(multi_rx_state & MULTI_RX_STATE_NBI_PENDING)) {
                        pktio_rx_wire_issue(&multi_rx_nbi_rxd,
                                            sizeof(multi_rx_nbi_rxd),
                                            sig_done,
                                            &multi_rx_nbi_sig);
                        multi_rx_state |= MULTI_RX_STATE_NBI_PENDING;
                    }

                    if (!(multi_rx_state & MULTI_RX_STATE_NFD_PENDING)) {
                        multi_rx_ctm_pkt_no = pktio_rx_host_issue(&multi_rx_nfd_rxd,
                                                                  sig_done,
                                                                  &multi_rx_nfd_sig);
                        multi_rx_state |= MULTI_RX_STATE_NFD_PENDING;
                    }


                    to_process = multi_rx_wait_signals(&multi_rx_nfd_sig,
                                                       &multi_rx_nbi_sig,
                                                       multi_rx_state);

                    if (to_process == MULTI_RX_SIG_NFD) {
                        /* clear the NFD pending bit */
                        multi_rx_state &= ~MULTI_RX_STATE_NFD_PENDING;

                        if (pktio_rx_host_process(&multi_rx_nfd_rxd, multi_rx_ctm_pkt_no) < 0) {
                            handle_error_packet(PACKET_ERROR_PKTMETA);
                            PIF_COUNT(ERROR_PKTMETA);
                            continue;
                        }
                        __critical_path();
                        PIF_COUNT(RX_NFD);
                    } else {
                        multi_rx_state &= ~MULTI_RX_STATE_NBI_PENDING;

                        if (pktio_rx_wire_process(&multi_rx_nbi_rxd) < 0) {
                            handle_error_packet(PACKET_ERROR_PKTMETA);
                            PIF_COUNT(ERROR_PKTMETA);
                            continue;
                        }

                        __critical_path();
                        PIF_COUNT(RX);
                    }

                    /* toggle the last used bit, this will mean
                     * we will service the other packet source next time
                     * TODO: check that this is fair enough
                     */
                    multi_rx_state ^= MULTI_RX_STATE_LAST_BIT;
                }
#else /* MULTI_RX */
#ifndef PIF_GLOBAL_NFD_DISABLED
                if (ctx() < PIF_APP_NUM_NBI_CTX) {
#endif
                    if (pktio_rx_wire() != 0) {
                        handle_error_packet(PACKET_ERROR_PKTMETA);
                        PIF_COUNT(ERROR_PKTMETA);
                        continue;
                    }
                    __critical_path();
                    PIF_COUNT(RX);

#ifndef PIF_GLOBAL_NFD_DISABLED

                } else {
                    if (pktio_rx_host() != 0) {
                        handle_error_packet(PACKET_ERROR_PKTMETA);
                        PIF_COUNT(ERROR_NFDMETA);
                        continue;
                    }
                    __critical_path();
                    PIF_COUNT(RX_NFD);
                }
#endif /* ! PIF_GLOBAL_NFD_DISABLED */
#endif /* ! MULTI_RX */

                pif_global_metadata_init((__lmem uint32_t *)parrep);
                rep_info.rid = 0;
            } else {

                /*
                * If resubmit/recirculate/clone ingress of packet; do ingress parsing again.
                */
                pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;          // clear duplication flag
            }
#ifdef CFLOW_TSTAMP
            /* Pkt Received TimeStamp */
            cflow_tstamp_start(pif_pkt_info_global.p_seq);
#endif
            {
                __mem __addr40 uint8_t *pkt_buf = ( __mem __addr40 uint8_t *)pkt_ctm_ptr40(0, pif_pkt_info_global.p_pnum, pif_pkt_info_global.p_offset);

                exit_node = pif_parrep_extract((__lmem uint32_t *)parrep,
                                            pkt_buf,
                                            pif_pkt_info_global.p_len,
                                            flowkey,
                                            (__gpr unsigned int *)&flowkey_len);
            }

            /* Verify calculated fields if required. Returns 0 if verification
             * successful and -PIF_PARSE_ERROR_CHECKSUM otherwise. */
            if (exit_node >= 0) {
                __gpr int flcalc_verify_result;
                flcalc_verify_result = pif_flcalc_verify((__lmem uint32_t *)parrep);
                if (flcalc_verify_result)
                    exit_node = flcalc_verify_result;
            }

            if (exit_node < 0) {
                int err_code = -exit_node;
                pif_global_set_stdmd_parser_status((__lmem uint32_t *)parrep,
                                                   err_code);
                /* handle parser error, run P4 parser exception code */

                PIF_COUNT_RANGE(ERROR_PARREP, err_code);

                exit_node = pif_parrep_exceptions((__lmem uint32_t *)parrep,
                                                  err_code,
                                                  flowkey,
                                                  (__gpr unsigned int *)&flowkey_len);
                if (exit_node < 0) {
                    handle_error_packet(PACKET_ERROR_PARREP);
                    continue;
                }
            }
            __critical_path();

#ifdef CFLOW_TSTAMP
            cflow_tstamp(CFLOW_TSTAMP_USER_PACKET_PARSED,
                         pif_pkt_info_global.p_seq);
#endif
            /* Pipeline with flow key */
#ifndef PIF_GLOBAL_FLOWCACHE_DISABLED
            if (flowkey_len > 0) {
                result = fc_do_lookup(
                            flowkey,
                            flowkey_len,
                            pif_pkt_info_global.p_len,
                            fc_get_version(),
                            30 /* TODO: timeout */ );
                act_buf = (__mem __addr40 uint32_t *)result.payload_address;

                if (result.result_code == FC_LOOKUP_EXACT_MATCH) {
                    __xread struct fc_control control;

                    mem_read32(&control, act_buf, sizeof(control));
                    ingress_cached = control.ingress_len;
                    egress_cached = control.egress_len;
                    PIF_COUNT(FLOWCACHE_HIT);

                    if (ingress_cached == PIF_FC_CACHE_BYPASS) {
                        /* if ingress is not cacheable, use the local action buffer
                        * and release the lock */
                        act_buf = action_buffer;
                        fc_do_release(result.lock);
                    }
#ifdef CFLOW_TSTAMP
                    cflow_tstamp(CFLOW_TSTAMP_USER_FLOWCACHE_HIT,
                                 pif_pkt_info_global.p_seq);
#endif
                } else if (result.result_code == FC_LOOKUP_ENTRY_ADDED) {
                    ingress_cached = PIF_FC_CACHE_INITIAL;
                    egress_cached = PIF_FC_CACHE_INITIAL;
                    PIF_COUNT(FLOWCACHE_MISS);
#ifdef CFLOW_TSTAMP
                    cflow_tstamp(CFLOW_TSTAMP_USER_FLOWCACHE_MISS,
                                 pif_pkt_info_global.p_seq);
#endif
                } else {
                    /* unexpected flow cache response */
                    handle_error_packet(PACKET_ERROR_FLOWCACHE);
                    PIF_COUNT(ERROR_FLOWCACHE);
                    continue;
                }
            } else {
                /* flowkey_len == 0 indicates no lookup */
                act_buf = action_buffer;
                ingress_cached = PIF_FC_CACHE_BYPASS;
                egress_cached = PIF_FC_CACHE_BYPASS;
                PIF_COUNT(FLOWCACHE_BYPASS);
            }
#else
            act_buf = action_buffer;
            ingress_cached = PIF_FC_CACHE_BYPASS;
            egress_cached = PIF_FC_CACHE_BYPASS;
#endif /* PIF_GLOBAL_FLOWCACHE_DISABLED */

            /*
            * Ingress processing
            */
            pif_pkt_info_spec.trunc_len = 0; /* set the truncate length to 0, indicate no truncate has happened */

#ifdef PIF_GLOBAL_FLOWCACHE_DISABLED
            {
                int ret;
                ret = pif_ctlflow_ingress_flow((int *)&exit_node,
                                               (__lmem uint32_t *)parrep,
                                               act_buf, PIF_FC_PAYLOAD_OFF);
                if (ret < 0) { /* drop */
                    int len = -ret;

                    /* check for error, make sure to disable the cached
                     * action for the error case
                     */
                    if (len & (1 << PIF_LOOKUP_ERROR_BIT)) {
                        int errcode = len & ((1 << PIF_LOOKUP_ERROR_BIT) - 1);
                        PIF_COUNT_RANGE(ERROR_INGRESS, errcode);
                    }

                    pktio_tx_drop();
#ifdef CFLOW_TSTAMP
                    cflow_tstamp_end(CFLOW_TSTAMP_USER_DROP_ON_INGRESS,
                                 pif_pkt_info_global.p_seq);
#endif
#ifdef PIF_RECURSE_PACKET
                    /* Reset duplication state if resubmit packet is dropped */
                    if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_RESUBMIT)
                        pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;
#endif
                    PIF_COUNT(DROP);
                    continue;
                }
                __critical_path();

                ingress_cached = ret;
            }
#else
            if (ingress_cached < 0) { /* uncached */
                int ret;

                if (ingress_cached == PIF_FC_CACHE_INITIAL) {
                    pif_global_set_flow_id((__lmem uint32_t *)parrep,
                                           act_buf);
                } else {
                    pif_global_set_flow_id((__lmem uint32_t *)parrep,
                                           (__mem __addr40 void *)-1);
                }

                ret = pif_ctlflow_ingress_flow((int *)&exit_node,
                                               (__lmem uint32_t *)parrep,
                                               act_buf, PIF_FC_PAYLOAD_OFF);
                if (ret < 0) { /* drop */
                    int len = -ret;

                    /* check for error, make sure to disable the cached
                     * action for the error case
                     */
                    if (len & (1 << PIF_LOOKUP_ERROR_BIT)) {
                        int errcode = len & ((1 << PIF_LOOKUP_ERROR_BIT) - 1);
                        PIF_COUNT_RANGE(ERROR_INGRESS, errcode);
                        pif_flowcache_bypass = 1;
                    }
                    /* store the action buffer length so actions up to and
                     * including drop will be done each time
                     */

                    /* release the lock if needed */
                    if (ingress_cached == PIF_FC_CACHE_INITIAL) {
                        if (pif_flowcache_bypass)
                            ret = PIF_FC_CACHE_BYPASS;
                        else
                            ret = len;
                        load_fc_control(act_buf, ret, 0);
                        fc_do_release(result.lock);
                    }

                    pktio_tx_drop();
#ifdef CFLOW_TSTAMP
                    cflow_tstamp_end(CFLOW_TSTAMP_USER_DROP_ON_INGRESS,
                                 pif_pkt_info_global.p_seq);
#endif
#ifdef PIF_RECURSE_PACKET
                    /* Reset duplication state if resubmit packet is dropped */
                    if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_RESUBMIT)
                        pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;
#endif
                    PIF_COUNT(DROP);
                    continue;
                }
                __critical_path();
#ifdef CFLOW_TSTAMP
                cflow_tstamp(CFLOW_TSTAMP_USER_INGRESS_UNCACHED,
                             pif_pkt_info_global.p_seq);
#endif

                if (pif_flowcache_bypass) {
                    ingress_cached = PIF_FC_CACHE_BYPASS;
                } else {
                    __critical_path();
                    ingress_cached = ret;
                }
            } else { /* cached */
                int ret;

                __critical_path();
                pif_global_set_flow_id((__lmem uint32_t *)parrep, act_buf);
                ret = pif_action_execute((__lmem uint32_t *)parrep,
                                         act_buf,
                                         PIF_FC_PAYLOAD_OFF,
                                         ingress_cached);
                if (ret < 0) { /* drop */
                    fc_do_release(result.lock);
                    pktio_tx_drop();
#ifdef CFLOW_TSTAMP
                cflow_tstamp_end(CFLOW_TSTAMP_USER_DROP_ON_INGRESS,
                             pif_pkt_info_global.p_seq);
#endif
#ifdef PIF_RECURSE_PACKET
                    /* Reset duplication state if resubmit packet is dropped */
                    if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_RESUBMIT)
                        pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;
#endif
                    PIF_COUNT(DROP);
                    continue;
                }
                __critical_path();

                if (egress_cached == PIF_FC_CACHE_BYPASS) {
                    /* if egress is not cacheable, use the local action buffer
                    * for the second part and release the lock
                    */
                    act_buf = action_buffer;
                    fc_do_release(result.lock);
                }
                __critical_path();
#ifdef CFLOW_TSTAMP
                cflow_tstamp(CFLOW_TSTAMP_USER_INGRESS_CACHED,
                             pif_pkt_info_global.p_seq);
#endif
            }
#endif /* !PIF_GLOBAL_FLOWCACHE_DISABLED */


            /*
            * Check for packet cloning (after ingress processing) unless packet
            * has to be dropped.
            */
            {
                __gpr uint32_t port_spec = pif_global_get_stdmd_egress_spec((__lmem uint32_t *)parrep);

#if (MCAST_GROUP_MAX > 0)
                if (PIF_PORT_SPEC_TYPE_of(port_spec) == PIF_PORT_SPEC_TYPE_MULTICAST) {

                    __gpr int32_t port = -1;

                    /*
                    * First time lookup for multicast, save egress length as 0
                    */
                    if (egress_cached == PIF_FC_CACHE_INITIAL)
                        load_fc_control(act_buf, ingress_cached, 0);

                    /*
                    * We can release the exclusive lock even if we are still sending out mcast packets
                    * as we won't be accessing the payload for egress. We have all the information we need.
                    */
#ifndef PIF_GLOBAL_FLOWCACHE_DISABLED
                    if (flowkey_len != 0)
                        fc_do_release(result.lock);
#endif /* PIF_GLOBAL_FLOWCACHE_DISABLED */

                    /*
                    * Do multicast processing
                    */
                    pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_REPLICATION;
                    rep_info.__raw = PIF_PORT_SPEC_PORT_of(port_spec) << 16;

                    if (rep_info.group < MCAST_GROUP_MAX)
                    {
                        port = process_multicast(rep_info);
                        PIF_COUNT(MULTICAST);
                    }

                    /*
                    * Completed multicast processing
                    */
                    if (port == -1) {
                        pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;
                        continue;
                    }

                    rep_info.rid++;

                    /*
                    * Set to always do ingress match/action.
                    * Egress actions are not saved to payload as these can be different between elements in the multicast group.
                    */
                    egress_cached = PIF_FC_CACHE_BYPASS;
                    act_buf = action_buffer;

                }
                else
#endif
                {
                    /*
                    * Set egress port for non-mcast packets.
                    * Mcast will set egress port for each element in the mcast group.
                    */
                    __critical_path();

#ifdef PIF_RECURSE_PACKET
                    if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_RESUBMIT) {
#ifndef PIF_GLOBAL_FLOWCACHE_DISABLED
                        /*
                        * Because the packet is resubmitted, another fc lookup
                        * will be done, release the lock on the current entry.
                        */
                        if (flowkey_len != 0)
                            fc_do_release(result.lock);
#endif /* PIF_GLOBAL_FLOWCACHE_DISABLED */
                        /*
                        * First time lookup for clone, save egress length as initial
                        */
                        if (egress_cached == PIF_FC_CACHE_INITIAL)
                            load_fc_control(act_buf, ingress_cached, egress_cached);

                        pif_resubmit_post_ingress((__lmem uint32_t *)parrep);

                        continue; // cycle to top of while loop - parsing
                    }
#endif
                    pif_global_set_stdmd_egress_port((__lmem uint32_t *)parrep, port_spec);

                    if (pkt_dup_state.action) {

                        /*
                        * First time lookup for clone, save egress length as initial
                        */
                        if (egress_cached == PIF_FC_CACHE_INITIAL)
                            load_fc_control(act_buf, ingress_cached, egress_cached);

                        /*
                         * Process any ingress duplication actions
                         */

#ifdef PIF_CLONE_PACKET
                        if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_I2I_CLONE) {

                            /*
                            * Do not release fc lock because the lock is
                            * released when the normal packet does egress processing
                            */

                            pif_clone_packet((__lmem uint32_t *)parrep, rep_info.rid);

                            /*
                            * original packet proceeds as normal
                            */

                        } else if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_I2E_CLONE) {

                            /*
                            * Do not release fc lock because the lock is
                            * released when the normal packet does egress processing.
                            * If the egress_spec is not set, packet will be dropped,
                            * clear the cloning action (without cloning).
                            */
                            if (port_spec != PIF_EGRESS_DROP)
                                pif_clone_i2e((__lmem uint32_t *)parrep, rep_info.rid);
                            else
                                pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;
                            /*
                            * original packet proceeds as normal
                            */
                        }
#endif
                    }
                }
            }
        } else {
            /*
             * Egress processing
             */
            __gpr uint32_t type = pkt_dup_state.type;

#if (MCAST_GROUP_MAX > 0)
            if (type == PIF_PKT_INSTANCE_TYPE_REPLICATION) {
                /*
                * If to do multicast processing
                */
                __gpr int32_t port = process_multicast(rep_info);

                /*
                * Drop the original packet if we have processed all multicast elements
                */
                if (port == -1) {
                    pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;          // finished multicast processing

                    continue;
                }

                rep_info.rid++;

                /*
                * Set to always do ingress match/action.
                * Egress actions are not saved to payload as these can be different between elements in the multicast group.
                */
                egress_cached = PIF_FC_CACHE_BYPASS;
                act_buf = action_buffer;

            }
#ifdef PIF_CLONE_PACKET
            else
#endif
#endif

#ifdef PIF_CLONE_PACKET
                if (type == PIF_PKT_INSTANCE_TYPE_I2E_CLONE
                    || type == PIF_PKT_INSTANCE_TYPE_E2E_CLONE) {
                /*
                * If clone ingress to egress, we do not have flow key,
                * no fc lookup and no egress cached. Clear clone flag
                */
                pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;
                egress_cached = PIF_FC_CACHE_BYPASS;
                act_buf = action_buffer;
            }
#endif
        }



        /*
         * Egress processing
         */

        /* set the nic offload bit if needed */
        pif_global_get_nic_rxoffload((__lmem uint32_t *)parrep);

#ifdef PIF_GLOBAL_FLOWCACHE_DISABLED
        {
            int ret;
            ret = pif_ctlflow_egress_flow((int *)&exit_node,
                                           (__lmem uint32_t *)parrep,
                                           act_buf, PIF_FC_PAYLOAD_OFF + ingress_cached);

            if (ret < 0) { /* drop */
                int len = -ret;

                if (len & (1 << PIF_LOOKUP_ERROR_BIT)) {
                    int errcode = len & ((1 << PIF_LOOKUP_ERROR_BIT) - 1);
                    /* check for error, make sure to disable the cached
                     * action for the error case
                     */
                    PIF_COUNT_RANGE(ERROR_EGRESS, errcode);
                }
                pktio_tx_drop();
#ifdef CFLOW_TSTAMP
                cflow_tstamp_end(CFLOW_TSTAMP_USER_DROP_ON_EGRESS,
                             pif_pkt_info_global.p_seq);
#endif
#ifdef PIF_RECURSE_PACKET
                /* Reset duplication state if recirc packet is dropped */
                if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_INGRESS_RECIRC)
                    pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;
#endif
                PIF_COUNT(DROP);
                continue;
            }
            __critical_path();
        }
#else
        if (egress_cached < 0) {
            int ret;

            ret = pif_ctlflow_egress_flow((int *)&exit_node,
                                           (__lmem uint32_t *)parrep,
                                           act_buf, PIF_FC_PAYLOAD_OFF + ingress_cached);

            if (ret < 0) { /* drop */
                int len = -ret;

                if (len & (1 << PIF_LOOKUP_ERROR_BIT)) {
                    int errcode = len & ((1 << PIF_LOOKUP_ERROR_BIT) - 1);
                    /* check for error, make sure to disable the cached
                     * action for the error case
                     */
                    PIF_COUNT_RANGE(ERROR_EGRESS, errcode);
                    pif_flowcache_bypass = 1;
                }
                /* store the action buffer length so actions up to and
                 * including drop will be done each time
                 */
                if (pif_flowcache_bypass)
                    ret = PIF_FC_CACHE_BYPASS;
                else
                    ret = len;
                load_fc_control(act_buf, ingress_cached, ret);

                if (egress_cached == PIF_FC_CACHE_INITIAL)
                    fc_do_release(result.lock);     // only release if not multicast processing

                pktio_tx_drop();
#ifdef CFLOW_TSTAMP
                cflow_tstamp_end(CFLOW_TSTAMP_USER_DROP_ON_EGRESS,
                             pif_pkt_info_global.p_seq);
#endif
#ifdef PIF_RECURSE_PACKET
                /* Reset duplication state if recirc packet is dropped */
                if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_INGRESS_RECIRC)
                    pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;
#endif
                PIF_COUNT(DROP);
                continue;
            }
            __critical_path();

            if (egress_cached == PIF_FC_CACHE_INITIAL) {
                /*
                 * save the egress length and release lock if not mcast
                 */

                if (pif_flowcache_bypass)
                    ret = PIF_FC_CACHE_BYPASS;

                load_fc_control(act_buf, ingress_cached, ret);
                fc_do_release(result.lock);
            }
            __critical_path();
#ifdef CFLOW_TSTAMP
            cflow_tstamp(CFLOW_TSTAMP_USER_EGRESS_UNCACHED,
                         pif_pkt_info_global.p_seq);
#endif
        } else {
            int ret;

            __critical_path();

            ret = pif_action_execute((__lmem uint32_t *)parrep,
                                     act_buf,
                                     PIF_FC_PAYLOAD_OFF + ingress_cached,
                                     egress_cached);

            /* always release the lock */
            fc_do_release(result.lock);

            if (ret < 0) {
                pktio_tx_drop();
#ifdef CFLOW_TSTAMP
                cflow_tstamp_end(CFLOW_TSTAMP_USER_DROP_ON_EGRESS,
                             pif_pkt_info_global.p_seq);
#endif
#ifdef PIF_RECURSE_PACKET
                /* Reset duplication state if recirc packet is dropped */
                if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_INGRESS_RECIRC)
                    pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;
#endif
                PIF_COUNT(DROP);
                continue;
            }
            __critical_path();
#ifdef CFLOW_TSTAMP
            cflow_tstamp(CFLOW_TSTAMP_USER_EGRESS_CACHED,
                         pif_pkt_info_global.p_seq);
#endif
        }
#endif /* !PIF_GLOBAL_FLOWCACHE_DISABLED */


        /* Do P4 forwarding */
        if (forward((__lmem uint32_t *)parrep)) {
            pktio_tx_drop();
#ifdef CFLOW_TSTAMP
            cflow_tstamp_end(CFLOW_TSTAMP_USER_DROP_ON_EGRESS,
                         pif_pkt_info_global.p_seq);
#endif
#ifdef PIF_RECURSE_PACKET
            /* Reset duplication state if resubmit or recirc packet is dropped */
            if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_INGRESS_RECIRC)
                pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;
#endif
#ifdef PIF_CLONE_PACKET
            /*
            * Setup cloned packet as next packet to process
            * if packet_type is set to:
            * PIF_PKT_INSTANCE_TYPE_I2I_CLONE
            */
            if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_E2I_CLONE
                || pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_E2E_CLONE)
                pkt_dup_state.__raw = PIF_PKT_INSTANCE_TYPE_NORMAL;

            rep_info.rid = pif_clone_packet_submit((__lmem uint32_t *)parrep, rep_info.rid);
#endif
            continue;
        }
        __critical_path();

        /* Update calculated fields if required. */
        pif_flcalc_update((__lmem uint32_t *)parrep);

#ifdef CFLOW_TSTAMP
        cflow_tstamp(CFLOW_TSTAMP_USER_CHECKSUMS,
                 pif_pkt_info_global.p_seq);
#endif

        /* Deparsing */
        PIF_DEBUG_SET_STATE(PIF_DEBUG_STATE_DEPARSING, 0);
        {
            __gpr int pkt_byteoffset = pif_deparse((__lmem uint32_t *)parrep, &pif_pkt_info_global);

            if (pkt_byteoffset < 0) {
                handle_error_packet(PACKET_ERROR_DEPARSE);
                PIF_COUNT_RANGE(ERROR_DEPARSE, (-pkt_byteoffset));
                continue;
            }
        }

        /* Check if a pkt duplication action was set during egress processing */
        if (pkt_dup_state.action)
        {
#ifdef PIF_RECURSE_PACKET
            if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_INGRESS_RECIRC) {
                /*
                * Recirculate
                */
                pif_resubmit_post_ingress((__lmem uint32_t *)parrep);
                continue;  //cycle to parsing
            }
#endif
#ifdef PIF_CLONE_PACKET
#ifdef PIF_RECURSE_PACKET
                else
#endif
                    if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_E2I_CLONE) {
                    /*
                    * Clone egress to ingress
                    */
                    pif_clone_packet((__lmem uint32_t *)parrep, rep_info.rid);

                    /*
                    * original packet proceeds as normal
                    */
                } else if (pkt_dup_state.type == PIF_PKT_INSTANCE_TYPE_E2E_CLONE) {
                    /*
                    * Clone egress to ingress
                    */
                    pif_clone_packet((__lmem uint32_t *)parrep, rep_info.rid);

                    /*
                    * original packet proceeds as normal
                    */
                }
#endif
        }
        __critical_path();

#ifdef CFLOW_TSTAMP
        cflow_tstamp(CFLOW_TSTAMP_USER_AFTER_DEPARSING,
                        pif_pkt_info_global.p_seq);
#endif

        /* Transmit packet */
        PIF_DEBUG_SET_STATE(PIF_DEBUG_STATE_SENDING, 0);
        {
            int ret_code;

#ifdef PIF_GLOBAL_WQ_DISABLED
            ret_code = pktio_tx();
#else /* PIF_GLOBAL_WQ_DISABLED */
            if (PKT_PORT_TYPE_of(pif_pkt_info_global.p_dst) == PKT_PTYPE_WQ) {
                /* Sadly pktio doesn't support all the WQ metadata words
                 * so we duplicate the wq stuff here
                 */
#ifdef PKTIO_GRO_ENABLED
                __xwrite union gro_meta gmeta;
#endif
                struct pktio_handle ph;
                __xwrite struct pktio_meta xwq;
                int dst_q = PKT_PORT_WQNUM_of(pkt.p_dst);
                __gpr uint32_t mw0, mw1;
                __mem40 uint32_t *ctm_ptr;

                xwq = pkt;
                ctm_ptr = pkt_ctm_ptr40(pkt.p_isl, pkt.p_pnum, 0);
                mem_write32(&xwq, ctm_ptr, sizeof(xwq));
                ph.__raw = 0;
                ph.ph_isl = pkt.p_isl;
                ph.ph_pnum = pkt.p_pnum;

                pif_global_get_wq_metawords((__lmem uint32_t *)parrep,
                                            &mw0, &mw1);

#define WQ_ISLNO 24
/* NOTE: we only support emu 0 for now */
#ifdef PKTIO_GRO_ENABLED
                if (pkt.p_is_gro_seq) {
                    gro_cli_build_workq_meta3(&gmeta.memq,
                                              WQ_ISLNO,
                                              dst_q,
                                              ph.__raw,
                                              mw0, mw1);
                    gro_cli_send(&gmeta, pkt.p_ro_ctx, pkt.p_seq);
                } else
#endif
                {
                    uint32_t ma = MUID_TO_MEM_RING_ADDR(ISL_TO_MUID(WQ_ISLNO));
                    __xwrite struct wq_handle {
                            struct pktio_handle xph;
                            uint32_t mw0;
                            uint32_t mw1;
                    } wh;

                    wh.xph = ph;
                    wh.mw0 = mw0;
                    wh.mw1 = mw1;

                    mem_workq_add_work(dst_q, ma, &wh, sizeof(wh));
                }
            } else
                ret_code = pktio_tx();
#endif /* ! PIF_GLOBAL_WQ_DISABLED */
            if (PKT_PORT_TYPE_of(pif_pkt_info_global.p_dst) == PKT_PTYPE_WIRE) {
                if (ret_code == 0) {
                    __critical_path();
                    PIF_COUNT(TX);
                } else if (ret_code == -2) {
                    PIF_COUNT(DROP_INVALID_PKT_MOD_SCRIPT);
                } else if (ret_code == -3) {
                    PIF_COUNT(DROP_PORT_PAUSED);
                }
            } else if (PKT_PORT_TYPE_of(pif_pkt_info_global.p_dst)
                     == PKT_PTYPE_WQ) {
                PIF_COUNT(TX_WQ);
            } else if (ret_code == 0 &&
                       PKT_PORT_TYPE_of(pif_pkt_info_global.p_dst)
                       == PKT_PTYPE_HOST) {
                PIF_COUNT(TX_NFD);
                __critical_path();
            } else {
                PIF_COUNT(DROP_NFD_NO_CREDITS);
            }
        }

#ifdef CFLOW_TSTAMP
        cflow_tstamp_end(CFLOW_TSTAMP_USER_TRANSMIT_PACKET,
                     pif_pkt_info_global.p_seq);
#endif
        /*
         * Go to top of while-loop for normal pkts (performance sake)
         */
        if (pkt_dup_state.__raw == PIF_PKT_INSTANCE_TYPE_NORMAL
            || pkt_dup_state.__raw == PIF_PKT_INSTANCE_TYPE_REPLICATION)
            continue;

#ifdef PIF_CLONE_PACKET
        /*
        * Setup cloned packet as next packet to process
        * if packet_type is set to:
        * PIF_PKT_INSTANCE_TYPE_I2I_CLONE
        * PIF_PKT_INSTANCE_TYPE_I2E_CLONE
        * PIF_PKT_INSTANCE_TYPE_E2I_CLONE
        * PIF_PKT_INSTANCE_TYPE_E2E_CLONE
        */
        rep_info.rid = pif_clone_packet_submit((__lmem uint32_t *)parrep, rep_info.rid);
#endif
    } // while (1)
}
