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
 * @file          apps/pif_app_nfd/config.h
 * @brief         Infrastructure configuration for the P4 application.
 */

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include <pif_design.h>
#include <pif_platform.h>

#ifndef PCIE_ISL_CNT
#define PCIE_ISL_CNT 1
#endif

#ifndef NUM_NBI
#define NUM_NBI 1
#endif

//#define PIF_GLOBAL_TRAFFIC_CLASS_DISABLED

/*
 * PIF app configuration.
 *
 * Define PIF_APP_CONFIG as one of the following to configure the main pif app.
 * - PIF_APP_DEFAULT
 *    Use 4 contexts each for NBI and PCIe/NFD packet ingress.
 *    Use GRO for packet egress.
 * - PIF_APP_NBI_INGRESS_ONLY
 *    Use all contexts for NBI packet ingress.
 *    Do not use GRO for egress; all packets unsequenced.
 * - PIF_APP_PCIE_INGRESS_ONLY
 *    Use all contexts for PCIe/NFD packet ingress.
 *    Do not use GRO for egress; all packets unsequenced.
 * - PIF_APP_CUSTOM
 *    Do not override pre-defined configuration constants.
 * - PIF_APP_NBI_ONLY
 *    Use all contexts for NBI packets.
 *    Do not use NFD
 *
 * The following configuration constants can be set directly after defining
 * PIF_APP_CONFIG as PIF_APP_CUSTOM.
 *  - PIF_APP_NUM_NBI_CTX
 *      Number of threads to use for NBI packet ingress.
 *  - PIF_APP_NUM_PCIE_CTX
 *      Number of threads to use for PCIe/NFD packet ingress.
 *  - PIF_USE_GRO
 *      Toggles whether or not GRO used. If undefined NBI and PCIe/NFD packets
 *      are set as unsequenced and use sequencer 0.
 */

#define PIF_APP_DEFAULT             0
#define PIF_APP_NBI_INGRESS_ONLY    1
#define PIF_APP_PCIE_INGRESS_ONLY   2
#define PIF_APP_CUSTOM              3
#define PIF_APP_NBI_ONLY            4

#ifndef PIF_APP_CONFIG
    #ifdef PIF_GLOBAL_NFD_DISABLED
        #define PIF_APP_CONFIG PIF_APP_NBI_ONLY
    #else
        #define PIF_APP_CONFIG PIF_APP_DEFAULT
    #endif
#else
    #ifdef PIF_GLOBAL_NFD_DISABLED
        #undef PIF_APP_CONFIG
        #define PIF_APP_CONFIG PIF_APP_NBI_ONLY
    #endif
#endif

#if (PIF_APP_CONFIG == PIF_APP_NBI_INGRESS_ONLY)

    #define PIF_APP_NUM_NBI_CTX     8
    #define PIF_APP_NUM_PCIE_CTX    (8 - PIF_APP_NUM_NBI_CTX)
    #ifdef PIF_USE_GRO
        #undef PIF_USE_GRO
    #endif

#elif (PIF_APP_CONFIG == PIF_APP_PCIE_INGRESS_ONLY)

    #define PIF_APP_NUM_NBI_CTX     0
    #define PIF_APP_NUM_PCIE_CTX    (8 - PIF_APP_NUM_NBI_CTX)
    #ifdef PIF_USE_GRO
        #undef PIF_USE_GRO
    #endif

#elif (PIF_APP_CONFIG == PIF_APP_CUSTOM)

    #ifndef PIF_APP_NUM_NBI_CTX
        #define PIF_APP_NUM_NBI_CTX     4
    #endif
    #ifndef PIF_APP_NUM_PCIE_CTX
        #define PIF_APP_NUM_PCIE_CTX    (8 - PIF_APP_NUM_NBI_CTX)
    #endif

#elif (PIF_APP_CONFIG == PIF_APP_NBI_ONLY)

    #define PIF_USE_GRO
    #define PIF_APP_NUM_NBI_CTX     8
    #define PIF_APP_NUM_PCIE_CTX    (8 - PIF_APP_NUM_NBI_CTX)

#else /* (PIF_APP_CONFIG == PIF_APP_DEFAULT) */

    #define PIF_USE_GRO
    #define PIF_APP_NUM_NBI_CTX     4
    #define PIF_APP_NUM_PCIE_CTX    (8 - PIF_APP_NUM_NBI_CTX)

