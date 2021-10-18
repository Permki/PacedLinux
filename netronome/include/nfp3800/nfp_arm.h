/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP3800_ARM_H
#define NFP3800_ARM_H

/*================================================================*/
/* TGT 10                                                         */

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + NFP3800_ARM_SMEM */
#define NFP3800_ARM_SMEM                                         0x000000
/* CPP_BASE + NFP3800_ARM_QCTLR[256] + (nfp_qctlr.h) */
#define NFP3800_ARM_Q(_x)                                        (0x100000 + ((_x) & 0xff) * 0x10)
/* CPP_BASE + NFP3800_ARM_IM + (nfp_im.h) */
#define NFP3800_ARM_IM                                           0x200000
/* CPP_BASE + NFP3800_ARM_EM + (nfp_em.h) */
#define NFP3800_ARM_EM                                           0x300000
/* CPP_BASE + NFP3800_ARM_GCSR + NFP3800_ARM_GCSR_* */
#define NFP3800_ARM_GCSR                                         0x400000
/* CPP_BASE + NFP3800_ARM_GPIO + (nfp_gpio.h) */
#define NFP3800_ARM_GPIO                                         (0x403000)
/* CPP_BASE + NFP3800_ARM_TIMER + (nfp_timer.h) */
#define NFP3800_ARM_TIMER                                        (0x403400)
/* CPP_BASE + NFP3800_ARM_UART + (nfp_uart.h) */
#define NFP3800_ARM_UART                                         (0x403800)
/* CPP_BASE + NFP3800_ARM_SPI + (nfp_spi.h) */
#define NFP3800_ARM_SPI                                          (0x403c00)
/* CPP_BASE + NFP3800_ARM_MPCORE + (...) */
#define NFP3800_ARM_MPCORE                                       0x800000
/* CPP_BASE + NFP3800_ARM_PL310 + (...) */
#define NFP3800_ARM_PL310                                        0xa00000
/* CPP_BASE + NFP3800_ARM_SPI_VIRTUAL(x) */
#define NFP3800_ARM_SPI_VIRTUAL(_x)                              (0x10000000 | (((_x) & 0xf) << 22))

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP3800_XPB_ISLAND(island))          */

#define ARMX_BASE                                            (0xa0000)

/* XPB_BASE + NFP3800_ARMX_IM + (nfp_im.h) */
#define NFP3800_ARMX_IM                                          (ARMX_BASE + 0x00000)
/* XPB_BASE + NFP3800_ARMX_QCTLR_ECC + (nfp_ecc.h) */
#define NFP3800_ARMX_QCTLR_ECC                                   (ARMX_BASE + 0x10000)
/* XPB_BASE + NFP3800_ARMX_SMEM_ECC + (nfp_ecc.h) */
#define NFP3800_ARMX_SMEM_ECC                                    (ARMX_BASE + 0x20000)
/* XPB_BASE + NFP3800_ARMX_L1DATA_ECC + (nfp_ecc.h) */
#define NFP3800_ARMX_L1DATA_ECC(_x)                              (ARMX_BASE + 0x50000 + ((_x) & 0x3) * 0x10000)
/* XPB_BASE + NFP3800_ARMX_L1INST_ECC + (nfp_ecc.h) */
#define NFP3800_ARMX_L1INST_ECC                                  (ARMX_BASE + 0x90000)
/* XPB_BASE + NFP3800_ARMX_L1DATATAG_ECC + (nfp_ecc.h) */
#define NFP3800_ARMX_L1DATATAG_ECC                               (ARMX_BASE + 0xa0000)
/* XPB_BASE + NFP3800_ARMX_L1INSTTAG_ECC + (nfp_ecc.h) */
#define NFP3800_ARMX_L1INSTTAG_ECC                               (ARMX_BASE + 0xb0000)
/* XPB_BASE + NFP3800_ARMX_L1DIRTYTAG_ECC + (nfp_ecc.h) */
#define NFP3800_ARMX_L1DIRTYTAG_ECC                              (ARMX_BASE + 0xc0000)
/* XPB_BASE + NFP3800_MEGUStore2Map_ARMX_BASE */
#define NFP3800_ARMX_MEGUSTORE_BASE                              (ARMX_BASE + 0x200000)
/* XPB_BASE + NFP3800_ARMX_PLU + (nfp_plu.h) */
#define NFP3800_ARMX_PLU                                         (ARMX_BASE + 0x400000)

/*----------------------------------------------------------------*/
/* NFP3800 ARMX ME Group UStore */
#define NFP3800_ARMX_MEG0USTORE_ECC                              (NFP3800_ARMX_MEGUSTORE_BASE + 0x00000)
#define NFP3800_ARMX_MEG1USTORE_ECC                              (NFP3800_ARMX_MEGUSTORE_BASE + 0x10000)

/*================================================================*/


/* HGID: c3e5162fe280 */

#ifndef NFP3800_LONGNAMES

/*----------------------------------------------------------------
  Register: BulkBarCfg[8] - Bulk BAR Configuration Register
  Register: BulkBar0Boot - Boot value for the Bulk Bar 0 (ARM soft reset loads this value into Bulk Bar 0)
    Bits: 31    BarType(rw) - BAR type
    Bits: 30:27 Target(rw) - CPP command target ID
    Bits: 26:25 Token(rw) - CPP command token
    Bits: 24    Length(rw) - CPP command length increment
    Bits: 12:11 Preference(rw) - CPP master command bus preference
    Bits: 10:0  Address(rw) - Bits [39:29] of CPP command address. Bits [28:0] of CPP command are taken from the AXI command address.
*/
#define NFP3800_ARM_GCSR_BULK_BAR(_x)                        (0x00000000 + (0x4 * ((_x) & 0x7)))
#define NFP3800_ARM_GCSR_BULKBAR0BOOT                        0x0000015c
#define   NFP3800_ARM_GCSR_BULKBARCFG_TYPE                   BIT(31)
#define     NFP3800_ARM_GCSR_BULKBARCFG_TYPE_BULK            (0 << 31)
#define     NFP3800_ARM_GCSR_BULKBARCFG_TYPE_EXPA            BIT(31)
#define   NFP3800_ARM_GCSR_BULKBARCFG_TGT(_x)                (((_x) & 0xf) << 27)
#define   NFP3800_ARM_GCSR_BULKBARCFG_TGT_of(_x)             (((_x) >> 27) & 0xf)
#define   NFP3800_ARM_GCSR_BULKBARCFG_TOK(_x)                (((_x) & 0x3) << 25)
#define   NFP3800_ARM_GCSR_BULKBARCFG_TOK_of(_x)             (((_x) >> 25) & 0x3)
#define   NFP3800_ARM_GCSR_BULKBARCFG_LEN                    BIT(24)
#define     NFP3800_ARM_GCSR_BULKBARCFG_LEN_32BIT            (0 << 24)
#define     NFP3800_ARM_GCSR_BULKBARCFG_LEN_64BIT            BIT(24)
#define   NFP3800_ARM_GCSR_BULKBARCFG_PREF(_x)               (((_x) & 0x3) << 11)
#define   NFP3800_ARM_GCSR_BULKBARCFG_PREF_of(_x)            (((_x) >> 11) & 0x3)
#define     NFP3800_ARM_GCSR_BULKBARCFG_PREF_NONE            (0)
#define     NFP3800_ARM_GCSR_BULKBARCFG_PREF_PREF_A          (2)
#define     NFP3800_ARM_GCSR_BULKBARCFG_PREF_PREF_B          (3)
#define   NFP3800_ARM_GCSR_BULKBARCFG_ADDR(_x)               (((_x) & 0x7ff) << 0)
#define   NFP3800_ARM_GCSR_BULKBARCFG_ADDR_of(_x)            (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: ExpansionBarCfg[16] - Expansion BAR Configuration Register
  Register: ExpansionBar0Boot - Boot value for the Expansion Bar 0 (ARM soft reset loads this value into Expansion Bar 0)
    Bits: 31    BarType(rw) - BAR type
    Bits: 30:27 Target(rw) - CPP command target ID
    Bits: 26:25 Token(rw) - CPP command token
    Bits: 24    Length(rw) - CPP command length increment
    Bits: 23:19 Action(rw) - CPP Command Action Encoding
    Bits: 14:0  Address(rw) - Bits [39:25] of CPP command address. Bits [24:0] of CPP command are taken from the AXI command address.
