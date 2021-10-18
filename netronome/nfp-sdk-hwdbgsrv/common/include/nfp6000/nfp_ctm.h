/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_CTM_H
#define NFP6000_CTM_H

/*================================================================*/
/* TGT 7                                                          */

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + NFP_CTM_MEM */
#define NFP_CTM_MEM                                          (0x000000)

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP_XPB_ISLAND(island))          */

#define CTMX_BASE                                            (0x60000)

/* XPB_BASE + NFP_CTMX_CFG + NFP_CTMX_CFG_* */
#define NFP_CTMX_CFG                                         (CTMX_BASE + 0x000000)
/* XPB_BASE + NFP_CTMX_PKT + NFP_CTMX_PKT_* */
#define NFP_CTMX_PKT                                         (CTMX_BASE + 0x010000)
/* XPB_BASE + NFP_CTMX_MISC + NFP_CTMX_MISC_* */
#define NFP_CTMX_MISC                                        (CTMX_BASE + 0x020000)
/* XPB_BASE + NFP_CTMX_DCACHE_ECC + (nfp_ecc.h) */
#define NFP_CTMX_DCACHE_ECC                                  (CTMX_BASE + 0x030000)

/*================================================================*/


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: ConfigCPP - CPP MU Configuration Register
    Bits: 15:8  DirAccWays(rw) - When bit n in this field is set to 1, permit MU commands to direct access that way of the cache; when low, maps direct access to that way to High Locality of Reference accesses. If all 1s then all MU commands, with the exception of the Queue Engine commands highlighted in the Note below, are forced to be direct access. Note: This applies only the External Memory Units, all other Memory Units are direct access only
    Bits:  5    CPPErrorSuppress(rw) - If set, data errors generated for outbound CPP data buses will be suppressed internally and will not be sent out on the bus. CPP targets will suppress Push bus errors, CPP masters will suppress Pull bus errors.
    Bits:  4    CPPErrorIgnore(rw) - If set, data errors received on inbound CPP data buses will be ignored internally. CPP targets will ignore Pull bus errors, CPP masters will ignore Push bus errors.
    Bits:  3    DisableDualChRead(rw) - Controls the use of the CTM Bulk Engine performance feature to use both DCache channels for parallel non-dependent reads if the ordered commands are blocked/empty (CTM Only).
    Bits:  2:1  Addr32Bit(rw) - Select mode of addressing. Note that only Bulk, Atomic and Queue operations are supported in 32-bit modes.
    Bits:  0    IgnBulkAlign(rw) - If set, then CPP address[3;0] are ignored for bulk DRAM access, as with the IXP. If clear, then CPP address[3;0] are used, and unaligned accesses occur if they are not zero.
