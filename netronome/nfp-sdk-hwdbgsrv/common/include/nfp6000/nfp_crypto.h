/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_CRYPTO_H
#define NFP6000_CRYPTO_H

/*================================================================*/
/* TGT 12                                                         */

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + NFP_CRYPTO_SRAM */
#define NFP_CRYPTO_SRAM                                         (0x000000)

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP_XPB_ISLAND(island))          */

#define CRYPTOX_BASE                                            (0xa0000)

/* XPB_BASE + NFP_CRYPTOX_CIF + NFP_CRYPTOX_CIF_* */
#define NFP_CRYPTOX_CIF                                         (CRYPTOX_BASE + 0x010000)
/* XPB_BASE + NFP_CRYPTOX_CDS_ECC + (nfp_ecc.h) */
#define NFP_CRYPTOX_CDS_ECC                                     (CRYPTOX_BASE + 0x020000)
/* XPB_BASE + NFP_CRYPTOX_CPPM_ECC + (nfp_ecc.h) */
#define NFP_CRYPTOX_CPPM_ECC                                    (CRYPTOX_BASE + 0x030000)
/* XPB_BASE + NFP_CRYPTOX_BULK + NFP_CRYPTOX_BULK_* */
#define NFP_CRYPTOX_BULK                                        (CRYPTOX_BASE + 0x070000)
/* XPB_BASE + NFP_CRYPTOX_CORE0 + NFP_CRYPTOX_CORE_* */
#define NFP_CRYPTOX_CORE0                                       (CRYPTOX_BASE + 0x080000)
/* XPB_BASE + NFP_CRYPTOX_CORE1 + NFP_CRYPTOX_CORE_* */
#define NFP_CRYPTOX_CORE1                                       (CRYPTOX_BASE + 0x090000)
/* XPB_BASE + NFP_CRYPTOX_CORE2 + NFP_CRYPTOX_CORE_* */
#define NFP_CRYPTOX_CORE2                                       (CRYPTOX_BASE + 0x0a0000)
/* XPB_BASE + NFP_CRYPTOX_CORE3 + NFP_CRYPTOX_CORE_* */
#define NFP_CRYPTOX_CORE3                                       (CRYPTOX_BASE + 0x0b0000)
/* XPB_BASE + NFP_CRYPTOX_CORE4 + NFP_CRYPTOX_CORE_* */
#define NFP_CRYPTOX_CORE4                                       (CRYPTOX_BASE + 0x0c0000)
/* XPB_BASE + NFP_CRYPTOX_CORE5 + NFP_CRYPTOX_CORE_* */
#define NFP_CRYPTOX_CORE5                                       (CRYPTOX_BASE + 0x0d0000)

/*================================================================*/


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: CIFConfig - CIF Configuration
*/
#define NFP_CRYPTOX_CIF_CFG                                  0x00000000
/*----------------------------------------------------------------
  Register: CIFErrFrst1 - Tracks the first error reported in error register since last clear.
    Bits: 29    CDSBlkUndflw(ro) - CDS Error. Dispatcher Bulk Req FIFO underflow
    Bits: 28    CDSBlkOvrflw(ro) - CDS Error. Dispatcher Bulk Req FIFO overflow
    Bits: 27    CDSDmaUndflw(ro) - CDS Error. Dispatcher DMA Req FIFO underflow
    Bits: 26    CDSDmsOvrflw(ro) - CDS Error. Dispatcher DMA Req FIFO overflow
    Bits: 25    CDSCbgUndflw(ro) - CDS Error. Dispatcher CBG Req FIFO underflow
    Bits: 24    CDSCbgOvrflw(ro) - CDS Error. Dispatcher CBG Req FIFO overflow
    Bits: 23    CDSEvtUndflw(ro) - CDS Error. Dispatcher Event Req FIFO underflow
    Bits: 22    CDSEvtOvrflw(ro) - CDS Error. Dispatcher Event Req FIFO overflow
    Bits: 21    CDSPreUndflw(ro) - CDS Error. Dispatcher Prefetch FIFO underflow
    Bits: 20    CDSPreOvrflw(ro) - CDS Error. Dispatcher Prefetch FIFO overflow
    Bits: 19    CDSSglUndflw(ro) - CDS Error. Dispatcher Signal FIFO underflow
    Bits: 18    CDSSglOvrflw(ro) - CDS Error. Dispatcher Signal FIFO overflow
    Bits: 17    CBGRdUndflw(ro) - CBG Error. Buffer Gasket Read Debug FIFO underflow
    Bits: 16    CBGRdOvrflw(ro) - CBG Error. Buffer Gasket Read Debug FIFO overflow
    Bits: 13    CBGBlkUndflw(ro) - CBG Error. Buffer Gasket Bulk FIFO underflow
    Bits: 12    CBGBlkOvrflw(ro) - CBG Error. Buffer Gasket Bulk FIFO overflow
    Bits: 11    CBGCppUndflw(ro) - CBG Error. Buffer Gasket CPP Target FIFO underflow
    Bits: 10    CBGCppOvrflw(ro) - CBG Error. Buffer Gasket CPP Target FIFO overflow
    Bits:  9    CBGDspUndflw(ro) - CBG Error. Buffer Gasket Dispatcher FIFO underflow
    Bits:  8    CBGDspOvrflw(ro) - CBG Error. Buffer Gasket Dispatcher FIFO overflow
    Bits:  5    CDCDspUndflw(ro) - CDC Error. DMA Controller Dispatcher response FIFO underflow
    Bits:  4    CDCDspOvrflw(ro) - CDC Error. DMA Controller Dispatcher response FIFO overflow
    Bits:  3    CPMPshUndflw(ro) - CPM Error. CPP Master Push Signal FIFO underflow
    Bits:  2    CPMPshOvrflw(ro) - CPM Error. CPP Master Push Signal FIFO overflow
    Bits:  1    CPMPulUndflw(ro) - CPM Error. CPP Master Pull Signal FIFO underflow
    Bits:  0    CPMPulOvrflw(ro) - CPM Error. CPP Master Pull Signal FIFO overflow
*/
#define NFP_CRYPTOX_CIF_FERR1                                0x00000010
#define   NFP_CRYPTOX_CIF_FERR1_CDS_BLK_UNDFLW               BIT(29)
#define   NFP_CRYPTOX_CIF_FERR1_CDS_BLK_OVRFLW               BIT(28)
#define   NFP_CRYPTOX_CIF_FERR1_CDS_DMA_UNDFLW               BIT(27)
#define   NFP_CRYPTOX_CIF_FERR1_CDS_DMS_OVRFLW               BIT(26)
#define   NFP_CRYPTOX_CIF_FERR1_CDS_CBG_UNDFLW               BIT(25)
#define   NFP_CRYPTOX_CIF_FERR1_CDS_CBG_OVRFLW               BIT(24)
#define   NFP_CRYPTOX_CIF_FERR1_CDS_EVT_UNDFLW               BIT(23)
#define   NFP_CRYPTOX_CIF_FERR1_CDS_EVT_OVRFLW               BIT(22)
#define   NFP_CRYPTOX_CIF_FERR1_CDS_PRE_UNDFLW               BIT(21)
#define   NFP_CRYPTOX_CIF_FERR1_CDS_PRE_OVRFLW               BIT(20)
#define   NFP_CRYPTOX_CIF_FERR1_CDS_SGL_UNDFLW               BIT(19)
#define   NFP_CRYPTOX_CIF_FERR1_CDS_SGL_OVRFLW               BIT(18)
#define   NFP_CRYPTOX_CIF_FERR1_CBG_RD_UNDFLW                BIT(17)
#define   NFP_CRYPTOX_CIF_FERR1_CBG_RD_OVRFLW                BIT(16)
#define   NFP_CRYPTOX_CIF_FERR1_CBG_BLK_UNDFLW               BIT(13)
#define   NFP_CRYPTOX_CIF_FERR1_CBG_BLK_OVRFLW               BIT(12)
#define   NFP_CRYPTOX_CIF_FERR1_CBG_CPP_UNDFLW               BIT(11)
#define   NFP_CRYPTOX_CIF_FERR1_CBG_CPP_OVRFLW               BIT(10)
#define   NFP_CRYPTOX_CIF_FERR1_CBG_DSP_UNDFLW               BIT(9)
#define   NFP_CRYPTOX_CIF_FERR1_CBG_DSP_OVRFLW               BIT(8)
#define   NFP_CRYPTOX_CIF_FERR1_CDC_DSP_UNDFLW               BIT(5)
#define   NFP_CRYPTOX_CIF_FERR1_CDC_DSP_OVRFLW               BIT(4)
#define   NFP_CRYPTOX_CIF_FERR1_CPM_PSH_UNDFLW               BIT(3)
#define   NFP_CRYPTOX_CIF_FERR1_CPM_PSH_OVRFLW               BIT(2)
#define   NFP_CRYPTOX_CIF_FERR1_CPM_PUL_UNDFLW               BIT(1)
#define   NFP_CRYPTOX_CIF_FERR1_CPM_PUL_OVRFLW               BIT(0)
/*----------------------------------------------------------------
  Register: CIFErrFrst2 - Tracks the first error reported in error register since last clear.
    Bits: 31:16 CntxtUndflw(ro) - 31 = Context 15 Queue underflow ... 16 = Context 0 Queue underflow
    Bits: 15:0  CntxtOvrflw(ro) - 15 = Context 15 Queue overflow ... 0 = Context 0 Queue overflow
*/
#define NFP_CRYPTOX_CIF_FERR2                                0x00000014
#define   NFP_CRYPTOX_CIF_FERR2_CNTXT_UNDFLW(_x)             (((_x) & 0xffff) << 16)
#define   NFP_CRYPTOX_CIF_FERR2_CNTXT_UNDFLW_of(_x)          (((_x) >> 16) & 0xffff)
#define   NFP_CRYPTOX_CIF_FERR2_CNTXT_OVRFLW(_x)             (((_x) & 0xffff) << 0)
#define   NFP_CRYPTOX_CIF_FERR2_CNTXT_OVRFLW_of(_x)          (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: CIFErrFrst3 - Tracks the first error reported in error register since last clear.
    Bits: 11    BlkSyncErr(ro) - Crypto Dispatcher Request Sync Error. When set, the dispatcher and bulk have permanently lost sync.
    Bits: 10    DecErr(ro) - Crypto Dispatcher instruction decode error. When set, an invalid instruction was popped from a context queue.
    Bits:  9    CPTPsUflw(ro) - CPP Target fault. Push Signal FIFO Underflow.
    Bits:  8    CPTPsOflw(ro) - CPP Target fault. Push Signal FIFO Overflow.
    Bits:  7    CPTDnUflw(ro) - CPP Target fault. Data Narrowing FIFO Underflow.
    Bits:  6    CPTDnOflw(ro) - CPP Target fault. Data Narrowing FIFO Overflow.
    Bits:  5    CPTPhUflw(ro) - CPP Target fault. Push Hold FIFO Underflow.
    Bits:  4    CPTPhOflw(ro) - CPP Target fault. Push Hold FIFO Overflow.
    Bits:  3    CPTCppDta(ro) - CPP Target fault. CPP Pull Data Error.
    Bits:  2    CmdErr(ro) - CPP Target command error (Causes dispatcher shutdown)
    Bits:  1:0  CBGErr(ro) - Buffer Gasket multi-bit ECC error from Shared Memory (Causes Dispatcher shutdown)
*/
#define NFP_CRYPTOX_CIF_FERR3                                0x00000018
#define   NFP_CRYPTOX_CIF_FERR3_BLK_SYNC_ERR                 BIT(11)
#define   NFP_CRYPTOX_CIF_FERR3_DEC_ERR                      BIT(10)
#define   NFP_CRYPTOX_CIF_FERR3_CPT_PS_UFLW                  BIT(9)
#define   NFP_CRYPTOX_CIF_FERR3_CPT_PS_OFLW                  BIT(8)
#define   NFP_CRYPTOX_CIF_FERR3_CPT_DN_UFLW                  BIT(7)
#define   NFP_CRYPTOX_CIF_FERR3_CPT_DN_OFLW                  BIT(6)
#define   NFP_CRYPTOX_CIF_FERR3_CPT_PH_UFLW                  BIT(5)
#define   NFP_CRYPTOX_CIF_FERR3_CPT_PH_OFLW                  BIT(4)
#define   NFP_CRYPTOX_CIF_FERR3_CPT_CPP_DTA                  BIT(3)
#define   NFP_CRYPTOX_CIF_FERR3_CMD_ERR                      BIT(2)
#define   NFP_CRYPTOX_CIF_FERR3_CBG_ERR(_x)                  (((_x) & 0x3) << 0)
#define   NFP_CRYPTOX_CIF_FERR3_CBG_ERR_of(_x)               (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: CIFErr1 - Set bits in this register will generate events if not masked. Use CIFErrClear1 to clear.
    Bits: 29:18 CDSError(ro) - CDS Error. See CIFErrFrst1 register description.
    Bits: 17:16 CBGError1(ro) - CBG Error. See CIFErrFrst1 register description.
    Bits: 13:8  CBGError0(ro) - CBG Error. See CIFErrFrst1 register description.
    Bits:  5:4  CDCError(ro) - CDC Error. See CIFErrFrst1 register description.
    Bits:  3:0  CPMError(ro) - CPM Error. See CIFErrFrst1 register description.
*/
#define NFP_CRYPTOX_CIF_ERR1                                 0x00000020
#define   NFP_CRYPTOX_CIF_ERR1_CDS_ERROR(_x)                 (((_x) & 0xfff) << 18)
#define   NFP_CRYPTOX_CIF_ERR1_CDS_ERROR_of(_x)              (((_x) >> 18) & 0xfff)
#define   NFP_CRYPTOX_CIF_ERR1_CBG_ERROR_1(_x)               (((_x) & 0x3) << 16)
#define   NFP_CRYPTOX_CIF_ERR1_CBG_ERROR_1_of(_x)            (((_x) >> 16) & 0x3)
#define   NFP_CRYPTOX_CIF_ERR1_CBG_ERROR_0(_x)               (((_x) & 0x3f) << 8)
#define   NFP_CRYPTOX_CIF_ERR1_CBG_ERROR_0_of(_x)            (((_x) >> 8) & 0x3f)
#define   NFP_CRYPTOX_CIF_ERR1_CDC_ERROR(_x)                 (((_x) & 0x3) << 4)
#define   NFP_CRYPTOX_CIF_ERR1_CDC_ERROR_of(_x)              (((_x) >> 4) & 0x3)
#define   NFP_CRYPTOX_CIF_ERR1_CPM_ERROR(_x)                 (((_x) & 0xf) << 0)
#define   NFP_CRYPTOX_CIF_ERR1_CPM_ERROR_of(_x)              (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: CIFErr2 - Set bits in this register will generate events if not masked. Use CIFErrClear2 to clear.
    Bits: 31:16 CntxtUndflw(ro) - 31 = Context 15 Queue underflow ... 16 = Context 0 Queue underflow
    Bits: 15:0  CntxtOvrflw(ro) - 15 = Context 15 Queue overflow ... 0 = Context 0 Queue overflow
