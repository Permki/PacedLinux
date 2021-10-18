/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP3800_IMB_H
#define NFP3800_IMB_H

/*================================================================*/

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP3800_XPB_ISLAND(island))          */

/* XPB_BASE + NFP3800_IMBX + NFP3800_IMB_* */
#define NFP3800_IMBX                                             (0x040000)
/*================================================================*/


/* HGID: 6ea1af6fca88 */

#ifndef NFP3800_LONGNAMES

/*----------------------------------------------------------------
  Register: Target0AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 0
  Register: Target1AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 1
  Register: Target2AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 2
  Register: Target3AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 3
  Register: Target4AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 4
  Register: Target5AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 5
  Register: Target6AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 6
  Register: Target7AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 7
  Register: Target8AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 8
  Register: Target9AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 9
  Register: Target10AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 10
  Register: Target11AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 11
  Register: Target12AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 12
  Register: Target13AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 13
  Register: Target14AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 14
  Register: Target15AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 15
    Bits: 15:13 Mode(rw) - The specific translation mode to be used by the IMB when generating the Island number where the Target resides
    Bits: 12    AddrMode(rw) - The specific addressing mode (32 or 40) used by the Master when referencing the Target
    Bits: 11:6  Island1(rw) - The actual Island number for Island 1
    Bits:  5:0  Island0(rw) - The actual Island number for Island 0
*/
#define NFP3800_IMB_TGT0_ADDRESS_MODE_CONFIGURATION          0x00000000
#define NFP3800_IMB_TGT1_ADDRESS_MODE_CONFIGURATION          0x00000004
#define NFP3800_IMB_TGT2_ADDRESS_MODE_CONFIGURATION          0x00000008
#define NFP3800_IMB_TGT3_ADDRESS_MODE_CONFIGURATION          0x0000000c
#define NFP3800_IMB_TGT4_ADDRESS_MODE_CONFIGURATION          0x00000010
#define NFP3800_IMB_TGT5_ADDRESS_MODE_CONFIGURATION          0x00000014
#define NFP3800_IMB_TGT6_ADDRESS_MODE_CONFIGURATION          0x00000018
#define NFP3800_IMB_TGT7_ADDRESS_MODE_CONFIGURATION          0x0000001c
#define NFP3800_IMB_TGT8_ADDRESS_MODE_CONFIGURATION          0x00000020
#define NFP3800_IMB_TGT9_ADDRESS_MODE_CONFIGURATION          0x00000024
#define NFP3800_IMB_TGT10_ADDRESS_MODE_CONFIGURATION         0x00000028
#define NFP3800_IMB_TGT11_ADDRESS_MODE_CONFIGURATION         0x0000002c
#define NFP3800_IMB_TGT12_ADDRESS_MODE_CONFIGURATION         0x00000030
#define NFP3800_IMB_TGT13_ADDRESS_MODE_CONFIGURATION         0x00000034
#define NFP3800_IMB_TGT14_ADDRESS_MODE_CONFIGURATION         0x00000038
#define NFP3800_IMB_TGT15_ADDRESS_MODE_CONFIGURATION         0x0000003c
#define   NFP3800_IMB_TGTADDRESSMODECFG_MODE(_x)             (((_x) & 0x7) << 13)
#define   NFP3800_IMB_TGTADDRESSMODECFG_MODE_of(_x)          (((_x) >> 13) & 0x7)
#define   NFP3800_IMB_TGTADDRESSMODECFG_ADDRESSING_MODE      BIT(12)
#define     NFP3800_IMB_TGTADDRESSMODECFG_ADDRESSING_MODE_32_BIT (0 << 12)
#define     NFP3800_IMB_TGTADDRESSMODECFG_ADDRESSING_MODE_40_BIT BIT(12)
#define   NFP3800_IMB_TGTADDRESSMODECFG_ISLAND_1(_x)         (((_x) & 0x3f) << 6)
#define   NFP3800_IMB_TGTADDRESSMODECFG_ISLAND_1_of(_x)      (((_x) >> 6) & 0x3f)
#define   NFP3800_IMB_TGTADDRESSMODECFG_ISLAND_0(_x)         (((_x) & 0x3f) << 0)
#define   NFP3800_IMB_TGTADDRESSMODECFG_ISLAND_0_of(_x)      (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: IslandConfigurationClassA - Overall Island Configuration
    Bits: 31    NNchainCfg(rw) - The specific NN chain (A/B schemes) used for linking all the MEs within the Island
    Bits: 30    BroadcastMode(rw) - Broadcast mode for parallel loading of the Control Store in all MEs within an Island (both ME and SM Islands)
*/
#define NFP3800_IMB_ISLAND_CONFIGURATION_CLASS_A             0x00000040
#define   NFP3800_IMB_ISLAND_CONFIGURATION_CLASS_A_NN_CHAIN_CONFIG BIT(31)
#define     NFP3800_IMB_ISLAND_CONFIGURATION_CLASS_A_NN_CHAIN_CONFIG_CHAIN_A (0 << 31)
#define     NFP3800_IMB_ISLAND_CONFIGURATION_CLASS_A_NN_CHAIN_CONFIG_CHAIN_B BIT(31)
#define   NFP3800_IMB_ISLAND_CONFIGURATION_CLASS_A_BROADCAST_MODE BIT(30)
/*----------------------------------------------------------------
  Register: ImbPushBusBroadcastEna - Push Bus enables for broadcast to MEs
    Bits:  5:0  PushBusBroadcastEna(rw) - Enable bit for each MG Pushes to ME Group, LSB is MG0 and MSB is MG5. The enable bit of the PushBus associated with the data_master of the PushData that needs to be broadcasted must be set.
*/
#define NFP3800_IMB_IMB_PUSH_BUS_BROADCAST_ENA               0x00000098
#define   NFP3800_IMB_IMB_PUSH_BUS_BROADCAST_ENA_PUSH_BUS_BROADCAST_ENA(_x) (((_x) & 0x3f) << 0)
#define   NFP3800_IMB_IMB_PUSH_BUS_BROADCAST_ENA_PUSH_BUS_BROADCAST_ENA_of(_x) (((_x) >> 0) & 0x3f)

