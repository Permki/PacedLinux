/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_ARM_H
#define NFP6000_ARM_H

/*================================================================*/
/* TGT 10                                                         */

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + NFP_ARM_SMEM */
#define NFP_ARM_SMEM                                         0x000000
/* CPP_BASE + NFP_ARM_QCTLR[256] + (nfp_qctlr.h) */
#define NFP_ARM_Q(_x)                                        (0x100000 + ((_x) & 0xff) * 0x10)
/* CPP_BASE + NFP_ARM_IM + (nfp_im.h) */
#define NFP_ARM_IM                                           0x200000
/* CPP_BASE + NFP_ARM_EM + (nfp_em.h) */
#define NFP_ARM_EM                                           0x300000
/* CPP_BASE + NFP_ARM_GCSR + NFP_ARM_GCSR_* */
#define NFP_ARM_GCSR                                         0x400000
/* CPP_BASE + NFP_ARM_GPIO + (nfp_gpio.h) */
#define NFP_ARM_GPIO                                         (0x403000)
/* CPP_BASE + NFP_ARM_TIMER + (nfp_timer.h) */
#define NFP_ARM_TIMER                                        (0x403400)
/* CPP_BASE + NFP_ARM_UART + (nfp_uart.h) */
#define NFP_ARM_UART                                         (0x403800)
/* CPP_BASE + NFP_ARM_SPI + (nfp_spi.h) */
#define NFP_ARM_SPI                                          (0x403c00)
/* CPP_BASE + NFP_ARM_MPCORE + (...) */
#define NFP_ARM_MPCORE                                       0x800000
/* CPP_BASE + NFP_ARM_PL310 + (...) */
#define NFP_ARM_PL310                                        0xa00000
/* CPP_BASE + NFP_ARM_SPI_VIRTUAL(x) */
#define NFP_ARM_SPI_VIRTUAL(_x)                              (0x10000000 | (((_x) & 0xf) << 22))

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP_XPB_ISLAND(island))          */

#define ARMX_BASE                                            (0xa0000)

/* XPB_BASE + NFP_ARMX_IM + (nfp_im.h) */
#define NFP_ARMX_IM                                     (ARMX_BASE + 0x00000)
/* XPB_BASE + NFP_ARMX_QCTLR_ECC + (nfp_ecc.h) */
#define NFP_ARMX_QCTLR_ECC                                   (ARMX_BASE + 0x10000)
/* XPB_BASE + NFP_ARMX_SMEM_ECC + (nfp_ecc.h) */
#define NFP_ARMX_SMEM_ECC                                    (ARMX_BASE + 0x20000)
/* XPB_BASE + NFP_ARMX_L2DATA_ECC + (nfp_ecc.h) */
#define NFP_ARMX_L2DATA_ECC                                  (ARMX_BASE + 0x30000)
/* XPB_BASE + NFP_ARMX_L2TAG_ECC + (nfp_ecc.h) */
#define NFP_ARMX_L2TAG_ECC                                   (ARMX_BASE + 0x40000)
/* XPB_BASE + NFP_ARMX_L1DATA_ECC(_x) + (nfp_ecc.h) */
#define NFP_ARMX_L1DATA_ECC(_x)                              (ARMX_BASE + 0x50000 + ((_x) & 0x3) * 0x10000)
/* XPB_BASE + NFP_ARMX_L1INST_ECC + (nfp_ecc.h) */
#define NFP_ARMX_L1INST_ECC                                  (ARMX_BASE + 0x90000)
/* XPB_BASE + NFP_ARMX_L1DATATAG_ECC + (nfp_ecc.h) */
#define NFP_ARMX_L1DATATAG_ECC                               (ARMX_BASE + 0xa0000)
/* XPB_BASE + NFP_ARMX_L1INSTTAG_ECC + (nfp_ecc.h) */
#define NFP_ARMX_L1INSTTAG_ECC                               (ARMX_BASE + 0xb0000)
/* XPB_BASE + NFP_ARMX_L1DIRTYTAG_ECC + (nfp_ecc.h) */
#define NFP_ARMX_L1DIRTYTAG_ECC                              (ARMX_BASE + 0xc0000)
/* XPB_BASE + NFP_ARMX_PLU + (nfp_plu.h) */
#define NFP_ARMX_PLU                                         (ARMX_BASE + 0x400000)

/*================================================================*/


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: BulkBarCfg[8] - Bulk BAR Configuration Register
  Register: BulkBar0Boot - Boot value for the Bulk Bar 0 (ARM soft reset loads this value into Bulk Bar 0)
    Bits: 31    BarType(rw) - BAR type
    Bits: 30:27 Target(rw) - CPP command target ID
    Bits: 26:25 Token(rw) - CPP command token
    Bits: 24    Length(rw) - CPP command length increment
    Bits: 10:0  Address(rw) - Bits [39:29] of CPP command address. Bits [28:0] of CPP command are taken from the AXI command address.
*/
#define NFP_ARM_GCSR_BULK_BAR(_x)                            (0x00000000 + (0x4 * ((_x) & 0x7)))
#define NFP_ARM_GCSR_BULKBAR0BOOT                            0x0000015c
#define   NFP_ARM_GCSR_BULKBARCFG_TYPE                       BIT(31)
#define     NFP_ARM_GCSR_BULKBARCFG_TYPE_BULK                (0 << 31)
#define     NFP_ARM_GCSR_BULKBARCFG_TYPE_EXPA                BIT(31)
#define   NFP_ARM_GCSR_BULKBARCFG_TGT(_x)                    (((_x) & 0xf) << 27)
#define   NFP_ARM_GCSR_BULKBARCFG_TGT_of(_x)                 (((_x) >> 27) & 0xf)
#define   NFP_ARM_GCSR_BULKBARCFG_TOK(_x)                    (((_x) & 0x3) << 25)
#define   NFP_ARM_GCSR_BULKBARCFG_TOK_of(_x)                 (((_x) >> 25) & 0x3)
#define   NFP_ARM_GCSR_BULKBARCFG_LEN                        BIT(24)
#define     NFP_ARM_GCSR_BULKBARCFG_LEN_32BIT                (0 << 24)
#define     NFP_ARM_GCSR_BULKBARCFG_LEN_64BIT                BIT(24)
#define   NFP_ARM_GCSR_BULKBARCFG_ADDR(_x)                   (((_x) & 0x7ff) << 0)
#define   NFP_ARM_GCSR_BULKBARCFG_ADDR_of(_x)                (((_x) >> 0) & 0x7ff)
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
#define NFP_ARM_GCSR_EXPA_BAR(_x)                            (0x00000020 + (0x4 * ((_x) & 0xf)))
#define NFP_ARM_GCSR_EXPANSIONBAR0BOOT                       0x00000160
#define   NFP_ARM_GCSR_EXPANSIONBARCFG_TYPE                  BIT(31)
#define     NFP_ARM_GCSR_EXPANSIONBARCFG_TYPE_EXPA           (0 << 31)
#define     NFP_ARM_GCSR_EXPANSIONBARCFG_TYPE_EXPL           BIT(31)
#define   NFP_ARM_GCSR_EXPANSIONBARCFG_TGT(_x)               (((_x) & 0xf) << 27)
#define   NFP_ARM_GCSR_EXPANSIONBARCFG_TGT_of(_x)            (((_x) >> 27) & 0xf)
#define   NFP_ARM_GCSR_EXPANSIONBARCFG_TOK(_x)               (((_x) & 0x3) << 25)
#define   NFP_ARM_GCSR_EXPANSIONBARCFG_TOK_of(_x)            (((_x) >> 25) & 0x3)
#define   NFP_ARM_GCSR_EXPANSIONBARCFG_LEN                   BIT(24)
#define     NFP_ARM_GCSR_EXPANSIONBARCFG_LEN_32BIT           (0 << 24)
#define     NFP_ARM_GCSR_EXPANSIONBARCFG_LEN_64BIT           BIT(24)
#define   NFP_ARM_GCSR_EXPANSIONBARCFG_ACT(_x)               (((_x) & 0x1f) << 19)
#define   NFP_ARM_GCSR_EXPANSIONBARCFG_ACT_of(_x)            (((_x) >> 19) & 0x1f)
#define     NFP_ARM_GCSR_EXPANSIONBARCFG_ACT_DERIVED         (0)
#define   NFP_ARM_GCSR_EXPANSIONBARCFG_ADDR(_x)              (((_x) & 0x7fff) << 0)
#define   NFP_ARM_GCSR_EXPANSIONBARCFG_ADDR_of(_x)           (((_x) >> 0) & 0x7fff)
/*----------------------------------------------------------------
  Register: ExplicitBarCfg0[8] - Explicit BAR Configuration Register 0_Reg
    Bits: 17:0  Address(rw) - Bits [39:22] of CPP command address. Bits [21:0] of CPP command are taken from the AXI command address.
*/
#define NFP_ARM_GCSR_EXPL0_BAR(_x)                           (0x00000060 + (0x4 * ((_x) & 0x7)))
#define   NFP_ARM_GCSR_EXPL0_BAR_ADDR(_x)                    (((_x) & 0x3ffff) << 0)
#define   NFP_ARM_GCSR_EXPL0_BAR_ADDR_of(_x)                 (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: ExplicitBarCfg1[8] - Explicit BAR Configuration Register 1_Reg
    Bits: 31    Posted(rw) - CPP command posted
    Bits: 30:24 SignalRef(rw) - CPP command signal ref
    Bits: 23:16 DataMaster(rw) - CPP command data master
    Bits: 13:0  DataRef(rw) - CPP command data ref
