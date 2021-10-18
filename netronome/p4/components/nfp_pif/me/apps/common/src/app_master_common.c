/*
 * Copyright 2015 Netronome, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file          app_master.c
 * @brief         Code for ME serving as the NFD app master,
 *                and MAC port pause polling
 */

#include <assert.h>
#include <nfp_chipres.h>
#include <nfp.h>

#include <nfp/me.h>
#include <nfp/mem_bulk.h>
#include <nfp/remote_me.h>
#include <nfp6000/nfp_me.h>
#include <modscript/modscript_init.h>
#include <macflush/macflushuser.h>

#include <mac_time.h>

#ifndef PIF_GLOBAL_NFD_DISABLED
#include <vnic/shared/nfd_cfg.h>
#include <vnic/pci_in.h>
#include <vnic/pci_out.h>
#endif /* PIF_GLOBAL_NFD_DISABLED */

#include <system_init_c.h>
#include <port_config.h>

#include <app_master_common.h>

__gpr __shared uint32_t app_master_nfd_config_changed = 0;

#ifndef PIF_GLOBAL_NFD_DISABLED
__visible SIGNAL nfd_cfg_sig_app_master0;
/* we declare these signals regardless of how many pcie islands there are */
__visible SIGNAL nfd_cfg_sig_app_master1;
__visible SIGNAL nfd_cfg_sig_app_master2;
__visible SIGNAL nfd_cfg_sig_app_master3;

NFD_CFG_BASE_DECLARE(0);
#if PCIE_ISL_CNT > 1
NFD_CFG_BASE_DECLARE(1);
#endif
#if PCIE_ISL_CNT > 2
NFD_CFG_BASE_DECLARE(2);
#endif
#if PCIE_ISL_CNT > 3
NFD_CFG_BASE_DECLARE(3);
#endif

struct nfd_cfg_msg cfg_msg[PCIE_ISL_CNT];
__xread unsigned int cfg_bar_data[6];

NFD_FLR_DECLARE;

PORT_CONFIG_DECLARE;

#define NFD_PROCESS(_pcie_isl) \
do { \
    __xwrite unsigned int link_state; \
    struct port_config pcfg; \
 \
    nfd_cfg_master_chk_cfg_msg(&cfg_msg[_pcie_isl], \
                               &nfd_cfg_sig_app_master##_pcie_isl, \
                               _pcie_isl); \
 \
    if (cfg_msg[_pcie_isl].msg_valid) { \
        mem_read64(cfg_bar_data, \
                   NFD_CFG_BAR_ISL(_pcie_isl, cfg_msg[_pcie_isl].vid), \
                   sizeof(cfg_bar_data)); \
 \
        local_csr_write(local_csr_mailbox_0, cfg_msg[_pcie_isl].vid); \
        local_csr_write(local_csr_mailbox_1, cfg_bar_data[0]); \
        local_csr_write(local_csr_mailbox_2, _pcie_isl); \
 \
        pcfg = port_config_get(PORT_CONFIG_TYPE_VF, \
                               _pcie_isl, \
                               cfg_msg[_pcie_isl].vid); \
        pcfg.nic_offload_vlan_rx = (cfg_bar_data[0] & NFP_NET_CFG_CTRL_RXVLAN) != 0; \
        pcfg.nic_offload_vlan_tx = (cfg_bar_data[0] & NFP_NET_CFG_CTRL_TXVLAN) != 0; \
        port_config_set(PORT_CONFIG_TYPE_VF, _pcie_isl, cfg_msg[_pcie_isl].vid, pcfg); \
        app_master_nfd_config_changed = 1; \
 \
        /* Set link state */ \
        if (!cfg_msg[_pcie_isl].error && (cfg_bar_data[NFP_NET_CFG_CTRL] & \
                               NFP_NET_CFG_CTRL_ENABLE)) { \
            link_state = NFP_NET_CFG_STS_LINK; \
        } else { \
            link_state = 0; \
        } \
        mem_write32(&link_state, \
                    (NFD_CFG_BAR_ISL(_pcie_isl, cfg_msg[_pcie_isl].vid) + \
                     NFP_NET_CFG_STS), \
                    sizeof link_state); \
 \
        /* Complete the message */ \
        cfg_msg[_pcie_isl].msg_valid = 0; \
        nfd_cfg_app_complete_cfg_msg(_pcie_isl, &cfg_msg[_pcie_isl], \
                                     NFD_CFG_BASE_LINK(_pcie_isl), \
                                     &nfd_cfg_sig_app_master##_pcie_isl); \
    } \
    ctx_swap(); \
} while(0)


void app_master_nfdcfg_poll(void)
{
    int i;

    nfd_cfg_init_cfg_msg(&nfd_cfg_sig_app_master0, &cfg_msg[0]);
#if PCIE_ISL_CNT > 1
    nfd_cfg_init_cfg_msg(&nfd_cfg_sig_app_master1, &cfg_msg[1]);
#endif
#if PCIE_ISL_CNT > 2
    nfd_cfg_init_cfg_msg(&nfd_cfg_sig_app_master2, &cfg_msg[2]);
#endif
#if PCIE_ISL_CNT > 3
    nfd_cfg_init_cfg_msg(&nfd_cfg_sig_app_master3, &cfg_msg[3]);
#endif

    while (1) {
        NFD_PROCESS(0);
#if PCIE_ISL_CNT > 1
        NFD_PROCESS(1);
#endif
#if PCIE_ISL_CNT > 2
        NFD_PROCESS(2);
#endif
#if PCIE_ISL_CNT > 3
        NFD_PROCESS(3);
#endif
        sleep(APP_MASTER_POLL_NFDCFG);
    }
}
#else /* !PIF_GLOBAL_NFD_DISABLED */
void app_master_nfdcfg_poll(void)
{
    __asm ctx_arb[kill];
}
#endif /* PIF_GLOBAL_NFD_DISABLED */

#ifndef PIF_GLOBAL_NFD_DISABLED
/*
 * Handle per Q statistics
 *
 * - Periodically push TX and RX queue counters maintained by the PCIe
 *   MEs to the control BAR.
 */
void app_master_nfdstats_poll(void)
{
    SIGNAL rxq_sig;
    SIGNAL txq_sig;
    unsigned int rxq;
    unsigned int txq;

    /* Initialisation */
    nfd_in_recv_init();
    nfd_out_send_init();

    // TODO: add support for multiple pcie devices
    for (;;) {
        for (txq = 0;
                txq < (NFD_MAX_VFS * NFD_MAX_VF_QUEUES + NFD_MAX_PF_QUEUES);
                txq++) {
            __nfd_out_push_pkt_cnt(0, txq, ctx_swap, &txq_sig);
        }

        for (rxq = 0;
                rxq < (NFD_MAX_VFS * NFD_MAX_VF_QUEUES + NFD_MAX_PF_QUEUES);
                rxq++) {
            __nfd_in_push_pkt_cnt(0, rxq, ctx_swap, &rxq_sig);
        }

        sleep(APP_MASTER_POLL_NFDSTATS);
    }
    /* NOTREACHED */
}
#else /* !PIF_GLOBAL_NFD_DISABLED */
void app_master_nfdstats_poll(void)
{
    __asm ctx_arb[kill];
}
#endif /* PIF_GLOBAL_NFD_DISABLED */

void app_master_mactime_poll(void)
{
    while (1) {
        mac_time_update();
        sleep(APP_MASTER_POLL_MACTIME);
    }
}

void app_master_macport_poll(void)
{
    while (1) {
        mac_poll();
        sleep(APP_MASTER_POLL_MACPORT);
    }
}
