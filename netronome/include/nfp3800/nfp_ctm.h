/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP3800_CTM_H
#define NFP3800_CTM_H

/*================================================================*/
/* TGT 7                                                          */

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + NFP3800_CTM_MEM */
#define NFP3800_CTM_MEM                                          (0x000000)

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP3800_XPB_ISLAND(island))          */

#define CTMX_BASE                                            (0x60000)

/* XPB_BASE + NFP3800_CTMX_CFG + NFP3800_CTMX_CFG_* */
#define NFP3800_CTMX_CFG                                         (CTMX_BASE + 0x000000)
/* XPB_BASE + NFP3800_CTMX_PKT + NFP3800_CTMX_PKT_* */
#define NFP3800_CTMX_PKT                                         (CTMX_BASE + 0x010000)
/* XPB_BASE + NFP3800_CTMX_MISC + NFP3800_CTMX_MISC_* */
#define NFP3800_CTMX_MISC                                        (CTMX_BASE + 0x020000)
/* XPB_BASE + NFP3800_CTMX_DCACHE_ECC + (nfp_ecc.h) */
#define NFP3800_CTMX_DCACHE_ECC                                  (CTMX_BASE + 0x030000)

/*================================================================*/


/* HGID: c3e5162fe280 */

#ifndef NFP3800_LONGNAMES

/*----------------------------------------------------------------
  Register: ConfigCPP - CPP MU Configuration Register
    Bits: 16    DisableTagPassingBulkEngine(rw) - This flag is used to disable tag passing completely in Bulk engine. All Tags would be requested and released.
    Bits: 15:8  DirAccWays(rw) - When bit n in this field is set to 1, permit MU commands to direct access that way of the cache; when low, address bits [39:38] determine locality. If all 1s then all MU commands, with the exception of the Queue Engine commands highlighted in the Note below, are forced to be direct access. Note: This applies only the External Memory Units, all other Memory Units are direct access only
    Bits:  7    DisableFixOnPartialTagPassingBulkEngine(rw) - This bit is set to Disable the fix which was put in for Partial Tag-passing. If this bit is set old mode of operation would take into effect for Bulk engine Tag-passing.
    Bits:  6    EnableBulkByteMaskSwap(rw) - Enable Bulk ByteMask Swap
    Bits:  5    CPPErrorSuppress(rw) - If set, data errors generated for outbound CPP data buses will be suppressed internally and will not be sent out on the bus. CPP targets will suppress Push bus errors, CPP masters will suppress Pull bus errors.
    Bits:  4    CPPErrorIgnore(rw) - If set, data errors received on inbound CPP data buses will be ignored internally. CPP targets will ignore Pull bus errors, CPP masters will ignore Push bus errors.
    Bits:  3    DisableDualChRead(rw) - Controls the use of the CTM Bulk Engine performance feature to use both DCache channels for parallel non-dependent reads if the ordered commands are blocked/empty (CTM Only).
    Bits:  2:1  Addr32Bit(rw) - Select mode of addressing. Note that only Bulk, Atomic and Queue operations are supported in 32-bit modes.
    Bits:  0    IgnBulkAlign(rw) - If set, then CPP address[3;0] are ignored for bulk DRAM access, as with the IXP. If clear, then CPP address[3;0] are used, and unaligned accesses occur if they are not zero.
*/
#define NFP3800_CTMX_CFG_CFG                                 0x00000000
#define   NFP3800_CTMX_CFG_CFG_DISABLE_TAG_PASSING_BULK_ENGINE BIT(16)
#define   NFP3800_CTMX_CFG_CFG_WAYS(_x)                      (((_x) & 0xff) << 8)
#define   NFP3800_CTMX_CFG_CFG_WAYS_of(_x)                   (((_x) >> 8) & 0xff)
#define   NFP3800_CTMX_CFG_CFG_DISABLE_FIX_ON_PARTIAL_TAG_PASSING_BULK_ENGINE BIT(7)
#define   NFP3800_CTMX_CFG_CFG_ENABLE_BULK_BYTEMASK_SWAP     BIT(6)
#define   NFP3800_CTMX_CFG_CFG_ERROR_SUPPRESS                BIT(5)
#define   NFP3800_CTMX_CFG_CFG_ERROR_IGNORE                  BIT(4)
#define   NFP3800_CTMX_CFG_CFG_DISABLE_DUAL_READ             BIT(3)
#define     NFP3800_CTMX_CFG_CFG_DISABLE_DUAL_READ_ENABLED   (0 << 3)
#define     NFP3800_CTMX_CFG_CFG_DISABLE_DUAL_READ_DISABLED  BIT(3)
#define   NFP3800_CTMX_CFG_CFG_ADDRESSING(_x)                (((_x) & 0x3) << 1)
#define   NFP3800_CTMX_CFG_CFG_ADDRESSING_of(_x)             (((_x) >> 1) & 0x3)
#define     NFP3800_CTMX_CFG_CFG_ADDRESSING_40BIT            (0)
#define     NFP3800_CTMX_CFG_CFG_ADDRESSING_32BIT            (1)
#define     NFP3800_CTMX_CFG_CFG_ADDRESSING_32BITHLR         (2)
#define     NFP3800_CTMX_CFG_CFG_ADDRESSING_32BITLLR         (3)
#define   NFP3800_CTMX_CFG_CFG_BULK_ALIGN                    BIT(0)
#define     NFP3800_CTMX_CFG_CFG_BULK_ALIGN_ENFORCE          (0 << 0)
#define     NFP3800_CTMX_CFG_CFG_BULK_ALIGN_IGNORE           BIT(0)
/*----------------------------------------------------------------
  Register: ConfigVQDR[4] - Virtual QDR X Configuration Register
    Bits: 21:20 QueueType(rw) - Type of queues in the channel, similar to IXP IgnoreEOP and IgnoreSegCnt
    Bits: 15:0  MemWinBase(rw) - Bits [16;24] of full 40-bit MU address space for the base of the virtual QDR channel
*/
#define NFP3800_CTMX_CFG_VQDR(_x)                            (0x00000004 + (0x4 * ((_x) & 0x3)))
#define   NFP3800_CTMX_CFG_VQDR_QUEUE_TYPE(_x)               (((_x) & 0x3) << 20)
#define   NFP3800_CTMX_CFG_VQDR_QUEUE_TYPE_of(_x)            (((_x) >> 20) & 0x3)
#define     NFP3800_CTMX_CFG_VQDR_QUEUE_TYPE_SEGMENTS        (0)
#define     NFP3800_CTMX_CFG_VQDR_QUEUE_TYPE_PACKETS         (1)
#define     NFP3800_CTMX_CFG_VQDR_QUEUE_TYPE_PACKETS32       (2)
#define     NFP3800_CTMX_CFG_VQDR_QUEUE_TYPE_BUFFERS         (3)
#define   NFP3800_CTMX_CFG_VQDR_BASE(_x)                     (((_x) & 0xffff) << 0)
#define   NFP3800_CTMX_CFG_VQDR_BASE_of(_x)                  (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: MUConfigBeSdet - Bulk Engine starvation Detection Control Register
*/
#define NFP3800_CTMX_CFG_MU_CONFIG_BE_SDET                   0x00000014
/*----------------------------------------------------------------
  Register: PushLoadBalancer - MU DSF Push Load Balancer Register
  Register: BulkPushLoadBalancer[2] - MU DSF Push Load Balancer Register for Bulk Engine Channel X
    Bits: 31:24 MapIfMaskMatchFails(wo) - Indexed by master_id[3;0] - result is the load balance answer if mask/match fails
    Bits: 23:16 MapIfMaskMatchPasses(wo) - Indexed by master_id[3;0] - result is the load balance answer if mask/match passes
    Bits: 15:8  ConfigMask(wo) - ANDed with island_id[6;0],master_id[2;2], and compare with mask&match - also if match&~mask then at least one must be set
    Bits:  7:0  ConfigMatch(wo) - Combined with match and island_id and top 2 bits of master_id
*/
#define NFP3800_CTMX_CFG_PUSH_LOAD_BALANCER                  0x00000800
#define NFP3800_CTMX_CFG_BE_PUSH_LOAD_BALANCER(_x)           (0x00001c10 + (0x4 * ((_x) & 0x1)))
#define   NFP3800_CTMX_CFG_DSFCPPLOADBALANCERCONFIG_MAP_IF_MASK_MATCH_FAILS(_x) (((_x) & 0xff) << 24)
#define   NFP3800_CTMX_CFG_DSFCPPLOADBALANCERCONFIG_MAP_IF_MASK_MATCH_FAILS_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_CTMX_CFG_DSFCPPLOADBALANCERCONFIG_MAP_IF_MASK_MATCH_PASSES(_x) (((_x) & 0xff) << 16)
#define   NFP3800_CTMX_CFG_DSFCPPLOADBALANCERCONFIG_MAP_IF_MASK_MATCH_PASSES_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_CTMX_CFG_DSFCPPLOADBALANCERCONFIG_CONFIG_MASK(_x) (((_x) & 0xff) << 8)
#define   NFP3800_CTMX_CFG_DSFCPPLOADBALANCERCONFIG_CONFIG_MASK_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_CTMX_CFG_DSFCPPLOADBALANCERCONFIG_CONFIG_MATCH(_x) (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_CFG_DSFCPPLOADBALANCERCONFIG_CONFIG_MATCH_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: XpbTimeout - XPB Timeout
    Bits: 31:0  XpbTimeoutData(rw) - XPB timeout data.
*/
#define NFP3800_CTMX_CFG_XPB_TIMEOUT                         0x00000c00
#define   NFP3800_CTMX_CFG_XPB_TIMEOUT_XPB_TIMEOUT_DATA(_x)  (((_x) & 0xffffffff) << 0)
#define   NFP3800_CTMX_CFG_XPB_TIMEOUT_XPB_TIMEOUT_DATA_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: QueueEngineReg - Queue Engine Error reset and Configurable events
    Bits: 31    ErrorReset(rw) - When set to 1, causes Queue engine to clear its error bit
    Bits: 30    ErrorIndicator(ro) - when set to 1, indicates that all the Queue operations on any of the QDescriptor contains multi-bit error
    Bits:  6    EnableDropBadPushIds(rw) - When this bit is set, push commands that get their IDs from DCache will be silently dropped If the Dcache read had an un-correctablr ECC error.
    Bits:  5    EnableErrorHandling(rw) - When this bit is set, Error-handling becomes active. i.e. from now on if any Qdescriptor gets error based on ReadQDescriptor then any command working on any of the Qdescriptor would be considered to be Error'd
    Bits:  4:0  ConfigurableWatermark(rw) - Legal values are from 0->19, when set to 0 event is raised when count goes above/below pow(2,(4+configurable_watermark)) value. If value greater than 19 is specified then RTL would default it to value of 0.
*/
#define NFP3800_CTMX_CFG_QUEUE_ENGINE_REG0                   0x00002400
#define   NFP3800_CTMX_CFG_QUEUE_ENGINE_REG0_ERRORRESET      BIT(31)
#define   NFP3800_CTMX_CFG_QUEUE_ENGINE_REG0_ERRORINDICATOR  BIT(30)
#define   NFP3800_CTMX_CFG_QUEUE_ENGINE_REG0_ENABLE_DROP_BAD_PUSH_IDS BIT(6)
#define   NFP3800_CTMX_CFG_QUEUE_ENGINE_REG0_ENABLE_ERROR_HANDLING BIT(5)
#define   NFP3800_CTMX_CFG_QUEUE_ENGINE_REG0_CONFIGURABLEWATERMARK(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_CTMX_CFG_QUEUE_ENGINE_REG0_CONFIGURABLEWATERMARK_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: AtomicEngineReg - Atomic engine, Enable shifting of Immediate data for ALU operations
    Bits:  0    AtomicEngineReg(rw) - Atomic engine, Enable shifting of Immediate data for ALU operations.
*/
#define NFP3800_CTMX_CFG_ATOMIC_ENGINE_REG0                  0x00002404
#define   NFP3800_CTMX_CFG_ATOMIC_ENGINE_REG0_ATOMIC_ENGINE_REG BIT(0)
/*----------------------------------------------------------------
  Register: MUPEMemConfig - MU PE Memory Configuration
    Bits:  2:0  MUPEMemConfig(rw) - Packet Mem Config. How much of the CTM memory can be used to store packets
*/
#define NFP3800_CTMX_PKT_MU_PE_MEM_CONFIG                    0x00000000
#define   NFP3800_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG(_x) (((_x) & 0x7) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_MEM_CONFIG_8_OF_8_PARTS (0)
#define     NFP3800_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_MEM_CONFIG_7_OF_8_PARTS (1)
#define     NFP3800_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_MEM_CONFIG_6_OF_8_PARTS (2)
#define     NFP3800_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_MEM_CONFIG_5_OF_8_PARTS (3)
#define     NFP3800_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_MEM_CONFIG_4_OF_8_PARTS (4)
#define     NFP3800_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_MEM_CONFIG_3_OF_8_PARTS (5)
#define     NFP3800_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_MEM_CONFIG_2_OF_8_PARTS (6)
#define     NFP3800_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_MEM_CONFIG_1_OF_8_PARTS (7)
/*----------------------------------------------------------------
  Register: MUPEWQConfig - MU PE Work Queue Config
    Bits:  1:0  MUPEWQConfig(rw) - Work Queue Configuration
*/
#define NFP3800_CTMX_PKT_MU_PE_WQ_CONFIG                     0x00000004
#define   NFP3800_CTMX_PKT_MU_PE_WQ_CONFIG_MU_PE_WQ_CONFIG(_x) (((_x) & 0x3) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_WQ_CONFIG_MU_PE_WQ_CONFIG_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_CTMX_PKT_MU_PE_WQ_CONFIG_MU_PE_WQ_CONFIG_SENDONLAST (0)
#define     NFP3800_CTMX_PKT_MU_PE_WQ_CONFIG_MU_PE_WQ_CONFIG_SENDONFIRST (1)
#define     NFP3800_CTMX_PKT_MU_PE_WQ_CONFIG_MU_PE_WQ_CONFIG_SENDONBOTH (2)
/*----------------------------------------------------------------
  Register: MUPEBufferConfig - MU PE Buffer Configuration
    Bits:  1:0  MUPEBufferConfig(rw) - Buffer Config
*/
#define NFP3800_CTMX_PKT_MU_PE_BUFFER_CONFIG                 0x00000008
#define   NFP3800_CTMX_PKT_MU_PE_BUFFER_CONFIG_MU_PE_BUFFER_CONFIG(_x) (((_x) & 0x3) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_BUFFER_CONFIG_MU_PE_BUFFER_CONFIG_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_CTMX_PKT_MU_PE_BUFFER_CONFIG_MU_PE_BUFFER_CONFIG_BUFFER_CONFIG_2KB (0)
#define     NFP3800_CTMX_PKT_MU_PE_BUFFER_CONFIG_MU_PE_BUFFER_CONFIG_BUFFER_CONFIG_4KB (1)
#define     NFP3800_CTMX_PKT_MU_PE_BUFFER_CONFIG_MU_PE_BUFFER_CONFIG_BUFFER_CONFIG_8KB (2)
#define     NFP3800_CTMX_PKT_MU_PE_BUFFER_CONFIG_MU_PE_BUFFER_CONFIG_BUFFER_CONFIG_16KB (3)
/*----------------------------------------------------------------
  Register: MUPEAllowNBIPushToCombineTo16B - Allow NBI Push to Combine to 16B
    Bits:  0    MUPEAllowNBIPushToCombineTo16B(rw) - Allow NBI Push to combine to 16B
*/
#define NFP3800_CTMX_PKT_MU_PE_ALLOW_NBI_PUSH_TO_COMBINE_TO_16B 0x0000000c
#define   NFP3800_CTMX_PKT_MU_PE_ALLOW_NBI_PUSH_TO_COMBINE_TO_16B_MU_PE_ALLOW_NBI_PUSH_TO_COMBINE_TO_16B BIT(0)
/*----------------------------------------------------------------
  Register: MUPEAllowDMAPushToCombineTo16B - Allow DMA Push to Combine to 16B
    Bits:  0    MUPEAllowDMAPushToCombineTo16B(rw) - Allow DMA Push to combine to 16B
*/
#define NFP3800_CTMX_PKT_MU_PE_ALLOW_DMA_PUSH_TO_COMBINE_TO_16B 0x00000010
#define   NFP3800_CTMX_PKT_MU_PE_ALLOW_DMA_PUSH_TO_COMBINE_TO_16B_MU_PE_ALLOW_DMA_PUSH_TO_COMBINE_TO_16B BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDMAOverflowIntoDCacheEnabled - Allow DMA Commands to be stored into the DCache
    Bits:  0    MUPEDMAOverflowIntoDCacheEnabled(rw) - Allow DMA Commands to be stored into the DCache
*/
#define NFP3800_CTMX_PKT_MU_PE_DMA_OVERFLOW_INTO_DCACHE_ENABLED 0x00000014
#define   NFP3800_CTMX_PKT_MU_PE_DMA_OVERFLOW_INTO_DCACHE_ENABLED_MU_PE_DMA_OVERFLOW_INTO_DCACHE_ENABLED BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDMAStorageSize - DMA Storage Size
    Bits:  1:0  MUPEDMAStorageSize(rw) - DMA Storage Size