*/
#define NFP_ARM_GCSR_EXPL1_BAR(_x)                           (0x00000080 + (0x4 * ((_x) & 0x7)))
#define   NFP_ARM_GCSR_EXPL1_BAR_POSTED                      BIT(31)
#define   NFP_ARM_GCSR_EXPL1_BAR_SIGNAL_REF(_x)              (((_x) & 0x7f) << 24)
#define   NFP_ARM_GCSR_EXPL1_BAR_SIGNAL_REF_of(_x)           (((_x) >> 24) & 0x7f)
#define   NFP_ARM_GCSR_EXPL1_BAR_DATA_MASTER(_x)             (((_x) & 0xff) << 16)
#define   NFP_ARM_GCSR_EXPL1_BAR_DATA_MASTER_of(_x)          (((_x) >> 16) & 0xff)
#define   NFP_ARM_GCSR_EXPL1_BAR_DATA_REF(_x)                (((_x) & 0x3fff) << 0)
#define   NFP_ARM_GCSR_EXPL1_BAR_DATA_REF_of(_x)             (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: ExplicitBarCfg2[8] - Explicit BAR Configuration Register 2_Reg
    Bits: 31:28 Target(rw) - CPP command target ID
    Bits: 27:23 Action(rw) - CPP Command Action Encoding
    Bits: 22:18 Length(rw) - CPP command length
    Bits: 17:10 ByteMask(rw) - CPP command byte mask
    Bits:  9:8  Token(rw) - CPP command token
    Bits:  7:0  SignalMaster(rw) - CPP command signal master
*/
#define NFP_ARM_GCSR_EXPL2_BAR(_x)                           (0x000000a0 + (0x4 * ((_x) & 0x7)))
#define   NFP_ARM_GCSR_EXPL2_BAR_TGT(_x)                     (((_x) & 0xf) << 28)
#define   NFP_ARM_GCSR_EXPL2_BAR_TGT_of(_x)                  (((_x) >> 28) & 0xf)
#define   NFP_ARM_GCSR_EXPL2_BAR_ACT(_x)                     (((_x) & 0x1f) << 23)
#define   NFP_ARM_GCSR_EXPL2_BAR_ACT_of(_x)                  (((_x) >> 23) & 0x1f)
#define   NFP_ARM_GCSR_EXPL2_BAR_LEN(_x)                     (((_x) & 0x1f) << 18)
#define   NFP_ARM_GCSR_EXPL2_BAR_LEN_of(_x)                  (((_x) >> 18) & 0x1f)
#define   NFP_ARM_GCSR_EXPL2_BAR_BYTE_MASK(_x)               (((_x) & 0xff) << 10)
#define   NFP_ARM_GCSR_EXPL2_BAR_BYTE_MASK_of(_x)            (((_x) >> 10) & 0xff)
#define   NFP_ARM_GCSR_EXPL2_BAR_TOK(_x)                     (((_x) & 0x3) << 8)
#define   NFP_ARM_GCSR_EXPL2_BAR_TOK_of(_x)                  (((_x) >> 8) & 0x3)
#define   NFP_ARM_GCSR_EXPL2_BAR_SIGNAL_MASTER(_x)           (((_x) & 0xff) << 0)
#define   NFP_ARM_GCSR_EXPL2_BAR_SIGNAL_MASTER_of(_x)        (((_x) >> 0) & 0xff)
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
#define NFP_ARM_GCSR_EXPL_POST(_x)                           (0x000000c0 + (0x4 * ((_x) & 0x7)))
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B(_x)                   (((_x) & 0x7f) << 25)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_of(_x)                (((_x) >> 25) & 0x7f)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_BUS                   BIT(24)
#define     NFP_ARM_GCSR_EXPL_POST_SIG_B_BUS_PULL            (0 << 24)
#define     NFP_ARM_GCSR_EXPL_POST_SIG_B_BUS_PUSH            BIT(24)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A(_x)                   (((_x) & 0x7f) << 17)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_of(_x)                (((_x) >> 17) & 0x7f)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_BUS                   BIT(16)
#define     NFP_ARM_GCSR_EXPL_POST_SIG_A_BUS_PULL            (0 << 16)
#define     NFP_ARM_GCSR_EXPL_POST_SIG_A_BUS_PUSH            BIT(16)
#define   NFP_ARM_GCSR_EXPL_POST_NOSPLIT_RCVD_B              BIT(11)
#define   NFP_ARM_GCSR_EXPL_POST_NOSPLIT_ENABLE_B            BIT(10)
#define   NFP_ARM_GCSR_EXPL_POST_NOSPLIT_RCVD_A              BIT(9)
#define   NFP_ARM_GCSR_EXPL_POST_NOSPLIT_ENABLE_A            BIT(8)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_RCVD                  BIT(7)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_B_VALID                 BIT(6)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_RCVD                  BIT(5)
#define   NFP_ARM_GCSR_EXPL_POST_SIG_A_VALID                 BIT(4)
#define   NFP_ARM_GCSR_EXPL_POST_CMD_COMPLETE                BIT(0)
/*----------------------------------------------------------------
  Register: MPCoreBaseAddress - MPCore Base Address
    Bits: 31:13 BaseAddress(rw) - Base address for CSRs in MPCore
*/
#define NFP_ARM_GCSR_MPCORE_BASE                             0x000000e0
#define   NFP_ARM_GCSR_MPCORE_BASE_ADDR(_x)                  (((_x) & 0x7ffff) << 13)
#define   NFP_ARM_GCSR_MPCORE_BASE_ADDR_of(_x)               (((_x) >> 13) & 0x7ffff)
/*----------------------------------------------------------------
  Register: PL310BaseAddress - PL310 Base Address
    Bits: 31:12 BaseAddress(rw) - Base address for CSRs in PL310 L2 cache
*/
#define NFP_ARM_GCSR_PL310_BASE                              0x000000e4
#define   NFP_ARM_GCSR_PL310_BASE_ADDR(_x)                   (((_x) & 0xfffff) << 12)
#define   NFP_ARM_GCSR_PL310_BASE_ADDR_of(_x)                (((_x) >> 12) & 0xfffff)
/*----------------------------------------------------------------
  Register: MPCoreCfg - MPCore Configuration
  Register: MPCoreCfgBoot - Boot value for the ARM MPCoreCfg (ARM soft reset loads this value into MPCoreCfg)
    Bits: 25    PL310_Cfg_Bigend(rw) - L2 Cache big-endian.
    Bits: 24:19 PL310_CacheID(rw) - L2 Cache ID.
    Bits: 18:15 ARM_Cfg_NMI(rw) - NMI configuration.
    Bits: 13:12 MP0ConfigEndian(rw) - MP0 endianess
    Bits:  8    MP0RstVecLoc(ro) - MP0 Reset Vector location
    Bits:  7:4  MP0OutClkEnable(rw) - MP Output clock enable
    Bits:  3:0  ARMClusterId(rw) - Base address for CSRs in PL310 L2 Cache
*/
#define NFP_ARM_GCSR_MP0_CFG                                 0x000000e8
#define NFP_ARM_GCSR_ARMCFG0BOOT                             0x00000164
#define   NFP_ARM_GCSR_MPCORECFG_PL310CFGBIGEND              BIT(25)
#define   NFP_ARM_GCSR_MPCORECFG_PL310CACHEID(_x)            (((_x) & 0x3f) << 19)
#define   NFP_ARM_GCSR_MPCORECFG_PL310CACHEID_of(_x)         (((_x) >> 19) & 0x3f)
#define   NFP_ARM_GCSR_MPCORECFG_ARMCFGNMI(_x)               (((_x) & 0xf) << 15)
#define   NFP_ARM_GCSR_MPCORECFG_ARMCFGNMI_of(_x)            (((_x) >> 15) & 0xf)
#define   NFP_ARM_GCSR_MPCORECFG_ENDIAN(_x)                  (((_x) & 0x3) << 12)
#define   NFP_ARM_GCSR_MPCORECFG_ENDIAN_of(_x)               (((_x) >> 12) & 0x3)
#define     NFP_ARM_GCSR_MPCORECFG_ENDIAN_LITTLE             (0)
#define     NFP_ARM_GCSR_MPCORECFG_ENDIAN_BIG                (1)
#define   NFP_ARM_GCSR_MPCORECFG_RESET_VECTOR                BIT(8)
#define     NFP_ARM_GCSR_MPCORECFG_RESET_VECTOR_LO           (0 << 8)
#define     NFP_ARM_GCSR_MPCORECFG_RESET_VECTOR_HI           BIT(8)
#define   NFP_ARM_GCSR_MPCORECFG_OUTCLK_EN(_x)               (((_x) & 0xf) << 4)
#define   NFP_ARM_GCSR_MPCORECFG_OUTCLK_EN_of(_x)            (((_x) >> 4) & 0xf)
#define   NFP_ARM_GCSR_MPCORECFG_ARMID(_x)                   (((_x) & 0xf) << 0)
#define   NFP_ARM_GCSR_MPCORECFG_ARMID_of(_x)                (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: ArmDft - ARM Design for Test register
    Bits: 20    DbgReq(rw) - MPCore0 Debug Request (Write 1 to initiate debug request, cleared by HW on DBGACK from ARM)
    Bits: 19    DbgEn(rw) - MPCore0 Debug Enable
    Bits: 18    WfeEvtTrg(rw) - CSR WFE event trigger
    Bits: 17    EtmWfiReady(rw) - ETM WFU Ready handshake response (active low)
    Bits: 16    EtmPowerOn(rw) - ETM interface power on (to MPCore)
    Bits: 11:8  BistFail(ro) - BIST Fail status from DAL
    Bits:  7:4  BistDone(ro) - BIST Done status from DAL
    Bits:  2:0  BistRun(rw) - BIST Run
