/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_IMU_H
#define NFP6000_IMU_H

/*================================================================*/
/* TGT 7                                                          */

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + NFP_IMU_MEM */
#define NFP_IMU_MEM                                          (0x000000)

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP_XPB_ISLAND(island))          */

#define IMUX_BASE                                            (0xa0000)

/* XPB_BASE + NFP_IMUX_CFG + NFP_IMUX_CFG_* */
#define NFP_IMUX_CFG                                         (IMUX_BASE + 0x000000)
/* XPB_BASE + NFP_IMUX_LE[2] + NFP_IMUX_LE_* */
#define NFP_IMUX_LE(_x)                                      (IMUX_BASE + 0x040000 + ((_x) & 1) * 0x010000)
/* XPB_BASE + NFP_IMUX_SE + NFP_IMUX_SE_* */
#define NFP_IMUX_SE                                          (IMUX_BASE + 0x080000)
/* XPB_BASE + NFP_IMUX_LB + NFP_IMUX_LB_* */
#define NFP_IMUX_LB                                          (IMUX_BASE + 0x100000)
/* XPB_BASE + NFP_IMUX_DCACHE_ECC + (nfp_ecc.h) */
#define NFP_IMUX_DCACHE_ECC                                  (IMUX_BASE + 0x390000)

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
#define NFP_IMUX_CFG_CFG                                     0x00000000
#define   NFP_IMUX_CFG_CFG_WAYS(_x)                          (((_x) & 0xff) << 8)
#define   NFP_IMUX_CFG_CFG_WAYS_of(_x)                       (((_x) >> 8) & 0xff)
#define   NFP_IMUX_CFG_CFG_ERROR_SUPPRESS                    BIT(5)
#define   NFP_IMUX_CFG_CFG_ERROR_IGNORE                      BIT(4)
#define   NFP_IMUX_CFG_CFG_DISABLE_DUAL_READ                 BIT(3)
#define     NFP_IMUX_CFG_CFG_DISABLE_DUAL_READ_ENABLED       (0 << 3)
#define     NFP_IMUX_CFG_CFG_DISABLE_DUAL_READ_DISABLED      BIT(3)
#define   NFP_IMUX_CFG_CFG_ADDRESSING(_x)                    (((_x) & 0x3) << 1)
#define   NFP_IMUX_CFG_CFG_ADDRESSING_of(_x)                 (((_x) >> 1) & 0x3)
#define     NFP_IMUX_CFG_CFG_ADDRESSING_40BIT                (0)
#define     NFP_IMUX_CFG_CFG_ADDRESSING_32BIT                (1)
#define     NFP_IMUX_CFG_CFG_ADDRESSING_32BITHLR             (2)
#define     NFP_IMUX_CFG_CFG_ADDRESSING_32BITLLR             (3)
#define   NFP_IMUX_CFG_CFG_BULK_ALIGN                        BIT(0)
#define     NFP_IMUX_CFG_CFG_BULK_ALIGN_ENFORCE              (0 << 0)
#define     NFP_IMUX_CFG_CFG_BULK_ALIGN_IGNORE               BIT(0)
/*----------------------------------------------------------------
  Register: ConfigVQDR[4] - Virtual QDR X Configuration Register
    Bits: 21:20 QueueType(rw) - Type of queues in the channel, similar to IXP IgnoreEOP and IgnoreSegCnt
    Bits: 15:0  MemWinBase(rw) - Bits [16;24] of full 40-bit MU address space for the base of the virtual QDR channel
*/
#define NFP_IMUX_CFG_VQDR(_x)                                (0x00000004 + (0x4 * ((_x) & 0x3)))
#define   NFP_IMUX_CFG_VQDR_QUEUE_TYPE(_x)                   (((_x) & 0x3) << 20)
#define   NFP_IMUX_CFG_VQDR_QUEUE_TYPE_of(_x)                (((_x) >> 20) & 0x3)
#define     NFP_IMUX_CFG_VQDR_QUEUE_TYPE_SEGMENTS            (0)
#define     NFP_IMUX_CFG_VQDR_QUEUE_TYPE_PACKETS             (1)
#define     NFP_IMUX_CFG_VQDR_QUEUE_TYPE_PACKETS32           (2)
#define     NFP_IMUX_CFG_VQDR_QUEUE_TYPE_BUFFERS             (3)
#define   NFP_IMUX_CFG_VQDR_BASE(_x)                         (((_x) & 0xffff) << 0)
#define   NFP_IMUX_CFG_VQDR_BASE_of(_x)                      (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: XpbTimeout - XPB Timeout
    Bits: 31:0  XpbTimeoutData(rw) - XPB timeout data.