*/
#define NFP_CTMX_CFG_CFG                                     0x00000000
#define   NFP_CTMX_CFG_CFG_WAYS(_x)                          (((_x) & 0xff) << 8)
#define   NFP_CTMX_CFG_CFG_WAYS_of(_x)                       (((_x) >> 8) & 0xff)
#define   NFP_CTMX_CFG_CFG_ERROR_SUPPRESS                    BIT(5)
#define   NFP_CTMX_CFG_CFG_ERROR_IGNORE                      BIT(4)
#define   NFP_CTMX_CFG_CFG_DISABLE_DUAL_READ                 BIT(3)
#define     NFP_CTMX_CFG_CFG_DISABLE_DUAL_READ_ENABLED       (0 << 3)
#define     NFP_CTMX_CFG_CFG_DISABLE_DUAL_READ_DISABLED      BIT(3)
#define   NFP_CTMX_CFG_CFG_ADDRESSING(_x)                    (((_x) & 0x3) << 1)
#define   NFP_CTMX_CFG_CFG_ADDRESSING_of(_x)                 (((_x) >> 1) & 0x3)
#define     NFP_CTMX_CFG_CFG_ADDRESSING_40BIT                (0)
#define     NFP_CTMX_CFG_CFG_ADDRESSING_32BIT                (1)
#define     NFP_CTMX_CFG_CFG_ADDRESSING_32BITHLR             (2)
#define     NFP_CTMX_CFG_CFG_ADDRESSING_32BITLLR             (3)
#define   NFP_CTMX_CFG_CFG_BULK_ALIGN                        BIT(0)
#define     NFP_CTMX_CFG_CFG_BULK_ALIGN_ENFORCE              (0 << 0)
#define     NFP_CTMX_CFG_CFG_BULK_ALIGN_IGNORE               BIT(0)
/*----------------------------------------------------------------
  Register: ConfigVQDR[4] - Virtual QDR X Configuration Register
    Bits: 21:20 QueueType(rw) - Type of queues in the channel, similar to IXP IgnoreEOP and IgnoreSegCnt
    Bits: 15:0  MemWinBase(rw) - Bits [16;24] of full 40-bit MU address space for the base of the virtual QDR channel
*/
#define NFP_CTMX_CFG_VQDR(_x)                                (0x00000004 + (0x4 * ((_x) & 0x3)))
#define   NFP_CTMX_CFG_VQDR_QUEUE_TYPE(_x)                   (((_x) & 0x3) << 20)
#define   NFP_CTMX_CFG_VQDR_QUEUE_TYPE_of(_x)                (((_x) >> 20) & 0x3)
#define     NFP_CTMX_CFG_VQDR_QUEUE_TYPE_SEGMENTS            (0)
#define     NFP_CTMX_CFG_VQDR_QUEUE_TYPE_PACKETS             (1)
#define     NFP_CTMX_CFG_VQDR_QUEUE_TYPE_PACKETS32           (2)
#define     NFP_CTMX_CFG_VQDR_QUEUE_TYPE_BUFFERS             (3)
#define   NFP_CTMX_CFG_VQDR_BASE(_x)                         (((_x) & 0xffff) << 0)
#define   NFP_CTMX_CFG_VQDR_BASE_of(_x)                      (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: XpbTimeout - XPB Timeout
    Bits: 31:0  XpbTimeoutData(rw) - XPB timeout data.
*/
#define NFP_CTMX_CFG_XPB_TIMEOUT                             0x00000c00
#define   NFP_CTMX_CFG_XPB_TIMEOUT_XPB_TIMEOUT_DATA(_x)      (((_x) & 0xffffffff) << 0)
#define   NFP_CTMX_CFG_XPB_TIMEOUT_XPB_TIMEOUT_DATA_of(_x)   (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MUPEMemConfig - MU PE Memory Configuration
    Bits:  1:0  MUPEMemConfig(rw) - Packet Mem Config