*/
#define NFP_ARM_GCSR_DFT                                     0x00000100
#define   NFP_ARM_GCSR_DFT_DBG_REQ                           BIT(20)
#define   NFP_ARM_GCSR_DFT_DBG_EN                            BIT(19)
#define   NFP_ARM_GCSR_DFT_WFE_EVT_TRG                       BIT(18)
#define   NFP_ARM_GCSR_DFT_ETM_WFI_RDY                       BIT(17)
#define   NFP_ARM_GCSR_DFT_ETM_PWR_ON                        BIT(16)
#define   NFP_ARM_GCSR_DFT_BIST_FAIL(_x)                     (((_x) & 0xf) << 8)
#define   NFP_ARM_GCSR_DFT_BIST_FAIL_of(_x)                  (((_x) >> 8) & 0xf)
#define   NFP_ARM_GCSR_DFT_BIST_DONE(_x)                     (((_x) & 0xf) << 4)
#define   NFP_ARM_GCSR_DFT_BIST_DONE_of(_x)                  (((_x) >> 4) & 0xf)
#define   NFP_ARM_GCSR_DFT_BIST_RUN(_x)                      (((_x) & 0x7) << 0)
#define   NFP_ARM_GCSR_DFT_BIST_RUN_of(_x)                   (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: ArmStatus - ARM Status register
    Bits:  7    commrx_cpu0(ro) - MPCore0 commrx_cpu0.
    Bits:  6    commtx_cpu0(ro) - MPCore0 commtx_cpu0.
    Bits:  5    smpnamp(ro) - MPCore0 smpnamp.
    Bits:  4:3  pwrctl0(ro) - MPCore0 pwrctl.
    Bits:  2    wfe(ro) - MPCore0 wfe.
    Bits:  1    wfi(ro) - MPCore0 wfi.
    Bits:  0    l2_idle(ro) - MPCore0 l2_idle.
*/
#define NFP_ARM_GCSR_ARMSTATUS                               0x00000104
#define   NFP_ARM_GCSR_ARMSTATUS_COMMRX_CPU0                 BIT(7)
#define   NFP_ARM_GCSR_ARMSTATUS_COMMTX_CPU0                 BIT(6)
#define   NFP_ARM_GCSR_ARMSTATUS_SMPNAMP                     BIT(5)
#define   NFP_ARM_GCSR_ARMSTATUS_PWRCTL0(_x)                 (((_x) & 0x3) << 3)
#define   NFP_ARM_GCSR_ARMSTATUS_PWRCTL0_of(_x)              (((_x) >> 3) & 0x3)
#define   NFP_ARM_GCSR_ARMSTATUS_WFE                         BIT(2)
#define   NFP_ARM_GCSR_ARMSTATUS_WFI                         BIT(1)
#define   NFP_ARM_GCSR_ARMSTATUS_L2_IDLE                     BIT(0)
/*----------------------------------------------------------------
  Register: HardModel - Hard Model register
    Bits: 31:0  HardModel(ro) - Hard Model value is derived from the configuration encryption key information. Interpretation of the bits is up to software.
*/
#define NFP_ARM_GCSR_HARDMODEL                               0x00000140
#define   NFP_ARM_GCSR_HARDMODEL_HARDMODEL(_x)               (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_HARDMODEL_HARDMODEL_of(_x)            (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SoftModel0 - Soft Model 0 register
    Bits: 31:0  SoftModel0(rw) - Soft Model value is loaded by the configurater and/or software. Interpretation of the bits is up to software.
*/
#define NFP_ARM_GCSR_SOFTMODEL0                              0x00000144
#define   NFP_ARM_GCSR_SOFTMODEL0_SOFTMODEL0(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SOFTMODEL0_SOFTMODEL0_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SoftModel1 - Soft Model 1 register
    Bits: 31:0  SoftModel1(rw) - Soft Model value is loaded by the configurater and/or software. Interpretation of the bits is up to software.
*/
#define NFP_ARM_GCSR_SOFTMODEL1                              0x00000148
#define   NFP_ARM_GCSR_SOFTMODEL1_SOFTMODEL1(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SOFTMODEL1_SOFTMODEL1_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SoftModel2 - Soft Model 2 register
    Bits: 31:0  SoftModel2(rw) - Soft Model value is loaded by the configurater and/or software. Interpretation of the bits is up to software.
*/
#define NFP_ARM_GCSR_SOFTMODEL2                              0x0000014c
#define   NFP_ARM_GCSR_SOFTMODEL2_SOFTMODEL2(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SOFTMODEL2_SOFTMODEL2_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SoftModel3 - Soft Model 3 register
    Bits: 31:0  SoftModel3(rw) - Soft Model value is loaded by the configurater and/or software. Interpretation of the bits is up to software.
*/
#define NFP_ARM_GCSR_SOFTMODEL3                              0x00000150
#define   NFP_ARM_GCSR_SOFTMODEL3_SOFTMODEL3(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SOFTMODEL3_SOFTMODEL3_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CfgConfigSts - Configurater Configuration Status register
    Bits:  8    CfgCompleted(ro) - This bit is set to indicate the configurater completed the configuration process.
    Bits:  7:5  Cfg2Err(ro) - This field indicates the error type for the second image copy. Only valid if CfgImage is set.
    Bits:  4:2  Cfg1Err(ro) - This field indicates the error type for the first image copy.
    Bits:  1    CfgSuccess(ro) - This bit is set if the configurater successfully completed the configuration process.
    Bits:  0    CfgImage(ro) - Zero indicates the configurater suceeded using the first image. One indicates the first image failed and the second image was used.