#endif

#ifdef PIF_GLOBAL_GRO_DISABLED
    #ifdef PIF_USE_GRO
        #undef PIF_USE_GRO
    #endif
#endif

#ifndef PIF_GLOBAL_NFD_DISABLED
    #define PKTIO_NFD_ENABLED
#endif

#ifdef PIF_USE_GRO
    #define PKTIO_GRO_ENABLED
#endif

/*
 * Defines for PKTIO
 */

#define PKTIO_VLAN_ENABLED
#define PKTIO_MAX_TM_QUEUES 1024


/*
 * Mapping of sequencers for reordering
 */

/* this is all a little complicated...
 * for 1xnbi 1xpci we do:
 * - PCI0 sequencers 0, 2, 4, 6
 * - NBI0 sequencers 1, 3, 5, 7
 * for 1xnbi 2xpci we do:
 * - PCI0 sequencers 0, 4
 * - PCI1 sequencers 2, 6
 * - NBI0 sequencers 1, 3, 5, 7
 * for 2xnbi 1xpci:
 * - PCI0 sequencers 0, 2,  4,  6, 8, 10, 12, 14
 * - NBI0 sequencers 1, 5,  9, 13
 * - NBI1 sequencers 3, 7, 11, 15
 * for 2xnbi 2xpci:
 * - PCI0 sequencers 0,  4,  8, 12
 * - PCI1 sequencers 2,  6, 10, 14
 * - NBI0 sequencers 1,  5,  9, 13
 * - NBI1 sequencers 3,  7, 11, 15
 * for 2xnbi 4xpci:
 * - PCI0 sequencers 0,  8
 * - PCI1 sequencers 2,  10
 * - PCI2 sequencers 4,  12
 * - PCI3 sequencers 6,  14
 * - NBI0 sequencers 1,  5,  9, 13
 * - NBI1 sequencers 3,  7, 11, 15
 */

#if NUM_NBI == 2 || PCIE_ISL_CNT == 4
#define GRO_CTX_PER_BLOCK 8
#define GRO_CTX_PCI_TOTAL 8
#else
#define GRO_CTX_PER_BLOCK 4
#define GRO_CTX_PCI_TOTAL 4
#endif

#define GRO_CTX_PER_PCI (GRO_CTX_PCI_TOTAL / PCIE_ISL_CNT)
#define GRO_CTX_PER_NBI 4

#if PCIE_ISL_CNT == 1
#define GRO_CTX_PCI_SHIFT       1
#define GRO_CTX_PCI_OFF(_n)     0
#elif PCIE_ISL_CNT == 2
#define GRO_CTX_PCI_SHIFT       2
#define GRO_CTX_PCI_OFF(_n)     ((_n) * 2)
#else
#define GRO_CTX_PCI_SHIFT       3
#define GRO_CTX_PCI_OFF(_n)     ((_n) * 2)
#endif

#if NUM_NBI == 1
#define GRO_CTX_NBI_OFF(_n)     1
#define GRO_CTX_NBI_SHIFT       1
#else
#define GRO_CTX_NBI_OFF(_n)     ((_n) * 2 + 1)
#define GRO_CTX_NBI_SHIFT       2
#endif

#ifdef PKTIO_GRO_ENABLED
    #ifndef PKTIO_NBI_SEQD_MAP_SEQR
        // Catamaran pico code uses sequencers 1-4
        #define PKTIO_NBI_SEQD_MAP_SEQR(_n, _p) \
                    (((_p - 1) << GRO_CTX_NBI_SHIFT) + GRO_CTX_NBI_OFF(_n))
    #endif
    #ifndef PKTIO_NBI_SEQD_MAP_ISGRO
        #define PKTIO_NBI_SEQD_MAP_ISGRO(metatype, domain)        1
    #endif
    #ifndef PKTIO_NFD_SEQD_MAP_SEQR
        #define PKTIO_NFD_SEQD_MAP_SEQR(_n, _p) \
                    (((_p) << GRO_CTX_PCI_SHIFT) + GRO_CTX_PCI_OFF(_n))
    #endif
    #ifndef PKTIO_NFD_SEQD_MAP_ISGRO
        #define PKTIO_NFD_SEQD_MAP_ISGRO(iface, domain)        1
    #endif