*/
#define NFP_CTMX_PKT_MU_PE_MEM_CONFIG                        0x00000000
#define   NFP_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG(_x) (((_x) & 0x3) << 0)
#define   NFP_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_of(_x) (((_x) >> 0) & 0x3)
#define     NFP_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_FULL___ (0)
#define     NFP_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_HALF___ (1)
#define     NFP_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_QUARTER (2)
#define     NFP_CTMX_PKT_MU_PE_MEM_CONFIG_MU_PE_MEM_CONFIG_NA_____ (3)
/*----------------------------------------------------------------
  Register: MUPEWQConfig - MU PE Work Queue Config
    Bits:  0    MUPEWQConfig(rw) - Work Queue Configuration
*/
#define NFP_CTMX_PKT_MU_PE_WQ_CONFIG                         0x00000004
#define   NFP_CTMX_PKT_MU_PE_WQ_CONFIG_MU_PE_WQ_CONFIG       BIT(0)
#define     NFP_CTMX_PKT_MU_PE_WQ_CONFIG_MU_PE_WQ_CONFIG_SENDONLAST (0 << 0)
#define     NFP_CTMX_PKT_MU_PE_WQ_CONFIG_MU_PE_WQ_CONFIG_SENDONFIRST BIT(0)
/*----------------------------------------------------------------
  Register: MUPECredits[4] - MU PE Credit Buckets
    Bits: 26:16 MUPEPacketCredits(ro) - Packet Credits
    Bits:  8:0  MUPEBufferCredits(ro) - Buffer Credits
*/
#define NFP_CTMX_PKT_MU_PE_CREDITS(_x)                       (0x00000008 + (0x4 * ((_x) & 0x3)))
#define   NFP_CTMX_PKT_MU_PE_CREDITS_MU_PE_PACKET_CREDITS(_x) (((_x) & 0x7ff) << 16)
#define   NFP_CTMX_PKT_MU_PE_CREDITS_MU_PE_PACKET_CREDITS_of(_x) (((_x) >> 16) & 0x7ff)
#define   NFP_CTMX_PKT_MU_PE_CREDITS_MU_PE_BUFFER_CREDITS(_x) (((_x) & 0x1ff) << 0)
#define   NFP_CTMX_PKT_MU_PE_CREDITS_MU_PE_BUFFER_CREDITS_of(_x) (((_x) >> 0) & 0x1ff)
/*----------------------------------------------------------------
  Register: MUPELBConfig - MU PE LB Config
    Bits:  0    MUPELBConfig(rw) - Load Balancer Config.
*/
#define NFP_CTMX_PKT_MU_PE_LB_CONFIG                         0x00000020
#define   NFP_CTMX_PKT_MU_PE_LB_CONFIG_MU_PE_LB_CONFIG       BIT(0)
#define     NFP_CTMX_PKT_MU_PE_LB_CONFIG_MU_PE_LB_CONFIG_DYNAMIC (0 << 0)
#define     NFP_CTMX_PKT_MU_PE_LB_CONFIG_MU_PE_LB_CONFIG_STATIC BIT(0)
/*----------------------------------------------------------------
  Register: MUPEActivePacketCount - Packet Stats -- Active Packet Count
  Register: MUPEPeakPacketCount - Packet Stats -- Peak Packet Count
    Bits:  9:0  Stats(ro) - Packet Count Stats
*/
#define NFP_CTMX_PKT_MU_PE_ACTIVE_PACKET_COUNT               0x00000400
#define NFP_CTMX_PKT_MU_PE_PEAK_PACKET_COUNT                 0x00000404
#define   NFP_CTMX_PKT_MUPESTATS_MU_PE_STAT(_x)              (((_x) & 0x3ff) << 0)
#define   NFP_CTMX_PKT_MUPESTATS_MU_PE_STAT_of(_x)           (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: XpbErrConfig - XPB Error Configuration Register
    Bits: 15:1  XpbMaxTimeout(rw) - XPB Max Timeout. Value to wait for an XPB response before declaring a timeout error.
    Bits:  0    XpbWrIfErr(rw) - If '1', execute XPB writes if data has data_error. If '0', drop XPB writes for data with data_error. If write is dropped, log the address information in the XPBWrErrStat csr.