*/
#define NFP_ARM_GCSR_CFGCONFIGSTS                            0x00000158
#define   NFP_ARM_GCSR_CFGCONFIGSTS_CFGCOMPLETED             BIT(8)
#define   NFP_ARM_GCSR_CFGCONFIGSTS_CFG2ERR(_x)              (((_x) & 0x7) << 5)
#define   NFP_ARM_GCSR_CFGCONFIGSTS_CFG2ERR_of(_x)           (((_x) >> 5) & 0x7)
#define     NFP_ARM_GCSR_CFGCONFIGSTS_CFG2ERR_NOERR          (0)
#define     NFP_ARM_GCSR_CFGCONFIGSTS_CFG2ERR_BADHDR         (1)
#define     NFP_ARM_GCSR_CFGCONFIGSTS_CFG2ERR_BADOPC         (2)
#define     NFP_ARM_GCSR_CFGCONFIGSTS_CFG2ERR_BADCHK         (3)
#define     NFP_ARM_GCSR_CFGCONFIGSTS_CFG2ERR_BADCS          (4)
#define   NFP_ARM_GCSR_CFGCONFIGSTS_CFG1ERR(_x)              (((_x) & 0x7) << 2)
#define   NFP_ARM_GCSR_CFGCONFIGSTS_CFG1ERR_of(_x)           (((_x) >> 2) & 0x7)
#define     NFP_ARM_GCSR_CFGCONFIGSTS_CFG1ERR_NOERR          (0)
#define     NFP_ARM_GCSR_CFGCONFIGSTS_CFG1ERR_BADHDR         (1)
#define     NFP_ARM_GCSR_CFGCONFIGSTS_CFG1ERR_BADOPC         (2)
#define     NFP_ARM_GCSR_CFGCONFIGSTS_CFG1ERR_BADCHK         (3)
#define     NFP_ARM_GCSR_CFGCONFIGSTS_CFG1ERR_BADCS          (4)
#define   NFP_ARM_GCSR_CFGCONFIGSTS_CFGSUCCESS               BIT(1)
#define   NFP_ARM_GCSR_CFGCONFIGSTS_CFGIMAGE                 BIT(0)
/*----------------------------------------------------------------
  Register: SidedoorCECore - ARM sidedoor ram ce for core rams
    Bits: 31    SidedoorCoreEnable(rw) - This bit is set to enable sidedoor access into the arm core memories.
    Bits: 17:0  SidedoorCE(rw) - This field controls the ram CE pins for the arm core memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SIDEDOORCECORE                          0x00000180
#define   NFP_ARM_GCSR_SIDEDOORCECORE_SIDEDOORCOREENABLE     BIT(31)
#define   NFP_ARM_GCSR_SIDEDOORCECORE_SIDEDOORCE(_x)         (((_x) & 0x3ffff) << 0)
#define   NFP_ARM_GCSR_SIDEDOORCECORE_SIDEDOORCE_of(_x)      (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: SidedoorCEPl310 - ARM sidedoor ram ce for L2 cache rams
    Bits: 31    SidedoorPl310Enable(rw) - This bit is set to enable sidedoor access into the arm L2 cache memories.
    Bits: 17:0  SidedoorCE(rw) - This field controls the ram CE pins for the arm L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SIDEDOORCEPL310                         0x00000184
#define   NFP_ARM_GCSR_SIDEDOORCEPL310_SIDEDOORPL310ENABLE   BIT(31)
#define   NFP_ARM_GCSR_SIDEDOORCEPL310_SIDEDOORCE(_x)        (((_x) & 0x3ffff) << 0)
#define   NFP_ARM_GCSR_SIDEDOORCEPL310_SIDEDOORCE_of(_x)     (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: SidedoorAddress - ARM sidedoor ram address
    Bits: 19:0  SidedoorAddr(rw) - This field controls the ram ADDR pins for the arm core and L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SIDEDOORADDRESS                         0x00000188
#define   NFP_ARM_GCSR_SIDEDOORADDRESS_SIDEDOORADDR(_x)      (((_x) & 0xfffff) << 0)
#define   NFP_ARM_GCSR_SIDEDOORADDRESS_SIDEDOORADDR_of(_x)   (((_x) >> 0) & 0xfffff)
/*----------------------------------------------------------------
  Register: SidedoorDataWriteLo - ARM sidedoor ram write data lo (bits [31:0])
    Bits: 31:0  SidedoorDataWrLo(rw) - This field controls the ram write DATA pins (bits [31:0]) for the arm core and L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SIDEDOORDATAWRITELO                     0x0000018c
#define   NFP_ARM_GCSR_SIDEDOORDATAWRITELO_SIDEDOORDATAWRLO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SIDEDOORDATAWRITELO_SIDEDOORDATAWRLO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SidedoorDataWriteHi - ARM sidedoor ram write data hi (bits [63:32])
    Bits: 31:0  SidedoorDataWrHi(rw) - This field controls the ram write DATA pins (bits [63:32]) for the arm core and L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SIDEDOORDATAWRITEHI                     0x00000190
#define   NFP_ARM_GCSR_SIDEDOORDATAWRITEHI_SIDEDOORDATAWRHI(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SIDEDOORDATAWRITEHI_SIDEDOORDATAWRHI_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SidedoorBECore - ARM sidedoor ram be (bit enables) and we (write enables) for core rams
    Bits: 24    SidedoorCoreWE(rw) - This bit is set to write enable sidedoor access into the arm core memories.
    Bits: 23:0  SidedoorCoreBE(rw) - This field controls the bit enables for the arm core memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SIDEDOORBECORE                          0x00000194
#define   NFP_ARM_GCSR_SIDEDOORBECORE_SIDEDOORCOREWE         BIT(24)
#define   NFP_ARM_GCSR_SIDEDOORBECORE_SIDEDOORCOREBE(_x)     (((_x) & 0xffffff) << 0)
#define   NFP_ARM_GCSR_SIDEDOORBECORE_SIDEDOORCOREBE_of(_x)  (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: SidedoorDCTLPl310 - ARM sidedoor ram DCTL for L2 cache rams
    Bits: 19:0  SidedoorPl310DCTL(rw) - This field controls the data read muxing for the arm L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SIDEDOORDCTL                            0x00000198
#define   NFP_ARM_GCSR_SIDEDOORDCTL_SIDEDOORPL310DCTL(_x)    (((_x) & 0xfffff) << 0)
#define   NFP_ARM_GCSR_SIDEDOORDCTL_SIDEDOORPL310DCTL_of(_x) (((_x) >> 0) & 0xfffff)
/*----------------------------------------------------------------
  Register: SidedoorWEPl310 - ARM sidedoor ram we for L2 cache rams
    Bits: 31:0  SidedoorPl310WE(rw) - This field controls the ram write enable pins for the arm L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SIDEDOORWEPL310                         0x0000019c
#define   NFP_ARM_GCSR_SIDEDOORWEPL310_SIDEDOORPL310WE(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SIDEDOORWEPL310_SIDEDOORPL310WE_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SidedoorCoreDataReadLo - ARM sidedoor ram read data lo (bits [31:0]) for core rams
    Bits: 31:0  SidedoorDataRdLo(ro) - This register returns ram read data (bits [31:0]) for the arm core memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SIDEDOORCOREDATAREADLO                  0x000001a0
#define   NFP_ARM_GCSR_SIDEDOORCOREDATAREADLO_SIDEDOORDATARDLO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SIDEDOORCOREDATAREADLO_SIDEDOORDATARDLO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SidedoorCoreDataReadHi - ARM sidedoor ram read data hi (bits [63:32]) for core rams
    Bits: 31:0  SidedoorDataRdHiCore(ro) - This register returns ram read data (bits [63:32]) for the arm core memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SIDEDOORCOREDATAREADHI                  0x000001a4
#define   NFP_ARM_GCSR_SIDEDOORCOREDATAREADHI_SIDEDOORDATARDHICORE(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SIDEDOORCOREDATAREADHI_SIDEDOORDATARDHICORE_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SidedoorPl310DataReadLo - ARM sidedoor ram read data lo (bits [31:0]) for L2 cache rams
    Bits: 31:0  SidedoorDataRdLoPl310(ro) - This register returns ram read data (bits [31:0]) for the arm L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SIDEDOORPL310DATAREADLO                 0x000001a8
#define   NFP_ARM_GCSR_SIDEDOORPL310DATAREADLO_SIDEDOORDATARDLOPL310(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SIDEDOORPL310DATAREADLO_SIDEDOORDATARDLOPL310_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SidedoorPl310DataReadHi - ARM sidedoor ram read data hi (bits [63:32]) for L2 cache rams
    Bits: 31:0  SidedoorDataRdHiPl310(ro) - This register returns ram read data (bits [63:32]) for the arm L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SIDEDOORPL310DATAREADHI                 0x000001ac
#define   NFP_ARM_GCSR_SIDEDOORPL310DATAREADHI_SIDEDOORDATARDHIPL310(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SIDEDOORPL310DATAREADHI_SIDEDOORDATARDHIPL310_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: JumpToSelf[16] - Jump To Self opcode register
    Bits: 31:0  JumpToSelf(ro) - This register provides a jump to self instruction to the arm core when the configurater process fails and the arm is spun up for the arm JTAG to come in and program the configurater flash.
*/
#define NFP_ARM_GCSR_JUMPTOSELF(_x)                          (0x00000400 + (0x4 * ((_x) & 0xf)))
#define   NFP_ARM_GCSR_JUMPTOSELF_JUMPTOSELF(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_JUMPTOSELF_JUMPTOSELF_of(_x)          (((_x) >> 0) & 0xffffffff)

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: BulkBarCfg[8] - Bulk BAR Configuration Register
  Register: BulkBar0Boot - Boot value for the Bulk Bar 0 (ARM soft reset loads this value into Bulk Bar 0)
    Bits: 31    BarType(rw) - BAR type
    Bits: 30:27 Target(rw) - CPP command target ID
    Bits: 26:25 Token(rw) - CPP command token
    Bits: 24    Length(rw) - CPP command length increment
    Bits: 10:0  Address(rw) - Bits [39:29] of CPP command address. Bits [28:0] of CPP command are taken from the AXI command address.