#else
    #ifndef PKTIO_NBI_SEQD_MAP_SEQR
        // GRO is disabled, send unsequenced
        #define PKTIO_NBI_SEQD_MAP_SEQR(metatype, domain)         0
    #endif
    #ifndef PKTIO_NBI_SEQD_MAP_ISGRO
        // set to drop NBI seqnum if PKTIO_NBI_SEQR_ENABLED and GRO disabled
        #define PKTIO_NBI_SEQD_MAP_ISGRO(metatype, domain)        0
    #endif
    #ifndef PKTIO_NFD_SEQD_MAP_SEQR
        // Nothing, map to 0
        #define PKTIO_NFD_SEQD_MAP_SEQR(iface, domain)         0
    #endif
    #ifndef PKTIO_NFD_SEQD_MAP_ISGRO
        #define PKTIO_NFD_SEQD_MAP_ISGRO(iface, domain)        0
    #endif
#endif




/* Configure MAC flush on worker islands - see SDN-1253 */
#define PKTIO_MAC_FLUSH_ENABLED

/* Configure worker islands that should be checked for NBI port paused */
#define WORKER_ISL0 32
#define WORKER_ISL1 33
#define WORKER_ISL2 34
#define WORKER_ISL3 35
#define WORKER_ISL4 36

#if defined(__NFP_LANG_ASM)
    #if (__nfp_has_island(37))
        #define WORKER_ISL5 37
    #endif
    #if (__nfp_has_island(38))
        #define WORKER_ISL6 38
    #endif
#elif defined(__NFP_LANG_MICROC)
    #if (_nfp_has_island("i37"))
        #define WORKER_ISL5 37
    #endif
    #if (_nfp_has_island("i38"))
        #define WORKER_ISL6 38
    #endif
#endif


/*
 * General
 */
#define BACKOFF_SLEEP           256

/*
 * DCFL
 */
#define DCFL_SUPPORT_MATCH_FILTERING
#define DCFL_CUSTOM_TABLE_LW 4


/*
 * RX/TX configuration
 * - Set pkt offset the NBI uses
 * - Set the number of bytes the MAC prepends data into
 * - Configure RX checksum offload so the wire can validate checksums
 */
#define PKT_NBI_OFFSET          128
#define MAC_EGRESS_PREPEND_BYTES 0
#define NBI_PKT_PREPEND_BYTES   8
#define HOST_PREPEND_BYTES      0
#define CFG_RX_CSUM_PREPEND
#define SPLIT_LENGTH            2       /* Split when packet > 1K bytes */
// #define PKTIO_NBI_SEQR_ENABLED          /* NBI sequencer is enabled */

/*
 * NBI configuration
 */
#if (PKT_NBI_OFFSET == 64)
    #define CTM_OFFSET  1       /* offset of 64 Bytes */
#else
    #define CTM_OFFSET  0       /* offset of 128 Bytes */
#endif

/*
 * MAC configuration
 */
#define MAC0_PORTS               0
#define MAC0_CHANNELS            3
#define MAC0_PORTS_LIST          MAC0_PORTS
#define MAC0_CHANNELS_LIST       MAC0_CHANNELS
#define NBI_MAC_MATCHED(_meta)  (((_meta).lif >> (_meta).port) & 0x1)


/*
 * Mapping of port to TrafficManager
 */

#define MAC_CHAN_PER_PORT       8
#define CHANNEL_TO_TMQ(y)       (y)
#define PORT_TO_CHANNEL(x)      ((x) / MAC_CHAN_PER_PORT)

/*
 * Constants used by INIT CSRs
 */

#define NBI_TM_NUM_SEQUENCERS       0 /* Reordering is disabled = unsequenced */

#define NBI_TM_ENABLE_SEQUENCER0        0 /* NBI seqr 0 is unsequenced */
#define NBI_TM_CHANNEL_LEVEL_SELECT     1 /* MAC channel num defined to be
                                           * the level 1 scheduler ports */
#define NBI_TM_L1_INPUT_SELECT          1 /* Level 1 schedulers enabled */
#define NBI_TM_ENABLE_SHAPER            0 /* Rate limiting shaper disabled */