*/
#define NFP_CTMX_MISC_XPB_ERR_CONFIG                         0x00000000
#define   NFP_CTMX_MISC_XPB_ERR_CONFIG_XPB_MAX_TIMEOUT(_x)   (((_x) & 0x7fff) << 1)
#define   NFP_CTMX_MISC_XPB_ERR_CONFIG_XPB_MAX_TIMEOUT_of(_x) (((_x) >> 1) & 0x7fff)
#define   NFP_CTMX_MISC_XPB_ERR_CONFIG_XPB_WRITE_IF_ERR      BIT(0)
/*----------------------------------------------------------------
  Register: XpbTOErrStat - XPB Timeout Error Status Register
    Bits: 14    XpbTOErr(rwc) - XPB Timeout Error. If '1', the requested XPB CLUSTER_ID and DEVICE_ID combination did not result in a XPB response on the XPB Bus. Requesting a non-existent XPB target is not allowed on the XPB bus. If '0', no current error.
    Bits: 13:8  XpbClId(rwc) - This is the XPB CLUSTER ID of the XPB request that caused the XPB Timeout Error.
    Bits:  7:0  XpbDevId(rwc) - This is the XPB Device ID of the XPB request that caused the XPB Timeout Error.
*/
#define NFP_CTMX_MISC_XPB_TIMEOUT_ERR_STAT                   0x00000004
#define   NFP_CTMX_MISC_XPB_TIMEOUT_ERR_STAT_XPB_TIMEOUT_ERR BIT(14)
#define   NFP_CTMX_MISC_XPB_TIMEOUT_ERR_STAT_XPB_CLUSTER_ID(_x) (((_x) & 0x3f) << 8)
#define   NFP_CTMX_MISC_XPB_TIMEOUT_ERR_STAT_XPB_CLUSTER_ID_of(_x) (((_x) >> 8) & 0x3f)
#define   NFP_CTMX_MISC_XPB_TIMEOUT_ERR_STAT_XPB_DEVICE_ID(_x) (((_x) & 0xff) << 0)
#define   NFP_CTMX_MISC_XPB_TIMEOUT_ERR_STAT_XPB_DEVICE_ID_of(_x) (((_x) >> 0) & 0xff)
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
#define NFP_CTMX_MISC_XPB_WR_ERR_STAT                        0x00000008
#define   NFP_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_MULTI     BIT(30)
#define   NFP_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_SINGLE    BIT(29)
#define   NFP_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_GLOBAL    BIT(28)
#define   NFP_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_TGT_ISLD(_x) (((_x) & 0x3f) << 22)
#define   NFP_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_TGT_ISLD_of(_x) (((_x) >> 22) & 0x3f)
#define   NFP_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_SLAVE_IDX(_x) (((_x) & 0x3) << 20)
#define   NFP_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_SLAVE_IDX_of(_x) (((_x) >> 20) & 0x3)
#define   NFP_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_DEV_ID(_x) (((_x) & 0x3f) << 14)
#define   NFP_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_DEV_ID_of(_x) (((_x) >> 14) & 0x3f)
#define   NFP_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_REG_ADDR(_x) (((_x) & 0x3fff) << 0)
#define   NFP_CTMX_MISC_XPB_WR_ERR_STAT_XPB_WR_ERR_REG_ADDR_of(_x) (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: Ring_[16]_Base - Ring X Base Register
    Bits: 31:29 Size(rw) - Size of ring in words
    Bits: 28    StatusType(rw) - Status type. This bit controls the type of status reported on the status bus for the ring.
    Bits: 17:9  BaseAddress(rw) - The Base Address. This value must be written to be consistent with the requirement that the Ring is aligned to its size. For example, if the Ring has 512 longwords, bits 10:9 must be written to 0.
