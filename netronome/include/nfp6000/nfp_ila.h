/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_ILA_H
#define NFP6000_ILA_H

/*================================================================*/
/* TGT 6                                                          */

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* CPP_BASE + NFP_ILA_SRAM + NFP_ILA_SRAM_* */
#define NFP_ILA_SRAM                                         (0x000000)
/* CPP_BASE + NFP_ILA_IM + (nfp_im.h) */
#define NFP_ILA_IM                                      (0x010000)
/* CPP_BASE + NFP_ILA_ERROR_CODE + NFP_ILA_ERROR_CODE_* */
#define NFP_ILA_ERROR_CODE                                   (0x020000)
/* CPP_BASE + NFP_ILA_BARCFG + NFP_ILA_BARCFG_* */
#define NFP_ILA_ERROR_BARCFG                                 (0x030000)
/* CPP_BASE + NFP_ILA_DMA + NFP_ILA_DMA_* */
#define NFP_ILA_DMA                                          (0x040000)
/* CPP_BASE + NFP_ILA_TCAM + NFP_ILA_TCAM_* */
#define NFP_ILA_TCAM                                         (0x050000)
/* CPP_BASE + NFP_ILA_Q[256] + (nfp_qctlr.h) */
#define NFP_ILA_Q(_x)                                        (0x080000 + ((_x) & 0xff) * 0x800)

/*----------------------------------------------------------------*/
/* XPB Address Map (XPB_BASE = NFP_XPB_ISLAND(island))          */

#define ILAX_BASE                                            (0xa0000)

/* XPB_BASE + NFP_ILAX_CSR + NFP_ILAX_CSR_* */
#define NFP_ILAX_CSR                                         (ILAX_BASE + 0x000000)
/* XPB_BASE + NFP_ILAX_IM + (nfp_im.h) */
#define NFP_ILAX_IM                                          (ILAX_BASE + 0x010000)
/* XPB_BASE + NFP_ILAX_SHARED_ECC + (nfp_ecc.h) */
#define NFP_ILAX_SHARED_ECC                                  (ILAX_BASE + 0x020000)
/* XPB_BASE + NFP_ILAX_QMNGR_ECC + (nfp_ecc.h) */
#define NFP_ILAX_QMNGR_ECC                                   (ILAX_BASE + 0x030000)

/*================================================================*/


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: IlaSSramEntry[16384] - ILA Shared SRAM Entry X
    Bits: 31:0  SSRAMData(rw) - 32-bits of data within the ILA Shared SRAM
*/
#define NFP_ILA_SRAM_ILA_SSRAM_ENTRY(_x)                     (0x00000000 + (0x4 * ((_x) & 0x3fff)))
#define   NFP_ILA_SRAM_ILA_SSRAM_ENTRY_SSRAM_DATA(_x)        (((_x) & 0xffffffff) << 0)
#define   NFP_ILA_SRAM_ILA_SSRAM_ENTRY_SSRAM_DATA_of(_x)     (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaErrorCode - ACCEL Mode ILA Error Code Command
    Bits:  1    IlaErrorCodeGen(rw) - Enable ILA Error Code Command
    Bits:  0    IlaErrorCodeChan(rw) - Select ILA Error Code Command Channel
*/
#define NFP_ILA_ERROR_CODE_ILA_ERROR_CODE                    0x00000000
#define   NFP_ILA_ERROR_CODE_ILA_ERROR_CODE_ILA_ERRORCODE_GEN BIT(1)
#define   NFP_ILA_ERROR_CODE_ILA_ERROR_CODE_ILA_ERRORCODE_CHAN BIT(0)
/*----------------------------------------------------------------
  Register: IlaToCppBar[8] - ILA to CPP translation BAR
    Bits: 29:25 TargetID(rw) - CPP target ID. Local Registers are accessed when TargetID=0
    Bits: 24:23 Token(rw) - CPP token field.
    Bits: 22    LengthSelect(rw) - Used to select the size of the CPP length field.
*/
#define NFP_ILA_BARCFG_I2C(_x)                               (0x00000000 + (0x4 * ((_x) & 0x7)))
#define   NFP_ILA_BARCFG_I2C_TGTID(_x)                       (((_x) & 0x1f) << 25)
#define   NFP_ILA_BARCFG_I2C_TGTID_of(_x)                    (((_x) >> 25) & 0x1f)
#define   NFP_ILA_BARCFG_I2C_TOKSEL(_x)                      (((_x) & 0x3) << 23)
#define   NFP_ILA_BARCFG_I2C_TOKSEL_of(_x)                   (((_x) >> 23) & 0x3)
#define   NFP_ILA_BARCFG_I2C_LEN                             BIT(22)
#define     NFP_ILA_BARCFG_I2C_LEN_32BIT                     (0 << 22)
#define     NFP_ILA_BARCFG_I2C_LEN_64BIT                     BIT(22)
/*----------------------------------------------------------------
  Register: CppToIlaBar[8] - CPP to ILA translation BAR
    Bits: 21    IlaChSel(rw) - Selects which ILA Channel to send the command on
    Bits: 20:0  Address(rw) - In 32-bit mode: ILA address bits [47:27]; In 40-bit mode: ILA address bits[47:35]
*/
#define NFP_ILA_BARCFG_C2I(_x)                               (0x00000020 + (0x4 * ((_x) & 0x7)))
#define   NFP_ILA_BARCFG_C2I_ILA_CH_SEL                      BIT(21)
#define   NFP_ILA_BARCFG_C2I_ADDR(_x)                        (((_x) & 0x1fffff) << 0)
#define   NFP_ILA_BARCFG_C2I_ADDR_of(_x)                     (((_x) >> 0) & 0x1fffff)
/*----------------------------------------------------------------
  Register: DmaCommandCh0ToIla - Insert command into Ch0ToIla DMA command queue
  Register: DmaCommandCh1ToIla - Insert command into Ch1ToIla DMA command queue
  Register: DmaCommandCh0FrmIla - Insert command into Ch0FrmIla DMA command queue
  Register: DmaCommandCh1FrmIla - Insert command into Ch1FrmIla DMA command queue
    Bits: 127:116 XferLength(wo) - Length of transfer in bytes
    Bits: 111:96 IlaAddressHi(wo) - Upper 16 bits of 48-bit ILA address
    Bits: 95:66 IlaAddressLo(wo) - Next 30 bits of 48-bit ILA address
    Bits: 65:48 CompletionIndication(wo) - Event to generate on completed command. Can be either no signal, generate an event on the event bus, or send a signal directly to a CPP master.
    Bits: 47:46 Token(wo) - CPP token bits for CPP command
    Bits: 45    SignalBothOnErrEn(wo) - Use to enable signaling both odd and even signals when a DMA encounters an ILA Rx Error.
    Bits: 44    Target64(wo) - CPP target is 64-bit. Set if target supports 64-bit transactions, or cleared if target only supports 32-bit transaction.
    Bits: 43:40 CppTarget(wo) - Target ID for CPP transaction
    Bits: 39:32 CppAddressHi(wo) - CPP bus address
    Bits: 31:0  CppAddressLo(wo) - CPP bus address
*/
#define NFP_ILA_DMA_CMD_TILA_CH0                             0x00000000
#define NFP_ILA_DMA_CMD_TILA_CH1                             0x00000010
#define NFP_ILA_DMA_CMD_FILA_CH0                             0x00000020
#define NFP_ILA_DMA_CMD_FILA_CH1                             0x00000030
#define   NFP_ILA_DMA_ILADMACOMMAND_LENGTH(_x)               (((_x) & 0xfff) << 116)
#define   NFP_ILA_DMA_ILADMACOMMAND_LENGTH_of(_x)            (((_x) >> 116) & 0xfff)
#define   NFP_ILA_DMA_ILADMACOMMAND_ILA_ADDR_HI(_x)          (((_x) & 0xffff) << 96)
#define   NFP_ILA_DMA_ILADMACOMMAND_ILA_ADDR_HI_of(_x)       (((_x) >> 96) & 0xffff)
#define   NFP_ILA_DMA_ILADMACOMMAND_ILA_ADDR_LO(_x)          (((_x) & 0x3fffffff) << 66)
#define   NFP_ILA_DMA_ILADMACOMMAND_ILA_ADDR_LO_of(_x)       (((_x) >> 66) & 0x3fffffff)
#define   NFP_ILA_DMA_ILADMACOMMAND_CPL(_x)                  (((_x) & 0x3ffff) << 48)
#define   NFP_ILA_DMA_ILADMACOMMAND_CPL_of(_x)               (((_x) >> 48) & 0x3ffff)
#define   NFP_ILA_DMA_ILADMACOMMAND_TOKEN(_x)                (((_x) & 0x3) << 46)
#define   NFP_ILA_DMA_ILADMACOMMAND_TOKEN_of(_x)             (((_x) >> 46) & 0x3)
#define   NFP_ILA_DMA_ILADMACOMMAND_SIGNAL_BOTH_ON_ERR_EN    BIT_ULL(45)
#define     NFP_ILA_DMA_ILADMACOMMAND_SIGNAL_BOTH_ON_ERR_EN_DONT_ASSERT_NO_SPLIT (0 << 45)
#define     NFP_ILA_DMA_ILADMACOMMAND_SIGNAL_BOTH_ON_ERR_EN_ASSERT_NO_SPLIT BIT_ULL(45)
#define   NFP_ILA_DMA_ILADMACOMMAND_TARGET64                 BIT_ULL(44)
#define     NFP_ILA_DMA_ILADMACOMMAND_TARGET64_DISABLE       (0 << 44)
#define     NFP_ILA_DMA_ILADMACOMMAND_TARGET64_ENABLE        BIT_ULL(44)
#define   NFP_ILA_DMA_ILADMACOMMAND_CPP_TARGET(_x)           (((_x) & 0xf) << 40)
#define   NFP_ILA_DMA_ILADMACOMMAND_CPP_TARGET_of(_x)        (((_x) >> 40) & 0xf)
#define   NFP_ILA_DMA_ILADMACOMMAND_CPP_ADDR_HI(_x)          (((_x) & 0xff) << 32)
#define   NFP_ILA_DMA_ILADMACOMMAND_CPP_ADDR_HI_of(_x)       (((_x) >> 32) & 0xff)
#define   NFP_ILA_DMA_ILADMACOMMAND_CPP_ADDR_LO(_x)          (((_x) & 0xffffffff) << 0)
#define   NFP_ILA_DMA_ILADMACOMMAND_CPP_ADDR_LO_of(_x)       (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: DmaCtrlCh0ToIla - Control Ch0ToIla DMA command queue
  Register: DmaCtrlCh1ToIla - Control Ch1ToIla DMA command queue
  Register: DmaCtrlCh0FrmIla - Control Ch0FrmIla DMA command queue
  Register: DmaCtrlCh1FrmIla - Control Ch1FrmIla DMA command queue
    Bits:  5    DescReadError(w1c) - Set to clear the error indication for descriptor reads. Setting to 0 has no effect.
    Bits:  4    IlaError(w1c) - Set to clear the error indication for ILA transaction failures. Setting to 0 has no effect.
    Bits:  2    QueueOverflow(w1c) - Set to clear queue overflow indication. Setting to 0 has no effect
    Bits:  1    ContinueOnError(rw) - Setting this bit will stop errors from blocking DMAs
    Bits:  0    QueueStop(rw) - Setting this bit will stop DMA engine from executing from queue. Clearing the bit will start up execution again.