*/
#define NFP_ARM_GCSR_BulkBarCfg(_x)                          (0x00000000 + (0x4 * ((_x) & 0x7)))
#define NFP_ARM_GCSR_BulkBar0Boot                            0x0000015c
#define   NFP_ARM_GCSR_BulkBarCfg_BarType                    BIT(31)
#define     NFP_ARM_GCSR_BulkBarCfg_BarType_BULK             (0 << 31)
#define     NFP_ARM_GCSR_BulkBarCfg_BarType_EXPA             BIT(31)
#define   NFP_ARM_GCSR_BulkBarCfg_Target(_x)                 (((_x) & 0xf) << 27)
#define   NFP_ARM_GCSR_BulkBarCfg_Target_of(_x)              (((_x) >> 27) & 0xf)
#define   NFP_ARM_GCSR_BulkBarCfg_Token(_x)                  (((_x) & 0x3) << 25)
#define   NFP_ARM_GCSR_BulkBarCfg_Token_of(_x)               (((_x) >> 25) & 0x3)
#define   NFP_ARM_GCSR_BulkBarCfg_Length                     BIT(24)
#define     NFP_ARM_GCSR_BulkBarCfg_Length_32BIT             (0 << 24)
#define     NFP_ARM_GCSR_BulkBarCfg_Length_64BIT             BIT(24)
#define   NFP_ARM_GCSR_BulkBarCfg_Address(_x)                (((_x) & 0x7ff) << 0)
#define   NFP_ARM_GCSR_BulkBarCfg_Address_of(_x)             (((_x) >> 0) & 0x7ff)
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
#define NFP_ARM_GCSR_ExpansionBarCfg(_x)                     (0x00000020 + (0x4 * ((_x) & 0xf)))
#define NFP_ARM_GCSR_ExpansionBar0Boot                       0x00000160
#define   NFP_ARM_GCSR_ExpansionBarCfg_BarType               BIT(31)
#define     NFP_ARM_GCSR_ExpansionBarCfg_BarType_EXPA        (0 << 31)
#define     NFP_ARM_GCSR_ExpansionBarCfg_BarType_EXPL        BIT(31)
#define   NFP_ARM_GCSR_ExpansionBarCfg_Target(_x)            (((_x) & 0xf) << 27)
#define   NFP_ARM_GCSR_ExpansionBarCfg_Target_of(_x)         (((_x) >> 27) & 0xf)
#define   NFP_ARM_GCSR_ExpansionBarCfg_Token(_x)             (((_x) & 0x3) << 25)
#define   NFP_ARM_GCSR_ExpansionBarCfg_Token_of(_x)          (((_x) >> 25) & 0x3)
#define   NFP_ARM_GCSR_ExpansionBarCfg_Length                BIT(24)
#define     NFP_ARM_GCSR_ExpansionBarCfg_Length_32BIT        (0 << 24)
#define     NFP_ARM_GCSR_ExpansionBarCfg_Length_64BIT        BIT(24)
#define   NFP_ARM_GCSR_ExpansionBarCfg_Action(_x)            (((_x) & 0x1f) << 19)
#define   NFP_ARM_GCSR_ExpansionBarCfg_Action_of(_x)         (((_x) >> 19) & 0x1f)
#define     NFP_ARM_GCSR_ExpansionBarCfg_Action_DERIVED      (0)
#define   NFP_ARM_GCSR_ExpansionBarCfg_Address(_x)           (((_x) & 0x7fff) << 0)
#define   NFP_ARM_GCSR_ExpansionBarCfg_Address_of(_x)        (((_x) >> 0) & 0x7fff)
/*----------------------------------------------------------------
  Register: ExplicitBarCfg0[8] - Explicit BAR Configuration Register 0_Reg
    Bits: 17:0  Address(rw) - Bits [39:22] of CPP command address. Bits [21:0] of CPP command are taken from the AXI command address.
*/
#define NFP_ARM_GCSR_ExplicitBarCfg0(_x)                     (0x00000060 + (0x4 * ((_x) & 0x7)))
#define   NFP_ARM_GCSR_ExplicitBarCfg0_Address(_x)           (((_x) & 0x3ffff) << 0)
#define   NFP_ARM_GCSR_ExplicitBarCfg0_Address_of(_x)        (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: ExplicitBarCfg1[8] - Explicit BAR Configuration Register 1_Reg
    Bits: 31    Posted(rw) - CPP command posted
    Bits: 30:24 SignalRef(rw) - CPP command signal ref
    Bits: 23:16 DataMaster(rw) - CPP command data master
    Bits: 13:0  DataRef(rw) - CPP command data ref
*/
#define NFP_ARM_GCSR_ExplicitBarCfg1(_x)                     (0x00000080 + (0x4 * ((_x) & 0x7)))
#define   NFP_ARM_GCSR_ExplicitBarCfg1_Posted                BIT(31)
#define   NFP_ARM_GCSR_ExplicitBarCfg1_SignalRef(_x)         (((_x) & 0x7f) << 24)
#define   NFP_ARM_GCSR_ExplicitBarCfg1_SignalRef_of(_x)      (((_x) >> 24) & 0x7f)
#define   NFP_ARM_GCSR_ExplicitBarCfg1_DataMaster(_x)        (((_x) & 0xff) << 16)
#define   NFP_ARM_GCSR_ExplicitBarCfg1_DataMaster_of(_x)     (((_x) >> 16) & 0xff)
#define   NFP_ARM_GCSR_ExplicitBarCfg1_DataRef(_x)           (((_x) & 0x3fff) << 0)
#define   NFP_ARM_GCSR_ExplicitBarCfg1_DataRef_of(_x)        (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: ExplicitBarCfg2[8] - Explicit BAR Configuration Register 2_Reg
    Bits: 31:28 Target(rw) - CPP command target ID
    Bits: 27:23 Action(rw) - CPP Command Action Encoding
    Bits: 22:18 Length(rw) - CPP command length
    Bits: 17:10 ByteMask(rw) - CPP command byte mask
    Bits:  9:8  Token(rw) - CPP command token
    Bits:  7:0  SignalMaster(rw) - CPP command signal master
*/
#define NFP_ARM_GCSR_ExplicitBarCfg2(_x)                     (0x000000a0 + (0x4 * ((_x) & 0x7)))
#define   NFP_ARM_GCSR_ExplicitBarCfg2_Target(_x)            (((_x) & 0xf) << 28)
#define   NFP_ARM_GCSR_ExplicitBarCfg2_Target_of(_x)         (((_x) >> 28) & 0xf)
#define   NFP_ARM_GCSR_ExplicitBarCfg2_Action(_x)            (((_x) & 0x1f) << 23)
#define   NFP_ARM_GCSR_ExplicitBarCfg2_Action_of(_x)         (((_x) >> 23) & 0x1f)
#define   NFP_ARM_GCSR_ExplicitBarCfg2_Length(_x)            (((_x) & 0x1f) << 18)
#define   NFP_ARM_GCSR_ExplicitBarCfg2_Length_of(_x)         (((_x) >> 18) & 0x1f)
#define   NFP_ARM_GCSR_ExplicitBarCfg2_ByteMask(_x)          (((_x) & 0xff) << 10)
#define   NFP_ARM_GCSR_ExplicitBarCfg2_ByteMask_of(_x)       (((_x) >> 10) & 0xff)
#define   NFP_ARM_GCSR_ExplicitBarCfg2_Token(_x)             (((_x) & 0x3) << 8)
#define   NFP_ARM_GCSR_ExplicitBarCfg2_Token_of(_x)          (((_x) >> 8) & 0x3)
#define   NFP_ARM_GCSR_ExplicitBarCfg2_SignalMaster(_x)      (((_x) & 0xff) << 0)
#define   NFP_ARM_GCSR_ExplicitBarCfg2_SignalMaster_of(_x)   (((_x) >> 0) & 0xff)
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
#define NFP_ARM_GCSR_PostedCommandSignal(_x)                 (0x000000c0 + (0x4 * ((_x) & 0x7)))
#define   NFP_ARM_GCSR_PostedCommandSignal_SignalB(_x)       (((_x) & 0x7f) << 25)
#define   NFP_ARM_GCSR_PostedCommandSignal_SignalB_of(_x)    (((_x) >> 25) & 0x7f)
#define   NFP_ARM_GCSR_PostedCommandSignal_SignalBBus        BIT(24)
#define     NFP_ARM_GCSR_PostedCommandSignal_SignalBBus_PULL (0 << 24)
#define     NFP_ARM_GCSR_PostedCommandSignal_SignalBBus_PUSH BIT(24)
#define   NFP_ARM_GCSR_PostedCommandSignal_SignalA(_x)       (((_x) & 0x7f) << 17)
#define   NFP_ARM_GCSR_PostedCommandSignal_SignalA_of(_x)    (((_x) >> 17) & 0x7f)
#define   NFP_ARM_GCSR_PostedCommandSignal_SignalABus        BIT(16)
#define     NFP_ARM_GCSR_PostedCommandSignal_SignalABus_PULL (0 << 16)
#define     NFP_ARM_GCSR_PostedCommandSignal_SignalABus_PUSH BIT(16)
#define   NFP_ARM_GCSR_PostedCommandSignal_NosplitReceivedB  BIT(11)
#define   NFP_ARM_GCSR_PostedCommandSignal_NosplitEnableB    BIT(10)
#define   NFP_ARM_GCSR_PostedCommandSignal_NosplitReceivedA  BIT(9)
#define   NFP_ARM_GCSR_PostedCommandSignal_NosplitEnableA    BIT(8)
#define   NFP_ARM_GCSR_PostedCommandSignal_SignalBReceived   BIT(7)
#define   NFP_ARM_GCSR_PostedCommandSignal_SignalBValid      BIT(6)
#define   NFP_ARM_GCSR_PostedCommandSignal_SignalAReceived   BIT(5)
#define   NFP_ARM_GCSR_PostedCommandSignal_SignalAValid      BIT(4)
#define   NFP_ARM_GCSR_PostedCommandSignal_CmdComplete       BIT(0)
/*----------------------------------------------------------------
  Register: MPCoreBaseAddress - MPCore Base Address
    Bits: 31:13 BaseAddress(rw) - Base address for CSRs in MPCore
*/
#define NFP_ARM_GCSR_MPCoreBaseAddress                       0x000000e0
#define   NFP_ARM_GCSR_MPCoreBaseAddress_BaseAddress(_x)     (((_x) & 0x7ffff) << 13)
#define   NFP_ARM_GCSR_MPCoreBaseAddress_BaseAddress_of(_x)  (((_x) >> 13) & 0x7ffff)
/*----------------------------------------------------------------
  Register: PL310BaseAddress - PL310 Base Address
    Bits: 31:12 BaseAddress(rw) - Base address for CSRs in PL310 L2 cache
*/
#define NFP_ARM_GCSR_PL310BaseAddress                        0x000000e4
#define   NFP_ARM_GCSR_PL310BaseAddress_BaseAddress(_x)      (((_x) & 0xfffff) << 12)
#define   NFP_ARM_GCSR_PL310BaseAddress_BaseAddress_of(_x)   (((_x) >> 12) & 0xfffff)
/*----------------------------------------------------------------
  Register: MPCoreCfg - MPCore Configuration
  Register: MPCoreCfgBoot - Boot value for the ARM MPCoreCfg (ARM soft reset loads this value into MPCoreCfg)
    Bits: 25    PL310_Cfg_Bigend(rw) - L2 Cache big-endian.
    Bits: 24:19 PL310_CacheID(rw) - L2 Cache ID.
    Bits: 18:15 ARM_Cfg_NMI(rw) - NMI configuration.
    Bits: 13:12 MP0ConfigEndian(rw) - MP0 endianess
    Bits:  8    MP0RstVecLoc(ro) - MP0 Reset Vector location
    Bits:  7:4  MP0OutClkEnable(rw) - MP Output clock enable
    Bits:  3:0  ARMClusterId(rw) - Base address for CSRs in PL310 L2 Cache
*/
#define NFP_ARM_GCSR_MPCoreCfg                               0x000000e8
#define NFP_ARM_GCSR_MPCoreCfgBoot                           0x00000164
#define   NFP_ARM_GCSR_MPCoreCfg_PL310_Cfg_Bigend            BIT(25)
#define   NFP_ARM_GCSR_MPCoreCfg_PL310_CacheID(_x)           (((_x) & 0x3f) << 19)
#define   NFP_ARM_GCSR_MPCoreCfg_PL310_CacheID_of(_x)        (((_x) >> 19) & 0x3f)
#define   NFP_ARM_GCSR_MPCoreCfg_ARM_Cfg_NMI(_x)             (((_x) & 0xf) << 15)
#define   NFP_ARM_GCSR_MPCoreCfg_ARM_Cfg_NMI_of(_x)          (((_x) >> 15) & 0xf)
#define   NFP_ARM_GCSR_MPCoreCfg_MP0ConfigEndian(_x)         (((_x) & 0x3) << 12)
#define   NFP_ARM_GCSR_MPCoreCfg_MP0ConfigEndian_of(_x)      (((_x) >> 12) & 0x3)
#define     NFP_ARM_GCSR_MPCoreCfg_MP0ConfigEndian_LITTLE    (0)
#define     NFP_ARM_GCSR_MPCoreCfg_MP0ConfigEndian_BIG       (1)
#define   NFP_ARM_GCSR_MPCoreCfg_MP0RstVecLoc                BIT(8)
#define     NFP_ARM_GCSR_MPCoreCfg_MP0RstVecLoc_LO           (0 << 8)
#define     NFP_ARM_GCSR_MPCoreCfg_MP0RstVecLoc_HI           BIT(8)
#define   NFP_ARM_GCSR_MPCoreCfg_MP0OutClkEnable(_x)         (((_x) & 0xf) << 4)
#define   NFP_ARM_GCSR_MPCoreCfg_MP0OutClkEnable_of(_x)      (((_x) >> 4) & 0xf)
#define   NFP_ARM_GCSR_MPCoreCfg_ARMClusterId(_x)            (((_x) & 0xf) << 0)
#define   NFP_ARM_GCSR_MPCoreCfg_ARMClusterId_of(_x)         (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: ArmDft - ARM Design for Test register
    Bits: 20    DbgReq(rw) - MPCore0 Debug Request (Write 1 to initiate debug request, cleared by HW on DBGACK from ARM)
    Bits: 19    DbgEn(rw) - MPCore0 Debug Enable
    Bits: 18    WfeEvtTrg(rw) - CSR WFE event trigger
    Bits: 17    EtmWfiReady(rw) - ETM WFU Ready handshake response (active low)
    Bits: 16    EtmPowerOn(rw) - ETM interface power on (to MPCore)
    Bits: 11:8  BistFail(ro) - BIST Fail status from DAL
    Bits:  7:4  BistDone(ro) - BIST Done status from DAL
    Bits:  2:0  BistRun(rw) - BIST Run