*/
#define NFP_CRYPTOX_CIF_ERR2                                 0x00000024
#define   NFP_CRYPTOX_CIF_ERR2_CNTXT_UNDFLW(_x)              (((_x) & 0xffff) << 16)
#define   NFP_CRYPTOX_CIF_ERR2_CNTXT_UNDFLW_of(_x)           (((_x) >> 16) & 0xffff)
#define   NFP_CRYPTOX_CIF_ERR2_CNTXT_OVRFLW(_x)              (((_x) & 0xffff) << 0)
#define   NFP_CRYPTOX_CIF_ERR2_CNTXT_OVRFLW_of(_x)           (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: CIFErr3 - Set bits in this register will generate events if not masked. Use CIFErrClear3 to clear.
    Bits: 11    BlkSyncErr(ro) - Crypto Dispatcher Request Sync Error. When set, the dispatcher and bulk have permanently lost sync.
    Bits: 10    DecErr(ro) - Crypto Dispatcher instruction decode error. When set, an invalid instruction was popped from a context queue.
    Bits:  9    CPTPsUflw(ro) - CPP Target fault. PS FIFO Underflow.
    Bits:  8    CPTPsOflw(ro) - CPP Target fault. PS FIFO Overflow.
    Bits:  7    CPTDnUflw(ro) - CPP Target fault. DN FIFO Underflow.
    Bits:  6    CPTDnOflw(ro) - CPP Target fault. DN FIFO Overflow.
    Bits:  5    CPTPhUflw(ro) - CPP Target fault. PH FIFO Underflow.
    Bits:  4    CPTPhOflw(ro) - CPP Target fault. PH FIFO Overflow.
    Bits:  3    CPTCppDta(ro) - CPP Target fault. CPP Data Error.
    Bits:  2    CmdErr(ro) - CPP Target command error (Causes dispatcher shutdown)
    Bits:  1:0  CBGErr(ro) - Buffer Gasket multi-bit ECC error (Causes dispatcher shutdown)
*/
#define NFP_CRYPTOX_CIF_ERR3                                 0x00000028
#define   NFP_CRYPTOX_CIF_ERR3_BLK_SYNC_ERR                  BIT(11)
#define   NFP_CRYPTOX_CIF_ERR3_DEC_ERR                       BIT(10)
#define   NFP_CRYPTOX_CIF_ERR3_CPT_PS_UFLW                   BIT(9)
#define   NFP_CRYPTOX_CIF_ERR3_CPT_PS_OFLW                   BIT(8)
#define   NFP_CRYPTOX_CIF_ERR3_CPT_DN_UFLW                   BIT(7)
#define   NFP_CRYPTOX_CIF_ERR3_CPT_DN_OFLW                   BIT(6)
#define   NFP_CRYPTOX_CIF_ERR3_CPT_PH_UFLW                   BIT(5)
#define   NFP_CRYPTOX_CIF_ERR3_CPT_PH_OFLW                   BIT(4)
#define   NFP_CRYPTOX_CIF_ERR3_CPT_CPP_DTA                   BIT(3)
#define   NFP_CRYPTOX_CIF_ERR3_CMD_ERR                       BIT(2)
#define   NFP_CRYPTOX_CIF_ERR3_CBG_ERR(_x)                   (((_x) & 0x3) << 0)
#define   NFP_CRYPTOX_CIF_ERR3_CBG_ERR_of(_x)                (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: CIFErrMask1 - Error mask enables. If set, the corresponding bit in the CIFErr1 register will generate an event.
    Bits: 31:30 Reserved_U(rw) - Reserved
    Bits: 29:18 CDSErRWr(rw) - Masks CDS ErRWr. See CIFErrFrst1 register description.
    Bits: 17:16 CBGErRWr1(rw) - Masks CBG ErRWr. See CIFErrFrst1 register description.
    Bits: 15:14 Reserved_M(rw) - Reserved
    Bits: 13:8  CBGErRWr(rw) - Masks CBG ErRWr. See CIFErrFrst1 register description.
    Bits:  7:6  Reserved_L(rw) - Reserved
    Bits:  5:4  CDCErRWr(rw) - Masks CDC ErRWr. See CIFErrFrst1 register description.
    Bits:  3:0  CPMErRWr(rw) - Masks CPM Error. See CIFErrFrst1 register description.
*/
#define NFP_CRYPTOX_CIF_ERRMASK1                             0x00000030
#define   NFP_CRYPTOX_CIF_ERRMASK1_RESERVED__U(_x)           (((_x) & 0x3) << 30)
#define   NFP_CRYPTOX_CIF_ERRMASK1_RESERVED__U_of(_x)        (((_x) >> 30) & 0x3)
#define   NFP_CRYPTOX_CIF_ERRMASK1_CDS_ER_R_WR(_x)           (((_x) & 0xfff) << 18)
#define   NFP_CRYPTOX_CIF_ERRMASK1_CDS_ER_R_WR_of(_x)        (((_x) >> 18) & 0xfff)
#define   NFP_CRYPTOX_CIF_ERRMASK1_CBG_ER_R_WR_1(_x)         (((_x) & 0x3) << 16)
#define   NFP_CRYPTOX_CIF_ERRMASK1_CBG_ER_R_WR_1_of(_x)      (((_x) >> 16) & 0x3)
#define   NFP_CRYPTOX_CIF_ERRMASK1_RESERVED__M(_x)           (((_x) & 0x3) << 14)
#define   NFP_CRYPTOX_CIF_ERRMASK1_RESERVED__M_of(_x)        (((_x) >> 14) & 0x3)
#define   NFP_CRYPTOX_CIF_ERRMASK1_CBG_ER_R_WR(_x)           (((_x) & 0x3f) << 8)
#define   NFP_CRYPTOX_CIF_ERRMASK1_CBG_ER_R_WR_of(_x)        (((_x) >> 8) & 0x3f)
#define   NFP_CRYPTOX_CIF_ERRMASK1_RESERVED__L(_x)           (((_x) & 0x3) << 6)
#define   NFP_CRYPTOX_CIF_ERRMASK1_RESERVED__L_of(_x)        (((_x) >> 6) & 0x3)
#define   NFP_CRYPTOX_CIF_ERRMASK1_CDC_ER_R_WR(_x)           (((_x) & 0x3) << 4)
#define   NFP_CRYPTOX_CIF_ERRMASK1_CDC_ER_R_WR_of(_x)        (((_x) >> 4) & 0x3)
#define   NFP_CRYPTOX_CIF_ERRMASK1_CPM_ER_R_WR(_x)           (((_x) & 0xf) << 0)
#define   NFP_CRYPTOX_CIF_ERRMASK1_CPM_ER_R_WR_of(_x)        (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: CIFErrMask2 - Error mask enables. If set, the corresponding bit in the CIFErr2 register will generate an event.
    Bits: 31:16 CntxtUndflw(rw) - Masks Error: 31 = Context 15 Queue underflow ... 16 = Context 0 Queue underflow
    Bits: 15:0  CntxtOvrflw(rw) - Masks Error: 15 = Context 15 Queue overflow ... 0 = Context 0 Queue overflow
*/
#define NFP_CRYPTOX_CIF_ERRMASK2                             0x00000034
#define   NFP_CRYPTOX_CIF_ERRMASK2_CNTXT_UNDFLW(_x)          (((_x) & 0xffff) << 16)
#define   NFP_CRYPTOX_CIF_ERRMASK2_CNTXT_UNDFLW_of(_x)       (((_x) >> 16) & 0xffff)
#define   NFP_CRYPTOX_CIF_ERRMASK2_CNTXT_OVRFLW(_x)          (((_x) & 0xffff) << 0)
#define   NFP_CRYPTOX_CIF_ERRMASK2_CNTXT_OVRFLW_of(_x)       (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: CIFErrMask3 - Error mask enables. If set, the corresponding bit in the CIFErr3 register will generate an event.
    Bits: 31:12 Reserved(rw) - Reserved
    Bits: 11    BlkSyncErr(rw) - Masks Error: Crypto Dispatcher Request Sync Error.
    Bits: 10    DecErr(rw) - Masks Error: Crypto Dispatcher instruction decode error.
    Bits:  9    CPTPsUflw(rw) - Masks Error: CPP Target fault. PS FIFO Underflow.
    Bits:  8    CPTPsOflw(rw) - Masks Error: CPP Target fault. PS FIFO Overflow.
    Bits:  7    CPTDnUflw(rw) - Masks Error: CPP Target fault. DN FIFO Underflow.
    Bits:  6    CPTDnOflw(rw) - Masks Error: CPP Target fault. DN FIFO Overflow.
    Bits:  5    CPTPhUflw(rw) - Masks Error: CPP Target fault. PH FIFO Underflow.
    Bits:  4    CPTPhOflw(rw) - Masks Error: CPP Target fault. PH FIFO Overflow.
    Bits:  3    CPTCppDta(rw) - Masks Error: CPP Target fault. CPP Data Error.
    Bits:  2    CmdErr(rw) - Masks Error: CPP Target command error (Causes dispatcher shutdown)
    Bits:  1:0  CBGErr(rw) - Masks Error: Buffer Gasket multi-bit ECC error (Causes dispatcher shutdown)
*/
#define NFP_CRYPTOX_CIF_ERRMASK3                             0x00000038
#define   NFP_CRYPTOX_CIF_ERRMASK3_RESERVED(_x)              (((_x) & 0xfffff) << 12)
#define   NFP_CRYPTOX_CIF_ERRMASK3_RESERVED_of(_x)           (((_x) >> 12) & 0xfffff)
#define   NFP_CRYPTOX_CIF_ERRMASK3_BLK_SYNC_ERR              BIT(11)
#define   NFP_CRYPTOX_CIF_ERRMASK3_DEC_ERR                   BIT(10)
#define   NFP_CRYPTOX_CIF_ERRMASK3_CPT_PS_UFLW               BIT(9)
#define   NFP_CRYPTOX_CIF_ERRMASK3_CPT_PS_OFLW               BIT(8)
#define   NFP_CRYPTOX_CIF_ERRMASK3_CPT_DN_UFLW               BIT(7)
#define   NFP_CRYPTOX_CIF_ERRMASK3_CPT_DN_OFLW               BIT(6)
#define   NFP_CRYPTOX_CIF_ERRMASK3_CPT_PH_UFLW               BIT(5)
#define   NFP_CRYPTOX_CIF_ERRMASK3_CPT_PH_OFLW               BIT(4)
#define   NFP_CRYPTOX_CIF_ERRMASK3_CPT_CPP_DTA               BIT(3)
#define   NFP_CRYPTOX_CIF_ERRMASK3_CMD_ERR                   BIT(2)
#define   NFP_CRYPTOX_CIF_ERRMASK3_CBG_ERR(_x)               (((_x) & 0x3) << 0)
#define   NFP_CRYPTOX_CIF_ERRMASK3_CBG_ERR_of(_x)            (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: CIFErrClear1 - Clears both the CIFErrFrst1 and CIFErr1 registers on a per bit basis. Write 1 to clear.
    Bits: 29:18 CDSError(w1c) - Clears CDS Error. See CIFErrFrst1 register description.
    Bits: 17:16 CBGError1(w1c) - Clears CBG Error. See CIFErrFrst1 register description.
    Bits: 13:8  CBGError(w1c) - Clears CBG Error. See CIFErrFrst1 register description.
    Bits:  5:4  CDCError(w1c) - Clears CDC Error. See CIFErrFrst1 register description.
    Bits:  3:0  CPMError(w1c) - Clears CPM Error. See CIFErrFrst1 register description.
*/
#define NFP_CRYPTOX_CIF_ERRCLR1                              0x00000040
#define   NFP_CRYPTOX_CIF_ERRCLR1_CDS_ERROR(_x)              (((_x) & 0xfff) << 18)
#define   NFP_CRYPTOX_CIF_ERRCLR1_CDS_ERROR_of(_x)           (((_x) >> 18) & 0xfff)
#define   NFP_CRYPTOX_CIF_ERRCLR1_CBG_ERROR_1(_x)            (((_x) & 0x3) << 16)
#define   NFP_CRYPTOX_CIF_ERRCLR1_CBG_ERROR_1_of(_x)         (((_x) >> 16) & 0x3)
#define   NFP_CRYPTOX_CIF_ERRCLR1_CBG_ERROR(_x)              (((_x) & 0x3f) << 8)
#define   NFP_CRYPTOX_CIF_ERRCLR1_CBG_ERROR_of(_x)           (((_x) >> 8) & 0x3f)
#define   NFP_CRYPTOX_CIF_ERRCLR1_CDC_ERROR(_x)              (((_x) & 0x3) << 4)
#define   NFP_CRYPTOX_CIF_ERRCLR1_CDC_ERROR_of(_x)           (((_x) >> 4) & 0x3)
#define   NFP_CRYPTOX_CIF_ERRCLR1_CPM_ERROR(_x)              (((_x) & 0xf) << 0)
#define   NFP_CRYPTOX_CIF_ERRCLR1_CPM_ERROR_of(_x)           (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: CIFErrClear2 - Clears both the CIFErrFrst2 and CIFErr2 registers on a per bit basis. Write 1 to clear.
    Bits: 31:16 CntxtUndflw(w1c) - Clears Error: 31 = Context 15 Queue underflow ... 16 = Context 0 Queue underflow
    Bits: 15:0  CntxtOvrflw(w1c) - Clears Error: 15 = Context 15 Queue overflow ... 0 = Context 0 Queue overflow
*/
#define NFP_CRYPTOX_CIF_ERRCLR2                              0x00000044
#define   NFP_CRYPTOX_CIF_ERRCLR2_CNTXT_UNDFLW(_x)           (((_x) & 0xffff) << 16)
#define   NFP_CRYPTOX_CIF_ERRCLR2_CNTXT_UNDFLW_of(_x)        (((_x) >> 16) & 0xffff)
#define   NFP_CRYPTOX_CIF_ERRCLR2_CNTXT_OVRFLW(_x)           (((_x) & 0xffff) << 0)
#define   NFP_CRYPTOX_CIF_ERRCLR2_CNTXT_OVRFLW_of(_x)        (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: CIFErrClear3 - Clears both the CIFErrFrst3 and CIFErr3 registers on a per bit basis. Write 1 to clear.
    Bits: 11    BlkSyncErr(w1c) - Clears Error: Crypto Dispatcher Request Sync Error.
    Bits: 10    DecErr(w1c) - Clears Error: Crypto Dispatcher instruction decode error.
    Bits:  9    CPTPsUflw(w1c) - Clears Error: CPP Target fault. PS FIFO Underflow.
    Bits:  8    CPTPsOflw(w1c) - Clears Error: CPP Target fault. PS FIFO Overflow.
    Bits:  7    CPTDnUflw(w1c) - Clears Error: CPP Target fault. DN FIFO Underflow.
    Bits:  6    CPTDnOflw(w1c) - Clears Error: CPP Target fault. DN FIFO Overflow.
    Bits:  5    CPTPhUflw(w1c) - Clears Error: CPP Target fault. PH FIFO Underflow.
    Bits:  4    CPTPhOflw(w1c) - Clears Error: CPP Target fault. PH FIFO Overflow.
    Bits:  3    CPTCppDta(w1c) - Clears Error: CPP Target fault. CPP Data Error.
    Bits:  2    CmdErr(w1c) - Clears Error: CPP Target command error (Causes dispatcher shutdown)
    Bits:  1:0  CBGErr(w1c) - Clears Error: Buffer Gasket multi-bit ECC error (Causes dispatcher shutdown)