*/
#define NFP_CTMX_MISC_RING_BASE(_x)                          (0x00000100 + (0x10 * ((_x) & 0xf)))
#define   NFP_CTMX_MISC_RING_BASE_SIZE(_x)                   (((_x) & 0x7) << 29)
#define   NFP_CTMX_MISC_RING_BASE_SIZE_of(_x)                (((_x) >> 29) & 0x7)
#define     NFP_CTMX_MISC_RING_BASE_SIZE_128                 (0)
#define     NFP_CTMX_MISC_RING_BASE_SIZE_256                 (1)
#define     NFP_CTMX_MISC_RING_BASE_SIZE_512                 (2)
#define     NFP_CTMX_MISC_RING_BASE_SIZE_1K                  (3)
#define     NFP_CTMX_MISC_RING_BASE_SIZE_2K                  (4)
#define     NFP_CTMX_MISC_RING_BASE_SIZE_4K                  (5)
#define     NFP_CTMX_MISC_RING_BASE_SIZE_8K                  (6)
#define     NFP_CTMX_MISC_RING_BASE_SIZE_16K                 (7)
#define   NFP_CTMX_MISC_RING_BASE_STATUS_TYPE                BIT(28)
#define     NFP_CTMX_MISC_RING_BASE_STATUS_TYPE_EMPTY        (0 << 28)
#define     NFP_CTMX_MISC_RING_BASE_STATUS_TYPE_FULL         BIT(28)
#define   NFP_CTMX_MISC_RING_BASE_BASE(_x)                   (((_x) & 0x1ff) << 9)
#define   NFP_CTMX_MISC_RING_BASE_BASE_of(_x)                (((_x) >> 9) & 0x1ff)
/*----------------------------------------------------------------
  Register: Ring_[16]_Head - Ring X Head Register
    Bits: 16:2  HeadOffset(rw) - Head pointer
*/
#define NFP_CTMX_MISC_RING_HEAD(_x)                          (0x00000104 + (0x10 * ((_x) & 0xf)))
#define   NFP_CTMX_MISC_RING_HEAD_OFF(_x)                    (((_x) & 0x7fff) << 2)
#define   NFP_CTMX_MISC_RING_HEAD_OFF_of(_x)                 (((_x) >> 2) & 0x7fff)
/*----------------------------------------------------------------
  Register: Ring_[16]_Tail - Ring X Tail Register
    Bits: 16:2  TailOffset(rw) - Tail pointer
*/
#define NFP_CTMX_MISC_RING_TAIL(_x)                          (0x00000108 + (0x10 * ((_x) & 0xf)))
#define   NFP_CTMX_MISC_RING_TAIL_OFF(_x)                    (((_x) & 0x7fff) << 2)
#define   NFP_CTMX_MISC_RING_TAIL_OFF_of(_x)                 (((_x) >> 2) & 0x7fff)

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: ConfigCPP - CPP MU Configuration Register
    Bits: 15:8  DirAccWays(rw) - When bit n in this field is set to 1, permit MU commands to direct access that way of the cache; when low, maps direct access to that way to High Locality of Reference accesses. If all 1s then all MU commands, with the exception of the Queue Engine commands highlighted in the Note below, are forced to be direct access. Note: This applies only the External Memory Units, all other Memory Units are direct access only
    Bits:  5    CPPErrorSuppress(rw) - If set, data errors generated for outbound CPP data buses will be suppressed internally and will not be sent out on the bus. CPP targets will suppress Push bus errors, CPP masters will suppress Pull bus errors.
    Bits:  4    CPPErrorIgnore(rw) - If set, data errors received on inbound CPP data buses will be ignored internally. CPP targets will ignore Pull bus errors, CPP masters will ignore Push bus errors.
    Bits:  3    DisableDualChRead(rw) - Controls the use of the CTM Bulk Engine performance feature to use both DCache channels for parallel non-dependent reads if the ordered commands are blocked/empty (CTM Only).
    Bits:  2:1  Addr32Bit(rw) - Select mode of addressing. Note that only Bulk, Atomic and Queue operations are supported in 32-bit modes.
    Bits:  0    IgnBulkAlign(rw) - If set, then CPP address[3;0] are ignored for bulk DRAM access, as with the IXP. If clear, then CPP address[3;0] are used, and unaligned accesses occur if they are not zero.
*/
#define NFP_CTMX_CFG_ConfigCPP                               0x00000000
#define   NFP_CTMX_CFG_ConfigCPP_DirAccWays(_x)              (((_x) & 0xff) << 8)
#define   NFP_CTMX_CFG_ConfigCPP_DirAccWays_of(_x)           (((_x) >> 8) & 0xff)
#define   NFP_CTMX_CFG_ConfigCPP_CPPErrorSuppress            BIT(5)
#define   NFP_CTMX_CFG_ConfigCPP_CPPErrorIgnore              BIT(4)
#define   NFP_CTMX_CFG_ConfigCPP_DisableDualChRead           BIT(3)
#define     NFP_CTMX_CFG_ConfigCPP_DisableDualChRead_Enabled (0 << 3)
#define     NFP_CTMX_CFG_ConfigCPP_DisableDualChRead_Disabled BIT(3)
#define   NFP_CTMX_CFG_ConfigCPP_Addr32Bit(_x)               (((_x) & 0x3) << 1)
#define   NFP_CTMX_CFG_ConfigCPP_Addr32Bit_of(_x)            (((_x) >> 1) & 0x3)
#define     NFP_CTMX_CFG_ConfigCPP_Addr32Bit_40BIT           (0)
#define     NFP_CTMX_CFG_ConfigCPP_Addr32Bit_32BIT           (1)
#define     NFP_CTMX_CFG_ConfigCPP_Addr32Bit_32BITHLR        (2)
#define     NFP_CTMX_CFG_ConfigCPP_Addr32Bit_32BITLLR        (3)
#define   NFP_CTMX_CFG_ConfigCPP_IgnBulkAlign                BIT(0)
#define     NFP_CTMX_CFG_ConfigCPP_IgnBulkAlign_ENFORCE      (0 << 0)
#define     NFP_CTMX_CFG_ConfigCPP_IgnBulkAlign_IGNORE       BIT(0)
/*----------------------------------------------------------------
  Register: ConfigVQDR[4] - Virtual QDR X Configuration Register
    Bits: 21:20 QueueType(rw) - Type of queues in the channel, similar to IXP IgnoreEOP and IgnoreSegCnt
    Bits: 15:0  MemWinBase(rw) - Bits [16;24] of full 40-bit MU address space for the base of the virtual QDR channel
*/
#define NFP_CTMX_CFG_ConfigVQDR(_x)                          (0x00000004 + (0x4 * ((_x) & 0x3)))
#define   NFP_CTMX_CFG_ConfigVQDR_QueueType(_x)              (((_x) & 0x3) << 20)
#define   NFP_CTMX_CFG_ConfigVQDR_QueueType_of(_x)           (((_x) >> 20) & 0x3)
#define     NFP_CTMX_CFG_ConfigVQDR_QueueType_SEGMENTS       (0)
#define     NFP_CTMX_CFG_ConfigVQDR_QueueType_PACKETS        (1)
#define     NFP_CTMX_CFG_ConfigVQDR_QueueType_PACKETS32      (2)
#define     NFP_CTMX_CFG_ConfigVQDR_QueueType_BUFFERS        (3)
#define   NFP_CTMX_CFG_ConfigVQDR_MemWinBase(_x)             (((_x) & 0xffff) << 0)
#define   NFP_CTMX_CFG_ConfigVQDR_MemWinBase_of(_x)          (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: XpbTimeout - XPB Timeout
    Bits: 31:0  XpbTimeoutData(rw) - XPB timeout data.