*/
#define NFP3800_CTMX_PKT_MU_PE_DMA_STORAGE_SIZE              0x00000018
#define   NFP3800_CTMX_PKT_MU_PE_DMA_STORAGE_SIZE_MU_PE_DMA_STORAGE_SIZE(_x) (((_x) & 0x3) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_DMA_STORAGE_SIZE_MU_PE_DMA_STORAGE_SIZE_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_STORAGE_SIZE_MU_PE_DMA_STORAGE_SIZE_DMA_STORAGE_FOR_1024_DESCRIPTORS (0)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_STORAGE_SIZE_MU_PE_DMA_STORAGE_SIZE_DMA_STORAGE_FOR_512_DESCRIPTORS (1)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_STORAGE_SIZE_MU_PE_DMA_STORAGE_SIZE_DMA_STORAGE_FOR_256_DESCRIPTORS (2)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_STORAGE_SIZE_MU_PE_DMA_STORAGE_SIZE_DMA_STORAGE_FOR_128_DESCRIPTORS (3)
/*----------------------------------------------------------------
  Register: MUPEDMAStorageBaseAddress - DMA Storage Base Address
    Bits: 16:0  MUPEDMAStorageBaseAddress(rw) - DMA Storage Base Address
*/
#define NFP3800_CTMX_PKT_MU_PE_DMA_STORAGE_BASE_ADDRESS      0x0000001c
#define   NFP3800_CTMX_PKT_MU_PE_DMA_STORAGE_BASE_ADDRESS_MU_PE_DMA_STORAGE_BASE_ADDRESS(_x) (((_x) & 0x1ffff) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_DMA_STORAGE_BASE_ADDRESS_MU_PE_DMA_STORAGE_BASE_ADDRESS_of(_x) (((_x) >> 0) & 0x1ffff)
/*----------------------------------------------------------------
  Register: MUPEDMAMaxOutstandingMem - DMA Outstanding Rd/Write Commands
    Bits:  2:0  MUPEDMAMaxOutstandingMem(rw) - DMA Outstanding Bulk Read/Write Commands
*/
#define NFP3800_CTMX_PKT_MU_PE_DMA_MAX_OUTSTANDING_MEM       0x00000020
#define   NFP3800_CTMX_PKT_MU_PE_DMA_MAX_OUTSTANDING_MEM_MU_PE_DMA_MAX_OUTSTANDING_MEM(_x) (((_x) & 0x7) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_DMA_MAX_OUTSTANDING_MEM_MU_PE_DMA_MAX_OUTSTANDING_MEM_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_MAX_OUTSTANDING_MEM_MU_PE_DMA_MAX_OUTSTANDING_MEM_32_OUSTANDINGS (0)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_MAX_OUTSTANDING_MEM_MU_PE_DMA_MAX_OUTSTANDING_MEM_64_OUSTANDING (1)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_MAX_OUTSTANDING_MEM_MU_PE_DMA_MAX_OUTSTANDING_MEM_128_OUSTANDING (2)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_MAX_OUTSTANDING_MEM_MU_PE_DMA_MAX_OUTSTANDING_MEM_256_OUSTANDING (3)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_MAX_OUTSTANDING_MEM_MU_PE_DMA_MAX_OUTSTANDING_MEM_512_OUSTANDINGS (4)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_MAX_OUTSTANDING_MEM_MU_PE_DMA_MAX_OUTSTANDING_MEM_1024_OUSTANDING (6)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_MAX_OUTSTANDING_MEM_MU_PE_DMA_MAX_OUTSTANDING_MEM_4096_OUSTANDING (7)
/*----------------------------------------------------------------
  Register: MUPEDMAHysteresis - Empty Slots that need to be avail in the DMA RH before reading new cmds from DCache
    Bits:  5:0  MUPEDMAHysteresis(rw) - DMA Outstanding Bulk Read/Write Commands
*/
#define NFP3800_CTMX_PKT_MU_PE_DMA_HYSTERESIS                0x00000024
#define   NFP3800_CTMX_PKT_MU_PE_DMA_HYSTERESIS_MU_PE_DMA_HYSTERESIS(_x) (((_x) & 0x3f) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_DMA_HYSTERESIS_MU_PE_DMA_HYSTERESIS_of(_x) (((_x) >> 0) & 0x3f)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_HYSTERESIS_MU_PE_DMA_HYSTERESIS_32 (32)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_HYSTERESIS_MU_PE_DMA_HYSTERESIS_4_ (4)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_HYSTERESIS_MU_PE_DMA_HYSTERESIS_8_ (8)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_HYSTERESIS_MU_PE_DMA_HYSTERESIS_12 (12)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_HYSTERESIS_MU_PE_DMA_HYSTERESIS_16 (16)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_HYSTERESIS_MU_PE_DMA_HYSTERESIS_20 (20)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_HYSTERESIS_MU_PE_DMA_HYSTERESIS_24 (24)
#define     NFP3800_CTMX_PKT_MU_PE_DMA_HYSTERESIS_MU_PE_DMA_HYSTERESIS_28 (28)
/*----------------------------------------------------------------
  Register: MUPEStopPQ0Dequeue - Set before starting WQ Flush. Setting this bit prevents the PQ from draining
    Bits:  0    MUPEStopPQ0Dequeue(rw) - Set before starting WQ Flush. Setting this bit will prevent the Packet Queue from draining.
*/
#define NFP3800_CTMX_PKT_MU_PE_STOP_PQ0_DEQUEUE              0x00000028
#define   NFP3800_CTMX_PKT_MU_PE_STOP_PQ0_DEQUEUE_MU_PE_STOP_PQ0_DEQUEUE BIT(0)
/*----------------------------------------------------------------
  Register: MUPEStopWQDequeue - Set before starting PQ0 Flush. Setting this bit prevents the WQ from draining
    Bits:  0    MUPEStopWQDequeue(rw) - Set before starting PQ0 Flush. Setting this bit will prevent the Work Queue from draining.
*/
#define NFP3800_CTMX_PKT_MU_PE_STOP_WQ_DEQUEUE               0x0000002c
#define   NFP3800_CTMX_PKT_MU_PE_STOP_WQ_DEQUEUE_MU_PE_STOP_WQ_DEQUEUE BIT(0)
/*----------------------------------------------------------------
  Register: MUPEGrantCycles - This reg controls how many cycles each requestor gets access to the DCache port for
    Bits:  0    MUPEGrantCycles(rw) - This register controls how many cycles each requestor gets access to the DCache port for.
*/
#define NFP3800_CTMX_PKT_MU_PE_GRANT_CYCLES                  0x00000030
#define   NFP3800_CTMX_PKT_MU_PE_GRANT_CYCLES_MU_PE_GRANT_CYCLES BIT(0)
#define     NFP3800_CTMX_PKT_MU_PE_GRANT_CYCLES_MU_PE_GRANT_CYCLES_8_ (0 << 0)
#define     NFP3800_CTMX_PKT_MU_PE_GRANT_CYCLES_MU_PE_GRANT_CYCLES_16_ BIT(0)
/*----------------------------------------------------------------
  Register: MUPEOverridePCIeLength - This reg is used to override the length from the add_thread command for PCIe packets
    Bits: 31    MUPEOverridePCIeLengthEnable(rw) - This register is used to enable overriding the length from the add_thread command for PCIe allocated packets.
    Bits: 14:0  MUPEOverridePCIeLength(rw) - This register is used to override the length from the add_thread command for PCIe allocated packets. This is a Byte aligned field, however length must be a 4B aligned field. Rules for AddThread must be adhered to while programming this field.
*/
#define NFP3800_CTMX_PKT_MU_PE_OVERRIDE_PCIE_LENGTH          0x00000034
#define   NFP3800_CTMX_PKT_MU_PE_OVERRIDE_PCIE_LENGTH_MU_PE_OVERRIDE_PCIE_LENGTH_ENABLE BIT(31)
#define   NFP3800_CTMX_PKT_MU_PE_OVERRIDE_PCIE_LENGTH_MU_PE_OVERRIDE_PCIE_LENGTH(_x) (((_x) & 0x7fff) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_OVERRIDE_PCIE_LENGTH_MU_PE_OVERRIDE_PCIE_LENGTH_of(_x) (((_x) >> 0) & 0x7fff)
/*----------------------------------------------------------------
  Register: MUPEOverridePCIeOffset - This reg is used to override the offset from the add_thread command for PCIe packets
    Bits: 31    MUPEOverridePCIeOffsetEnable(rw) - This register is used to enable overriding the offset from the add_thread command for PCIe allocated packets. This is a Byte aligned field. Rules for AddThread must be adhered to while programming this field.
    Bits: 14:0  MUPEOverridePCIeOffset(rw) - This register is used to override the offset from the add_thread command for PCIe allocated packets. This is a Byte aligned field. Rules for AddThread must be adhered to while programming this field.
*/
#define NFP3800_CTMX_PKT_MU_PE_OVERRIDE_PCIE_OFFSET          0x00000038
#define   NFP3800_CTMX_PKT_MU_PE_OVERRIDE_PCIE_OFFSET_MU_PE_OVERRIDE_PCIE_OFFSET_ENABLE BIT(31)
#define   NFP3800_CTMX_PKT_MU_PE_OVERRIDE_PCIE_OFFSET_MU_PE_OVERRIDE_PCIE_OFFSET(_x) (((_x) & 0x7fff) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_OVERRIDE_PCIE_OFFSET_MU_PE_OVERRIDE_PCIE_OFFSET_of(_x) (((_x) >> 0) & 0x7fff)
/*----------------------------------------------------------------
  Register: MUPEMultiCastEnable - Enable Hardware Support for Multicast
    Bits:  0    MUPEMultiCastEnable(rw) - Enable Hardware Support for Multicast
*/
#define NFP3800_CTMX_PKT_MU_PE_MULTICAST_ENABLE              0x0000003c
#define   NFP3800_CTMX_PKT_MU_PE_MULTICAST_ENABLE_MU_PE_MULTICAST_ENABLE BIT(0)
/*----------------------------------------------------------------
  Register: MUPEMultiCastFingerprintEnable - FingerprintEnable Hardware Support for Multicast
    Bits:  0    MUPEMultiCastFingerprintEnable(rw) - Hardware Support for Multicast with Fingerprint written by ME
*/
#define NFP3800_CTMX_PKT_MU_PE_MULTICAST_FINGERPRINT_ENABLE  0x00000040
#define   NFP3800_CTMX_PKT_MU_PE_MULTICAST_FINGERPRINT_ENABLE_MU_PE_MULTICAST_FINGERPRINT_ENABLE BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDropAwareModeEnable - Enable DropAwareMode (all dropped packets except multicast dropped packets)
    Bits:  0    MUPEDropAwareModeEnable(rw) - Enable DropAwareMode (all dropped packets except multicast dropped packets)
*/
#define NFP3800_CTMX_PKT_MU_PE_DROP_AWARE_MODE_ENABLE        0x00000044
#define   NFP3800_CTMX_PKT_MU_PE_DROP_AWARE_MODE_ENABLE_MU_PE_DROP_AWARE_MODE_ENABLE BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDropRule[3] - Drop Rule X
    Bits: 31    MUPEDropRuleEnable(rw) - Enable this rule
    Bits: 27    MUPEDropRuleFreePacket(rw) - If masked packet data at starting index matches this rule, then free the packet only if this bit is set
    Bits: 26    MUPEDropRuleAddtoPacketQueue(rw) - If masked packet data at starting index matches this rule, then add packet back to PacketQueue only if this bit is set
    Bits: 25    MUPEDropRuleSendToRing1(rw) - If masked packet data at starting index matches this rule, then send information to Ring1 only if this bit is set
    Bits: 24    MUPEDropRuleSendToRing0(rw) - If masked packet data at starting index matches this rule, then send information to Ring0
    Bits: 19:16 MUPEDropRuleStartingIndex(rw) - Starting Index for Mask and Match Data
    Bits: 15:8  MUPEDropRuleMatch(rw) - Drop Match to match packet data
    Bits:  7:0  MUPEDropRuleMask(rw) - Drop Mask to mask packet data