*/
#define NFP_CRYPTOX_CIF_ERRCLR3                              0x00000048
#define   NFP_CRYPTOX_CIF_ERRCLR3_BLK_SYNC_ERR               BIT(11)
#define   NFP_CRYPTOX_CIF_ERRCLR3_DEC_ERR                    BIT(10)
#define   NFP_CRYPTOX_CIF_ERRCLR3_CPT_PS_UFLW                BIT(9)
#define   NFP_CRYPTOX_CIF_ERRCLR3_CPT_PS_OFLW                BIT(8)
#define   NFP_CRYPTOX_CIF_ERRCLR3_CPT_DN_UFLW                BIT(7)
#define   NFP_CRYPTOX_CIF_ERRCLR3_CPT_DN_OFLW                BIT(6)
#define   NFP_CRYPTOX_CIF_ERRCLR3_CPT_PH_UFLW                BIT(5)
#define   NFP_CRYPTOX_CIF_ERRCLR3_CPT_PH_OFLW                BIT(4)
#define   NFP_CRYPTOX_CIF_ERRCLR3_CPT_CPP_DTA                BIT(3)
#define   NFP_CRYPTOX_CIF_ERRCLR3_CMD_ERR                    BIT(2)
#define   NFP_CRYPTOX_CIF_ERRCLR3_CBG_ERR(_x)                (((_x) & 0x3) << 0)
#define   NFP_CRYPTOX_CIF_ERRCLR3_CBG_ERR_of(_x)             (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: CIFDbgRdCtrl - Control debug reads of context queues or DMA channel state.
    Bits: 31    Busy(rw) - Write 1 to kick off the debug read. Logic will clear this bit after the data is written to the crypto buffer.
    Bits: 16    Source(rw) - Source of the read data
    Bits:  7:0  Count(rw) - Number of 16B words to read
*/
#define NFP_CRYPTOX_CIF_DBGRC                                0x00000054
#define   NFP_CRYPTOX_CIF_DBGRC_BUSY                         BIT(31)
#define     NFP_CRYPTOX_CIF_DBGRC_BUSY_READING               BIT(31)
#define   NFP_CRYPTOX_CIF_DBGRC_SOURCE                       BIT(16)
#define     NFP_CRYPTOX_CIF_DBGRC_SOURCE_DISPATCHER          (0 << 16)
#define   NFP_CRYPTOX_CIF_DBGRC_COUNT(_x)                    (((_x) & 0xff) << 0)
#define   NFP_CRYPTOX_CIF_DBGRC_COUNT_of(_x)                 (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: CIFDbgRdAddr - Address control for debug reads.
    Bits: 28:16 CryptoAddr(rw) - Starting address of the crypto buffer. 16B read debug words will be written to consecutive addresses starting at this one.
    Bits: 15    CtxtInc(rw) - Valid only if Source = 0:
    Bits:  7:0  SrcAddr(rw) - Starting address for the read source: For Source = 0 (Dispatcher): [7:4] = 16B entry pointer, [3:0] = context ID; For Source = 1 (DMA Controller): [4:0] = DMA channel
*/
#define NFP_CRYPTOX_CIF_DBGRA                                0x00000058
#define   NFP_CRYPTOX_CIF_DBGRA_CRYPTO_ADDR(_x)              (((_x) & 0x1fff) << 16)
#define   NFP_CRYPTOX_CIF_DBGRA_CRYPTO_ADDR_of(_x)           (((_x) >> 16) & 0x1fff)
#define   NFP_CRYPTOX_CIF_DBGRA_CTXTINC                      BIT(15)
#define   NFP_CRYPTOX_CIF_DBGRA_SRC_ADDR(_x)                 (((_x) & 0xff) << 0)
#define   NFP_CRYPTOX_CIF_DBGRA_SRC_ADDR_of(_x)              (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: BasicConfig - Basic Configuration Register
    Bits:  7:6  DesMCP(rw) - Multicycle path for DES algorithm. The path is designed for 5 cycles with a 700MHz Crypto_Bulk block clock. This value may be adjusted to improve performance or algorithm stability when using other clock frequencies.
    Bits:  5:0  Enable(rw) - per bit enable for each bulk crypt core. '1' to enable. There must be at least one bulk crypt core enabled to activate the Bulk Crypt Director (interface to the bulk crypt block). Activating an core that is already active has no effect. It is the transition of the bit value that performs an action.
*/
#define NFP_CRYPTOX_BULK_BULK_CFG                            0x00000000
#define   NFP_CRYPTOX_BULK_BULK_CFG_DES_MCP(_x)              (((_x) & 0x3) << 6)
#define   NFP_CRYPTOX_BULK_BULK_CFG_DES_MCP_of(_x)           (((_x) >> 6) & 0x3)
#define   NFP_CRYPTOX_BULK_BULK_CFG_ENABLE(_x)               (((_x) & 0x3f) << 0)
#define   NFP_CRYPTOX_BULK_BULK_CFG_ENABLE_of(_x)            (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: WtchDgTmrCnf - Watch dog timer configuration
    Bits: 31    Clear(rw) - Set to '1' to clear the watchdog timers.
    Bits: 30    Enable(rw) - Set to '1' to enable the watchdog timers.
    Bits: 29:28 Size(rw) - Counter Size
    Bits: 27:0  Trip(rw) - Value for counter to trigger an event. This field maps to the most significant bits in the counter. For counter sizes larger than 28 bits, the least significant bits of the trip vector are zeros.