*/
#define NFP3800_ARM_GCSR_EXPA_BAR(_x)                        (0x00000020 + (0x4 * ((_x) & 0xf)))
#define NFP3800_ARM_GCSR_EXPANSIONBAR0BOOT                   0x00000160
#define   NFP3800_ARM_GCSR_EXPANSIONBARCFG_TYPE              BIT(31)
#define     NFP3800_ARM_GCSR_EXPANSIONBARCFG_TYPE_EXPA       (0 << 31)
#define     NFP3800_ARM_GCSR_EXPANSIONBARCFG_TYPE_EXPL       BIT(31)
#define   NFP3800_ARM_GCSR_EXPANSIONBARCFG_TGT(_x)           (((_x) & 0xf) << 27)
#define   NFP3800_ARM_GCSR_EXPANSIONBARCFG_TGT_of(_x)        (((_x) >> 27) & 0xf)
#define   NFP3800_ARM_GCSR_EXPANSIONBARCFG_TOK(_x)           (((_x) & 0x3) << 25)
#define   NFP3800_ARM_GCSR_EXPANSIONBARCFG_TOK_of(_x)        (((_x) >> 25) & 0x3)
#define   NFP3800_ARM_GCSR_EXPANSIONBARCFG_LEN               BIT(24)
#define     NFP3800_ARM_GCSR_EXPANSIONBARCFG_LEN_32BIT       (0 << 24)
#define     NFP3800_ARM_GCSR_EXPANSIONBARCFG_LEN_64BIT       BIT(24)
#define   NFP3800_ARM_GCSR_EXPANSIONBARCFG_ACT(_x)           (((_x) & 0x1f) << 19)
#define   NFP3800_ARM_GCSR_EXPANSIONBARCFG_ACT_of(_x)        (((_x) >> 19) & 0x1f)
#define     NFP3800_ARM_GCSR_EXPANSIONBARCFG_ACT_DERIVED     (0)
#define   NFP3800_ARM_GCSR_EXPANSIONBARCFG_ADDR(_x)          (((_x) & 0x7fff) << 0)
#define   NFP3800_ARM_GCSR_EXPANSIONBARCFG_ADDR_of(_x)       (((_x) >> 0) & 0x7fff)
/*----------------------------------------------------------------
  Register: ExplicitBarCfg0[8] - Explicit BAR Configuration Register 0_Reg
    Bits: 17:0  Address(rw) - Bits [39:22] of CPP command address. Bits [21:0] of CPP command are taken from the AXI command address.
*/
#define NFP3800_ARM_GCSR_EXPL0_BAR(_x)                       (0x00000060 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_ARM_GCSR_EXPL0_BAR_ADDR(_x)                (((_x) & 0x3ffff) << 0)
#define   NFP3800_ARM_GCSR_EXPL0_BAR_ADDR_of(_x)             (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: ExplicitBarCfg1[8] - Explicit BAR Configuration Register 1_Reg
    Bits: 31    Posted(rw) - CPP command posted
    Bits: 30:24 SignalRef(rw) - CPP command signal ref
    Bits: 23:16 DataMaster(rw) - CPP command data master
    Bits: 13:0  DataRef(rw) - CPP command data ref
*/
#define NFP3800_ARM_GCSR_EXPL1_BAR(_x)                       (0x00000080 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_ARM_GCSR_EXPL1_BAR_POSTED                  BIT(31)
#define   NFP3800_ARM_GCSR_EXPL1_BAR_SIGNAL_REF(_x)          (((_x) & 0x7f) << 24)
#define   NFP3800_ARM_GCSR_EXPL1_BAR_SIGNAL_REF_of(_x)       (((_x) >> 24) & 0x7f)
#define   NFP3800_ARM_GCSR_EXPL1_BAR_DATA_MASTER(_x)         (((_x) & 0xff) << 16)
#define   NFP3800_ARM_GCSR_EXPL1_BAR_DATA_MASTER_of(_x)      (((_x) >> 16) & 0xff)
#define   NFP3800_ARM_GCSR_EXPL1_BAR_DATA_REF(_x)            (((_x) & 0x3fff) << 0)
#define   NFP3800_ARM_GCSR_EXPL1_BAR_DATA_REF_of(_x)         (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: ExplicitBarCfg2[8] - Explicit BAR Configuration Register 2_Reg
    Bits: 31:28 Target(rw) - CPP command target ID
    Bits: 27:23 Action(rw) - CPP Command Action Encoding
    Bits: 22:18 Length(rw) - CPP command length
    Bits: 17:10 ByteMask(rw) - CPP command byte mask
    Bits:  9:8  Token(rw) - CPP command token
    Bits:  7:0  SignalMaster(rw) - CPP command signal master
*/
#define NFP3800_ARM_GCSR_EXPL2_BAR(_x)                       (0x000000a0 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_ARM_GCSR_EXPL2_BAR_TGT(_x)                 (((_x) & 0xf) << 28)
#define   NFP3800_ARM_GCSR_EXPL2_BAR_TGT_of(_x)              (((_x) >> 28) & 0xf)
#define   NFP3800_ARM_GCSR_EXPL2_BAR_ACT(_x)                 (((_x) & 0x1f) << 23)
#define   NFP3800_ARM_GCSR_EXPL2_BAR_ACT_of(_x)              (((_x) >> 23) & 0x1f)
#define   NFP3800_ARM_GCSR_EXPL2_BAR_LEN(_x)                 (((_x) & 0x1f) << 18)
#define   NFP3800_ARM_GCSR_EXPL2_BAR_LEN_of(_x)              (((_x) >> 18) & 0x1f)
#define   NFP3800_ARM_GCSR_EXPL2_BAR_BYTE_MASK(_x)           (((_x) & 0xff) << 10)
#define   NFP3800_ARM_GCSR_EXPL2_BAR_BYTE_MASK_of(_x)        (((_x) >> 10) & 0xff)
#define   NFP3800_ARM_GCSR_EXPL2_BAR_TOK(_x)                 (((_x) & 0x3) << 8)
#define   NFP3800_ARM_GCSR_EXPL2_BAR_TOK_of(_x)              (((_x) >> 8) & 0x3)
#define   NFP3800_ARM_GCSR_EXPL2_BAR_SIGNAL_MASTER(_x)       (((_x) & 0xff) << 0)
#define   NFP3800_ARM_GCSR_EXPL2_BAR_SIGNAL_MASTER_of(_x)    (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: PostedCommandSignal[8] - Posted_Cmd_Signal
    Bits: 31:25 SignalB(rw) - Expected return signal ref for signal B
    Bits: 24    SignalBBus(rw) - Signal B data bus
    Bits: 23:17 SignalA(rw) - Expected return signal ref for signal A
    Bits: 16    SignalABus(rw) - Signal A data bus
    Bits: 11    NosplitReceivedB(rw) - Set to '1' by hardware when Signal B received and the appropriate [Pull/Push]_Signal_Nosplit is active, cleared on CSR write.
    Bits: 10    NosplitEnableB(rw) - Nosplit B and Signal B received will terminate a command waiting for both signals.
    Bits:  9    NosplitReceivedA(rw) - Set to '1' by hardware when Signal A received and the appropriate [Pull/Push]_Signal_Nosplit is active, cleared on CSR write.
    Bits:  8    NosplitEnableA(rw) - Nosplit A and Signal A received will terminate a command waiting for both signals.
    Bits:  7    SignalBReceived(rw) - Set to '1' by hardware when Signal B received, cleared on CSR write.
    Bits:  6    SignalBValid(rw) - Value in Signal B field is valid and must be received to complete cmd.
    Bits:  5    SignalAReceived(rw) - Set to '1' by hardware when Signal A received, cleared on CSR write.
    Bits:  4    SignalAValid(rw) - Value in Signal A field is valid and must be received to complete cmd.
    Bits:  0    CmdComplete(rw) - Command complete, set to '1' by hardware when all valid signal(s) received, cleared on CSR write.