*/
#define NFP_CTMX_CFG_XpbTimeout                              0x00000c00
#define   NFP_CTMX_CFG_XpbTimeout_XpbTimeoutData(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP_CTMX_CFG_XpbTimeout_XpbTimeoutData_of(_x)      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MUPEMemConfig - MU PE Memory Configuration
    Bits:  1:0  MUPEMemConfig(rw) - Packet Mem Config
*/
#define NFP_CTMX_PKT_MUPEMemConfig                           0x00000000
#define   NFP_CTMX_PKT_MUPEMemConfig_MUPEMemConfig(_x)       (((_x) & 0x3) << 0)
#define   NFP_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_of(_x)    (((_x) >> 0) & 0x3)
#define     NFP_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_FULL___ (0)
#define     NFP_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_HALF___ (1)
#define     NFP_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_QUARTER (2)
#define     NFP_CTMX_PKT_MUPEMemConfig_MUPEMemConfig_NA_____ (3)
/*----------------------------------------------------------------
  Register: MUPEWQConfig - MU PE Work Queue Config
    Bits:  0    MUPEWQConfig(rw) - Work Queue Configuration
*/
#define NFP_CTMX_PKT_MUPEWQConfig                            0x00000004
#define   NFP_CTMX_PKT_MUPEWQConfig_MUPEWQConfig             BIT(0)
#define     NFP_CTMX_PKT_MUPEWQConfig_MUPEWQConfig_SendOnLast (0 << 0)
#define     NFP_CTMX_PKT_MUPEWQConfig_MUPEWQConfig_SendOnFirst BIT(0)
/*----------------------------------------------------------------
  Register: MUPECredits[4] - MU PE Credit Buckets
    Bits: 26:16 MUPEPacketCredits(ro) - Packet Credits
    Bits:  8:0  MUPEBufferCredits(ro) - Buffer Credits
*/
#define NFP_CTMX_PKT_MUPECredits(_x)                         (0x00000008 + (0x4 * ((_x) & 0x3)))
#define   NFP_CTMX_PKT_MUPECredits_MUPEPacketCredits(_x)     (((_x) & 0x7ff) << 16)
#define   NFP_CTMX_PKT_MUPECredits_MUPEPacketCredits_of(_x)  (((_x) >> 16) & 0x7ff)
#define   NFP_CTMX_PKT_MUPECredits_MUPEBufferCredits(_x)     (((_x) & 0x1ff) << 0)
#define   NFP_CTMX_PKT_MUPECredits_MUPEBufferCredits_of(_x)  (((_x) >> 0) & 0x1ff)
/*----------------------------------------------------------------
  Register: MUPELBConfig - MU PE LB Config
    Bits:  0    MUPELBConfig(rw) - Load Balancer Config.
*/
#define NFP_CTMX_PKT_MUPELBConfig                            0x00000020
#define   NFP_CTMX_PKT_MUPELBConfig_MUPELBConfig             BIT(0)
#define     NFP_CTMX_PKT_MUPELBConfig_MUPELBConfig_Dynamic   (0 << 0)
#define     NFP_CTMX_PKT_MUPELBConfig_MUPELBConfig_Static    BIT(0)
/*----------------------------------------------------------------
  Register: MUPEActivePacketCount - Packet Stats -- Active Packet Count
  Register: MUPEPeakPacketCount - Packet Stats -- Peak Packet Count
    Bits:  9:0  Stats(ro) - Packet Count Stats
*/
#define NFP_CTMX_PKT_MUPEActivePacketCount                   0x00000400
#define NFP_CTMX_PKT_MUPEPeakPacketCount                     0x00000404
#define   NFP_CTMX_PKT_MUPEStats_Stats(_x)                   (((_x) & 0x3ff) << 0)
#define   NFP_CTMX_PKT_MUPEStats_Stats_of(_x)                (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: XpbErrConfig - XPB Error Configuration Register
    Bits: 15:1  XpbMaxTimeout(rw) - XPB Max Timeout. Value to wait for an XPB response before declaring a timeout error.
    Bits:  0    XpbWrIfErr(rw) - If '1', execute XPB writes if data has data_error. If '0', drop XPB writes for data with data_error. If write is dropped, log the address information in the XPBWrErrStat csr.