*/
#define NFP_ILA_DMA_CTRL_TILA_CH0                            0x00000040
#define NFP_ILA_DMA_CTRL_TILA_CH1                            0x00000044
#define NFP_ILA_DMA_CTRL_FILA_CH0                            0x00000048
#define NFP_ILA_DMA_CTRL_FILA_CH1                            0x0000004c
#define   NFP_ILA_DMA_ILADMACONTROL_DESC_ERROR               BIT(5)
#define     NFP_ILA_DMA_ILADMACONTROL_DESC_ERROR_NO_EFFECT   (0 << 5)
#define     NFP_ILA_DMA_ILADMACONTROL_DESC_ERROR_CLEAR       BIT(5)
#define   NFP_ILA_DMA_ILADMACONTROL_ILA_ERROR                BIT(4)
#define     NFP_ILA_DMA_ILADMACONTROL_ILA_ERROR_NO_EFFECT    (0 << 4)
#define     NFP_ILA_DMA_ILADMACONTROL_ILA_ERROR_CLEAR        BIT(4)
#define   NFP_ILA_DMA_ILADMACONTROL_QUEUE_OVERFLOW           BIT(2)
#define     NFP_ILA_DMA_ILADMACONTROL_QUEUE_OVERFLOW_NO_EFFECT (0 << 2)
#define     NFP_ILA_DMA_ILADMACONTROL_QUEUE_OVERFLOW_CLEAR   BIT(2)
#define   NFP_ILA_DMA_ILADMACONTROL_CONT_ON_ERROR            BIT(1)
#define     NFP_ILA_DMA_ILADMACONTROL_CONT_ON_ERROR_DISABLE  (0 << 1)
#define     NFP_ILA_DMA_ILADMACONTROL_CONT_ON_ERROR_ENABLE   BIT(1)
#define   NFP_ILA_DMA_ILADMACONTROL_QUEUE_STOP               BIT(0)
#define     NFP_ILA_DMA_ILADMACONTROL_QUEUE_STOP_DISABLE     (0 << 0)
#define     NFP_ILA_DMA_ILADMACONTROL_QUEUE_STOP_ENABLE      BIT(0)
/*----------------------------------------------------------------
  Register: DmaWritePtrCh0ToIla - Write pointer for Ch0ToIla DMA command queue
  Register: DmaWritePtrCh1ToIla - Write pointer for Ch1ToIla DMA command queue
    Bits: 23:16 CmplFifoPopPtr(ro) - Number of entries popped from the Completion FIFO.
    Bits:  8:0  Pointer(ro) - Write pointer address in queue memory
*/
#define NFP_ILA_DMA_WRITEPTR_TILA_CH0                        0x00000050
#define NFP_ILA_DMA_WRITEPTR_TILA_CH1                        0x00000060
#define   NFP_ILA_DMA_ILATODMAWRITEPTR_CMPL_FIFO_POP_PTR(_x) (((_x) & 0xff) << 16)
#define   NFP_ILA_DMA_ILATODMAWRITEPTR_CMPL_FIFO_POP_PTR_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_ILA_DMA_ILATODMAWRITEPTR_PTR(_x)               (((_x) & 0x1ff) << 0)
#define   NFP_ILA_DMA_ILATODMAWRITEPTR_PTR_of(_x)            (((_x) >> 0) & 0x1ff)
/*----------------------------------------------------------------
  Register: DmaWritePtrCh0FrmIla - Write pointer for Ch0FrmIla DMA command queue
  Register: DmaWritePtrCh1FrmIla - Write pointer for Ch1FrmIla DMA command queue
    Bits:  8:0  Pointer(ro) - Write pointer address in queue memory
*/
#define NFP_ILA_DMA_WRITEPTR_FILA_CH0                        0x00000070
#define NFP_ILA_DMA_WRITEPTR_FILA_CH1                        0x00000080
#define   NFP_ILA_DMA_ILAFRMDMAWRITEPTR_PTR(_x)              (((_x) & 0x1ff) << 0)
#define   NFP_ILA_DMA_ILAFRMDMAWRITEPTR_PTR_of(_x)           (((_x) >> 0) & 0x1ff)
/*----------------------------------------------------------------
  Register: DmaQStatusCh0ToIla - Queue status for Ch0ToIla DMA command queue
  Register: DmaQStatusCh1ToIla - Queue status for Ch1ToIla DMA command queue
  Register: DmaQStatusCh0FrmIla - Queue status for Ch0FrmIla DMA command queue
  Register: DmaQStatusCh1FrmIla - Queue status for Ch1FrmIla DMA command queue
    Bits: 31:24 Avail(ro) - Amount of available space in queue. A value of 0 indicates 256 entries available. Use QueueFull flag to determine if there is no space.
    Bits: 23:16 ReadPointer(ro) - Pointer to the next descriptor to be issued by the DMA engine.
    Bits: 15:8  PopPointer(ro) - Pointer to the next descriptor to be completed by the DMA engine. The DMA queue write pointer can be calculated as (DmaQueueStatus[PopPointer] - DmaQueueStatus[Avail]) mod 256.
    Bits:  7    QueueFull(ro) - Set when queue is full, cleared otherwise.
    Bits:  5    DescReadError(ro) - Set when a multi-bit error is detected on reading the descriptor from the SRAM queue. Must be cleared explicitly by writing the DMA Queue Control register.
    Bits:  4    IlaError(ro) - Set when an error is detected in the received ILA transaction. Must be cleared explicitly by writing the DMA Queue Control register. Specific error latched in the IlaErrStat register.
    Bits:  2    QueueOverflow(ro) - Set when a descriptor is written to a full queue. Must be cleared explicitly by writing the DMA Queue Control register.
    Bits:  1    ContinueOnError(ro) - If set errors will not block further DMAs.
    Bits:  0    QueueStop(ro) - Set when the DMA engine is stopped due to an error, or if DMA engine is explicitly stopped by writing to the DMA Queue Control Register.
*/
#define NFP_ILA_DMA_QSTS_TILA_CH0                            0x00000090
#define NFP_ILA_DMA_QSTS_TILA_CH1                            0x00000094
#define NFP_ILA_DMA_QSTS_FILA_CH0                            0x00000098
#define NFP_ILA_DMA_QSTS_FILA_CH1                            0x0000009c
#define   NFP_ILA_DMA_ILADMAQUEUESTATUS_AVAIL(_x)            (((_x) & 0xff) << 24)
#define   NFP_ILA_DMA_ILADMAQUEUESTATUS_AVAIL_of(_x)         (((_x) >> 24) & 0xff)
#define   NFP_ILA_DMA_ILADMAQUEUESTATUS_READ_PTR(_x)         (((_x) & 0xff) << 16)
#define   NFP_ILA_DMA_ILADMAQUEUESTATUS_READ_PTR_of(_x)      (((_x) >> 16) & 0xff)
#define   NFP_ILA_DMA_ILADMAQUEUESTATUS_POP_PTR(_x)          (((_x) & 0xff) << 8)
#define   NFP_ILA_DMA_ILADMAQUEUESTATUS_POP_PTR_of(_x)       (((_x) >> 8) & 0xff)
#define   NFP_ILA_DMA_ILADMAQUEUESTATUS_QUEUE_FULL           BIT(7)
#define   NFP_ILA_DMA_ILADMAQUEUESTATUS_DESC_ERROR           BIT(5)
#define   NFP_ILA_DMA_ILADMAQUEUESTATUS_ILA_ERROR            BIT(4)
#define   NFP_ILA_DMA_ILADMAQUEUESTATUS_QUEUE_OVERFLOW       BIT(2)
#define   NFP_ILA_DMA_ILADMAQUEUESTATUS_CONT_ON_ERROR        BIT(1)
#define   NFP_ILA_DMA_ILADMAQUEUESTATUS_QUEUE_STOP           BIT(0)
/*----------------------------------------------------------------
  Register: DmaCommandToIlaTcam - Insert command into ToIla TCAM DMA command queue
    Bits: 127:116 XferLength(wo) - Length of transfer in bytes
    Bits: 110    Target64Ret(wo) - CPP target is 64-bit for return transaction. Set if target supports 64-bit transactions, or cleared if target only supports 32-bit transaction.
    Bits: 109:108 TokenRet(wo) - CPP token bits for return CPP command
    Bits: 107:104 CppTargetRet(wo) - Target ID for CPP return transaction
    Bits: 103:96 CppAddressHiRet(wo) - Upper 8-bits of the 40-bit return CPP Address used to write the return TCAM data.
    Bits: 95:66 CppAddressLoRet(wo) - Next 30 bits of 40-bit return CPP address used to write the return TCAM data.
    Bits: 65:48 CompletionIndication(wo) - Event to generate on completed command. Can be either no signal, generate an event on the event bus, or send a signal directly to a CPP master.
    Bits: 47:46 Token(wo) - CPP token bits for CPP command
    Bits: 45    SignalBothOnErrEn(wo) - Use to enable signaling both odd and even signals when a DMA encounters an ILA Rx Error.
    Bits: 44    Target64(wo) - CPP target is 64-bit. Set if target supports 64-bit transactions, or cleared if target only supports 32-bit transaction.
    Bits: 43:40 CppTarget(wo) - Target ID for CPP transaction
    Bits: 39:32 CppAddressHi(wo) - CPP bus address
    Bits: 31:0  CppAddressLo(wo) - CPP bus address
*/
#define NFP_ILA_TCAM_CMD_TILA_TCAM                           0x00000000
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_LENGTH(_x)              (((_x) & 0xfff) << 116)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_LENGTH_of(_x)           (((_x) >> 116) & 0xfff)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_TARGET64_RET            BIT_ULL(110)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_TOKEN_RET(_x)           (((_x) & 0x3) << 108)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_TOKEN_RET_of(_x)        (((_x) >> 108) & 0x3)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_CPP_TARGET_RET(_x)      (((_x) & 0xf) << 104)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_CPP_TARGET_RET_of(_x)   (((_x) >> 104) & 0xf)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_CPP_ADDR_HI_RET(_x)     (((_x) & 0xff) << 96)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_CPP_ADDR_HI_RET_of(_x)  (((_x) >> 96) & 0xff)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_ILA_ADDR_LO(_x)         (((_x) & 0x3fffffff) << 66)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_ILA_ADDR_LO_of(_x)      (((_x) >> 66) & 0x3fffffff)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_CPL(_x)                 (((_x) & 0x3ffff) << 48)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_CPL_of(_x)              (((_x) >> 48) & 0x3ffff)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_TOKEN(_x)               (((_x) & 0x3) << 46)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_TOKEN_of(_x)            (((_x) >> 46) & 0x3)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_SIGNAL_BOTH_ON_ERR_EN   BIT_ULL(45)
#define     NFP_ILA_TCAM_CMD_TILA_TCAM_SIGNAL_BOTH_ON_ERR_EN_DONT_ASSERT_NO_SPLIT (0 << 45)
#define     NFP_ILA_TCAM_CMD_TILA_TCAM_SIGNAL_BOTH_ON_ERR_EN_ASSERT_NO_SPLIT BIT_ULL(45)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_TARGET64                BIT_ULL(44)
#define     NFP_ILA_TCAM_CMD_TILA_TCAM_TARGET64_DISABLE      (0 << 44)
#define     NFP_ILA_TCAM_CMD_TILA_TCAM_TARGET64_ENABLE       BIT_ULL(44)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_CPP_TARGET(_x)          (((_x) & 0xf) << 40)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_CPP_TARGET_of(_x)       (((_x) >> 40) & 0xf)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_CPP_ADDR_HI(_x)         (((_x) & 0xff) << 32)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_CPP_ADDR_HI_of(_x)      (((_x) >> 32) & 0xff)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_CPP_ADDR_LO(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP_ILA_TCAM_CMD_TILA_TCAM_CPP_ADDR_LO_of(_x)      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: DmaCtrlToIlaTcam - Control DMA command queue for TCAM operations
    Bits:  5    DescReadError(w1c) - Set to clear the error indication for descriptor reads. Setting to 0 has no effect.
    Bits:  4    IlaError(w1c) - Set to clear the error indication for ILA transaction failures. Setting to 0 has no effect.
    Bits:  2    QueueOverflow(w1c) - Set to clear queue overflow indication. Setting to 0 has no effect
    Bits:  1    ContinueOnError(rw) - Setting this bit will stop errors from blocking DMAs
    Bits:  0    QueueStop(rw) - Setting this bit will stop DMA engine from executing from queue. Clearing the bit will start up execution again.