*/
#define NFP3800_ARM_GCSR_EXPL_POST(_x)                       (0x000000c0 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_ARM_GCSR_EXPL_POST_SIG_B(_x)               (((_x) & 0x7f) << 25)
#define   NFP3800_ARM_GCSR_EXPL_POST_SIG_B_of(_x)            (((_x) >> 25) & 0x7f)
#define   NFP3800_ARM_GCSR_EXPL_POST_SIG_B_BUS               BIT(24)
#define     NFP3800_ARM_GCSR_EXPL_POST_SIG_B_BUS_PULL        (0 << 24)
#define     NFP3800_ARM_GCSR_EXPL_POST_SIG_B_BUS_PUSH        BIT(24)
#define   NFP3800_ARM_GCSR_EXPL_POST_SIG_A(_x)               (((_x) & 0x7f) << 17)
#define   NFP3800_ARM_GCSR_EXPL_POST_SIG_A_of(_x)            (((_x) >> 17) & 0x7f)
#define   NFP3800_ARM_GCSR_EXPL_POST_SIG_A_BUS               BIT(16)
#define     NFP3800_ARM_GCSR_EXPL_POST_SIG_A_BUS_PULL        (0 << 16)
#define     NFP3800_ARM_GCSR_EXPL_POST_SIG_A_BUS_PUSH        BIT(16)
#define   NFP3800_ARM_GCSR_EXPL_POST_NOSPLIT_RCVD_B          BIT(11)
#define   NFP3800_ARM_GCSR_EXPL_POST_NOSPLIT_ENABLE_B        BIT(10)
#define   NFP3800_ARM_GCSR_EXPL_POST_NOSPLIT_RCVD_A          BIT(9)
#define   NFP3800_ARM_GCSR_EXPL_POST_NOSPLIT_ENABLE_A        BIT(8)
#define   NFP3800_ARM_GCSR_EXPL_POST_SIG_B_RCVD              BIT(7)
#define   NFP3800_ARM_GCSR_EXPL_POST_SIG_B_VALID             BIT(6)
#define   NFP3800_ARM_GCSR_EXPL_POST_SIG_A_RCVD              BIT(5)
#define   NFP3800_ARM_GCSR_EXPL_POST_SIG_A_VALID             BIT(4)
#define   NFP3800_ARM_GCSR_EXPL_POST_CMD_COMPLETE            BIT(0)
/*----------------------------------------------------------------
  Register: MPCoreBaseAddress - MPCore Base Address
    Bits: 31:13 BaseAddress(rw) - Base address for CSRs in MPCore
*/
#define NFP3800_ARM_GCSR_MPCORE_BASE                         0x000000e0
#define   NFP3800_ARM_GCSR_MPCORE_BASE_ADDR(_x)              (((_x) & 0x7ffff) << 13)
#define   NFP3800_ARM_GCSR_MPCORE_BASE_ADDR_of(_x)           (((_x) >> 13) & 0x7ffff)
/*----------------------------------------------------------------
  Register: MPCoreCfg - MPCore Configuration
  Register: MPCoreCfgBoot - Boot value for the ARM MPCoreCfg (ARM soft reset loads this value into MPCoreCfg)
    Bits: 18:15 ARM_Cfg_NMI(rw) - NMI configuration.
    Bits: 13:12 MP0ConfigEndian(rw) - MP0 endianess
    Bits:  8    MP0RstVecLoc(ro) - MP0 Reset Vector location
    Bits:  7:4  MP0OutClkEnable(rw) - MP Output clock enable
*/
#define NFP3800_ARM_GCSR_MP0_CFG                             0x000000e8
#define NFP3800_ARM_GCSR_ARMCFG0BOOT                         0x00000164
#define   NFP3800_ARM_GCSR_MPCORECFG_ARMCFGNMI(_x)           (((_x) & 0xf) << 15)
#define   NFP3800_ARM_GCSR_MPCORECFG_ARMCFGNMI_of(_x)        (((_x) >> 15) & 0xf)
#define   NFP3800_ARM_GCSR_MPCORECFG_ENDIAN(_x)              (((_x) & 0x3) << 12)
#define   NFP3800_ARM_GCSR_MPCORECFG_ENDIAN_of(_x)           (((_x) >> 12) & 0x3)
#define     NFP3800_ARM_GCSR_MPCORECFG_ENDIAN_LITTLE         (0)
#define     NFP3800_ARM_GCSR_MPCORECFG_ENDIAN_BIG            (1)
#define   NFP3800_ARM_GCSR_MPCORECFG_RESET_VECTOR            BIT(8)
#define     NFP3800_ARM_GCSR_MPCORECFG_RESET_VECTOR_LO       (0 << 8)
#define     NFP3800_ARM_GCSR_MPCORECFG_RESET_VECTOR_HI       BIT(8)
#define   NFP3800_ARM_GCSR_MPCORECFG_OUTCLK_EN(_x)           (((_x) & 0xf) << 4)
#define   NFP3800_ARM_GCSR_MPCORECFG_OUTCLK_EN_of(_x)        (((_x) >> 4) & 0xf)
/*----------------------------------------------------------------
  Register: ArmStatus - ARM Status register
    Bits:  7    commrx_cpu0(ro) - MPCore0 commrx_cpu0.
    Bits:  6    commtx_cpu0(ro) - MPCore0 commtx_cpu0.
    Bits:  5    smpnamp(ro) - MPCore0 smpnamp.
    Bits:  4:3  pwrctl0(ro) - MPCore0 pwrctl.
    Bits:  2    wfe(ro) - MPCore0 wfe.
    Bits:  1    wfi(ro) - MPCore0 wfi.
*/
#define NFP3800_ARM_GCSR_ARMSTATUS                           0x00000104
#define   NFP3800_ARM_GCSR_ARMSTATUS_COMMRX_CPU0             BIT(7)
#define   NFP3800_ARM_GCSR_ARMSTATUS_COMMTX_CPU0             BIT(6)
#define   NFP3800_ARM_GCSR_ARMSTATUS_SMPNAMP                 BIT(5)
#define   NFP3800_ARM_GCSR_ARMSTATUS_PWRCTL0(_x)             (((_x) & 0x3) << 3)
#define   NFP3800_ARM_GCSR_ARMSTATUS_PWRCTL0_of(_x)          (((_x) >> 3) & 0x3)
#define   NFP3800_ARM_GCSR_ARMSTATUS_WFE                     BIT(2)
#define   NFP3800_ARM_GCSR_ARMSTATUS_WFI                     BIT(1)
/*----------------------------------------------------------------
  Register: GPIO_Sel - GPIO Mux Select register
    Bits: 31    Gpio_Sel_31(rw) - Selects NCSI_TXD[1] signal onto GPIO[31]. Uses TXD[1] OE
    Bits: 30    Gpio_Sel_30(rw) - Selects NCSI_TXD[0] signal onto GPIO[30]. Uses TXD[0] OE
    Bits: 29    Gpio_Sel_29(rw) - Selects NCSI_TX_EN signal onto GPIO[29]. Uses TX_EN OE
    Bits: 28    Gpio_Sel_28(ro) - Unused. NCSI_RX_D[1] maps in from GPIO_IN[28].
    Bits: 27    Gpio_Sel_27(ro) - Unused. NCSI_RX_D[0] maps in from GPIO_IN[27].
    Bits: 26    Gpio_Sel_26(ro) - Unused. SPIX_HOLD_N_IN signal maps in from GPIO_IN[26].
    Bits: 25    Gpio_Sel_25(ro) - Unused. NCSI_CRS_DV maps in from GPIO_IN[25].
    Bits: 24    Gpio_Sel_24(ro) - Unused. NCSI_REF_CLK maps in from GPIO_IN[24].
    Bits: 23    Gpio_Sel_23(rw) - Selects NCSI_ArbOut onto GPIO[23]. Use standard GPIO_OUT_OE[23].
    Bits: 22    Gpio_Sel_22(ro) - Unused. NCSI_ArbIn maps in from GPIO_IN[22]
    Bits: 21    Gpio_Sel_21(rw) - Selects I2C Data port 5 onto GPIO[21]. Selects I2C_SDA5 for GPIO_OUT_OE[21].
    Bits: 20    Gpio_Sel_20(rw) - Selects I2C clock port 5 onto GPIO[20]. Selects I2C_SCL5 for GPIO_OUT_OE[20].
    Bits: 19    Gpio_Sel_19(rw) - Selects I2C Data Port 4 onto GPIO[19]. Selects I2C_SDA4 for GPIO_OUT_OE[19].
    Bits: 18    Gpio_Sel_18(rw) - Selects I2C clock port 4 onto GPIO[18]. Selects I2C_SCL4 for GPIO_OUT_OE[18].
    Bits: 17    Gpio_Sel_17(rw) - Selects I2C Data Port 3 onto GPIO[17]. Selects I2C_SDA3 for GPIO_OUT_OE[17].
    Bits: 16    Gpio_Sel_16(rw) - Selects I2C clock port 3 onto GPIO[16]. Selects I2C_SCL3 GPIO_OUT_OE[16].
    Bits: 15    Gpio_Sel_15(rw) - Selects I2C Data Port 2 onto GPIO[15]. Selects I2C_SDA2 for GPIO_OUT_OE[15].
    Bits: 14    Gpio_Sel_14(rw) - Selects I2C clock port 2 onto GPIO[14]. Selects I2C_SCL2 for GPIO_OUT_OE[14].
    Bits: 13    Gpio_Sel_13(rw) - Selects SPIX_SEL3_OUT onto GPIO_OUT[13]. Selects SPIX_SEL3_ENB for GPIO_OUT_OE[13]. SPIX_SEL3_IN maps in from GPIO_IN[13]
    Bits: 12    Gpio_Sel_12(rw) - Selects SPIX_SEL2_OUT onto GPIO_OUT[12]. Selects SPIX_SEL2_ENB for GPIO_OUT_OE[12]. SPIX_SEL2_IN maps in from GPIO_IN[12]
    Bits: 11    Gpio_Sel_11(rw) - Selects SPIX_SEL1_OUT onto GPIO_OUT[11]. Selects SPIX_SEL1_ENB for GPIO_OUT_OE[11]. SPIX_SEL1_IN maps in from GPIO_IN[11]
    Bits: 10    Gpio_Sel_10(ro) - Unused. MAC_XONOFF1 maps in from GPIO_IN[10].
    Bits:  9    Gpio_Sel_9(ro) - Unused. MAC_XONOFF0 maps in from GPIO_IN[9].
    Bits:  8    Gpio_Sel_8(rw) - Selects UART1_TXD onto GPIO_OUT[8]. Use standard GPIO_OUT_OE[8].
    Bits:  7    Gpio_Sel_7(ro) - Unused. UART1_RXD maps in from GPIO_IN[7]
    Bits:  6    Gpio_Sel_6(rw) - Selects I2C Data Port 7 onto GPIO[6]. Selects I2C_SDA7 for GPIO_OUT_OE[6].
    Bits:  5    Gpio_Sel_5(rw) - Selects I2C clock port 7 onto GPIO[5]. Selects I2C_SCL2 for GPIO_OUT_OE[14].
*/
#define NFP3800_ARM_GCSR_GPIO_SEL                            0x00000108
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_31              BIT(31)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_31_GPIO_OUT_R31 (0 << 31)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_31_NCSI_TXD1  BIT(31)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_30              BIT(30)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_30_GPIO_OUT_R30 (0 << 30)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_30_NCSI_TXD0  BIT(30)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_29              BIT(29)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_29_GPIO_OUT_R29 (0 << 29)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_29_NCSI_TX_EN BIT(29)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_28              BIT(28)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_27              BIT(27)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_26              BIT(26)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_25              BIT(25)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_24              BIT(24)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_23              BIT(23)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_23_GPIO_OUT_R23 (0 << 23)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_23_NCSI_ARBOUT BIT(23)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_22              BIT(22)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_21              BIT(21)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_21_GPIO_OUT_R21 (0 << 21)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_21_I2C_SDA5   BIT(21)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_20              BIT(20)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_20_GPIO_OUT_R20 (0 << 20)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_20_I2C_SCL5   BIT(20)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_19              BIT(19)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_19_GPIO_OUT_R19 (0 << 19)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_19_I2C_SDA4   BIT(19)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_18              BIT(18)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_18_GPIO_OUT_R18 (0 << 18)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_18_I2C_SCL4   BIT(18)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_17              BIT(17)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_17_GPIO_OUT_R17 (0 << 17)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_17_I2C_SDA3   BIT(17)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_16              BIT(16)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_16_GPIO_OUT_R16 (0 << 16)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_16_I2C_SCL3   BIT(16)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_15              BIT(15)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_15_GPIO_OUT_R15 (0 << 15)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_15_I2C_SDA2   BIT(15)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_14              BIT(14)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_14_GPIO_OUT_R14 (0 << 14)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_14_I2C_SCL2   BIT(14)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_13              BIT(13)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_13_GPIO_OUT_R13 (0 << 13)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_13_SPIX_SEL3  BIT(13)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_12              BIT(12)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_12_GPIO_OUT_R12 (0 << 12)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_12_SPIX_SEL2  BIT(12)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_11              BIT(11)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_11_GPIO_OUT_R11 (0 << 11)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_11_SPIX_SEL1  BIT(11)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_10              BIT(10)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_9               BIT(9)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_8               BIT(8)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_8_UART_TXD    BIT(8)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_7               BIT(7)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_6               BIT(6)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_6_GPIO_OUT_R6 (0 << 6)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_6_I2C_SDA7    BIT(6)
#define   NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_5               BIT(5)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_5_GPIO_OUT_R5 (0 << 5)
#define     NFP3800_ARM_GCSR_GPIO_SEL_GPIO_SEL_5_I2C_SCL7    BIT(5)
/*----------------------------------------------------------------
  Register: SIG_MUX_Sel - I/O Signal Mux Select register
    Bits: 31    Sig_Sel_31(rw) - Select GPIO[31] signal onto SMBALRT pin.
    Bits: 30:29 Sig_Sel_29(rw) - Select GPIO[30] or SDA6 signals onto SMBDAT pin.
    Bits: 28:27 Sig_Sel_27(rw) - Select GPIO[29] or SCL6 signals onto SMBCLK pin.
    Bits: 26    Sig_Sel_26(rw) - Select GPIO[28] signal onto UART0_TX pin.
    Bits: 25    Sig_Sel_25(rw) - Select GPIO[27] signal onto UART0_RX pin.
    Bits: 24    Sig_Sel_24(rw) - Select GPIO[26] signal onto SPIX_D3 pin.
    Bits: 23:22 Sig_Sel_22(rw) - Select GPIO[25] or MAC_ACTIVITY signal onto SPIX_D2 pin.
    Bits: 21    Sig_Sel_21(rw) - Select GPIO[23] signal onto SPI3_MOSI pin.
    Bits: 20    Sig_Sel_20(rw) - Select GPIO[22] signal onto SPI3_SCK pin.
    Bits: 19    Sig_Sel_19(rw) - Select GPIO[21] or signal onto SPI2_MOSI pin.
    Bits: 18    Sig_Sel_18(rw) - Select GPIO[20] or signal onto SPI2_SCK pin.
    Bits: 17    Sig_Sel_17(rw) - Select GPIO[19] signal onto SPI0_MISO pin.
    Bits: 16    Sig_Sel_16(rw) - Select GPIO[18] signal onto SPI0_SEL pin.
    Bits: 15    Sig_Sel_15(rw) - Select GPIO[17] signal onto SPI0_MOSI pin.
    Bits: 14    Sig_Sel_14(rw) - Select GPIO[16] signal onto SPI0_SCK pin.
*/
#define NFP3800_ARM_GCSR_SIG_MUX_SEL                         0x0000010c
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_31            BIT(31)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_31_SMBALRT  (0 << 31)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_31_GPIO_OUT_R31 BIT(31)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_29(_x)        (((_x) & 0x3) << 29)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_29_of(_x)     (((_x) >> 29) & 0x3)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_29_SMBDAT   (0)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_29_SDA6     (1)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_29_GPIO_OUT_R30 (2)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_27(_x)        (((_x) & 0x3) << 27)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_27_of(_x)     (((_x) >> 27) & 0x3)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_27_SMBCLK   (0)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_27_SCL6     (1)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_27_GPIO_OUT_R29 (2)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_26            BIT(26)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_26_UART0_TX (0 << 26)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_26_GPIO_OUT_R28 BIT(26)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_25            BIT(25)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_25_UART0_RX (0 << 25)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_25_GPIO_OUT_R27 BIT(25)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_24            BIT(24)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_24_SPIX_D3  (0 << 24)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_24_GPIO_OUT_R26 BIT(24)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_22(_x)        (((_x) & 0x3) << 22)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_22_of(_x)     (((_x) >> 22) & 0x3)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_22_SPIX_D2  (0)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_22_MAC_ACTIVITY (1)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_22_GPIO_OUT_R25 (2)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_21            BIT(21)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_21_SPI3_MOSI(MDIO1) (0 << 21)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_21_GPIO_OUT_R23 BIT(21)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_20            BIT(20)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_20_SPI3_SCK(MDC1) (0 << 20)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_20_GPIO_OUT_R22 BIT(20)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_19            BIT(19)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_19_SPI2_MOSI(MDIO0) (0 << 19)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_19_GPIO_OUT_R21 BIT(19)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_18            BIT(18)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_18_SPI2_SCK(MDC0) (0 << 18)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_18_GPIO_OUT_R20 BIT(18)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_17            BIT(17)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_17_SPI0_MISO (0 << 17)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_17_GPIO_OUT_R19 BIT(17)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_16            BIT(16)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_16_SPI0_SEL (0 << 16)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_16_GPIO_OUT_R18 BIT(16)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_15            BIT(15)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_15_SPI0_MOSI (0 << 15)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_15_GPIO_OUT_R17 BIT(15)
#define   NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_14            BIT(14)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_14_SPI0_SCK (0 << 14)
#define     NFP3800_ARM_GCSR_SIG_MUX_SEL_SIG_SEL_14_GPIO_OUT_R16 BIT(14)
/*----------------------------------------------------------------
  Register: SoftModel0 - Soft Model 0 register
  Register: SoftModel1 - Soft Model 1 register
  Register: SoftModel2 - Soft Model 2 register
  Register: SoftModel3 - Soft Model 3 register
  Register: SoftModel4 - Soft Model 4 register
  Register: SoftModel5 - Soft Model 5 register
  Register: SoftModel6 - Soft Model 6 register
  Register: SoftModel7 - Soft Model 7 register
    Bits: 31:0  SoftModel(rw) - Soft Model value is loaded by the configurater and/or software. Interpretation of the bits is up to software.
*/
#define NFP3800_ARM_GCSR_SOFTMODEL0                          0x00000144
#define NFP3800_ARM_GCSR_SOFTMODEL1                          0x00000148
#define NFP3800_ARM_GCSR_SOFTMODEL2                          0x0000014c
#define NFP3800_ARM_GCSR_SOFTMODEL3                          0x00000150
#define NFP3800_ARM_GCSR_SOFTMODEL4                          0x000001b0
#define NFP3800_ARM_GCSR_SOFTMODEL5                          0x000001b4
#define NFP3800_ARM_GCSR_SOFTMODEL6                          0x000001b8
#define NFP3800_ARM_GCSR_SOFTMODEL7                          0x000001bc
#define   NFP3800_ARM_GCSR_SOFTMODEL_SOFTMODEL(_x)           (((_x) & 0xffffffff) << 0)
#define   NFP3800_ARM_GCSR_SOFTMODEL_SOFTMODEL_of(_x)        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SoftModelLock - Soft Model Lock register
    Bits: 31:0  SoftModelLock(rw) - This lock controls the writability of the eight SoftModel[7:0] registers. On reads, the MSB is zero if the registers are UNLOCKED, set otherwise. If the lock is UNLOCKED, any write to this register will set the lock. Once locked, this register must be written consecutivly with the sequence 0x4f70656e, 0x2053657a, 0x204d6521 to unlock it.