*/
#define NFP_IMUX_CFG_XPB_TIMEOUT                             0x00000c00
#define   NFP_IMUX_CFG_XPB_TIMEOUT_XPB_TIMEOUT_DATA(_x)      (((_x) & 0xffffffff) << 0)
#define   NFP_IMUX_CFG_XPB_TIMEOUT_XPB_TIMEOUT_DATA_of(_x)   (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: LookupEngineStatus - Lookup Engine Status Register. Contains read only status information for the Lookup Engine Instance.
    Bits:  3:0  ErrorCounter(ro) - It is a saturating counter and Holds the Error-count of Invalid opcode,pull-data-error,dcache-data-error
*/
#define NFP_IMUX_LE_LOOKUP_ENGINE_STATUS                     0x00000000
#define   NFP_IMUX_LE_LOOKUP_ENGINE_STATUS_ERROR_COUNTER(_x) (((_x) & 0xf) << 0)
#define   NFP_IMUX_LE_LOOKUP_ENGINE_STATUS_ERROR_COUNTER_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: LookupEngineConfig - Lookup Engine Configuration Register. Read/Write configuration information for the Lookup Engine.
    Bits:  8    Reset_Error_Counter(rw) - Reset Error-counter in LookupEngineStatus register when set.
    Bits:  7:6  MemoryLocalityConfig(rw) - What locality to use when accessing the External memory through Tcache
    Bits:  5:0  RecursionLimit(rw) - The recursion limit value is set to limit the maximum number of recursive lookups allowed
*/
#define NFP_IMUX_LE_LOOKUP_ENGINE_CONFIG                     0x00000004
#define   NFP_IMUX_LE_LOOKUP_ENGINE_CONFIG_RESET_ERR_COUNTER BIT(8)
#define   NFP_IMUX_LE_LOOKUP_ENGINE_CONFIG_LOCALITY_CONFIG(_x) (((_x) & 0x3) << 6)
#define   NFP_IMUX_LE_LOOKUP_ENGINE_CONFIG_LOCALITY_CONFIG_of(_x) (((_x) >> 6) & 0x3)
#define   NFP_IMUX_LE_LOOKUP_ENGINE_CONFIG_RECURSION_LIMIT(_x) (((_x) & 0x3f) << 0)
#define   NFP_IMUX_LE_LOOKUP_ENGINE_CONFIG_RECURSION_LIMIT_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: AlgorithmicTableLocation[8] - Lookup Engine Algorithmic Table Types. These Read/Write registers define the location for each of the eight algorithmic table types.
    Bits:  5:1  Address(rw) - Upper address bits 32:28 of the table
    Bits:  0    Location(rw) - The location of the table