*/
#define NFP_CRYPTOX_BULK_BULK_WDCFG                          0x00000008
#define   NFP_CRYPTOX_BULK_BULK_WDCFG_CLEAR                  BIT(31)
#define   NFP_CRYPTOX_BULK_BULK_WDCFG_ENABLE                 BIT(30)
#define   NFP_CRYPTOX_BULK_BULK_WDCFG_SIZE(_x)               (((_x) & 0x3) << 28)
#define   NFP_CRYPTOX_BULK_BULK_WDCFG_SIZE_of(_x)            (((_x) >> 28) & 0x3)
#define   NFP_CRYPTOX_BULK_BULK_WDCFG_TRIP(_x)               (((_x) & 0xfffffff) << 0)
#define   NFP_CRYPTOX_BULK_BULK_WDCFG_TRIP_of(_x)            (((_x) >> 0) & 0xfffffff)
/*----------------------------------------------------------------
  Register: ErrorReg - Error Register. Reading this register will show all errors that have occurred regardless of mask value. Writing a '1' to a bit will clear the appropriate bit in this register and the "who's on first" register.
    Bits: 22    EventBulk5(rw1c) - Event/Error was for a Bulk_Crypto 5
    Bits: 21    EventBulk4(rw1c) - Event/Error was for a Bulk_Crypto 4
    Bits: 20    EventBulk3(rw1c) - Event/Error was for a Bulk_Crypto 3
    Bits: 19    EventBulk2(rw1c) - Event/Error was for a Bulk_Crypto 2
    Bits: 18    EventBulk1(rw1c) - Event/Error was for a Bulk_Crypto 1
    Bits: 17    EventBulk0(rw1c) - Event/Error was for a Bulk_Crypto 0
    Bits: 16    SeqnumErr(rw1c) - SRAM controller mismatch or read data error
    Bits: 15    WrtFifoPty(rw1c) - fifo output parity error.
    Bits: 14    WrtFifoErr(rw1c) - fifo overflow or has a 'hole'/missing location
    Bits: 13    WrtArbErr(rw1c) - a hot arb has multiple/no states active
    Bits: 12    AllocWdog5(rw1c) - watch-dog for Bulk_Crypto 5 tripped
    Bits: 11    AllocWdog4(rw1c) - watch-dog for Bulk_Crypto 4 tripped
    Bits: 10    AllocWdog3(rw1c) - watch-dog for Bulk_Crypto 3 tripped
    Bits:  9    AllocWdog2(rw1c) - watch-dog for Bulk_Crypto 2 tripped
    Bits:  8    AllocWdog1(rw1c) - watch-dog for Bulk_Crypto 1 tripped
    Bits:  7    AllocWdog0(rw1c) - watch-dog for Bulk_Crypto 0 tripped
    Bits:  6    AllocFifoPerr(rw1c) - parity error on allocator get() queue memory
    Bits:  5    AllocMemPerr(rw1c) - parity error on allocator context memory
    Bits:  4    AllocBusyErr(rw1c) - new instruction arrives while state indicates busy or instruction completion arrives while state indicates not_busy.
    Bits:  3    AllocGcntErr(rw1c) - get() context id queue exceeds depth of 12.
    Bits:  2    AllocFreeErr(rw1c) - context id for free() instruction has not been allocated
    Bits:  1    AllocGetErr(rw1c) - context id for get() instructions has already been allocated
    Bits:  0    AllocChkErr(rw1c) - context id on instruction has not been allocated
*/
#define NFP_CRYPTOX_BULK_BULK_ERR                            0x00000010
#define   NFP_CRYPTOX_BULK_BULK_ERR_EVENTC_5                 BIT(22)
#define   NFP_CRYPTOX_BULK_BULK_ERR_EVENTC_4                 BIT(21)
#define   NFP_CRYPTOX_BULK_BULK_ERR_EVENTC_3                 BIT(20)
#define   NFP_CRYPTOX_BULK_BULK_ERR_EVENTC_2                 BIT(19)
#define   NFP_CRYPTOX_BULK_BULK_ERR_EVENTC_1                 BIT(18)
#define   NFP_CRYPTOX_BULK_BULK_ERR_EVENTC_0                 BIT(17)
#define   NFP_CRYPTOX_BULK_BULK_ERR_SEQNUM_ERR               BIT(16)
#define   NFP_CRYPTOX_BULK_BULK_ERR_WRT_FIFO_PTY             BIT(15)
#define   NFP_CRYPTOX_BULK_BULK_ERR_WRT_FIFO_ERR             BIT(14)
#define   NFP_CRYPTOX_BULK_BULK_ERR_WRT_ARB_ERR              BIT(13)
#define   NFP_CRYPTOX_BULK_BULK_ERR_ALLOC_WDOG_5             BIT(12)
#define   NFP_CRYPTOX_BULK_BULK_ERR_ALLOC_WDOG_4             BIT(11)
#define   NFP_CRYPTOX_BULK_BULK_ERR_ALLOC_WDOG_3             BIT(10)
#define   NFP_CRYPTOX_BULK_BULK_ERR_ALLOC_WDOG_2             BIT(9)
#define   NFP_CRYPTOX_BULK_BULK_ERR_ALLOC_WDOG_1             BIT(8)
#define   NFP_CRYPTOX_BULK_BULK_ERR_ALLOC_WDOG_0             BIT(7)
#define   NFP_CRYPTOX_BULK_BULK_ERR_ALLOC_FIFO_PERR          BIT(6)
#define   NFP_CRYPTOX_BULK_BULK_ERR_ALLOC_MEM_PERR           BIT(5)
#define   NFP_CRYPTOX_BULK_BULK_ERR_ALLOC_BUSY_ERR           BIT(4)
#define   NFP_CRYPTOX_BULK_BULK_ERR_ALLOC_GCNT_ERR           BIT(3)
#define   NFP_CRYPTOX_BULK_BULK_ERR_ALLOC_FREE_ERR           BIT(2)
#define   NFP_CRYPTOX_BULK_BULK_ERR_ALLOC_GET_ERR            BIT(1)
#define   NFP_CRYPTOX_BULK_BULK_ERR_ALLOC_CHK_ERR            BIT(0)
/*----------------------------------------------------------------
  Register: WhsOnFrstErr - "Who's on first" register. Each bit maps directly to a bit in the Error Register. This register captures the first error bit which is selected by the mask register, blocking future error bits into this register until the register has been cleared. This register is used to determine which error happened first if multiple errors are indicated in the error register. Writing a '1' to a bit will clear the appropriate bit in this register.
    Bits: 22:0  WhsOnFrstErr(rw1c) - Same bitmap as ErrorReg
*/
#define NFP_CRYPTOX_BULK_BULK_FERR                           0x00000014
#define   NFP_CRYPTOX_BULK_BULK_FERR_WHS_ON_FRST_ERR(_x)     (((_x) & 0x7fffff) << 0)
#define   NFP_CRYPTOX_BULK_BULK_FERR_WHS_ON_FRST_ERR_of(_x)  (((_x) >> 0) & 0x7fffff)
/*----------------------------------------------------------------
  Register: ErrorMask - Error Mask. Each bit maps directly to a bit in the Error Register. This mask is used with the "Who's on first" register and the Clock Stop Mask. Setting a bit position to a '1' will enable the associated error to be detected.
    Bits: 22:0  ErrorMask(rw) - Same bitmap as ErrorReg
*/
#define NFP_CRYPTOX_BULK_BULK_ERRMASK                        0x00000018
#define   NFP_CRYPTOX_BULK_BULK_ERRMASK_ERROR_MASK(_x)       (((_x) & 0x7fffff) << 0)
#define   NFP_CRYPTOX_BULK_BULK_ERRMASK_ERROR_MASK_of(_x)    (((_x) >> 0) & 0x7fffff)
/*----------------------------------------------------------------
  Register: ClkStopMask - Clock Stop Mask. This register is used along with the Error Mask register to select which error bits will trigger a local clock stop. Setting a bit position to a '1' will enable the associated error to cause a local clock stop.
    Bits: 22:0  ClkStopMask(rw) - Same bitmap as ErrorReg
*/
#define NFP_CRYPTOX_BULK_BULK_CLKSTOP                        0x0000001c
#define   NFP_CRYPTOX_BULK_BULK_CLKSTOP_CLK_STOP_MASK(_x)    (((_x) & 0x7fffff) << 0)
#define   NFP_CRYPTOX_BULK_BULK_CLKSTOP_CLK_STOP_MASK_of(_x) (((_x) >> 0) & 0x7fffff)
/*----------------------------------------------------------------
  Register: TestMode - Test mode / Error inject. This address is used to send single cycle pulses to test the error detect registers.
    Bits: 31:24 Target(ro) - Select target logic to send error inject pulse.
    Bits: 21:0  Data(ro) - Set specific bit(s) to '1' to inject error pulse. Bit breakout depends on target value selected in Target Field.
*/
#define NFP_CRYPTOX_BULK_BULK_TESTMODE                       0x00000020
#define   NFP_CRYPTOX_BULK_BULK_TESTMODE_TARGET(_x)          (((_x) & 0xff) << 24)
#define   NFP_CRYPTOX_BULK_BULK_TESTMODE_TARGET_of(_x)       (((_x) >> 24) & 0xff)
#define   NFP_CRYPTOX_BULK_BULK_TESTMODE_DATA(_x)            (((_x) & 0x3fffff) << 0)
#define   NFP_CRYPTOX_BULK_BULK_TESTMODE_DATA_of(_x)         (((_x) >> 0) & 0x3fffff)
/*----------------------------------------------------------------
  Register: ErrRegLo - Error register low bits [31:0]. Reading this register will show all errors that have occurred regardless of mask value. Writing a '1' to a bit will clear the appropriate bit in this register and the "who's on first" Lo register.
    Bits: 29    KasumiCfgPty(w1c) - Config data from 4B path has parity error.
    Bits: 28    KasumiCfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits: 27    Snow3GCfgPty(w1c) - Config data from 4B path has parity error.
    Bits: 26    Snow3GCfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits: 25    CiphWrtPser(w1c) - Serial path fifo parity error.
    Bits: 24    CiphWrtFser(w1c) - Fifo overflow.
    Bits: 23    CiphWrtFdat(w1c) - Fifo overflow.
    Bits: 22    Arc4RdPty(w1c) - Ready data from state ram has parity error.
    Bits: 21    Arc4CfgPty(w1c) - Config data from 4B path has parity error or key read parity.
    Bits: 20    Arc4CfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits: 19    DesFifoFdm(w1c) - Fifo overflow ( mask ).
    Bits: 18    DesFifoFdf(w1c) - Fifo overflow ( flag ).
    Bits: 17    DesFifoFdo(w1c) - Fifo overflow ( data out ).
    Bits: 16    DesFifoFdi(w1c) - Fifo overflow ( data in ).
    Bits: 15    DesCfgPty(w1c) - Config data from 4B path has parity error.
    Bits: 14    DesCfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits: 13    AesFifoGcmx(w1c) - Fifo overflow ( flag ).
    Bits: 12    AesFifoGcm(w1c) - Fifo overflow ( data ).
    Bits: 11    AesFifoFdm(w1c) - Fifo overflow ( mask ).
    Bits: 10    AesFifoFdf(w1c) - aes_fifo_fdf - fifo overflow ( flag ).
    Bits:  9    AesFifoFdo(w1c) - Fifo overflow ( data out ).
    Bits:  8    AesFifoFdi(w1c) - Fifo overflow ( data in ).
    Bits:  7    AesKeyRpty(w1c) - Key read from ram has parity error.
    Bits:  6    AesKeyIpty(w1c) - Input key data from 4B path has parity error.
    Bits:  5    AesKeySize(w1c) - Input key data from 4B path has size or sequence errors.
    Bits:  4    AesCfgPty(w1c) - Config data from 4B path has parity error.
    Bits:  3    AesCfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits:  2    CpcCpkHole(w1c) - Packing function encountered hole in byte-stream.
    Bits:  1    CpcFifoOv(w1c) - Fifo overflow.
    Bits:  0    CpcFifoParity(w1c) - Parity error on fifo output (data, flags, parity)
*/
#define NFP_CRYPTOX_CORE_ERRLO                               0x00000000
#define   NFP_CRYPTOX_CORE_ERRLO_KASUMI_CFG_PTY              BIT(29)
#define   NFP_CRYPTOX_CORE_ERRLO_KASUMI_CFG_SIZE             BIT(28)
#define   NFP_CRYPTOX_CORE_ERRLO_SNOW_3G_CFG_PTY             BIT(27)
#define   NFP_CRYPTOX_CORE_ERRLO_SNOW_3G_CFG_SIZE            BIT(26)
#define   NFP_CRYPTOX_CORE_ERRLO_CIPH_WRT_PSER               BIT(25)
#define   NFP_CRYPTOX_CORE_ERRLO_CIPH_WRT_FSER               BIT(24)
#define   NFP_CRYPTOX_CORE_ERRLO_CIPH_WRT_FDAT               BIT(23)
#define   NFP_CRYPTOX_CORE_ERRLO_ARC_4_RD_PTY                BIT(22)
#define   NFP_CRYPTOX_CORE_ERRLO_ARC_4_CFG_PTY               BIT(21)
#define   NFP_CRYPTOX_CORE_ERRLO_ARC_4_CFG_SIZE              BIT(20)
#define   NFP_CRYPTOX_CORE_ERRLO_DES_FIFO_FDM                BIT(19)
#define   NFP_CRYPTOX_CORE_ERRLO_DES_FIFO_FDF                BIT(18)
#define   NFP_CRYPTOX_CORE_ERRLO_DES_FIFO_FDO                BIT(17)
#define   NFP_CRYPTOX_CORE_ERRLO_DES_FIFO_FDI                BIT(16)
#define   NFP_CRYPTOX_CORE_ERRLO_DES_CFG_PTY                 BIT(15)
#define   NFP_CRYPTOX_CORE_ERRLO_DES_CFG_SIZE                BIT(14)
#define   NFP_CRYPTOX_CORE_ERRLO_AES_FIFO_GCMX               BIT(13)
#define   NFP_CRYPTOX_CORE_ERRLO_AES_FIFO_GCM                BIT(12)
#define   NFP_CRYPTOX_CORE_ERRLO_AES_FIFO_FDM                BIT(11)
#define   NFP_CRYPTOX_CORE_ERRLO_AES_FIFO_FDF                BIT(10)
#define   NFP_CRYPTOX_CORE_ERRLO_AES_FIFO_FDO                BIT(9)
#define   NFP_CRYPTOX_CORE_ERRLO_AES_FIFO_FDI                BIT(8)
#define   NFP_CRYPTOX_CORE_ERRLO_AES_KEY_RPTY                BIT(7)
#define   NFP_CRYPTOX_CORE_ERRLO_AES_KEY_IPTY                BIT(6)
#define   NFP_CRYPTOX_CORE_ERRLO_AES_KEY_SIZE                BIT(5)
#define   NFP_CRYPTOX_CORE_ERRLO_AES_CFG_PTY                 BIT(4)
#define   NFP_CRYPTOX_CORE_ERRLO_AES_CFG_SIZE                BIT(3)
#define   NFP_CRYPTOX_CORE_ERRLO_CPC_CPK_HOLE                BIT(2)
#define   NFP_CRYPTOX_CORE_ERRLO_CPC_FIFO_OV                 BIT(1)
#define   NFP_CRYPTOX_CORE_ERRLO_CPC_FIFO_PARITY             BIT(0)
/*----------------------------------------------------------------
  Register: ErrRegHi - Error register high bits [63:32]. Reading this register will show all errors that have occurred regardless of mask value. Writing a '1' to a bit will clear the appropriate bit in this register and the "who's on first" Hi register.
    Bits: 27    CrcMemErr(w1c) - Read data error from SRAM controller (seq-num or RERR).
    Bits: 26    CrcIcfCmd(w1c) - Compressed instruction collision with normal instruction.
    Bits: 25    CpsFlag(w1c) - Flag/type mismatch on input data.
    Bits: 24    CpsCpkHole(w1c) - Packing function encountered hole in byte-stream.
    Bits: 23    CdiFifoPty(w1c) - Fifo output parity error.
    Bits: 22    CdiFifoOv(w1c) - Fifo overflow.
    Bits: 21    IcfPtr1(w1c) - Parity error on pointer_1 read.
    Bits: 20    IcfPtr0(w1c) - Parity error on pointer_0 read.
    Bits: 19    IcfData(w1c) - Parity error on instruction read.
    Bits: 18    RdrBusyErr(w1c) - New request while still busy or clear request when not busy.
    Bits: 17    RdrFifoPty(w1c) - Fifo output parity error.
    Bits: 16    RdrFifoOv(w1c) - Fifo overflow.
    Bits: 11    CRC16TopFlag(w1c) - Input data has flag/type mismatch.
    Bits: 10    CRC16CfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits:  9    CRC16CfgPty(w1c) - Config data from 4B path has parity error.
    Bits:  8    HashWrtFdat(w1c) - Fifo overflow.
    Bits:  7    ShaCfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits:  6    ShaCfgPty(w1c) - Config data from 4B path has parity error.
    Bits:  5    GfTopFlag(w1c) - Input data has flag/type mismatch.
    Bits:  4    GfCfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits:  3    GfCfgPty(w1c) - Config data from 4B path has parity error.
    Bits:  2    CphCpkHole(w1c) - Packing function encountered hole in byte-stream.
    Bits:  1    CphFifoOv(w1c) - Fifo overflow.
    Bits:  0    CphFifoPrty(w1c) - Parity error on fifo output (data, flags, parity).
*/
#define NFP_CRYPTOX_CORE_ERRHI                               0x00000004
#define   NFP_CRYPTOX_CORE_ERRHI_CRC_MEM_ERR                 BIT(27)
#define   NFP_CRYPTOX_CORE_ERRHI_CRC_ICF_CMD                 BIT(26)
#define   NFP_CRYPTOX_CORE_ERRHI_CPS_FLAG                    BIT(25)
#define   NFP_CRYPTOX_CORE_ERRHI_CPS_CPK_HOLE                BIT(24)
#define   NFP_CRYPTOX_CORE_ERRHI_CDI_FIFO_PTY                BIT(23)
#define   NFP_CRYPTOX_CORE_ERRHI_CDI_FIFO_OV                 BIT(22)
#define   NFP_CRYPTOX_CORE_ERRHI_ICF_PTR_1                   BIT(21)
#define   NFP_CRYPTOX_CORE_ERRHI_ICF_PTR_0                   BIT(20)
#define   NFP_CRYPTOX_CORE_ERRHI_ICF_DATA                    BIT(19)
#define   NFP_CRYPTOX_CORE_ERRHI_RDR_BUSY_ERR                BIT(18)
#define   NFP_CRYPTOX_CORE_ERRHI_RDR_FIFO_PTY                BIT(17)
#define   NFP_CRYPTOX_CORE_ERRHI_RDR_FIFO_OV                 BIT(16)
#define   NFP_CRYPTOX_CORE_ERRHI_CRC16_TOP_FLAG              BIT(11)
#define   NFP_CRYPTOX_CORE_ERRHI_CRC16_CFG_SIZE              BIT(10)
#define   NFP_CRYPTOX_CORE_ERRHI_CRC16_CFG_PTY               BIT(9)
#define   NFP_CRYPTOX_CORE_ERRHI_HASH_WRT_FDAT               BIT(8)
#define   NFP_CRYPTOX_CORE_ERRHI_SHA_CFG_SIZE                BIT(7)
#define   NFP_CRYPTOX_CORE_ERRHI_SHA_CFG_PTY                 BIT(6)
#define   NFP_CRYPTOX_CORE_ERRHI_GF_TOP_FLAG                 BIT(5)
#define   NFP_CRYPTOX_CORE_ERRHI_GF_CFG_SIZE                 BIT(4)
#define   NFP_CRYPTOX_CORE_ERRHI_GF_CFG_PTY                  BIT(3)
#define   NFP_CRYPTOX_CORE_ERRHI_CPH_CPK_HOLE                BIT(2)
#define   NFP_CRYPTOX_CORE_ERRHI_CPH_FIFO_OV                 BIT(1)
#define   NFP_CRYPTOX_CORE_ERRHI_CPH_FIFO_PRTY               BIT(0)
/*----------------------------------------------------------------
  Register: WhosOnFrstLo - Who's on first error register low bits [31:0]. Each bit maps directly to a bit in the Error Lo Register. This register captures the first error bit which is selected by the Mask Lo register, blocking future error bits into this register until the register has been cleared. This register is used to determine which error happened first if multiple errors are indicated in the error register. Writing a '1' to a bit will clear the appropriate bit in this register.
    Bits: 29:0  WhosOnFrstLo(w1c) - Same bitmap as ErrRegLo.
*/
#define NFP_CRYPTOX_CORE_FERRLO                              0x00000008
#define   NFP_CRYPTOX_CORE_FERRLO_WHOS_ON_FRST_LO(_x)        (((_x) & 0x3fffffff) << 0)
#define   NFP_CRYPTOX_CORE_FERRLO_WHOS_ON_FRST_LO_of(_x)     (((_x) >> 0) & 0x3fffffff)
/*----------------------------------------------------------------
  Register: WhosOnFrstHi - Who's on first error register high bits [63:32]. Each bit maps directly to a bit in the Error Hi Register. This register captures the first error bit which is selected by the Mask Hi register, blocking future error bits into this register until the register has been cleared. This register is used to determine which error happened first if multiple errors are indicated in the error register. Writing a '1' to a bit will clear the appropriate bit in this register.
    Bits: 27:0  WhosOnFrstHi(w1c) - Same bitmap as ErrRegHi.
*/
#define NFP_CRYPTOX_CORE_FERRHI                              0x0000000c
#define   NFP_CRYPTOX_CORE_FERRHI_WHOS_ON_FRST_HI(_x)        (((_x) & 0xfffffff) << 0)
#define   NFP_CRYPTOX_CORE_FERRHI_WHOS_ON_FRST_HI_of(_x)     (((_x) >> 0) & 0xfffffff)
/*----------------------------------------------------------------
  Register: ErrMaskLo - Error mask low bits [31:0]. Each bit maps directly to a bit in the Error Lo Register. This mask is used with the "Who's on First" Lo register and the Clock Stop Lo Mask. Setting a bit position to a '1' will enable the associated error to be detected.
    Bits: 29:0  ErrMaskLo(rw) - Same bitmap as ErrRegLo.
*/
#define NFP_CRYPTOX_CORE_ERRMASKLO                           0x00000010
#define   NFP_CRYPTOX_CORE_ERRMASKLO_ERR_MASK_LO(_x)         (((_x) & 0x3fffffff) << 0)
#define   NFP_CRYPTOX_CORE_ERRMASKLO_ERR_MASK_LO_of(_x)      (((_x) >> 0) & 0x3fffffff)
/*----------------------------------------------------------------
  Register: ErrMaskHi - Error mask high bits [63:32]. Each bit maps directly to a bit in the Error Hi Register. This mask is used with the "Who's on First" Hi register and the Clock Stop Hi Mask. Setting a bit position to a '1' will enable the associated error to be detected.
    Bits: 27:16 ErrMaskHi1(rw) - Same bitmap as ErrRegHi.
    Bits: 11:0  ErrMaskHi0(rw) - Same bitmap as ErrRegHi.
*/
#define NFP_CRYPTOX_CORE_ERRMASKHI                           0x00000014
#define   NFP_CRYPTOX_CORE_ERRMASKHI_ERR_MASK_HI_1(_x)       (((_x) & 0xfff) << 16)
#define   NFP_CRYPTOX_CORE_ERRMASKHI_ERR_MASK_HI_1_of(_x)    (((_x) >> 16) & 0xfff)
#define   NFP_CRYPTOX_CORE_ERRMASKHI_ERR_MASK_HI_0(_x)       (((_x) & 0xfff) << 0)
#define   NFP_CRYPTOX_CORE_ERRMASKHI_ERR_MASK_HI_0_of(_x)    (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: ClkStpMskLo - Clock stop mask low bits [31:0]. This register is used along with the Error Mask Lo register to select which error bits will trigger a local clock stop. Setting a bit position to a '1' will enable the associated error to cause a local clock stop.
    Bits: 29:0  ClkStpMskLo(rw) - Same bitmap as ErrRegLo.