*/
#define NFP_ILA_TCAM_CTRL_TILA_TCAM                          0x00000040
#define   NFP_ILA_TCAM_CTRL_TILA_TCAM_DESC_ERROR             BIT(5)
#define     NFP_ILA_TCAM_CTRL_TILA_TCAM_DESC_ERROR_NO_EFFECT (0 << 5)
#define     NFP_ILA_TCAM_CTRL_TILA_TCAM_DESC_ERROR_CLEAR     BIT(5)
#define   NFP_ILA_TCAM_CTRL_TILA_TCAM_ILA_ERROR              BIT(4)
#define     NFP_ILA_TCAM_CTRL_TILA_TCAM_ILA_ERROR_NO_EFFECT  (0 << 4)
#define     NFP_ILA_TCAM_CTRL_TILA_TCAM_ILA_ERROR_CLEAR      BIT(4)
#define   NFP_ILA_TCAM_CTRL_TILA_TCAM_QUEUE_OVERFLOW         BIT(2)
#define     NFP_ILA_TCAM_CTRL_TILA_TCAM_QUEUE_OVERFLOW_NO_EFFECT (0 << 2)
#define     NFP_ILA_TCAM_CTRL_TILA_TCAM_QUEUE_OVERFLOW_CLEAR BIT(2)
#define   NFP_ILA_TCAM_CTRL_TILA_TCAM_CONT_ON_ERROR          BIT(1)
#define     NFP_ILA_TCAM_CTRL_TILA_TCAM_CONT_ON_ERROR_DISABLE (0 << 1)
#define     NFP_ILA_TCAM_CTRL_TILA_TCAM_CONT_ON_ERROR_ENABLE BIT(1)
#define   NFP_ILA_TCAM_CTRL_TILA_TCAM_QUEUE_STOP             BIT(0)
#define     NFP_ILA_TCAM_CTRL_TILA_TCAM_QUEUE_STOP_DISABLE   (0 << 0)
#define     NFP_ILA_TCAM_CTRL_TILA_TCAM_QUEUE_STOP_ENABLE    BIT(0)
/*----------------------------------------------------------------
  Register: DmaWritePtrToIlaTcam - Write pointer for ToIla TCAM DMA command queue
    Bits: 23:16 CmplFifoPopPtr(ro) - Number of entries popped from the Completion FIFO.
    Bits:  8:0  Pointer(ro) - Write pointer address in queue memory
*/
#define NFP_ILA_TCAM_WRITEPTR_TILA_TCAM                      0x00000050
#define   NFP_ILA_TCAM_WRITEPTR_TILA_TCAM_CMPL_FIFO_POP_PTR(_x) (((_x) & 0xff) << 16)
#define   NFP_ILA_TCAM_WRITEPTR_TILA_TCAM_CMPL_FIFO_POP_PTR_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_ILA_TCAM_WRITEPTR_TILA_TCAM_PTR(_x)            (((_x) & 0x1ff) << 0)
#define   NFP_ILA_TCAM_WRITEPTR_TILA_TCAM_PTR_of(_x)         (((_x) >> 0) & 0x1ff)
/*----------------------------------------------------------------
  Register: DmaQStatusToIlaTcam - Queue status for ToIla TCAM DMA command queue
    Bits: 31:24 Avail(ro) - Amount of available space in queue. A value of 0 indicates 256 entries available. Use QueueFull flag to determine if there is no space.
    Bits: 23:16 ReadPointer(ro) - Pointer to the next descriptor to be issued by the DMA engine.
    Bits: 15:8  PopPointer(ro) - Pointer to the next descriptor to be completed by the DMA engine. The DMA queue write pointer can be calculated as (DmaQueueStatus[PopPointer] - DmaQueueStatus[Avail]) mod 256.
    Bits:  7    QueueFull(ro) - Set when queue is full, cleared otherwise.
    Bits:  5    DescReadError(ro) - Set when a multi-bit error is detected on reading the descriptor from the SRAM queue. Must be cleared explicitly by writing the DMA Queue Control register.
    Bits:  4    IlaError(ro) - Set when an error is detected in the received ILA transaction. Must be cleared explicitly by writing the DMA Queue Control register. Specific error latched in the IlaErrStat register.
    Bits:  2    QueueOverflow(ro) - Set when a descriptor is written to a full queue. Must be cleared explicitly by writing the DMA Queue Control register.
    Bits:  1    ContinueOnError(ro) - If set errors will not block further DMAs.
    Bits:  0    QueueStop(ro) - Set when the DMA engine is stopped due to an error, or if DMA engine is explicitly stopped by writing to the DMA Queue Control Register.
*/
#define NFP_ILA_TCAM_QSTS_TILA_TCAM                          0x00000090
#define   NFP_ILA_TCAM_QSTS_TILA_TCAM_AVAIL(_x)              (((_x) & 0xff) << 24)
#define   NFP_ILA_TCAM_QSTS_TILA_TCAM_AVAIL_of(_x)           (((_x) >> 24) & 0xff)
#define   NFP_ILA_TCAM_QSTS_TILA_TCAM_READ_PTR(_x)           (((_x) & 0xff) << 16)
#define   NFP_ILA_TCAM_QSTS_TILA_TCAM_READ_PTR_of(_x)        (((_x) >> 16) & 0xff)
#define   NFP_ILA_TCAM_QSTS_TILA_TCAM_POP_PTR(_x)            (((_x) & 0xff) << 8)
#define   NFP_ILA_TCAM_QSTS_TILA_TCAM_POP_PTR_of(_x)         (((_x) >> 8) & 0xff)
#define   NFP_ILA_TCAM_QSTS_TILA_TCAM_QUEUE_FULL             BIT(7)
#define   NFP_ILA_TCAM_QSTS_TILA_TCAM_DESC_ERROR             BIT(5)
#define   NFP_ILA_TCAM_QSTS_TILA_TCAM_ILA_ERROR              BIT(4)
#define   NFP_ILA_TCAM_QSTS_TILA_TCAM_QUEUE_OVERFLOW         BIT(2)
#define   NFP_ILA_TCAM_QSTS_TILA_TCAM_CONT_ON_ERROR          BIT(1)
#define   NFP_ILA_TCAM_QSTS_TILA_TCAM_QUEUE_STOP             BIT(0)
/*----------------------------------------------------------------
  Register: IlaCtrlCfg - Selects operation mode for the ILA controller
    Bits: 20    IlaRxFifoPack(rw) - ILA RX FIFO PACKING
    Bits: 19:10 IlaTxIdleCnt(rw) - Controls how often idle packets are transmitted. Idle packets communicate the LCC value to the peer device when no active packets are being transmitted. Software must program this value AFTER changing the IlaMode=ACCEL. The recommended value is 15.
    Bits:  9    IlaRxFifoSize(rw) - ILA RX FIFO SIZE
    Bits:  8    IlaTxWrCreditCntEna(rw) - ILA TX Credit Counter Enable
    Bits:  5    IlaErrPoisonEn(rw) - Enables poisoning of push or pull data upon receipt of an ILA Receive Error. Poisoning the data sets the data_error flag on the bus during the data transfer.
    Bits:  4    IlaCtrlRsvd2(rw) - Reserved
    Bits:  3    BurstMax(rw) - Controls the maximum size of a burst on the Interlaken Look-Aside bus. Must match the CSR setting in the Interlaken Look-Aside IP core (See MAC block registers).
    Bits:  2    IlaCtrlRsvd(rw) - Reserved for future use
    Bits:  1    IlaMode(rw) - Configures the Mode of ILA Block
    Bits:  0    IlaCppAddrMode(rw) - Configure whether to interpret the CPP address bits as 32-bit or 40-bit.
*/
#define NFP_ILAX_CSR_ILA_CFG                                 0x00000000
#define   NFP_ILAX_CSR_ILA_CFG_ILA_RX_FIFO_PACK              BIT(20)
#define     NFP_ILAX_CSR_ILA_CFG_ILA_RX_FIFO_PACK_ILARXFIFOPACKENA (0 << 20)
#define     NFP_ILAX_CSR_ILA_CFG_ILA_RX_FIFO_PACK_ILARXFIFOPACKDIS BIT(20)
#define   NFP_ILAX_CSR_ILA_CFG_ILA_TX_IDLE_CNT(_x)           (((_x) & 0x3ff) << 10)
#define   NFP_ILAX_CSR_ILA_CFG_ILA_TX_IDLE_CNT_of(_x)        (((_x) >> 10) & 0x3ff)
#define   NFP_ILAX_CSR_ILA_CFG_ILA_RX_FIFO_SIZE              BIT(9)
#define     NFP_ILAX_CSR_ILA_CFG_ILA_RX_FIFO_SIZE_ILARXFIFO4K (0 << 9)
#define     NFP_ILAX_CSR_ILA_CFG_ILA_RX_FIFO_SIZE_ILARXFIFO8K BIT(9)
#define   NFP_ILAX_CSR_ILA_CFG_ILA_TX_WR_CREDIT_CNT_ENA      BIT(8)
#define     NFP_ILAX_CSR_ILA_CFG_ILA_TX_WR_CREDIT_CNT_ENA_DIS (0 << 8)
#define     NFP_ILAX_CSR_ILA_CFG_ILA_TX_WR_CREDIT_CNT_ENA_ENA BIT(8)
#define   NFP_ILAX_CSR_ILA_CFG_ILA_ERR_POISON_EN             BIT(5)
#define     NFP_ILAX_CSR_ILA_CFG_ILA_ERR_POISON_EN_DISABLE   (0 << 5)
#define     NFP_ILAX_CSR_ILA_CFG_ILA_ERR_POISON_EN_ENABLE    BIT(5)
#define   NFP_ILAX_CSR_ILA_CFG_ILA_CTRL_RSVD2                BIT(4)
#define   NFP_ILAX_CSR_ILA_CFG_BURST_MAX                     BIT(3)
#define     NFP_ILAX_CSR_ILA_CFG_BURST_MAX_256               (0 << 3)
#define     NFP_ILAX_CSR_ILA_CFG_BURST_MAX_512               BIT(3)
#define   NFP_ILAX_CSR_ILA_CFG_ILA_CTRL_RSVD                 BIT(2)
#define   NFP_ILAX_CSR_ILA_CFG_ILA_MODE                      BIT(1)
#define     NFP_ILAX_CSR_ILA_CFG_ILA_MODE_TCAM               (0 << 1)
#define     NFP_ILAX_CSR_ILA_CFG_ILA_MODE_ACCEL              BIT(1)
#define   NFP_ILAX_CSR_ILA_CFG_ILA_CPP_ADDR_MODE             BIT(0)
#define     NFP_ILAX_CSR_ILA_CFG_ILA_CPP_ADDR_MODE_40        (0 << 0)
#define     NFP_ILAX_CSR_ILA_CFG_ILA_CPP_ADDR_MODE_32        BIT(0)
/*----------------------------------------------------------------
  Register: IlaSpare - General Purpose Register
    Bits: 31:0  IlaSpare(rw) - General use register.
*/
#define NFP_ILAX_CSR_ILA_SPARE                               0x00000004
#define   NFP_ILAX_CSR_ILA_SPARE_ILA_SPARE(_x)               (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_ILA_SPARE_ILA_SPARE_of(_x)            (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxChTimeOutIntvl - ILA Rx Channel 1 and 0 Timeout Interval
    Bits: 31:16 IlaRxTimeOutIntvlCh1(rw) - Time allowed for a response from the attached device in increments of 256ns. A value of zero disables the timer.
    Bits: 15:0  IlaRxTimeOutIntvlCh0(rw) - Time allowed for a response from the attached device in increments of 256ns. A value of zero disables the timer.
*/
#define NFP_ILAX_CSR_ILA_RX_CH_TO_INTVL                      0x0000000c
#define   NFP_ILAX_CSR_ILA_RX_CH_TO_INTVL_ILA_RXTO_INTVL_CH1(_x) (((_x) & 0xffff) << 16)
#define   NFP_ILAX_CSR_ILA_RX_CH_TO_INTVL_ILA_RXTO_INTVL_CH1_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_ILAX_CSR_ILA_RX_CH_TO_INTVL_ILA_RXTO_INTVL_CH0(_x) (((_x) & 0xffff) << 0)
#define   NFP_ILAX_CSR_ILA_RX_CH_TO_INTVL_ILA_RXTO_INTVL_CH0_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: IlaTxByteCntHi - ILA Tx Byte Counter High 32-bits
    Bits: 31:0  IlaTxByteCntHi(rc) - Upper 32-bits of 64-bit ILA Tx Byte count. Reading this register latches the value in IlaTxByteCntLo
*/
#define NFP_ILAX_CSR_ILA_TXBYTE_CNT_HI                       0x0000001c
#define   NFP_ILAX_CSR_ILA_TXBYTE_CNT_HI_ILA_TXBYTE_CNT_HI(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_ILA_TXBYTE_CNT_HI_ILA_TXBYTE_CNT_HI_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaTxByteCntLo - ILA Tx Byte Counter Low 32-bits
    Bits: 31:0  IlaTxByteCntLo(rc) - Lower 32-bits of 64-bit ILA Tx Byte count. Reading this register after reading the IlaTxByteCntHi register
*/
#define NFP_ILAX_CSR_ILA_TXBYTE_CNT_LO                       0x00000020
#define   NFP_ILAX_CSR_ILA_TXBYTE_CNT_LO_ILA_TXBYTE_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_ILA_TXBYTE_CNT_LO_ILA_TXBYTE_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaTxPktCntHi - ILA Tx Pkt Counter High 16-bits
    Bits: 15:0  IlaTxPktCntHi(rc) - Upper 16-bits of 48-bit ILA Tx Packet count. Reading this register latches the value in IlaTxPktCntLo
*/
#define NFP_ILAX_CSR_ILA_TXPKT_CNT_HI                        0x00000024
#define   NFP_ILAX_CSR_ILA_TXPKT_CNT_HI_ILA_TXPKT_CNT_HI(_x) (((_x) & 0xffff) << 0)
#define   NFP_ILAX_CSR_ILA_TXPKT_CNT_HI_ILA_TXPKT_CNT_HI_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: IlaTxPktCntLo - ILA Tx Pkt Counter Low 32-bits
    Bits: 31:0  IlaTxPktCntLo(rc) - Lower 32-bits of 48-bit ILA Tx Packet count. Reading this register after reading the IlaTxPktCntHi register
*/
#define NFP_ILAX_CSR_ILA_TXPKT_CNT_LO                        0x00000028
#define   NFP_ILAX_CSR_ILA_TXPKT_CNT_LO_ILA_TXPKT_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_ILA_TXPKT_CNT_LO_ILA_TXPKT_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxByteCntHi - ILA Rx Byte Counter High 32-bits
    Bits: 31:0  IlaRxByteCntHi(rc) - Upper 32-bits of 64-bit ILA Rx Byte count. Reading this register latches the value in IlaRxByteCntLo
*/
#define NFP_ILAX_CSR_ILA_RXBYTE_CNT_HI                       0x0000002c
#define   NFP_ILAX_CSR_ILA_RXBYTE_CNT_HI_ILA_RXBYTE_CNT_HI(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_ILA_RXBYTE_CNT_HI_ILA_RXBYTE_CNT_HI_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxByteCntLo - ILA Rx Byte Counter Low 32-bits
    Bits: 31:0  IlaRxByteCntLo(rc) - Lower 32-bits of 64-bit ILA Rx Byte count. Reading this register after reading the IlaRxByteCntHi register
*/
#define NFP_ILAX_CSR_ILA_RXBYTE_CNT_LO                       0x00000030
#define   NFP_ILAX_CSR_ILA_RXBYTE_CNT_LO_ILA_RXBYTE_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_ILA_RXBYTE_CNT_LO_ILA_RXBYTE_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxPktCntHi - ILA Rx Pkt Counter High 16-bits
    Bits: 15:0  IlaRxPktCntHi(rc) - Upper 16-bits of 48-bit ILA Rx Packet count. Reading this register latches the value in IlaRxPktCntLo
*/
#define NFP_ILAX_CSR_ILA_RXPKT_CNT_HI                        0x00000034
#define   NFP_ILAX_CSR_ILA_RXPKT_CNT_HI_ILA_RXPKT_CNT_HI(_x) (((_x) & 0xffff) << 0)
#define   NFP_ILAX_CSR_ILA_RXPKT_CNT_HI_ILA_RXPKT_CNT_HI_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: IlaRxPktCntLo - ILA Rx Pkt Counter Low 32-bits
    Bits: 31:0  IlaRxPktCntLo(rc) - Lower 32-bits of 48-bit ILA Rx Packet count. Reading this register after reading the IlaRxPktCntHi register