*/
#define NFP_IMUX_LE_ALGORITHMIC_TABLE_LOCATION(_x)           (0x00000008 + (0x4 * ((_x) & 0x7)))
#define   NFP_IMUX_LE_ALGORITHMIC_TABLE_LOCATION_ADDRESS(_x) (((_x) & 0x1f) << 1)
#define   NFP_IMUX_LE_ALGORITHMIC_TABLE_LOCATION_ADDRESS_of(_x) (((_x) >> 1) & 0x1f)
#define   NFP_IMUX_LE_ALGORITHMIC_TABLE_LOCATION_LOCATION    BIT(0)
#define     NFP_IMUX_LE_ALGORITHMIC_TABLE_LOCATION_LOCATION_INTERNAL (0 << 0)
#define     NFP_IMUX_LE_ALGORITHMIC_TABLE_LOCATION_LOCATION_EXTERNAL BIT(0)
/*----------------------------------------------------------------
  Register: DirectTableLocation - Lookup Engine Direct Table Type. Read/Write registers define the bits[32:28] of the Small Direct lookup table DLUT32 and DLUT24
    Bits:  5:1  Address(rw) - Upper address bits 32:28 of Small Direct Lookup Table - DLUT24 and DLUT32
*/
#define NFP_IMUX_LE_DIRECT_TABLE_LOCATION                    0x00000028
#define   NFP_IMUX_LE_DIRECT_TABLE_LOCATION_ADDRESS(_x)      (((_x) & 0x1f) << 1)
#define   NFP_IMUX_LE_DIRECT_TABLE_LOCATION_ADDRESS_of(_x)   (((_x) >> 1) & 0x1f)
/*----------------------------------------------------------------
  Register: MUSEBaseAddr[4] - MU SE Base Address
    Bits: 18:0  MUSEBaseAddr(rw) - Base Address For Statistic
*/
#define NFP_IMUX_SE_MU_SE_BASE_ADDR(_x)                      (0x00000000 + (0x4 * ((_x) & 0x3)))
#define   NFP_IMUX_SE_MU_SE_BASE_ADDR_MU_SE_BASE_ADDR(_x)    (((_x) & 0x7ffff) << 0)
#define   NFP_IMUX_SE_MU_SE_BASE_ADDR_MU_SE_BASE_ADDR_of(_x) (((_x) >> 0) & 0x7ffff)
/*----------------------------------------------------------------
  Register: MULBHashMultiply - Memory Unit Load Balancer Hash Multiply
    Bits:  3    Mult63(rw) - Value of 'm63' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
    Bits:  2    Mult53(rw) - Value of 'm53' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
    Bits:  1    Mult36(rw) - Value of 'm36' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
    Bits:  0    Mult4(rw) - Value of 'm4' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
*/
#define NFP_IMUX_LB_MULB_HASH_MULTIPLY                       0x00000000
#define   NFP_IMUX_LB_MULB_HASH_MULTIPLY_MULT63              BIT(3)
#define   NFP_IMUX_LB_MULB_HASH_MULTIPLY_MULT53              BIT(2)
#define   NFP_IMUX_LB_MULB_HASH_MULTIPLY_MULT36              BIT(1)
#define   NFP_IMUX_LB_MULB_HASH_MULTIPLY_MULT4               BIT(0)
/*----------------------------------------------------------------
  Register: MULBHashSBoxes - Memory Unit Load Balancer Hash SBoxes
    Bits:  4    HashSboxEnable(rw) - If deasserted, residue is not put through sboxes.
    Bits:  3:0  HashSbox(rw) - Number of sboxes to use, minus 1 - bottom 4*(n+1) bits are put through sboxes, rest not.
*/
#define NFP_IMUX_LB_MULB_HASH_SBOXES                         0x00000004
#define   NFP_IMUX_LB_MULB_HASH_SBOXES_HASH_SBOX_ENABLE      BIT(4)
#define   NFP_IMUX_LB_MULB_HASH_SBOXES_HASH_SBOX(_x)         (((_x) & 0xf) << 0)
#define   NFP_IMUX_LB_MULB_HASH_SBOXES_HASH_SBOX_of(_x)      (((_x) >> 0) & 0xf)

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
#define NFP_IMUX_CFG_ConfigCPP                               0x00000000
#define   NFP_IMUX_CFG_ConfigCPP_DirAccWays(_x)              (((_x) & 0xff) << 8)
#define   NFP_IMUX_CFG_ConfigCPP_DirAccWays_of(_x)           (((_x) >> 8) & 0xff)
#define   NFP_IMUX_CFG_ConfigCPP_CPPErrorSuppress            BIT(5)
#define   NFP_IMUX_CFG_ConfigCPP_CPPErrorIgnore              BIT(4)
#define   NFP_IMUX_CFG_ConfigCPP_DisableDualChRead           BIT(3)
#define     NFP_IMUX_CFG_ConfigCPP_DisableDualChRead_Enabled (0 << 3)
#define     NFP_IMUX_CFG_ConfigCPP_DisableDualChRead_Disabled BIT(3)
#define   NFP_IMUX_CFG_ConfigCPP_Addr32Bit(_x)               (((_x) & 0x3) << 1)
#define   NFP_IMUX_CFG_ConfigCPP_Addr32Bit_of(_x)            (((_x) >> 1) & 0x3)
#define     NFP_IMUX_CFG_ConfigCPP_Addr32Bit_40BIT           (0)
#define     NFP_IMUX_CFG_ConfigCPP_Addr32Bit_32BIT           (1)
#define     NFP_IMUX_CFG_ConfigCPP_Addr32Bit_32BITHLR        (2)
#define     NFP_IMUX_CFG_ConfigCPP_Addr32Bit_32BITLLR        (3)
#define   NFP_IMUX_CFG_ConfigCPP_IgnBulkAlign                BIT(0)
#define     NFP_IMUX_CFG_ConfigCPP_IgnBulkAlign_ENFORCE      (0 << 0)
#define     NFP_IMUX_CFG_ConfigCPP_IgnBulkAlign_IGNORE       BIT(0)
/*----------------------------------------------------------------
  Register: ConfigVQDR[4] - Virtual QDR X Configuration Register
    Bits: 21:20 QueueType(rw) - Type of queues in the channel, similar to IXP IgnoreEOP and IgnoreSegCnt
    Bits: 15:0  MemWinBase(rw) - Bits [16;24] of full 40-bit MU address space for the base of the virtual QDR channel
*/
#define NFP_IMUX_CFG_ConfigVQDR(_x)                          (0x00000004 + (0x4 * ((_x) & 0x3)))
#define   NFP_IMUX_CFG_ConfigVQDR_QueueType(_x)              (((_x) & 0x3) << 20)
#define   NFP_IMUX_CFG_ConfigVQDR_QueueType_of(_x)           (((_x) >> 20) & 0x3)
#define     NFP_IMUX_CFG_ConfigVQDR_QueueType_SEGMENTS       (0)
#define     NFP_IMUX_CFG_ConfigVQDR_QueueType_PACKETS        (1)
#define     NFP_IMUX_CFG_ConfigVQDR_QueueType_PACKETS32      (2)
#define     NFP_IMUX_CFG_ConfigVQDR_QueueType_BUFFERS        (3)
#define   NFP_IMUX_CFG_ConfigVQDR_MemWinBase(_x)             (((_x) & 0xffff) << 0)
#define   NFP_IMUX_CFG_ConfigVQDR_MemWinBase_of(_x)          (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: XpbTimeout - XPB Timeout
    Bits: 31:0  XpbTimeoutData(rw) - XPB timeout data.