*/
#define NFP_CRYPTOX_CORE_CLKSTOPLO                           0x00000018
#define   NFP_CRYPTOX_CORE_CLKSTOPLO_CLK_STP_MSK_LO(_x)      (((_x) & 0x3fffffff) << 0)
#define   NFP_CRYPTOX_CORE_CLKSTOPLO_CLK_STP_MSK_LO_of(_x)   (((_x) >> 0) & 0x3fffffff)
/*----------------------------------------------------------------
  Register: ClkStpMskHi - Clock stop mask high bits [63:32]. This register is used along with the Error Mask Hi register to select which error bits will trigger a local clock stop. Setting a bit position to a '1' will enable the associated error to cause a local clock stop.
    Bits: 27:16 ClkStpMskHi1(rw) - Same bitmap as ErrRegHi.
    Bits: 11:0  ClkStpMskHi0(rw) - Same bitmap as ErrRegHi.
*/
#define NFP_CRYPTOX_CORE_CLKSTOPHI                           0x0000001c
#define   NFP_CRYPTOX_CORE_CLKSTOPHI_CLK_STP_MSK_HI_1(_x)    (((_x) & 0xfff) << 16)
#define   NFP_CRYPTOX_CORE_CLKSTOPHI_CLK_STP_MSK_HI_1_of(_x) (((_x) >> 16) & 0xfff)
#define   NFP_CRYPTOX_CORE_CLKSTOPHI_CLK_STP_MSK_HI_0(_x)    (((_x) & 0xfff) << 0)
#define   NFP_CRYPTOX_CORE_CLKSTOPHI_CLK_STP_MSK_HI_0_of(_x) (((_x) >> 0) & 0xfff)

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: CIFConfig - CIF Configuration
*/
#define NFP_CRYPTOX_CIFConfig                                0x00000000
/*----------------------------------------------------------------
  Register: CIFErrFrst1 - Tracks the first error reported in error register since last clear.
    Bits: 29    CDSBlkUndflw(ro) - CDS Error. Dispatcher Bulk Req FIFO underflow
    Bits: 28    CDSBlkOvrflw(ro) - CDS Error. Dispatcher Bulk Req FIFO overflow
    Bits: 27    CDSDmaUndflw(ro) - CDS Error. Dispatcher DMA Req FIFO underflow
    Bits: 26    CDSDmsOvrflw(ro) - CDS Error. Dispatcher DMA Req FIFO overflow
    Bits: 25    CDSCbgUndflw(ro) - CDS Error. Dispatcher CBG Req FIFO underflow
    Bits: 24    CDSCbgOvrflw(ro) - CDS Error. Dispatcher CBG Req FIFO overflow
    Bits: 23    CDSEvtUndflw(ro) - CDS Error. Dispatcher Event Req FIFO underflow
    Bits: 22    CDSEvtOvrflw(ro) - CDS Error. Dispatcher Event Req FIFO overflow
    Bits: 21    CDSPreUndflw(ro) - CDS Error. Dispatcher Prefetch FIFO underflow
    Bits: 20    CDSPreOvrflw(ro) - CDS Error. Dispatcher Prefetch FIFO overflow
    Bits: 19    CDSSglUndflw(ro) - CDS Error. Dispatcher Signal FIFO underflow
    Bits: 18    CDSSglOvrflw(ro) - CDS Error. Dispatcher Signal FIFO overflow
    Bits: 17    CBGRdUndflw(ro) - CBG Error. Buffer Gasket Read Debug FIFO underflow
    Bits: 16    CBGRdOvrflw(ro) - CBG Error. Buffer Gasket Read Debug FIFO overflow
    Bits: 13    CBGBlkUndflw(ro) - CBG Error. Buffer Gasket Bulk FIFO underflow
    Bits: 12    CBGBlkOvrflw(ro) - CBG Error. Buffer Gasket Bulk FIFO overflow
    Bits: 11    CBGCppUndflw(ro) - CBG Error. Buffer Gasket CPP Target FIFO underflow
    Bits: 10    CBGCppOvrflw(ro) - CBG Error. Buffer Gasket CPP Target FIFO overflow
    Bits:  9    CBGDspUndflw(ro) - CBG Error. Buffer Gasket Dispatcher FIFO underflow
    Bits:  8    CBGDspOvrflw(ro) - CBG Error. Buffer Gasket Dispatcher FIFO overflow
    Bits:  5    CDCDspUndflw(ro) - CDC Error. DMA Controller Dispatcher response FIFO underflow
    Bits:  4    CDCDspOvrflw(ro) - CDC Error. DMA Controller Dispatcher response FIFO overflow
    Bits:  3    CPMPshUndflw(ro) - CPM Error. CPP Master Push Signal FIFO underflow
    Bits:  2    CPMPshOvrflw(ro) - CPM Error. CPP Master Push Signal FIFO overflow
    Bits:  1    CPMPulUndflw(ro) - CPM Error. CPP Master Pull Signal FIFO underflow
    Bits:  0    CPMPulOvrflw(ro) - CPM Error. CPP Master Pull Signal FIFO overflow
*/
#define NFP_CRYPTOX_CIFErrFrst1                              0x00000010
#define   NFP_CRYPTOX_CIFErrFrst1_CDSBlkUndflw               BIT(29)
#define   NFP_CRYPTOX_CIFErrFrst1_CDSBlkOvrflw               BIT(28)
#define   NFP_CRYPTOX_CIFErrFrst1_CDSDmaUndflw               BIT(27)
#define   NFP_CRYPTOX_CIFErrFrst1_CDSDmsOvrflw               BIT(26)
#define   NFP_CRYPTOX_CIFErrFrst1_CDSCbgUndflw               BIT(25)
#define   NFP_CRYPTOX_CIFErrFrst1_CDSCbgOvrflw               BIT(24)
#define   NFP_CRYPTOX_CIFErrFrst1_CDSEvtUndflw               BIT(23)
#define   NFP_CRYPTOX_CIFErrFrst1_CDSEvtOvrflw               BIT(22)
#define   NFP_CRYPTOX_CIFErrFrst1_CDSPreUndflw               BIT(21)
#define   NFP_CRYPTOX_CIFErrFrst1_CDSPreOvrflw               BIT(20)
#define   NFP_CRYPTOX_CIFErrFrst1_CDSSglUndflw               BIT(19)
#define   NFP_CRYPTOX_CIFErrFrst1_CDSSglOvrflw               BIT(18)
#define   NFP_CRYPTOX_CIFErrFrst1_CBGRdUndflw                BIT(17)
#define   NFP_CRYPTOX_CIFErrFrst1_CBGRdOvrflw                BIT(16)
#define   NFP_CRYPTOX_CIFErrFrst1_CBGBlkUndflw               BIT(13)
#define   NFP_CRYPTOX_CIFErrFrst1_CBGBlkOvrflw               BIT(12)
#define   NFP_CRYPTOX_CIFErrFrst1_CBGCppUndflw               BIT(11)
#define   NFP_CRYPTOX_CIFErrFrst1_CBGCppOvrflw               BIT(10)
#define   NFP_CRYPTOX_CIFErrFrst1_CBGDspUndflw               BIT(9)
#define   NFP_CRYPTOX_CIFErrFrst1_CBGDspOvrflw               BIT(8)
#define   NFP_CRYPTOX_CIFErrFrst1_CDCDspUndflw               BIT(5)
#define   NFP_CRYPTOX_CIFErrFrst1_CDCDspOvrflw               BIT(4)
#define   NFP_CRYPTOX_CIFErrFrst1_CPMPshUndflw               BIT(3)
#define   NFP_CRYPTOX_CIFErrFrst1_CPMPshOvrflw               BIT(2)
#define   NFP_CRYPTOX_CIFErrFrst1_CPMPulUndflw               BIT(1)
#define   NFP_CRYPTOX_CIFErrFrst1_CPMPulOvrflw               BIT(0)
/*----------------------------------------------------------------
  Register: CIFErrFrst2 - Tracks the first error reported in error register since last clear.
    Bits: 31:16 CntxtUndflw(ro) - 31 = Context 15 Queue underflow ... 16 = Context 0 Queue underflow
    Bits: 15:0  CntxtOvrflw(ro) - 15 = Context 15 Queue overflow ... 0 = Context 0 Queue overflow
*/
#define NFP_CRYPTOX_CIFErrFrst2                              0x00000014
#define   NFP_CRYPTOX_CIFErrFrst2_CntxtUndflw(_x)            (((_x) & 0xffff) << 16)
#define   NFP_CRYPTOX_CIFErrFrst2_CntxtUndflw_of(_x)         (((_x) >> 16) & 0xffff)
#define   NFP_CRYPTOX_CIFErrFrst2_CntxtOvrflw(_x)            (((_x) & 0xffff) << 0)
#define   NFP_CRYPTOX_CIFErrFrst2_CntxtOvrflw_of(_x)         (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: CIFErrFrst3 - Tracks the first error reported in error register since last clear.
    Bits: 11    BlkSyncErr(ro) - Crypto Dispatcher Request Sync Error. When set, the dispatcher and bulk have permanently lost sync.
    Bits: 10    DecErr(ro) - Crypto Dispatcher instruction decode error. When set, an invalid instruction was popped from a context queue.
    Bits:  9    CPTPsUflw(ro) - CPP Target fault. Push Signal FIFO Underflow.
    Bits:  8    CPTPsOflw(ro) - CPP Target fault. Push Signal FIFO Overflow.
    Bits:  7    CPTDnUflw(ro) - CPP Target fault. Data Narrowing FIFO Underflow.
    Bits:  6    CPTDnOflw(ro) - CPP Target fault. Data Narrowing FIFO Overflow.
    Bits:  5    CPTPhUflw(ro) - CPP Target fault. Push Hold FIFO Underflow.
    Bits:  4    CPTPhOflw(ro) - CPP Target fault. Push Hold FIFO Overflow.
    Bits:  3    CPTCppDta(ro) - CPP Target fault. CPP Pull Data Error.
    Bits:  2    CmdErr(ro) - CPP Target command error (Causes dispatcher shutdown)
    Bits:  1:0  CBGErr(ro) - Buffer Gasket multi-bit ECC error from Shared Memory (Causes Dispatcher shutdown)
*/
#define NFP_CRYPTOX_CIFErrFrst3                              0x00000018
#define   NFP_CRYPTOX_CIFErrFrst3_BlkSyncErr                 BIT(11)
#define   NFP_CRYPTOX_CIFErrFrst3_DecErr                     BIT(10)
#define   NFP_CRYPTOX_CIFErrFrst3_CPTPsUflw                  BIT(9)
#define   NFP_CRYPTOX_CIFErrFrst3_CPTPsOflw                  BIT(8)
#define   NFP_CRYPTOX_CIFErrFrst3_CPTDnUflw                  BIT(7)
#define   NFP_CRYPTOX_CIFErrFrst3_CPTDnOflw                  BIT(6)
#define   NFP_CRYPTOX_CIFErrFrst3_CPTPhUflw                  BIT(5)
#define   NFP_CRYPTOX_CIFErrFrst3_CPTPhOflw                  BIT(4)
#define   NFP_CRYPTOX_CIFErrFrst3_CPTCppDta                  BIT(3)
#define   NFP_CRYPTOX_CIFErrFrst3_CmdErr                     BIT(2)
#define   NFP_CRYPTOX_CIFErrFrst3_CBGErr(_x)                 (((_x) & 0x3) << 0)
#define   NFP_CRYPTOX_CIFErrFrst3_CBGErr_of(_x)              (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: CIFErr1 - Set bits in this register will generate events if not masked. Use CIFErrClear1 to clear.
    Bits: 29:18 CDSError(ro) - CDS Error. See CIFErrFrst1 register description.
    Bits: 17:16 CBGError1(ro) - CBG Error. See CIFErrFrst1 register description.
    Bits: 13:8  CBGError0(ro) - CBG Error. See CIFErrFrst1 register description.
    Bits:  5:4  CDCError(ro) - CDC Error. See CIFErrFrst1 register description.
    Bits:  3:0  CPMError(ro) - CPM Error. See CIFErrFrst1 register description.
*/
#define NFP_CRYPTOX_CIFErr1                                  0x00000020
#define   NFP_CRYPTOX_CIFErr1_CDSError(_x)                   (((_x) & 0xfff) << 18)
#define   NFP_CRYPTOX_CIFErr1_CDSError_of(_x)                (((_x) >> 18) & 0xfff)
#define   NFP_CRYPTOX_CIFErr1_CBGError1(_x)                  (((_x) & 0x3) << 16)
#define   NFP_CRYPTOX_CIFErr1_CBGError1_of(_x)               (((_x) >> 16) & 0x3)
#define   NFP_CRYPTOX_CIFErr1_CBGError0(_x)                  (((_x) & 0x3f) << 8)
#define   NFP_CRYPTOX_CIFErr1_CBGError0_of(_x)               (((_x) >> 8) & 0x3f)
#define   NFP_CRYPTOX_CIFErr1_CDCError(_x)                   (((_x) & 0x3) << 4)
#define   NFP_CRYPTOX_CIFErr1_CDCError_of(_x)                (((_x) >> 4) & 0x3)
#define   NFP_CRYPTOX_CIFErr1_CPMError(_x)                   (((_x) & 0xf) << 0)
#define   NFP_CRYPTOX_CIFErr1_CPMError_of(_x)                (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: CIFErr2 - Set bits in this register will generate events if not masked. Use CIFErrClear2 to clear.
    Bits: 31:16 CntxtUndflw(ro) - 31 = Context 15 Queue underflow ... 16 = Context 0 Queue underflow
    Bits: 15:0  CntxtOvrflw(ro) - 15 = Context 15 Queue overflow ... 0 = Context 0 Queue overflow
*/
#define NFP_CRYPTOX_CIFErr2                                  0x00000024
#define   NFP_CRYPTOX_CIFErr2_CntxtUndflw(_x)                (((_x) & 0xffff) << 16)
#define   NFP_CRYPTOX_CIFErr2_CntxtUndflw_of(_x)             (((_x) >> 16) & 0xffff)
#define   NFP_CRYPTOX_CIFErr2_CntxtOvrflw(_x)                (((_x) & 0xffff) << 0)
#define   NFP_CRYPTOX_CIFErr2_CntxtOvrflw_of(_x)             (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: CIFErr3 - Set bits in this register will generate events if not masked. Use CIFErrClear3 to clear.
    Bits: 11    BlkSyncErr(ro) - Crypto Dispatcher Request Sync Error. When set, the dispatcher and bulk have permanently lost sync.
    Bits: 10    DecErr(ro) - Crypto Dispatcher instruction decode error. When set, an invalid instruction was popped from a context queue.
    Bits:  9    CPTPsUflw(ro) - CPP Target fault. PS FIFO Underflow.
    Bits:  8    CPTPsOflw(ro) - CPP Target fault. PS FIFO Overflow.
    Bits:  7    CPTDnUflw(ro) - CPP Target fault. DN FIFO Underflow.
    Bits:  6    CPTDnOflw(ro) - CPP Target fault. DN FIFO Overflow.
    Bits:  5    CPTPhUflw(ro) - CPP Target fault. PH FIFO Underflow.
    Bits:  4    CPTPhOflw(ro) - CPP Target fault. PH FIFO Overflow.
    Bits:  3    CPTCppDta(ro) - CPP Target fault. CPP Data Error.
    Bits:  2    CmdErr(ro) - CPP Target command error (Causes dispatcher shutdown)
    Bits:  1:0  CBGErr(ro) - Buffer Gasket multi-bit ECC error (Causes dispatcher shutdown)