*/
#define NFP3800_ARM_GCSR_SOFTMODELLOCK                       0x00000154
#define   NFP3800_ARM_GCSR_SOFTMODELLOCK_SOFTMODELLOCK(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP3800_ARM_GCSR_SOFTMODELLOCK_SOFTMODELLOCK_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CfgConfigSts - Configurater Configuration Status register
    Bits:  8    CfgCompleted(ro) - This bit is set to indicate the configurater completed the configuration process.
    Bits:  7:5  Cfg2Err(ro) - This field indicates the error type for the second image copy. Only valid if CfgCompleted is set.
    Bits:  4:2  Cfg1Err(ro) - This field indicates the error type for the first image copy. Only valid if CfgImage is set.
    Bits:  1    CfgSuccess(ro) - This bit is set if the configurater successfully completed the configuration process.
    Bits:  0    CfgImage(ro) - Zero indicates the configurater suceeded using the first image. One indicates the first image failed and the second image was used.
*/
#define NFP3800_ARM_GCSR_CFGCONFIGSTS                        0x00000158
#define   NFP3800_ARM_GCSR_CFGCONFIGSTS_CFGCOMPLETED         BIT(8)
#define   NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG2ERR(_x)          (((_x) & 0x7) << 5)
#define   NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG2ERR_of(_x)       (((_x) >> 5) & 0x7)
#define     NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG2ERR_NOERR      (0)
#define     NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG2ERR_BADHDR     (1)
#define     NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG2ERR_BADOPC     (2)
#define     NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG2ERR_BADCHK     (3)
#define     NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG2ERR_BADCS      (4)
#define     NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG2ERR_BADSRST    (7)
#define   NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG1ERR(_x)          (((_x) & 0x7) << 2)
#define   NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG1ERR_of(_x)       (((_x) >> 2) & 0x7)
#define     NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG1ERR_NOERR      (0)
#define     NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG1ERR_BADHDR     (1)
#define     NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG1ERR_BADOPC     (2)
#define     NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG1ERR_BADCHK     (3)
#define     NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG1ERR_BADCS      (4)
#define     NFP3800_ARM_GCSR_CFGCONFIGSTS_CFG1ERR_BADSRST    (7)
#define   NFP3800_ARM_GCSR_CFGCONFIGSTS_CFGSUCCESS           BIT(1)
#define   NFP3800_ARM_GCSR_CFGCONFIGSTS_CFGIMAGE             BIT(0)
/*----------------------------------------------------------------
  Register: JumpToSelf[16] - Jump To Self opcode register
    Bits: 31:0  JumpToSelf(ro) - This register provides a jump to self instruction to the arm core when the configurater process fails and the arm is spun up for the arm JTAG to come in and program the configurater flash.
*/
#define NFP3800_ARM_GCSR_JUMPTOSELF(_x)                      (0x00000400 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_ARM_GCSR_JUMPTOSELF_JUMPTOSELF(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP3800_ARM_GCSR_JUMPTOSELF_JUMPTOSELF_of(_x)      (((_x) >> 0) & 0xffffffff)