/*
 * CTM credit management, required for RX from host and packet cloning
 * Divide the total buffer and packet credits between NBI and ME.
 * Each Packet Engine has 128 2-KB buffer credits, in the case that all of the
 * Dcache has been allocated to store packets, and 512 packet credits.
 *
 * We only allocate half of the CTM for packets, hence we use 64 buffer
 * credits (and not 128).
 */

/* Total number of packet and buffer credits per CTM */
#define TOTAL_CTM_PACKET_CREDITS            512
#define TOTAL_CTM_BUFFER_CREDITS            64

/* NBI pool per CTM */
#define NBI_PACKET_CREDITS              (TOTAL_CTM_PACKET_CREDITS/2)
#define NBI_BUFFER_CREDITS              (TOTAL_CTM_BUFFER_CREDITS/2)


/* ME pool per CTM */
#define ME_CTM_ALLOC_MAX_PKT_CREDITS    (TOTAL_CTM_PACKET_CREDITS - NBI_PACKET_CREDITS)
#define ME_CTM_ALLOC_MAX_BUF_CREDITS    (TOTAL_CTM_BUFFER_CREDITS - NBI_BUFFER_CREDITS)

/* Total number of worker islands and MEs */
#if (PLATFORM_PORT_SELECTION == H_PLATFORM)
    #define NUM_WORKER_ISLANDS          5
#else
    #define NUM_WORKER_ISLANDS          7
#endif

#define NUM_WORKER_ME                   12
#define NUM_REDUCED_WORKER_ME           8       // i36 has less worker MEs


/* i32..i38 use all 12 MEs for workers except i36 which only use 8 */
#define NBI_PACKET_CREDITS_REDUCED \
                (((NBI_PACKET_CREDITS)*NUM_REDUCED_WORKER_ME)/NUM_WORKER_ME)
#define NBI_BUFFER_CREDITS_REDUCED \
                    (((NBI_BUFFER_CREDITS)*NUM_REDUCED_WORKER_ME)/NUM_WORKER_ME)


/* i32 to i36 */
#define NBI0_DMA_BPE_CONFIG_ME_ISLAND0   1,\
                                        NBI_PACKET_CREDITS, \
                                        (NBI_BUFFER_CREDITS-1)
#define NBI0_DMA_BPE_CONFIG_ME_ISLAND1   1,\
                                        NBI_PACKET_CREDITS, \
                                        (NBI_BUFFER_CREDITS-1)
#define NBI0_DMA_BPE_CONFIG_ME_ISLAND2   1,\
                                        NBI_PACKET_CREDITS, \
                                        (NBI_BUFFER_CREDITS-1)
#define NBI0_DMA_BPE_CONFIG_ME_ISLAND3   1,\
                                        NBI_PACKET_CREDITS, \
                                        (NBI_BUFFER_CREDITS-1)
#define NBI0_DMA_BPE_CONFIG_ME_ISLAND4   1,\
                                        NBI_PACKET_CREDITS_REDUCED, \
                                        (NBI_BUFFER_CREDITS_REDUCED-1)

/* Island 37 and i38, only for some cards */
#if (PLATFORM_PORT_SELECTION == H_PLATFORM)
    #define NBI0_DMA_BPE_CONFIG_ME_ISLAND5  0,0,0
    #define NBI0_DMA_BPE_CONFIG_ME_ISLAND6  0,0,0
#else
    #define NBI0_DMA_BPE_CONFIG_ME_ISLAND5   1,\
                                            NBI_PACKET_CREDITS, \
                                            (NBI_BUFFER_CREDITS-1)
    #define NBI0_DMA_BPE_CONFIG_ME_ISLAND6   1,\
                                            NBI_PACKET_CREDITS, \
                                            (NBI_BUFFER_CREDITS-1)
#endif



#define NBI1_DMA_BPE_CONFIG_ME_ISLAND0   0,0,0
#define NBI1_DMA_BPE_CONFIG_ME_ISLAND1   0,0,0
#define NBI1_DMA_BPE_CONFIG_ME_ISLAND2   0,0,0
#define NBI1_DMA_BPE_CONFIG_ME_ISLAND3   0,0,0
#define NBI1_DMA_BPE_CONFIG_ME_ISLAND4   0,0,0
#define NBI1_DMA_BPE_CONFIG_ME_ISLAND5   0,0,0
#define NBI1_DMA_BPE_CONFIG_ME_ISLAND6   0,0,0