*/
#define NFP_CRYPTOX_CIFErr3                                  0x00000028
#define   NFP_CRYPTOX_CIFErr3_BlkSyncErr                     BIT(11)
#define   NFP_CRYPTOX_CIFErr3_DecErr                         BIT(10)
#define   NFP_CRYPTOX_CIFErr3_CPTPsUflw                      BIT(9)
#define   NFP_CRYPTOX_CIFErr3_CPTPsOflw                      BIT(8)
#define   NFP_CRYPTOX_CIFErr3_CPTDnUflw                      BIT(7)
#define   NFP_CRYPTOX_CIFErr3_CPTDnOflw                      BIT(6)
#define   NFP_CRYPTOX_CIFErr3_CPTPhUflw                      BIT(5)
#define   NFP_CRYPTOX_CIFErr3_CPTPhOflw                      BIT(4)
#define   NFP_CRYPTOX_CIFErr3_CPTCppDta                      BIT(3)
#define   NFP_CRYPTOX_CIFErr3_CmdErr                         BIT(2)
#define   NFP_CRYPTOX_CIFErr3_CBGErr(_x)                     (((_x) & 0x3) << 0)
#define   NFP_CRYPTOX_CIFErr3_CBGErr_of(_x)                  (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: CIFErrMask1 - Error mask enables. If set, the corresponding bit in the CIFErr1 register will generate an event.
    Bits: 31:30 Reserved_U(rw) - Reserved
    Bits: 29:18 CDSErRWr(rw) - Masks CDS ErRWr. See CIFErrFrst1 register description.
    Bits: 17:16 CBGErRWr1(rw) - Masks CBG ErRWr. See CIFErrFrst1 register description.
    Bits: 15:14 Reserved_M(rw) - Reserved
    Bits: 13:8  CBGErRWr(rw) - Masks CBG ErRWr. See CIFErrFrst1 register description.
    Bits:  7:6  Reserved_L(rw) - Reserved
    Bits:  5:4  CDCErRWr(rw) - Masks CDC ErRWr. See CIFErrFrst1 register description.
    Bits:  3:0  CPMErRWr(rw) - Masks CPM Error. See CIFErrFrst1 register description.
*/
#define NFP_CRYPTOX_CIFErrMask1                              0x00000030
#define   NFP_CRYPTOX_CIFErrMask1_Reserved_U(_x)             (((_x) & 0x3) << 30)
#define   NFP_CRYPTOX_CIFErrMask1_Reserved_U_of(_x)          (((_x) >> 30) & 0x3)
#define   NFP_CRYPTOX_CIFErrMask1_CDSErRWr(_x)               (((_x) & 0xfff) << 18)
#define   NFP_CRYPTOX_CIFErrMask1_CDSErRWr_of(_x)            (((_x) >> 18) & 0xfff)
#define   NFP_CRYPTOX_CIFErrMask1_CBGErRWr1(_x)              (((_x) & 0x3) << 16)
#define   NFP_CRYPTOX_CIFErrMask1_CBGErRWr1_of(_x)           (((_x) >> 16) & 0x3)
#define   NFP_CRYPTOX_CIFErrMask1_Reserved_M(_x)             (((_x) & 0x3) << 14)
#define   NFP_CRYPTOX_CIFErrMask1_Reserved_M_of(_x)          (((_x) >> 14) & 0x3)
#define   NFP_CRYPTOX_CIFErrMask1_CBGErRWr(_x)               (((_x) & 0x3f) << 8)
#define   NFP_CRYPTOX_CIFErrMask1_CBGErRWr_of(_x)            (((_x) >> 8) & 0x3f)
#define   NFP_CRYPTOX_CIFErrMask1_Reserved_L(_x)             (((_x) & 0x3) << 6)
#define   NFP_CRYPTOX_CIFErrMask1_Reserved_L_of(_x)          (((_x) >> 6) & 0x3)
#define   NFP_CRYPTOX_CIFErrMask1_CDCErRWr(_x)               (((_x) & 0x3) << 4)
#define   NFP_CRYPTOX_CIFErrMask1_CDCErRWr_of(_x)            (((_x) >> 4) & 0x3)
#define   NFP_CRYPTOX_CIFErrMask1_CPMErRWr(_x)               (((_x) & 0xf) << 0)
#define   NFP_CRYPTOX_CIFErrMask1_CPMErRWr_of(_x)            (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: CIFErrMask2 - Error mask enables. If set, the corresponding bit in the CIFErr2 register will generate an event.
    Bits: 31:16 CntxtUndflw(rw) - Masks Error: 31 = Context 15 Queue underflow ... 16 = Context 0 Queue underflow
    Bits: 15:0  CntxtOvrflw(rw) - Masks Error: 15 = Context 15 Queue overflow ... 0 = Context 0 Queue overflow
*/
#define NFP_CRYPTOX_CIFErrMask2                              0x00000034
#define   NFP_CRYPTOX_CIFErrMask2_CntxtUndflw(_x)            (((_x) & 0xffff) << 16)
#define   NFP_CRYPTOX_CIFErrMask2_CntxtUndflw_of(_x)         (((_x) >> 16) & 0xffff)
#define   NFP_CRYPTOX_CIFErrMask2_CntxtOvrflw(_x)            (((_x) & 0xffff) << 0)
#define   NFP_CRYPTOX_CIFErrMask2_CntxtOvrflw_of(_x)         (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: CIFErrMask3 - Error mask enables. If set, the corresponding bit in the CIFErr3 register will generate an event.
    Bits: 31:12 Reserved(rw) - Reserved
    Bits: 11    BlkSyncErr(rw) - Masks Error: Crypto Dispatcher Request Sync Error.
    Bits: 10    DecErr(rw) - Masks Error: Crypto Dispatcher instruction decode error.
    Bits:  9    CPTPsUflw(rw) - Masks Error: CPP Target fault. PS FIFO Underflow.
    Bits:  8    CPTPsOflw(rw) - Masks Error: CPP Target fault. PS FIFO Overflow.
    Bits:  7    CPTDnUflw(rw) - Masks Error: CPP Target fault. DN FIFO Underflow.
    Bits:  6    CPTDnOflw(rw) - Masks Error: CPP Target fault. DN FIFO Overflow.
    Bits:  5    CPTPhUflw(rw) - Masks Error: CPP Target fault. PH FIFO Underflow.
    Bits:  4    CPTPhOflw(rw) - Masks Error: CPP Target fault. PH FIFO Overflow.
    Bits:  3    CPTCppDta(rw) - Masks Error: CPP Target fault. CPP Data Error.
    Bits:  2    CmdErr(rw) - Masks Error: CPP Target command error (Causes dispatcher shutdown)
    Bits:  1:0  CBGErr(rw) - Masks Error: Buffer Gasket multi-bit ECC error (Causes dispatcher shutdown)
*/
#define NFP_CRYPTOX_CIFErrMask3                              0x00000038
#define   NFP_CRYPTOX_CIFErrMask3_Reserved(_x)               (((_x) & 0xfffff) << 12)
#define   NFP_CRYPTOX_CIFErrMask3_Reserved_of(_x)            (((_x) >> 12) & 0xfffff)
#define   NFP_CRYPTOX_CIFErrMask3_BlkSyncErr                 BIT(11)
#define   NFP_CRYPTOX_CIFErrMask3_DecErr                     BIT(10)
#define   NFP_CRYPTOX_CIFErrMask3_CPTPsUflw                  BIT(9)
#define   NFP_CRYPTOX_CIFErrMask3_CPTPsOflw                  BIT(8)
#define   NFP_CRYPTOX_CIFErrMask3_CPTDnUflw                  BIT(7)
#define   NFP_CRYPTOX_CIFErrMask3_CPTDnOflw                  BIT(6)
#define   NFP_CRYPTOX_CIFErrMask3_CPTPhUflw                  BIT(5)
#define   NFP_CRYPTOX_CIFErrMask3_CPTPhOflw                  BIT(4)
#define   NFP_CRYPTOX_CIFErrMask3_CPTCppDta                  BIT(3)
#define   NFP_CRYPTOX_CIFErrMask3_CmdErr                     BIT(2)
#define   NFP_CRYPTOX_CIFErrMask3_CBGErr(_x)                 (((_x) & 0x3) << 0)
#define   NFP_CRYPTOX_CIFErrMask3_CBGErr_of(_x)              (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: CIFErrClear1 - Clears both the CIFErrFrst1 and CIFErr1 registers on a per bit basis. Write 1 to clear.
    Bits: 29:18 CDSError(w1c) - Clears CDS Error. See CIFErrFrst1 register description.
    Bits: 17:16 CBGError1(w1c) - Clears CBG Error. See CIFErrFrst1 register description.
    Bits: 13:8  CBGError(w1c) - Clears CBG Error. See CIFErrFrst1 register description.
    Bits:  5:4  CDCError(w1c) - Clears CDC Error. See CIFErrFrst1 register description.
    Bits:  3:0  CPMError(w1c) - Clears CPM Error. See CIFErrFrst1 register description.
*/
#define NFP_CRYPTOX_CIFErrClear1                             0x00000040
#define   NFP_CRYPTOX_CIFErrClear1_CDSError(_x)              (((_x) & 0xfff) << 18)
#define   NFP_CRYPTOX_CIFErrClear1_CDSError_of(_x)           (((_x) >> 18) & 0xfff)
#define   NFP_CRYPTOX_CIFErrClear1_CBGError1(_x)             (((_x) & 0x3) << 16)
#define   NFP_CRYPTOX_CIFErrClear1_CBGError1_of(_x)          (((_x) >> 16) & 0x3)
#define   NFP_CRYPTOX_CIFErrClear1_CBGError(_x)              (((_x) & 0x3f) << 8)
#define   NFP_CRYPTOX_CIFErrClear1_CBGError_of(_x)           (((_x) >> 8) & 0x3f)
#define   NFP_CRYPTOX_CIFErrClear1_CDCError(_x)              (((_x) & 0x3) << 4)
#define   NFP_CRYPTOX_CIFErrClear1_CDCError_of(_x)           (((_x) >> 4) & 0x3)
#define   NFP_CRYPTOX_CIFErrClear1_CPMError(_x)              (((_x) & 0xf) << 0)
#define   NFP_CRYPTOX_CIFErrClear1_CPMError_of(_x)           (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: CIFErrClear2 - Clears both the CIFErrFrst2 and CIFErr2 registers on a per bit basis. Write 1 to clear.
    Bits: 31:16 CntxtUndflw(w1c) - Clears Error: 31 = Context 15 Queue underflow ... 16 = Context 0 Queue underflow
    Bits: 15:0  CntxtOvrflw(w1c) - Clears Error: 15 = Context 15 Queue overflow ... 0 = Context 0 Queue overflow
*/
#define NFP_CRYPTOX_CIFErrClear2                             0x00000044
#define   NFP_CRYPTOX_CIFErrClear2_CntxtUndflw(_x)           (((_x) & 0xffff) << 16)
#define   NFP_CRYPTOX_CIFErrClear2_CntxtUndflw_of(_x)        (((_x) >> 16) & 0xffff)
#define   NFP_CRYPTOX_CIFErrClear2_CntxtOvrflw(_x)           (((_x) & 0xffff) << 0)
#define   NFP_CRYPTOX_CIFErrClear2_CntxtOvrflw_of(_x)        (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: CIFErrClear3 - Clears both the CIFErrFrst3 and CIFErr3 registers on a per bit basis. Write 1 to clear.
    Bits: 11    BlkSyncErr(w1c) - Clears Error: Crypto Dispatcher Request Sync Error.
    Bits: 10    DecErr(w1c) - Clears Error: Crypto Dispatcher instruction decode error.
    Bits:  9    CPTPsUflw(w1c) - Clears Error: CPP Target fault. PS FIFO Underflow.
    Bits:  8    CPTPsOflw(w1c) - Clears Error: CPP Target fault. PS FIFO Overflow.
    Bits:  7    CPTDnUflw(w1c) - Clears Error: CPP Target fault. DN FIFO Underflow.
    Bits:  6    CPTDnOflw(w1c) - Clears Error: CPP Target fault. DN FIFO Overflow.
    Bits:  5    CPTPhUflw(w1c) - Clears Error: CPP Target fault. PH FIFO Underflow.
    Bits:  4    CPTPhOflw(w1c) - Clears Error: CPP Target fault. PH FIFO Overflow.
    Bits:  3    CPTCppDta(w1c) - Clears Error: CPP Target fault. CPP Data Error.
    Bits:  2    CmdErr(w1c) - Clears Error: CPP Target command error (Causes dispatcher shutdown)
    Bits:  1:0  CBGErr(w1c) - Clears Error: Buffer Gasket multi-bit ECC error (Causes dispatcher shutdown)
*/
#define NFP_CRYPTOX_CIFErrClear3                             0x00000048
#define   NFP_CRYPTOX_CIFErrClear3_BlkSyncErr                BIT(11)
#define   NFP_CRYPTOX_CIFErrClear3_DecErr                    BIT(10)
#define   NFP_CRYPTOX_CIFErrClear3_CPTPsUflw                 BIT(9)
#define   NFP_CRYPTOX_CIFErrClear3_CPTPsOflw                 BIT(8)
#define   NFP_CRYPTOX_CIFErrClear3_CPTDnUflw                 BIT(7)
#define   NFP_CRYPTOX_CIFErrClear3_CPTDnOflw                 BIT(6)
#define   NFP_CRYPTOX_CIFErrClear3_CPTPhUflw                 BIT(5)
#define   NFP_CRYPTOX_CIFErrClear3_CPTPhOflw                 BIT(4)
#define   NFP_CRYPTOX_CIFErrClear3_CPTCppDta                 BIT(3)
#define   NFP_CRYPTOX_CIFErrClear3_CmdErr                    BIT(2)
#define   NFP_CRYPTOX_CIFErrClear3_CBGErr(_x)                (((_x) & 0x3) << 0)
#define   NFP_CRYPTOX_CIFErrClear3_CBGErr_of(_x)             (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: CIFDbgRdCtrl - Control debug reads of context queues or DMA channel state.
    Bits: 31    Busy(rw) - Write 1 to kick off the debug read. Logic will clear this bit after the data is written to the crypto buffer.
    Bits: 16    Source(rw) - Source of the read data
    Bits:  7:0  Count(rw) - Number of 16B words to read