#else /* NFP3800_LONGNAMES */

/*----------------------------------------------------------------
  Register: BulkBarCfg[8] - Bulk BAR Configuration Register
  Register: BulkBar0Boot - Boot value for the Bulk Bar 0 (ARM soft reset loads this value into Bulk Bar 0)
    Bits: 31    BarType(rw) - BAR type
    Bits: 30:27 Target(rw) - CPP command target ID
    Bits: 26:25 Token(rw) - CPP command token
    Bits: 24    Length(rw) - CPP command length increment
    Bits: 12:11 Preference(rw) - CPP master command bus preference
    Bits: 10:0  Address(rw) - Bits [39:29] of CPP command address. Bits [28:0] of CPP command are taken from the AXI command address.
*/
#define NFP3800_ARM_GCSR_BulkBarCfg(_x)                      (0x00000000 + (0x4 * ((_x) & 0x7)))
#define NFP3800_ARM_GCSR_BulkBar0Boot                        0x0000015c
#define   NFP3800_ARM_GCSR_BulkBarCfg_BarType                BIT(31)
#define     NFP3800_ARM_GCSR_BulkBarCfg_BarType_BULK         (0 << 31)
#define     NFP3800_ARM_GCSR_BulkBarCfg_BarType_EXPA         BIT(31)
#define   NFP3800_ARM_GCSR_BulkBarCfg_Target(_x)             (((_x) & 0xf) << 27)
#define   NFP3800_ARM_GCSR_BulkBarCfg_Target_of(_x)          (((_x) >> 27) & 0xf)
#define   NFP3800_ARM_GCSR_BulkBarCfg_Token(_x)              (((_x) & 0x3) << 25)
#define   NFP3800_ARM_GCSR_BulkBarCfg_Token_of(_x)           (((_x) >> 25) & 0x3)
#define   NFP3800_ARM_GCSR_BulkBarCfg_Length                 BIT(24)
#define     NFP3800_ARM_GCSR_BulkBarCfg_Length_32BIT         (0 << 24)
#define     NFP3800_ARM_GCSR_BulkBarCfg_Length_64BIT         BIT(24)
#define   NFP3800_ARM_GCSR_BulkBarCfg_Preference(_x)         (((_x) & 0x3) << 11)
#define   NFP3800_ARM_GCSR_BulkBarCfg_Preference_of(_x)      (((_x) >> 11) & 0x3)
#define     NFP3800_ARM_GCSR_BulkBarCfg_Preference_None      (0)
#define     NFP3800_ARM_GCSR_BulkBarCfg_Preference_Pref_A    (2)
#define     NFP3800_ARM_GCSR_BulkBarCfg_Preference_Pref_B    (3)
#define   NFP3800_ARM_GCSR_BulkBarCfg_Address(_x)            (((_x) & 0x7ff) << 0)
#define   NFP3800_ARM_GCSR_BulkBarCfg_Address_of(_x)         (((_x) >> 0) & 0x7ff)
/*----------------------------------------------------------------
  Register: ExpansionBarCfg[16] - Expansion BAR Configuration Register
  Register: ExpansionBar0Boot - Boot value for the Expansion Bar 0 (ARM soft reset loads this value into Expansion Bar 0)
    Bits: 31    BarType(rw) - BAR type
    Bits: 30:27 Target(rw) - CPP command target ID
    Bits: 26:25 Token(rw) - CPP command token
    Bits: 24    Length(rw) - CPP command length increment
    Bits: 23:19 Action(rw) - CPP Command Action Encoding
    Bits: 14:0  Address(rw) - Bits [39:25] of CPP command address. Bits [24:0] of CPP command are taken from the AXI command address.
*/
#define NFP3800_ARM_GCSR_ExpansionBarCfg(_x)                 (0x00000020 + (0x4 * ((_x) & 0xf)))
#define NFP3800_ARM_GCSR_ExpansionBar0Boot                   0x00000160
#define   NFP3800_ARM_GCSR_ExpansionBarCfg_BarType           BIT(31)
#define     NFP3800_ARM_GCSR_ExpansionBarCfg_BarType_EXPA    (0 << 31)
#define     NFP3800_ARM_GCSR_ExpansionBarCfg_BarType_EXPL    BIT(31)
#define   NFP3800_ARM_GCSR_ExpansionBarCfg_Target(_x)        (((_x) & 0xf) << 27)
#define   NFP3800_ARM_GCSR_ExpansionBarCfg_Target_of(_x)     (((_x) >> 27) & 0xf)
#define   NFP3800_ARM_GCSR_ExpansionBarCfg_Token(_x)         (((_x) & 0x3) << 25)
#define   NFP3800_ARM_GCSR_ExpansionBarCfg_Token_of(_x)      (((_x) >> 25) & 0x3)
#define   NFP3800_ARM_GCSR_ExpansionBarCfg_Length            BIT(24)
#define     NFP3800_ARM_GCSR_ExpansionBarCfg_Length_32BIT    (0 << 24)
#define     NFP3800_ARM_GCSR_ExpansionBarCfg_Length_64BIT    BIT(24)
#define   NFP3800_ARM_GCSR_ExpansionBarCfg_Action(_x)        (((_x) & 0x1f) << 19)
#define   NFP3800_ARM_GCSR_ExpansionBarCfg_Action_of(_x)     (((_x) >> 19) & 0x1f)
#define     NFP3800_ARM_GCSR_ExpansionBarCfg_Action_DERIVED  (0)
#define   NFP3800_ARM_GCSR_ExpansionBarCfg_Address(_x)       (((_x) & 0x7fff) << 0)
#define   NFP3800_ARM_GCSR_ExpansionBarCfg_Address_of(_x)    (((_x) >> 0) & 0x7fff)
/*----------------------------------------------------------------
  Register: ExplicitBarCfg0[8] - Explicit BAR Configuration Register 0_Reg
    Bits: 17:0  Address(rw) - Bits [39:22] of CPP command address. Bits [21:0] of CPP command are taken from the AXI command address.
*/
#define NFP3800_ARM_GCSR_ExplicitBarCfg0(_x)                 (0x00000060 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_ARM_GCSR_ExplicitBarCfg0_Address(_x)       (((_x) & 0x3ffff) << 0)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg0_Address_of(_x)    (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: ExplicitBarCfg1[8] - Explicit BAR Configuration Register 1_Reg
    Bits: 31    Posted(rw) - CPP command posted
    Bits: 30:24 SignalRef(rw) - CPP command signal ref
    Bits: 23:16 DataMaster(rw) - CPP command data master
    Bits: 13:0  DataRef(rw) - CPP command data ref