*/
#define NFP_ILAX_CSR_ILA_RXPKT_CNT_LO                        0x00000038
#define   NFP_ILAX_CSR_ILA_RXPKT_CNT_LO_ILA_RXPKT_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_ILA_RXPKT_CNT_LO_ILA_RXPKT_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxErrByteCntHi - ILA Rx Errored Byte Counter High 32-bits
    Bits: 31:0  IlaRxErrByteCntHi(rc) - Upper 32-bits of 64-bit ILA Rx Errored Byte count. Reading this register latches the value in IlaRxErrByteCntLo
*/
#define NFP_ILAX_CSR_ILA_RXERRBYTE_CNT_HI                    0x0000003c
#define   NFP_ILAX_CSR_ILA_RXERRBYTE_CNT_HI_ILA_RXERRBYTE_CNT_HI(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_ILA_RXERRBYTE_CNT_HI_ILA_RXERRBYTE_CNT_HI_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxErrByteCntLo - ILA Rx Errored Byte Counter Low 32-bits
    Bits: 31:0  IlaRxErrByteCntLo(rc) - Lower 32-bits of 64-bit ILA Rx Errored Byte count. Reading this register after reading the IlaRxErrByteCntHi register
*/
#define NFP_ILAX_CSR_ILA_RXERRBYTE_CNT_LO                    0x00000040
#define   NFP_ILAX_CSR_ILA_RXERRBYTE_CNT_LO_ILA_RXERRBYTE_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_ILA_RXERRBYTE_CNT_LO_ILA_RXERRBYTE_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxErrPktCntHi - ILA Rx Errored Pkt Counter High 16-bits
    Bits: 15:0  IlaRxErrPktCntHi(rc) - Upper 16-bits of 48-bit ILA Rx Errored Packet count. Reading this register latches the value in IlaRxErrPktCntLo
*/
#define NFP_ILAX_CSR_ILA_RXERRPKT_CNT_HI                     0x00000044
#define   NFP_ILAX_CSR_ILA_RXERRPKT_CNT_HI_ILA_RXERRPKT_CNT_HI(_x) (((_x) & 0xffff) << 0)
#define   NFP_ILAX_CSR_ILA_RXERRPKT_CNT_HI_ILA_RXERRPKT_CNT_HI_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: IlaRxErrPktCntLo - ILA Rx Errored Pkt Counter Low 32-bits
    Bits: 31:0  IlaRxErrPktCntLo(rc) - Lower 32-bits of 48-bit ILA Rx Errored Packet count. Reading this register after reading the IlaRxErrPktCntHi register
*/
#define NFP_ILAX_CSR_ILA_RXERRPKT_CNT_LO                     0x00000048
#define   NFP_ILAX_CSR_ILA_RXERRPKT_CNT_LO_ILA_RXERRPKT_CNT_LO(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_ILA_RXERRPKT_CNT_LO_ILA_RXERRPKT_CNT_LO_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRetDescStat - Return Descriptor FIFO Status
    Bits: 15:8  IlaRetDescCurDepth(ro) - Indicates the current depth the Return Descriptor FIFO
    Bits:  7:0  IlaRetDescMaxDepth(ro) - Indicates the maximum depth the Return Descriptor has reached.
*/
#define NFP_ILAX_CSR_ILA_RETDESC_STAT                        0x0000004c
#define   NFP_ILAX_CSR_ILA_RETDESC_STAT_ILA_RETDESC_CURDEPTH(_x) (((_x) & 0xff) << 8)
#define   NFP_ILAX_CSR_ILA_RETDESC_STAT_ILA_RETDESC_CURDEPTH_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_ILAX_CSR_ILA_RETDESC_STAT_ILA_RETDESC_MAXDEPTH(_x) (((_x) & 0xff) << 0)
#define   NFP_ILAX_CSR_ILA_RETDESC_STAT_ILA_RETDESC_MAXDEPTH_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: IlaErrStat - ILA Error Status Register.
    Bits: 13    InvalidTag(rw1c) - Set when Invalid Tags are received from the ILA Rx Interface.
    Bits: 12    RxErrAll(rw1c) - Set when all open channels are errored out by the ILA Rx Core.
    Bits: 11    CppTBadCmd(rw1c) - Indicates Illegal CPP Target Command.
    Bits: 10    TgtWrStartErr(rw1c) - Accel Target Write Command Start Error.
    Bits:  9    CppTCPullErr(rw1c) - Indicates data going to the ILA Tx interface encountered a pull data error.
    Bits:  8    CppTCPushErr(rw1c) - Indicates an ILA Rx error or propagated to the CPP Target push bus.
    Bits:  7    TgtWrEndErr(rw1c) - Accel Target Write Command End Error.
    Bits:  6    RxErr(rw1c) - Set when a packet error is signaled by the ILA Rx Core.
    Bits:  5    EopErr(rw1c) - Set when an EOP error is signaled by the ILA Rx Core
    Bits:  1    TimeOutErr(rw1c) - Set when no response has been received for a configurable amount of time.
    Bits:  0    IlaRxAlignErr(rw1c) - Set when the ILA Rx Alignment is lost. Indicates the SERDES lanes alignment has dropprd.
*/
#define NFP_ILAX_CSR_ILA_ERR_STAT                            0x00000050
#define   NFP_ILAX_CSR_ILA_ERR_STAT_INVALID_TAG              BIT(13)
#define   NFP_ILAX_CSR_ILA_ERR_STAT_RX_ERR_ALL               BIT(12)
#define   NFP_ILAX_CSR_ILA_ERR_STAT_CPPT_BAD_CMB             BIT(11)
#define   NFP_ILAX_CSR_ILA_ERR_STAT_TGT_WR_START_ERR         BIT(10)
#define   NFP_ILAX_CSR_ILA_ERR_STAT_CPP_TC_PULL_ERR          BIT(9)
#define   NFP_ILAX_CSR_ILA_ERR_STAT_CPP_TC_PUSH_ERR          BIT(8)
#define   NFP_ILAX_CSR_ILA_ERR_STAT_TGT_WR_END_ERR           BIT(7)
#define   NFP_ILAX_CSR_ILA_ERR_STAT_RX_ERR                   BIT(6)
#define   NFP_ILAX_CSR_ILA_ERR_STAT_EOP_ERR                  BIT(5)
#define   NFP_ILAX_CSR_ILA_ERR_STAT_TIME_OUT_ERR             BIT(1)
#define   NFP_ILAX_CSR_ILA_ERR_STAT_ILA_RX_ALIGN_ERR         BIT(0)
/*----------------------------------------------------------------
  Register: IlaErrMask - ILA Error Mask Register.
    Bits: 13    InvalidTagMask(rw) - Set to mask Invalid_TAG error interrupts.
    Bits: 12    RxErrAllMask(rw) - Set to mask Rx Err_All error interrupts.
    Bits: 11    CppTBadCmdMask(rw) - Set to mask CPPT Illegal Command error interrupts.
    Bits: 10    TgtWrStartErrMask(rw) - Set to mask Accel Target Write Command Start Error.
    Bits:  9    CppTCPullErrMask(rw) - Set to mask CPP Target pull data error interrupts
    Bits:  8    CppTCPushErrMask(rw) - Set to mask CPP Taget push data error interrupts.
    Bits:  7    TgtWrEndErrMask(rw) - Set to mask Accel Target Write Command End Error.
    Bits:  6    RxErrMask(rw) - Set to mask packet error interrupts.
    Bits:  5    EopErrMask(rw) - Set to mask EOP error interrrupts.
    Bits:  1    TimeOutErrMask(rw) - Set to mask Time out error interrupts.
    Bits:  0    IlaRxAlignErrMask(rw) - Set to mask ILA Rx alignment error interrupts.
*/
#define NFP_ILAX_CSR_ILA_ERR_MASK                            0x00000054
#define   NFP_ILAX_CSR_ILA_ERR_MASK_INVALID_TAG_MASK         BIT(13)
#define   NFP_ILAX_CSR_ILA_ERR_MASK_RX_ERR_ALL_MASK          BIT(12)
#define   NFP_ILAX_CSR_ILA_ERR_MASK_CPPT_BAD_CMB_MASK        BIT(11)
#define   NFP_ILAX_CSR_ILA_ERR_MASK_TGT_WR_START_ERR_MASK    BIT(10)
#define   NFP_ILAX_CSR_ILA_ERR_MASK_CPP_TC_PULL_ERR_MASK     BIT(9)
#define   NFP_ILAX_CSR_ILA_ERR_MASK_CPP_TC_PUSH_ERR_MASK     BIT(8)
#define   NFP_ILAX_CSR_ILA_ERR_MASK_TGT_WR_END_ERR_MASK      BIT(7)
#define   NFP_ILAX_CSR_ILA_ERR_MASK_RX_ERR_MASK              BIT(6)
#define   NFP_ILAX_CSR_ILA_ERR_MASK_EOP_ERR_MASK             BIT(5)
#define   NFP_ILAX_CSR_ILA_ERR_MASK_TIME_OUT_ERR_MASK        BIT(1)
#define   NFP_ILAX_CSR_ILA_ERR_MASK_ILA_RX_ALIGN_ERR_MASK    BIT(0)
/*----------------------------------------------------------------
  Register: IlaErrCnt - ILA Error Counter
    Bits: 29:20 EopErrCnt(ro) - Stores the ILA EOP_ERR Counter.
    Bits: 19:10 CppPullDataErrCnt(ro) - Stores the CPP Pull Data Error Counter.
    Bits:  9:0  CppPushDataErrCnt(ro) - Stores the CPP Push Data Error Counter.
*/
#define NFP_ILAX_CSR_ILA_ERR_CNT                             0x00000060
#define   NFP_ILAX_CSR_ILA_ERR_CNT_EOP_ERR_CNT(_x)           (((_x) & 0x3ff) << 20)
#define   NFP_ILAX_CSR_ILA_ERR_CNT_EOP_ERR_CNT_of(_x)        (((_x) >> 20) & 0x3ff)
#define   NFP_ILAX_CSR_ILA_ERR_CNT_CPP_PULL_DATA_ERR_CNT(_x) (((_x) & 0x3ff) << 10)
#define   NFP_ILAX_CSR_ILA_ERR_CNT_CPP_PULL_DATA_ERR_CNT_of(_x) (((_x) >> 10) & 0x3ff)
#define   NFP_ILAX_CSR_ILA_ERR_CNT_CPP_PUSH_DATA_ERR_CNT(_x) (((_x) & 0x3ff) << 0)
#define   NFP_ILAX_CSR_ILA_ERR_CNT_CPP_PUSH_DATA_ERR_CNT_of(_x) (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: IlaErrCnt_2 - ILA Error Counter 2
    Bits: 31:24 TimeOutErrCnt(ro) - Stores the Time Out Error Counter.
    Bits: 23:16 RxErrCnt(ro) - Stores the RX_ERR Error Counter.
    Bits: 15:8  ErrAllCnt(ro) - Stores the ERR_ALL Error Counter.
    Bits:  7:0  CppTBadCmdCnt(ro) - Stores the CPPT Bad Command Error Counter.
*/
#define NFP_ILAX_CSR_ILA_ERR_CNT_2                           0x00000068
#define   NFP_ILAX_CSR_ILA_ERR_CNT_2_TIME_OUT_ERR_CNT(_x)    (((_x) & 0xff) << 24)
#define   NFP_ILAX_CSR_ILA_ERR_CNT_2_TIME_OUT_ERR_CNT_of(_x) (((_x) >> 24) & 0xff)
#define   NFP_ILAX_CSR_ILA_ERR_CNT_2_RX_ERR_CNT(_x)          (((_x) & 0xff) << 16)
#define   NFP_ILAX_CSR_ILA_ERR_CNT_2_RX_ERR_CNT_of(_x)       (((_x) >> 16) & 0xff)
#define   NFP_ILAX_CSR_ILA_ERR_CNT_2_ERR_ALL_CNT(_x)         (((_x) & 0xff) << 8)
#define   NFP_ILAX_CSR_ILA_ERR_CNT_2_ERR_ALL_CNT_of(_x)      (((_x) >> 8) & 0xff)
#define   NFP_ILAX_CSR_ILA_ERR_CNT_2_CPPT_BAD_CMD_CNT(_x)    (((_x) & 0xff) << 0)
#define   NFP_ILAX_CSR_ILA_ERR_CNT_2_CPPT_BAD_CMD_CNT_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: IlaSramCtrlRtnDesc - SRAM Control register for ILA Return Descriptor SRAM
  Register: IlaSramCtrlRxFifo - SRAM Control register for ILA Rx FIFO SRAM
    Bits: 14    BC2_0(rw) - SRAM0: Bias Level Adjust Input. This is bias control for controlling the diode connected to VSSCORE.
    Bits: 13    BC1_0(rw) - SRAM0: Bias Level Adjust Input. This is bias control for controlling the diode connected to VSSCORE.
    Bits: 12    Test1B0(rw) - SRAM0: Test pin for Port B to bypass self-timed circuit. The external clock controls the read and write control signals.
    Bits: 11    Test1Test1A0(rw) - SRAM0: Test pin for Port A to bypass self-timed circuit. The external clock controls the read and write control signals.
    Bits: 10    RaRmOveride0(rw) - SRAM0: Enables the override of the sense-amp controls by both RmRma0 and Rmb0 fields.
    Bits:  9:8  Ra0(rw) - SRAM0: RA for 1-Port SRAMs. Read Assist(RA) to control WL under-drive. See SRAMs specs.
    Bits:  7:4  Rmb0(rw) - SRAM0: RMB for 2-port SRAMs. Read Margin Port B(RMB) sets the Read-Write margin by adjusting the sense amp differential setting. See SRAMs specs.
    Bits:  3:0  RmRma0(rw) - SRAM0: RM for 1-Port SRAMs / RMA for 2-port SRAMs. Read Margin(RM) / Read Margin Port A(RMA) sets the Read-Write margin by adjusting the sense amp differential setting. See SRAMs specs.