*/
#define NFP_CRYPTOX_CIFDbgRdCtrl                             0x00000054
#define   NFP_CRYPTOX_CIFDbgRdCtrl_Busy                      BIT(31)
#define     NFP_CRYPTOX_CIFDbgRdCtrl_Busy_Reading            BIT(31)
#define   NFP_CRYPTOX_CIFDbgRdCtrl_Source                    BIT(16)
#define     NFP_CRYPTOX_CIFDbgRdCtrl_Source_Dispatcher       (0 << 16)
#define   NFP_CRYPTOX_CIFDbgRdCtrl_Count(_x)                 (((_x) & 0xff) << 0)
#define   NFP_CRYPTOX_CIFDbgRdCtrl_Count_of(_x)              (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: CIFDbgRdAddr - Address control for debug reads.
    Bits: 28:16 CryptoAddr(rw) - Starting address of the crypto buffer. 16B read debug words will be written to consecutive addresses starting at this one.
    Bits: 15    CtxtInc(rw) - Valid only if Source = 0:
    Bits:  7:0  SrcAddr(rw) - Starting address for the read source: For Source = 0 (Dispatcher): [7:4] = 16B entry pointer, [3:0] = context ID; For Source = 1 (DMA Controller): [4:0] = DMA channel
*/
#define NFP_CRYPTOX_CIFDbgRdAddr                             0x00000058
#define   NFP_CRYPTOX_CIFDbgRdAddr_CryptoAddr(_x)            (((_x) & 0x1fff) << 16)
#define   NFP_CRYPTOX_CIFDbgRdAddr_CryptoAddr_of(_x)         (((_x) >> 16) & 0x1fff)
#define   NFP_CRYPTOX_CIFDbgRdAddr_CtxtInc                   BIT(15)
#define   NFP_CRYPTOX_CIFDbgRdAddr_SrcAddr(_x)               (((_x) & 0xff) << 0)
#define   NFP_CRYPTOX_CIFDbgRdAddr_SrcAddr_of(_x)            (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: BasicConfig - Basic Configuration Register
    Bits:  7:6  DesMCP(rw) - Multicycle path for DES algorithm. The path is designed for 5 cycles with a 700MHz Crypto_Bulk block clock. This value may be adjusted to improve performance or algorithm stability when using other clock frequencies.
    Bits:  5:0  Enable(rw) - per bit enable for each bulk crypt core. '1' to enable. There must be at least one bulk crypt core enabled to activate the Bulk Crypt Director (interface to the bulk crypt block). Activating an core that is already active has no effect. It is the transition of the bit value that performs an action.