*/
#define NFP3800_ARM_GCSR_ExplicitBarCfg1(_x)                 (0x00000080 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_ARM_GCSR_ExplicitBarCfg1_Posted            BIT(31)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg1_SignalRef(_x)     (((_x) & 0x7f) << 24)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg1_SignalRef_of(_x)  (((_x) >> 24) & 0x7f)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg1_DataMaster(_x)    (((_x) & 0xff) << 16)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg1_DataMaster_of(_x) (((_x) >> 16) & 0xff)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg1_DataRef(_x)       (((_x) & 0x3fff) << 0)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg1_DataRef_of(_x)    (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: ExplicitBarCfg2[8] - Explicit BAR Configuration Register 2_Reg
    Bits: 31:28 Target(rw) - CPP command target ID
    Bits: 27:23 Action(rw) - CPP Command Action Encoding
    Bits: 22:18 Length(rw) - CPP command length
    Bits: 17:10 ByteMask(rw) - CPP command byte mask
    Bits:  9:8  Token(rw) - CPP command token
    Bits:  7:0  SignalMaster(rw) - CPP command signal master
*/
#define NFP3800_ARM_GCSR_ExplicitBarCfg2(_x)                 (0x000000a0 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_ARM_GCSR_ExplicitBarCfg2_Target(_x)        (((_x) & 0xf) << 28)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg2_Target_of(_x)     (((_x) >> 28) & 0xf)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg2_Action(_x)        (((_x) & 0x1f) << 23)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg2_Action_of(_x)     (((_x) >> 23) & 0x1f)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg2_Length(_x)        (((_x) & 0x1f) << 18)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg2_Length_of(_x)     (((_x) >> 18) & 0x1f)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg2_ByteMask(_x)      (((_x) & 0xff) << 10)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg2_ByteMask_of(_x)   (((_x) >> 10) & 0xff)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg2_Token(_x)         (((_x) & 0x3) << 8)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg2_Token_of(_x)      (((_x) >> 8) & 0x3)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg2_SignalMaster(_x)  (((_x) & 0xff) << 0)
#define   NFP3800_ARM_GCSR_ExplicitBarCfg2_SignalMaster_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: PostedCommandSignal[8] - Posted_Cmd_Signal
    Bits: 31:25 SignalB(rw) - Expected return signal ref for signal B
    Bits: 24    SignalBBus(rw) - Signal B data bus
    Bits: 23:17 SignalA(rw) - Expected return signal ref for signal A
    Bits: 16    SignalABus(rw) - Signal A data bus
    Bits: 11    NosplitReceivedB(rw) - Set to '1' by hardware when Signal B received and the appropriate [Pull/Push]_Signal_Nosplit is active, cleared on CSR write.
    Bits: 10    NosplitEnableB(rw) - Nosplit B and Signal B received will terminate a command waiting for both signals.
    Bits:  9    NosplitReceivedA(rw) - Set to '1' by hardware when Signal A received and the appropriate [Pull/Push]_Signal_Nosplit is active, cleared on CSR write.
    Bits:  8    NosplitEnableA(rw) - Nosplit A and Signal A received will terminate a command waiting for both signals.
    Bits:  7    SignalBReceived(rw) - Set to '1' by hardware when Signal B received, cleared on CSR write.
    Bits:  6    SignalBValid(rw) - Value in Signal B field is valid and must be received to complete cmd.
    Bits:  5    SignalAReceived(rw) - Set to '1' by hardware when Signal A received, cleared on CSR write.
    Bits:  4    SignalAValid(rw) - Value in Signal A field is valid and must be received to complete cmd.
    Bits:  0    CmdComplete(rw) - Command complete, set to '1' by hardware when all valid signal(s) received, cleared on CSR write.