*/
#define NFP_ARM_GCSR_ArmDft                                  0x00000100
#define   NFP_ARM_GCSR_ArmDft_DbgReq                         BIT(20)
#define   NFP_ARM_GCSR_ArmDft_DbgEn                          BIT(19)
#define   NFP_ARM_GCSR_ArmDft_WfeEvtTrg                      BIT(18)
#define   NFP_ARM_GCSR_ArmDft_EtmWfiReady                    BIT(17)
#define   NFP_ARM_GCSR_ArmDft_EtmPowerOn                     BIT(16)
#define   NFP_ARM_GCSR_ArmDft_BistFail(_x)                   (((_x) & 0xf) << 8)
#define   NFP_ARM_GCSR_ArmDft_BistFail_of(_x)                (((_x) >> 8) & 0xf)
#define   NFP_ARM_GCSR_ArmDft_BistDone(_x)                   (((_x) & 0xf) << 4)
#define   NFP_ARM_GCSR_ArmDft_BistDone_of(_x)                (((_x) >> 4) & 0xf)
#define   NFP_ARM_GCSR_ArmDft_BistRun(_x)                    (((_x) & 0x7) << 0)
#define   NFP_ARM_GCSR_ArmDft_BistRun_of(_x)                 (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: ArmStatus - ARM Status register
    Bits:  7    commrx_cpu0(ro) - MPCore0 commrx_cpu0.
    Bits:  6    commtx_cpu0(ro) - MPCore0 commtx_cpu0.
    Bits:  5    smpnamp(ro) - MPCore0 smpnamp.
    Bits:  4:3  pwrctl0(ro) - MPCore0 pwrctl.
    Bits:  2    wfe(ro) - MPCore0 wfe.
    Bits:  1    wfi(ro) - MPCore0 wfi.
    Bits:  0    l2_idle(ro) - MPCore0 l2_idle.
*/
#define NFP_ARM_GCSR_ArmStatus                               0x00000104
#define   NFP_ARM_GCSR_ArmStatus_commrx_cpu0                 BIT(7)
#define   NFP_ARM_GCSR_ArmStatus_commtx_cpu0                 BIT(6)
#define   NFP_ARM_GCSR_ArmStatus_smpnamp                     BIT(5)
#define   NFP_ARM_GCSR_ArmStatus_pwrctl0(_x)                 (((_x) & 0x3) << 3)
#define   NFP_ARM_GCSR_ArmStatus_pwrctl0_of(_x)              (((_x) >> 3) & 0x3)
#define   NFP_ARM_GCSR_ArmStatus_wfe                         BIT(2)
#define   NFP_ARM_GCSR_ArmStatus_wfi                         BIT(1)
#define   NFP_ARM_GCSR_ArmStatus_l2_idle                     BIT(0)
/*----------------------------------------------------------------
  Register: HardModel - Hard Model register
    Bits: 31:0  HardModel(ro) - Hard Model value is derived from the configuration encryption key information. Interpretation of the bits is up to software.
*/
#define NFP_ARM_GCSR_HardModel                               0x00000140
#define   NFP_ARM_GCSR_HardModel_HardModel(_x)               (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_HardModel_HardModel_of(_x)            (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SoftModel0 - Soft Model 0 register
    Bits: 31:0  SoftModel0(rw) - Soft Model value is loaded by the configurater and/or software. Interpretation of the bits is up to software.
*/
#define NFP_ARM_GCSR_SoftModel0                              0x00000144
#define   NFP_ARM_GCSR_SoftModel0_SoftModel0(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SoftModel0_SoftModel0_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SoftModel1 - Soft Model 1 register
    Bits: 31:0  SoftModel1(rw) - Soft Model value is loaded by the configurater and/or software. Interpretation of the bits is up to software.
*/
#define NFP_ARM_GCSR_SoftModel1                              0x00000148
#define   NFP_ARM_GCSR_SoftModel1_SoftModel1(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SoftModel1_SoftModel1_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SoftModel2 - Soft Model 2 register
    Bits: 31:0  SoftModel2(rw) - Soft Model value is loaded by the configurater and/or software. Interpretation of the bits is up to software.
*/
#define NFP_ARM_GCSR_SoftModel2                              0x0000014c
#define   NFP_ARM_GCSR_SoftModel2_SoftModel2(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SoftModel2_SoftModel2_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SoftModel3 - Soft Model 3 register
    Bits: 31:0  SoftModel3(rw) - Soft Model value is loaded by the configurater and/or software. Interpretation of the bits is up to software.
*/
#define NFP_ARM_GCSR_SoftModel3                              0x00000150
#define   NFP_ARM_GCSR_SoftModel3_SoftModel3(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SoftModel3_SoftModel3_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CfgConfigSts - Configurater Configuration Status register
    Bits:  8    CfgCompleted(ro) - This bit is set to indicate the configurater completed the configuration process.
    Bits:  7:5  Cfg2Err(ro) - This field indicates the error type for the second image copy. Only valid if CfgImage is set.
    Bits:  4:2  Cfg1Err(ro) - This field indicates the error type for the first image copy.
    Bits:  1    CfgSuccess(ro) - This bit is set if the configurater successfully completed the configuration process.
    Bits:  0    CfgImage(ro) - Zero indicates the configurater suceeded using the first image. One indicates the first image failed and the second image was used.