*/
#define NFP_CRYPTOX_BULK_BasicConfig                         0x00000000
#define   NFP_CRYPTOX_BULK_BasicConfig_DesMCP(_x)            (((_x) & 0x3) << 6)
#define   NFP_CRYPTOX_BULK_BasicConfig_DesMCP_of(_x)         (((_x) >> 6) & 0x3)
#define   NFP_CRYPTOX_BULK_BasicConfig_Enable(_x)            (((_x) & 0x3f) << 0)
#define   NFP_CRYPTOX_BULK_BasicConfig_Enable_of(_x)         (((_x) >> 0) & 0x3f)
/*----------------------------------------------------------------
  Register: WtchDgTmrCnf - Watch dog timer configuration
    Bits: 31    Clear(rw) - Set to '1' to clear the watchdog timers.
    Bits: 30    Enable(rw) - Set to '1' to enable the watchdog timers.
    Bits: 29:28 Size(rw) - Counter Size
    Bits: 27:0  Trip(rw) - Value for counter to trigger an event. This field maps to the most significant bits in the counter. For counter sizes larger than 28 bits, the least significant bits of the trip vector are zeros.
*/
#define NFP_CRYPTOX_BULK_WtchDgTmrCnf                        0x00000008
#define   NFP_CRYPTOX_BULK_WtchDgTmrCnf_Clear                BIT(31)
#define   NFP_CRYPTOX_BULK_WtchDgTmrCnf_Enable               BIT(30)
#define   NFP_CRYPTOX_BULK_WtchDgTmrCnf_Size(_x)             (((_x) & 0x3) << 28)
#define   NFP_CRYPTOX_BULK_WtchDgTmrCnf_Size_of(_x)          (((_x) >> 28) & 0x3)
#define   NFP_CRYPTOX_BULK_WtchDgTmrCnf_Trip(_x)             (((_x) & 0xfffffff) << 0)
#define   NFP_CRYPTOX_BULK_WtchDgTmrCnf_Trip_of(_x)          (((_x) >> 0) & 0xfffffff)
/*----------------------------------------------------------------
  Register: ErrorReg - Error Register. Reading this register will show all errors that have occurred regardless of mask value. Writing a '1' to a bit will clear the appropriate bit in this register and the "who's on first" register.
    Bits: 22    EventBulk5(rw1c) - Event/Error was for a Bulk_Crypto 5
    Bits: 21    EventBulk4(rw1c) - Event/Error was for a Bulk_Crypto 4
    Bits: 20    EventBulk3(rw1c) - Event/Error was for a Bulk_Crypto 3
    Bits: 19    EventBulk2(rw1c) - Event/Error was for a Bulk_Crypto 2
    Bits: 18    EventBulk1(rw1c) - Event/Error was for a Bulk_Crypto 1
    Bits: 17    EventBulk0(rw1c) - Event/Error was for a Bulk_Crypto 0
    Bits: 16    SeqnumErr(rw1c) - SRAM controller mismatch or read data error
    Bits: 15    WrtFifoPty(rw1c) - fifo output parity error.
    Bits: 14    WrtFifoErr(rw1c) - fifo overflow or has a 'hole'/missing location
    Bits: 13    WrtArbErr(rw1c) - a hot arb has multiple/no states active
    Bits: 12    AllocWdog5(rw1c) - watch-dog for Bulk_Crypto 5 tripped
    Bits: 11    AllocWdog4(rw1c) - watch-dog for Bulk_Crypto 4 tripped
    Bits: 10    AllocWdog3(rw1c) - watch-dog for Bulk_Crypto 3 tripped
    Bits:  9    AllocWdog2(rw1c) - watch-dog for Bulk_Crypto 2 tripped
    Bits:  8    AllocWdog1(rw1c) - watch-dog for Bulk_Crypto 1 tripped
    Bits:  7    AllocWdog0(rw1c) - watch-dog for Bulk_Crypto 0 tripped
    Bits:  6    AllocFifoPerr(rw1c) - parity error on allocator get() queue memory
    Bits:  5    AllocMemPerr(rw1c) - parity error on allocator context memory
    Bits:  4    AllocBusyErr(rw1c) - new instruction arrives while state indicates busy or instruction completion arrives while state indicates not_busy.
    Bits:  3    AllocGcntErr(rw1c) - get() context id queue exceeds depth of 12.
    Bits:  2    AllocFreeErr(rw1c) - context id for free() instruction has not been allocated
    Bits:  1    AllocGetErr(rw1c) - context id for get() instructions has already been allocated
    Bits:  0    AllocChkErr(rw1c) - context id on instruction has not been allocated
*/
#define NFP_CRYPTOX_BULK_ErrorReg                            0x00000010
#define   NFP_CRYPTOX_BULK_ErrorReg_EventBulk5               BIT(22)
#define   NFP_CRYPTOX_BULK_ErrorReg_EventBulk4               BIT(21)
#define   NFP_CRYPTOX_BULK_ErrorReg_EventBulk3               BIT(20)
#define   NFP_CRYPTOX_BULK_ErrorReg_EventBulk2               BIT(19)
#define   NFP_CRYPTOX_BULK_ErrorReg_EventBulk1               BIT(18)
#define   NFP_CRYPTOX_BULK_ErrorReg_EventBulk0               BIT(17)
#define   NFP_CRYPTOX_BULK_ErrorReg_SeqnumErr                BIT(16)
#define   NFP_CRYPTOX_BULK_ErrorReg_WrtFifoPty               BIT(15)
#define   NFP_CRYPTOX_BULK_ErrorReg_WrtFifoErr               BIT(14)
#define   NFP_CRYPTOX_BULK_ErrorReg_WrtArbErr                BIT(13)
#define   NFP_CRYPTOX_BULK_ErrorReg_AllocWdog5               BIT(12)
#define   NFP_CRYPTOX_BULK_ErrorReg_AllocWdog4               BIT(11)
#define   NFP_CRYPTOX_BULK_ErrorReg_AllocWdog3               BIT(10)
#define   NFP_CRYPTOX_BULK_ErrorReg_AllocWdog2               BIT(9)
#define   NFP_CRYPTOX_BULK_ErrorReg_AllocWdog1               BIT(8)
#define   NFP_CRYPTOX_BULK_ErrorReg_AllocWdog0               BIT(7)
#define   NFP_CRYPTOX_BULK_ErrorReg_AllocFifoPerr            BIT(6)
#define   NFP_CRYPTOX_BULK_ErrorReg_AllocMemPerr             BIT(5)
#define   NFP_CRYPTOX_BULK_ErrorReg_AllocBusyErr             BIT(4)
#define   NFP_CRYPTOX_BULK_ErrorReg_AllocGcntErr             BIT(3)
#define   NFP_CRYPTOX_BULK_ErrorReg_AllocFreeErr             BIT(2)
#define   NFP_CRYPTOX_BULK_ErrorReg_AllocGetErr              BIT(1)
#define   NFP_CRYPTOX_BULK_ErrorReg_AllocChkErr              BIT(0)
/*----------------------------------------------------------------
  Register: WhsOnFrstErr - "Who's on first" register. Each bit maps directly to a bit in the Error Register. This register captures the first error bit which is selected by the mask register, blocking future error bits into this register until the register has been cleared. This register is used to determine which error happened first if multiple errors are indicated in the error register. Writing a '1' to a bit will clear the appropriate bit in this register.
    Bits: 22:0  WhsOnFrstErr(rw1c) - Same bitmap as ErrorReg
*/
#define NFP_CRYPTOX_BULK_WhsOnFrstErr                        0x00000014
#define   NFP_CRYPTOX_BULK_WhsOnFrstErr_WhsOnFrstErr(_x)     (((_x) & 0x7fffff) << 0)
#define   NFP_CRYPTOX_BULK_WhsOnFrstErr_WhsOnFrstErr_of(_x)  (((_x) >> 0) & 0x7fffff)
/*----------------------------------------------------------------
  Register: ErrorMask - Error Mask. Each bit maps directly to a bit in the Error Register. This mask is used with the "Who's on first" register and the Clock Stop Mask. Setting a bit position to a '1' will enable the associated error to be detected.
    Bits: 22:0  ErrorMask(rw) - Same bitmap as ErrorReg
*/
#define NFP_CRYPTOX_BULK_ErrorMask                           0x00000018
#define   NFP_CRYPTOX_BULK_ErrorMask_ErrorMask(_x)           (((_x) & 0x7fffff) << 0)
#define   NFP_CRYPTOX_BULK_ErrorMask_ErrorMask_of(_x)        (((_x) >> 0) & 0x7fffff)
/*----------------------------------------------------------------
  Register: ClkStopMask - Clock Stop Mask. This register is used along with the Error Mask register to select which error bits will trigger a local clock stop. Setting a bit position to a '1' will enable the associated error to cause a local clock stop.
    Bits: 22:0  ClkStopMask(rw) - Same bitmap as ErrorReg
*/
#define NFP_CRYPTOX_BULK_ClkStopMask                         0x0000001c
#define   NFP_CRYPTOX_BULK_ClkStopMask_ClkStopMask(_x)       (((_x) & 0x7fffff) << 0)
#define   NFP_CRYPTOX_BULK_ClkStopMask_ClkStopMask_of(_x)    (((_x) >> 0) & 0x7fffff)
/*----------------------------------------------------------------
  Register: TestMode - Test mode / Error inject. This address is used to send single cycle pulses to test the error detect registers.
    Bits: 31:24 Target(ro) - Select target logic to send error inject pulse.
    Bits: 21:0  Data(ro) - Set specific bit(s) to '1' to inject error pulse. Bit breakout depends on target value selected in Target Field.
*/
#define NFP_CRYPTOX_BULK_TestMode                            0x00000020
#define   NFP_CRYPTOX_BULK_TestMode_Target(_x)               (((_x) & 0xff) << 24)
#define   NFP_CRYPTOX_BULK_TestMode_Target_of(_x)            (((_x) >> 24) & 0xff)
#define   NFP_CRYPTOX_BULK_TestMode_Data(_x)                 (((_x) & 0x3fffff) << 0)
#define   NFP_CRYPTOX_BULK_TestMode_Data_of(_x)              (((_x) >> 0) & 0x3fffff)
/*----------------------------------------------------------------
  Register: ErrRegLo - Error register low bits [31:0]. Reading this register will show all errors that have occurred regardless of mask value. Writing a '1' to a bit will clear the appropriate bit in this register and the "who's on first" Lo register.
    Bits: 29    KasumiCfgPty(w1c) - Config data from 4B path has parity error.
    Bits: 28    KasumiCfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits: 27    Snow3GCfgPty(w1c) - Config data from 4B path has parity error.
    Bits: 26    Snow3GCfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits: 25    CiphWrtPser(w1c) - Serial path fifo parity error.
    Bits: 24    CiphWrtFser(w1c) - Fifo overflow.
    Bits: 23    CiphWrtFdat(w1c) - Fifo overflow.
    Bits: 22    Arc4RdPty(w1c) - Ready data from state ram has parity error.
    Bits: 21    Arc4CfgPty(w1c) - Config data from 4B path has parity error or key read parity.
    Bits: 20    Arc4CfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits: 19    DesFifoFdm(w1c) - Fifo overflow ( mask ).
    Bits: 18    DesFifoFdf(w1c) - Fifo overflow ( flag ).
    Bits: 17    DesFifoFdo(w1c) - Fifo overflow ( data out ).
    Bits: 16    DesFifoFdi(w1c) - Fifo overflow ( data in ).
    Bits: 15    DesCfgPty(w1c) - Config data from 4B path has parity error.
    Bits: 14    DesCfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits: 13    AesFifoGcmx(w1c) - Fifo overflow ( flag ).
    Bits: 12    AesFifoGcm(w1c) - Fifo overflow ( data ).
    Bits: 11    AesFifoFdm(w1c) - Fifo overflow ( mask ).
    Bits: 10    AesFifoFdf(w1c) - aes_fifo_fdf - fifo overflow ( flag ).
    Bits:  9    AesFifoFdo(w1c) - Fifo overflow ( data out ).
    Bits:  8    AesFifoFdi(w1c) - Fifo overflow ( data in ).
    Bits:  7    AesKeyRpty(w1c) - Key read from ram has parity error.
    Bits:  6    AesKeyIpty(w1c) - Input key data from 4B path has parity error.
    Bits:  5    AesKeySize(w1c) - Input key data from 4B path has size or sequence errors.
    Bits:  4    AesCfgPty(w1c) - Config data from 4B path has parity error.
    Bits:  3    AesCfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits:  2    CpcCpkHole(w1c) - Packing function encountered hole in byte-stream.
    Bits:  1    CpcFifoOv(w1c) - Fifo overflow.
    Bits:  0    CpcFifoParity(w1c) - Parity error on fifo output (data, flags, parity)
*/
#define NFP_CRYPTOX_CORE_ErrRegLo                            0x00000000
#define   NFP_CRYPTOX_CORE_ErrRegLo_KasumiCfgPty             BIT(29)
#define   NFP_CRYPTOX_CORE_ErrRegLo_KasumiCfgSize            BIT(28)
#define   NFP_CRYPTOX_CORE_ErrRegLo_Snow3GCfgPty             BIT(27)
#define   NFP_CRYPTOX_CORE_ErrRegLo_Snow3GCfgSize            BIT(26)
#define   NFP_CRYPTOX_CORE_ErrRegLo_CiphWrtPser              BIT(25)
#define   NFP_CRYPTOX_CORE_ErrRegLo_CiphWrtFser              BIT(24)
#define   NFP_CRYPTOX_CORE_ErrRegLo_CiphWrtFdat              BIT(23)
#define   NFP_CRYPTOX_CORE_ErrRegLo_Arc4RdPty                BIT(22)
#define   NFP_CRYPTOX_CORE_ErrRegLo_Arc4CfgPty               BIT(21)
#define   NFP_CRYPTOX_CORE_ErrRegLo_Arc4CfgSize              BIT(20)
#define   NFP_CRYPTOX_CORE_ErrRegLo_DesFifoFdm               BIT(19)
#define   NFP_CRYPTOX_CORE_ErrRegLo_DesFifoFdf               BIT(18)
#define   NFP_CRYPTOX_CORE_ErrRegLo_DesFifoFdo               BIT(17)
#define   NFP_CRYPTOX_CORE_ErrRegLo_DesFifoFdi               BIT(16)
#define   NFP_CRYPTOX_CORE_ErrRegLo_DesCfgPty                BIT(15)
#define   NFP_CRYPTOX_CORE_ErrRegLo_DesCfgSize               BIT(14)
#define   NFP_CRYPTOX_CORE_ErrRegLo_AesFifoGcmx              BIT(13)
#define   NFP_CRYPTOX_CORE_ErrRegLo_AesFifoGcm               BIT(12)
#define   NFP_CRYPTOX_CORE_ErrRegLo_AesFifoFdm               BIT(11)
#define   NFP_CRYPTOX_CORE_ErrRegLo_AesFifoFdf               BIT(10)
#define   NFP_CRYPTOX_CORE_ErrRegLo_AesFifoFdo               BIT(9)
#define   NFP_CRYPTOX_CORE_ErrRegLo_AesFifoFdi               BIT(8)
#define   NFP_CRYPTOX_CORE_ErrRegLo_AesKeyRpty               BIT(7)
#define   NFP_CRYPTOX_CORE_ErrRegLo_AesKeyIpty               BIT(6)
#define   NFP_CRYPTOX_CORE_ErrRegLo_AesKeySize               BIT(5)
#define   NFP_CRYPTOX_CORE_ErrRegLo_AesCfgPty                BIT(4)
#define   NFP_CRYPTOX_CORE_ErrRegLo_AesCfgSize               BIT(3)
#define   NFP_CRYPTOX_CORE_ErrRegLo_CpcCpkHole               BIT(2)
#define   NFP_CRYPTOX_CORE_ErrRegLo_CpcFifoOv                BIT(1)
#define   NFP_CRYPTOX_CORE_ErrRegLo_CpcFifoParity            BIT(0)
/*----------------------------------------------------------------
  Register: ErrRegHi - Error register high bits [63:32]. Reading this register will show all errors that have occurred regardless of mask value. Writing a '1' to a bit will clear the appropriate bit in this register and the "who's on first" Hi register.
    Bits: 27    CrcMemErr(w1c) - Read data error from SRAM controller (seq-num or RERR).
    Bits: 26    CrcIcfCmd(w1c) - Compressed instruction collision with normal instruction.
    Bits: 25    CpsFlag(w1c) - Flag/type mismatch on input data.
    Bits: 24    CpsCpkHole(w1c) - Packing function encountered hole in byte-stream.
    Bits: 23    CdiFifoPty(w1c) - Fifo output parity error.
    Bits: 22    CdiFifoOv(w1c) - Fifo overflow.
    Bits: 21    IcfPtr1(w1c) - Parity error on pointer_1 read.
    Bits: 20    IcfPtr0(w1c) - Parity error on pointer_0 read.
    Bits: 19    IcfData(w1c) - Parity error on instruction read.
    Bits: 18    RdrBusyErr(w1c) - New request while still busy or clear request when not busy.
    Bits: 17    RdrFifoPty(w1c) - Fifo output parity error.
    Bits: 16    RdrFifoOv(w1c) - Fifo overflow.
    Bits: 11    CRC16TopFlag(w1c) - Input data has flag/type mismatch.
    Bits: 10    CRC16CfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits:  9    CRC16CfgPty(w1c) - Config data from 4B path has parity error.
    Bits:  8    HashWrtFdat(w1c) - Fifo overflow.
    Bits:  7    ShaCfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits:  6    ShaCfgPty(w1c) - Config data from 4B path has parity error.
    Bits:  5    GfTopFlag(w1c) - Input data has flag/type mismatch.
    Bits:  4    GfCfgSize(w1c) - Config data from 4B path has size or sequence errors.
    Bits:  3    GfCfgPty(w1c) - Config data from 4B path has parity error.
    Bits:  2    CphCpkHole(w1c) - Packing function encountered hole in byte-stream.
    Bits:  1    CphFifoOv(w1c) - Fifo overflow.
    Bits:  0    CphFifoPrty(w1c) - Parity error on fifo output (data, flags, parity).
*/
#define NFP_CRYPTOX_CORE_ErrRegHi                            0x00000004
#define   NFP_CRYPTOX_CORE_ErrRegHi_CrcMemErr                BIT(27)
#define   NFP_CRYPTOX_CORE_ErrRegHi_CrcIcfCmd                BIT(26)
#define   NFP_CRYPTOX_CORE_ErrRegHi_CpsFlag                  BIT(25)
#define   NFP_CRYPTOX_CORE_ErrRegHi_CpsCpkHole               BIT(24)
#define   NFP_CRYPTOX_CORE_ErrRegHi_CdiFifoPty               BIT(23)
#define   NFP_CRYPTOX_CORE_ErrRegHi_CdiFifoOv                BIT(22)
#define   NFP_CRYPTOX_CORE_ErrRegHi_IcfPtr1                  BIT(21)
#define   NFP_CRYPTOX_CORE_ErrRegHi_IcfPtr0                  BIT(20)
#define   NFP_CRYPTOX_CORE_ErrRegHi_IcfData                  BIT(19)
#define   NFP_CRYPTOX_CORE_ErrRegHi_RdrBusyErr               BIT(18)
#define   NFP_CRYPTOX_CORE_ErrRegHi_RdrFifoPty               BIT(17)
#define   NFP_CRYPTOX_CORE_ErrRegHi_RdrFifoOv                BIT(16)
#define   NFP_CRYPTOX_CORE_ErrRegHi_CRC16TopFlag             BIT(11)
#define   NFP_CRYPTOX_CORE_ErrRegHi_CRC16CfgSize             BIT(10)
#define   NFP_CRYPTOX_CORE_ErrRegHi_CRC16CfgPty              BIT(9)
#define   NFP_CRYPTOX_CORE_ErrRegHi_HashWrtFdat              BIT(8)
#define   NFP_CRYPTOX_CORE_ErrRegHi_ShaCfgSize               BIT(7)
#define   NFP_CRYPTOX_CORE_ErrRegHi_ShaCfgPty                BIT(6)
#define   NFP_CRYPTOX_CORE_ErrRegHi_GfTopFlag                BIT(5)
#define   NFP_CRYPTOX_CORE_ErrRegHi_GfCfgSize                BIT(4)
#define   NFP_CRYPTOX_CORE_ErrRegHi_GfCfgPty                 BIT(3)
#define   NFP_CRYPTOX_CORE_ErrRegHi_CphCpkHole               BIT(2)
#define   NFP_CRYPTOX_CORE_ErrRegHi_CphFifoOv                BIT(1)
#define   NFP_CRYPTOX_CORE_ErrRegHi_CphFifoPrty              BIT(0)
/*----------------------------------------------------------------
  Register: WhosOnFrstLo - Who's on first error register low bits [31:0]. Each bit maps directly to a bit in the Error Lo Register. This register captures the first error bit which is selected by the Mask Lo register, blocking future error bits into this register until the register has been cleared. This register is used to determine which error happened first if multiple errors are indicated in the error register. Writing a '1' to a bit will clear the appropriate bit in this register.
    Bits: 29:0  WhosOnFrstLo(w1c) - Same bitmap as ErrRegLo.
*/
#define NFP_CRYPTOX_CORE_WhosOnFrstLo                        0x00000008
#define   NFP_CRYPTOX_CORE_WhosOnFrstLo_WhosOnFrstLo(_x)     (((_x) & 0x3fffffff) << 0)
#define   NFP_CRYPTOX_CORE_WhosOnFrstLo_WhosOnFrstLo_of(_x)  (((_x) >> 0) & 0x3fffffff)
/*----------------------------------------------------------------
  Register: WhosOnFrstHi - Who's on first error register high bits [63:32]. Each bit maps directly to a bit in the Error Hi Register. This register captures the first error bit which is selected by the Mask Hi register, blocking future error bits into this register until the register has been cleared. This register is used to determine which error happened first if multiple errors are indicated in the error register. Writing a '1' to a bit will clear the appropriate bit in this register.
    Bits: 27:0  WhosOnFrstHi(w1c) - Same bitmap as ErrRegHi.
*/
#define NFP_CRYPTOX_CORE_WhosOnFrstHi                        0x0000000c
#define   NFP_CRYPTOX_CORE_WhosOnFrstHi_WhosOnFrstHi(_x)     (((_x) & 0xfffffff) << 0)
#define   NFP_CRYPTOX_CORE_WhosOnFrstHi_WhosOnFrstHi_of(_x)  (((_x) >> 0) & 0xfffffff)
/*----------------------------------------------------------------
  Register: ErrMaskLo - Error mask low bits [31:0]. Each bit maps directly to a bit in the Error Lo Register. This mask is used with the "Who's on First" Lo register and the Clock Stop Lo Mask. Setting a bit position to a '1' will enable the associated error to be detected.
    Bits: 29:0  ErrMaskLo(rw) - Same bitmap as ErrRegLo.
*/
#define NFP_CRYPTOX_CORE_ErrMaskLo                           0x00000010
#define   NFP_CRYPTOX_CORE_ErrMaskLo_ErrMaskLo(_x)           (((_x) & 0x3fffffff) << 0)
#define   NFP_CRYPTOX_CORE_ErrMaskLo_ErrMaskLo_of(_x)        (((_x) >> 0) & 0x3fffffff)
/*----------------------------------------------------------------
  Register: ErrMaskHi - Error mask high bits [63:32]. Each bit maps directly to a bit in the Error Hi Register. This mask is used with the "Who's on First" Hi register and the Clock Stop Hi Mask. Setting a bit position to a '1' will enable the associated error to be detected.
    Bits: 27:16 ErrMaskHi1(rw) - Same bitmap as ErrRegHi.
    Bits: 11:0  ErrMaskHi0(rw) - Same bitmap as ErrRegHi.
*/
#define NFP_CRYPTOX_CORE_ErrMaskHi                           0x00000014
#define   NFP_CRYPTOX_CORE_ErrMaskHi_ErrMaskHi1(_x)          (((_x) & 0xfff) << 16)
#define   NFP_CRYPTOX_CORE_ErrMaskHi_ErrMaskHi1_of(_x)       (((_x) >> 16) & 0xfff)
#define   NFP_CRYPTOX_CORE_ErrMaskHi_ErrMaskHi0(_x)          (((_x) & 0xfff) << 0)
#define   NFP_CRYPTOX_CORE_ErrMaskHi_ErrMaskHi0_of(_x)       (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: ClkStpMskLo - Clock stop mask low bits [31:0]. This register is used along with the Error Mask Lo register to select which error bits will trigger a local clock stop. Setting a bit position to a '1' will enable the associated error to cause a local clock stop.
    Bits: 29:0  ClkStpMskLo(rw) - Same bitmap as ErrRegLo.
*/
#define NFP_CRYPTOX_CORE_ClkStpMskLo                         0x00000018
#define   NFP_CRYPTOX_CORE_ClkStpMskLo_ClkStpMskLo(_x)       (((_x) & 0x3fffffff) << 0)
#define   NFP_CRYPTOX_CORE_ClkStpMskLo_ClkStpMskLo_of(_x)    (((_x) >> 0) & 0x3fffffff)
/*----------------------------------------------------------------
  Register: ClkStpMskHi - Clock stop mask high bits [63:32]. This register is used along with the Error Mask Hi register to select which error bits will trigger a local clock stop. Setting a bit position to a '1' will enable the associated error to cause a local clock stop.
    Bits: 27:16 ClkStpMskHi1(rw) - Same bitmap as ErrRegHi.
    Bits: 11:0  ClkStpMskHi0(rw) - Same bitmap as ErrRegHi.
*/
#define NFP_CRYPTOX_CORE_ClkStpMskHi                         0x0000001c
#define   NFP_CRYPTOX_CORE_ClkStpMskHi_ClkStpMskHi1(_x)      (((_x) & 0xfff) << 16)
#define   NFP_CRYPTOX_CORE_ClkStpMskHi_ClkStpMskHi1_of(_x)   (((_x) >> 16) & 0xfff)
#define   NFP_CRYPTOX_CORE_ClkStpMskHi_ClkStpMskHi0(_x)      (((_x) & 0xfff) << 0)
#define   NFP_CRYPTOX_CORE_ClkStpMskHi_ClkStpMskHi0_of(_x)   (((_x) >> 0) & 0xfff)
/*----------------------------------------------------------------
  Register: TstMdErrInj - Test Mode And Error Inject.
    Bits: 31:24 Target(wo) - Select target logic to send error inject pulse. See following target table.
    Bits: 23:0  Data(wo) - Set specific bit(s) to '1' to inject error pulse. See following bit map tables for each target.
*/
#define NFP_CRYPTOX_CORE_TstMdErrInj                         0x00000020
#define   NFP_CRYPTOX_CORE_TstMdErrInj_Target(_x)            (((_x) & 0xff) << 24)
#define   NFP_CRYPTOX_CORE_TstMdErrInj_Target_of(_x)         (((_x) >> 24) & 0xff)
#define   NFP_CRYPTOX_CORE_TstMdErrInj_Data(_x)              (((_x) & 0xffffff) << 0)
#define   NFP_CRYPTOX_CORE_TstMdErrInj_Data_of(_x)           (((_x) >> 0) & 0xffffff)

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_CRYPTO_H */