#else /* NFP3800_LONGNAMES */

/*----------------------------------------------------------------
  Register: Target0AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 0
  Register: Target1AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 1
  Register: Target2AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 2
  Register: Target3AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 3
  Register: Target4AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 4
  Register: Target5AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 5
  Register: Target6AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 6
  Register: Target7AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 7
  Register: Target8AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 8
  Register: Target9AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 9
  Register: Target10AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 10
  Register: Target11AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 11
  Register: Target12AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 12
  Register: Target13AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 13
  Register: Target14AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 14
  Register: Target15AddressModeCfg - Configuration CSR for programming the addressing mode for Target ID 15
    Bits: 15:13 Mode(rw) - The specific translation mode to be used by the IMB when generating the Island number where the Target resides
    Bits: 12    AddrMode(rw) - The specific addressing mode (32 or 40) used by the Master when referencing the Target
    Bits: 11:6  Island1(rw) - The actual Island number for Island 1
    Bits:  5:0  Island0(rw) - The actual Island number for Island 0
*/
#define NFP3800_IMB_Target0AddressModeCfg                    0x00000000
#define NFP3800_IMB_Target1AddressModeCfg                    0x00000004
#define NFP3800_IMB_Target2AddressModeCfg                    0x00000008
#define NFP3800_IMB_Target3AddressModeCfg                    0x0000000c
#define NFP3800_IMB_Target4AddressModeCfg                    0x00000010
#define NFP3800_IMB_Target5AddressModeCfg                    0x00000014
#define NFP3800_IMB_Target6AddressModeCfg                    0x00000018
#define NFP3800_IMB_Target7AddressModeCfg                    0x0000001c
#define NFP3800_IMB_Target8AddressModeCfg                    0x00000020
#define NFP3800_IMB_Target9AddressModeCfg                    0x00000024
#define NFP3800_IMB_Target10AddressModeCfg                   0x00000028
#define NFP3800_IMB_Target11AddressModeCfg                   0x0000002c
#define NFP3800_IMB_Target12AddressModeCfg                   0x00000030
#define NFP3800_IMB_Target13AddressModeCfg                   0x00000034
#define NFP3800_IMB_Target14AddressModeCfg                   0x00000038
#define NFP3800_IMB_Target15AddressModeCfg                   0x0000003c
#define   NFP3800_IMB_TgtAddressModeCfg_Mode(_x)             (((_x) & 0x7) << 13)
#define   NFP3800_IMB_TgtAddressModeCfg_Mode_of(_x)          (((_x) >> 13) & 0x7)
#define   NFP3800_IMB_TgtAddressModeCfg_AddrMode             BIT(12)
#define     NFP3800_IMB_TgtAddressModeCfg_AddrMode_32_bit    (0 << 12)
#define     NFP3800_IMB_TgtAddressModeCfg_AddrMode_40_bit    BIT(12)
#define   NFP3800_IMB_TgtAddressModeCfg_Island1(_x)          (((_x) & 0x3f) << 6)
#define   NFP3800_IMB_TgtAddressModeCfg_Island1_of(_x)       (((_x) >> 6) & 0x3f)
#define   NFP3800_IMB_TgtAddressModeCfg_Island0(_x)          (((_x) & 0x3f) << 0)
#define   NFP3800_IMB_TgtAddressModeCfg_Island0_of(_x)       (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: IslandConfigurationClassA - Overall Island Configuration
    Bits: 31    NNchainCfg(rw) - The specific NN chain (A/B schemes) used for linking all the MEs within the Island
    Bits: 30    BroadcastMode(rw) - Broadcast mode for parallel loading of the Control Store in all MEs within an Island (both ME and SM Islands)
*/
#define NFP3800_IMB_IslandConfigurationClassA                0x00000040
#define   NFP3800_IMB_IslandConfigurationClassA_NNchainCfg   BIT(31)
#define     NFP3800_IMB_IslandConfigurationClassA_NNchainCfg_Chain_A (0 << 31)
#define     NFP3800_IMB_IslandConfigurationClassA_NNchainCfg_Chain_B BIT(31)
#define   NFP3800_IMB_IslandConfigurationClassA_BroadcastMode BIT(30)
/*----------------------------------------------------------------
  Register: ImbPushBusBroadcastEna - Push Bus enables for broadcast to MEs
    Bits:  5:0  PushBusBroadcastEna(rw) - Enable bit for each MG Pushes to ME Group, LSB is MG0 and MSB is MG5. The enable bit of the PushBus associated with the data_master of the PushData that needs to be broadcasted must be set.
*/
#define NFP3800_IMB_ImbPushBusBroadcastEna                   0x00000098
#define   NFP3800_IMB_ImbPushBusBroadcastEna_PushBusBroadcastEna(_x) (((_x) & 0x3f) << 0)
#define   NFP3800_IMB_ImbPushBusBroadcastEna_PushBusBroadcastEna_of(_x) (((_x) >> 0) & 0x3f)

#endif /* NFP3800_LONGNAMES */


#endif /* NFP3800_IMB_H */