*/
#define NFP_ARM_GCSR_CfgConfigSts                            0x00000158
#define   NFP_ARM_GCSR_CfgConfigSts_CfgCompleted             BIT(8)
#define   NFP_ARM_GCSR_CfgConfigSts_Cfg2Err(_x)              (((_x) & 0x7) << 5)
#define   NFP_ARM_GCSR_CfgConfigSts_Cfg2Err_of(_x)           (((_x) >> 5) & 0x7)
#define     NFP_ARM_GCSR_CfgConfigSts_Cfg2Err_NOERR          (0)
#define     NFP_ARM_GCSR_CfgConfigSts_Cfg2Err_BADHDR         (1)
#define     NFP_ARM_GCSR_CfgConfigSts_Cfg2Err_BADOPC         (2)
#define     NFP_ARM_GCSR_CfgConfigSts_Cfg2Err_BADCHK         (3)
#define     NFP_ARM_GCSR_CfgConfigSts_Cfg2Err_BADCS          (4)
#define   NFP_ARM_GCSR_CfgConfigSts_Cfg1Err(_x)              (((_x) & 0x7) << 2)
#define   NFP_ARM_GCSR_CfgConfigSts_Cfg1Err_of(_x)           (((_x) >> 2) & 0x7)
#define     NFP_ARM_GCSR_CfgConfigSts_Cfg1Err_NOERR          (0)
#define     NFP_ARM_GCSR_CfgConfigSts_Cfg1Err_BADHDR         (1)
#define     NFP_ARM_GCSR_CfgConfigSts_Cfg1Err_BADOPC         (2)
#define     NFP_ARM_GCSR_CfgConfigSts_Cfg1Err_BADCHK         (3)
#define     NFP_ARM_GCSR_CfgConfigSts_Cfg1Err_BADCS          (4)
#define   NFP_ARM_GCSR_CfgConfigSts_CfgSuccess               BIT(1)
#define   NFP_ARM_GCSR_CfgConfigSts_CfgImage                 BIT(0)
/*----------------------------------------------------------------
  Register: SidedoorCECore - ARM sidedoor ram ce for core rams
    Bits: 31    SidedoorCoreEnable(rw) - This bit is set to enable sidedoor access into the arm core memories.
    Bits: 17:0  SidedoorCE(rw) - This field controls the ram CE pins for the arm core memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SidedoorCECore                          0x00000180
#define   NFP_ARM_GCSR_SidedoorCECore_SidedoorCoreEnable     BIT(31)
#define   NFP_ARM_GCSR_SidedoorCECore_SidedoorCE(_x)         (((_x) & 0x3ffff) << 0)
#define   NFP_ARM_GCSR_SidedoorCECore_SidedoorCE_of(_x)      (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: SidedoorCEPl310 - ARM sidedoor ram ce for L2 cache rams
    Bits: 31    SidedoorPl310Enable(rw) - This bit is set to enable sidedoor access into the arm L2 cache memories.
    Bits: 17:0  SidedoorCE(rw) - This field controls the ram CE pins for the arm L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SidedoorCEPl310                         0x00000184
#define   NFP_ARM_GCSR_SidedoorCEPl310_SidedoorPl310Enable   BIT(31)
#define   NFP_ARM_GCSR_SidedoorCEPl310_SidedoorCE(_x)        (((_x) & 0x3ffff) << 0)
#define   NFP_ARM_GCSR_SidedoorCEPl310_SidedoorCE_of(_x)     (((_x) >> 0) & 0x3ffff)
/*----------------------------------------------------------------
  Register: SidedoorAddress - ARM sidedoor ram address
    Bits: 19:0  SidedoorAddr(rw) - This field controls the ram ADDR pins for the arm core and L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SidedoorAddress                         0x00000188
#define   NFP_ARM_GCSR_SidedoorAddress_SidedoorAddr(_x)      (((_x) & 0xfffff) << 0)
#define   NFP_ARM_GCSR_SidedoorAddress_SidedoorAddr_of(_x)   (((_x) >> 0) & 0xfffff)
/*----------------------------------------------------------------
  Register: SidedoorDataWriteLo - ARM sidedoor ram write data lo (bits [31:0])
    Bits: 31:0  SidedoorDataWrLo(rw) - This field controls the ram write DATA pins (bits [31:0]) for the arm core and L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SidedoorDataWriteLo                     0x0000018c
#define   NFP_ARM_GCSR_SidedoorDataWriteLo_SidedoorDataWrLo(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SidedoorDataWriteLo_SidedoorDataWrLo_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SidedoorDataWriteHi - ARM sidedoor ram write data hi (bits [63:32])
    Bits: 31:0  SidedoorDataWrHi(rw) - This field controls the ram write DATA pins (bits [63:32]) for the arm core and L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SidedoorDataWriteHi                     0x00000190
#define   NFP_ARM_GCSR_SidedoorDataWriteHi_SidedoorDataWrHi(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SidedoorDataWriteHi_SidedoorDataWrHi_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SidedoorBECore - ARM sidedoor ram be (bit enables) and we (write enables) for core rams
    Bits: 24    SidedoorCoreWE(rw) - This bit is set to write enable sidedoor access into the arm core memories.
    Bits: 23:0  SidedoorCoreBE(rw) - This field controls the bit enables for the arm core memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SidedoorBECore                          0x00000194
#define   NFP_ARM_GCSR_SidedoorBECore_SidedoorCoreWE         BIT(24)
#define   NFP_ARM_GCSR_SidedoorBECore_SidedoorCoreBE(_x)     (((_x) & 0xffffff) << 0)
#define   NFP_ARM_GCSR_SidedoorBECore_SidedoorCoreBE_of(_x)  (((_x) >> 0) & 0xffffff)
/*----------------------------------------------------------------
  Register: SidedoorDCTLPl310 - ARM sidedoor ram DCTL for L2 cache rams
    Bits: 19:0  SidedoorPl310DCTL(rw) - This field controls the data read muxing for the arm L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SidedoorDCTLPl310                       0x00000198
#define   NFP_ARM_GCSR_SidedoorDCTLPl310_SidedoorPl310DCTL(_x) (((_x) & 0xfffff) << 0)
#define   NFP_ARM_GCSR_SidedoorDCTLPl310_SidedoorPl310DCTL_of(_x) (((_x) >> 0) & 0xfffff)
/*----------------------------------------------------------------
  Register: SidedoorWEPl310 - ARM sidedoor ram we for L2 cache rams
    Bits: 31:0  SidedoorPl310WE(rw) - This field controls the ram write enable pins for the arm L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SidedoorWEPl310                         0x0000019c
#define   NFP_ARM_GCSR_SidedoorWEPl310_SidedoorPl310WE(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SidedoorWEPl310_SidedoorPl310WE_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SidedoorCoreDataReadLo - ARM sidedoor ram read data lo (bits [31:0]) for core rams
    Bits: 31:0  SidedoorDataRdLo(ro) - This register returns ram read data (bits [31:0]) for the arm core memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SidedoorCoreDataReadLo                  0x000001a0
#define   NFP_ARM_GCSR_SidedoorCoreDataReadLo_SidedoorDataRdLo(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SidedoorCoreDataReadLo_SidedoorDataRdLo_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SidedoorCoreDataReadHi - ARM sidedoor ram read data hi (bits [63:32]) for core rams
    Bits: 31:0  SidedoorDataRdHiCore(ro) - This register returns ram read data (bits [63:32]) for the arm core memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SidedoorCoreDataReadHi                  0x000001a4
#define   NFP_ARM_GCSR_SidedoorCoreDataReadHi_SidedoorDataRdHiCore(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SidedoorCoreDataReadHi_SidedoorDataRdHiCore_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SidedoorPl310DataReadLo - ARM sidedoor ram read data lo (bits [31:0]) for L2 cache rams
    Bits: 31:0  SidedoorDataRdLoPl310(ro) - This register returns ram read data (bits [31:0]) for the arm L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SidedoorPl310DataReadLo                 0x000001a8
#define   NFP_ARM_GCSR_SidedoorPl310DataReadLo_SidedoorDataRdLoPl310(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SidedoorPl310DataReadLo_SidedoorDataRdLoPl310_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: SidedoorPl310DataReadHi - ARM sidedoor ram read data hi (bits [63:32]) for L2 cache rams
    Bits: 31:0  SidedoorDataRdHiPl310(ro) - This register returns ram read data (bits [63:32]) for the arm L2 cache memories during sidedoor accesses.
*/
#define NFP_ARM_GCSR_SidedoorPl310DataReadHi                 0x000001ac
#define   NFP_ARM_GCSR_SidedoorPl310DataReadHi_SidedoorDataRdHiPl310(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_SidedoorPl310DataReadHi_SidedoorDataRdHiPl310_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: JumpToSelf[16] - Jump To Self opcode register
    Bits: 31:0  JumpToSelf(ro) - This register provides a jump to self instruction to the arm core when the configurater process fails and the arm is spun up for the arm JTAG to come in and program the configurater flash.
*/
#define NFP_ARM_GCSR_JumpToSelf(_x)                          (0x00000400 + (0x4 * ((_x) & 0xf)))
#define   NFP_ARM_GCSR_JumpToSelf_JumpToSelf(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_ARM_GCSR_JumpToSelf_JumpToSelf_of(_x)          (((_x) >> 0) & 0xffffffff)

#endif /* NFP6000_LONGNAMES */


/* Gasket CSRs */
/* NOTE: These cannot be remapped, and are always at this location.
 */
#define NFP_ARM_GCSR_START	(0xd6000000 + NFP_ARM_GCSR)
#define NFP_ARM_GCSR_SIZE	SZ_64K

/* BAR CSRs
 */
#define NFP_ARM_GCSR_BULK_BITS	11
#define NFP_ARM_GCSR_EXPA_BITS	15
#define NFP_ARM_GCSR_EXPL_BITS	18