*/
#define NFP_ILAX_CSR_ILA_SRAM_CTRL_RDESC                     0x00000074
#define NFP_ILAX_CSR_ILA_SRAM_CTRL_RXFIFO                    0x00000078
#define   NFP_ILAX_CSR_ILASRAMCTRL_BC2_0                     BIT(14)
#define   NFP_ILAX_CSR_ILASRAMCTRL_BC1_0                     BIT(13)
#define   NFP_ILAX_CSR_ILASRAMCTRL_TEST1B_0                  BIT(12)
#define   NFP_ILAX_CSR_ILASRAMCTRL_TEST_TEST1A_0             BIT(11)
#define   NFP_ILAX_CSR_ILASRAMCTRL_RA_RM_OVRD_0              BIT(10)
#define   NFP_ILAX_CSR_ILASRAMCTRL_RA_0(_x)                  (((_x) & 0x3) << 8)
#define   NFP_ILAX_CSR_ILASRAMCTRL_RA_0_of(_x)               (((_x) >> 8) & 0x3)
#define   NFP_ILAX_CSR_ILASRAMCTRL_RMB_0(_x)                 (((_x) & 0xf) << 4)
#define   NFP_ILAX_CSR_ILASRAMCTRL_RMB_0_of(_x)              (((_x) >> 4) & 0xf)
#define   NFP_ILAX_CSR_ILASRAMCTRL_RM_RMA_0(_x)              (((_x) & 0xf) << 0)
#define   NFP_ILAX_CSR_ILASRAMCTRL_RM_RMA_0_of(_x)           (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: IlaTxErrCtrl - ILA Tx Error propagation control register
    Bits:  0    IlaTxErrDisable(rw) - ILA Transmit error propagation disable. When set data errors are disabled from propagating to the Interlaken_la transmit interface.
*/
#define NFP_ILAX_CSR_ILA_TXERR_CTRL                          0x00000088
#define   NFP_ILAX_CSR_ILA_TXERR_CTRL_ILA_TX_ERR_DISABLE     BIT(0)

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: IlaSSramEntry[16384] - ILA Shared SRAM Entry X
    Bits: 31:0  SSRAMData(rw) - 32-bits of data within the ILA Shared SRAM
*/
#define NFP_ILA_SRAM_IlaSSramEntry(_x)                       (0x00000000 + (0x4 * ((_x) & 0x3fff)))
#define   NFP_ILA_SRAM_IlaSSramEntry_SSRAMData(_x)           (((_x) & 0xffffffff) << 0)
#define   NFP_ILA_SRAM_IlaSSramEntry_SSRAMData_of(_x)        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaErrorCode - ACCEL Mode ILA Error Code Command
    Bits:  1    IlaErrorCodeGen(rw) - Enable ILA Error Code Command
    Bits:  0    IlaErrorCodeChan(rw) - Select ILA Error Code Command Channel
*/
#define NFP_ILA_ERROR_CODE_IlaErrorCode                      0x00000000
#define   NFP_ILA_ERROR_CODE_IlaErrorCode_IlaErrorCodeGen    BIT(1)
#define   NFP_ILA_ERROR_CODE_IlaErrorCode_IlaErrorCodeChan   BIT(0)
/*----------------------------------------------------------------
  Register: IlaToCppBar[8] - ILA to CPP translation BAR
    Bits: 29:25 TargetID(rw) - CPP target ID. Local Registers are accessed when TargetID=0
    Bits: 24:23 Token(rw) - CPP token field.
    Bits: 22    LengthSelect(rw) - Used to select the size of the CPP length field.
*/
#define NFP_ILA_BARCFG_IlaToCppBar(_x)                       (0x00000000 + (0x4 * ((_x) & 0x7)))
#define   NFP_ILA_BARCFG_IlaToCppBar_TargetID(_x)            (((_x) & 0x1f) << 25)
#define   NFP_ILA_BARCFG_IlaToCppBar_TargetID_of(_x)         (((_x) >> 25) & 0x1f)
#define   NFP_ILA_BARCFG_IlaToCppBar_Token(_x)               (((_x) & 0x3) << 23)
#define   NFP_ILA_BARCFG_IlaToCppBar_Token_of(_x)            (((_x) >> 23) & 0x3)
#define   NFP_ILA_BARCFG_IlaToCppBar_LengthSelect            BIT(22)
#define     NFP_ILA_BARCFG_IlaToCppBar_LengthSelect_32BIT    (0 << 22)
#define     NFP_ILA_BARCFG_IlaToCppBar_LengthSelect_64BIT    BIT(22)
/*----------------------------------------------------------------
  Register: CppToIlaBar[8] - CPP to ILA translation BAR
    Bits: 21    IlaChSel(rw) - Selects which ILA Channel to send the command on
    Bits: 20:0  Address(rw) - In 32-bit mode: ILA address bits [47:27]; In 40-bit mode: ILA address bits[47:35]
*/
#define NFP_ILA_BARCFG_CppToIlaBar(_x)                       (0x00000020 + (0x4 * ((_x) & 0x7)))
#define   NFP_ILA_BARCFG_CppToIlaBar_IlaChSel                BIT(21)
#define   NFP_ILA_BARCFG_CppToIlaBar_Address(_x)             (((_x) & 0x1fffff) << 0)
#define   NFP_ILA_BARCFG_CppToIlaBar_Address_of(_x)          (((_x) >> 0) & 0x1fffff)
/*----------------------------------------------------------------
  Register: DmaCommandCh0ToIla - Insert command into Ch0ToIla DMA command queue
  Register: DmaCommandCh1ToIla - Insert command into Ch1ToIla DMA command queue
  Register: DmaCommandCh0FrmIla - Insert command into Ch0FrmIla DMA command queue
  Register: DmaCommandCh1FrmIla - Insert command into Ch1FrmIla DMA command queue
    Bits: 127:116 XferLength(wo) - Length of transfer in bytes
    Bits: 111:96 IlaAddressHi(wo) - Upper 16 bits of 48-bit ILA address
    Bits: 95:66 IlaAddressLo(wo) - Next 30 bits of 48-bit ILA address
    Bits: 65:48 CompletionIndication(wo) - Event to generate on completed command. Can be either no signal, generate an event on the event bus, or send a signal directly to a CPP master.
    Bits: 47:46 Token(wo) - CPP token bits for CPP command
    Bits: 45    SignalBothOnErrEn(wo) - Use to enable signaling both odd and even signals when a DMA encounters an ILA Rx Error.
    Bits: 44    Target64(wo) - CPP target is 64-bit. Set if target supports 64-bit transactions, or cleared if target only supports 32-bit transaction.
    Bits: 43:40 CppTarget(wo) - Target ID for CPP transaction
    Bits: 39:32 CppAddressHi(wo) - CPP bus address
    Bits: 31:0  CppAddressLo(wo) - CPP bus address
*/
#define NFP_ILA_DMA_DmaCommandCh0ToIla                       0x00000000
#define NFP_ILA_DMA_DmaCommandCh1ToIla                       0x00000010
#define NFP_ILA_DMA_DmaCommandCh0FrmIla                      0x00000020
#define NFP_ILA_DMA_DmaCommandCh1FrmIla                      0x00000030
#define   NFP_ILA_DMA_IlaDmaCommand_XferLength(_x)           (((_x) & 0xfff) << 116)
#define   NFP_ILA_DMA_IlaDmaCommand_XferLength_of(_x)        (((_x) >> 116) & 0xfff)
#define   NFP_ILA_DMA_IlaDmaCommand_IlaAddressHi(_x)         (((_x) & 0xffff) << 96)
#define   NFP_ILA_DMA_IlaDmaCommand_IlaAddressHi_of(_x)      (((_x) >> 96) & 0xffff)
#define   NFP_ILA_DMA_IlaDmaCommand_IlaAddressLo(_x)         (((_x) & 0x3fffffff) << 66)
#define   NFP_ILA_DMA_IlaDmaCommand_IlaAddressLo_of(_x)      (((_x) >> 66) & 0x3fffffff)
#define   NFP_ILA_DMA_IlaDmaCommand_CompletionIndication(_x) (((_x) & 0x3ffff) << 48)
#define   NFP_ILA_DMA_IlaDmaCommand_CompletionIndication_of(_x) (((_x) >> 48) & 0x3ffff)
#define   NFP_ILA_DMA_IlaDmaCommand_Token(_x)                (((_x) & 0x3) << 46)
#define   NFP_ILA_DMA_IlaDmaCommand_Token_of(_x)             (((_x) >> 46) & 0x3)
#define   NFP_ILA_DMA_IlaDmaCommand_SignalBothOnErrEn        BIT_ULL(45)
#define     NFP_ILA_DMA_IlaDmaCommand_SignalBothOnErrEn_Dont_assert_no_split (0 << 45)
#define     NFP_ILA_DMA_IlaDmaCommand_SignalBothOnErrEn_Assert_no_split BIT_ULL(45)
#define   NFP_ILA_DMA_IlaDmaCommand_Target64                 BIT_ULL(44)
#define     NFP_ILA_DMA_IlaDmaCommand_Target64_DISABLE       (0 << 44)
#define     NFP_ILA_DMA_IlaDmaCommand_Target64_ENABLE        BIT_ULL(44)
#define   NFP_ILA_DMA_IlaDmaCommand_CppTarget(_x)            (((_x) & 0xf) << 40)
#define   NFP_ILA_DMA_IlaDmaCommand_CppTarget_of(_x)         (((_x) >> 40) & 0xf)
#define   NFP_ILA_DMA_IlaDmaCommand_CppAddressHi(_x)         (((_x) & 0xff) << 32)
#define   NFP_ILA_DMA_IlaDmaCommand_CppAddressHi_of(_x)      (((_x) >> 32) & 0xff)
#define   NFP_ILA_DMA_IlaDmaCommand_CppAddressLo(_x)         (((_x) & 0xffffffff) << 0)
#define   NFP_ILA_DMA_IlaDmaCommand_CppAddressLo_of(_x)      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: DmaCtrlCh0ToIla - Control Ch0ToIla DMA command queue
  Register: DmaCtrlCh1ToIla - Control Ch1ToIla DMA command queue
  Register: DmaCtrlCh0FrmIla - Control Ch0FrmIla DMA command queue
  Register: DmaCtrlCh1FrmIla - Control Ch1FrmIla DMA command queue
    Bits:  5    DescReadError(w1c) - Set to clear the error indication for descriptor reads. Setting to 0 has no effect.
    Bits:  4    IlaError(w1c) - Set to clear the error indication for ILA transaction failures. Setting to 0 has no effect.
    Bits:  2    QueueOverflow(w1c) - Set to clear queue overflow indication. Setting to 0 has no effect
    Bits:  1    ContinueOnError(rw) - Setting this bit will stop errors from blocking DMAs
    Bits:  0    QueueStop(rw) - Setting this bit will stop DMA engine from executing from queue. Clearing the bit will start up execution again.