*/
#define NFP_CTMX_MISC_XpbErrConfig                           0x00000000
#define   NFP_CTMX_MISC_XpbErrConfig_XpbMaxTimeout(_x)       (((_x) & 0x7fff) << 1)
#define   NFP_CTMX_MISC_XpbErrConfig_XpbMaxTimeout_of(_x)    (((_x) >> 1) & 0x7fff)
#define   NFP_CTMX_MISC_XpbErrConfig_XpbWrIfErr              BIT(0)
/*----------------------------------------------------------------
  Register: XpbTOErrStat - XPB Timeout Error Status Register
    Bits: 14    XpbTOErr(rwc) - XPB Timeout Error. If '1', the requested XPB CLUSTER_ID and DEVICE_ID combination did not result in a XPB response on the XPB Bus. Requesting a non-existent XPB target is not allowed on the XPB bus. If '0', no current error.
    Bits: 13:8  XpbClId(rwc) - This is the XPB CLUSTER ID of the XPB request that caused the XPB Timeout Error.
    Bits:  7:0  XpbDevId(rwc) - This is the XPB Device ID of the XPB request that caused the XPB Timeout Error.
*/
#define NFP_CTMX_MISC_XpbTOErrStat                           0x00000004
#define   NFP_CTMX_MISC_XpbTOErrStat_XpbTOErr                BIT(14)
#define   NFP_CTMX_MISC_XpbTOErrStat_XpbClId(_x)             (((_x) & 0x3f) << 8)
#define   NFP_CTMX_MISC_XpbTOErrStat_XpbClId_of(_x)          (((_x) >> 8) & 0x3f)
#define   NFP_CTMX_MISC_XpbTOErrStat_XpbDevId(_x)            (((_x) & 0xff) << 0)
#define   NFP_CTMX_MISC_XpbTOErrStat_XpbDevId_of(_x)         (((_x) >> 0) & 0xff)
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
#define NFP_CTMX_MISC_XpbWrErrStat                           0x00000008
#define   NFP_CTMX_MISC_XpbWrErrStat_XpbWrErrMulti           BIT(30)
#define   NFP_CTMX_MISC_XpbWrErrStat_XpbWrErrSingle          BIT(29)
#define   NFP_CTMX_MISC_XpbWrErrStat_XpbWrErrGlobal          BIT(28)
#define   NFP_CTMX_MISC_XpbWrErrStat_XpbWrErrTgtIsld(_x)     (((_x) & 0x3f) << 22)
#define   NFP_CTMX_MISC_XpbWrErrStat_XpbWrErrTgtIsld_of(_x)  (((_x) >> 22) & 0x3f)
#define   NFP_CTMX_MISC_XpbWrErrStat_XpbWrErrSlaveIdx(_x)    (((_x) & 0x3) << 20)
#define   NFP_CTMX_MISC_XpbWrErrStat_XpbWrErrSlaveIdx_of(_x) (((_x) >> 20) & 0x3)
#define   NFP_CTMX_MISC_XpbWrErrStat_XpbWrErrDevID(_x)       (((_x) & 0x3f) << 14)
#define   NFP_CTMX_MISC_XpbWrErrStat_XpbWrErrDevID_of(_x)    (((_x) >> 14) & 0x3f)
#define   NFP_CTMX_MISC_XpbWrErrStat_XpbWrErrRegAddr(_x)     (((_x) & 0x3fff) << 0)
#define   NFP_CTMX_MISC_XpbWrErrStat_XpbWrErrRegAddr_of(_x)  (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: Ring_[16]_Base - Ring X Base Register
    Bits: 31:29 Size(rw) - Size of ring in words
    Bits: 28    StatusType(rw) - Status type. This bit controls the type of status reported on the status bus for the ring.
    Bits: 17:9  BaseAddress(rw) - The Base Address. This value must be written to be consistent with the requirement that the Ring is aligned to its size. For example, if the Ring has 512 longwords, bits 10:9 must be written to 0.