#define NFP_ARM_GCSR_BULK_SHIFT	(40 - 11)
#define NFP_ARM_GCSR_EXPA_SHIFT	(40 - 15)
#define NFP_ARM_GCSR_EXPL_SHIFT	(40 - 18)

#define NFP_ARM_GCSR_BULK_SIZE	BIT(NFP_ARM_GCSR_BULK_SHIFT)
#define NFP_ARM_GCSR_EXPA_SIZE	BIT(NFP_ARM_GCSR_EXPA_SHIFT)
#define NFP_ARM_GCSR_EXPL_SIZE	BIT(NFP_ARM_GCSR_EXPL_SHIFT)

#define NFP_ARM_GCSR_EXPA_MASK          ((NFP_ARM_GCSR_EXPA_SIZE) - 1)
#define NFP_ARM_GCSR_EXPL_MASK          ((NFP_ARM_GCSR_EXPL_SIZE) - 1)

#ifdef NFP6000_LONGNAMES

#define NFP_ARM_GCSR_EXPL2_CSR(target, action, length, byte_mask, token, signal_master) \
	(NFP_ARM_GCSR_ExplicitBarCfg2_Target(target) | \
	 NFP_ARM_GCSR_ExplicitBarCfg2_Action(action) | \
	 NFP_ARM_GCSR_ExplicitBarCfg2_Length(length) | \
	 NFP_ARM_GCSR_ExplicitBarCfg2_ByteMask(byte_mask) | \
	 NFP_ARM_GCSR_ExplicitBarCfg2_Token(token) | \
	 NFP_ARM_GCSR_ExplicitBarCfg2_SignalMaster(signal_master))
#define NFP_ARM_GCSR_EXPL1_CSR(posted, signal_ref, data_master, data_ref) \
	(((posted) ? NFP_ARM_GCSR_EXPL1_BAR_POSTED : 0) | \
	 NFP_ARM_GCSR_ExplicitBarCfg1_SignalRef(signal_ref) | \
	 NFP_ARM_GCSR_ExplicitBarCfg1_DataMaster(data_master) | \
	 NFP_ARM_GCSR_ExplicitBarCfg1_DataRef(data_ref))
#define NFP_ARM_GCSR_EXPL0_CSR(address) \
	NFP_ARM_GCSR_ExplicitBarCfg0_Address((address) >> NFP_ARM_GCSR_EXPL_SHIFT)
#define NFP_ARM_GCSR_EXPL_POST_EXPECT_A(sig_ref, is_push, is_required) \
	(NFP_ARM_GCSR_PostedCommandSignal_SignalA(sig_ref) | \
	 ((is_push) ? NFP_ARM_GCSR_PostedCommandSignal_SignalABus_PUSH : NFP_ARM_GCSR_PostedCommandSignal_SignalABus_PULL) | \
	 ((is_required) ? NFP_ARM_GCSR_PostedCommandSignal_SignalAValid : 0))
#define NFP_ARM_GCSR_EXPL_POST_EXPECT_B(sig_ref, is_push, is_required) \
	(NFP_ARM_GCSR_PostedCommandSignal_SignalB(sig_ref) | \
	 ((is_push) ? NFP_ARM_GCSR_PostedCommandSignal_SignalBBus_PUSH : NFP_ARM_GCSR_PostedCommandSignal_SignalBBus_PULL) | \
	 ((is_required) ? NFP_ARM_GCSR_PostedCommandSignal_SignalBValid : 0))

#define NFP_ARM_GCSR_EXPA_CSR(mode, target, token, is_64, action, address) \
	(((mode) ? NFP_ARM_GCSR_ExpansionBarCfg_BarType_EXPL : NFP_ARM_GCSR_ExpansionBarCfg_BarType_EXPA) | \
	 NFP_ARM_GCSR_ExpansionBarCfg_Target(target) | \
	 NFP_ARM_GCSR_ExpansionBarCfg_Token(token) | \
	 ((is_64) ? NFP_ARM_GCSR_ExpansionBarCfg_Length_64BIT : NFP_ARM_GCSR_ExpansionBarCfg_Length_32BIT) | \
	 NFP_ARM_GCSR_ExpansionBarCfg_Action(action) | \
	 NFP_ARM_GCSR_ExpansionBarCfg_Address((address) >> NFP_ARM_GCSR_EXPA_SHIFT))

#define NFP_ARM_GCSR_BULK_CSR(mode, target, token, is_64, address) \
	(((mode) ? NFP_ARM_GCSR_BulkBarCfg_BarType_EXPA : NFP_ARM_GCSR_BulkBarCfg_BarType_BULK) | \
	 NFP_ARM_GCSR_BulkBarCfg_Target(target) | \
	 NFP_ARM_GCSR_BulkBarCfg_Token(token) | \
	 ((is_64) ? NFP_ARM_GCSR_BulkBarCfg_Length_64BIT : NFP_ARM_GCSR_BulkBarCfg_Length_32BIT) | \
	 NFP_ARM_GCSR_BulkBarCfg_Address((address) >> NFP_ARM_GCSR_BULK_SHIFT))

#else /* NFP6000_LONGNAMES */

#define NFP_ARM_GCSR_EXPL2_CSR(target, action, length, byte_mask, token, signal_master) \
	(NFP_ARM_GCSR_EXPL2_BAR_TGT(target) | \
	 NFP_ARM_GCSR_EXPL2_BAR_ACT(action) | \
	 NFP_ARM_GCSR_EXPL2_BAR_LEN(length) | \
	 NFP_ARM_GCSR_EXPL2_BAR_BYTE_MASK(byte_mask) | \
	 NFP_ARM_GCSR_EXPL2_BAR_TOK(token) | \
	 NFP_ARM_GCSR_EXPL2_BAR_SIGNAL_MASTER(signal_master))
#define NFP_ARM_GCSR_EXPL1_CSR(posted, signal_ref, data_master, data_ref) \
	(((posted) ? NFP_ARM_GCSR_EXPL1_BAR_POSTED : 0) | \
	 NFP_ARM_GCSR_EXPL1_BAR_SIGNAL_REF(signal_ref) | \
	 NFP_ARM_GCSR_EXPL1_BAR_DATA_MASTER(data_master) | \
	 NFP_ARM_GCSR_EXPL1_BAR_DATA_REF(data_ref))
#define NFP_ARM_GCSR_EXPL0_CSR(address) \
	NFP_ARM_GCSR_EXPL0_BAR_ADDR((address) >> NFP_ARM_GCSR_EXPL_SHIFT)
#define NFP_ARM_GCSR_EXPL_POST_EXPECT_A(sig_ref, is_push, is_required) \
	(NFP_ARM_GCSR_EXPL_POST_SIG_A(sig_ref) | \
	 ((is_push) ? NFP_ARM_GCSR_EXPL_POST_SIG_A_BUS_PUSH : NFP_ARM_GCSR_EXPL_POST_SIG_A_BUS_PULL) | \
	 ((is_required) ? NFP_ARM_GCSR_EXPL_POST_SIG_A_VALID : 0))
#define NFP_ARM_GCSR_EXPL_POST_EXPECT_B(sig_ref, is_push, is_required) \
	(NFP_ARM_GCSR_EXPL_POST_SIG_B(sig_ref) | \
	 ((is_push) ? NFP_ARM_GCSR_EXPL_POST_SIG_B_BUS_PUSH : NFP_ARM_GCSR_EXPL_POST_SIG_B_BUS_PULL) | \
	 ((is_required) ? NFP_ARM_GCSR_EXPL_POST_SIG_B_VALID : 0))

#define NFP_ARM_GCSR_EXPA_CSR(mode, target, token, is_64, action, address) \
	(((mode) ? NFP_ARM_GCSR_EXPA_BAR_TYPE_EXPL : NFP_ARM_GCSR_EXPA_BAR_TYPE_EXPA) | \
	 NFP_ARM_GCSR_EXPA_BAR_TGT(target) | \
	 NFP_ARM_GCSR_EXPA_BAR_TOK(token) | \
	 ((is_64) ? NFP_ARM_GCSR_EXPA_BAR_LEN_64BIT : NFP_ARM_GCSR_EXPA_BAR_LEN_32BIT) | \
	 NFP_ARM_GCSR_EXPA_BAR_ACT(action) | \
	 NFP_ARM_GCSR_EXPA_BAR_ADDR((address) >> NFP_ARM_GCSR_EXPA_SHIFT))

#define NFP_ARM_GCSR_BULK_CSR(mode, target, token, is_64, address) \
	(((mode) ? NFP_ARM_GCSR_BULK_BAR_TYPE_EXPA : NFP_ARM_GCSR_BULK_BAR_TYPE_BULK) | \
	 NFP_ARM_GCSR_BULK_BAR_TGT(target) | \
	 NFP_ARM_GCSR_BULK_BAR_TOK(token) | \
	 ((is_64) ? NFP_ARM_GCSR_BULK_BAR_LEN_64BIT : NFP_ARM_GCSR_BULK_BAR_LEN_32BIT) | \
	 NFP_ARM_GCSR_BULK_BAR_ADDR((address) >> NFP_ARM_GCSR_BULK_SHIFT))

#endif /* NFP6000_LONGNAMES */

#endif /* NFP6000_ARM_H */