*/
#define NFP3800_CTMX_PKT_MU_PE_DROP_RULE(_x)                 (0x00000048 + (0x4 * ((_x) & 0x3)))
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RULE_MU_PE_DROP_RULE_ENABLE BIT(31)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RULE_MU_PE_DROP_RULE_FREE_PACKET BIT(27)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RULE_MU_PE_DROP_RULE_ADD_TO_PACKET_QUEUE BIT(26)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RULE_MU_PE_DROP_RULE_SEND_TO_RING1 BIT(25)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RULE_MU_PE_DROP_RULE_SEND_TO_RING0 BIT(24)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RULE_MU_PE_DROP_RULE_STARTING_IDNEX(_x) (((_x) & 0xf) << 16)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RULE_MU_PE_DROP_RULE_STARTING_IDNEX_of(_x) (((_x) >> 16) & 0xf)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RULE_MU_PE_DROP_RULE_MATCH(_x) (((_x) & 0xff) << 8)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RULE_MU_PE_DROP_RULE_MATCH_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RULE_MU_PE_DROP_RULE_MASK(_x) (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RULE_MU_PE_DROP_RULE_MASK_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: MUPEDropRing[2] - Drop Ring X
    Bits: 31:28 MUPEDropRingNumber(rw) - Ring Number
    Bits: 25:24 MUPEDropRingDataSelect(rw) - Which 32 bits of data out of 128 bits of data should be sent to the Ring
    Bits: 23:16 MUPEDropRingIsland(rw) - Island Number to use while adding data to Ring
    Bits: 11:8  MUPEDropRingTarget(rw) - Target Encoding to use while adding data to Ring
    Bits:  6:5  MUPEDropRingToken(rw) - Token Encoding to use while adding data to Ring
    Bits:  4:0  MUPEDropRingAction(rw) - Action Encoding to use while adding data to Ring
*/
#define NFP3800_CTMX_PKT_MU_PE_DROP_RING(_x)                 (0x00000054 + (0x4 * ((_x) & 0x1)))
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RING_MU_PE_DROP_RING_NUMBER(_x) (((_x) & 0xf) << 28)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RING_MU_PE_DROP_RING_NUMBER_of(_x) (((_x) >> 28) & 0xf)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RING_MU_PE_DROP_RING_DATA_SELECT(_x) (((_x) & 0x3) << 24)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RING_MU_PE_DROP_RING_DATA_SELECT_of(_x) (((_x) >> 24) & 0x3)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RING_MU_PE_DROP_RING_ISLAND(_x) (((_x) & 0xff) << 16)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RING_MU_PE_DROP_RING_ISLAND_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RING_MU_PE_DROP_RING_TARGET(_x) (((_x) & 0xf) << 8)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RING_MU_PE_DROP_RING_TARGET_of(_x) (((_x) >> 8) & 0xf)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RING_MU_PE_DROP_RING_TOKEN(_x) (((_x) & 0x3) << 5)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RING_MU_PE_DROP_RING_TOKEN_of(_x) (((_x) >> 5) & 0x3)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RING_MU_PE_DROP_RING_ACTION(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_DROP_RING_MU_PE_DROP_RING_ACTION_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: MUPENullMUPtr - Null MUPtr. Should be configured to the same value as NBI DMA and NBI TM
    Bits: 28:0  MUPENullMUPtr(rw) - Null MUPtr. Should be configured to the same value as NBI DMA and NBI TM
*/
#define NFP3800_CTMX_PKT_MU_PE_NULL_MUPTR                    0x0000005c
#define   NFP3800_CTMX_PKT_MU_PE_NULL_MUPTR_MU_PE_NULL_MUPTR(_x) (((_x) & 0x1fffffff) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_NULL_MUPTR_MU_PE_NULL_MUPTR_of(_x) (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: MUPEEnableByteAlignedAddThread - Set to enable byte aligned add_thread commands
    Bits:  0    MUPEEnableByteAlignedAddThread(rw) - Set to enable byte aligned add_thread commands
*/
#define NFP3800_CTMX_PKT_MU_PE_BYTE_ALIGNED_ADD_THREAD       0x00000060
#define   NFP3800_CTMX_PKT_MU_PE_BYTE_ALIGNED_ADD_THREAD_MU_PE_ENABLE_BYTE_ALIGNED_ADD_THREAD BIT(0)
/*----------------------------------------------------------------
  Register: MUPEEnableDMAPoll - Set to enable polling of DMA Queue. If the Queue is full, the ME will be pushed back an error/retry response.
    Bits:  0    MUPEEnableDMAPoll(rw) - Set to enable polling of DMA Queue. If the Queue is full, the ME will be pushed back an error/retry response.
*/
#define NFP3800_CTMX_PKT_MU_PE_ENABLE_DMA_POLL               0x00000064
#define   NFP3800_CTMX_PKT_MU_PE_ENABLE_DMA_POLL_MU_PE_ENABLE_BYTE_ALIGNED_ADD_THREAD BIT(0)
/*----------------------------------------------------------------
  Register: MUPEEnableDMALengthAndOffset - Set to enable addnl length and offset inputs into DMATo/FromMemPacket and DMATo/FromMemIndirect Cmds.
    Bits:  0    MUPEEnableDMALengthAndOffset(rw) - Set to enable additional length and offset inputs into DMATo/FromMemPacket and DMATo/FromMemIndirect Commands.
*/
#define NFP3800_CTMX_PKT_MU_PE_ENABLE_DMA_LENGTH_AND_OFFSET  0x00000068
#define   NFP3800_CTMX_PKT_MU_PE_ENABLE_DMA_LENGTH_AND_OFFSET_MU_PE_ENABLE_BYTE_ALIGNED_ADD_THREAD BIT(0)
/*----------------------------------------------------------------
  Register: MUPEEnableMPullIDWithNoRead - Set to enable MpullIDs with NoRead. (Recommended for Performance)
    Bits:  0    MUPEEnableMPullIDWithNoRead(rw) - Set to enable MpullIDs with NoRead. (Recommended for Performance)
*/
#define NFP3800_CTMX_PKT_MU_PE_ENABLE_MPULL_ID_WITH_NO_READ  0x0000006c
#define   NFP3800_CTMX_PKT_MU_PE_ENABLE_MPULL_ID_WITH_NO_READ_MU_PE_ENABLE_MPULLID_WITH_NOREAD BIT(0)
/*----------------------------------------------------------------
  Register: MUPEEnableDMAReadback - Issue a single read after writes, to ensure that writes have completed in the Bulk Engine
    Bits:  0    MUPEEnableDMAReadback(rw) - Issue a single read after writes, to ensure that writes have completed in the Bulk Engine
*/
#define NFP3800_CTMX_PKT_MU_PE_ENABLE_DMA_READBACK           0x00000070
#define   NFP3800_CTMX_PKT_MU_PE_ENABLE_DMA_READBACK_MU_PE_ENABLE_DMA_READBACK BIT(0)
/*----------------------------------------------------------------
  Register: MUPEPacketLifeSpanPacketOwner - Packet Owner for the Packet Number whose lifespan we want to track. Program this prior to starting traffic, or wait sufficiently long for the update to take place.
    Bits:  2:0  MUPEPacketLifeSpanPacketOwner(rw) - Packet Owner for the Packet Number whose lifespan we want to track. Program this prior to starting traffic, or wait sufficiently long for the update to take place.
*/
#define NFP3800_CTMX_PKT_MU_PE_PACKET_LIFESPAN_PACKET_OWNER  0x00000074
#define   NFP3800_CTMX_PKT_MU_PE_PACKET_LIFESPAN_PACKET_OWNER_MU_PE_PACKET_LIFESPAN_PACKET_OWNER(_x) (((_x) & 0x7) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_PACKET_LIFESPAN_PACKET_OWNER_MU_PE_PACKET_LIFESPAN_PACKET_OWNER_of(_x) (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: MUPEPacketLifeSpanPacketNumber - Packet Number whose lifespan we want to track. Program this prior to starting traffic, or wait sufficiently long for the update to take place.
    Bits:  7:0  MUPEPacketLifeSpanPacketNumber(rw) - Packet Number whose lifespan we want to track. Program this prior to starting traffic, or wait sufficiently long for the update to take place.
*/
#define NFP3800_CTMX_PKT_MU_PE_PACKET_LIFESPAN_PACKET_NUMBER 0x00000078
#define   NFP3800_CTMX_PKT_MU_PE_PACKET_LIFESPAN_PACKET_NUMBER_MU_PE_PACKET_LIFESPAN_PACKET_NUMBER(_x) (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_PACKET_LIFESPAN_PACKET_NUMBER_MU_PE_PACKET_LIFESPAN_PACKET_NUMBER_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: MUPEConfiguredCredits[8] - MU PE Configured Credits
    Bits: 24:16 MUPEPacketCredits(rw) - Packet Credits (Max 512)
    Bits:  6:0  MUPEBufferCredits(rw) - Buffer Credits (Max 64)
*/
#define NFP3800_CTMX_PKT_MU_PE_CONFIGURED_CREDITS(_x)        (0x00000080 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_CTMX_PKT_MU_PE_CONFIGURED_CREDITS_MU_PE_PACKET_CREDITS(_x) (((_x) & 0x1ff) << 16)
#define   NFP3800_CTMX_PKT_MU_PE_CONFIGURED_CREDITS_MU_PE_PACKET_CREDITS_of(_x) (((_x) >> 16) & 0x1ff)
#define   NFP3800_CTMX_PKT_MU_PE_CONFIGURED_CREDITS_MU_PE_BUFFER_CREDITS(_x) (((_x) & 0x7f) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_CONFIGURED_CREDITS_MU_PE_BUFFER_CREDITS_of(_x) (((_x) >> 0) & 0x7f)
/*----------------------------------------------------------------
  Register: MUPEClearAllocPacketCount - LifeOfAPacket:(Clear) Allocated Packet Count
  Register: MUPEClearFirstSegmentRxPacketCount - LifeOfAPacket:(Clear) First Segment Received Count
  Register: MUPEClearLastSegmentRxPacketCount - LifeOfAPacket:(Clear) Last Segment Received Count
  Register: MUPEClearFirstSegmentSentToMEPacketCount - LifeOfAPacket:(Clear) First Segment Sent to ME Count
  Register: MUPEClearLastSegmentSentToMEPacketCount - LifeOfAPacket:(Clear) Last Segment Sent to ME Count
  Register: MUPEClearTxPacketCount - LifeOfAPacket:(Clear) Last PullID Received Count
  Register: MUPEClearFreePacketCount - LifeOfAPacket:(Clear) Free Packet Count
  Register: MUPEClearAddPacketTotalCount - (Clear) Total Number of packets added to PQ
  Register: MUPEClearCmdAddPacketTotalCount - (Clear) Total Number of packets added to PQ via AddPacket Cmd
  Register: MUPEClearMpushAddPacketTotalCount - (Clear) Total Number of packets added to PQ via MPush
    Bits: 31:0  MUPECountWithClear(rc) - 32 Bit Count that clears on read. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_ALLOC_PACKET_COUNT      0x00002800
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_FIRST_SEGMENT_RX_PACKET_COUNT 0x00002804
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_LAST_SEGMENT_RX_PACKET_COUNT 0x00002808
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_FIRST_SEGMENT_SENT_TO_ME_PACKET_COUNT 0x0000280c
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_LAST_SEGMENT_SENT_TO_ME_PACKET_COUNT 0x00002810
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_TX_PACKET_COUNT         0x00002814
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_FREE_PACKET_COUNT       0x00002818
#define NFP3800_CTMX_PKT_MU_PE_ADD_PACKET_TOTAL_COUNT        0x000028c0
#define NFP3800_CTMX_PKT_MU_PE_CMD_ADD_PACKET_TOTAL_COUNT    0x000028c4
#define NFP3800_CTMX_PKT_MU_PE_MPUSH_ADD_PACKET_TOTAL_COUNT  0x000028c8
#define   NFP3800_CTMX_PKT_MUPECOUNT32WITHCLEAR_MU_PE_COUNT_WITH_CLEAR(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_CTMX_PKT_MUPECOUNT32WITHCLEAR_MU_PE_COUNT_WITH_CLEAR_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MUPEActivePacketCount - Active Packet Count
  Register: MUPEActivePacketCountOwner[8] - Active Packet Count (Per Owner). Owner 3 maps to Owner 0.
  Register: MUPEActiveByteCount - Active Byte Count
  Register: MUPEActiveByteCountOwner[8] - Active Byte Count (Per Owner). Owner 3 maps to Owner 0.
  Register: MUPEPeakPacketCount - Peak Packet Count
  Register: MUPEPeakByteCount - Peak Byte Count
  Register: MUPEDMAsAvailableInDcacheCount - Number of DMA descriptors Available in DCache
    Bits: 31:0  MUPECount(ro) - 11 Bit Count. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MU_PE_ACTIVE_PACKET_COUNT           0x0000281c
#define NFP3800_CTMX_PKT_MU_PE_ACTIVE_PACKET_COUNT_OWNER(_x) (0x00002820 + (0x4 * ((_x) & 0x7)))
#define NFP3800_CTMX_PKT_MU_PE_ACTIVE_BYTE_COUNT             0x00002840
#define NFP3800_CTMX_PKT_MU_PE_ACTIVE_BYTE_COUNT_OWNER(_x)   (0x00002844 + (0x4 * ((_x) & 0x7)))
#define NFP3800_CTMX_PKT_MU_PE_PEAK_PACKET_COUNT             0x00002864
#define NFP3800_CTMX_PKT_MU_PE_PEAK_BYTE_COUNT               0x0000288c
#define NFP3800_CTMX_PKT_MU_PE_DMAS_AVAILABLE_IN_DCACHE_COUNT 0x000028cc
#define   NFP3800_CTMX_PKT_MUPECOUNT11_MU_PE_COUNT(_x)       (((_x) & 0xffffffff) << 0)
#define   NFP3800_CTMX_PKT_MUPECOUNT11_MU_PE_COUNT_of(_x)    (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MUPEClearPeakPacketCount - (Clear) Peak Packet Count
  Register: MUPEClearPeakPacketCountOwner[8] - (Clear) Peak Packet Count (Per Owner). Owner 3 maps to Owner 0.
  Register: MUPEClearPeakByteCount - (Clear) Peak Byte Count
  Register: MUPEClearPeakByteCountOwner[8] - (Clear) Peak Byte Count (Per Owner). Owner 3 maps to Owner 0.
  Register: MUPEClearFingerprintWasErroredCount - (Clear) Fingerprint was Errored Count
  Register: MUPEClearDMADescriptorWasErroredCount - (Clear) DMADescriptor was Errored Count
    Bits: 31:0  MUPECountWithClear(rc) - 11 Bit Count that clears on read. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_PEAK_PACKET_COUNT       0x00002868
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_PEAK_PACKET_COUNT_OWNER(_x) (0x0000286c + (0x4 * ((_x) & 0x7)))
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_PEAK_BYTE_COUNT         0x00002890
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_PEAK_BYTE_COUNT_OWNER(_x) (0x00002894 + (0x4 * ((_x) & 0x7)))
#define NFP3800_CTMX_PKT_MU_PE_FINGERPRINT_WAS_ERRORED_COUNT 0x000028d4
#define NFP3800_CTMX_PKT_MU_PE_DMA_DESCRIPTOR_WAS_ERRORED_COUNT 0x000028d8
#define   NFP3800_CTMX_PKT_MUPECOUNT11WITHCLEAR_MU_PE_COUNT_WITH_CLEAR(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_CTMX_PKT_MUPECOUNT11WITHCLEAR_MU_PE_COUNT_WITH_CLEAR_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MUPEWQActiveCount - Number of Entries currently present in WQ
    Bits:  7:0  MUPECount8(ro) - 8 Bit Count that clears on read. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MU_PE_WQ_ACTIVE_COUNT               0x000028b4
#define   NFP3800_CTMX_PKT_MU_PE_WQ_ACTIVE_COUNT_MU_PE_COUNT_8(_x) (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_WQ_ACTIVE_COUNT_MU_PE_COUNT_8_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: MUPEPQ0ActiveCount - Number of Entries currently present in PQ0
  Register: MUPEPQ1ActiveCount - Number of Entries currently present in PQ1
    Bits:  9:0  MUPECount10(ro) - 10 Bit Count that clears on read. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MU_PE_PQ0_ACTIVE_COUNT              0x000028b8
#define NFP3800_CTMX_PKT_MU_PE_PQ1_ACTIVE_COUNT              0x000028bc
#define   NFP3800_CTMX_PKT_MUPECOUNT10_MU_PE_COUNT_10(_x)    (((_x) & 0x3ff) << 0)
#define   NFP3800_CTMX_PKT_MUPECOUNT10_MU_PE_COUNT_10_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: MUPEDMARequestCreditsAvailable - Incoming DMA Request Credits Avail (Credits Avail in DMA RH)
    Bits:  5:0  MUPECount6(ro) - 6 Bit Count that clears on read. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MU_PE_INCOMING_DMA_REQUEST_CREDITS_AVAILABLE 0x000028d0
#define   NFP3800_CTMX_PKT_MU_PE_INCOMING_DMA_REQUEST_CREDITS_AVAILABLE_MU_PE_COUNT_6(_x) (((_x) & 0x3f) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_INCOMING_DMA_REQUEST_CREDITS_AVAILABLE_MU_PE_COUNT_6_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: MUPEPacketLifeSpanCount - Packet LifeSpan count for Packet Number and Packet Owner described in MUPEPacketLifeSpanPacketNumber and MUPEPacketLifeSpanPacketOwner CSRs.
    Bits: 31:0  MUPECount(ro) - 32 Bit Count. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MU_PE_PACKET_LIFESPAN_COUNT         0x000028dc
#define   NFP3800_CTMX_PKT_MU_PE_PACKET_LIFESPAN_COUNT_MU_PE_COUNT(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_PACKET_LIFESPAN_COUNT_MU_PE_COUNT_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MUPEClearAssertionsFired - This reg sets a bit per assertions_drive module that was fired
    Bits: 15:0  MUPEClearAssertionsFired(rc) - Each bit indicates which assertions_drive module had an assertion fire. Register is cleared on read.
*/
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_FIRED        0x00002a00
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_FIRED_MU_PE_CLEAR_ASSERTIONS_FIRED(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_FIRED_MU_PE_CLEAR_ASSERTIONS_FIRED_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: MUPEClearAssertionsBus - Some useful assertions put into a reg to avoid binary search
    Bits: 30    PatInvalidFree(rc) - PatInvalidFree
    Bits: 29    PatPatTranslateInvalidPacketAtomic(rc) - PatPatTranslateInvalidPacketAtomic
    Bits: 28    PatPatTranslateInvalidPacketBulk(rc) - PatPatTranslateInvalidPacketBulk
    Bits: 27    PatPatTranslateInvalidPacketDma(rc) - PatPatTranslateInvalidPacketDma
    Bits: 26    PatPatTranslateInvalidPacketDcMpushCh0a(rc) - PatPatTranslateInvalidPacketDcMpushCh0a
    Bits: 25    PatPatTranslateInvalidPacketDcMpushCh0b(rc) - PatPatTranslateInvalidPacketDcMpushCh0b
    Bits: 24    PatPatTranslateInvalidPacketDcMpushCh1a(rc) - PatPatTranslateInvalidPacketDcMpushCh1a
    Bits: 23    PatPatTranslateInvalidPacketDcMpushCh1b(rc) - PatPatTranslateInvalidPacketDcMpushCh1b
    Bits: 22    PatPatTranslateInvalidPacketDcMpullCh0(rc) - PatPatTranslateInvalidPacketDcMpullCh0
    Bits: 21    PatPatTranslateInvalidPacketDcMpullCh1(rc) - PatPatTranslateInvalidPacketDcMpullCh1
    Bits: 20    PatPatTranslateInvalidPacketDcTpushCh0(rc) - PatPatTranslateInvalidPacketDcTpushCh0
    Bits: 19    PatPatTranslateInvalidPacketDcTpushCh1(rc) - PatPatTranslateInvalidPacketDcTpushCh1
    Bits: 18    PatPatTranslateInvalidPacketDcMptrCh0(rc) - PatPatTranslateInvalidPacketDcMptrCh0
    Bits: 17    PatPatTranslateInvalidPacketDcMptrCh1(rc) - PatPatTranslateInvalidPacketDcMptrCh1
    Bits: 16    PatPatTranslateInvalidOffsetAtomic(rc) - PatPatTranslateInvalidOffsetAtomic
    Bits: 15    PatPatTranslateInvalidOffsetBulk(rc) - PatPatTranslateInvalidOffsetBulk
    Bits: 14    PatPatTranslateInvalidOffsetDma(rc) - PatPatTranslateInvalidOffsetDma
    Bits: 13    PatPatTranslateInvalidOffsetDcMpushCh0a(rc) - PatPatTranslateInvalidOffsetDcMpushCh0a
    Bits: 12    PatPatTranslateInvalidOffsetDcMpushCh0b(rc) - PatPatTranslateInvalidOffsetDcMpushCh0b
    Bits: 11    PatPatTranslateInvalidOffsetDcMpushCh1a(rc) - PatPatTranslateInvalidOffsetDcMpushCh1a
    Bits: 10    PatPatTranslateInvalidOffsetDcMpushCh1b(rc) - PatPatTranslateInvalidOffsetDcMpushCh1b
    Bits:  9    PatPatTranslateInvalidOffsetDcMpullCh0(rc) - PatPatTranslateInvalidOffsetDcMpullCh0
    Bits:  8    PatPatTranslateInvalidOffsetDcMpullCh1(rc) - PatPatTranslateInvalidOffsetDcMpullCh1
    Bits:  7    PatPatTranslateInvalidOffsetDcTpushCh0(rc) - PatPatTranslateInvalidOffsetDcTpushCh0
    Bits:  6    PatPatTranslateInvalidOffsetDcTpushCh1(rc) - PatPatTranslateInvalidOffsetDcTpushCh1
    Bits:  5    PatPatTranslateInvalidOffsetDcMptrCh0(rc) - PatPatTranslateInvalidOffsetDcMptrCh0
    Bits:  4    PatPatTranslateInvalidOffsetDcMptrCh1(rc) - PatPatTranslateInvalidOffsetDcMptrCh1
    Bits:  3    TargetCommandHandlerWqFifoFull(rc) - TargetCommandHandlerWqFifoFull
    Bits:  2    DcacheInterfaceTopWqWqEmpty(rc) - DcacheInterfaceTopWqWqEmpty
    Bits:  1    DcacheInterfaceTopWqPq0FifoFullOnCommand(rc) - DcacheInterfaceTopWqPq0FifoFullOnCommand
    Bits:  0    DcacheInterfaceTopWqPq0FifoFullOnPush(rc) - DcacheInterfaceTopWqPq0FifoFullOnPush
*/
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS          0x00002a04
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_INVALID_FREE BIT(30)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_PACKET_ATOMIC BIT(29)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_PACKET_BULK BIT(28)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_PACKET_DMA BIT(27)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_PACKET_DC_MPUSH_CH0A BIT(26)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_PACKET_DC_MPUSH_CH0B BIT(25)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_PACKET_DC_MPUSH_CH1A BIT(24)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_PACKET_DC_MPUSH_CH1B BIT(23)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_PACKET_DC_MPULL_CH0 BIT(22)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_PACKET_DC_MPULL_CH1 BIT(21)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_PACKET_DC_TPUSH_CH0 BIT(20)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_PACKET_DC_TPUSH_CH1 BIT(19)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_PACKET_DC_MPTR_CH0 BIT(18)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_PACKET_DC_MPTR_CH1 BIT(17)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_OFFSET_ATOMIC BIT(16)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_OFFSET_BULK BIT(15)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_OFFSET_DMA BIT(14)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_OFFSET_DC_MPUSH_CH0A BIT(13)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_OFFSET_DC_MPUSH_CH0B BIT(12)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_OFFSET_DC_MPUSH_CH1A BIT(11)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_OFFSET_DC_MPUSH_CH1B BIT(10)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_OFFSET_DC_MPULL_CH0 BIT(9)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_OFFSET_DC_MPULL_CH1 BIT(8)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_OFFSET_DC_TPUSH_CH0 BIT(7)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_OFFSET_DC_TPUSH_CH1 BIT(6)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_OFFSET_DC_MPTR_CH0 BIT(5)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_PAT_PAT_TRANSLATE_INVALID_OFFSET_DC_MPTR_CH1 BIT(4)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_TARGET_COMMAND_HANDLER_WQ_FIFO_FULL BIT(3)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_DCACHE_INTERFACE_TOP_WQ_WQ_EMPTY BIT(2)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_DCACHE_INTERFACE_TOP_WQ_PQ0_FIFO_FULL_ON_COMMAND BIT(1)
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_ASSERTIONS_BUS_DCACHE_INTERFACE_TOP_WQ_PQ0_FIFO_FULL_ON_PUSH BIT(0)
/*----------------------------------------------------------------
  Register: MUPEAvailableCredits[8] - MU PE Available Credits
  Register: MUPEReturnCredits[8] - MU PE Return Credits
    Bits: 25:16 MUPEPacketCredits(ro) - Packet Credits
    Bits:  7:0  MUPEBufferCredits(ro) - Buffer Credits
*/
#define NFP3800_CTMX_PKT_MU_PE_AVAILABLE_CREDITS(_x)         (0x00002c00 + (0x4 * ((_x) & 0x7)))
#define NFP3800_CTMX_PKT_MU_PE_RETURN_CREDITS(_x)            (0x00002c20 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_CTMX_PKT_MUPECREDITS_MU_PE_PACKET_CREDITS(_x) (((_x) & 0x3ff) << 16)
#define   NFP3800_CTMX_PKT_MUPECREDITS_MU_PE_PACKET_CREDITS_of(_x) (((_x) >> 16) & 0x3ff)
#define   NFP3800_CTMX_PKT_MUPECREDITS_MU_PE_BUFFER_CREDITS(_x) (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_PKT_MUPECREDITS_MU_PE_BUFFER_CREDITS_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: MUPEClearWQ - Clear WQ FIFO
    Bits:  0    MUPEClearWQ(wo) - Clear WQ FIFO
*/
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_WQ                      0x00003000
#define   NFP3800_CTMX_PKT_MU_PE_CLEAR_WQ_MU_PE_CLEAR_WQ     BIT(0)
/*----------------------------------------------------------------
  Register: MUPEClearPQ0 - Clear PQ0 FIFO
  Register: MUPEClearPQ1 - Clear PQ1 FIFO
    Bits:  0    MUPEClearPQ0(wo) - Clear PQ FIFO
*/
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_PQ0                     0x00003004
#define NFP3800_CTMX_PKT_MU_PE_CLEAR_PQ1                     0x00003008
#define   NFP3800_CTMX_PKT_MUPECLEARPQ0_MU_PE_CLEAR_PQ0      BIT(0)
/*----------------------------------------------------------------
  Register: MUPEAssertionsConfig - Assertion Reporting Configuration Reg for Packet Engine
    Bits: 31:30 MUPEEnableFSMs(rw) - Enable FSMS (Only assertions drive 0 and 1 have FSM status connected to them
    Bits: 23:16 MUPEFSMConfig(rw) - FSM Config
    Bits: 15:0  MUPEDisableAssertions(rw) - Disable Assertions
*/
#define NFP3800_CTMX_PKT_MU_PE_ASSERTION_CONFIG              0x00004000
#define   NFP3800_CTMX_PKT_MU_PE_ASSERTION_CONFIG_MU_PE_ENABLE_FSMS(_x) (((_x) & 0x3) << 30)
#define   NFP3800_CTMX_PKT_MU_PE_ASSERTION_CONFIG_MU_PE_ENABLE_FSMS_of(_x) (((_x) >> 30) & 0x3)
#define   NFP3800_CTMX_PKT_MU_PE_ASSERTION_CONFIG_MU_PE_FSM_CONFIG(_x) (((_x) & 0xff) << 16)
#define   NFP3800_CTMX_PKT_MU_PE_ASSERTION_CONFIG_MU_PE_FSM_CONFIG_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_CTMX_PKT_MU_PE_ASSERTION_CONFIG_MU_PE_DISABLE_ASSERTIONS(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_ASSERTION_CONFIG_MU_PE_DISABLE_ASSERTIONS_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: MUPEMPullBusLBConfig[2] - Load Balance Config for CH X
    Bits:  0    MUPELBConfig(rw) - Choose between static and dynamic load balancing
*/
#define NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_LB_CONFIG(_x)       (0x00006c00 + (0x4 * ((_x) & 0x1)))
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_LB_CONFIG_MU_PE_LB_CONFIG BIT(0)
#define     NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_LB_CONFIG_MU_PE_LB_CONFIG_USE_DYNAMIC (0 << 0)
#define     NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_LB_CONFIG_MU_PE_LB_CONFIG_USE_STATIC BIT(0)
/*----------------------------------------------------------------
  Register: MUPEMPullBusDWRRCreditsConfig[2] - Dynamic Load Balance Config (DWRR) for CH X
  Register: MUPECommandBusDWRRCreditsConfig - Dynamic Load Balance Config (DWRR) for CMD CH
  Register: MUPECommandMasterDWRRCreditsConfig - Dynamic Load Balance Config (DWRR) for CMD CH
  Register: MUPEDmaBusDWRRCreditsConfig - Dynamic Load Balance Config (DWRR) for DMA Bus
  Register: MUPEDmaMasterDWRRCreditsConfig - Dynamic Load Balance Config (DWRR) for DMA Master
    Bits: 31    Disable(wo) - Disable
    Bits: 29:28 Select(wo) - Select
    Bits: 27:20 WriteData(wo) - (Write data) Each unit corresponds to a single cycle over the bus. For MPullData, each cycle over the bus is 8B. To write new values into the LoadBalancer, the DWRR must first be disabled. The new values must be written in, while the DWRR is in the disabled state. The DWRR can be re-enabled for the new values to take effect.
*/
#define NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_DWRR_CREDITS_CONFIG(_x) (0x00006c08 + (0x4 * ((_x) & 0x1)))
#define NFP3800_CTMX_PKT_MU_PE_COMMAND_BUS_DWRR_CREDITS_CONFIG 0x00006c2c
#define NFP3800_CTMX_PKT_MU_PE_COMMAND_MASTER_DWRR_CREDITS_CONFIG 0x00006c34
#define NFP3800_CTMX_PKT_MU_PE_DMA_BUS_DWRR_CREDITS_CONFIG   0x00006c3c
#define NFP3800_CTMX_PKT_MU_PE_DMA_MASTER_DWRR_CREDITS_CONFIG 0x00006c44
#define   NFP3800_CTMX_PKT_DSFCPPDYNAMICLOADBALANCERCONFIG_DISABLE BIT(31)
#define   NFP3800_CTMX_PKT_DSFCPPDYNAMICLOADBALANCERCONFIG_SELECT(_x) (((_x) & 0x3) << 28)
#define   NFP3800_CTMX_PKT_DSFCPPDYNAMICLOADBALANCERCONFIG_SELECT_of(_x) (((_x) >> 28) & 0x3)
#define   NFP3800_CTMX_PKT_DSFCPPDYNAMICLOADBALANCERCONFIG_WRITE_DATA(_x) (((_x) & 0xff) << 20)
#define   NFP3800_CTMX_PKT_DSFCPPDYNAMICLOADBALANCERCONFIG_WRITE_DATA_of(_x) (((_x) >> 20) & 0xff)
/*----------------------------------------------------------------
  Register: MUPEMPullBusDWRRDisableGte - Mpull Bus DWRR Disable GTE for CH0, CH1)
    Bits:  1:0  MUPEMPullBusDWRRDisableGte(rw) - Disable GTE
*/
#define NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_DWRR_DISABLE_GTE    0x00006c10
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_DWRR_DISABLE_GTE_MU_PE_MPULL_BUS_DWRR_DISABLE_GTE(_x) (((_x) & 0x3) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_DWRR_DISABLE_GTE_MU_PE_MPULL_BUS_DWRR_DISABLE_GTE_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: MUPEMPullBusStaticLBConfig[2] - Static Load Balance Config for MPULL CH X
    Bits: 31:24 MapIfMaskMatchFails(wo) - Indexed by master_id[3;0] - result is the load balance answer if mask/match fails
    Bits: 23:16 MapIfMaskMatchPasses(wo) - Indexed by master_id[3;0] - result is the load balance answer if mask/match passes
    Bits: 15:8  ConfigMask(wo) - ANDed with island_id[6;0],master_id[2;2], and compare with mask&match - also if match&~mask then at least one must be set
    Bits:  7:0  ConfigMatch(wo) - Combined with match and island_id and top 2 bits of master_id
*/
#define NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_STATIC_LB_CONFIG(_x) (0x00006c14 + (0x4 * ((_x) & 0x1)))
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_STATIC_LB_CONFIG_MAP_IF_MASK_MATCH_FAILS(_x) (((_x) & 0xff) << 24)
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_STATIC_LB_CONFIG_MAP_IF_MASK_MATCH_FAILS_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_STATIC_LB_CONFIG_MAP_IF_MASK_MATCH_PASSES(_x) (((_x) & 0xff) << 16)
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_STATIC_LB_CONFIG_MAP_IF_MASK_MATCH_PASSES_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_STATIC_LB_CONFIG_CONFIG_MASK(_x) (((_x) & 0xff) << 8)
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_STATIC_LB_CONFIG_CONFIG_MASK_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_STATIC_LB_CONFIG_CONFIG_MATCH(_x) (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_STATIC_LB_CONFIG_CONFIG_MATCH_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: MUPEMPullBusRLConfig[4] - Rate Limiter Config for CH0A (0), CH0B (1), CH1A(2), CH1B(3)
    Bits: 13:8  Increment(wo) - Credit to add every cycle
    Bits:  7:0  Max(wo) - Maximum value for the credit.
*/
#define NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_RL_CONFIG(_x)       (0x00006c1c + (0x4 * ((_x) & 0x3)))
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_RL_CONFIG_INCREMENT(_x) (((_x) & 0x3f) << 8)
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_RL_CONFIG_INCREMENT_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_RL_CONFIG_MAX(_x) (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_MPULL_BUS_RL_CONFIG_MAX_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: MUPECommandBusDWRRDisableGte - Command Bus DWRR Disable GTE
    Bits:  0    MUPECommandBusDWRRDisableGte(rw) - Disable GTE
*/
#define NFP3800_CTMX_PKT_MU_PE_COMMAND_BUS_DWRR_DISABLE_GTE  0x00006c30
#define   NFP3800_CTMX_PKT_MU_PE_COMMAND_BUS_DWRR_DISABLE_GTE_MU_PE_COMMAND_BUS_DWRR_DISABLE_GTE BIT(0)
/*----------------------------------------------------------------
  Register: MUPECommandMasterDWRRDisableGte - Command Master DWRR Disable GTE
    Bits:  0    MUPECommandMasterDWRRDisableGte(rw) - Disable GTE
*/
#define NFP3800_CTMX_PKT_MU_PE_COMMAND_MASTER_DWRR_DISABLE_GTE 0x00006c38
#define   NFP3800_CTMX_PKT_MU_PE_COMMAND_MASTER_DWRR_DISABLE_GTE_MU_PE_COMMAND_MASTER_DWRR_DISABLE_GTE BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDmaBusDWRRDisableGte - Dma Bus DWRR Disable GTE
    Bits:  0    MUPEDmaBusDWRRDisableGte(rw) - Disable GTE
*/
#define NFP3800_CTMX_PKT_MU_PE_DMA_BUS_DWRR_DISABLE_GTE      0x00006c40
#define   NFP3800_CTMX_PKT_MU_PE_DMA_BUS_DWRR_DISABLE_GTE_MU_PE_DMA_BUS_DWRR_DISABLE_GTE BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDmaMasterDWRRDisableGte - Dma Master DWRR Disable GTE
    Bits:  0    MUPEDmaMasterDWRRDisableGte(rw) - Disable GTE
*/
#define NFP3800_CTMX_PKT_MU_PE_DMA_MASTER_DWRR_DISABLE_GTE   0x00006c48
#define   NFP3800_CTMX_PKT_MU_PE_DMA_MASTER_DWRR_DISABLE_GTE_MU_PE_DMA_MASTER_DWRR_DISABLE_GTE BIT(0)
/*----------------------------------------------------------------
  Register: MUPERateSelect - Select bus whose rate needs to be measured
    Bits: 31    MUPERateEnable(rw) - Enable Rate Measurement on bus selected via Select bits
    Bits:  8:4  MUPERateSelect(rw) - Choose bus whose rate needs to be measured
    Bits:  2:0  MUPERateResolution(rw) - Resolution at which rate needs to be measured
*/
#define NFP3800_CTMX_PKT_MU_PE_RATE_SELECT                   0x00006c4c
#define   NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_ENABLE BIT(31)
#define   NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT(_x) (((_x) & 0x1f) << 4)
#define   NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_of(_x) (((_x) >> 4) & 0x1f)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_MASTER_PUSH_DATA_0A (0)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_MASTER_PUSH_DATA_0B (1)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_MASTER_PUSH_DATA_1A (2)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_MASTER_PUSH_DATA_1B (3)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_MASTER_PULL_DATA_0A (4)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_MASTER_PULL_DATA_0B (5)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_MASTER_PULL_DATA_1A (6)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_MASTER_PULL_DATA_1B (7)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_MASTER_PULL_ID_0 (8)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_MASTER_PULL_ID_1 (9)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_MASTER_COMMAND (10)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_TARGET_COMMAND_0 (11)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_TARGET_COMMAND_1 (12)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_TARGET_PUSH_DATA_0 (13)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_TARGET_PUSH_DATA_1 (14)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_DCACHE_REQUEST_0 (15)
#define     NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_SELECT_DCACHE_REQUEST_1 (26)
#define   NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_RESOLUTION(_x) (((_x) & 0x7) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_RATE_SELECT_MU_PE_RATE_RESOLUTION_of(_x) (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: MUPERate - Measured rate for selected bus
    Bits: 31:0  MUPERate(ro) - Rate measured on bus selected using RateSelect register
*/
#define NFP3800_CTMX_PKT_MU_PE_RATE                          0x00006c50
#define   NFP3800_CTMX_PKT_MU_PE_RATE_MU_PE_RATE(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP3800_CTMX_PKT_MU_PE_RATE_MU_PE_RATE_of(_x)      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MUPEWQ - ECC Control Reg for the WQ
  Register: MUPEPQ0 - ECC Control Reg for PQ0
  Register: MUPEPQ1 - ECC Control Reg for PQ1
  Register: MUPEPP - ECC Control Reg for Mpush, Tpush Memories
    Bits: 10    TStab0(rw) - SRAM0: Stability test during Read operation. Low during functional mode.
    Bits:  9    TWbt0(rw) - SRAM0: Weak Bit Test. Low during functional mode. Used for margin test by implementing small speed up in Sense-amp timings during read operations and small decrease in Wordline pulse-width during write operation.
    Bits:  8    MaTpb0(rw) - SRAM0: Margin Adjust Port B.
    Bits:  7    MaTpa0(rw) - SRAM0: Margin Adjust Port A.
    Bits:  6:5  MaWras0(rw) - SRAM0: Margin Adjust: Write Assist timings.
    Bits:  4:3  MaWl0(rw) - SRAM0: Margin Adjust: Wordline pulse width only. See SRAMs specs.
    Bits:  2:1  MaSawl0(rw) - SRAM0: Margin Adjust: Sense-Amp timing and Wordline pulse width. See SRAMs specs.
    Bits:  0    MaWrasd0(rw) - SRAM0: Margin Adjust: Write Assist function disable. See SRAMs specs.
*/
#define NFP3800_CTMX_PKT_MU_PE_WQ                            0x00008000
#define NFP3800_CTMX_PKT_MU_PE_PQ0                           0x00008004
#define NFP3800_CTMX_PKT_MU_PE_PQ1                           0x00008008
#define NFP3800_CTMX_PKT_MU_PE_PP                            0x0000800c
#define   NFP3800_CTMX_PKT_ECCSRAMCNT_T_STAB_0               BIT(10)
#define   NFP3800_CTMX_PKT_ECCSRAMCNT_T_WBT_0                BIT(9)
#define   NFP3800_CTMX_PKT_ECCSRAMCNT_MA_TPB_0               BIT(8)
#define   NFP3800_CTMX_PKT_ECCSRAMCNT_MA_TPA_0               BIT(7)
#define   NFP3800_CTMX_PKT_ECCSRAMCNT_MA_WRAS_0(_x)          (((_x) & 0x3) << 5)
#define   NFP3800_CTMX_PKT_ECCSRAMCNT_MA_WRAS_0_of(_x)       (((_x) >> 5) & 0x3)
#define   NFP3800_CTMX_PKT_ECCSRAMCNT_MA_WL_0(_x)            (((_x) & 0x3) << 3)
#define   NFP3800_CTMX_PKT_ECCSRAMCNT_MA_WL_0_of(_x)         (((_x) >> 3) & 0x3)
#define   NFP3800_CTMX_PKT_ECCSRAMCNT_MA_SAWL_0(_x)          (((_x) & 0x3) << 1)
#define   NFP3800_CTMX_PKT_ECCSRAMCNT_MA_SAWL_0_of(_x)       (((_x) >> 1) & 0x3)
#define   NFP3800_CTMX_PKT_ECCSRAMCNT_MA_WRASD_0             BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDropOnRetry - MU PE Drop Packet Processing Complete on Retry
    Bits:  0    MUPEDropOnRetry(rw) - Drop Packet Processing Complete On Retry
*/
#define NFP3800_CTMX_PKT_MU_PE_DROP_ON_RETRY                 0x00008280
#define   NFP3800_CTMX_PKT_MU_PE_DROP_ON_RETRY_MU_PE_DROP_ON_RETRY BIT(0)
/*----------------------------------------------------------------
  Register: MUPERaiseEventOnRetry - MU PE Raise Event on Retry
    Bits:  0    MUPERaiseEventOnRetry(rw) - Raise Event when Packet Processing Complete with Retry bit set is received
*/
#define NFP3800_CTMX_PKT_MU_PE_RAISE_EVENT_ON_RETRY          0x00008284
#define   NFP3800_CTMX_PKT_MU_PE_RAISE_EVENT_ON_RETRY_MU_PE_RAISE_EVENT_ON_RETRY BIT(0)
#define     NFP3800_CTMX_PKT_MU_PE_RAISE_EVENT_ON_RETRY_MU_PE_RAISE_EVENT_ON_RETRY_DONTRAISEEVENTONRETRY (0 << 0)
#define     NFP3800_CTMX_PKT_MU_PE_RAISE_EVENT_ON_RETRY_MU_PE_RAISE_EVENT_ON_RETRY_RAISEEVENTONRETRY BIT(0)
/*----------------------------------------------------------------
  Register: MUPEPPCWasRetried - MU PE PPC Was Retried
    Bits:  0    MUPEPPCWasRetried(rc) - PPC Was Retried. Clears on Read.
*/
#define NFP3800_CTMX_PKT_MU_PE_PPC_WAS_RETRIED               0x00008288
#define   NFP3800_CTMX_PKT_MU_PE_PPC_WAS_RETRIED_MU_PE_PPC_WAS_RETRIED BIT(0)
/*----------------------------------------------------------------
  Register: XpbErrConfig - XPB Error Configuration Register
    Bits: 15:1  XpbMaxTimeout(rw) - XPB Max Timeout. Value to wait for an XPB response before declaring a timeout error.
    Bits:  0    XpbWrIfErr(rw) - If '1', execute XPB writes if data has data_error. If '0', drop XPB writes for data with data_error. If write is dropped, log the address information in the XPBWrErrStat csr.
*/
#define NFP3800_CTMX_MISC_XPB_ERR_CONFIG                     0x00000000
#define   NFP3800_CTMX_MISC_XPB_ERR_CONFIG_XPB_MAX_TIMEOUT(_x) (((_x) & 0x7fff) << 1)
#define   NFP3800_CTMX_MISC_XPB_ERR_CONFIG_XPB_MAX_TIMEOUT_of(_x) (((_x) >> 1) & 0x7fff)
#define   NFP3800_CTMX_MISC_XPB_ERR_CONFIG_XPB_WRITE_IF_ERR  BIT(0)
/*----------------------------------------------------------------
  Register: XpbTOErrStat - XPB Timeout Error Status Register
    Bits: 14    XpbTOErr(rwc) - XPB Timeout Error. If '1', the requested XPB CLUSTER_ID and DEVICE_ID combination did not result in a XPB response on the XPB Bus. Requesting a non-existent XPB target is not allowed on the XPB bus. If '0', no current error.
    Bits: 13:8  XpbClId(rwc) - This is the XPB CLUSTER ID of the XPB request that caused the XPB Timeout Error.
    Bits:  7:0  XpbDevId(rwc) - This is the XPB Device ID of the XPB request that caused the XPB Timeout Error.
*/
#define NFP3800_CTMX_MISC_XPB_TIMEOUT_ERR_STAT               0x00000004
#define   NFP3800_CTMX_MISC_XPB_TIMEOUT_ERR_STAT_XPB_TIMEOUT_ERR BIT(14)
#define   NFP3800_CTMX_MISC_XPB_TIMEOUT_ERR_STAT_XPB_CLUSTER_ID(_x) (((_x) & 0x3f) << 8)
#define   NFP3800_CTMX_MISC_XPB_TIMEOUT_ERR_STAT_XPB_CLUSTER_ID_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP3800_CTMX_MISC_XPB_TIMEOUT_ERR_STAT_XPB_DEVICE_ID(_x) (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_MISC_XPB_TIMEOUT_ERR_STAT_XPB_DEVICE_ID_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: XpbWrErrStat - XPB Write Error Status Register
    Bits: 30    XpbWrErrMulti(rwc) - Indicates multiple XPB writes were dropped due to a pull data error. Only the XPB information for the first dropped write is logged.
    Bits: 29    XpbWrErrSingle(rwc) - Indicates a single XPB write was dropped due to a pull data error.
    Bits: 28    XpbWrErrGlobal(rwc) - Global_access bit logged from first dropped XPB write.
    Bits: 27:22 XpbWrErrTgtIsld(rwc) - Target island logged from first dropped XPB write.
    Bits: 21:20 XpbWrErrSlaveIdx(rwc) - Slave index logged from first dropped XPB write.
    Bits: 19:14 XpbWrErrDevID(rwc) - DeviceID logged from first dropped XPB write.
    Bits: 13:0  XpbWrErrRegAddr(rwc) - Register address logged from first dropped XPB write.
*/
#define NFP3800_CTMX_MISC_XPB_WR_ERR_STAT                    0x00000008
#define   NFP3800_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_MULTI BIT(30)
#define   NFP3800_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_SINGLE BIT(29)
#define   NFP3800_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_GLOBAL BIT(28)
#define   NFP3800_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_TGT_ISLD(_x) (((_x) & 0x3f) << 22)
#define   NFP3800_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_TGT_ISLD_of(_x) (((_x) >> 22) & 0x3f)
#define   NFP3800_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_SLAVE_IDX(_x) (((_x) & 0x3) << 20)
#define   NFP3800_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_SLAVE_IDX_of(_x) (((_x) >> 20) & 0x3)
#define   NFP3800_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_DEV_ID(_x) (((_x) & 0x3f) << 14)
#define   NFP3800_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_DEV_ID_of(_x) (((_x) >> 14) & 0x3f)
#define   NFP3800_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_REG_ADDR(_x) (((_x) & 0x3fff) << 0)
#define   NFP3800_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_REG_ADDR_of(_x) (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: Ring_[16]_Base - Ring X Base Register
    Bits: 31:29 Size(rw) - Size of ring in words
    Bits: 28    StatusType(rw) - Status type. This bit controls the type of status reported on the status bus for the ring.
    Bits: 17:9  BaseAddress(rw) - The Base Address. This value must be written to be consistent with the requirement that the Ring is aligned to its size. For example, if the Ring has 512 longwords, bits 10:9 must be written to 0.
*/
#define NFP3800_CTMX_MISC_RING_BASE(_x)                      (0x00000100 + (0x10 * ((_x) & 0xf)))
#define   NFP3800_CTMX_MISC_RING_BASE_SIZE(_x)               (((_x) & 0x7) << 29)
#define   NFP3800_CTMX_MISC_RING_BASE_SIZE_of(_x)            (((_x) >> 29) & 0x7)
#define     NFP3800_CTMX_MISC_RING_BASE_SIZE_128             (0)
#define     NFP3800_CTMX_MISC_RING_BASE_SIZE_256             (1)
#define     NFP3800_CTMX_MISC_RING_BASE_SIZE_512             (2)
#define     NFP3800_CTMX_MISC_RING_BASE_SIZE_1K              (3)
#define     NFP3800_CTMX_MISC_RING_BASE_SIZE_2K              (4)
#define     NFP3800_CTMX_MISC_RING_BASE_SIZE_4K              (5)
#define     NFP3800_CTMX_MISC_RING_BASE_SIZE_8K              (6)
#define     NFP3800_CTMX_MISC_RING_BASE_SIZE_16K             (7)
#define   NFP3800_CTMX_MISC_RING_BASE_STATUS_TYPE            BIT(28)
#define     NFP3800_CTMX_MISC_RING_BASE_STATUS_TYPE_EMPTY    (0 << 28)
#define     NFP3800_CTMX_MISC_RING_BASE_STATUS_TYPE_FULL     BIT(28)
#define   NFP3800_CTMX_MISC_RING_BASE_BASE(_x)               (((_x) & 0x1ff) << 9)
#define   NFP3800_CTMX_MISC_RING_BASE_BASE_of(_x)            (((_x) >> 9) & 0x1ff)
/*----------------------------------------------------------------
  Register: Ring_[16]_Head - Ring X Head Register
    Bits: 16:2  HeadOffset(rw) - Head pointer
*/
#define NFP3800_CTMX_MISC_RING_HEAD(_x)                      (0x00000104 + (0x10 * ((_x) & 0xf)))
#define   NFP3800_CTMX_MISC_RING_HEAD_OFF(_x)                (((_x) & 0x7fff) << 2)
#define   NFP3800_CTMX_MISC_RING_HEAD_OFF_of(_x)             (((_x) >> 2) & 0x7fff)
/*----------------------------------------------------------------
  Register: Ring_[16]_Tail - Ring X Tail Register
    Bits: 16:2  TailOffset(rw) - Tail pointer
*/
#define NFP3800_CTMX_MISC_RING_TAIL(_x)                      (0x00000108 + (0x10 * ((_x) & 0xf)))
#define   NFP3800_CTMX_MISC_RING_TAIL_OFF(_x)                (((_x) & 0x7fff) << 2)
#define   NFP3800_CTMX_MISC_RING_TAIL_OFF_of(_x)             (((_x) >> 2) & 0x7fff)

#else /* NFP3800_LONGNAMES */

/*----------------------------------------------------------------
  Register: ConfigCPP - CPP MU Configuration Register
    Bits: 16    DisableTagPassingBulkEngine(rw) - This flag is used to disable tag passing completely in Bulk engine. All Tags would be requested and released.
    Bits: 15:8  DirAccWays(rw) - When bit n in this field is set to 1, permit MU commands to direct access that way of the cache; when low, address bits [39:38] determine locality. If all 1s then all MU commands, with the exception of the Queue Engine commands highlighted in the Note below, are forced to be direct access. Note: This applies only the External Memory Units, all other Memory Units are direct access only
    Bits:  7    DisableFixOnPartialTagPassingBulkEngine(rw) - This bit is set to Disable the fix which was put in for Partial Tag-passing. If this bit is set old mode of operation would take into effect for Bulk engine Tag-passing.
    Bits:  6    EnableBulkByteMaskSwap(rw) - Enable Bulk ByteMask Swap
    Bits:  5    CPPErrorSuppress(rw) - If set, data errors generated for outbound CPP data buses will be suppressed internally and will not be sent out on the bus. CPP targets will suppress Push bus errors, CPP masters will suppress Pull bus errors.
    Bits:  4    CPPErrorIgnore(rw) - If set, data errors received on inbound CPP data buses will be ignored internally. CPP targets will ignore Pull bus errors, CPP masters will ignore Push bus errors.
    Bits:  3    DisableDualChRead(rw) - Controls the use of the CTM Bulk Engine performance feature to use both DCache channels for parallel non-dependent reads if the ordered commands are blocked/empty (CTM Only).
    Bits:  2:1  Addr32Bit(rw) - Select mode of addressing. Note that only Bulk, Atomic and Queue operations are supported in 32-bit modes.
    Bits:  0    IgnBulkAlign(rw) - If set, then CPP address[3;0] are ignored for bulk DRAM access, as with the IXP. If clear, then CPP address[3;0] are used, and unaligned accesses occur if they are not zero.
*/
#define NFP3800_CTMX_CFG_ConfigCPP                           0x00000000
#define   NFP3800_CTMX_CFG_ConfigCPP_DisableTagPassingBulkEngine BIT(16)
#define   NFP3800_CTMX_CFG_ConfigCPP_DirAccWays(_x)          (((_x) & 0xff) << 8)
#define   NFP3800_CTMX_CFG_ConfigCPP_DirAccWays_of(_x)       (((_x) >> 8) & 0xff)
#define   NFP3800_CTMX_CFG_ConfigCPP_DisableFixOnPartialTagPassingBulkEngine BIT(7)
#define   NFP3800_CTMX_CFG_ConfigCPP_EnableBulkByteMaskSwap  BIT(6)
#define   NFP3800_CTMX_CFG_ConfigCPP_CPPErrorSuppress        BIT(5)
#define   NFP3800_CTMX_CFG_ConfigCPP_CPPErrorIgnore          BIT(4)
#define   NFP3800_CTMX_CFG_ConfigCPP_DisableDualChRead       BIT(3)
#define     NFP3800_CTMX_CFG_ConfigCPP_DisableDualChRead_Enabled (0 << 3)
#define     NFP3800_CTMX_CFG_ConfigCPP_DisableDualChRead_Disabled BIT(3)
#define   NFP3800_CTMX_CFG_ConfigCPP_Addr32Bit(_x)           (((_x) & 0x3) << 1)
#define   NFP3800_CTMX_CFG_ConfigCPP_Addr32Bit_of(_x)        (((_x) >> 1) & 0x3)
#define     NFP3800_CTMX_CFG_ConfigCPP_Addr32Bit_40BIT       (0)
#define     NFP3800_CTMX_CFG_ConfigCPP_Addr32Bit_32BIT       (1)
#define     NFP3800_CTMX_CFG_ConfigCPP_Addr32Bit_32BITHLR    (2)
#define     NFP3800_CTMX_CFG_ConfigCPP_Addr32Bit_32BITLLR    (3)
#define   NFP3800_CTMX_CFG_ConfigCPP_IgnBulkAlign            BIT(0)
#define     NFP3800_CTMX_CFG_ConfigCPP_IgnBulkAlign_ENFORCE  (0 << 0)
#define     NFP3800_CTMX_CFG_ConfigCPP_IgnBulkAlign_IGNORE   BIT(0)
/*----------------------------------------------------------------
  Register: ConfigVQDR[4] - Virtual QDR X Configuration Register
    Bits: 21:20 QueueType(rw) - Type of queues in the channel, similar to IXP IgnoreEOP and IgnoreSegCnt
    Bits: 15:0  MemWinBase(rw) - Bits [16;24] of full 40-bit MU address space for the base of the virtual QDR channel
*/
#define NFP3800_CTMX_CFG_ConfigVQDR(_x)                      (0x00000004 + (0x4 * ((_x) & 0x3)))
#define   NFP3800_CTMX_CFG_ConfigVQDR_QueueType(_x)          (((_x) & 0x3) << 20)
#define   NFP3800_CTMX_CFG_ConfigVQDR_QueueType_of(_x)       (((_x) >> 20) & 0x3)
#define     NFP3800_CTMX_CFG_ConfigVQDR_QueueType_SEGMENTS   (0)
#define     NFP3800_CTMX_CFG_ConfigVQDR_QueueType_PACKETS    (1)
#define     NFP3800_CTMX_CFG_ConfigVQDR_QueueType_PACKETS32  (2)
#define     NFP3800_CTMX_CFG_ConfigVQDR_QueueType_BUFFERS    (3)
#define   NFP3800_CTMX_CFG_ConfigVQDR_MemWinBase(_x)         (((_x) & 0xffff) << 0)
#define   NFP3800_CTMX_CFG_ConfigVQDR_MemWinBase_of(_x)      (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: MUConfigBeSdet - Bulk Engine starvation Detection Control Register
*/
#define NFP3800_CTMX_CFG_MUConfigBeSdet                      0x00000014
/*----------------------------------------------------------------
  Register: PushLoadBalancer - MU DSF Push Load Balancer Register
  Register: BulkPushLoadBalancer[2] - MU DSF Push Load Balancer Register for Bulk Engine Channel X
    Bits: 31:24 MapIfMaskMatchFails(wo) - Indexed by master_id[3;0] - result is the load balance answer if mask/match fails
    Bits: 23:16 MapIfMaskMatchPasses(wo) - Indexed by master_id[3;0] - result is the load balance answer if mask/match passes
    Bits: 15:8  ConfigMask(wo) - ANDed with island_id[6;0],master_id[2;2], and compare with mask&match - also if match&~mask then at least one must be set
    Bits:  7:0  ConfigMatch(wo) - Combined with match and island_id and top 2 bits of master_id
*/
#define NFP3800_CTMX_CFG_PushLoadBalancer                    0x00000800
#define NFP3800_CTMX_CFG_BulkPushLoadBalancer(_x)            (0x00001c10 + (0x4 * ((_x) & 0x1)))
#define   NFP3800_CTMX_CFG_DsfCppLoadBalancerConfig_MapIfMaskMatchFails(_x) (((_x) & 0xff) << 24)
#define   NFP3800_CTMX_CFG_DsfCppLoadBalancerConfig_MapIfMaskMatchFails_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_CTMX_CFG_DsfCppLoadBalancerConfig_MapIfMaskMatchPasses(_x) (((_x) & 0xff) << 16)
#define   NFP3800_CTMX_CFG_DsfCppLoadBalancerConfig_MapIfMaskMatchPasses_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_CTMX_CFG_DsfCppLoadBalancerConfig_ConfigMask(_x) (((_x) & 0xff) << 8)
#define   NFP3800_CTMX_CFG_DsfCppLoadBalancerConfig_ConfigMask_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_CTMX_CFG_DsfCppLoadBalancerConfig_ConfigMatch(_x) (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_CFG_DsfCppLoadBalancerConfig_ConfigMatch_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: XpbTimeout - XPB Timeout
    Bits: 31:0  XpbTimeoutData(rw) - XPB timeout data.
*/
#define NFP3800_CTMX_CFG_XpbTimeout                          0x00000c00
#define   NFP3800_CTMX_CFG_XpbTimeout_XpbTimeoutData(_x)     (((_x) & 0xffffffff) << 0)
#define   NFP3800_CTMX_CFG_XpbTimeout_XpbTimeoutData_of(_x)  (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: QueueEngineReg - Queue Engine Error reset and Configurable events
    Bits: 31    ErrorReset(rw) - When set to 1, causes Queue engine to clear its error bit
    Bits: 30    ErrorIndicator(ro) - when set to 1, indicates that all the Queue operations on any of the QDescriptor contains multi-bit error
    Bits:  6    EnableDropBadPushIds(rw) - When this bit is set, push commands that get their IDs from DCache will be silently dropped If the Dcache read had an un-correctablr ECC error.
    Bits:  5    EnableErrorHandling(rw) - When this bit is set, Error-handling becomes active. i.e. from now on if any Qdescriptor gets error based on ReadQDescriptor then any command working on any of the Qdescriptor would be considered to be Error'd
    Bits:  4:0  ConfigurableWatermark(rw) - Legal values are from 0->19, when set to 0 event is raised when count goes above/below pow(2,(4+configurable_watermark)) value. If value greater than 19 is specified then RTL would default it to value of 0.
*/
#define NFP3800_CTMX_CFG_QueueEngineReg                      0x00002400
#define   NFP3800_CTMX_CFG_QueueEngineReg_ErrorReset         BIT(31)
#define   NFP3800_CTMX_CFG_QueueEngineReg_ErrorIndicator     BIT(30)
#define   NFP3800_CTMX_CFG_QueueEngineReg_EnableDropBadPushIds BIT(6)
#define   NFP3800_CTMX_CFG_QueueEngineReg_EnableErrorHandling BIT(5)
#define   NFP3800_CTMX_CFG_QueueEngineReg_ConfigurableWatermark(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_CTMX_CFG_QueueEngineReg_ConfigurableWatermark_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: AtomicEngineReg - Atomic engine, Enable shifting of Immediate data for ALU operations
    Bits:  0    AtomicEngineReg(rw) - Atomic engine, Enable shifting of Immediate data for ALU operations.
*/
#define NFP3800_CTMX_CFG_AtomicEngineReg                     0x00002404
#define   NFP3800_CTMX_CFG_AtomicEngineReg_AtomicEngineReg   BIT(0)
/*----------------------------------------------------------------
  Register: MUPEMemConfig - MU PE Memory Configuration
    Bits:  2:0  MUPEMemConfig(rw) - Packet Mem Config. How much of the CTM memory can be used to store packets
*/
#define NFP3800_CTMX_PKT_MUPEMemConfig                       0x00000000
#define   NFP3800_CTMX_PKT_MUPEMemConfig_MUPEMemConfig(_x)   (((_x) & 0x7) << 0)
#define   NFP3800_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_mem_config_8_of_8_parts (0)
#define     NFP3800_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_mem_config_7_of_8_parts (1)
#define     NFP3800_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_mem_config_6_of_8_parts (2)
#define     NFP3800_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_mem_config_5_of_8_parts (3)
#define     NFP3800_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_mem_config_4_of_8_parts (4)
#define     NFP3800_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_mem_config_3_of_8_parts (5)
#define     NFP3800_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_mem_config_2_of_8_parts (6)
#define     NFP3800_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_mem_config_1_of_8_parts (7)
/*----------------------------------------------------------------
  Register: MUPEWQConfig - MU PE Work Queue Config
    Bits:  1:0  MUPEWQConfig(rw) - Work Queue Configuration
*/
#define NFP3800_CTMX_PKT_MUPEWQConfig                        0x00000004
#define   NFP3800_CTMX_PKT_MUPEWQConfig_MUPEWQConfig(_x)     (((_x) & 0x3) << 0)
#define   NFP3800_CTMX_PKT_MUPEWQConfig_MUPEWQConfig_of(_x)  (((_x) >> 0) & 0x3)
#define     NFP3800_CTMX_PKT_MUPEWQConfig_MUPEWQConfig_SendOnLast (0)
#define     NFP3800_CTMX_PKT_MUPEWQConfig_MUPEWQConfig_SendOnFirst (1)
#define     NFP3800_CTMX_PKT_MUPEWQConfig_MUPEWQConfig_SendOnBoth (2)
/*----------------------------------------------------------------
  Register: MUPEBufferConfig - MU PE Buffer Configuration
    Bits:  1:0  MUPEBufferConfig(rw) - Buffer Config
*/
#define NFP3800_CTMX_PKT_MUPEBufferConfig                    0x00000008
#define   NFP3800_CTMX_PKT_MUPEBufferConfig_MUPEBufferConfig(_x) (((_x) & 0x3) << 0)
#define   NFP3800_CTMX_PKT_MUPEBufferConfig_MUPEBufferConfig_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_CTMX_PKT_MUPEBufferConfig_MUPEBufferConfig_buffer_config_2KB (0)
#define     NFP3800_CTMX_PKT_MUPEBufferConfig_MUPEBufferConfig_buffer_config_4KB (1)
#define     NFP3800_CTMX_PKT_MUPEBufferConfig_MUPEBufferConfig_buffer_config_8KB (2)
#define     NFP3800_CTMX_PKT_MUPEBufferConfig_MUPEBufferConfig_buffer_config_16KB (3)
/*----------------------------------------------------------------
  Register: MUPEAllowNBIPushToCombineTo16B - Allow NBI Push to Combine to 16B
    Bits:  0    MUPEAllowNBIPushToCombineTo16B(rw) - Allow NBI Push to combine to 16B
*/
#define NFP3800_CTMX_PKT_MUPEAllowNBIPushToCombineTo16B      0x0000000c
#define   NFP3800_CTMX_PKT_MUPEAllowNBIPushToCombineTo16B_MUPEAllowNBIPushToCombineTo16B BIT(0)
/*----------------------------------------------------------------
  Register: MUPEAllowDMAPushToCombineTo16B - Allow DMA Push to Combine to 16B
    Bits:  0    MUPEAllowDMAPushToCombineTo16B(rw) - Allow DMA Push to combine to 16B
*/
#define NFP3800_CTMX_PKT_MUPEAllowDMAPushToCombineTo16B      0x00000010
#define   NFP3800_CTMX_PKT_MUPEAllowDMAPushToCombineTo16B_MUPEAllowDMAPushToCombineTo16B BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDMAOverflowIntoDCacheEnabled - Allow DMA Commands to be stored into the DCache
    Bits:  0    MUPEDMAOverflowIntoDCacheEnabled(rw) - Allow DMA Commands to be stored into the DCache
*/
#define NFP3800_CTMX_PKT_MUPEDMAOverflowIntoDCacheEnabled    0x00000014
#define   NFP3800_CTMX_PKT_MUPEDMAOverflowIntoDCacheEnabled_MUPEDMAOverflowIntoDCacheEnabled BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDMAStorageSize - DMA Storage Size
    Bits:  1:0  MUPEDMAStorageSize(rw) - DMA Storage Size
*/
#define NFP3800_CTMX_PKT_MUPEDMAStorageSize                  0x00000018
#define   NFP3800_CTMX_PKT_MUPEDMAStorageSize_MUPEDMAStorageSize(_x) (((_x) & 0x3) << 0)
#define   NFP3800_CTMX_PKT_MUPEDMAStorageSize_MUPEDMAStorageSize_of(_x) (((_x) >> 0) & 0x3)
#define     NFP3800_CTMX_PKT_MUPEDMAStorageSize_MUPEDMAStorageSize_dma_storage_for_1024_descriptors (0)
#define     NFP3800_CTMX_PKT_MUPEDMAStorageSize_MUPEDMAStorageSize_dma_storage_for_512_descriptors (1)
#define     NFP3800_CTMX_PKT_MUPEDMAStorageSize_MUPEDMAStorageSize_dma_storage_for_256_descriptors (2)
#define     NFP3800_CTMX_PKT_MUPEDMAStorageSize_MUPEDMAStorageSize_dma_storage_for_128_descriptors (3)
/*----------------------------------------------------------------
  Register: MUPEDMAStorageBaseAddress - DMA Storage Base Address
    Bits: 16:0  MUPEDMAStorageBaseAddress(rw) - DMA Storage Base Address
*/
#define NFP3800_CTMX_PKT_MUPEDMAStorageBaseAddress           0x0000001c
#define   NFP3800_CTMX_PKT_MUPEDMAStorageBaseAddress_MUPEDMAStorageBaseAddress(_x) (((_x) & 0x1ffff) << 0)
#define   NFP3800_CTMX_PKT_MUPEDMAStorageBaseAddress_MUPEDMAStorageBaseAddress_of(_x) (((_x) >> 0) & 0x1ffff)
/*----------------------------------------------------------------
  Register: MUPEDMAMaxOutstandingMem - DMA Outstanding Rd/Write Commands
    Bits:  2:0  MUPEDMAMaxOutstandingMem(rw) - DMA Outstanding Bulk Read/Write Commands
*/
#define NFP3800_CTMX_PKT_MUPEDMAMaxOutstandingMem            0x00000020
#define   NFP3800_CTMX_PKT_MUPEDMAMaxOutstandingMem_MUPEDMAMaxOutstandingMem(_x) (((_x) & 0x7) << 0)
#define   NFP3800_CTMX_PKT_MUPEDMAMaxOutstandingMem_MUPEDMAMaxOutstandingMem_of(_x) (((_x) >> 0) & 0x7)
#define     NFP3800_CTMX_PKT_MUPEDMAMaxOutstandingMem_MUPEDMAMaxOutstandingMem_32_oustandings (0)
#define     NFP3800_CTMX_PKT_MUPEDMAMaxOutstandingMem_MUPEDMAMaxOutstandingMem_64_oustanding (1)
#define     NFP3800_CTMX_PKT_MUPEDMAMaxOutstandingMem_MUPEDMAMaxOutstandingMem_128_oustanding (2)
#define     NFP3800_CTMX_PKT_MUPEDMAMaxOutstandingMem_MUPEDMAMaxOutstandingMem_256_oustanding (3)
#define     NFP3800_CTMX_PKT_MUPEDMAMaxOutstandingMem_MUPEDMAMaxOutstandingMem_512_oustandings (4)
#define     NFP3800_CTMX_PKT_MUPEDMAMaxOutstandingMem_MUPEDMAMaxOutstandingMem_1024_oustanding (6)
#define     NFP3800_CTMX_PKT_MUPEDMAMaxOutstandingMem_MUPEDMAMaxOutstandingMem_4096_oustanding (7)
/*----------------------------------------------------------------
  Register: MUPEDMAHysteresis - Empty Slots that need to be avail in the DMA RH before reading new cmds from DCache
    Bits:  5:0  MUPEDMAHysteresis(rw) - DMA Outstanding Bulk Read/Write Commands
*/
#define NFP3800_CTMX_PKT_MUPEDMAHysteresis                   0x00000024
#define   NFP3800_CTMX_PKT_MUPEDMAHysteresis_MUPEDMAHysteresis(_x) (((_x) & 0x3f) << 0)
#define   NFP3800_CTMX_PKT_MUPEDMAHysteresis_MUPEDMAHysteresis_of(_x) (((_x) >> 0) & 0x3f)
#define     NFP3800_CTMX_PKT_MUPEDMAHysteresis_MUPEDMAHysteresis_32 (32)
#define     NFP3800_CTMX_PKT_MUPEDMAHysteresis_MUPEDMAHysteresis_4_ (4)
#define     NFP3800_CTMX_PKT_MUPEDMAHysteresis_MUPEDMAHysteresis_8_ (8)
#define     NFP3800_CTMX_PKT_MUPEDMAHysteresis_MUPEDMAHysteresis_12 (12)
#define     NFP3800_CTMX_PKT_MUPEDMAHysteresis_MUPEDMAHysteresis_16 (16)
#define     NFP3800_CTMX_PKT_MUPEDMAHysteresis_MUPEDMAHysteresis_20 (20)
#define     NFP3800_CTMX_PKT_MUPEDMAHysteresis_MUPEDMAHysteresis_24 (24)
#define     NFP3800_CTMX_PKT_MUPEDMAHysteresis_MUPEDMAHysteresis_28 (28)
/*----------------------------------------------------------------
  Register: MUPEStopPQ0Dequeue - Set before starting WQ Flush. Setting this bit prevents the PQ from draining
    Bits:  0    MUPEStopPQ0Dequeue(rw) - Set before starting WQ Flush. Setting this bit will prevent the Packet Queue from draining.
*/
#define NFP3800_CTMX_PKT_MUPEStopPQ0Dequeue                  0x00000028
#define   NFP3800_CTMX_PKT_MUPEStopPQ0Dequeue_MUPEStopPQ0Dequeue BIT(0)
/*----------------------------------------------------------------
  Register: MUPEStopWQDequeue - Set before starting PQ0 Flush. Setting this bit prevents the WQ from draining
    Bits:  0    MUPEStopWQDequeue(rw) - Set before starting PQ0 Flush. Setting this bit will prevent the Work Queue from draining.
*/
#define NFP3800_CTMX_PKT_MUPEStopWQDequeue                   0x0000002c
#define   NFP3800_CTMX_PKT_MUPEStopWQDequeue_MUPEStopWQDequeue BIT(0)
/*----------------------------------------------------------------
  Register: MUPEGrantCycles - This reg controls how many cycles each requestor gets access to the DCache port for
    Bits:  0    MUPEGrantCycles(rw) - This register controls how many cycles each requestor gets access to the DCache port for.
*/
#define NFP3800_CTMX_PKT_MUPEGrantCycles                     0x00000030
#define   NFP3800_CTMX_PKT_MUPEGrantCycles_MUPEGrantCycles   BIT(0)
#define     NFP3800_CTMX_PKT_MUPEGrantCycles_MUPEGrantCycles_8_ (0 << 0)
#define     NFP3800_CTMX_PKT_MUPEGrantCycles_MUPEGrantCycles_16_ BIT(0)
/*----------------------------------------------------------------
  Register: MUPEOverridePCIeLength - This reg is used to override the length from the add_thread command for PCIe packets
    Bits: 31    MUPEOverridePCIeLengthEnable(rw) - This register is used to enable overriding the length from the add_thread command for PCIe allocated packets.
    Bits: 14:0  MUPEOverridePCIeLength(rw) - This register is used to override the length from the add_thread command for PCIe allocated packets. This is a Byte aligned field, however length must be a 4B aligned field. Rules for AddThread must be adhered to while programming this field.
*/
#define NFP3800_CTMX_PKT_MUPEOverridePCIeLength              0x00000034
#define   NFP3800_CTMX_PKT_MUPEOverridePCIeLength_MUPEOverridePCIeLengthEnable BIT(31)
#define   NFP3800_CTMX_PKT_MUPEOverridePCIeLength_MUPEOverridePCIeLength(_x) (((_x) & 0x7fff) << 0)
#define   NFP3800_CTMX_PKT_MUPEOverridePCIeLength_MUPEOverridePCIeLength_of(_x) (((_x) >> 0) & 0x7fff)
/*----------------------------------------------------------------
  Register: MUPEOverridePCIeOffset - This reg is used to override the offset from the add_thread command for PCIe packets
    Bits: 31    MUPEOverridePCIeOffsetEnable(rw) - This register is used to enable overriding the offset from the add_thread command for PCIe allocated packets. This is a Byte aligned field. Rules for AddThread must be adhered to while programming this field.
    Bits: 14:0  MUPEOverridePCIeOffset(rw) - This register is used to override the offset from the add_thread command for PCIe allocated packets. This is a Byte aligned field. Rules for AddThread must be adhered to while programming this field.
*/
#define NFP3800_CTMX_PKT_MUPEOverridePCIeOffset              0x00000038
#define   NFP3800_CTMX_PKT_MUPEOverridePCIeOffset_MUPEOverridePCIeOffsetEnable BIT(31)
#define   NFP3800_CTMX_PKT_MUPEOverridePCIeOffset_MUPEOverridePCIeOffset(_x) (((_x) & 0x7fff) << 0)
#define   NFP3800_CTMX_PKT_MUPEOverridePCIeOffset_MUPEOverridePCIeOffset_of(_x) (((_x) >> 0) & 0x7fff)
/*----------------------------------------------------------------
  Register: MUPEMultiCastEnable - Enable Hardware Support for Multicast
    Bits:  0    MUPEMultiCastEnable(rw) - Enable Hardware Support for Multicast
*/
#define NFP3800_CTMX_PKT_MUPEMultiCastEnable                 0x0000003c
#define   NFP3800_CTMX_PKT_MUPEMultiCastEnable_MUPEMultiCastEnable BIT(0)
/*----------------------------------------------------------------
  Register: MUPEMultiCastFingerprintEnable - FingerprintEnable Hardware Support for Multicast
    Bits:  0    MUPEMultiCastFingerprintEnable(rw) - Hardware Support for Multicast with Fingerprint written by ME
*/
#define NFP3800_CTMX_PKT_MUPEMultiCastFingerprintEnable      0x00000040
#define   NFP3800_CTMX_PKT_MUPEMultiCastFingerprintEnable_MUPEMultiCastFingerprintEnable BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDropAwareModeEnable - Enable DropAwareMode (all dropped packets except multicast dropped packets)
    Bits:  0    MUPEDropAwareModeEnable(rw) - Enable DropAwareMode (all dropped packets except multicast dropped packets)
*/
#define NFP3800_CTMX_PKT_MUPEDropAwareModeEnable             0x00000044
#define   NFP3800_CTMX_PKT_MUPEDropAwareModeEnable_MUPEDropAwareModeEnable BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDropRule[3] - Drop Rule X
    Bits: 31    MUPEDropRuleEnable(rw) - Enable this rule
    Bits: 27    MUPEDropRuleFreePacket(rw) - If masked packet data at starting index matches this rule, then free the packet only if this bit is set
    Bits: 26    MUPEDropRuleAddtoPacketQueue(rw) - If masked packet data at starting index matches this rule, then add packet back to PacketQueue only if this bit is set
    Bits: 25    MUPEDropRuleSendToRing1(rw) - If masked packet data at starting index matches this rule, then send information to Ring1 only if this bit is set
    Bits: 24    MUPEDropRuleSendToRing0(rw) - If masked packet data at starting index matches this rule, then send information to Ring0
    Bits: 19:16 MUPEDropRuleStartingIndex(rw) - Starting Index for Mask and Match Data
    Bits: 15:8  MUPEDropRuleMatch(rw) - Drop Match to match packet data
    Bits:  7:0  MUPEDropRuleMask(rw) - Drop Mask to mask packet data
*/
#define NFP3800_CTMX_PKT_MUPEDropRule(_x)                    (0x00000048 + (0x4 * ((_x) & 0x3)))
#define   NFP3800_CTMX_PKT_MUPEDropRule_MUPEDropRuleEnable   BIT(31)
#define   NFP3800_CTMX_PKT_MUPEDropRule_MUPEDropRuleFreePacket BIT(27)
#define   NFP3800_CTMX_PKT_MUPEDropRule_MUPEDropRuleAddtoPacketQueue BIT(26)
#define   NFP3800_CTMX_PKT_MUPEDropRule_MUPEDropRuleSendToRing1 BIT(25)
#define   NFP3800_CTMX_PKT_MUPEDropRule_MUPEDropRuleSendToRing0 BIT(24)
#define   NFP3800_CTMX_PKT_MUPEDropRule_MUPEDropRuleStartingIndex(_x) (((_x) & 0xf) << 16)
#define   NFP3800_CTMX_PKT_MUPEDropRule_MUPEDropRuleStartingIndex_of(_x) (((_x) >> 16) & 0xf)
#define   NFP3800_CTMX_PKT_MUPEDropRule_MUPEDropRuleMatch(_x) (((_x) & 0xff) << 8)
#define   NFP3800_CTMX_PKT_MUPEDropRule_MUPEDropRuleMatch_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_CTMX_PKT_MUPEDropRule_MUPEDropRuleMask(_x) (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_PKT_MUPEDropRule_MUPEDropRuleMask_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: MUPEDropRing[2] - Drop Ring X
    Bits: 31:28 MUPEDropRingNumber(rw) - Ring Number
    Bits: 25:24 MUPEDropRingDataSelect(rw) - Which 32 bits of data out of 128 bits of data should be sent to the Ring
    Bits: 23:16 MUPEDropRingIsland(rw) - Island Number to use while adding data to Ring
    Bits: 11:8  MUPEDropRingTarget(rw) - Target Encoding to use while adding data to Ring
    Bits:  6:5  MUPEDropRingToken(rw) - Token Encoding to use while adding data to Ring
    Bits:  4:0  MUPEDropRingAction(rw) - Action Encoding to use while adding data to Ring
*/
#define NFP3800_CTMX_PKT_MUPEDropRing(_x)                    (0x00000054 + (0x4 * ((_x) & 0x1)))
#define   NFP3800_CTMX_PKT_MUPEDropRing_MUPEDropRingNumber(_x) (((_x) & 0xf) << 28)
#define   NFP3800_CTMX_PKT_MUPEDropRing_MUPEDropRingNumber_of(_x) (((_x) >> 28) & 0xf)
#define   NFP3800_CTMX_PKT_MUPEDropRing_MUPEDropRingDataSelect(_x) (((_x) & 0x3) << 24)
#define   NFP3800_CTMX_PKT_MUPEDropRing_MUPEDropRingDataSelect_of(_x) (((_x) >> 24) & 0x3)
#define   NFP3800_CTMX_PKT_MUPEDropRing_MUPEDropRingIsland(_x) (((_x) & 0xff) << 16)
#define   NFP3800_CTMX_PKT_MUPEDropRing_MUPEDropRingIsland_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_CTMX_PKT_MUPEDropRing_MUPEDropRingTarget(_x) (((_x) & 0xf) << 8)
#define   NFP3800_CTMX_PKT_MUPEDropRing_MUPEDropRingTarget_of(_x) (((_x) >> 8) & 0xf)
#define   NFP3800_CTMX_PKT_MUPEDropRing_MUPEDropRingToken(_x) (((_x) & 0x3) << 5)
#define   NFP3800_CTMX_PKT_MUPEDropRing_MUPEDropRingToken_of(_x) (((_x) >> 5) & 0x3)
#define   NFP3800_CTMX_PKT_MUPEDropRing_MUPEDropRingAction(_x) (((_x) & 0x1f) << 0)
#define   NFP3800_CTMX_PKT_MUPEDropRing_MUPEDropRingAction_of(_x) (((_x) >> 0) & 0x1f)
/*----------------------------------------------------------------
  Register: MUPENullMUPtr - Null MUPtr. Should be configured to the same value as NBI DMA and NBI TM
    Bits: 28:0  MUPENullMUPtr(rw) - Null MUPtr. Should be configured to the same value as NBI DMA and NBI TM
*/
#define NFP3800_CTMX_PKT_MUPENullMUPtr                       0x0000005c
#define   NFP3800_CTMX_PKT_MUPENullMUPtr_MUPENullMUPtr(_x)   (((_x) & 0x1fffffff) << 0)
#define   NFP3800_CTMX_PKT_MUPENullMUPtr_MUPENullMUPtr_of(_x) (((_x) >> 0) & 0x1fffffff)
/*----------------------------------------------------------------
  Register: MUPEEnableByteAlignedAddThread - Set to enable byte aligned add_thread commands
    Bits:  0    MUPEEnableByteAlignedAddThread(rw) - Set to enable byte aligned add_thread commands
*/
#define NFP3800_CTMX_PKT_MUPEEnableByteAlignedAddThread      0x00000060
#define   NFP3800_CTMX_PKT_MUPEEnableByteAlignedAddThread_MUPEEnableByteAlignedAddThread BIT(0)
/*----------------------------------------------------------------
  Register: MUPEEnableDMAPoll - Set to enable polling of DMA Queue. If the Queue is full, the ME will be pushed back an error/retry response.
    Bits:  0    MUPEEnableDMAPoll(rw) - Set to enable polling of DMA Queue. If the Queue is full, the ME will be pushed back an error/retry response.
*/
#define NFP3800_CTMX_PKT_MUPEEnableDMAPoll                   0x00000064
#define   NFP3800_CTMX_PKT_MUPEEnableDMAPoll_MUPEEnableDMAPoll BIT(0)
/*----------------------------------------------------------------
  Register: MUPEEnableDMALengthAndOffset - Set to enable addnl length and offset inputs into DMATo/FromMemPacket and DMATo/FromMemIndirect Cmds.
    Bits:  0    MUPEEnableDMALengthAndOffset(rw) - Set to enable additional length and offset inputs into DMATo/FromMemPacket and DMATo/FromMemIndirect Commands.
*/
#define NFP3800_CTMX_PKT_MUPEEnableDMALengthAndOffset        0x00000068
#define   NFP3800_CTMX_PKT_MUPEEnableDMALengthAndOffset_MUPEEnableDMALengthAndOffset BIT(0)
/*----------------------------------------------------------------
  Register: MUPEEnableMPullIDWithNoRead - Set to enable MpullIDs with NoRead. (Recommended for Performance)
    Bits:  0    MUPEEnableMPullIDWithNoRead(rw) - Set to enable MpullIDs with NoRead. (Recommended for Performance)
*/
#define NFP3800_CTMX_PKT_MUPEEnableMPullIDWithNoRead         0x0000006c
#define   NFP3800_CTMX_PKT_MUPEEnableMPullIDWithNoRead_MUPEEnableMPullIDWithNoRead BIT(0)
/*----------------------------------------------------------------
  Register: MUPEEnableDMAReadback - Issue a single read after writes, to ensure that writes have completed in the Bulk Engine
    Bits:  0    MUPEEnableDMAReadback(rw) - Issue a single read after writes, to ensure that writes have completed in the Bulk Engine
*/
#define NFP3800_CTMX_PKT_MUPEEnableDMAReadback               0x00000070
#define   NFP3800_CTMX_PKT_MUPEEnableDMAReadback_MUPEEnableDMAReadback BIT(0)
/*----------------------------------------------------------------
  Register: MUPEPacketLifeSpanPacketOwner - Packet Owner for the Packet Number whose lifespan we want to track. Program this prior to starting traffic, or wait sufficiently long for the update to take place.
    Bits:  2:0  MUPEPacketLifeSpanPacketOwner(rw) - Packet Owner for the Packet Number whose lifespan we want to track. Program this prior to starting traffic, or wait sufficiently long for the update to take place.
*/
#define NFP3800_CTMX_PKT_MUPEPacketLifeSpanPacketOwner       0x00000074
#define   NFP3800_CTMX_PKT_MUPEPacketLifeSpanPacketOwner_MUPEPacketLifeSpanPacketOwner(_x) (((_x) & 0x7) << 0)
#define   NFP3800_CTMX_PKT_MUPEPacketLifeSpanPacketOwner_MUPEPacketLifeSpanPacketOwner_of(_x) (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: MUPEPacketLifeSpanPacketNumber - Packet Number whose lifespan we want to track. Program this prior to starting traffic, or wait sufficiently long for the update to take place.
    Bits:  7:0  MUPEPacketLifeSpanPacketNumber(rw) - Packet Number whose lifespan we want to track. Program this prior to starting traffic, or wait sufficiently long for the update to take place.
*/
#define NFP3800_CTMX_PKT_MUPEPacketLifeSpanPacketNumber      0x00000078
#define   NFP3800_CTMX_PKT_MUPEPacketLifeSpanPacketNumber_MUPEPacketLifeSpanPacketNumber(_x) (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_PKT_MUPEPacketLifeSpanPacketNumber_MUPEPacketLifeSpanPacketNumber_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: MUPEConfiguredCredits[8] - MU PE Configured Credits
    Bits: 24:16 MUPEPacketCredits(rw) - Packet Credits (Max 512)
    Bits:  6:0  MUPEBufferCredits(rw) - Buffer Credits (Max 64)
*/
#define NFP3800_CTMX_PKT_MUPEConfiguredCredits(_x)           (0x00000080 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_CTMX_PKT_MUPEConfiguredCredits_MUPEPacketCredits(_x) (((_x) & 0x1ff) << 16)
#define   NFP3800_CTMX_PKT_MUPEConfiguredCredits_MUPEPacketCredits_of(_x) (((_x) >> 16) & 0x1ff)
#define   NFP3800_CTMX_PKT_MUPEConfiguredCredits_MUPEBufferCredits(_x) (((_x) & 0x7f) << 0)
#define   NFP3800_CTMX_PKT_MUPEConfiguredCredits_MUPEBufferCredits_of(_x) (((_x) >> 0) & 0x7f)
/*----------------------------------------------------------------
  Register: MUPEClearAllocPacketCount - LifeOfAPacket:(Clear) Allocated Packet Count
  Register: MUPEClearFirstSegmentRxPacketCount - LifeOfAPacket:(Clear) First Segment Received Count
  Register: MUPEClearLastSegmentRxPacketCount - LifeOfAPacket:(Clear) Last Segment Received Count
  Register: MUPEClearFirstSegmentSentToMEPacketCount - LifeOfAPacket:(Clear) First Segment Sent to ME Count
  Register: MUPEClearLastSegmentSentToMEPacketCount - LifeOfAPacket:(Clear) Last Segment Sent to ME Count
  Register: MUPEClearTxPacketCount - LifeOfAPacket:(Clear) Last PullID Received Count
  Register: MUPEClearFreePacketCount - LifeOfAPacket:(Clear) Free Packet Count
  Register: MUPEClearAddPacketTotalCount - (Clear) Total Number of packets added to PQ
  Register: MUPEClearCmdAddPacketTotalCount - (Clear) Total Number of packets added to PQ via AddPacket Cmd
  Register: MUPEClearMpushAddPacketTotalCount - (Clear) Total Number of packets added to PQ via MPush
    Bits: 31:0  MUPECountWithClear(rc) - 32 Bit Count that clears on read. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MUPEClearAllocPacketCount           0x00002800
#define NFP3800_CTMX_PKT_MUPEClearFirstSegmentRxPacketCount  0x00002804
#define NFP3800_CTMX_PKT_MUPEClearLastSegmentRxPacketCount   0x00002808
#define NFP3800_CTMX_PKT_MUPEClearFirstSegmentSentToMEPacketCount 0x0000280c
#define NFP3800_CTMX_PKT_MUPEClearLastSegmentSentToMEPacketCount 0x00002810
#define NFP3800_CTMX_PKT_MUPEClearTxPacketCount              0x00002814
#define NFP3800_CTMX_PKT_MUPEClearFreePacketCount            0x00002818
#define NFP3800_CTMX_PKT_MUPEClearAddPacketTotalCount        0x000028c0
#define NFP3800_CTMX_PKT_MUPEClearCmdAddPacketTotalCount     0x000028c4
#define NFP3800_CTMX_PKT_MUPEClearMpushAddPacketTotalCount   0x000028c8
#define   NFP3800_CTMX_PKT_MUPECount32WithClear_MUPECountWithClear(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_CTMX_PKT_MUPECount32WithClear_MUPECountWithClear_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MUPEActivePacketCount - Active Packet Count
  Register: MUPEActivePacketCountOwner[8] - Active Packet Count (Per Owner). Owner 3 maps to Owner 0.
  Register: MUPEActiveByteCount - Active Byte Count
  Register: MUPEActiveByteCountOwner[8] - Active Byte Count (Per Owner). Owner 3 maps to Owner 0.
  Register: MUPEPeakPacketCount - Peak Packet Count
  Register: MUPEPeakByteCount - Peak Byte Count
  Register: MUPEDMAsAvailableInDcacheCount - Number of DMA descriptors Available in DCache
    Bits: 31:0  MUPECount(ro) - 11 Bit Count. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MUPEActivePacketCount               0x0000281c
#define NFP3800_CTMX_PKT_MUPEActivePacketCountOwner(_x)      (0x00002820 + (0x4 * ((_x) & 0x7)))
#define NFP3800_CTMX_PKT_MUPEActiveByteCount                 0x00002840
#define NFP3800_CTMX_PKT_MUPEActiveByteCountOwner(_x)        (0x00002844 + (0x4 * ((_x) & 0x7)))
#define NFP3800_CTMX_PKT_MUPEPeakPacketCount                 0x00002864
#define NFP3800_CTMX_PKT_MUPEPeakByteCount                   0x0000288c
#define NFP3800_CTMX_PKT_MUPEDMAsAvailableInDcacheCount      0x000028cc
#define   NFP3800_CTMX_PKT_MUPECount11_MUPECount(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP3800_CTMX_PKT_MUPECount11_MUPECount_of(_x)      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MUPEClearPeakPacketCount - (Clear) Peak Packet Count
  Register: MUPEClearPeakPacketCountOwner[8] - (Clear) Peak Packet Count (Per Owner). Owner 3 maps to Owner 0.
  Register: MUPEClearPeakByteCount - (Clear) Peak Byte Count
  Register: MUPEClearPeakByteCountOwner[8] - (Clear) Peak Byte Count (Per Owner). Owner 3 maps to Owner 0.
  Register: MUPEClearFingerprintWasErroredCount - (Clear) Fingerprint was Errored Count
  Register: MUPEClearDMADescriptorWasErroredCount - (Clear) DMADescriptor was Errored Count
    Bits: 31:0  MUPECountWithClear(rc) - 11 Bit Count that clears on read. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MUPEClearPeakPacketCount            0x00002868
#define NFP3800_CTMX_PKT_MUPEClearPeakPacketCountOwner(_x)   (0x0000286c + (0x4 * ((_x) & 0x7)))
#define NFP3800_CTMX_PKT_MUPEClearPeakByteCount              0x00002890
#define NFP3800_CTMX_PKT_MUPEClearPeakByteCountOwner(_x)     (0x00002894 + (0x4 * ((_x) & 0x7)))
#define NFP3800_CTMX_PKT_MUPEClearFingerprintWasErroredCount 0x000028d4
#define NFP3800_CTMX_PKT_MUPEClearDMADescriptorWasErroredCount 0x000028d8
#define   NFP3800_CTMX_PKT_MUPECount11WithClear_MUPECountWithClear(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_CTMX_PKT_MUPECount11WithClear_MUPECountWithClear_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MUPEWQActiveCount - Number of Entries currently present in WQ
    Bits:  7:0  MUPECount8(ro) - 8 Bit Count that clears on read. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MUPEWQActiveCount                   0x000028b4
#define   NFP3800_CTMX_PKT_MUPEWQActiveCount_MUPECount8(_x)  (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_PKT_MUPEWQActiveCount_MUPECount8_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: MUPEPQ0ActiveCount - Number of Entries currently present in PQ0
  Register: MUPEPQ1ActiveCount - Number of Entries currently present in PQ1
    Bits:  9:0  MUPECount10(ro) - 10 Bit Count that clears on read. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MUPEPQ0ActiveCount                  0x000028b8
#define NFP3800_CTMX_PKT_MUPEPQ1ActiveCount                  0x000028bc
#define   NFP3800_CTMX_PKT_MUPECount10_MUPECount10(_x)       (((_x) & 0x3ff) << 0)
#define   NFP3800_CTMX_PKT_MUPECount10_MUPECount10_of(_x)    (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: MUPEDMARequestCreditsAvailable - Incoming DMA Request Credits Avail (Credits Avail in DMA RH)
    Bits:  5:0  MUPECount6(ro) - 6 Bit Count that clears on read. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MUPEDMARequestCreditsAvailable      0x000028d0
#define   NFP3800_CTMX_PKT_MUPEDMARequestCreditsAvailable_MUPECount6(_x) (((_x) & 0x3f) << 0)
#define   NFP3800_CTMX_PKT_MUPEDMARequestCreditsAvailable_MUPECount6_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: MUPEPacketLifeSpanCount - Packet LifeSpan count for Packet Number and Packet Owner described in MUPEPacketLifeSpanPacketNumber and MUPEPacketLifeSpanPacketOwner CSRs.
    Bits: 31:0  MUPECount(ro) - 32 Bit Count. Multiple registers map to this type, look at AddressMap for description.
*/
#define NFP3800_CTMX_PKT_MUPEPacketLifeSpanCount             0x000028dc
#define   NFP3800_CTMX_PKT_MUPEPacketLifeSpanCount_MUPECount(_x) (((_x) & 0xffffffff) << 0)
#define   NFP3800_CTMX_PKT_MUPEPacketLifeSpanCount_MUPECount_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MUPEClearAssertionsFired - This reg sets a bit per assertions_drive module that was fired
    Bits: 15:0  MUPEClearAssertionsFired(rc) - Each bit indicates which assertions_drive module had an assertion fire. Register is cleared on read.
*/
#define NFP3800_CTMX_PKT_MUPEClearAssertionsFired            0x00002a00
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsFired_MUPEClearAssertionsFired(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsFired_MUPEClearAssertionsFired_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: MUPEClearAssertionsBus - Some useful assertions put into a reg to avoid binary search
    Bits: 30    PatInvalidFree(rc) - PatInvalidFree
    Bits: 29    PatPatTranslateInvalidPacketAtomic(rc) - PatPatTranslateInvalidPacketAtomic
    Bits: 28    PatPatTranslateInvalidPacketBulk(rc) - PatPatTranslateInvalidPacketBulk
    Bits: 27    PatPatTranslateInvalidPacketDma(rc) - PatPatTranslateInvalidPacketDma
    Bits: 26    PatPatTranslateInvalidPacketDcMpushCh0a(rc) - PatPatTranslateInvalidPacketDcMpushCh0a
    Bits: 25    PatPatTranslateInvalidPacketDcMpushCh0b(rc) - PatPatTranslateInvalidPacketDcMpushCh0b
    Bits: 24    PatPatTranslateInvalidPacketDcMpushCh1a(rc) - PatPatTranslateInvalidPacketDcMpushCh1a
    Bits: 23    PatPatTranslateInvalidPacketDcMpushCh1b(rc) - PatPatTranslateInvalidPacketDcMpushCh1b
    Bits: 22    PatPatTranslateInvalidPacketDcMpullCh0(rc) - PatPatTranslateInvalidPacketDcMpullCh0
    Bits: 21    PatPatTranslateInvalidPacketDcMpullCh1(rc) - PatPatTranslateInvalidPacketDcMpullCh1
    Bits: 20    PatPatTranslateInvalidPacketDcTpushCh0(rc) - PatPatTranslateInvalidPacketDcTpushCh0
    Bits: 19    PatPatTranslateInvalidPacketDcTpushCh1(rc) - PatPatTranslateInvalidPacketDcTpushCh1
    Bits: 18    PatPatTranslateInvalidPacketDcMptrCh0(rc) - PatPatTranslateInvalidPacketDcMptrCh0
    Bits: 17    PatPatTranslateInvalidPacketDcMptrCh1(rc) - PatPatTranslateInvalidPacketDcMptrCh1
    Bits: 16    PatPatTranslateInvalidOffsetAtomic(rc) - PatPatTranslateInvalidOffsetAtomic
    Bits: 15    PatPatTranslateInvalidOffsetBulk(rc) - PatPatTranslateInvalidOffsetBulk
    Bits: 14    PatPatTranslateInvalidOffsetDma(rc) - PatPatTranslateInvalidOffsetDma
    Bits: 13    PatPatTranslateInvalidOffsetDcMpushCh0a(rc) - PatPatTranslateInvalidOffsetDcMpushCh0a
    Bits: 12    PatPatTranslateInvalidOffsetDcMpushCh0b(rc) - PatPatTranslateInvalidOffsetDcMpushCh0b
    Bits: 11    PatPatTranslateInvalidOffsetDcMpushCh1a(rc) - PatPatTranslateInvalidOffsetDcMpushCh1a
    Bits: 10    PatPatTranslateInvalidOffsetDcMpushCh1b(rc) - PatPatTranslateInvalidOffsetDcMpushCh1b
    Bits:  9    PatPatTranslateInvalidOffsetDcMpullCh0(rc) - PatPatTranslateInvalidOffsetDcMpullCh0
    Bits:  8    PatPatTranslateInvalidOffsetDcMpullCh1(rc) - PatPatTranslateInvalidOffsetDcMpullCh1
    Bits:  7    PatPatTranslateInvalidOffsetDcTpushCh0(rc) - PatPatTranslateInvalidOffsetDcTpushCh0
    Bits:  6    PatPatTranslateInvalidOffsetDcTpushCh1(rc) - PatPatTranslateInvalidOffsetDcTpushCh1
    Bits:  5    PatPatTranslateInvalidOffsetDcMptrCh0(rc) - PatPatTranslateInvalidOffsetDcMptrCh0
    Bits:  4    PatPatTranslateInvalidOffsetDcMptrCh1(rc) - PatPatTranslateInvalidOffsetDcMptrCh1
    Bits:  3    TargetCommandHandlerWqFifoFull(rc) - TargetCommandHandlerWqFifoFull
    Bits:  2    DcacheInterfaceTopWqWqEmpty(rc) - DcacheInterfaceTopWqWqEmpty
    Bits:  1    DcacheInterfaceTopWqPq0FifoFullOnCommand(rc) - DcacheInterfaceTopWqPq0FifoFullOnCommand
    Bits:  0    DcacheInterfaceTopWqPq0FifoFullOnPush(rc) - DcacheInterfaceTopWqPq0FifoFullOnPush
*/
#define NFP3800_CTMX_PKT_MUPEClearAssertionsBus              0x00002a04
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatInvalidFree BIT(30)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidPacketAtomic BIT(29)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidPacketBulk BIT(28)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidPacketDma BIT(27)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidPacketDcMpushCh0a BIT(26)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidPacketDcMpushCh0b BIT(25)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidPacketDcMpushCh1a BIT(24)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidPacketDcMpushCh1b BIT(23)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidPacketDcMpullCh0 BIT(22)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidPacketDcMpullCh1 BIT(21)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidPacketDcTpushCh0 BIT(20)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidPacketDcTpushCh1 BIT(19)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidPacketDcMptrCh0 BIT(18)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidPacketDcMptrCh1 BIT(17)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidOffsetAtomic BIT(16)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidOffsetBulk BIT(15)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidOffsetDma BIT(14)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidOffsetDcMpushCh0a BIT(13)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidOffsetDcMpushCh0b BIT(12)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidOffsetDcMpushCh1a BIT(11)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidOffsetDcMpushCh1b BIT(10)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidOffsetDcMpullCh0 BIT(9)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidOffsetDcMpullCh1 BIT(8)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidOffsetDcTpushCh0 BIT(7)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidOffsetDcTpushCh1 BIT(6)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidOffsetDcMptrCh0 BIT(5)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_PatPatTranslateInvalidOffsetDcMptrCh1 BIT(4)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_TargetCommandHandlerWqFifoFull BIT(3)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_DcacheInterfaceTopWqWqEmpty BIT(2)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_DcacheInterfaceTopWqPq0FifoFullOnCommand BIT(1)
#define   NFP3800_CTMX_PKT_MUPEClearAssertionsBus_DcacheInterfaceTopWqPq0FifoFullOnPush BIT(0)
/*----------------------------------------------------------------
  Register: MUPEAvailableCredits[8] - MU PE Available Credits
  Register: MUPEReturnCredits[8] - MU PE Return Credits
    Bits: 25:16 MUPEPacketCredits(ro) - Packet Credits
    Bits:  7:0  MUPEBufferCredits(ro) - Buffer Credits
*/
#define NFP3800_CTMX_PKT_MUPEAvailableCredits(_x)            (0x00002c00 + (0x4 * ((_x) & 0x7)))
#define NFP3800_CTMX_PKT_MUPEReturnCredits(_x)               (0x00002c20 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_CTMX_PKT_MUPECredits_MUPEPacketCredits(_x) (((_x) & 0x3ff) << 16)
#define   NFP3800_CTMX_PKT_MUPECredits_MUPEPacketCredits_of(_x) (((_x) >> 16) & 0x3ff)
#define   NFP3800_CTMX_PKT_MUPECredits_MUPEBufferCredits(_x) (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_PKT_MUPECredits_MUPEBufferCredits_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: MUPEClearWQ - Clear WQ FIFO
    Bits:  0    MUPEClearWQ(wo) - Clear WQ FIFO
*/
#define NFP3800_CTMX_PKT_MUPEClearWQ                         0x00003000
#define   NFP3800_CTMX_PKT_MUPEClearWQ_MUPEClearWQ           BIT(0)
/*----------------------------------------------------------------
  Register: MUPEClearPQ0 - Clear PQ0 FIFO
  Register: MUPEClearPQ1 - Clear PQ1 FIFO
    Bits:  0    MUPEClearPQ0(wo) - Clear PQ FIFO
*/
#define NFP3800_CTMX_PKT_MUPEClearPQ0                        0x00003004
#define NFP3800_CTMX_PKT_MUPEClearPQ1                        0x00003008
#define   NFP3800_CTMX_PKT_MUPEClearPQ0_MUPEClearPQ0         BIT(0)
/*----------------------------------------------------------------
  Register: MUPEAssertionsConfig - Assertion Reporting Configuration Reg for Packet Engine
    Bits: 31:30 MUPEEnableFSMs(rw) - Enable FSMS (Only assertions drive 0 and 1 have FSM status connected to them
    Bits: 23:16 MUPEFSMConfig(rw) - FSM Config
    Bits: 15:0  MUPEDisableAssertions(rw) - Disable Assertions
*/
#define NFP3800_CTMX_PKT_MUPEAssertionsConfig                0x00004000
#define   NFP3800_CTMX_PKT_MUPEAssertionsConfig_MUPEEnableFSMs(_x) (((_x) & 0x3) << 30)
#define   NFP3800_CTMX_PKT_MUPEAssertionsConfig_MUPEEnableFSMs_of(_x) (((_x) >> 30) & 0x3)
#define   NFP3800_CTMX_PKT_MUPEAssertionsConfig_MUPEFSMConfig(_x) (((_x) & 0xff) << 16)
#define   NFP3800_CTMX_PKT_MUPEAssertionsConfig_MUPEFSMConfig_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_CTMX_PKT_MUPEAssertionsConfig_MUPEDisableAssertions(_x) (((_x) & 0xffff) << 0)
#define   NFP3800_CTMX_PKT_MUPEAssertionsConfig_MUPEDisableAssertions_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: MUPEMPullBusLBConfig[2] - Load Balance Config for CH X
    Bits:  0    MUPELBConfig(rw) - Choose between static and dynamic load balancing
*/
#define NFP3800_CTMX_PKT_MUPEMPullBusLBConfig(_x)            (0x00006c00 + (0x4 * ((_x) & 0x1)))
#define   NFP3800_CTMX_PKT_MUPEMPullBusLBConfig_MUPELBConfig BIT(0)
#define     NFP3800_CTMX_PKT_MUPEMPullBusLBConfig_MUPELBConfig_use_dynamic (0 << 0)
#define     NFP3800_CTMX_PKT_MUPEMPullBusLBConfig_MUPELBConfig_use_static BIT(0)
/*----------------------------------------------------------------
  Register: MUPEMPullBusDWRRCreditsConfig[2] - Dynamic Load Balance Config (DWRR) for CH X
  Register: MUPECommandBusDWRRCreditsConfig - Dynamic Load Balance Config (DWRR) for CMD CH
  Register: MUPECommandMasterDWRRCreditsConfig - Dynamic Load Balance Config (DWRR) for CMD CH
  Register: MUPEDmaBusDWRRCreditsConfig - Dynamic Load Balance Config (DWRR) for DMA Bus
  Register: MUPEDmaMasterDWRRCreditsConfig - Dynamic Load Balance Config (DWRR) for DMA Master
    Bits: 31    Disable(wo) - Disable
    Bits: 29:28 Select(wo) - Select
    Bits: 27:20 WriteData(wo) - (Write data) Each unit corresponds to a single cycle over the bus. For MPullData, each cycle over the bus is 8B. To write new values into the LoadBalancer, the DWRR must first be disabled. The new values must be written in, while the DWRR is in the disabled state. The DWRR can be re-enabled for the new values to take effect.
*/
#define NFP3800_CTMX_PKT_MUPEMPullBusDWRRCreditsConfig(_x)   (0x00006c08 + (0x4 * ((_x) & 0x1)))
#define NFP3800_CTMX_PKT_MUPECommandBusDWRRCreditsConfig     0x00006c2c
#define NFP3800_CTMX_PKT_MUPECommandMasterDWRRCreditsConfig  0x00006c34
#define NFP3800_CTMX_PKT_MUPEDmaBusDWRRCreditsConfig         0x00006c3c
#define NFP3800_CTMX_PKT_MUPEDmaMasterDWRRCreditsConfig      0x00006c44
#define   NFP3800_CTMX_PKT_DsfCppDynamicLoadBalancerConfig_Disable BIT(31)
#define   NFP3800_CTMX_PKT_DsfCppDynamicLoadBalancerConfig_Select(_x) (((_x) & 0x3) << 28)
#define   NFP3800_CTMX_PKT_DsfCppDynamicLoadBalancerConfig_Select_of(_x) (((_x) >> 28) & 0x3)
#define   NFP3800_CTMX_PKT_DsfCppDynamicLoadBalancerConfig_WriteData(_x) (((_x) & 0xff) << 20)
#define   NFP3800_CTMX_PKT_DsfCppDynamicLoadBalancerConfig_WriteData_of(_x) (((_x) >> 20) & 0xff)
/*----------------------------------------------------------------
  Register: MUPEMPullBusDWRRDisableGte - Mpull Bus DWRR Disable GTE for CH0, CH1)
    Bits:  1:0  MUPEMPullBusDWRRDisableGte(rw) - Disable GTE
*/
#define NFP3800_CTMX_PKT_MUPEMPullBusDWRRDisableGte          0x00006c10
#define   NFP3800_CTMX_PKT_MUPEMPullBusDWRRDisableGte_MUPEMPullBusDWRRDisableGte(_x) (((_x) & 0x3) << 0)
#define   NFP3800_CTMX_PKT_MUPEMPullBusDWRRDisableGte_MUPEMPullBusDWRRDisableGte_of(_x) (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: MUPEMPullBusStaticLBConfig[2] - Static Load Balance Config for MPULL CH X
    Bits: 31:24 MapIfMaskMatchFails(wo) - Indexed by master_id[3;0] - result is the load balance answer if mask/match fails
    Bits: 23:16 MapIfMaskMatchPasses(wo) - Indexed by master_id[3;0] - result is the load balance answer if mask/match passes
    Bits: 15:8  ConfigMask(wo) - ANDed with island_id[6;0],master_id[2;2], and compare with mask&match - also if match&~mask then at least one must be set
    Bits:  7:0  ConfigMatch(wo) - Combined with match and island_id and top 2 bits of master_id
*/
#define NFP3800_CTMX_PKT_MUPEMPullBusStaticLBConfig(_x)      (0x00006c14 + (0x4 * ((_x) & 0x1)))
#define   NFP3800_CTMX_PKT_MUPEMPullBusStaticLBConfig_MapIfMaskMatchFails(_x) (((_x) & 0xff) << 24)
#define   NFP3800_CTMX_PKT_MUPEMPullBusStaticLBConfig_MapIfMaskMatchFails_of(_x) (((_x) >> 24) & 0xff)
#define   NFP3800_CTMX_PKT_MUPEMPullBusStaticLBConfig_MapIfMaskMatchPasses(_x) (((_x) & 0xff) << 16)
#define   NFP3800_CTMX_PKT_MUPEMPullBusStaticLBConfig_MapIfMaskMatchPasses_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_CTMX_PKT_MUPEMPullBusStaticLBConfig_ConfigMask(_x) (((_x) & 0xff) << 8)
#define   NFP3800_CTMX_PKT_MUPEMPullBusStaticLBConfig_ConfigMask_of(_x) (((_x) >> 8) & 0xff)
#define   NFP3800_CTMX_PKT_MUPEMPullBusStaticLBConfig_ConfigMatch(_x) (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_PKT_MUPEMPullBusStaticLBConfig_ConfigMatch_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: MUPEMPullBusRLConfig[4] - Rate Limiter Config for CH0A (0), CH0B (1), CH1A(2), CH1B(3)
    Bits: 13:8  Increment(wo) - Credit to add every cycle
    Bits:  7:0  Max(wo) - Maximum value for the credit.
*/
#define NFP3800_CTMX_PKT_MUPEMPullBusRLConfig(_x)            (0x00006c1c + (0x4 * ((_x) & 0x3)))
#define   NFP3800_CTMX_PKT_MUPEMPullBusRLConfig_Increment(_x) (((_x) & 0x3f) << 8)
#define   NFP3800_CTMX_PKT_MUPEMPullBusRLConfig_Increment_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP3800_CTMX_PKT_MUPEMPullBusRLConfig_Max(_x)      (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_PKT_MUPEMPullBusRLConfig_Max_of(_x)   (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: MUPECommandBusDWRRDisableGte - Command Bus DWRR Disable GTE
    Bits:  0    MUPECommandBusDWRRDisableGte(rw) - Disable GTE
*/
#define NFP3800_CTMX_PKT_MUPECommandBusDWRRDisableGte        0x00006c30
#define   NFP3800_CTMX_PKT_MUPECommandBusDWRRDisableGte_MUPECommandBusDWRRDisableGte BIT(0)
/*----------------------------------------------------------------
  Register: MUPECommandMasterDWRRDisableGte - Command Master DWRR Disable GTE
    Bits:  0    MUPECommandMasterDWRRDisableGte(rw) - Disable GTE
*/
#define NFP3800_CTMX_PKT_MUPECommandMasterDWRRDisableGte     0x00006c38
#define   NFP3800_CTMX_PKT_MUPECommandMasterDWRRDisableGte_MUPECommandMasterDWRRDisableGte BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDmaBusDWRRDisableGte - Dma Bus DWRR Disable GTE
    Bits:  0    MUPEDmaBusDWRRDisableGte(rw) - Disable GTE
*/
#define NFP3800_CTMX_PKT_MUPEDmaBusDWRRDisableGte            0x00006c40
#define   NFP3800_CTMX_PKT_MUPEDmaBusDWRRDisableGte_MUPEDmaBusDWRRDisableGte BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDmaMasterDWRRDisableGte - Dma Master DWRR Disable GTE
    Bits:  0    MUPEDmaMasterDWRRDisableGte(rw) - Disable GTE
*/
#define NFP3800_CTMX_PKT_MUPEDmaMasterDWRRDisableGte         0x00006c48
#define   NFP3800_CTMX_PKT_MUPEDmaMasterDWRRDisableGte_MUPEDmaMasterDWRRDisableGte BIT(0)
/*----------------------------------------------------------------
  Register: MUPERateSelect - Select bus whose rate needs to be measured
    Bits: 31    MUPERateEnable(rw) - Enable Rate Measurement on bus selected via Select bits
    Bits:  8:4  MUPERateSelect(rw) - Choose bus whose rate needs to be measured
    Bits:  2:0  MUPERateResolution(rw) - Resolution at which rate needs to be measured
*/
#define NFP3800_CTMX_PKT_MUPERateSelect                      0x00006c4c
#define   NFP3800_CTMX_PKT_MUPERateSelect_MUPERateEnable     BIT(31)
#define   NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect(_x) (((_x) & 0x1f) << 4)
#define   NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_of(_x) (((_x) >> 4) & 0x1f)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Master_Push_Data_0A (0)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Master_Push_Data_0B (1)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Master_Push_Data_1A (2)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Master_Push_Data_1B (3)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Master_Pull_Data_0A (4)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Master_Pull_Data_0B (5)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Master_Pull_Data_1A (6)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Master_Pull_Data_1B (7)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Master_Pull_ID_0 (8)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Master_Pull_ID_1 (9)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Master_Command (10)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Target_Command_0 (11)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Target_Command_1 (12)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Target_Push_Data_0 (13)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_Target_Push_Data_1 (14)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_DCache_Request_0 (15)
#define     NFP3800_CTMX_PKT_MUPERateSelect_MUPERateSelect_DCache_Request_1 (26)
#define   NFP3800_CTMX_PKT_MUPERateSelect_MUPERateResolution(_x) (((_x) & 0x7) << 0)
#define   NFP3800_CTMX_PKT_MUPERateSelect_MUPERateResolution_of(_x) (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: MUPERate - Measured rate for selected bus
    Bits: 31:0  MUPERate(ro) - Rate measured on bus selected using RateSelect register
*/
#define NFP3800_CTMX_PKT_MUPERate                            0x00006c50
#define   NFP3800_CTMX_PKT_MUPERate_MUPERate(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP3800_CTMX_PKT_MUPERate_MUPERate_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MUPEWQ - ECC Control Reg for the WQ
  Register: MUPEPQ0 - ECC Control Reg for PQ0
  Register: MUPEPQ1 - ECC Control Reg for PQ1
  Register: MUPEPP - ECC Control Reg for Mpush, Tpush Memories
    Bits: 10    TStab0(rw) - SRAM0: Stability test during Read operation. Low during functional mode.
    Bits:  9    TWbt0(rw) - SRAM0: Weak Bit Test. Low during functional mode. Used for margin test by implementing small speed up in Sense-amp timings during read operations and small decrease in Wordline pulse-width during write operation.
    Bits:  8    MaTpb0(rw) - SRAM0: Margin Adjust Port B.
    Bits:  7    MaTpa0(rw) - SRAM0: Margin Adjust Port A.
    Bits:  6:5  MaWras0(rw) - SRAM0: Margin Adjust: Write Assist timings.
    Bits:  4:3  MaWl0(rw) - SRAM0: Margin Adjust: Wordline pulse width only. See SRAMs specs.
    Bits:  2:1  MaSawl0(rw) - SRAM0: Margin Adjust: Sense-Amp timing and Wordline pulse width. See SRAMs specs.
    Bits:  0    MaWrasd0(rw) - SRAM0: Margin Adjust: Write Assist function disable. See SRAMs specs.
*/
#define NFP3800_CTMX_PKT_MUPEWQ                              0x00008000
#define NFP3800_CTMX_PKT_MUPEPQ0                             0x00008004
#define NFP3800_CTMX_PKT_MUPEPQ1                             0x00008008
#define NFP3800_CTMX_PKT_MUPEPP                              0x0000800c
#define   NFP3800_CTMX_PKT_ECCSramCnt_TStab0                 BIT(10)
#define   NFP3800_CTMX_PKT_ECCSramCnt_TWbt0                  BIT(9)
#define   NFP3800_CTMX_PKT_ECCSramCnt_MaTpb0                 BIT(8)
#define   NFP3800_CTMX_PKT_ECCSramCnt_MaTpa0                 BIT(7)
#define   NFP3800_CTMX_PKT_ECCSramCnt_MaWras0(_x)            (((_x) & 0x3) << 5)
#define   NFP3800_CTMX_PKT_ECCSramCnt_MaWras0_of(_x)         (((_x) >> 5) & 0x3)
#define   NFP3800_CTMX_PKT_ECCSramCnt_MaWl0(_x)              (((_x) & 0x3) << 3)
#define   NFP3800_CTMX_PKT_ECCSramCnt_MaWl0_of(_x)           (((_x) >> 3) & 0x3)
#define   NFP3800_CTMX_PKT_ECCSramCnt_MaSawl0(_x)            (((_x) & 0x3) << 1)
#define   NFP3800_CTMX_PKT_ECCSramCnt_MaSawl0_of(_x)         (((_x) >> 1) & 0x3)
#define   NFP3800_CTMX_PKT_ECCSramCnt_MaWrasd0               BIT(0)
/*----------------------------------------------------------------
  Register: MUPEDropOnRetry - MU PE Drop Packet Processing Complete on Retry
    Bits:  0    MUPEDropOnRetry(rw) - Drop Packet Processing Complete On Retry
*/
#define NFP3800_CTMX_PKT_MUPEDropOnRetry                     0x00008280
#define   NFP3800_CTMX_PKT_MUPEDropOnRetry_MUPEDropOnRetry   BIT(0)
/*----------------------------------------------------------------
  Register: MUPERaiseEventOnRetry - MU PE Raise Event on Retry
    Bits:  0    MUPERaiseEventOnRetry(rw) - Raise Event when Packet Processing Complete with Retry bit set is received
*/
#define NFP3800_CTMX_PKT_MUPERaiseEventOnRetry               0x00008284
#define   NFP3800_CTMX_PKT_MUPERaiseEventOnRetry_MUPERaiseEventOnRetry BIT(0)
#define     NFP3800_CTMX_PKT_MUPERaiseEventOnRetry_MUPERaiseEventOnRetry_DontRaiseEventOnRetry (0 << 0)
#define     NFP3800_CTMX_PKT_MUPERaiseEventOnRetry_MUPERaiseEventOnRetry_RaiseEventOnRetry BIT(0)
/*----------------------------------------------------------------
  Register: MUPEPPCWasRetried - MU PE PPC Was Retried
    Bits:  0    MUPEPPCWasRetried(rc) - PPC Was Retried. Clears on Read.
*/
#define NFP3800_CTMX_PKT_MUPEPPCWasRetried                   0x00008288
#define   NFP3800_CTMX_PKT_MUPEPPCWasRetried_MUPEPPCWasRetried BIT(0)
/*----------------------------------------------------------------
  Register: XpbErrConfig - XPB Error Configuration Register
    Bits: 15:1  XpbMaxTimeout(rw) - XPB Max Timeout. Value to wait for an XPB response before declaring a timeout error.
    Bits:  0    XpbWrIfErr(rw) - If '1', execute XPB writes if data has data_error. If '0', drop XPB writes for data with data_error. If write is dropped, log the address information in the XPBWrErrStat csr.
*/
#define NFP3800_CTMX_MISC_XpbErrConfig                       0x00000000
#define   NFP3800_CTMX_MISC_XpbErrConfig_XpbMaxTimeout(_x)   (((_x) & 0x7fff) << 1)
#define   NFP3800_CTMX_MISC_XpbErrConfig_XpbMaxTimeout_of(_x) (((_x) >> 1) & 0x7fff)
#define   NFP3800_CTMX_MISC_XpbErrConfig_XpbWrIfErr          BIT(0)
/*----------------------------------------------------------------
  Register: XpbTOErrStat - XPB Timeout Error Status Register
    Bits: 14    XpbTOErr(rwc) - XPB Timeout Error. If '1', the requested XPB CLUSTER_ID and DEVICE_ID combination did not result in a XPB response on the XPB Bus. Requesting a non-existent XPB target is not allowed on the XPB bus. If '0', no current error.
    Bits: 13:8  XpbClId(rwc) - This is the XPB CLUSTER ID of the XPB request that caused the XPB Timeout Error.
    Bits:  7:0  XpbDevId(rwc) - This is the XPB Device ID of the XPB request that caused the XPB Timeout Error.
*/
#define NFP3800_CTMX_MISC_XpbTOErrStat                       0x00000004
#define   NFP3800_CTMX_MISC_XpbTOErrStat_XpbTOErr            BIT(14)
#define   NFP3800_CTMX_MISC_XpbTOErrStat_XpbClId(_x)         (((_x) & 0x3f) << 8)
#define   NFP3800_CTMX_MISC_XpbTOErrStat_XpbClId_of(_x)      (((_x) >> 8) & 0x3f)
#define   NFP3800_CTMX_MISC_XpbTOErrStat_XpbDevId(_x)        (((_x) & 0xff) << 0)
#define   NFP3800_CTMX_MISC_XpbTOErrStat_XpbDevId_of(_x)     (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: XpbWrErrStat - XPB Write Error Status Register
    Bits: 30    XpbWrErrMulti(rwc) - Indicates multiple XPB writes were dropped due to a pull data error. Only the XPB information for the first dropped write is logged.
    Bits: 29    XpbWrErrSingle(rwc) - Indicates a single XPB write was dropped due to a pull data error.
    Bits: 28    XpbWrErrGlobal(rwc) - Global_access bit logged from first dropped XPB write.
    Bits: 27:22 XpbWrErrTgtIsld(rwc) - Target island logged from first dropped XPB write.
    Bits: 21:20 XpbWrErrSlaveIdx(rwc) - Slave index logged from first dropped XPB write.
    Bits: 19:14 XpbWrErrDevID(rwc) - DeviceID logged from first dropped XPB write.
    Bits: 13:0  XpbWrErrRegAddr(rwc) - Register address logged from first dropped XPB write.
*/
#define NFP3800_CTMX_MISC_XpbWrErrStat                       0x00000008
#define   NFP3800_CTMX_MISC_XpbWrErrStat_XpbWrErrMulti       BIT(30)
#define   NFP3800_CTMX_MISC_XpbWrErrStat_XpbWrErrSingle      BIT(29)
#define   NFP3800_CTMX_MISC_XpbWrErrStat_XpbWrErrGlobal      BIT(28)
#define   NFP3800_CTMX_MISC_XpbWrErrStat_XpbWrErrTgtIsld(_x) (((_x) & 0x3f) << 22)
#define   NFP3800_CTMX_MISC_XpbWrErrStat_XpbWrErrTgtIsld_of(_x) (((_x) >> 22) & 0x3f)
#define   NFP3800_CTMX_MISC_XpbWrErrStat_XpbWrErrSlaveIdx(_x) (((_x) & 0x3) << 20)
#define   NFP3800_CTMX_MISC_XpbWrErrStat_XpbWrErrSlaveIdx_of(_x) (((_x) >> 20) & 0x3)
#define   NFP3800_CTMX_MISC_XpbWrErrStat_XpbWrErrDevID(_x)   (((_x) & 0x3f) << 14)
#define   NFP3800_CTMX_MISC_XpbWrErrStat_XpbWrErrDevID_of(_x) (((_x) >> 14) & 0x3f)
#define   NFP3800_CTMX_MISC_XpbWrErrStat_XpbWrErrRegAddr(_x) (((_x) & 0x3fff) << 0)
#define   NFP3800_CTMX_MISC_XpbWrErrStat_XpbWrErrRegAddr_of(_x) (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: Ring_[16]_Base - Ring X Base Register
    Bits: 31:29 Size(rw) - Size of ring in words
    Bits: 28    StatusType(rw) - Status type. This bit controls the type of status reported on the status bus for the ring.
    Bits: 17:9  BaseAddress(rw) - The Base Address. This value must be written to be consistent with the requirement that the Ring is aligned to its size. For example, if the Ring has 512 longwords, bits 10:9 must be written to 0.
*/
#define NFP3800_CTMX_MISC_Ring_Base(_x)                      (0x00000100 + (0x10 * ((_x) & 0xf)))
#define   NFP3800_CTMX_MISC_Ring_Base_Size(_x)               (((_x) & 0x7) << 29)
#define   NFP3800_CTMX_MISC_Ring_Base_Size_of(_x)            (((_x) >> 29) & 0x7)
#define     NFP3800_CTMX_MISC_Ring_Base_Size_128             (0)
#define     NFP3800_CTMX_MISC_Ring_Base_Size_256             (1)
#define     NFP3800_CTMX_MISC_Ring_Base_Size_512             (2)
#define     NFP3800_CTMX_MISC_Ring_Base_Size_1k              (3)
#define     NFP3800_CTMX_MISC_Ring_Base_Size_2k              (4)
#define     NFP3800_CTMX_MISC_Ring_Base_Size_4k              (5)
#define     NFP3800_CTMX_MISC_Ring_Base_Size_8k              (6)
#define     NFP3800_CTMX_MISC_Ring_Base_Size_16k             (7)
#define   NFP3800_CTMX_MISC_Ring_Base_StatusType             BIT(28)
#define     NFP3800_CTMX_MISC_Ring_Base_StatusType_Empty     (0 << 28)
#define     NFP3800_CTMX_MISC_Ring_Base_StatusType_Full      BIT(28)
#define   NFP3800_CTMX_MISC_Ring_Base_BaseAddress(_x)        (((_x) & 0x1ff) << 9)
#define   NFP3800_CTMX_MISC_Ring_Base_BaseAddress_of(_x)     (((_x) >> 9) & 0x1ff)
/*----------------------------------------------------------------
  Register: Ring_[16]_Head - Ring X Head Register
    Bits: 16:2  HeadOffset(rw) - Head pointer
*/
#define NFP3800_CTMX_MISC_Ring_Head(_x)                      (0x00000104 + (0x10 * ((_x) & 0xf)))
#define   NFP3800_CTMX_MISC_Ring_Head_HeadOffset(_x)         (((_x) & 0x7fff) << 2)
#define   NFP3800_CTMX_MISC_Ring_Head_HeadOffset_of(_x)      (((_x) >> 2) & 0x7fff)
/*----------------------------------------------------------------
  Register: Ring_[16]_Tail - Ring X Tail Register
    Bits: 16:2  TailOffset(rw) - Tail pointer
*/
#define NFP3800_CTMX_MISC_Ring_Tail(_x)                      (0x00000108 + (0x10 * ((_x) & 0xf)))
#define   NFP3800_CTMX_MISC_Ring_Tail_TailOffset(_x)         (((_x) & 0x7fff) << 2)
#define   NFP3800_CTMX_MISC_Ring_Tail_TailOffset_of(_x)      (((_x) >> 2) & 0x7fff)

#endif /* NFP3800_LONGNAMES */


#endif /* NFP3800_CTM_H */