*/
#define NFP3800_ARM_GCSR_PostedCommandSignal(_x)             (0x000000c0 + (0x4 * ((_x) & 0x7)))
#define   NFP3800_ARM_GCSR_PostedCommandSignal_SignalB(_x)   (((_x) & 0x7f) << 25)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_SignalB_of(_x) (((_x) >> 25) & 0x7f)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_SignalBBus    BIT(24)
#define     NFP3800_ARM_GCSR_PostedCommandSignal_SignalBBus_PULL (0 << 24)
#define     NFP3800_ARM_GCSR_PostedCommandSignal_SignalBBus_PUSH BIT(24)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_SignalA(_x)   (((_x) & 0x7f) << 17)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_SignalA_of(_x) (((_x) >> 17) & 0x7f)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_SignalABus    BIT(16)
#define     NFP3800_ARM_GCSR_PostedCommandSignal_SignalABus_PULL (0 << 16)
#define     NFP3800_ARM_GCSR_PostedCommandSignal_SignalABus_PUSH BIT(16)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_NosplitReceivedB BIT(11)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_NosplitEnableB BIT(10)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_NosplitReceivedA BIT(9)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_NosplitEnableA BIT(8)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_SignalBReceived BIT(7)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_SignalBValid  BIT(6)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_SignalAReceived BIT(5)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_SignalAValid  BIT(4)
#define   NFP3800_ARM_GCSR_PostedCommandSignal_CmdComplete   BIT(0)
/*----------------------------------------------------------------
  Register: MPCoreBaseAddress - MPCore Base Address
    Bits: 31:13 BaseAddress(rw) - Base address for CSRs in MPCore
*/
#define NFP3800_ARM_GCSR_MPCoreBaseAddress                   0x000000e0
#define   NFP3800_ARM_GCSR_MPCoreBaseAddress_BaseAddress(_x) (((_x) & 0x7ffff) << 13)
#define   NFP3800_ARM_GCSR_MPCoreBaseAddress_BaseAddress_of(_x) (((_x) >> 13) & 0x7ffff)
/*----------------------------------------------------------------
  Register: MPCoreCfg - MPCore Configuration
  Register: MPCoreCfgBoot - Boot value for the ARM MPCoreCfg (ARM soft reset loads this value into MPCoreCfg)
    Bits: 18:15 ARM_Cfg_NMI(rw) - NMI configuration.
    Bits: 13:12 MP0ConfigEndian(rw) - MP0 endianess
    Bits:  8    MP0RstVecLoc(ro) - MP0 Reset Vector location
    Bits:  7:4  MP0OutClkEnable(rw) - MP Output clock enable
*/
#define NFP3800_ARM_GCSR_MPCoreCfg                           0x000000e8
#define NFP3800_ARM_GCSR_MPCoreCfgBoot                       0x00000164
#define   NFP3800_ARM_GCSR_MPCoreCfg_ARM_Cfg_NMI(_x)         (((_x) & 0xf) << 15)
#define   NFP3800_ARM_GCSR_MPCoreCfg_ARM_Cfg_NMI_of(_x)      (((_x) >> 15) & 0xf)
#define   NFP3800_ARM_GCSR_MPCoreCfg_MP0ConfigEndian(_x)     (((_x) & 0x3) << 12)
#define   NFP3800_ARM_GCSR_MPCoreCfg_MP0ConfigEndian_of(_x)  (((_x) >> 12) & 0x3)
#define     NFP3800_ARM_GCSR_MPCoreCfg_MP0ConfigEndian_LITTLE (0)
#define     NFP3800_ARM_GCSR_MPCoreCfg_MP0ConfigEndian_BIG   (1)
#define   NFP3800_ARM_GCSR_MPCoreCfg_MP0RstVecLoc            BIT(8)
#define     NFP3800_ARM_GCSR_MPCoreCfg_MP0RstVecLoc_LO       (0 << 8)
#define     NFP3800_ARM_GCSR_MPCoreCfg_MP0RstVecLoc_HI       BIT(8)
#define   NFP3800_ARM_GCSR_MPCoreCfg_MP0OutClkEnable(_x)     (((_x) & 0xf) << 4)
#define   NFP3800_ARM_GCSR_MPCoreCfg_MP0OutClkEnable_of(_x)  (((_x) >> 4) & 0xf)
/*----------------------------------------------------------------
  Register: ArmStatus - ARM Status register
    Bits:  7    commrx_cpu0(ro) - MPCore0 commrx_cpu0.
    Bits:  6    commtx_cpu0(ro) - MPCore0 commtx_cpu0.
    Bits:  5    smpnamp(ro) - MPCore0 smpnamp.
    Bits:  4:3  pwrctl0(ro) - MPCore0 pwrctl.
    Bits:  2    wfe(ro) - MPCore0 wfe.
    Bits:  1    wfi(ro) - MPCore0 wfi.
*/
#define NFP3800_ARM_GCSR_ArmStatus                           0x00000104
#define   NFP3800_ARM_GCSR_ArmStatus_commrx_cpu0             BIT(7)
#define   NFP3800_ARM_GCSR_ArmStatus_commtx_cpu0             BIT(6)
#define   NFP3800_ARM_GCSR_ArmStatus_smpnamp                 BIT(5)
#define   NFP3800_ARM_GCSR_ArmStatus_pwrctl0(_x)             (((_x) & 0x3) << 3)
#define   NFP3800_ARM_GCSR_ArmStatus_pwrctl0_of(_x)          (((_x) >> 3) & 0x3)
#define   NFP3800_ARM_GCSR_ArmStatus_wfe                     BIT(2)
#define   NFP3800_ARM_GCSR_ArmStatus_wfi                     BIT(1)
/*----------------------------------------------------------------
  Register: GPIO_Sel - GPIO Mux Select register
    Bits: 31    Gpio_Sel_31(rw) - Selects NCSI_TXD[1] signal onto GPIO[31]. Uses TXD[1] OE
    Bits: 30    Gpio_Sel_30(rw) - Selects NCSI_TXD[0] signal onto GPIO[30]. Uses TXD[0] OE
    Bits: 29    Gpio_Sel_29(rw) - Selects NCSI_TX_EN signal onto GPIO[29]. Uses TX_EN OE
    Bits: 28    Gpio_Sel_28(ro) - Unused. NCSI_RX_D[1] maps in from GPIO_IN[28].
    Bits: 27    Gpio_Sel_27(ro) - Unused. NCSI_RX_D[0] maps in from GPIO_IN[27].
    Bits: 26    Gpio_Sel_26(ro) - Unused. SPIX_HOLD_N_IN signal maps in from GPIO_IN[26].
    Bits: 25    Gpio_Sel_25(ro) - Unused. NCSI_CRS_DV maps in from GPIO_IN[25].
    Bits: 24    Gpio_Sel_24(ro) - Unused. NCSI_REF_CLK maps in from GPIO_IN[24].
    Bits: 23    Gpio_Sel_23(rw) - Selects NCSI_ArbOut onto GPIO[23]. Use standard GPIO_OUT_OE[23].
    Bits: 22    Gpio_Sel_22(ro) - Unused. NCSI_ArbIn maps in from GPIO_IN[22]
    Bits: 21    Gpio_Sel_21(rw) - Selects I2C Data port 5 onto GPIO[21]. Selects I2C_SDA5 for GPIO_OUT_OE[21].
    Bits: 20    Gpio_Sel_20(rw) - Selects I2C clock port 5 onto GPIO[20]. Selects I2C_SCL5 for GPIO_OUT_OE[20].
    Bits: 19    Gpio_Sel_19(rw) - Selects I2C Data Port 4 onto GPIO[19]. Selects I2C_SDA4 for GPIO_OUT_OE[19].
    Bits: 18    Gpio_Sel_18(rw) - Selects I2C clock port 4 onto GPIO[18]. Selects I2C_SCL4 for GPIO_OUT_OE[18].
    Bits: 17    Gpio_Sel_17(rw) - Selects I2C Data Port 3 onto GPIO[17]. Selects I2C_SDA3 for GPIO_OUT_OE[17].
    Bits: 16    Gpio_Sel_16(rw) - Selects I2C clock port 3 onto GPIO[16]. Selects I2C_SCL3 GPIO_OUT_OE[16].
    Bits: 15    Gpio_Sel_15(rw) - Selects I2C Data Port 2 onto GPIO[15]. Selects I2C_SDA2 for GPIO_OUT_OE[15].
    Bits: 14    Gpio_Sel_14(rw) - Selects I2C clock port 2 onto GPIO[14]. Selects I2C_SCL2 for GPIO_OUT_OE[14].
    Bits: 13    Gpio_Sel_13(rw) - Selects SPIX_SEL3_OUT onto GPIO_OUT[13]. Selects SPIX_SEL3_ENB for GPIO_OUT_OE[13]. SPIX_SEL3_IN maps in from GPIO_IN[13]
    Bits: 12    Gpio_Sel_12(rw) - Selects SPIX_SEL2_OUT onto GPIO_OUT[12]. Selects SPIX_SEL2_ENB for GPIO_OUT_OE[12]. SPIX_SEL2_IN maps in from GPIO_IN[12]
    Bits: 11    Gpio_Sel_11(rw) - Selects SPIX_SEL1_OUT onto GPIO_OUT[11]. Selects SPIX_SEL1_ENB for GPIO_OUT_OE[11]. SPIX_SEL1_IN maps in from GPIO_IN[11]
    Bits: 10    Gpio_Sel_10(ro) - Unused. MAC_XONOFF1 maps in from GPIO_IN[10].
    Bits:  9    Gpio_Sel_9(ro) - Unused. MAC_XONOFF0 maps in from GPIO_IN[9].
    Bits:  8    Gpio_Sel_8(rw) - Selects UART1_TXD onto GPIO_OUT[8]. Use standard GPIO_OUT_OE[8].
    Bits:  7    Gpio_Sel_7(ro) - Unused. UART1_RXD maps in from GPIO_IN[7]
    Bits:  6    Gpio_Sel_6(rw) - Selects I2C Data Port 7 onto GPIO[6]. Selects I2C_SDA7 for GPIO_OUT_OE[6].
    Bits:  5    Gpio_Sel_5(rw) - Selects I2C clock port 7 onto GPIO[5]. Selects I2C_SCL2 for GPIO_OUT_OE[14].
*/
#define NFP3800_ARM_GCSR_GPIO_Sel                            0x00000108
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_31              BIT(31)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_31_Gpio_Out_R31 (0 << 31)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_31_NCSI_TXD1  BIT(31)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_30              BIT(30)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_30_Gpio_Out_R30 (0 << 30)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_30_NCSI_TXD0  BIT(30)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_29              BIT(29)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_29_Gpio_Out_R29 (0 << 29)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_29_NCSI_TX_EN BIT(29)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_28              BIT(28)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_27              BIT(27)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_26              BIT(26)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_25              BIT(25)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_24              BIT(24)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_23              BIT(23)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_23_Gpio_Out_R23 (0 << 23)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_23_NCSI_ArbOut BIT(23)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_22              BIT(22)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_21              BIT(21)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_21_Gpio_Out_R21 (0 << 21)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_21_I2C_SDA5   BIT(21)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_20              BIT(20)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_20_Gpio_Out_R20 (0 << 20)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_20_I2C_SCL5   BIT(20)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_19              BIT(19)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_19_Gpio_Out_R19 (0 << 19)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_19_I2C_SDA4   BIT(19)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_18              BIT(18)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_18_Gpio_Out_R18 (0 << 18)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_18_I2C_SCL4   BIT(18)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_17              BIT(17)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_17_Gpio_Out_R17 (0 << 17)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_17_I2C_SDA3   BIT(17)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_16              BIT(16)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_16_Gpio_Out_R16 (0 << 16)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_16_I2C_SCL3   BIT(16)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_15              BIT(15)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_15_Gpio_Out_R15 (0 << 15)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_15_I2C_SDA2   BIT(15)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_14              BIT(14)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_14_Gpio_Out_R14 (0 << 14)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_14_I2C_SCL2   BIT(14)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_13              BIT(13)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_13_Gpio_Out_R13 (0 << 13)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_13_SPIX_SEL3  BIT(13)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_12              BIT(12)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_12_Gpio_Out_R12 (0 << 12)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_12_SPIX_SEL2  BIT(12)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_11              BIT(11)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_11_Gpio_Out_R11 (0 << 11)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_11_SPIX_SEL1  BIT(11)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_10              BIT(10)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_9               BIT(9)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_8               BIT(8)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_8_UART_TXD    BIT(8)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_7               BIT(7)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_6               BIT(6)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_6_Gpio_Out_R6 (0 << 6)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_6_I2C_SDA7    BIT(6)
#define   NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_5               BIT(5)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_5_Gpio_Out_R5 (0 << 5)
#define     NFP3800_ARM_GCSR_GPIO_Sel_Gpio_Sel_5_I2C_SCL7    BIT(5)
/*----------------------------------------------------------------
  Register: SIG_MUX_Sel - I/O Signal Mux Select register
    Bits: 31    Sig_Sel_31(rw) - Select GPIO[31] signal onto SMBALRT pin.
    Bits: 30:29 Sig_Sel_29(rw) - Select GPIO[30] or SDA6 signals onto SMBDAT pin.
    Bits: 28:27 Sig_Sel_27(rw) - Select GPIO[29] or SCL6 signals onto SMBCLK pin.
    Bits: 26    Sig_Sel_26(rw) - Select GPIO[28] signal onto UART0_TX pin.
    Bits: 25    Sig_Sel_25(rw) - Select GPIO[27] signal onto UART0_RX pin.
    Bits: 24    Sig_Sel_24(rw) - Select GPIO[26] signal onto SPIX_D3 pin.
    Bits: 23:22 Sig_Sel_22(rw) - Select GPIO[25] or MAC_ACTIVITY signal onto SPIX_D2 pin.
    Bits: 21    Sig_Sel_21(rw) - Select GPIO[23] signal onto SPI3_MOSI pin.
    Bits: 20    Sig_Sel_20(rw) - Select GPIO[22] signal onto SPI3_SCK pin.
    Bits: 19    Sig_Sel_19(rw) - Select GPIO[21] or signal onto SPI2_MOSI pin.
    Bits: 18    Sig_Sel_18(rw) - Select GPIO[20] or signal onto SPI2_SCK pin.
    Bits: 17    Sig_Sel_17(rw) - Select GPIO[19] signal onto SPI0_MISO pin.
    Bits: 16    Sig_Sel_16(rw) - Select GPIO[18] signal onto SPI0_SEL pin.
    Bits: 15    Sig_Sel_15(rw) - Select GPIO[17] signal onto SPI0_MOSI pin.
    Bits: 14    Sig_Sel_14(rw) - Select GPIO[16] signal onto SPI0_SCK pin.
*/
#define NFP3800_ARM_GCSR_SIG_MUX_Sel                         0x0000010c
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_31            BIT(31)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_31_SMBALRT  (0 << 31)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_31_Gpio_Out_R31 BIT(31)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_29(_x)        (((_x) & 0x3) << 29)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_29_of(_x)     (((_x) >> 29) & 0x3)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_29_SMBDAT   (0)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_29_SDA6     (1)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_29_Gpio_Out_R30 (2)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_27(_x)        (((_x) & 0x3) << 27)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_27_of(_x)     (((_x) >> 27) & 0x3)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_27_SMBCLK   (0)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_27_SCL6     (1)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_27_Gpio_Out_R29 (2)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_26            BIT(26)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_26_UART0_TX (0 << 26)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_26_Gpio_Out_R28 BIT(26)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_25            BIT(25)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_25_UART0_RX (0 << 25)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_25_Gpio_Out_R27 BIT(25)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_24            BIT(24)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_24_SPIX_D3  (0 << 24)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_24_Gpio_Out_R26 BIT(24)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_22(_x)        (((_x) & 0x3) << 22)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_22_of(_x)     (((_x) >> 22) & 0x3)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_22_SPIX_D2  (0)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_22_MAC_ACTIVITY (1)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_22_Gpio_Out_R25 (2)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_21            BIT(21)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_21_SPI3_MOSI(MDIO1) (0 << 21)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_21_Gpio_Out_R23 BIT(21)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_20            BIT(20)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_20_SPI3_SCK(MDC1) (0 << 20)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_20_Gpio_Out_R22 BIT(20)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_19            BIT(19)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_19_SPI2_MOSI(MDIO0) (0 << 19)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_19_Gpio_Out_R21 BIT(19)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_18            BIT(18)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_18_SPI2_SCK(MDC0) (0 << 18)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_18_Gpio_Out_R20 BIT(18)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_17            BIT(17)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_17_SPI0_MISO (0 << 17)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_17_Gpio_Out_R19 BIT(17)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_16            BIT(16)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_16_SPI0_SEL (0 << 16)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_16_Gpio_Out_R18 BIT(16)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_15            BIT(15)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_15_SPI0_MOSI (0 << 15)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_15_Gpio_Out_R17 BIT(15)
#define   NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_14            BIT(14)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_14_SPI0_SCK (0 << 14)
#define     NFP3800_ARM_GCSR_SIG_MUX_Sel_Sig_Sel_14_Gpio_Out_R16 BIT(14)
/*----------------------------------------------------------------
  Register: SoftModel0 - Soft Model 0 register
  Register: SoftModel1 - Soft Model 1 register
  Register: SoftModel2 - Soft Model 2 register
  Register: SoftModel3 - Soft Model 3 register
  Register: SoftModel4 - Soft Model 4 register
  Register: SoftModel5 - Soft Model 5 register
  Register: SoftModel6 - Soft Model 6 register
  Register: SoftModel7 - Soft Model 7 register
    Bits: 31:0  SoftModel(rw) - Soft Model value is loaded by the configurater and/or software. Interpretation of the bits is up to software.
*/
#define NFP3800_ARM_GCSR_SoftModel0                          0x00000144
#define NFP3800_ARM_GCSR_SoftModel1                          0x00000148
#define NFP3800_ARM_GCSR_SoftModel2                          0x0000014c
#define NFP3800_ARM_GCSR_SoftModel3                          0x00000150
#define NFP3800_ARM_GCSR_SoftModel4                          0x000001b0
#define NFP3800_ARM_GCSR_SoftModel5                          0x000001b4
#define NFP3800_ARM_GCSR_SoftModel6                          0x000001b8
#define NFP3800_ARM_GCSR_SoftModel7                          0x000001bc
#define   NFP3800_ARM_GCSR_SoftModel_SoftModel(_x)           (((_x) & 0xffffffff) << 0)
#define   NFP3800_ARM_GCSR_SoftModel_SoftModel_of(_x)        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SoftModelLock - Soft Model Lock register
    Bits: 31:0  SoftModelLock(rw) - This lock controls the writability of the eight SoftModel[7:0] registers. On reads, the MSB is zero if the registers are UNLOCKED, set otherwise. If the lock is UNLOCKED, any write to this register will set the lock. Once locked, this register must be written consecutivly with the sequence 0x4f70656e, 0x2053657a, 0x204d6521 to unlock it.