*/
#define NFP_IMUX_CFG_XpbTimeout                              0x00000c00
#define   NFP_IMUX_CFG_XpbTimeout_XpbTimeoutData(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP_IMUX_CFG_XpbTimeout_XpbTimeoutData_of(_x)      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: LookupEngineStatus - Lookup Engine Status Register. Contains read only status information for the Lookup Engine Instance.
    Bits:  3:0  ErrorCounter(ro) - It is a saturating counter and Holds the Error-count of Invalid opcode,pull-data-error,dcache-data-error
*/
#define NFP_IMUX_LE_LookupEngineStatus                       0x00000000
#define   NFP_IMUX_LE_LookupEngineStatus_ErrorCounter(_x)    (((_x) & 0xf) << 0)
#define   NFP_IMUX_LE_LookupEngineStatus_ErrorCounter_of(_x) (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: LookupEngineConfig - Lookup Engine Configuration Register. Read/Write configuration information for the Lookup Engine.
    Bits:  8    Reset_Error_Counter(rw) - Reset Error-counter in LookupEngineStatus register when set.
    Bits:  7:6  MemoryLocalityConfig(rw) - What locality to use when accessing the External memory through Tcache
    Bits:  5:0  RecursionLimit(rw) - The recursion limit value is set to limit the maximum number of recursive lookups allowed
*/
#define NFP_IMUX_LE_LookupEngineConfig                       0x00000004
#define   NFP_IMUX_LE_LookupEngineConfig_Reset_Error_Counter BIT(8)
#define   NFP_IMUX_LE_LookupEngineConfig_MemoryLocalityConfig(_x) (((_x) & 0x3) << 6)
#define   NFP_IMUX_LE_LookupEngineConfig_MemoryLocalityConfig_of(_x) (((_x) >> 6) & 0x3)
#define   NFP_IMUX_LE_LookupEngineConfig_RecursionLimit(_x)  (((_x) & 0x3f) << 0)
#define   NFP_IMUX_LE_LookupEngineConfig_RecursionLimit_of(_x) (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: AlgorithmicTableLocation[8] - Lookup Engine Algorithmic Table Types. These Read/Write registers define the location for each of the eight algorithmic table types.
    Bits:  5:1  Address(rw) - Upper address bits 32:28 of the table
    Bits:  0    Location(rw) - The location of the table