#define NBI_DMA_BP0_BLQ_TARGET   0,0
#define NBI_DMA_BP1_BLQ_TARGET   0,0
#define NBI_DMA_BP2_BLQ_TARGET   0,0
#define NBI_DMA_BP3_BLQ_TARGET   0,0
#define NBI_DMA_BP4_BLQ_TARGET   0,0
#define NBI_DMA_BP5_BLQ_TARGET   0,0
#define NBI_DMA_BP6_BLQ_TARGET   0,0
#define NBI_DMA_BP7_BLQ_TARGET   0,0

/*
 * NFD configuration - calculate the MAX NFD credit cache size per ME
 */
#define _ISL_RX(x,y,z)          x
#define ISL_RX(x)               _ISL_RX(x)


#define NFD_QUEUE_CREDITS       256
#define ME_PER_ISL              12
#define NUM_RX_ISL              (ISL_RX(NBI0_DMA_BPE_CONFIG_ME_ISLAND0) + \
                                 ISL_RX(NBI0_DMA_BPE_CONFIG_ME_ISLAND1) + \
                                 ISL_RX(NBI0_DMA_BPE_CONFIG_ME_ISLAND2) + \
                                 ISL_RX(NBI0_DMA_BPE_CONFIG_ME_ISLAND3) + \
                                 ISL_RX(NBI0_DMA_BPE_CONFIG_ME_ISLAND4) + \
                                 ISL_RX(NBI0_DMA_BPE_CONFIG_ME_ISLAND5) + \
                                 ISL_RX(NBI0_DMA_BPE_CONFIG_ME_ISLAND6))

#define CREDIT_BATCH            (NFD_QUEUE_CREDITS / (NUM_RX_ISL * ME_PER_ISL))

#define NFD_VNIC_VF





/*
 * Flowcache config
 */
#ifndef NUM_IMU
#define FC_LB_IMU 1
#else
#define FC_LB_IMU NUM_IMU
#endif

#ifndef NUM_EMU
#define FC_LB_EMU 2
#else
#define FC_LB_EMU NUM_EMU
#endif

#define FC_ENABLE_OVERFLOW

/*
 * BLM configuration
 */
#define BLM_CUSTOM_CONFIG
#define BLM_BLQ_EMEM_TYPE emem

/*
 * GRO configuration
 */

#define GRO_NUM_BLOCKS          2
#define GRO_REDUCED             0
#define GRO_ISL                 48
#define GRO_EMEM_ISL            24

/* Configure which Traffic Manager sequencer GRO uses for NBI egress. */
#define GRO0_NBI_SEQR           1
#define GRO_NBI0                0

/*
 * App master configuration
 */
#define APP_MASTER              ENABLE_CHANNEL_STATS

/* Enable debug counters */
#define CFG_NIC_LIB_DBG_CNTRS

/* For the VF specialisation */
#define NIC_INTF                0


#if defined(__NFP_LANG_MICROC)
struct port_accumulate_octets {
    unsigned long long rx_discards;
    unsigned long long rx_errors;
    unsigned long long rx_uc_octets;
    unsigned long long rx_mc_octets;
    unsigned long long rx_bc_octets;
    unsigned long long tx_discards;
    unsigned long long tx_errors;
    unsigned long long tx_uc_octets;
    unsigned long long tx_mc_octets;
    unsigned long long tx_bc_octets;
};
#endif

/* The missing stats in the MAC stats - must be accumulated by the app */
//__import __shared __emem struct port_accumulate_octets port_stats_extra[2][24] = {0};

#define PORT_STATS_INCR_IND(_pci, _port, _cntr) \
        mem_incr64(&port_stats_extra[_pci][_port].##_cntr)
#define PORT_STATS_INCR(_port, _cntr) \
    PORT_STATS_INCR_IND(NIC_PCI, _port, _cntr)

#define PORT_STATS_ADD_IND(_pci, _port, _cntr, _value) \
        mem_add64_imm(_value, &port_stats_extra[_pci][_port].##_cntr)
#define PORT_STATS_ADD(_port, _cntr, _value) \
    PORT_STATS_ADD_IND(NIC_PCI, _port, _cntr, _value)

#endif /* __APP_CONFIG_H__ */