*/
#define NFP_ILA_DMA_DmaCtrlCh0ToIla                          0x00000040
#define NFP_ILA_DMA_DmaCtrlCh1ToIla                          0x00000044
#define NFP_ILA_DMA_DmaCtrlCh0FrmIla                         0x00000048
#define NFP_ILA_DMA_DmaCtrlCh1FrmIla                         0x0000004c
#define   NFP_ILA_DMA_IlaDmaControl_DescReadError            BIT(5)
#define     NFP_ILA_DMA_IlaDmaControl_DescReadError_No_Effect (0 << 5)
#define     NFP_ILA_DMA_IlaDmaControl_DescReadError_CLEAR    BIT(5)
#define   NFP_ILA_DMA_IlaDmaControl_IlaError                 BIT(4)
#define     NFP_ILA_DMA_IlaDmaControl_IlaError_No_Effect     (0 << 4)
#define     NFP_ILA_DMA_IlaDmaControl_IlaError_CLEAR         BIT(4)
#define   NFP_ILA_DMA_IlaDmaControl_QueueOverflow            BIT(2)
#define     NFP_ILA_DMA_IlaDmaControl_QueueOverflow_No_Effect (0 << 2)
#define     NFP_ILA_DMA_IlaDmaControl_QueueOverflow_CLEAR    BIT(2)
#define   NFP_ILA_DMA_IlaDmaControl_ContinueOnError          BIT(1)
#define     NFP_ILA_DMA_IlaDmaControl_ContinueOnError_DISABLE (0 << 1)
#define     NFP_ILA_DMA_IlaDmaControl_ContinueOnError_ENABLE BIT(1)
#define   NFP_ILA_DMA_IlaDmaControl_QueueStop                BIT(0)
#define     NFP_ILA_DMA_IlaDmaControl_QueueStop_DISABLE      (0 << 0)
#define     NFP_ILA_DMA_IlaDmaControl_QueueStop_ENABLE       BIT(0)
/*----------------------------------------------------------------
  Register: DmaWritePtrCh0ToIla - Write pointer for Ch0ToIla DMA command queue
  Register: DmaWritePtrCh1ToIla - Write pointer for Ch1ToIla DMA command queue
    Bits: 23:16 CmplFifoPopPtr(ro) - Number of entries popped from the Completion FIFO.
    Bits:  8:0  Pointer(ro) - Write pointer address in queue memory
*/
#define NFP_ILA_DMA_DmaWritePtrCh0ToIla                      0x00000050
#define NFP_ILA_DMA_DmaWritePtrCh1ToIla                      0x00000060
#define   NFP_ILA_DMA_IlaToDmaWritePtr_CmplFifoPopPtr(_x)    (((_x) & 0xff) << 16)
#define   NFP_ILA_DMA_IlaToDmaWritePtr_CmplFifoPopPtr_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_ILA_DMA_IlaToDmaWritePtr_Pointer(_x)           (((_x) & 0x1ff) << 0)
#define   NFP_ILA_DMA_IlaToDmaWritePtr_Pointer_of(_x)        (((_x) >> 0) & 0x1ff)
/*----------------------------------------------------------------
  Register: DmaWritePtrCh0FrmIla - Write pointer for Ch0FrmIla DMA command queue
  Register: DmaWritePtrCh1FrmIla - Write pointer for Ch1FrmIla DMA command queue
    Bits:  8:0  Pointer(ro) - Write pointer address in queue memory
*/
#define NFP_ILA_DMA_DmaWritePtrCh0FrmIla                     0x00000070
#define NFP_ILA_DMA_DmaWritePtrCh1FrmIla                     0x00000080
#define   NFP_ILA_DMA_IlaFrmDmaWritePtr_Pointer(_x)          (((_x) & 0x1ff) << 0)
#define   NFP_ILA_DMA_IlaFrmDmaWritePtr_Pointer_of(_x)       (((_x) >> 0) & 0x1ff)
/*----------------------------------------------------------------
  Register: DmaQStatusCh0ToIla - Queue status for Ch0ToIla DMA command queue
  Register: DmaQStatusCh1ToIla - Queue status for Ch1ToIla DMA command queue
  Register: DmaQStatusCh0FrmIla - Queue status for Ch0FrmIla DMA command queue
  Register: DmaQStatusCh1FrmIla - Queue status for Ch1FrmIla DMA command queue
    Bits: 31:24 Avail(ro) - Amount of available space in queue. A value of 0 indicates 256 entries available. Use QueueFull flag to determine if there is no space.
    Bits: 23:16 ReadPointer(ro) - Pointer to the next descriptor to be issued by the DMA engine.
    Bits: 15:8  PopPointer(ro) - Pointer to the next descriptor to be completed by the DMA engine. The DMA queue write pointer can be calculated as (DmaQueueStatus[PopPointer] - DmaQueueStatus[Avail]) mod 256.
    Bits:  7    QueueFull(ro) - Set when queue is full, cleared otherwise.
    Bits:  5    DescReadError(ro) - Set when a multi-bit error is detected on reading the descriptor from the SRAM queue. Must be cleared explicitly by writing the DMA Queue Control register.
    Bits:  4    IlaError(ro) - Set when an error is detected in the received ILA transaction. Must be cleared explicitly by writing the DMA Queue Control register. Specific error latched in the IlaErrStat register.
    Bits:  2    QueueOverflow(ro) - Set when a descriptor is written to a full queue. Must be cleared explicitly by writing the DMA Queue Control register.
    Bits:  1    ContinueOnError(ro) - If set errors will not block further DMAs.
    Bits:  0    QueueStop(ro) - Set when the DMA engine is stopped due to an error, or if DMA engine is explicitly stopped by writing to the DMA Queue Control Register.
*/
#define NFP_ILA_DMA_DmaQStatusCh0ToIla                       0x00000090
#define NFP_ILA_DMA_DmaQStatusCh1ToIla                       0x00000094
#define NFP_ILA_DMA_DmaQStatusCh0FrmIla                      0x00000098
#define NFP_ILA_DMA_DmaQStatusCh1FrmIla                      0x0000009c
#define   NFP_ILA_DMA_IlaDmaQueueStatus_Avail(_x)            (((_x) & 0xff) << 24)
#define   NFP_ILA_DMA_IlaDmaQueueStatus_Avail_of(_x)         (((_x) >> 24) & 0xff)
#define   NFP_ILA_DMA_IlaDmaQueueStatus_ReadPointer(_x)      (((_x) & 0xff) << 16)
#define   NFP_ILA_DMA_IlaDmaQueueStatus_ReadPointer_of(_x)   (((_x) >> 16) & 0xff)
#define   NFP_ILA_DMA_IlaDmaQueueStatus_PopPointer(_x)       (((_x) & 0xff) << 8)
#define   NFP_ILA_DMA_IlaDmaQueueStatus_PopPointer_of(_x)    (((_x) >> 8) & 0xff)
#define   NFP_ILA_DMA_IlaDmaQueueStatus_QueueFull            BIT(7)
#define   NFP_ILA_DMA_IlaDmaQueueStatus_DescReadError        BIT(5)
#define   NFP_ILA_DMA_IlaDmaQueueStatus_IlaError             BIT(4)
#define   NFP_ILA_DMA_IlaDmaQueueStatus_QueueOverflow        BIT(2)
#define   NFP_ILA_DMA_IlaDmaQueueStatus_ContinueOnError      BIT(1)
#define   NFP_ILA_DMA_IlaDmaQueueStatus_QueueStop            BIT(0)
/*----------------------------------------------------------------
  Register: DmaCommandToIlaTcam - Insert command into ToIla TCAM DMA command queue
    Bits: 127:116 XferLength(wo) - Length of transfer in bytes
    Bits: 110    Target64Ret(wo) - CPP target is 64-bit for return transaction. Set if target supports 64-bit transactions, or cleared if target only supports 32-bit transaction.
    Bits: 109:108 TokenRet(wo) - CPP token bits for return CPP command
    Bits: 107:104 CppTargetRet(wo) - Target ID for CPP return transaction
    Bits: 103:96 CppAddressHiRet(wo) - Upper 8-bits of the 40-bit return CPP Address used to write the return TCAM data.
    Bits: 95:66 CppAddressLoRet(wo) - Next 30 bits of 40-bit return CPP address used to write the return TCAM data.
    Bits: 65:48 CompletionIndication(wo) - Event to generate on completed command. Can be either no signal, generate an event on the event bus, or send a signal directly to a CPP master.
    Bits: 47:46 Token(wo) - CPP token bits for CPP command
    Bits: 45    SignalBothOnErrEn(wo) - Use to enable signaling both odd and even signals when a DMA encounters an ILA Rx Error.
    Bits: 44    Target64(wo) - CPP target is 64-bit. Set if target supports 64-bit transactions, or cleared if target only supports 32-bit transaction.
    Bits: 43:40 CppTarget(wo) - Target ID for CPP transaction
    Bits: 39:32 CppAddressHi(wo) - CPP bus address
    Bits: 31:0  CppAddressLo(wo) - CPP bus address
*/
#define NFP_ILA_TCAM_DmaCommandToIlaTcam                     0x00000000
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_XferLength(_x)    (((_x) & 0xfff) << 116)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_XferLength_of(_x) (((_x) >> 116) & 0xfff)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_Target64Ret       BIT_ULL(110)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_TokenRet(_x)      (((_x) & 0x3) << 108)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_TokenRet_of(_x)   (((_x) >> 108) & 0x3)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CppTargetRet(_x)  (((_x) & 0xf) << 104)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CppTargetRet_of(_x) (((_x) >> 104) & 0xf)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CppAddressHiRet(_x) (((_x) & 0xff) << 96)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CppAddressHiRet_of(_x) (((_x) >> 96) & 0xff)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CppAddressLoRet(_x) (((_x) & 0x3fffffff) << 66)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CppAddressLoRet_of(_x) (((_x) >> 66) & 0x3fffffff)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CompletionIndication(_x) (((_x) & 0x3ffff) << 48)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CompletionIndication_of(_x) (((_x) >> 48) & 0x3ffff)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_Token(_x)         (((_x) & 0x3) << 46)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_Token_of(_x)      (((_x) >> 46) & 0x3)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_SignalBothOnErrEn BIT_ULL(45)
#define     NFP_ILA_TCAM_DmaCommandToIlaTcam_SignalBothOnErrEn_Dont_assert_no_split (0 << 45)
#define     NFP_ILA_TCAM_DmaCommandToIlaTcam_SignalBothOnErrEn_Assert_no_split BIT_ULL(45)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_Target64          BIT_ULL(44)
#define     NFP_ILA_TCAM_DmaCommandToIlaTcam_Target64_DISABLE (0 << 44)
#define     NFP_ILA_TCAM_DmaCommandToIlaTcam_Target64_ENABLE BIT_ULL(44)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CppTarget(_x)     (((_x) & 0xf) << 40)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CppTarget_of(_x)  (((_x) >> 40) & 0xf)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CppAddressHi(_x)  (((_x) & 0xff) << 32)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CppAddressHi_of(_x) (((_x) >> 32) & 0xff)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CppAddressLo(_x)  (((_x) & 0xffffffff) << 0)
#define   NFP_ILA_TCAM_DmaCommandToIlaTcam_CppAddressLo_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: DmaCtrlToIlaTcam - Control DMA command queue for TCAM operations
    Bits:  5    DescReadError(w1c) - Set to clear the error indication for descriptor reads. Setting to 0 has no effect.
    Bits:  4    IlaError(w1c) - Set to clear the error indication for ILA transaction failures. Setting to 0 has no effect.
    Bits:  2    QueueOverflow(w1c) - Set to clear queue overflow indication. Setting to 0 has no effect
    Bits:  1    ContinueOnError(rw) - Setting this bit will stop errors from blocking DMAs
    Bits:  0    QueueStop(rw) - Setting this bit will stop DMA engine from executing from queue. Clearing the bit will start up execution again.