*/
#define NFP_CTMX_MISC_Ring_Base(_x)                          (0x00000100 + (0x10 * ((_x) & 0xf)))
#define   NFP_CTMX_MISC_Ring_Base_Size(_x)                   (((_x) & 0x7) << 29)
#define   NFP_CTMX_MISC_Ring_Base_Size_of(_x)                (((_x) >> 29) & 0x7)
#define     NFP_CTMX_MISC_Ring_Base_Size_128                 (0)
#define     NFP_CTMX_MISC_Ring_Base_Size_256                 (1)
#define     NFP_CTMX_MISC_Ring_Base_Size_512                 (2)
#define     NFP_CTMX_MISC_Ring_Base_Size_1k                  (3)
#define     NFP_CTMX_MISC_Ring_Base_Size_2k                  (4)
#define     NFP_CTMX_MISC_Ring_Base_Size_4k                  (5)
#define     NFP_CTMX_MISC_Ring_Base_Size_8k                  (6)
#define     NFP_CTMX_MISC_Ring_Base_Size_16k                 (7)
#define   NFP_CTMX_MISC_Ring_Base_StatusType                 BIT(28)
#define     NFP_CTMX_MISC_Ring_Base_StatusType_Empty         (0 << 28)
#define     NFP_CTMX_MISC_Ring_Base_StatusType_Full          BIT(28)
#define   NFP_CTMX_MISC_Ring_Base_BaseAddress(_x)            (((_x) & 0x1ff) << 9)
#define   NFP_CTMX_MISC_Ring_Base_BaseAddress_of(_x)         (((_x) >> 9) & 0x1ff)
/*----------------------------------------------------------------
  Register: Ring_[16]_Head - Ring X Head Register
    Bits: 16:2  HeadOffset(rw) - Head pointer
*/
#define NFP_CTMX_MISC_Ring_Head(_x)                          (0x00000104 + (0x10 * ((_x) & 0xf)))
#define   NFP_CTMX_MISC_Ring_Head_HeadOffset(_x)             (((_x) & 0x7fff) << 2)
#define   NFP_CTMX_MISC_Ring_Head_HeadOffset_of(_x)          (((_x) >> 2) & 0x7fff)
/*----------------------------------------------------------------
  Register: Ring_[16]_Tail - Ring X Tail Register
    Bits: 16:2  TailOffset(rw) - Tail pointer
*/
#define NFP_CTMX_MISC_Ring_Tail(_x)                          (0x00000108 + (0x10 * ((_x) & 0xf)))
#define   NFP_CTMX_MISC_Ring_Tail_TailOffset(_x)             (((_x) & 0x7fff) << 2)
#define   NFP_CTMX_MISC_Ring_Tail_TailOffset_of(_x)          (((_x) >> 2) & 0x7fff)

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_CTM_H */