*/
#define NFP_IMUX_LE_AlgorithmicTableLocation(_x)             (0x00000008 + (0x4 * ((_x) & 0x7)))
#define   NFP_IMUX_LE_AlgorithmicTableLocation_Address(_x)   (((_x) & 0x1f) << 1)
#define   NFP_IMUX_LE_AlgorithmicTableLocation_Address_of(_x) (((_x) >> 1) & 0x1f)
#define   NFP_IMUX_LE_AlgorithmicTableLocation_Location      BIT(0)
#define     NFP_IMUX_LE_AlgorithmicTableLocation_Location_Internal (0 << 0)
#define     NFP_IMUX_LE_AlgorithmicTableLocation_Location_External BIT(0)
/*----------------------------------------------------------------
  Register: DirectTableLocation - Lookup Engine Direct Table Type. Read/Write registers define the bits[32:28] of the Small Direct lookup table DLUT32 and DLUT24
    Bits:  5:1  Address(rw) - Upper address bits 32:28 of Small Direct Lookup Table - DLUT24 and DLUT32
*/
#define NFP_IMUX_LE_DirectTableLocation                      0x00000028
#define   NFP_IMUX_LE_DirectTableLocation_Address(_x)        (((_x) & 0x1f) << 1)
#define   NFP_IMUX_LE_DirectTableLocation_Address_of(_x)     (((_x) >> 1) & 0x1f)
/*----------------------------------------------------------------
  Register: MUSEBaseAddr[4] - MU SE Base Address
    Bits: 18:0  MUSEBaseAddr(rw) - Base Address For Statistic
*/
#define NFP_IMUX_SE_MUSEBaseAddr(_x)                         (0x00000000 + (0x4 * ((_x) & 0x3)))
#define   NFP_IMUX_SE_MUSEBaseAddr_MUSEBaseAddr(_x)          (((_x) & 0x7ffff) << 0)
#define   NFP_IMUX_SE_MUSEBaseAddr_MUSEBaseAddr_of(_x)       (((_x) >> 0) & 0x7ffff)
/*----------------------------------------------------------------
  Register: MULBHashMultiply - Memory Unit Load Balancer Hash Multiply
    Bits:  3    Mult63(rw) - Value of 'm63' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
    Bits:  2    Mult53(rw) - Value of 'm53' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
    Bits:  1    Mult36(rw) - Value of 'm36' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
    Bits:  0    Mult4(rw) - Value of 'm4' for hash multiplication equation m63.x^63 + m53.x^53 + m36.x^36 + m4.x^4
*/
#define NFP_IMUX_LB_MULBHashMultiply                         0x00000000
#define   NFP_IMUX_LB_MULBHashMultiply_Mult63                BIT(3)
#define   NFP_IMUX_LB_MULBHashMultiply_Mult53                BIT(2)
#define   NFP_IMUX_LB_MULBHashMultiply_Mult36                BIT(1)
#define   NFP_IMUX_LB_MULBHashMultiply_Mult4                 BIT(0)
/*----------------------------------------------------------------
  Register: MULBHashSBoxes - Memory Unit Load Balancer Hash SBoxes
    Bits:  4    HashSboxEnable(rw) - If deasserted, residue is not put through sboxes.
    Bits:  3:0  HashSbox(rw) - Number of sboxes to use, minus 1 - bottom 4*(n+1) bits are put through sboxes, rest not.
*/
#define NFP_IMUX_LB_MULBHashSBoxes                           0x00000004
#define   NFP_IMUX_LB_MULBHashSBoxes_HashSboxEnable          BIT(4)
#define   NFP_IMUX_LB_MULBHashSBoxes_HashSbox(_x)            (((_x) & 0xf) << 0)
#define   NFP_IMUX_LB_MULBHashSBoxes_HashSbox_of(_x)         (((_x) >> 0) & 0xf)

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_IMU_H */