*/
#define NFP_ILA_TCAM_DmaCtrlToIlaTcam                        0x00000040
#define   NFP_ILA_TCAM_DmaCtrlToIlaTcam_DescReadError        BIT(5)
#define     NFP_ILA_TCAM_DmaCtrlToIlaTcam_DescReadError_No_Effect (0 << 5)
#define     NFP_ILA_TCAM_DmaCtrlToIlaTcam_DescReadError_CLEAR BIT(5)
#define   NFP_ILA_TCAM_DmaCtrlToIlaTcam_IlaError             BIT(4)
#define     NFP_ILA_TCAM_DmaCtrlToIlaTcam_IlaError_No_Effect (0 << 4)
#define     NFP_ILA_TCAM_DmaCtrlToIlaTcam_IlaError_CLEAR     BIT(4)
#define   NFP_ILA_TCAM_DmaCtrlToIlaTcam_QueueOverflow        BIT(2)
#define     NFP_ILA_TCAM_DmaCtrlToIlaTcam_QueueOverflow_No_Effect (0 << 2)
#define     NFP_ILA_TCAM_DmaCtrlToIlaTcam_QueueOverflow_CLEAR BIT(2)
#define   NFP_ILA_TCAM_DmaCtrlToIlaTcam_ContinueOnError      BIT(1)
#define     NFP_ILA_TCAM_DmaCtrlToIlaTcam_ContinueOnError_DISABLE (0 << 1)
#define     NFP_ILA_TCAM_DmaCtrlToIlaTcam_ContinueOnError_ENABLE BIT(1)
#define   NFP_ILA_TCAM_DmaCtrlToIlaTcam_QueueStop            BIT(0)
#define     NFP_ILA_TCAM_DmaCtrlToIlaTcam_QueueStop_DISABLE  (0 << 0)
#define     NFP_ILA_TCAM_DmaCtrlToIlaTcam_QueueStop_ENABLE   BIT(0)
/*----------------------------------------------------------------
  Register: DmaWritePtrToIlaTcam - Write pointer for ToIla TCAM DMA command queue
    Bits: 23:16 CmplFifoPopPtr(ro) - Number of entries popped from the Completion FIFO.
    Bits:  8:0  Pointer(ro) - Write pointer address in queue memory
*/
#define NFP_ILA_TCAM_DmaWritePtrToIlaTcam                    0x00000050
#define   NFP_ILA_TCAM_DmaWritePtrToIlaTcam_CmplFifoPopPtr(_x) (((_x) & 0xff) << 16)
#define   NFP_ILA_TCAM_DmaWritePtrToIlaTcam_CmplFifoPopPtr_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_ILA_TCAM_DmaWritePtrToIlaTcam_Pointer(_x)      (((_x) & 0x1ff) << 0)
#define   NFP_ILA_TCAM_DmaWritePtrToIlaTcam_Pointer_of(_x)   (((_x) >> 0) & 0x1ff)
/*----------------------------------------------------------------
  Register: DmaQStatusToIlaTcam - Queue status for ToIla TCAM DMA command queue
    Bits: 31:24 Avail(ro) - Amount of available space in queue. A value of 0 indicates 256 entries available. Use QueueFull flag to determine if there is no space.
    Bits: 23:16 ReadPointer(ro) - Pointer to the next descriptor to be issued by the DMA engine.
    Bits: 15:8  PopPointer(ro) - Pointer to the next descriptor to be completed by the DMA engine. The DMA queue write pointer can be calculated as (DmaQueueStatus[PopPointer] - DmaQueueStatus[Avail]) mod 256.
    Bits:  7    QueueFull(ro) - Set when queue is full, cleared otherwise.
    Bits:  5    DescReadError(ro) - Set when a multi-bit error is detected on reading the descriptor from the SRAM queue. Must be cleared explicitly by writing the DMA Queue Control register.
    Bits:  4    IlaError(ro) - Set when an error is detected in the received ILA transaction. Must be cleared explicitly by writing the DMA Queue Control register. Specific error latched in the IlaErrStat register.
    Bits:  2    QueueOverflow(ro) - Set when a descriptor is written to a full queue. Must be cleared explicitly by writing the DMA Queue Control register.
    Bits:  1    ContinueOnError(ro) - If set errors will not block further DMAs.
    Bits:  0    QueueStop(ro) - Set when the DMA engine is stopped due to an error, or if DMA engine is explicitly stopped by writing to the DMA Queue Control Register.
*/
#define NFP_ILA_TCAM_DmaQStatusToIlaTcam                     0x00000090
#define   NFP_ILA_TCAM_DmaQStatusToIlaTcam_Avail(_x)         (((_x) & 0xff) << 24)
#define   NFP_ILA_TCAM_DmaQStatusToIlaTcam_Avail_of(_x)      (((_x) >> 24) & 0xff)
#define   NFP_ILA_TCAM_DmaQStatusToIlaTcam_ReadPointer(_x)   (((_x) & 0xff) << 16)
#define   NFP_ILA_TCAM_DmaQStatusToIlaTcam_ReadPointer_of(_x) (((_x) >> 16) & 0xff)
#define   NFP_ILA_TCAM_DmaQStatusToIlaTcam_PopPointer(_x)    (((_x) & 0xff) << 8)
#define   NFP_ILA_TCAM_DmaQStatusToIlaTcam_PopPointer_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_ILA_TCAM_DmaQStatusToIlaTcam_QueueFull         BIT(7)
#define   NFP_ILA_TCAM_DmaQStatusToIlaTcam_DescReadError     BIT(5)
#define   NFP_ILA_TCAM_DmaQStatusToIlaTcam_IlaError          BIT(4)
#define   NFP_ILA_TCAM_DmaQStatusToIlaTcam_QueueOverflow     BIT(2)
#define   NFP_ILA_TCAM_DmaQStatusToIlaTcam_ContinueOnError   BIT(1)
#define   NFP_ILA_TCAM_DmaQStatusToIlaTcam_QueueStop         BIT(0)
/*----------------------------------------------------------------
  Register: IlaCtrlCfg - Selects operation mode for the ILA controller
    Bits: 20    IlaRxFifoPack(rw) - ILA RX FIFO PACKING
    Bits: 19:10 IlaTxIdleCnt(rw) - Controls how often idle packets are transmitted. Idle packets communicate the LCC value to the peer device when no active packets are being transmitted. Software must program this value AFTER changing the IlaMode=ACCEL. The recommended value is 15.
    Bits:  9    IlaRxFifoSize(rw) - ILA RX FIFO SIZE
    Bits:  8    IlaTxWrCreditCntEna(rw) - ILA TX Credit Counter Enable
    Bits:  5    IlaErrPoisonEn(rw) - Enables poisoning of push or pull data upon receipt of an ILA Receive Error. Poisoning the data sets the data_error flag on the bus during the data transfer.
    Bits:  4    IlaCtrlRsvd2(rw) - Reserved
    Bits:  3    BurstMax(rw) - Controls the maximum size of a burst on the Interlaken Look-Aside bus. Must match the CSR setting in the Interlaken Look-Aside IP core (See MAC block registers).
    Bits:  2    IlaCtrlRsvd(rw) - Reserved for future use
    Bits:  1    IlaMode(rw) - Configures the Mode of ILA Block
    Bits:  0    IlaCppAddrMode(rw) - Configure whether to interpret the CPP address bits as 32-bit or 40-bit.
*/
#define NFP_ILAX_CSR_IlaCtrlCfg                              0x00000000
#define   NFP_ILAX_CSR_IlaCtrlCfg_IlaRxFifoPack              BIT(20)
#define     NFP_ILAX_CSR_IlaCtrlCfg_IlaRxFifoPack_ILARXFIFOPACKENA (0 << 20)
#define     NFP_ILAX_CSR_IlaCtrlCfg_IlaRxFifoPack_ILARXFIFOPACKDIS BIT(20)
#define   NFP_ILAX_CSR_IlaCtrlCfg_IlaTxIdleCnt(_x)           (((_x) & 0x3ff) << 10)
#define   NFP_ILAX_CSR_IlaCtrlCfg_IlaTxIdleCnt_of(_x)        (((_x) >> 10) & 0x3ff)
#define   NFP_ILAX_CSR_IlaCtrlCfg_IlaRxFifoSize              BIT(9)
#define     NFP_ILAX_CSR_IlaCtrlCfg_IlaRxFifoSize_ILARXFIFO4k (0 << 9)
#define     NFP_ILAX_CSR_IlaCtrlCfg_IlaRxFifoSize_ILARXFIFO8K BIT(9)
#define   NFP_ILAX_CSR_IlaCtrlCfg_IlaTxWrCreditCntEna        BIT(8)
#define     NFP_ILAX_CSR_IlaCtrlCfg_IlaTxWrCreditCntEna_DIS  (0 << 8)
#define     NFP_ILAX_CSR_IlaCtrlCfg_IlaTxWrCreditCntEna_ENA  BIT(8)
#define   NFP_ILAX_CSR_IlaCtrlCfg_IlaErrPoisonEn             BIT(5)
#define     NFP_ILAX_CSR_IlaCtrlCfg_IlaErrPoisonEn_Disable   (0 << 5)
#define     NFP_ILAX_CSR_IlaCtrlCfg_IlaErrPoisonEn_Enable    BIT(5)
#define   NFP_ILAX_CSR_IlaCtrlCfg_IlaCtrlRsvd2               BIT(4)
#define   NFP_ILAX_CSR_IlaCtrlCfg_BurstMax                   BIT(3)
#define     NFP_ILAX_CSR_IlaCtrlCfg_BurstMax_256             (0 << 3)
#define     NFP_ILAX_CSR_IlaCtrlCfg_BurstMax_512             BIT(3)
#define   NFP_ILAX_CSR_IlaCtrlCfg_IlaCtrlRsvd                BIT(2)
#define   NFP_ILAX_CSR_IlaCtrlCfg_IlaMode                    BIT(1)
#define     NFP_ILAX_CSR_IlaCtrlCfg_IlaMode_TCAM             (0 << 1)
#define     NFP_ILAX_CSR_IlaCtrlCfg_IlaMode_ACCEL            BIT(1)
#define   NFP_ILAX_CSR_IlaCtrlCfg_IlaCppAddrMode             BIT(0)
#define     NFP_ILAX_CSR_IlaCtrlCfg_IlaCppAddrMode_40        (0 << 0)
#define     NFP_ILAX_CSR_IlaCtrlCfg_IlaCppAddrMode_32        BIT(0)
/*----------------------------------------------------------------
  Register: IlaSpare - General Purpose Register
    Bits: 31:0  IlaSpare(rw) - General use register.
*/
#define NFP_ILAX_CSR_IlaSpare                                0x00000004
#define   NFP_ILAX_CSR_IlaSpare_IlaSpare(_x)                 (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_IlaSpare_IlaSpare_of(_x)              (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxChTimeOutIntvl - ILA Rx Channel 1 and 0 Timeout Interval
    Bits: 31:16 IlaRxTimeOutIntvlCh1(rw) - Time allowed for a response from the attached device in increments of 256ns. A value of zero disables the timer.
    Bits: 15:0  IlaRxTimeOutIntvlCh0(rw) - Time allowed for a response from the attached device in increments of 256ns. A value of zero disables the timer.
*/
#define NFP_ILAX_CSR_IlaRxChTimeOutIntvl                     0x0000000c
#define   NFP_ILAX_CSR_IlaRxChTimeOutIntvl_IlaRxTimeOutIntvlCh1(_x) (((_x) & 0xffff) << 16)
#define   NFP_ILAX_CSR_IlaRxChTimeOutIntvl_IlaRxTimeOutIntvlCh1_of(_x) (((_x) >> 16) & 0xffff)
#define   NFP_ILAX_CSR_IlaRxChTimeOutIntvl_IlaRxTimeOutIntvlCh0(_x) (((_x) & 0xffff) << 0)
#define   NFP_ILAX_CSR_IlaRxChTimeOutIntvl_IlaRxTimeOutIntvlCh0_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: IlaTxByteCntHi - ILA Tx Byte Counter High 32-bits
    Bits: 31:0  IlaTxByteCntHi(rc) - Upper 32-bits of 64-bit ILA Tx Byte count. Reading this register latches the value in IlaTxByteCntLo
*/
#define NFP_ILAX_CSR_IlaTxByteCntHi                          0x0000001c
#define   NFP_ILAX_CSR_IlaTxByteCntHi_IlaTxByteCntHi(_x)     (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_IlaTxByteCntHi_IlaTxByteCntHi_of(_x)  (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaTxByteCntLo - ILA Tx Byte Counter Low 32-bits
    Bits: 31:0  IlaTxByteCntLo(rc) - Lower 32-bits of 64-bit ILA Tx Byte count. Reading this register after reading the IlaTxByteCntHi register
*/
#define NFP_ILAX_CSR_IlaTxByteCntLo                          0x00000020
#define   NFP_ILAX_CSR_IlaTxByteCntLo_IlaTxByteCntLo(_x)     (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_IlaTxByteCntLo_IlaTxByteCntLo_of(_x)  (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaTxPktCntHi - ILA Tx Pkt Counter High 16-bits
    Bits: 15:0  IlaTxPktCntHi(rc) - Upper 16-bits of 48-bit ILA Tx Packet count. Reading this register latches the value in IlaTxPktCntLo
*/
#define NFP_ILAX_CSR_IlaTxPktCntHi                           0x00000024
#define   NFP_ILAX_CSR_IlaTxPktCntHi_IlaTxPktCntHi(_x)       (((_x) & 0xffff) << 0)
#define   NFP_ILAX_CSR_IlaTxPktCntHi_IlaTxPktCntHi_of(_x)    (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: IlaTxPktCntLo - ILA Tx Pkt Counter Low 32-bits
    Bits: 31:0  IlaTxPktCntLo(rc) - Lower 32-bits of 48-bit ILA Tx Packet count. Reading this register after reading the IlaTxPktCntHi register
*/
#define NFP_ILAX_CSR_IlaTxPktCntLo                           0x00000028
#define   NFP_ILAX_CSR_IlaTxPktCntLo_IlaTxPktCntLo(_x)       (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_IlaTxPktCntLo_IlaTxPktCntLo_of(_x)    (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxByteCntHi - ILA Rx Byte Counter High 32-bits
    Bits: 31:0  IlaRxByteCntHi(rc) - Upper 32-bits of 64-bit ILA Rx Byte count. Reading this register latches the value in IlaRxByteCntLo
*/
#define NFP_ILAX_CSR_IlaRxByteCntHi                          0x0000002c
#define   NFP_ILAX_CSR_IlaRxByteCntHi_IlaRxByteCntHi(_x)     (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_IlaRxByteCntHi_IlaRxByteCntHi_of(_x)  (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxByteCntLo - ILA Rx Byte Counter Low 32-bits
    Bits: 31:0  IlaRxByteCntLo(rc) - Lower 32-bits of 64-bit ILA Rx Byte count. Reading this register after reading the IlaRxByteCntHi register
*/
#define NFP_ILAX_CSR_IlaRxByteCntLo                          0x00000030
#define   NFP_ILAX_CSR_IlaRxByteCntLo_IlaRxByteCntLo(_x)     (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_IlaRxByteCntLo_IlaRxByteCntLo_of(_x)  (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxPktCntHi - ILA Rx Pkt Counter High 16-bits
    Bits: 15:0  IlaRxPktCntHi(rc) - Upper 16-bits of 48-bit ILA Rx Packet count. Reading this register latches the value in IlaRxPktCntLo
*/
#define NFP_ILAX_CSR_IlaRxPktCntHi                           0x00000034
#define   NFP_ILAX_CSR_IlaRxPktCntHi_IlaRxPktCntHi(_x)       (((_x) & 0xffff) << 0)
#define   NFP_ILAX_CSR_IlaRxPktCntHi_IlaRxPktCntHi_of(_x)    (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: IlaRxPktCntLo - ILA Rx Pkt Counter Low 32-bits
    Bits: 31:0  IlaRxPktCntLo(rc) - Lower 32-bits of 48-bit ILA Rx Packet count. Reading this register after reading the IlaRxPktCntHi register
*/
#define NFP_ILAX_CSR_IlaRxPktCntLo                           0x00000038
#define   NFP_ILAX_CSR_IlaRxPktCntLo_IlaRxPktCntLo(_x)       (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_IlaRxPktCntLo_IlaRxPktCntLo_of(_x)    (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxErrByteCntHi - ILA Rx Errored Byte Counter High 32-bits
    Bits: 31:0  IlaRxErrByteCntHi(rc) - Upper 32-bits of 64-bit ILA Rx Errored Byte count. Reading this register latches the value in IlaRxErrByteCntLo
*/
#define NFP_ILAX_CSR_IlaRxErrByteCntHi                       0x0000003c
#define   NFP_ILAX_CSR_IlaRxErrByteCntHi_IlaRxErrByteCntHi(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_IlaRxErrByteCntHi_IlaRxErrByteCntHi_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxErrByteCntLo - ILA Rx Errored Byte Counter Low 32-bits
    Bits: 31:0  IlaRxErrByteCntLo(rc) - Lower 32-bits of 64-bit ILA Rx Errored Byte count. Reading this register after reading the IlaRxErrByteCntHi register
*/
#define NFP_ILAX_CSR_IlaRxErrByteCntLo                       0x00000040
#define   NFP_ILAX_CSR_IlaRxErrByteCntLo_IlaRxErrByteCntLo(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_IlaRxErrByteCntLo_IlaRxErrByteCntLo_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRxErrPktCntHi - ILA Rx Errored Pkt Counter High 16-bits
    Bits: 15:0  IlaRxErrPktCntHi(rc) - Upper 16-bits of 48-bit ILA Rx Errored Packet count. Reading this register latches the value in IlaRxErrPktCntLo