*/
#define NFP3800_ARM_GCSR_SoftModelLock                       0x00000154
#define   NFP3800_ARM_GCSR_SoftModelLock_SoftModelLock(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP3800_ARM_GCSR_SoftModelLock_SoftModelLock_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CfgConfigSts - Configurater Configuration Status register
    Bits:  8    CfgCompleted(ro) - This bit is set to indicate the configurater completed the configuration process.
    Bits:  7:5  Cfg2Err(ro) - This field indicates the error type for the second image copy. Only valid if CfgCompleted is set.
    Bits:  4:2  Cfg1Err(ro) - This field indicates the error type for the first image copy. Only valid if CfgImage is set.
    Bits:  1    CfgSuccess(ro) - This bit is set if the configurater successfully completed the configuration process.
    Bits:  0    CfgImage(ro) - Zero indicates the configurater suceeded using the first image. One indicates the first image failed and the second image was used.
*/
#define NFP3800_ARM_GCSR_CfgConfigSts                        0x00000158
#define   NFP3800_ARM_GCSR_CfgConfigSts_CfgCompleted         BIT(8)
#define   NFP3800_ARM_GCSR_CfgConfigSts_Cfg2Err(_x)          (((_x) & 0x7) << 5)
#define   NFP3800_ARM_GCSR_CfgConfigSts_Cfg2Err_of(_x)       (((_x) >> 5) & 0x7)
#define     NFP3800_ARM_GCSR_CfgConfigSts_Cfg2Err_NOERR      (0)
#define     NFP3800_ARM_GCSR_CfgConfigSts_Cfg2Err_BADHDR     (1)
#define     NFP3800_ARM_GCSR_CfgConfigSts_Cfg2Err_BADOPC     (2)
#define     NFP3800_ARM_GCSR_CfgConfigSts_Cfg2Err_BADCHK     (3)
#define     NFP3800_ARM_GCSR_CfgConfigSts_Cfg2Err_BADCS      (4)
#define     NFP3800_ARM_GCSR_CfgConfigSts_Cfg2Err_BADSRST    (7)
#define   NFP3800_ARM_GCSR_CfgConfigSts_Cfg1Err(_x)          (((_x) & 0x7) << 2)
#define   NFP3800_ARM_GCSR_CfgConfigSts_Cfg1Err_of(_x)       (((_x) >> 2) & 0x7)
#define     NFP3800_ARM_GCSR_CfgConfigSts_Cfg1Err_NOERR      (0)
#define     NFP3800_ARM_GCSR_CfgConfigSts_Cfg1Err_BADHDR     (1)
#define     NFP3800_ARM_GCSR_CfgConfigSts_Cfg1Err_BADOPC     (2)
#define     NFP3800_ARM_GCSR_CfgConfigSts_Cfg1Err_BADCHK     (3)
#define     NFP3800_ARM_GCSR_CfgConfigSts_Cfg1Err_BADCS      (4)
#define     NFP3800_ARM_GCSR_CfgConfigSts_Cfg1Err_BADSRST    (7)
#define   NFP3800_ARM_GCSR_CfgConfigSts_CfgSuccess           BIT(1)
#define   NFP3800_ARM_GCSR_CfgConfigSts_CfgImage             BIT(0)
/*----------------------------------------------------------------
  Register: JumpToSelf[16] - Jump To Self opcode register
    Bits: 31:0  JumpToSelf(ro) - This register provides a jump to self instruction to the arm core when the configurater process fails and the arm is spun up for the arm JTAG to come in and program the configurater flash.
*/
#define NFP3800_ARM_GCSR_JumpToSelf(_x)                      (0x00000400 + (0x4 * ((_x) & 0xf)))
#define   NFP3800_ARM_GCSR_JumpToSelf_JumpToSelf(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP3800_ARM_GCSR_JumpToSelf_JumpToSelf_of(_x)      (((_x) >> 0) & 0xffffffff)

#endif /* NFP3800_LONGNAMES */


#endif /* NFP3800_ARM_H */
