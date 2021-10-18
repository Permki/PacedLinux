/**
 * Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved.
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
 * @file          nfd_user_cfg.h
 * @brief         File for specifying user configuration parameters
 */
#ifndef _NFD_USER_CFG_H_
#define _NFD_USER_CFG_H_

/* Include BLM defines for BLM_NBI8_BLQ0_EMU_Q_BASE */
#if !defined(__NFP_LANG_ASM)
#include <blm_c.h>
#endif

#ifndef NFD_MAX_VF_QUEUES
#define NFD_MAX_VF_QUEUES       1
#endif

#ifndef NFD_MAX_PF_QUEUES
#define NFD_MAX_PF_QUEUES       0
#endif

#ifndef NFD_MAX_VFS
#define NFD_MAX_VFS             64
#endif

#ifndef NFD_MAX_PFS
#define NFD_MAX_PFS             0
#endif

/* Configure VF expansion BARs to access the NFP, this seems to be required
 * even when just using the PF */
#define NFD_VNIC_VF

/* Use a service ME which will handle MSIX generation when enabled */
#define USE_SVC_ME

#define NFD_PCIE0_EMEM          emem0
/* Increase the number of free list buffers and cache them in emem_cache_upper
 * rather than CTM */

#if PCIE_ISL_CNT >= 4
#define FL_BUFS_PER_QUEUE 256
#define NFD_PCIE3_EMEM          emem0
#define NFD_PCIE2_EMEM          emem0
#define NFD_PCIE1_EMEM          emem0
#elif PCIE_ISL_CNT >= 2
#define FL_BUFS_PER_QUEUE 512
#define NFD_PCIE1_EMEM          emem0
#else
#define FL_BUFS_PER_QUEUE 1024
#endif
#define NFD_PCIE0_FL_CACHE_MEM      emem0.emem_cache_upper

#if FL_BUFS_PER_QUEUE > 256
/* Increase the number of free list buffers and cache them in emem_cache_upper
 * rather than CTM */
#define NFD_OUT_FL_BUFS_PER_QUEUE   FL_BUFS_PER_QUEUE
#define NFD_PCIE1_FL_CACHE_MEM      emem0.emem_cache_upper
#define NFD_OUT_FL_BUFS_PER_QUEUE   FL_BUFS_PER_QUEUE
#endif

#define NFD_IN_DATA_OFFSET      128

/* Number of credits allocated per VNIC queue */
#ifndef NFD_QUEUE_CREDITS
#define NFD_QUEUE_CREDITS       256
#endif

/* Configuration mechanism defines */
#define NFD_CFG_MAX_MTU         9216

#ifdef PKTIO_VLAN_ENABLED
#define NFD_NET_CFG_VLAN (NFP_NET_CFG_CTRL_RXVLAN | NFP_NET_CFG_CTRL_TXVLAN)
#else
#define NFD_NET_CFG_VLAN 0
#endif

#define NFD_CFG_VF_CAP                                          \
    (NFP_NET_CFG_CTRL_ENABLE | NFP_NET_CFG_CTRL_PROMISC |       \
     NFP_NET_CFG_CTRL_L2BC | NFP_NET_CFG_CTRL_L2MC |            \
     NFP_NET_CFG_CTRL_RXCSUM | NFP_NET_CFG_CTRL_TXCSUM |        \
     NFD_NET_CFG_VLAN |                                         \
     NFP_NET_CFG_CTRL_RSS    | NFP_NET_CFG_CTRL_MSIXAUTO |      \
     NFP_NET_CFG_CTRL_L2SWITCH | NFP_NET_CFG_CTRL_GATHER |      \
     NFP_NET_CFG_CTRL_IRQMOD)


#define NFD_CFG_PF_CAP                                          \
    (NFP_NET_CFG_CTRL_ENABLE | NFP_NET_CFG_CTRL_PROMISC |       \
     NFP_NET_CFG_CTRL_L2BC | NFP_NET_CFG_CTRL_L2MC |            \
     NFP_NET_CFG_CTRL_RXCSUM | NFP_NET_CFG_CTRL_TXCSUM |        \
     NFP_NET_CFG_CTRL_RXVLAN | NFP_NET_CFG_CTRL_TXVLAN |        \
     NFP_NET_CFG_CTRL_RSS    | NFP_NET_CFG_CTRL_MSIXAUTO |      \
     NFP_NET_CFG_CTRL_L2SWITCH | NFP_NET_CFG_CTRL_GATHER |      \
     NFP_NET_CFG_CTRL_IRQMOD)


#define NFD_RSS_HASH_FUNC NFP_NET_CFG_RSS_TOEPLITZ
#define NFD_CFG_RING_EMEM emem0

/* PCI.IN block defines */
#define NFD_IN_BLM_REG_BLS          0
#define NFD_IN_BLM_REG_POOL         BLM_NBI8_BLQ0_EMU_QID
#define NFD_IN_BLM_REG_SIZE     (10 * 1024)
#define NFD_IN_BLM_JUMBO_BLS    0
#define NFD_IN_BLM_JUMBO_POOL   BLM_NBI8_BLQ0_EMU_QID
#define NFD_IN_BLM_JUMBO_SIZE   10240
#define NFD_IN_BLM_RADDR        __LoadTimeConstant("__addr_emem0")
#define NFD_IN_HAS_ISSUE0       1
#define NFD_IN_HAS_ISSUE1       1
#define NFD_IN_ISSUE_DMA_QSHIFT 0
#define NFD_IN_ISSUE_DMA_QXOR   0

/* NFD_IN_WQ_SZ must be set to hold the maximum number of MU buffers available
 * through the selected BLM queue in the system. */
#define NFD_IN_WQ_SZ            (BLM_NBI8_BLQ0_Q_SIZE*16)

/* Optional defines */
#define NFD_IN_ADD_SEQN
#define NFD_IN_NUM_SEQRS        GRO_CTX_PER_PCI
#define NFD_IN_NUM_WQS          1

/* PCI.OUT block defines */
#define NFD_OUT_BLM_POOL_START  BLM_NBI8_BLQ0_EMU_QID
#define NFD_OUT_BLM_RADDR       __LoadTimeConstant("__addr_emem0")
#define NFD_OUT_BLM_RADDR_UC    __ADDR_EMEM0

/* Set either NFP_CACHED or HOST_ISSUED credits
 * Only NFP_CACHED are officially supported currently */
/* #define NFD_OUT_CREDITS_HOST_ISSUED */
#define NFD_OUT_CREDITS_NFP_CACHED

/* NFD_OUT_RING_SZ must be set to hold double the maximum number of credits
 * that might be issued at any time. */
#define NFD_OUT_RING_SZ         (2 * 16 * 64 * NFD_QUEUE_CREDITS)

#define NFD_OUT_RX_OFFSET       0

#define NFD_IN_WQ_SHARED emem0

#endif /* !_NFD_USER_CFG_H_ */