*/
#define NFP_ILAX_CSR_IlaRxErrPktCntHi                        0x00000044
#define   NFP_ILAX_CSR_IlaRxErrPktCntHi_IlaRxErrPktCntHi(_x) (((_x) & 0xffff) << 0)
#define   NFP_ILAX_CSR_IlaRxErrPktCntHi_IlaRxErrPktCntHi_of(_x) (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: IlaRxErrPktCntLo - ILA Rx Errored Pkt Counter Low 32-bits
    Bits: 31:0  IlaRxErrPktCntLo(rc) - Lower 32-bits of 48-bit ILA Rx Errored Packet count. Reading this register after reading the IlaRxErrPktCntHi register
*/
#define NFP_ILAX_CSR_IlaRxErrPktCntLo                        0x00000048
#define   NFP_ILAX_CSR_IlaRxErrPktCntLo_IlaRxErrPktCntLo(_x) (((_x) & 0xffffffff) << 0)
#define   NFP_ILAX_CSR_IlaRxErrPktCntLo_IlaRxErrPktCntLo_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IlaRetDescStat - Return Descriptor FIFO Status
    Bits: 15:8  IlaRetDescCurDepth(ro) - Indicates the current depth the Return Descriptor FIFO
    Bits:  7:0  IlaRetDescMaxDepth(ro) - Indicates the maximum depth the Return Descriptor has reached.
*/
#define NFP_ILAX_CSR_IlaRetDescStat                          0x0000004c
#define   NFP_ILAX_CSR_IlaRetDescStat_IlaRetDescCurDepth(_x) (((_x) & 0xff) << 8)
#define   NFP_ILAX_CSR_IlaRetDescStat_IlaRetDescCurDepth_of(_x) (((_x) >> 8) & 0xff)
#define   NFP_ILAX_CSR_IlaRetDescStat_IlaRetDescMaxDepth(_x) (((_x) & 0xff) << 0)
#define   NFP_ILAX_CSR_IlaRetDescStat_IlaRetDescMaxDepth_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: IlaErrStat - ILA Error Status Register.
    Bits: 13    InvalidTag(rw1c) - Set when Invalid Tags are received from the ILA Rx Interface.
    Bits: 12    RxErrAll(rw1c) - Set when all open channels are errored out by the ILA Rx Core.
    Bits: 11    CppTBadCmd(rw1c) - Indicates Illegal CPP Target Command.
    Bits: 10    TgtWrStartErr(rw1c) - Accel Target Write Command Start Error.
    Bits:  9    CppTCPullErr(rw1c) - Indicates data going to the ILA Tx interface encountered a pull data error.
    Bits:  8    CppTCPushErr(rw1c) - Indicates an ILA Rx error or propagated to the CPP Target push bus.
    Bits:  7    TgtWrEndErr(rw1c) - Accel Target Write Command End Error.
    Bits:  6    RxErr(rw1c) - Set when a packet error is signaled by the ILA Rx Core.
    Bits:  5    EopErr(rw1c) - Set when an EOP error is signaled by the ILA Rx Core
    Bits:  1    TimeOutErr(rw1c) - Set when no response has been received for a configurable amount of time.
    Bits:  0    IlaRxAlignErr(rw1c) - Set when the ILA Rx Alignment is lost. Indicates the SERDES lanes alignment has dropprd.
*/
#define NFP_ILAX_CSR_IlaErrStat                              0x00000050
#define   NFP_ILAX_CSR_IlaErrStat_InvalidTag                 BIT(13)
#define   NFP_ILAX_CSR_IlaErrStat_RxErrAll                   BIT(12)
#define   NFP_ILAX_CSR_IlaErrStat_CppTBadCmd                 BIT(11)
#define   NFP_ILAX_CSR_IlaErrStat_TgtWrStartErr              BIT(10)
#define   NFP_ILAX_CSR_IlaErrStat_CppTCPullErr               BIT(9)
#define   NFP_ILAX_CSR_IlaErrStat_CppTCPushErr               BIT(8)
#define   NFP_ILAX_CSR_IlaErrStat_TgtWrEndErr                BIT(7)
#define   NFP_ILAX_CSR_IlaErrStat_RxErr                      BIT(6)
#define   NFP_ILAX_CSR_IlaErrStat_EopErr                     BIT(5)
#define   NFP_ILAX_CSR_IlaErrStat_TimeOutErr                 BIT(1)
#define   NFP_ILAX_CSR_IlaErrStat_IlaRxAlignErr              BIT(0)
/*----------------------------------------------------------------
  Register: IlaErrMask - ILA Error Mask Register.
    Bits: 13    InvalidTagMask(rw) - Set to mask Invalid_TAG error interrupts.
    Bits: 12    RxErrAllMask(rw) - Set to mask Rx Err_All error interrupts.
    Bits: 11    CppTBadCmdMask(rw) - Set to mask CPPT Illegal Command error interrupts.
    Bits: 10    TgtWrStartErrMask(rw) - Set to mask Accel Target Write Command Start Error.
    Bits:  9    CppTCPullErrMask(rw) - Set to mask CPP Target pull data error interrupts
    Bits:  8    CppTCPushErrMask(rw) - Set to mask CPP Taget push data error interrupts.
    Bits:  7    TgtWrEndErrMask(rw) - Set to mask Accel Target Write Command End Error.
    Bits:  6    RxErrMask(rw) - Set to mask packet error interrupts.
    Bits:  5    EopErrMask(rw) - Set to mask EOP error interrrupts.
    Bits:  1    TimeOutErrMask(rw) - Set to mask Time out error interrupts.
    Bits:  0    IlaRxAlignErrMask(rw) - Set to mask ILA Rx alignment error interrupts.
*/
#define NFP_ILAX_CSR_IlaErrMask                              0x00000054
#define   NFP_ILAX_CSR_IlaErrMask_InvalidTagMask             BIT(13)
#define   NFP_ILAX_CSR_IlaErrMask_RxErrAllMask               BIT(12)
#define   NFP_ILAX_CSR_IlaErrMask_CppTBadCmdMask             BIT(11)
#define   NFP_ILAX_CSR_IlaErrMask_TgtWrStartErrMask          BIT(10)
#define   NFP_ILAX_CSR_IlaErrMask_CppTCPullErrMask           BIT(9)
#define   NFP_ILAX_CSR_IlaErrMask_CppTCPushErrMask           BIT(8)
#define   NFP_ILAX_CSR_IlaErrMask_TgtWrEndErrMask            BIT(7)
#define   NFP_ILAX_CSR_IlaErrMask_RxErrMask                  BIT(6)
#define   NFP_ILAX_CSR_IlaErrMask_EopErrMask                 BIT(5)
#define   NFP_ILAX_CSR_IlaErrMask_TimeOutErrMask             BIT(1)
#define   NFP_ILAX_CSR_IlaErrMask_IlaRxAlignErrMask          BIT(0)
/*----------------------------------------------------------------
  Register: IlaErrCnt - ILA Error Counter
    Bits: 29:20 EopErrCnt(ro) - Stores the ILA EOP_ERR Counter.
    Bits: 19:10 CppPullDataErrCnt(ro) - Stores the CPP Pull Data Error Counter.
    Bits:  9:0  CppPushDataErrCnt(ro) - Stores the CPP Push Data Error Counter.
*/
#define NFP_ILAX_CSR_IlaErrCnt                               0x00000060
#define   NFP_ILAX_CSR_IlaErrCnt_EopErrCnt(_x)               (((_x) & 0x3ff) << 20)
#define   NFP_ILAX_CSR_IlaErrCnt_EopErrCnt_of(_x)            (((_x) >> 20) & 0x3ff)
#define   NFP_ILAX_CSR_IlaErrCnt_CppPullDataErrCnt(_x)       (((_x) & 0x3ff) << 10)
#define   NFP_ILAX_CSR_IlaErrCnt_CppPullDataErrCnt_of(_x)    (((_x) >> 10) & 0x3ff)
#define   NFP_ILAX_CSR_IlaErrCnt_CppPushDataErrCnt(_x)       (((_x) & 0x3ff) << 0)
#define   NFP_ILAX_CSR_IlaErrCnt_CppPushDataErrCnt_of(_x)    (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: IlaErrCnt_2 - ILA Error Counter 2
    Bits: 31:24 TimeOutErrCnt(ro) - Stores the Time Out Error Counter.
    Bits: 23:16 RxErrCnt(ro) - Stores the RX_ERR Error Counter.
    Bits: 15:8  ErrAllCnt(ro) - Stores the ERR_ALL Error Counter.
    Bits:  7:0  CppTBadCmdCnt(ro) - Stores the CPPT Bad Command Error Counter.
*/
#define NFP_ILAX_CSR_IlaErrCnt_2                             0x00000068
#define   NFP_ILAX_CSR_IlaErrCnt_2_TimeOutErrCnt(_x)         (((_x) & 0xff) << 24)
#define   NFP_ILAX_CSR_IlaErrCnt_2_TimeOutErrCnt_of(_x)      (((_x) >> 24) & 0xff)
#define   NFP_ILAX_CSR_IlaErrCnt_2_RxErrCnt(_x)              (((_x) & 0xff) << 16)
#define   NFP_ILAX_CSR_IlaErrCnt_2_RxErrCnt_of(_x)           (((_x) >> 16) & 0xff)
#define   NFP_ILAX_CSR_IlaErrCnt_2_ErrAllCnt(_x)             (((_x) & 0xff) << 8)
#define   NFP_ILAX_CSR_IlaErrCnt_2_ErrAllCnt_of(_x)          (((_x) >> 8) & 0xff)
#define   NFP_ILAX_CSR_IlaErrCnt_2_CppTBadCmdCnt(_x)         (((_x) & 0xff) << 0)
#define   NFP_ILAX_CSR_IlaErrCnt_2_CppTBadCmdCnt_of(_x)      (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: IlaSramCtrlRtnDesc - SRAM Control register for ILA Return Descriptor SRAM
  Register: IlaSramCtrlRxFifo - SRAM Control register for ILA Rx FIFO SRAM
    Bits: 14    BC2_0(rw) - SRAM0: Bias Level Adjust Input. This is bias control for controlling the diode connected to VSSCORE.
    Bits: 13    BC1_0(rw) - SRAM0: Bias Level Adjust Input. This is bias control for controlling the diode connected to VSSCORE.
    Bits: 12    Test1B0(rw) - SRAM0: Test pin for Port B to bypass self-timed circuit. The external clock controls the read and write control signals.
    Bits: 11    Test1Test1A0(rw) - SRAM0: Test pin for Port A to bypass self-timed circuit. The external clock controls the read and write control signals.
    Bits: 10    RaRmOveride0(rw) - SRAM0: Enables the override of the sense-amp controls by both RmRma0 and Rmb0 fields.
    Bits:  9:8  Ra0(rw) - SRAM0: RA for 1-Port SRAMs. Read Assist(RA) to control WL under-drive. See SRAMs specs.
    Bits:  7:4  Rmb0(rw) - SRAM0: RMB for 2-port SRAMs. Read Margin Port B(RMB) sets the Read-Write margin by adjusting the sense amp differential setting. See SRAMs specs.
    Bits:  3:0  RmRma0(rw) - SRAM0: RM for 1-Port SRAMs / RMA for 2-port SRAMs. Read Margin(RM) / Read Margin Port A(RMA) sets the Read-Write margin by adjusting the sense amp differential setting. See SRAMs specs.
*/
#define NFP_ILAX_CSR_IlaSramCtrlRtnDesc                      0x00000074
#define NFP_ILAX_CSR_IlaSramCtrlRxFifo                       0x00000078
#define   NFP_ILAX_CSR_IlaSramCtrl_BC2_0                     BIT(14)
#define   NFP_ILAX_CSR_IlaSramCtrl_BC1_0                     BIT(13)
#define   NFP_ILAX_CSR_IlaSramCtrl_Test1B0                   BIT(12)
#define   NFP_ILAX_CSR_IlaSramCtrl_Test1Test1A0              BIT(11)
#define   NFP_ILAX_CSR_IlaSramCtrl_RaRmOveride0              BIT(10)
#define   NFP_ILAX_CSR_IlaSramCtrl_Ra0(_x)                   (((_x) & 0x3) << 8)
#define   NFP_ILAX_CSR_IlaSramCtrl_Ra0_of(_x)                (((_x) >> 8) & 0x3)
#define   NFP_ILAX_CSR_IlaSramCtrl_Rmb0(_x)                  (((_x) & 0xf) << 4)
#define   NFP_ILAX_CSR_IlaSramCtrl_Rmb0_of(_x)               (((_x) >> 4) & 0xf)
#define   NFP_ILAX_CSR_IlaSramCtrl_RmRma0(_x)                (((_x) & 0xf) << 0)
#define   NFP_ILAX_CSR_IlaSramCtrl_RmRma0_of(_x)             (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: IlaTxErrCtrl - ILA Tx Error propagation control register
    Bits:  0    IlaTxErrDisable(rw) - ILA Transmit error propagation disable. When set data errors are disabled from propagating to the Interlaken_la transmit interface.
*/
#define NFP_ILAX_CSR_IlaTxErrCtrl                            0x00000088
#define   NFP_ILAX_CSR_IlaTxErrCtrl_IlaTxErrDisable          BIT(0)

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_ILA_H */
