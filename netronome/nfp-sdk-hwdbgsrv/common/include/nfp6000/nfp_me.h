/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_ME_H
#define NFP6000_ME_H

/*================================================================*/
/* TGT 14                                                         */

/*----------------------------------------------------------------*/
/* CPP Address Map (CPP_BASE = IMB translation: island + indexes) */

/* NOTE: Action 2/3, Token 1 */
/* CPP_BASE + NFP_ME[12] + NFP_ME_* */
#define NFP_CT_ME(_x)      (0x00010000 + (((_x + 4) & 0xf) << 10))

#define NFP_CT_ME_of(_offset)      (((_offset >> 10) & 0xf) - 4)

/*================================================================*/


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: UstorAddr - Used to load programs into the Control Store
    Bits: 31    EnableCS(rw) - Enable Control Store: The Microengine should be in Idle state (no contexts running). The address in Uaddr field specifies the Control Store address where the data written to UstorDataLwr and UstorDataUpr will be written. Also set in debug mode. This bit can be used to dump data from Microengine GPRs and Read Transfer registers. The Microengine should be in an idle state (no contexts running). This forces the Microengine to continuously execute an instruction at the address specified by Uaddr. Only the ALU instruction is supported in this mode and the result of the execution is written to ALUOut CSR rather than a destination register.
    Bits: 30    UstoreWrStrb(rw) - UstoreWriteStrobe
    Bits: 29    UstoreDataInvert(rw) - UstoreDataInvert
    Bits: 12:0  Uaddr(rw) - Address of control store location to be accessed. Valid values are 0 to 8191.
*/
#define NFP_ME_USTORE_ADDRESS                                0x00000000
#define   NFP_ME_USTORE_ADDRESS_ECS                          BIT(31)
#define     NFP_ME_USTORE_ADDRESS_ECS_NORMAL                 (0 << 31)
#define     NFP_ME_USTORE_ADDRESS_ECS_RW                     BIT(31)
#define   NFP_ME_USTORE_ADDRESS_WRITE_STROBE                 BIT(30)
#define     NFP_ME_USTORE_ADDRESS_WRITE_STROBE_DISABLE       (0 << 30)
#define     NFP_ME_USTORE_ADDRESS_WRITE_STROBE_ENABLE        BIT(30)
#define   NFP_ME_USTORE_ADDRESS_DATA_INVERT                  BIT(29)
#define     NFP_ME_USTORE_ADDRESS_DATA_INVERT_DISABLE        (0 << 29)
#define     NFP_ME_USTORE_ADDRESS_DATA_INVERT_ENABLE         BIT(29)
#define   NFP_ME_USTORE_ADDRESS_ADDR(_x)                     (((_x) & 0x1fff) << 0)
#define   NFP_ME_USTORE_ADDRESS_ADDR_of(_x)                  (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: UstorDataLwr - Control Store Data - lower
    Bits: 31:0  UdataLower(rw) - Contains bits 31:00 of the instruction of the Control Store location specified by the UstorAddr CSR.
*/
#define NFP_ME_USTORE_DATA_LOWER                             0x00000004
#define   NFP_ME_USTORE_DATA_LOWER_DATA(_x)                  (((_x) & 0xffffffff) << 0)
#define   NFP_ME_USTORE_DATA_LOWER_DATA_of(_x)               (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: UstorDataUpr - Control Store Data - upper
    Bits: 19:13 ECC(rw) - Contains the ECC Check bits for the instruction
    Bits: 12:0  UdataUpper(rw) - Contains the data from bits [43:32] of the control store location specified by the UstorAddr.
*/
#define NFP_ME_USTORE_DATA_UPPER                             0x00000008
#define   NFP_ME_USTORE_DATA_UPPER_ECC(_x)                   (((_x) & 0x7f) << 13)
#define   NFP_ME_USTORE_DATA_UPPER_ECC_of(_x)                (((_x) >> 13) & 0x7f)
#define   NFP_ME_USTORE_DATA_UPPER_HIVAL(_x)                 (((_x) & 0x1fff) << 0)
#define   NFP_ME_USTORE_DATA_UPPER_HIVAL_of(_x)              (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: UstorErrStat - ECC errors during Control Store reads
    Bits: 31    UncorrectableErr(ro) - Error Type. This bit indicates the type of error detected.
    Bits: 26:20 Syndrome(ro) - This field records the syndrome that was found when the error occurred.
    Bits: 18:16 Context(ro) - Context that was executing when the ECC error occurred
    Bits: 13:0  Uaddr(ro) - Contains the address that had the ECC error.
*/
#define NFP_ME_USTORE_ERROR_STATUS                           0x0000000c
#define   NFP_ME_USTORE_ERROR_STATUS_UNCORRECTABLE           BIT(31)
#define     NFP_ME_USTORE_ERROR_STATUS_UNCORRECTABLE_NOT_OCCURED (0 << 31)
#define     NFP_ME_USTORE_ERROR_STATUS_UNCORRECTABLE_OCCURED BIT(31)
#define   NFP_ME_USTORE_ERROR_STATUS_SYNDROME(_x)            (((_x) & 0x7f) << 20)
#define   NFP_ME_USTORE_ERROR_STATUS_SYNDROME_of(_x)         (((_x) >> 20) & 0x7f)
#define   NFP_ME_USTORE_ERROR_STATUS_CTX(_x)                 (((_x) & 0x7) << 16)
#define   NFP_ME_USTORE_ERROR_STATUS_CTX_of(_x)              (((_x) >> 16) & 0x7)
#define   NFP_ME_USTORE_ERROR_STATUS_UADDR(_x)               (((_x) & 0x3fff) << 0)
#define   NFP_ME_USTORE_ERROR_STATUS_UADDR_of(_x)            (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: ALUOut - Debug to show state of ALU
    Bits: 31:0  ALUOutput(ro) - ALU output.
*/
#define NFP_ME_ALU_OUT                                       0x00000010
#define   NFP_ME_ALU_OUT_VALUE(_x)                           (((_x) & 0xffffffff) << 0)
#define   NFP_ME_ALU_OUT_VALUE_of(_x)                        (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CtxArbCtrl - Context Arbiter Control - used by the context arbiter and for debug
    Bits:  6:4  PreviousCtx(ro) - Previous Context. This field contains the number of the last context that was running.
    Bits:  2:0  NextCtx(rw) - Next Context. This field contains the number of the next context that will be run.
*/
#define NFP_ME_CTX_ARB_CNTL                                  0x00000014
#define   NFP_ME_CTX_ARB_CNTL_PCTX(_x)                       (((_x) & 0x7) << 4)
#define   NFP_ME_CTX_ARB_CNTL_PCTX_of(_x)                    (((_x) >> 4) & 0x7)
#define   NFP_ME_CTX_ARB_CNTL_NCTX(_x)                       (((_x) & 0x7) << 0)
#define   NFP_ME_CTX_ARB_CNTL_NCTX_of(_x)                    (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: CtxEnables - Context Enables - used by the context arbiter and for debug
    Bits: 31    InUseContexts(rw) - Indicates the number of in-use contexts, which determines the GPR and Transfer Register allocation. Note that although this information could be inferred from bits C0 to C7, this field allows for contexts to be temporarily disabled due to error or debugging conditions. It is illegal to enable Contexts that are not currently in-use according to this field.
    Bits: 30    PseudoRandNum(rw) - Controls when the Pseudo_Random_Number is generated.
    Bits: 29    CSEccError(rw1c) - Indicates that an ECC error was detected in the Control Store when an instruction was read. This bit will never be set if ECCErrorEnable bit is 0. When this bit is set the Microengine's attn output is asserted. More information about the error is available in UstorErrStat Register.
    Bits: 28    CSEccEnable(rw) - Enables ECC error detection on Control Store.
    Bits: 27    Breakpoint(rw1c) - The ctx_arb[bpt] instruction was executed. When this bit is set, the Microengine's attn output is asserted and all CtxEnables bits in this register are cleared.
    Bits: 25    RegisterParityErr(rw1c) - Indicates that a parity error was detected when reading a datapath register. When this bit is set, the Microengine's attn output is asserted and all CtxEnables bits in this register are cleared. RegErrStatus Local CSR has information about the error.
    Bits: 23    LMEccEnable(rw) - Enables ECC error detection on Local Memory.
    Bits: 22    LMAddr3Global(rw) - Controls usage of ActLMAddr3.
    Bits: 21    LMAddr2Global(rw) - Controls usage of ActLMAddr2.
    Bits: 20    NNsendConfig(rw) - This bit controls the ME's Next-Neighbor 'Send' Interface; it controls whether a Next-Neighbor destination from this ME is written to the Next Neighbor ME (general-usage) or to this same ME (mostly for debugging usage); 'same ME' refers to the ME executing the instruction which carries the NN destination; 'same ME' or 'Self' refer to the same configuration.
    Bits: 19:18 NextNeighborEmptyAssert(rw) - Controls threshold when NN_Empty asserts. The number of entries valid is determined by comparing NNPut and NNGet Local CSRs. The use of indicating empty when there is really something on the Ring is if the cooperating processes transfer data in a block, and the consumer does not want to get a partial block.
    Bits: 17    LMAddr1Global(rw) - Controls usage of ActLMAddr1.
    Bits: 16    LMAddr0Global(rw) - Controls usage of ActLMAddr0.
    Bits: 15:8  CtxEnables(rw) - Context Enables for Context 7 through Context 0.
    Bits:  2:0  NNreceiveConfig(rw) - This field controls the ME's Next-Neighbor 'Receive' Interface; it selects which path has access to the NN Register File's write port; in MEv2.8, four different NN paths may be selected: NN from previous ME, Self, CTnn, and PushBus; please note that when the NNsendConfig bit is set to 'Self', the NNreceiveConfig field must also be set to 'Self'; otherwise, the NN destination data generated from 'this ME' is lost
*/
#define NFP_ME_CTX_ENABLES                                   0x00000018
#define   NFP_ME_CTX_ENABLES_IN_USE                          BIT(31)
#define     NFP_ME_CTX_ENABLES_IN_USE_8CTX                   (0 << 31)
#define     NFP_ME_CTX_ENABLES_IN_USE_4CTX                   BIT(31)
#define   NFP_ME_CTX_ENABLES_PRN_MODE                        BIT(30)
#define     NFP_ME_CTX_ENABLES_PRN_MODE_ME_INSTR             (0 << 30)
#define     NFP_ME_CTX_ENABLES_PRN_MODE_EVERY_CYCLE          BIT(30)
#define   NFP_ME_CTX_ENABLES_USTORE_ECC_ERR                  BIT(29)
#define   NFP_ME_CTX_ENABLES_USTORE_ECC_ENABLE               BIT(28)
#define     NFP_ME_CTX_ENABLES_USTORE_ECC_ENABLE_DISABLE     (0 << 28)
#define     NFP_ME_CTX_ENABLES_USTORE_ECC_ENABLE_ENABLE      BIT(28)
#define   NFP_ME_CTX_ENABLES_BREAKPOINT                      BIT(27)
#define   NFP_ME_CTX_ENABLES_REG_PAR_ERR                     BIT(25)
#define     NFP_ME_CTX_ENABLES_REG_PAR_ERR_NONE              (0 << 25)
#define     NFP_ME_CTX_ENABLES_REG_PAR_ERR_DETECTED          BIT(25)
#define   NFP_ME_CTX_ENABLES_LOCAL_MEMORY_ECC_ENABLE         BIT(23)
#define     NFP_ME_CTX_ENABLES_LOCAL_MEMORY_ECC_ENABLE_DISABLE (0 << 23)
#define     NFP_ME_CTX_ENABLES_LOCAL_MEMORY_ECC_ENABLE_ENABLE BIT(23)
#define   NFP_ME_CTX_ENABLES_LM_ADDR_3_GLOB                  BIT(22)
#define     NFP_ME_CTX_ENABLES_LM_ADDR_3_GLOB_RELATIVE       (0 << 22)
#define     NFP_ME_CTX_ENABLES_LM_ADDR_3_GLOB_GLOBAL         BIT(22)
#define   NFP_ME_CTX_ENABLES_LM_ADDR_2_GLOB                  BIT(21)
#define     NFP_ME_CTX_ENABLES_LM_ADDR_2_GLOB_RELATIVE       (0 << 21)
#define     NFP_ME_CTX_ENABLES_LM_ADDR_2_GLOB_GLOBAL         BIT(21)
#define   NFP_ME_CTX_ENABLES_NN_SEND_CONFIGURATION           BIT(20)
#define     NFP_ME_CTX_ENABLES_NN_SEND_CONFIGURATION_NEXT    (0 << 20)
#define     NFP_ME_CTX_ENABLES_NN_SEND_CONFIGURATION_SELF    BIT(20)
#define   NFP_ME_CTX_ENABLES_NN_RING_EMPTY(_x)               (((_x) & 0x3) << 18)
#define   NFP_ME_CTX_ENABLES_NN_RING_EMPTY_of(_x)            (((_x) >> 18) & 0x3)
#define     NFP_ME_CTX_ENABLES_NN_RING_EMPTY_NONE            (0)
#define     NFP_ME_CTX_ENABLES_NN_RING_EMPTY_LE1             (1)
#define     NFP_ME_CTX_ENABLES_NN_RING_EMPTY_LE2             (2)
#define     NFP_ME_CTX_ENABLES_NN_RING_EMPTY_LE3             (3)
#define   NFP_ME_CTX_ENABLES_LM_ADDR_1_GLOB                  BIT(17)
#define     NFP_ME_CTX_ENABLES_LM_ADDR_1_GLOB_RELATIVE       (0 << 17)
#define     NFP_ME_CTX_ENABLES_LM_ADDR_1_GLOB_GLOBAL         BIT(17)
#define   NFP_ME_CTX_ENABLES_LM_ADDR_0_GLOB                  BIT(16)
#define     NFP_ME_CTX_ENABLES_LM_ADDR_0_GLOB_RELATIVE       (0 << 16)
#define     NFP_ME_CTX_ENABLES_LM_ADDR_0_GLOB_GLOBAL         BIT(16)
#define   NFP_ME_CTX_ENABLES_CONTEXTS(_x)                    (((_x) & 0xff) << 8)
#define   NFP_ME_CTX_ENABLES_CONTEXTS_of(_x)                 (((_x) >> 8) & 0xff)
#define     NFP_ME_CTX_ENABLES_CONTEXTS_NONE                 (0)
#define     NFP_ME_CTX_ENABLES_CONTEXTS_LE1                  (1)
#define   NFP_ME_CTX_ENABLES_NN_RECEIVE_CONFIGURATION(_x)    (((_x) & 0x7) << 0)
#define   NFP_ME_CTX_ENABLES_NN_RECEIVE_CONFIGURATION_of(_x) (((_x) >> 0) & 0x7)
#define     NFP_ME_CTX_ENABLES_NN_RECEIVE_CONFIGURATION_NN   (0)
#define     NFP_ME_CTX_ENABLES_NN_RECEIVE_CONFIGURATION_SELF (1)
#define     NFP_ME_CTX_ENABLES_NN_RECEIVE_CONFIGURATION_CTNN (2)
#define     NFP_ME_CTX_ENABLES_NN_RECEIVE_CONFIGURATION_PUSHBUS (3)
/*----------------------------------------------------------------
  Register: CondCodeEn - Condition Code Enable
    Bits: 13    CurCndtnCodeEn(rw) - Current Condition Code Enable. Set to 1 to update the condition codes. When 0, condition codes will not be updated.
*/
#define NFP_ME_CC_ENABLE                                     0x0000001c
#define   NFP_ME_CC_ENABLE_CCCE                              BIT(13)
/*----------------------------------------------------------------
  Register: CSRCtxPtr - CSR Context Pointer
    Bits:  2:0  CtxSelect(rw) - Selects which contexts Local CSR is accessed by local_csr_read, local_csr_write, and by the IA core.
*/
#define NFP_ME_CSR_CTX_POINTER                               0x00000020
#define   NFP_ME_CSR_CTX_POINTER_CTX(_x)                     (((_x) & 0x7) << 0)
#define   NFP_ME_CSR_CTX_POINTER_CTX_of(_x)                  (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: PcBreakpoint0 - PC Breakpoint 0 - PCB system
  Register: PcBreakpoint1 - PC Breakpoint 1 - PCB system
    Bits: 31:24 PcBrkpntCntl(rw) - PC Breakpoint Control: Controls the PC Breakpoint0 SDS-mechanism.
    Bits: 13:0  PcBrkpntAddr(rw) - PC Breakpoint Address: PC Address corresponding to PcBreakpoint0; it gets compared to the ME Active PC value.
*/
#define NFP_ME_PC_BREAKPOINT_0                               0x00000024
#define NFP_ME_PC_BREAKPOINT_1                               0x00000028
#define   NFP_ME_PCBREAKPOINT_CNTL(_x)                       (((_x) & 0xff) << 24)
#define   NFP_ME_PCBREAKPOINT_CNTL_of(_x)                    (((_x) >> 24) & 0xff)
#define   NFP_ME_PCBREAKPOINT_ADDR(_x)                       (((_x) & 0x3fff) << 0)
#define   NFP_ME_PCBREAKPOINT_ADDR_of(_x)                    (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: PcBreakpointStatus - PC Breakpoint - Status register associated with the PCB system
    Bits:  9:2  PcBrkpntedCtx(rw1c) - Indicates which context has triggered the PC breakpoint event; the least significant bit of this fields is associated with ctx 0; the most significant bit with ctx 7
    Bits:  1    PcBrkpnt1Status(rw1c) - Indicates the status of the PcBreakpoint1 SDS mechanism
    Bits:  0    PcBrkpnt0Status(rw1c) - Indicates the status of the PcBreakpoint0 SDS mechanism
*/
#define NFP_ME_PC_BREAKPOINT_STATUS                          0x0000002c
#define   NFP_ME_PC_BREAKPOINT_STATUS_CTX(_x)                (((_x) & 0xff) << 2)
#define   NFP_ME_PC_BREAKPOINT_STATUS_CTX_of(_x)             (((_x) >> 2) & 0xff)
#define   NFP_ME_PC_BREAKPOINT_STATUS_BRKPNT1                BIT(1)
#define     NFP_ME_PC_BREAKPOINT_STATUS_BRKPNT1_VAL0         (0 << 1)
#define     NFP_ME_PC_BREAKPOINT_STATUS_BRKPNT1_VAL1         BIT(1)
#define   NFP_ME_PC_BREAKPOINT_STATUS_BRKPNT0                BIT(0)
#define     NFP_ME_PC_BREAKPOINT_STATUS_BRKPNT0_VAL0         (0 << 0)
#define     NFP_ME_PC_BREAKPOINT_STATUS_BRKPNT0_VAL1         BIT(0)
/*----------------------------------------------------------------
  Register: RegErrStatus - Information about parity errors detected on Datapath Regs
    Bits: 17:16 Type(ro) - Type of Register File where a Parity Error has been detected.
    Bits: 15:0  RegisterNumber(ro) - Entry Address of the Register File where a Parity Error has been detected. Unused upper bits will read 0.
*/
#define NFP_ME_REGISTER_ERROR_STATUS                         0x00000030
#define   NFP_ME_REGISTER_ERROR_STATUS_TYPE(_x)              (((_x) & 0x3) << 16)
#define   NFP_ME_REGISTER_ERROR_STATUS_TYPE_of(_x)           (((_x) >> 16) & 0x3)
#define     NFP_ME_REGISTER_ERROR_STATUS_TYPE_GPR            (0)
#define     NFP_ME_REGISTER_ERROR_STATUS_TYPE_XFER           (1)
#define     NFP_ME_REGISTER_ERROR_STATUS_TYPE_NN             (2)
#define   NFP_ME_REGISTER_ERROR_STATUS_REG(_x)               (((_x) & 0xffff) << 0)
#define   NFP_ME_REGISTER_ERROR_STATUS_REG_of(_x)            (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: LMErrStatus - Status on ECC errors recorded on Local Memory reads
    Bits: 31    UncorrectableErr(ro) - This bit indicates that the ECC error detected on Local Memory is uncorrectable
    Bits: 26:20 Syndrome(ro) - This field records the syndrome that was found when the ECC error occurred.
    Bits: 18:16 Context(ro) - Context that was executing when the ECC error occurred
    Bits:  9:0  LMaddress(ro) - Contains the LM address where the ECC error has been detected.
*/
#define NFP_ME_LM_ERROR_STATUS                               0x00000034
#define   NFP_ME_LM_ERROR_STATUS_UNCORRECTABLE               BIT(31)
#define     NFP_ME_LM_ERROR_STATUS_UNCORRECTABLE_NOT_OCCURED (0 << 31)
#define     NFP_ME_LM_ERROR_STATUS_UNCORRECTABLE_OCCURED     BIT(31)
#define   NFP_ME_LM_ERROR_STATUS_SYNDROME(_x)                (((_x) & 0x7f) << 20)
#define   NFP_ME_LM_ERROR_STATUS_SYNDROME_of(_x)             (((_x) >> 20) & 0x7f)
#define   NFP_ME_LM_ERROR_STATUS_CTX(_x)                     (((_x) & 0x7) << 16)
#define   NFP_ME_LM_ERROR_STATUS_CTX_of(_x)                  (((_x) >> 16) & 0x7)
#define   NFP_ME_LM_ERROR_STATUS_LM_ADDRESS(_x)              (((_x) & 0x3ff) << 0)
#define   NFP_ME_LM_ERROR_STATUS_LM_ADDRESS_of(_x)           (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: LMeccErrorMask - Controls Error Injection bits into an LM data-path word.
    Bits: 31:0  ErrorInjectionMask(rw) - This field is used for controlling Error Injection bits into an LM data-path word.
*/
#define NFP_ME_FORCE_BAD_LM_ECC                              0x00000038
#define   NFP_ME_FORCE_BAD_LM_ECC_ERROR_INJECTION_MASK(_x)   (((_x) & 0xffffffff) << 0)
#define   NFP_ME_FORCE_BAD_LM_ECC_ERROR_INJECTION_MASK_of(_x) (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IndCtxStatus - Indirect Context Status Register
    Bits: 31    ReadyToRun(ro) - Ready to Run. Indicates that the context is in Ready state. (This bit will be 0 if the context is in any of the other three states.)
    Bits: 13:0  ContextPC(rw) - The program counter at which the context begins executing when it is put into the executing state.
*/
#define NFP_ME_INDIRECT_CTX_STS                              0x00000040
#define   NFP_ME_INDIRECT_CTX_STS_RR                         BIT(31)
#define     NFP_ME_INDIRECT_CTX_STS_RR_NOT_READY             (0 << 31)
#define     NFP_ME_INDIRECT_CTX_STS_RR_READY                 BIT(31)
#define   NFP_ME_INDIRECT_CTX_STS_CTX_PC(_x)                 (((_x) & 0x3fff) << 0)
#define   NFP_ME_INDIRECT_CTX_STS_CTX_PC_of(_x)              (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: ActCtxStatus - Active Context Status Register
    Bits: 31    AB0(ro) - If set, the microengine has a context in the Executing state. If clear, no context is in Executing state.
    Bits: 30:25 IslandId(ro) - Island number where the ME happens to be instantiated.
    Bits: 24:8  ActiveContextPC(ro) - PC of Executing Context. Only valid if AB0 is a 1. This field provides a snapshot value of the PC. This value is used for tracking/code profiling purposes. When issued as a local_csr_read from the Microengine, the PC value may not be the exact PC value of the local_csr_rd instruction.
    Bits:  6:3  MENumber(ro) - A unique number which identifies the ME within the Island where it happens to be instantiated.
    Bits:  2:0  ActiveContextNo(rw) - The number of the Executing context. Only valid if AB0 bit is a 1.
*/
#define NFP_ME_ACTIVE_CTX_STS                                0x00000044
#define   NFP_ME_ACTIVE_CTX_STS_AB0                          BIT(31)
#define     NFP_ME_ACTIVE_CTX_STS_AB0_NOT_ACTIVE             (0 << 31)
#define     NFP_ME_ACTIVE_CTX_STS_AB0_ACTIVE                 BIT(31)
#define   NFP_ME_ACTIVE_CTX_STS_IL_ID(_x)                    (((_x) & 0x3f) << 25)
#define   NFP_ME_ACTIVE_CTX_STS_IL_ID_of(_x)                 (((_x) >> 25) & 0x3f)
#define   NFP_ME_ACTIVE_CTX_STS_ACTXPC(_x)                   (((_x) & 0x1ffff) << 8)
#define   NFP_ME_ACTIVE_CTX_STS_ACTXPC_of(_x)                (((_x) >> 8) & 0x1ffff)
#define   NFP_ME_ACTIVE_CTX_STS_ME_ID(_x)                    (((_x) & 0xf) << 3)
#define   NFP_ME_ACTIVE_CTX_STS_ME_ID_of(_x)                 (((_x) >> 3) & 0xf)
#define   NFP_ME_ACTIVE_CTX_STS_ACNO(_x)                     (((_x) & 0x7) << 0)
#define   NFP_ME_ACTIVE_CTX_STS_ACNO_of(_x)                  (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: IndCtxSglEvt - Indirect Context Signal Events Register
  Register: ActCtxSglEvt - Active Context Signal Events Register
    Bits: 15:1  Signals(rw) - Each bit is set as described in the Event Signals section. Each is cleared by microengine hardware when: the signal is used to transition to Ready state if the CtxWkpEvt[AnyWakeEvnts] bit is clear, or a br_!signal on this signal is not taken, or a br_signal on this signal is taken.
    Bits:  0    VoluntaryWakeup(ro) - Corresponds to Event for Voluntary arb wakeup event.
*/
#define NFP_ME_INDIRECT_CTX_SIG_EVENTS                       0x00000048
#define NFP_ME_ACTIVE_CTX_SIG_EVENTS                         0x0000004c
#define   NFP_ME_CTXSGLEVT_SIGNALS(_x)                       (((_x) & 0x7fff) << 1)
#define   NFP_ME_CTXSGLEVT_SIGNALS_of(_x)                    (((_x) >> 1) & 0x7fff)
#define   NFP_ME_CTXSGLEVT_VOLUNTARY                         BIT(0)
/*----------------------------------------------------------------
  Register: IndCtxWkpEvt - Indirect Context Wakeup Events Register
  Register: ActCtxWkpEvt - Active Context Wakeup Events Register
    Bits: 16    AnyWakeEvnts(rw) - Any Wakeup Events. Set by the ANY token on a ctx_arb instruction. Note: this bit is undefined after a wakeup.
    Bits: 15:1  WakeupEvents(rw) - Each wakeup event bit is set by either a ctx_swap_# token on an instruction, or by the Event Signal Mask of the ctx_arb instruction. All wakeup event bits are cleared by microengine hardware whether the context is put into execute state.
    Bits:  0    Voluntary(rw) - Set by ctx_arb[voluntary]. Cleared when the context is put into Execute state.
*/
#define NFP_ME_INDIRECT_CTX_WAKEUP_EVENTS                    0x00000050
#define NFP_ME_ACTIVE_CTX_WAKEUP_EVENTS                      0x00000054
#define   NFP_ME_CTXWKPEVT_ANY                               BIT(16)
#define     NFP_ME_CTXWKPEVT_ANY_ALL                         (0 << 16)
#define     NFP_ME_CTXWKPEVT_ANY_ANY                         BIT(16)
#define   NFP_ME_CTXWKPEVT_EVENTS(_x)                        (((_x) & 0x7fff) << 1)
#define   NFP_ME_CTXWKPEVT_EVENTS_of(_x)                     (((_x) >> 1) & 0x7fff)
#define   NFP_ME_CTXWKPEVT_VOLUNTARY                         BIT(0)
/*----------------------------------------------------------------
  Register: IndCtxFtrCnt - Indirect Context Future Count Register
  Register: ActCtxFtrCnt - Active Context Future Count Register
    Bits: 15:0  FutureCount(rw) - Value to match against low 32-bits of Timestamp.
*/
#define NFP_ME_INDIRECT_CTX_FUTURE_COUNT                     0x00000058
#define NFP_ME_ACTIVE_CTX_FUTURE_COUNT                       0x0000005c
#define   NFP_ME_CTXFTRCNT_CNT(_x)                           (((_x) & 0xffff) << 0)
#define   NFP_ME_CTXFTRCNT_CNT_of(_x)                        (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: IndLMAddr0 - Indirect Local Memory Address 0 Register
  Register: ActLMAddr0 - Active Local Memory Address 0 Register
  Register: IndLMAddr1 - Indirect Local Memory Address 1 Register
  Register: ActLMAddr1 - Active Local Memory Address 1 Register
  Register: IndLMAddr2 - Indirect Local Memory Address 2 Register
  Register: ActLMAddr2 - Active Local Memory Address 2 Register
  Register: IndLMAddr3 - Indirect Local Memory Address 3 Register
  Register: ActLMAddr3 - Active Local Memory Address 3 Register
    Bits: 11:2  LocalMemoryAddr(rw) - Selects the specific 32-bit word in Local Memory. This field can be incremented or decremented by 1, or left unchanged after access, as specified in the instruction.
*/
#define NFP_ME_INDIRECT_LM_ADDR_0                            0x00000060
#define NFP_ME_ACTIVE_LM_ADDR_0                              0x00000064
#define NFP_ME_INDIRECT_LM_ADDR_1                            0x00000068
#define NFP_ME_ACTIVE_LM_ADDR_1                              0x0000006c
#define NFP_ME_INDIRECT_LM_ADDR_2                            0x00000090
#define NFP_ME_ACTIVE_LM_ADDR_2                              0x00000094
#define NFP_ME_INDIRECT_LM_ADDR_3                            0x00000098
#define NFP_ME_ACTIVE_LM_ADDR_3                              0x0000009c
#define   NFP_ME_LMADDR_ADDR(_x)                             (((_x) & 0x3ff) << 2)
#define   NFP_ME_LMADDR_ADDR_of(_x)                          (((_x) >> 2) & 0x3ff)
/*----------------------------------------------------------------
  Register: ByteIndex - Byte Index Register
    Bits:  1:0  ByteNumber(rw) - Specifies a byte for use with the byte_align instruction.
*/
#define NFP_ME_BYTE_INDEX                                    0x00000070
#define   NFP_ME_BYTE_INDEX_N0(_x)                           (((_x) & 0x3) << 0)
#define   NFP_ME_BYTE_INDEX_N0_of(_x)                        (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: XferIndex - Transfer Index Register
    Bits:  9:2  XferIndex(rw) - Transfer Register Index. Specifies one of 256 registers. The choice of TRANSFER_IN vs. TRANSFER_OUT is made based on the register use (either source or destination). This field can be incremented or decremented by 1, or left unchanged after the access, as specified in the instruction.
*/
#define NFP_ME_T_INDEX                                       0x00000074
#define   NFP_ME_T_INDEX_INDEX(_x)                           (((_x) & 0xff) << 2)
#define   NFP_ME_T_INDEX_INDEX_of(_x)                        (((_x) >> 2) & 0xff)
/*----------------------------------------------------------------
  Register: IndFtrCntSgl - Which signal to set when FUTURE_COUNT == TIMESTAMP
  Register: ActFtrCntSgl - Which signal to set when FUTURE_COUNT == TIMESTAMP
    Bits:  3:0  SignalNo(rw) - The signal number to set when FUTURE_COUNT == TIMESTAMP.
*/
#define NFP_ME_INDIRECT_FUTURE_COUNT_SIGNAL                  0x00000078
#define NFP_ME_ACTIVE_FUTURE_COUNT_SIGNAL                    0x0000007c
#define   NFP_ME_FTRCNTSGL_NO(_x)                            (((_x) & 0xf) << 0)
#define   NFP_ME_FTRCNTSGL_NO_of(_x)                         (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: NNPut - Next Neighbor Put Register
  Register: NNGet - Next Neighbor Get Register
    Bits:  6:0  NNRegIndex(rw) - Specifies one of 128 NN registers to read.
*/
#define NFP_ME_NN_PUT                                        0x00000080
#define NFP_ME_NN_GET                                        0x00000084
#define   NFP_ME_NNPUTGET_REG_INDEX(_x)                      (((_x) & 0x7f) << 0)
#define   NFP_ME_NNPUTGET_REG_INDEX_of(_x)                   (((_x) >> 0) & 0x7f)
/*----------------------------------------------------------------
  Register: IndLMAddr2BytIdx - Alias of IndLMAddr2 and ByteIndex
  Register: ActLMAddr2BytIdx - Alias of ActLMAddr2 and ByteIndex
  Register: IndLMAddr3BytIdx - Alias of IndLMAddr3 and ByteIndex
  Register: ActLMAddr3BytIdx - Alias of ActLMAddr3 and ByteIndex
  Register: IndLMAddr0BytIdx - Alias of IndLMAddr0 and ByteIndex
  Register: ActLMAddr0BytIdx - Alias of ActLMAddr0 and ByteIndex
  Register: IndLMAddr1BytIdx - Alias of IndLMAddr1 and ByteIndex
  Register: ActLMAddr1BytIdx - Alias of ActLMAddr1 and ByteIndex
    Bits: 11:2  LocalMemoryAddr(rw) - Indirect Local Memory Address Register.
    Bits:  1:0  ByteNumber(rw) - Byte Index Register.
*/
#define NFP_ME_INDIRECT_LM_ADDR_2_BYTE_INDEX                 0x000000a0
#define NFP_ME_ACTIVE_LM_ADDR_2_BYTE_INDEX                   0x000000a4
#define NFP_ME_INDIRECT_LM_ADDR_3_BYTE_INDEX                 0x000000a8
#define NFP_ME_ACTIVE_LM_ADDR_3_BYTE_INDEX                   0x000000ac
#define NFP_ME_INDIRECT_LM_ADDR_0_BYTE_INDEX                 0x000000e0
#define NFP_ME_ACTIVE_LM_ADDR_0_BYTE_INDEX                   0x000000e4
#define NFP_ME_INDIRECT_LM_ADDR_1_BYTE_INDEX                 0x000000e8
#define NFP_ME_ACTIVE_LM_ADDR_1_BYTE_INDEX                   0x000000ec
#define   NFP_ME_LMADDRBYTIDX_LM_ADDR(_x)                    (((_x) & 0x3ff) << 2)
#define   NFP_ME_LMADDRBYTIDX_LM_ADDR_of(_x)                 (((_x) >> 2) & 0x3ff)
#define   NFP_ME_LMADDRBYTIDX_BYTE_NO(_x)                    (((_x) & 0x3) << 0)
#define   NFP_ME_LMADDRBYTIDX_BYTE_NO_of(_x)                 (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: IndPredCC - Indirect Predicate CC select
    Bits:  3:0  PredCCSel(rw) - Encoded value for selecting the Predicate Condition Code; encoded values select Condition Codes as shown below
*/
#define NFP_ME_INDIRECT_PREDICATE_CC                         0x000000b0
#define   NFP_ME_INDIRECT_PREDICATE_CC_SELECT(_x)            (((_x) & 0xf) << 0)
#define   NFP_ME_INDIRECT_PREDICATE_CC_SELECT_of(_x)         (((_x) >> 0) & 0xf)
#define     NFP_ME_INDIRECT_PREDICATE_CC_SELECT_PEQ          (0)
#define     NFP_ME_INDIRECT_PREDICATE_CC_SELECT_PNE          (1)
#define     NFP_ME_INDIRECT_PREDICATE_CC_SELECT_PMI          (2)
#define     NFP_ME_INDIRECT_PREDICATE_CC_SELECT_PPL          (3)
#define     NFP_ME_INDIRECT_PREDICATE_CC_SELECT_PCS          (4)
#define     NFP_ME_INDIRECT_PREDICATE_CC_SELECT_PCC          (5)
#define     NFP_ME_INDIRECT_PREDICATE_CC_SELECT_PVS          (6)
#define     NFP_ME_INDIRECT_PREDICATE_CC_SELECT_PVC          (7)
#define     NFP_ME_INDIRECT_PREDICATE_CC_SELECT_PGE          (8)
#define     NFP_ME_INDIRECT_PREDICATE_CC_SELECT_PLT          (9)
#define     NFP_ME_INDIRECT_PREDICATE_CC_SELECT_PLE          (10)
#define     NFP_ME_INDIRECT_PREDICATE_CC_SELECT_PGT          (11)
/*----------------------------------------------------------------
  Register: TimestampLow - Timestamp is 64 bits. It counts up by one every sixteen cycles
    Bits: 31:0  CountLower(rw) - Current 64-bit count value (lower 32 bits).
*/
#define NFP_ME_TIMESTAMP_LOW                                 0x000000c0
#define   NFP_ME_TIMESTAMP_LOW_COUNT(_x)                     (((_x) & 0xffffffff) << 0)
#define   NFP_ME_TIMESTAMP_LOW_COUNT_of(_x)                  (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TimestampHgh - Timestamp is 64 bits. It counts up by one every sixteen cycles
    Bits: 31:0  CountUpper(rw) - Current 64-bit count value (upper 32 bits).
*/
#define NFP_ME_TIMESTAMP_HIGH                                0x000000c4
#define   NFP_ME_TIMESTAMP_HIGH_COUNT(_x)                    (((_x) & 0xffffffff) << 0)
#define   NFP_ME_TIMESTAMP_HIGH_COUNT_of(_x)                 (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: XfrAndBytIdx - This register is used when Transfer registers are accessed via indexed mode
    Bits:  9:2  XferIndex(rw) - XferIndex: Transfer Index Register
    Bits:  1:0  ByteIndex(rw) - ByteIndex - Byte Index Register
*/
#define NFP_ME_T_INDEX_BYTE_INDEX                            0x000000f4
#define   NFP_ME_T_INDEX_BYTE_INDEX_XFER_INDEX(_x)           (((_x) & 0xff) << 2)
#define   NFP_ME_T_INDEX_BYTE_INDEX_XFER_INDEX_of(_x)        (((_x) >> 2) & 0xff)
#define   NFP_ME_T_INDEX_BYTE_INDEX_BYTE_NO(_x)              (((_x) & 0x3) << 0)
#define   NFP_ME_T_INDEX_BYTE_INDEX_BYTE_NO_of(_x)           (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: NxtNghbrSgl - Signal a Context in Next Neighbor
    Bits:  7    ThisContext(wo) - Controls whether or not the context field of this register is used in selecting the context that is signaled in the next neighbor microengine.
    Bits:  6:3  SignalNo(wo) - Signal to set in the next neighbor microengine.
    Bits:  2:0  Context(wo) - Context to signal in the next neighbor microengine. This field is only used if ThisContext is not asserted.
*/
#define NFP_ME_NEXT_NEIGHBOR_SIGNAL                          0x00000100
#define   NFP_ME_NEXT_NEIGHBOR_SIGNAL_THIS_CTX               BIT(7)
#define     NFP_ME_NEXT_NEIGHBOR_SIGNAL_THIS_CTX_USECTX      (0 << 7)
#define     NFP_ME_NEXT_NEIGHBOR_SIGNAL_THIS_CTX_SAMECTX     BIT(7)
#define   NFP_ME_NEXT_NEIGHBOR_SIGNAL_SIG_NO(_x)             (((_x) & 0xf) << 3)
#define   NFP_ME_NEXT_NEIGHBOR_SIGNAL_SIG_NO_of(_x)          (((_x) >> 3) & 0xf)
#define   NFP_ME_NEXT_NEIGHBOR_SIGNAL_CTX(_x)                (((_x) & 0x7) << 0)
#define   NFP_ME_NEXT_NEIGHBOR_SIGNAL_CTX_of(_x)             (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: PrvNghbrSgl - Signal a Context in Previous Neighbor
    Bits:  7    ThisContext(wo) - Controls whether or not the context field of this register is used in selecting the context that is signaled in the previous neighbor microengine.
    Bits:  6:3  SignalNo(wo) - Signal to set in the previous neighbor microengine.
    Bits:  2:0  Context(wo) - Context to signal in the previous neighbor microengine. This field is only used if ThisContext is not asserted.
*/
#define NFP_ME_PREV_NEIGHBOR_SIGNAL                          0x00000104
#define   NFP_ME_PREV_NEIGHBOR_SIGNAL_THIS_CTX               BIT(7)
#define     NFP_ME_PREV_NEIGHBOR_SIGNAL_THIS_CTX_USECTX      (0 << 7)
#define     NFP_ME_PREV_NEIGHBOR_SIGNAL_THIS_CTX_SAMECTX     BIT(7)
#define   NFP_ME_PREV_NEIGHBOR_SIGNAL_SIG_NO(_x)             (((_x) & 0xf) << 3)
#define   NFP_ME_PREV_NEIGHBOR_SIGNAL_SIG_NO_of(_x)          (((_x) >> 3) & 0xf)
#define   NFP_ME_PREV_NEIGHBOR_SIGNAL_CTX(_x)                (((_x) & 0x7) << 0)
#define   NFP_ME_PREV_NEIGHBOR_SIGNAL_CTX_of(_x)             (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: SameMESignal - Signal another Context in same Microengine.
    Bits:  7    NextContext(wo) - Controls whether or not the Context field of this register is used in selecting the context that is signaled in the microengine.
    Bits:  6:3  SignalNo(wo) - Signal to set in microengine.
    Bits:  2:0  Context(wo) - Context to signal in the microengine. This field is only used if NextContext field is not asserted.
*/
#define NFP_ME_SAME_ME_SIGNAL                                0x00000108
#define   NFP_ME_SAME_ME_SIGNAL_NEXT_CTX                     BIT(7)
#define     NFP_ME_SAME_ME_SIGNAL_NEXT_CTX_USECTX            (0 << 7)
#define     NFP_ME_SAME_ME_SIGNAL_NEXT_CTX_NEXT              BIT(7)
#define   NFP_ME_SAME_ME_SIGNAL_SIG_NO(_x)                   (((_x) & 0xf) << 3)
#define   NFP_ME_SAME_ME_SIGNAL_SIG_NO_of(_x)                (((_x) >> 3) & 0xf)
#define   NFP_ME_SAME_ME_SIGNAL_CTX(_x)                      (((_x) & 0x7) << 0)
#define   NFP_ME_SAME_ME_SIGNAL_CTX_of(_x)                   (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: CRCRemainder - Result of the CRC operation after a crc instruction.
    Bits: 31:0  CRCRemainder(rw) - Input operand and result of the CRC instruction.
*/
#define NFP_ME_CRC_REMAINDER                                 0x00000140
#define   NFP_ME_CRC_REMAINDER_CRC_REMAINDER(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_ME_CRC_REMAINDER_CRC_REMAINDER_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: ProfileCnt - The profile count is used for code profiling and tuning
    Bits: 15:0  Count(rw) - Advances by one every cycle.
*/
#define NFP_ME_PROFILE_COUNT                                 0x00000144
#define   NFP_ME_PROFILE_COUNT_COUNT(_x)                     (((_x) & 0xffff) << 0)
#define   NFP_ME_PROFILE_COUNT_COUNT_of(_x)                  (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PseudoRndNum - Random number generator
    Bits: 31:0  Number(rw) - Pseudo Random Number. This field must be non-zero to generate a pseudo random number.
*/
#define NFP_ME_PSEUDO_RANDOM_NUMBER                          0x00000148
#define   NFP_ME_PSEUDO_RANDOM_NUMBER_NUMBER(_x)             (((_x) & 0xffffffff) << 0)
#define   NFP_ME_PSEUDO_RANDOM_NUMBER_NUMBER_of(_x)          (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MiscControl - Miscellaneous Control Register
    Bits: 30    ReqOnCsEccError(rw) - This bit enables the ME to generate an external Event request (ATTN asserts) on CS ECC correctable error.
    Bits: 26    Interrupt(rw) - This bit supports the External Interrupt functionality.
    Bits: 25    ECCCorrGate(rw) - Correction on Control Store ECC errors. This bit is used to enable testing that the microengine responds correctly to hard ECC errors.
    Bits: 24    ParityEnable(rw) - Parity checking on register files.
    Bits: 23    ForceBadParity(rw) - Force incorrect parity value on register file when registers are written. This bit is used for manufacturing test and should be left as 0 for normal operation.
    Bits: 12    UstorECCCrctEn(rw) - UstoreECCCorrectEnable: Enables the microengine to correct correctable control store ECC errors. Note that ECC checking must be enabled in CtxEnables for this to have any meaning. Also note that when ECC error correcting is enabled, local CSR shadow rule is in effect. This means that any time a local CSR is changed by program control, it must not be read until the write-to-use latency is met. Specifically, software must not count on being able to user the value prior to the write in the write-to-use latency shadow. This applies to all local CSRs except LMAddr, XferIndex, NNPut, anf NNGet. For those registers the HW will guarantee that the old value can be used in the write-to-use latency shadow.
    Bits:  4    ThrdPrtyAdrMode(rw) - Selects between 32-bit and 40-bit addressing for the 3rd Party command instruction.
    Bits:  3    LegacyIndRef(rw) - Selects between Normal and Legacy Indirect Reference Modes.
    Bits:  2    ShareUstore(rw) - Enables Shared Control Store between the two MEs in an ME group.
    Bits:  1    LMRegionPull(rw) - Selects between the lower and upper halves of the 1K LM address space for CPP Pull transactions.
    Bits:  0    LMRegionPush(rw) - Selects between the lower and upper halves of the 1K LM address space for CPP Push transactions.
*/
#define NFP_ME_MISC_CONTROL                                  0x00000160
#define   NFP_ME_MISC_CONTROL_REQUEST_ON_CS_ECC_ERROR        BIT(30)
#define     NFP_ME_MISC_CONTROL_REQUEST_ON_CS_ECC_ERROR_NO_REQUEST (0 << 30)
#define     NFP_ME_MISC_CONTROL_REQUEST_ON_CS_ECC_ERROR_REQUEST BIT(30)
#define   NFP_ME_MISC_CONTROL_INTERRUPT                      BIT(26)
#define     NFP_ME_MISC_CONTROL_INTERRUPT_NOT_INTERRUPTED    (0 << 26)
#define     NFP_ME_MISC_CONTROL_INTERRUPT_INTERRUPTED        BIT(26)
#define   NFP_ME_MISC_CONTROL_ECC_CORR_GATE                  BIT(25)
#define     NFP_ME_MISC_CONTROL_ECC_CORR_GATE_DISABLE        (0 << 25)
#define     NFP_ME_MISC_CONTROL_ECC_CORR_GATE_ENABLE         BIT(25)
#define   NFP_ME_MISC_CONTROL_PARITY_ENBLE                   BIT(24)
#define     NFP_ME_MISC_CONTROL_PARITY_ENBLE_DISABLE         (0 << 24)
#define     NFP_ME_MISC_CONTROL_PARITY_ENBLE_ENABLE          BIT(24)
#define   NFP_ME_MISC_CONTROL_FORCE_BAR_PARITY               BIT(23)
#define     NFP_ME_MISC_CONTROL_FORCE_BAR_PARITY_DISABLE     (0 << 23)
#define     NFP_ME_MISC_CONTROL_FORCE_BAR_PARITY_ENABLE      BIT(23)
#define   NFP_ME_MISC_CONTROL_USTORE_ECC_CORRECT             BIT(12)
#define     NFP_ME_MISC_CONTROL_USTORE_ECC_CORRECT_DISABLE   (0 << 12)
#define     NFP_ME_MISC_CONTROL_USTORE_ECC_CORRECT_ENABLE    BIT(12)
#define   NFP_ME_MISC_CONTROL_THIRD_PARTY_ADDRESS_MODE       BIT(4)
#define     NFP_ME_MISC_CONTROL_THIRD_PARTY_ADDRESS_MODE_40BIT (0 << 4)
#define     NFP_ME_MISC_CONTROL_THIRD_PARTY_ADDRESS_MODE_32BIT BIT(4)
#define   NFP_ME_MISC_CONTROL_LEGACY_INDIRECT_REF_MODE       BIT(3)
#define     NFP_ME_MISC_CONTROL_LEGACY_INDIRECT_REF_MODE_NORMAL (0 << 3)
#define     NFP_ME_MISC_CONTROL_LEGACY_INDIRECT_REF_MODE_LEGACY BIT(3)
#define   NFP_ME_MISC_CONTROL_SHARE_USTORE                   BIT(2)
#define     NFP_ME_MISC_CONTROL_SHARE_USTORE_NOT_SHARED      (0 << 2)
#define     NFP_ME_MISC_CONTROL_SHARE_USTORE_SHARED          BIT(2)
#define   NFP_ME_MISC_CONTROL_LM_REGION_PULL                 BIT(1)
#define     NFP_ME_MISC_CONTROL_LM_REGION_PULL_LOWER_HALF    (0 << 1)
#define     NFP_ME_MISC_CONTROL_LM_REGION_PULL_UPPER_HALF    BIT(1)
#define   NFP_ME_MISC_CONTROL_LM_REGION_PUSH                 BIT(0)
#define     NFP_ME_MISC_CONTROL_LM_REGION_PUSH_LOWER_HALF    (0 << 0)
#define     NFP_ME_MISC_CONTROL_LM_REGION_PUSH_UPPER_HALF    BIT(0)
/*----------------------------------------------------------------
  Register: PcBreakpoint0Mask - Mask register associated with PC Breakpoint 0
  Register: PcBreakpoint1Mask - Mask register associated with PC Breakpoint 1
    Bits: 13:0  PcMask(rw) - Masks the PC (program counter) bits which should not be considered for comparison against the value in the PcBrkpnt field of the PcBreakpoint CSR.
*/
#define NFP_ME_PC_BREAKPOINT_0_MASK                          0x00000164
#define NFP_ME_PC_BREAKPOINT_1_MASK                          0x00000168
#define   NFP_ME_PCBREAKPOINTMASK_MASK(_x)                   (((_x) & 0x3fff) << 0)
#define   NFP_ME_PCBREAKPOINTMASK_MASK_of(_x)                (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: Mailbox0 - Mailbox Register 0
  Register: Mailbox1 - Mailbox Register 1
  Register: Mailbox2 - Mailbox Register 2
  Register: Mailbox3 - Mailbox Register 3
    Bits: 31:0  Mailbox(rw) - Mailbox Data value defined by the application.
*/
#define NFP_ME_MAILBOX_0                                     0x00000170
#define NFP_ME_MAILBOX_1                                     0x00000174
#define NFP_ME_MAILBOX_2                                     0x00000178
#define NFP_ME_MAILBOX_3                                     0x0000017c
#define   NFP_ME_MAILBOX_MAILBOX(_x)                         (((_x) & 0xffffffff) << 0)
#define   NFP_ME_MAILBOX_MAILBOX_of(_x)                      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CmdIndirectRef0 - Command Indirect Reference Register Type 0
    Bits: 29:24 Island(rw) - Override value for the CPP command's island field.
    Bits: 23:20 Master(rw) - Various overriding definitions depending on the value of the OVE_MASTER field in the previous ALU.
    Bits: 19:16 SignalMaster(rw) - Override value for the CPP command's signal master field.
    Bits: 15:13 SignalCtx(rw) - Override value for the CPP command's signal context field.
    Bits: 12:9  SignalNum(rw) - Override value for the CPP command's signal number field.
    Bits:  7:0  ByteMask(rw) - Override value for the CPP command's byte mask field.
*/
#define NFP_ME_CMD_INDIRECT_REF_0                            0x00000190
#define   NFP_ME_CMD_INDIRECT_REF_0_ISLAND(_x)               (((_x) & 0x3f) << 24)
#define   NFP_ME_CMD_INDIRECT_REF_0_ISLAND_of(_x)            (((_x) >> 24) & 0x3f)
#define   NFP_ME_CMD_INDIRECT_REF_0_MASTER(_x)               (((_x) & 0xf) << 20)
#define   NFP_ME_CMD_INDIRECT_REF_0_MASTER_of(_x)            (((_x) >> 20) & 0xf)
#define   NFP_ME_CMD_INDIRECT_REF_0_SIGNAL_MASTER(_x)        (((_x) & 0xf) << 16)
#define   NFP_ME_CMD_INDIRECT_REF_0_SIGNAL_MASTER_of(_x)     (((_x) >> 16) & 0xf)
#define   NFP_ME_CMD_INDIRECT_REF_0_SIGNAL_CTX(_x)           (((_x) & 0x7) << 13)
#define   NFP_ME_CMD_INDIRECT_REF_0_SIGNAL_CTX_of(_x)        (((_x) >> 13) & 0x7)
#define   NFP_ME_CMD_INDIRECT_REF_0_SIGNAL_NUM(_x)           (((_x) & 0xf) << 9)
#define   NFP_ME_CMD_INDIRECT_REF_0_SIGNAL_NUM_of(_x)        (((_x) >> 9) & 0xf)
#define   NFP_ME_CMD_INDIRECT_REF_0_BYTE_MASK(_x)            (((_x) & 0xff) << 0)
#define   NFP_ME_CMD_INDIRECT_REF_0_BYTE_MASK_of(_x)         (((_x) >> 0) & 0xff)

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: UstorAddr - Used to load programs into the Control Store
    Bits: 31    EnableCS(rw) - Enable Control Store: The Microengine should be in Idle state (no contexts running). The address in Uaddr field specifies the Control Store address where the data written to UstorDataLwr and UstorDataUpr will be written. Also set in debug mode. This bit can be used to dump data from Microengine GPRs and Read Transfer registers. The Microengine should be in an idle state (no contexts running). This forces the Microengine to continuously execute an instruction at the address specified by Uaddr. Only the ALU instruction is supported in this mode and the result of the execution is written to ALUOut CSR rather than a destination register.
    Bits: 30    UstoreWrStrb(rw) - UstoreWriteStrobe
    Bits: 29    UstoreDataInvert(rw) - UstoreDataInvert
    Bits: 12:0  Uaddr(rw) - Address of control store location to be accessed. Valid values are 0 to 8191.
*/
#define NFP_ME_UstorAddr                                     0x00000000
#define   NFP_ME_UstorAddr_EnableCS                          BIT(31)
#define     NFP_ME_UstorAddr_EnableCS_NORMAL                 (0 << 31)
#define     NFP_ME_UstorAddr_EnableCS_RW                     BIT(31)
#define   NFP_ME_UstorAddr_UstoreWrStrb                      BIT(30)
#define     NFP_ME_UstorAddr_UstoreWrStrb_DISABLE            (0 << 30)
#define     NFP_ME_UstorAddr_UstoreWrStrb_ENABLE             BIT(30)
#define   NFP_ME_UstorAddr_UstoreDataInvert                  BIT(29)
#define     NFP_ME_UstorAddr_UstoreDataInvert_DISABLE        (0 << 29)
#define     NFP_ME_UstorAddr_UstoreDataInvert_ENABLE         BIT(29)
#define   NFP_ME_UstorAddr_Uaddr(_x)                         (((_x) & 0x1fff) << 0)
#define   NFP_ME_UstorAddr_Uaddr_of(_x)                      (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: UstorDataLwr - Control Store Data - lower
    Bits: 31:0  UdataLower(rw) - Contains bits 31:00 of the instruction of the Control Store location specified by the UstorAddr CSR.
*/
#define NFP_ME_UstorDataLwr                                  0x00000004
#define   NFP_ME_UstorDataLwr_UdataLower(_x)                 (((_x) & 0xffffffff) << 0)
#define   NFP_ME_UstorDataLwr_UdataLower_of(_x)              (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: UstorDataUpr - Control Store Data - upper
    Bits: 19:13 ECC(rw) - Contains the ECC Check bits for the instruction
    Bits: 12:0  UdataUpper(rw) - Contains the data from bits [43:32] of the control store location specified by the UstorAddr.
*/
#define NFP_ME_UstorDataUpr                                  0x00000008
#define   NFP_ME_UstorDataUpr_ECC(_x)                        (((_x) & 0x7f) << 13)
#define   NFP_ME_UstorDataUpr_ECC_of(_x)                     (((_x) >> 13) & 0x7f)
#define   NFP_ME_UstorDataUpr_UdataUpper(_x)                 (((_x) & 0x1fff) << 0)
#define   NFP_ME_UstorDataUpr_UdataUpper_of(_x)              (((_x) >> 0) & 0x1fff)
/*----------------------------------------------------------------
  Register: UstorErrStat - ECC errors during Control Store reads
    Bits: 31    UncorrectableErr(ro) - Error Type. This bit indicates the type of error detected.
    Bits: 26:20 Syndrome(ro) - This field records the syndrome that was found when the error occurred.
    Bits: 18:16 Context(ro) - Context that was executing when the ECC error occurred
    Bits: 13:0  Uaddr(ro) - Contains the address that had the ECC error.
*/
#define NFP_ME_UstorErrStat                                  0x0000000c
#define   NFP_ME_UstorErrStat_UncorrectableErr               BIT(31)
#define     NFP_ME_UstorErrStat_UncorrectableErr_NOT_OCCURED (0 << 31)
#define     NFP_ME_UstorErrStat_UncorrectableErr_OCCURED     BIT(31)
#define   NFP_ME_UstorErrStat_Syndrome(_x)                   (((_x) & 0x7f) << 20)
#define   NFP_ME_UstorErrStat_Syndrome_of(_x)                (((_x) >> 20) & 0x7f)
#define   NFP_ME_UstorErrStat_Context(_x)                    (((_x) & 0x7) << 16)
#define   NFP_ME_UstorErrStat_Context_of(_x)                 (((_x) >> 16) & 0x7)
#define   NFP_ME_UstorErrStat_Uaddr(_x)                      (((_x) & 0x3fff) << 0)
#define   NFP_ME_UstorErrStat_Uaddr_of(_x)                   (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: ALUOut - Debug to show state of ALU
    Bits: 31:0  ALUOutput(ro) - ALU output.
*/
#define NFP_ME_ALUOut                                        0x00000010
#define   NFP_ME_ALUOut_ALUOutput(_x)                        (((_x) & 0xffffffff) << 0)
#define   NFP_ME_ALUOut_ALUOutput_of(_x)                     (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CtxArbCtrl - Context Arbiter Control - used by the context arbiter and for debug
    Bits:  6:4  PreviousCtx(ro) - Previous Context. This field contains the number of the last context that was running.
    Bits:  2:0  NextCtx(rw) - Next Context. This field contains the number of the next context that will be run.
*/
#define NFP_ME_CtxArbCtrl                                    0x00000014
#define   NFP_ME_CtxArbCtrl_PreviousCtx(_x)                  (((_x) & 0x7) << 4)
#define   NFP_ME_CtxArbCtrl_PreviousCtx_of(_x)               (((_x) >> 4) & 0x7)
#define   NFP_ME_CtxArbCtrl_NextCtx(_x)                      (((_x) & 0x7) << 0)
#define   NFP_ME_CtxArbCtrl_NextCtx_of(_x)                   (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: CtxEnables - Context Enables - used by the context arbiter and for debug
    Bits: 31    InUseContexts(rw) - Indicates the number of in-use contexts, which determines the GPR and Transfer Register allocation. Note that although this information could be inferred from bits C0 to C7, this field allows for contexts to be temporarily disabled due to error or debugging conditions. It is illegal to enable Contexts that are not currently in-use according to this field.
    Bits: 30    PseudoRandNum(rw) - Controls when the Pseudo_Random_Number is generated.
    Bits: 29    CSEccError(rw1c) - Indicates that an ECC error was detected in the Control Store when an instruction was read. This bit will never be set if ECCErrorEnable bit is 0. When this bit is set the Microengine's attn output is asserted. More information about the error is available in UstorErrStat Register.
    Bits: 28    CSEccEnable(rw) - Enables ECC error detection on Control Store.
    Bits: 27    Breakpoint(rw1c) - The ctx_arb[bpt] instruction was executed. When this bit is set, the Microengine's attn output is asserted and all CtxEnables bits in this register are cleared.
    Bits: 25    RegisterParityErr(rw1c) - Indicates that a parity error was detected when reading a datapath register. When this bit is set, the Microengine's attn output is asserted and all CtxEnables bits in this register are cleared. RegErrStatus Local CSR has information about the error.
    Bits: 23    LMEccEnable(rw) - Enables ECC error detection on Local Memory.
    Bits: 22    LMAddr3Global(rw) - Controls usage of ActLMAddr3.
    Bits: 21    LMAddr2Global(rw) - Controls usage of ActLMAddr2.
    Bits: 20    NNsendConfig(rw) - This bit controls the ME's Next-Neighbor 'Send' Interface; it controls whether a Next-Neighbor destination from this ME is written to the Next Neighbor ME (general-usage) or to this same ME (mostly for debugging usage); 'same ME' refers to the ME executing the instruction which carries the NN destination; 'same ME' or 'Self' refer to the same configuration.
    Bits: 19:18 NextNeighborEmptyAssert(rw) - Controls threshold when NN_Empty asserts. The number of entries valid is determined by comparing NNPut and NNGet Local CSRs. The use of indicating empty when there is really something on the Ring is if the cooperating processes transfer data in a block, and the consumer does not want to get a partial block.
    Bits: 17    LMAddr1Global(rw) - Controls usage of ActLMAddr1.
    Bits: 16    LMAddr0Global(rw) - Controls usage of ActLMAddr0.
    Bits: 15:8  CtxEnables(rw) - Context Enables for Context 7 through Context 0.
    Bits:  2:0  NNreceiveConfig(rw) - This field controls the ME's Next-Neighbor 'Receive' Interface; it selects which path has access to the NN Register File's write port; in MEv2.8, four different NN paths may be selected: NN from previous ME, Self, CTnn, and PushBus; please note that when the NNsendConfig bit is set to 'Self', the NNreceiveConfig field must also be set to 'Self'; otherwise, the NN destination data generated from 'this ME' is lost
*/
#define NFP_ME_CtxEnables                                    0x00000018
#define   NFP_ME_CtxEnables_InUseContexts                    BIT(31)
#define     NFP_ME_CtxEnables_InUseContexts_8CTX             (0 << 31)
#define     NFP_ME_CtxEnables_InUseContexts_4CTX             BIT(31)
#define   NFP_ME_CtxEnables_PseudoRandNum                    BIT(30)
#define     NFP_ME_CtxEnables_PseudoRandNum_ME_INSTR         (0 << 30)
#define     NFP_ME_CtxEnables_PseudoRandNum_EVERY_CYCLE      BIT(30)
#define   NFP_ME_CtxEnables_CSEccError                       BIT(29)
#define   NFP_ME_CtxEnables_CSEccEnable                      BIT(28)
#define     NFP_ME_CtxEnables_CSEccEnable_DISABLE            (0 << 28)
#define     NFP_ME_CtxEnables_CSEccEnable_ENABLE             BIT(28)
#define   NFP_ME_CtxEnables_Breakpoint                       BIT(27)
#define   NFP_ME_CtxEnables_RegisterParityErr                BIT(25)
#define     NFP_ME_CtxEnables_RegisterParityErr_NONE         (0 << 25)
#define     NFP_ME_CtxEnables_RegisterParityErr_DETECTED     BIT(25)
#define   NFP_ME_CtxEnables_LMEccEnable                      BIT(23)
#define     NFP_ME_CtxEnables_LMEccEnable_DISABLE            (0 << 23)
#define     NFP_ME_CtxEnables_LMEccEnable_ENABLE             BIT(23)
#define   NFP_ME_CtxEnables_LMAddr3Global                    BIT(22)
#define     NFP_ME_CtxEnables_LMAddr3Global_RELATIVE         (0 << 22)
#define     NFP_ME_CtxEnables_LMAddr3Global_GLOBAL           BIT(22)
#define   NFP_ME_CtxEnables_LMAddr2Global                    BIT(21)
#define     NFP_ME_CtxEnables_LMAddr2Global_RELATIVE         (0 << 21)
#define     NFP_ME_CtxEnables_LMAddr2Global_GLOBAL           BIT(21)
#define   NFP_ME_CtxEnables_NNsendConfig                     BIT(20)
#define     NFP_ME_CtxEnables_NNsendConfig_NEXT              (0 << 20)
#define     NFP_ME_CtxEnables_NNsendConfig_SELF              BIT(20)
#define   NFP_ME_CtxEnables_NextNeighborEmptyAssert(_x)      (((_x) & 0x3) << 18)
#define   NFP_ME_CtxEnables_NextNeighborEmptyAssert_of(_x)   (((_x) >> 18) & 0x3)
#define     NFP_ME_CtxEnables_NextNeighborEmptyAssert_NONE   (0)
#define     NFP_ME_CtxEnables_NextNeighborEmptyAssert_LE1    (1)
#define     NFP_ME_CtxEnables_NextNeighborEmptyAssert_LE2    (2)
#define     NFP_ME_CtxEnables_NextNeighborEmptyAssert_LE3    (3)
#define   NFP_ME_CtxEnables_LMAddr1Global                    BIT(17)
#define     NFP_ME_CtxEnables_LMAddr1Global_RELATIVE         (0 << 17)
#define     NFP_ME_CtxEnables_LMAddr1Global_GLOBAL           BIT(17)
#define   NFP_ME_CtxEnables_LMAddr0Global                    BIT(16)
#define     NFP_ME_CtxEnables_LMAddr0Global_RELATIVE         (0 << 16)
#define     NFP_ME_CtxEnables_LMAddr0Global_GLOBAL           BIT(16)
#define   NFP_ME_CtxEnables_CtxEnables(_x)                   (((_x) & 0xff) << 8)
#define   NFP_ME_CtxEnables_CtxEnables_of(_x)                (((_x) >> 8) & 0xff)
#define     NFP_ME_CtxEnables_CtxEnables_NONE                (0)
#define     NFP_ME_CtxEnables_CtxEnables_LE1                 (1)
#define   NFP_ME_CtxEnables_NNreceiveConfig(_x)              (((_x) & 0x7) << 0)
#define   NFP_ME_CtxEnables_NNreceiveConfig_of(_x)           (((_x) >> 0) & 0x7)
#define     NFP_ME_CtxEnables_NNreceiveConfig_NN             (0)
#define     NFP_ME_CtxEnables_NNreceiveConfig_SELF           (1)
#define     NFP_ME_CtxEnables_NNreceiveConfig_CTnn           (2)
#define     NFP_ME_CtxEnables_NNreceiveConfig_PushBus        (3)
/*----------------------------------------------------------------
  Register: CondCodeEn - Condition Code Enable
    Bits: 13    CurCndtnCodeEn(rw) - Current Condition Code Enable. Set to 1 to update the condition codes. When 0, condition codes will not be updated.
*/
#define NFP_ME_CondCodeEn                                    0x0000001c
#define   NFP_ME_CondCodeEn_CurCndtnCodeEn                   BIT(13)
/*----------------------------------------------------------------
  Register: CSRCtxPtr - CSR Context Pointer
    Bits:  2:0  CtxSelect(rw) - Selects which contexts Local CSR is accessed by local_csr_read, local_csr_write, and by the IA core.
*/
#define NFP_ME_CSRCtxPtr                                     0x00000020
#define   NFP_ME_CSRCtxPtr_CtxSelect(_x)                     (((_x) & 0x7) << 0)
#define   NFP_ME_CSRCtxPtr_CtxSelect_of(_x)                  (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: PcBreakpoint0 - PC Breakpoint 0 - PCB system
  Register: PcBreakpoint1 - PC Breakpoint 1 - PCB system
    Bits: 31:24 PcBrkpntCntl(rw) - PC Breakpoint Control: Controls the PC Breakpoint0 SDS-mechanism.
    Bits: 13:0  PcBrkpntAddr(rw) - PC Breakpoint Address: PC Address corresponding to PcBreakpoint0; it gets compared to the ME Active PC value.
*/
#define NFP_ME_PcBreakpoint0                                 0x00000024
#define NFP_ME_PcBreakpoint1                                 0x00000028
#define   NFP_ME_PcBreakpoint_PcBrkpntCntl(_x)               (((_x) & 0xff) << 24)
#define   NFP_ME_PcBreakpoint_PcBrkpntCntl_of(_x)            (((_x) >> 24) & 0xff)
#define   NFP_ME_PcBreakpoint_PcBrkpntAddr(_x)               (((_x) & 0x3fff) << 0)
#define   NFP_ME_PcBreakpoint_PcBrkpntAddr_of(_x)            (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: PcBreakpointStatus - PC Breakpoint - Status register associated with the PCB system
    Bits:  9:2  PcBrkpntedCtx(rw1c) - Indicates which context has triggered the PC breakpoint event; the least significant bit of this fields is associated with ctx 0; the most significant bit with ctx 7
    Bits:  1    PcBrkpnt1Status(rw1c) - Indicates the status of the PcBreakpoint1 SDS mechanism
    Bits:  0    PcBrkpnt0Status(rw1c) - Indicates the status of the PcBreakpoint0 SDS mechanism
*/
#define NFP_ME_PcBreakpointStatus                            0x0000002c
#define   NFP_ME_PcBreakpointStatus_PcBrkpntedCtx(_x)        (((_x) & 0xff) << 2)
#define   NFP_ME_PcBreakpointStatus_PcBrkpntedCtx_of(_x)     (((_x) >> 2) & 0xff)
#define   NFP_ME_PcBreakpointStatus_PcBrkpnt1Status          BIT(1)
#define     NFP_ME_PcBreakpointStatus_PcBrkpnt1Status_VAL0   (0 << 1)
#define     NFP_ME_PcBreakpointStatus_PcBrkpnt1Status_VAL1   BIT(1)
#define   NFP_ME_PcBreakpointStatus_PcBrkpnt0Status          BIT(0)
#define     NFP_ME_PcBreakpointStatus_PcBrkpnt0Status_VAL0   (0 << 0)
#define     NFP_ME_PcBreakpointStatus_PcBrkpnt0Status_VAL1   BIT(0)
/*----------------------------------------------------------------
  Register: RegErrStatus - Information about parity errors detected on Datapath Regs
    Bits: 17:16 Type(ro) - Type of Register File where a Parity Error has been detected.
    Bits: 15:0  RegisterNumber(ro) - Entry Address of the Register File where a Parity Error has been detected. Unused upper bits will read 0.
*/
#define NFP_ME_RegErrStatus                                  0x00000030
#define   NFP_ME_RegErrStatus_Type(_x)                       (((_x) & 0x3) << 16)
#define   NFP_ME_RegErrStatus_Type_of(_x)                    (((_x) >> 16) & 0x3)
#define     NFP_ME_RegErrStatus_Type_GPR                     (0)
#define     NFP_ME_RegErrStatus_Type_XFER                    (1)
#define     NFP_ME_RegErrStatus_Type_NN                      (2)
#define   NFP_ME_RegErrStatus_RegisterNumber(_x)             (((_x) & 0xffff) << 0)
#define   NFP_ME_RegErrStatus_RegisterNumber_of(_x)          (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: LMErrStatus - Status on ECC errors recorded on Local Memory reads
    Bits: 31    UncorrectableErr(ro) - This bit indicates that the ECC error detected on Local Memory is uncorrectable
    Bits: 26:20 Syndrome(ro) - This field records the syndrome that was found when the ECC error occurred.
    Bits: 18:16 Context(ro) - Context that was executing when the ECC error occurred
    Bits:  9:0  LMaddress(ro) - Contains the LM address where the ECC error has been detected.
*/
#define NFP_ME_LMErrStatus                                   0x00000034
#define   NFP_ME_LMErrStatus_UncorrectableErr                BIT(31)
#define     NFP_ME_LMErrStatus_UncorrectableErr_NOT_OCCURED  (0 << 31)
#define     NFP_ME_LMErrStatus_UncorrectableErr_OCCURED      BIT(31)
#define   NFP_ME_LMErrStatus_Syndrome(_x)                    (((_x) & 0x7f) << 20)
#define   NFP_ME_LMErrStatus_Syndrome_of(_x)                 (((_x) >> 20) & 0x7f)
#define   NFP_ME_LMErrStatus_Context(_x)                     (((_x) & 0x7) << 16)
#define   NFP_ME_LMErrStatus_Context_of(_x)                  (((_x) >> 16) & 0x7)
#define   NFP_ME_LMErrStatus_LMaddress(_x)                   (((_x) & 0x3ff) << 0)
#define   NFP_ME_LMErrStatus_LMaddress_of(_x)                (((_x) >> 0) & 0x3ff)
/*----------------------------------------------------------------
  Register: LMeccErrorMask - Controls Error Injection bits into an LM data-path word.
    Bits: 31:0  ErrorInjectionMask(rw) - This field is used for controlling Error Injection bits into an LM data-path word.
*/
#define NFP_ME_LMeccErrorMask                                0x00000038
#define   NFP_ME_LMeccErrorMask_ErrorInjectionMask(_x)       (((_x) & 0xffffffff) << 0)
#define   NFP_ME_LMeccErrorMask_ErrorInjectionMask_of(_x)    (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: IndCtxStatus - Indirect Context Status Register
    Bits: 31    ReadyToRun(ro) - Ready to Run. Indicates that the context is in Ready state. (This bit will be 0 if the context is in any of the other three states.)
    Bits: 13:0  ContextPC(rw) - The program counter at which the context begins executing when it is put into the executing state.
*/
#define NFP_ME_IndCtxStatus                                  0x00000040
#define   NFP_ME_IndCtxStatus_ReadyToRun                     BIT(31)
#define     NFP_ME_IndCtxStatus_ReadyToRun_NOT_READY         (0 << 31)
#define     NFP_ME_IndCtxStatus_ReadyToRun_READY             BIT(31)
#define   NFP_ME_IndCtxStatus_ContextPC(_x)                  (((_x) & 0x3fff) << 0)
#define   NFP_ME_IndCtxStatus_ContextPC_of(_x)               (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: ActCtxStatus - Active Context Status Register
    Bits: 31    AB0(ro) - If set, the microengine has a context in the Executing state. If clear, no context is in Executing state.
    Bits: 30:25 IslandId(ro) - Island number where the ME happens to be instantiated.
    Bits: 24:8  ActiveContextPC(ro) - PC of Executing Context. Only valid if AB0 is a 1. This field provides a snapshot value of the PC. This value is used for tracking/code profiling purposes. When issued as a local_csr_read from the Microengine, the PC value may not be the exact PC value of the local_csr_rd instruction.
    Bits:  6:3  MENumber(ro) - A unique number which identifies the ME within the Island where it happens to be instantiated.
    Bits:  2:0  ActiveContextNo(rw) - The number of the Executing context. Only valid if AB0 bit is a 1.
*/
#define NFP_ME_ActCtxStatus                                  0x00000044
#define   NFP_ME_ActCtxStatus_AB0                            BIT(31)
#define     NFP_ME_ActCtxStatus_AB0_NOT_ACTIVE               (0 << 31)
#define     NFP_ME_ActCtxStatus_AB0_ACTIVE                   BIT(31)
#define   NFP_ME_ActCtxStatus_IslandId(_x)                   (((_x) & 0x3f) << 25)
#define   NFP_ME_ActCtxStatus_IslandId_of(_x)                (((_x) >> 25) & 0x3f)
#define   NFP_ME_ActCtxStatus_ActiveContextPC(_x)            (((_x) & 0x1ffff) << 8)
#define   NFP_ME_ActCtxStatus_ActiveContextPC_of(_x)         (((_x) >> 8) & 0x1ffff)
#define   NFP_ME_ActCtxStatus_MENumber(_x)                   (((_x) & 0xf) << 3)
#define   NFP_ME_ActCtxStatus_MENumber_of(_x)                (((_x) >> 3) & 0xf)
#define   NFP_ME_ActCtxStatus_ActiveContextNo(_x)            (((_x) & 0x7) << 0)
#define   NFP_ME_ActCtxStatus_ActiveContextNo_of(_x)         (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: IndCtxSglEvt - Indirect Context Signal Events Register
  Register: ActCtxSglEvt - Active Context Signal Events Register
    Bits: 15:1  Signals(rw) - Each bit is set as described in the Event Signals section. Each is cleared by microengine hardware when: the signal is used to transition to Ready state if the CtxWkpEvt[AnyWakeEvnts] bit is clear, or a br_!signal on this signal is not taken, or a br_signal on this signal is taken.
    Bits:  0    VoluntaryWakeup(ro) - Corresponds to Event for Voluntary arb wakeup event.
*/
#define NFP_ME_IndCtxSglEvt                                  0x00000048
#define NFP_ME_ActCtxSglEvt                                  0x0000004c
#define   NFP_ME_CtxSglEvt_Signals(_x)                       (((_x) & 0x7fff) << 1)
#define   NFP_ME_CtxSglEvt_Signals_of(_x)                    (((_x) >> 1) & 0x7fff)
#define   NFP_ME_CtxSglEvt_VoluntaryWakeup                   BIT(0)
/*----------------------------------------------------------------
  Register: IndCtxWkpEvt - Indirect Context Wakeup Events Register
  Register: ActCtxWkpEvt - Active Context Wakeup Events Register
    Bits: 16    AnyWakeEvnts(rw) - Any Wakeup Events. Set by the ANY token on a ctx_arb instruction. Note: this bit is undefined after a wakeup.
    Bits: 15:1  WakeupEvents(rw) - Each wakeup event bit is set by either a ctx_swap_# token on an instruction, or by the Event Signal Mask of the ctx_arb instruction. All wakeup event bits are cleared by microengine hardware whether the context is put into execute state.
    Bits:  0    Voluntary(rw) - Set by ctx_arb[voluntary]. Cleared when the context is put into Execute state.
*/
#define NFP_ME_IndCtxWkpEvt                                  0x00000050
#define NFP_ME_ActCtxWkpEvt                                  0x00000054
#define   NFP_ME_CtxWkpEvt_AnyWakeEvnts                      BIT(16)
#define     NFP_ME_CtxWkpEvt_AnyWakeEvnts_ALL                (0 << 16)
#define     NFP_ME_CtxWkpEvt_AnyWakeEvnts_ANY                BIT(16)
#define   NFP_ME_CtxWkpEvt_WakeupEvents(_x)                  (((_x) & 0x7fff) << 1)
#define   NFP_ME_CtxWkpEvt_WakeupEvents_of(_x)               (((_x) >> 1) & 0x7fff)
#define   NFP_ME_CtxWkpEvt_Voluntary                         BIT(0)
/*----------------------------------------------------------------
  Register: IndCtxFtrCnt - Indirect Context Future Count Register
  Register: ActCtxFtrCnt - Active Context Future Count Register
    Bits: 15:0  FutureCount(rw) - Value to match against low 32-bits of Timestamp.
*/
#define NFP_ME_IndCtxFtrCnt                                  0x00000058
#define NFP_ME_ActCtxFtrCnt                                  0x0000005c
#define   NFP_ME_CtxFtrCnt_FutureCount(_x)                   (((_x) & 0xffff) << 0)
#define   NFP_ME_CtxFtrCnt_FutureCount_of(_x)                (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: IndLMAddr0 - Indirect Local Memory Address 0 Register
  Register: ActLMAddr0 - Active Local Memory Address 0 Register
  Register: IndLMAddr1 - Indirect Local Memory Address 1 Register
  Register: ActLMAddr1 - Active Local Memory Address 1 Register
  Register: IndLMAddr2 - Indirect Local Memory Address 2 Register
  Register: ActLMAddr2 - Active Local Memory Address 2 Register
  Register: IndLMAddr3 - Indirect Local Memory Address 3 Register
  Register: ActLMAddr3 - Active Local Memory Address 3 Register
    Bits: 11:2  LocalMemoryAddr(rw) - Selects the specific 32-bit word in Local Memory. This field can be incremented or decremented by 1, or left unchanged after access, as specified in the instruction.
*/
#define NFP_ME_IndLMAddr0                                    0x00000060
#define NFP_ME_ActLMAddr0                                    0x00000064
#define NFP_ME_IndLMAddr1                                    0x00000068
#define NFP_ME_ActLMAddr1                                    0x0000006c
#define NFP_ME_IndLMAddr2                                    0x00000090
#define NFP_ME_ActLMAddr2                                    0x00000094
#define NFP_ME_IndLMAddr3                                    0x00000098
#define NFP_ME_ActLMAddr3                                    0x0000009c
#define   NFP_ME_LMAddr_LocalMemoryAddr(_x)                  (((_x) & 0x3ff) << 2)
#define   NFP_ME_LMAddr_LocalMemoryAddr_of(_x)               (((_x) >> 2) & 0x3ff)
/*----------------------------------------------------------------
  Register: ByteIndex - Byte Index Register
    Bits:  1:0  ByteNumber(rw) - Specifies a byte for use with the byte_align instruction.
*/
#define NFP_ME_ByteIndex                                     0x00000070
#define   NFP_ME_ByteIndex_ByteNumber(_x)                    (((_x) & 0x3) << 0)
#define   NFP_ME_ByteIndex_ByteNumber_of(_x)                 (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: XferIndex - Transfer Index Register
    Bits:  9:2  XferIndex(rw) - Transfer Register Index. Specifies one of 256 registers. The choice of TRANSFER_IN vs. TRANSFER_OUT is made based on the register use (either source or destination). This field can be incremented or decremented by 1, or left unchanged after the access, as specified in the instruction.
*/
#define NFP_ME_XferIndex                                     0x00000074
#define   NFP_ME_XferIndex_XferIndex(_x)                     (((_x) & 0xff) << 2)
#define   NFP_ME_XferIndex_XferIndex_of(_x)                  (((_x) >> 2) & 0xff)
/*----------------------------------------------------------------
  Register: IndFtrCntSgl - Which signal to set when FUTURE_COUNT == TIMESTAMP
  Register: ActFtrCntSgl - Which signal to set when FUTURE_COUNT == TIMESTAMP
    Bits:  3:0  SignalNo(rw) - The signal number to set when FUTURE_COUNT == TIMESTAMP.
*/
#define NFP_ME_IndFtrCntSgl                                  0x00000078
#define NFP_ME_ActFtrCntSgl                                  0x0000007c
#define   NFP_ME_FtrCntSgl_SignalNo(_x)                      (((_x) & 0xf) << 0)
#define   NFP_ME_FtrCntSgl_SignalNo_of(_x)                   (((_x) >> 0) & 0xf)
/*----------------------------------------------------------------
  Register: NNPut - Next Neighbor Put Register
  Register: NNGet - Next Neighbor Get Register
    Bits:  6:0  NNRegIndex(rw) - Specifies one of 128 NN registers to read.
*/
#define NFP_ME_NNPut                                         0x00000080
#define NFP_ME_NNGet                                         0x00000084
#define   NFP_ME_NNPutGet_NNRegIndex(_x)                     (((_x) & 0x7f) << 0)
#define   NFP_ME_NNPutGet_NNRegIndex_of(_x)                  (((_x) >> 0) & 0x7f)
/*----------------------------------------------------------------
  Register: IndLMAddr2BytIdx - Alias of IndLMAddr2 and ByteIndex
  Register: ActLMAddr2BytIdx - Alias of ActLMAddr2 and ByteIndex
  Register: IndLMAddr3BytIdx - Alias of IndLMAddr3 and ByteIndex
  Register: ActLMAddr3BytIdx - Alias of ActLMAddr3 and ByteIndex
  Register: IndLMAddr0BytIdx - Alias of IndLMAddr0 and ByteIndex
  Register: ActLMAddr0BytIdx - Alias of ActLMAddr0 and ByteIndex
  Register: IndLMAddr1BytIdx - Alias of IndLMAddr1 and ByteIndex
  Register: ActLMAddr1BytIdx - Alias of ActLMAddr1 and ByteIndex
    Bits: 11:2  LocalMemoryAddr(rw) - Indirect Local Memory Address Register.
    Bits:  1:0  ByteNumber(rw) - Byte Index Register.
*/
#define NFP_ME_IndLMAddr2BytIdx                              0x000000a0
#define NFP_ME_ActLMAddr2BytIdx                              0x000000a4
#define NFP_ME_IndLMAddr3BytIdx                              0x000000a8
#define NFP_ME_ActLMAddr3BytIdx                              0x000000ac
#define NFP_ME_IndLMAddr0BytIdx                              0x000000e0
#define NFP_ME_ActLMAddr0BytIdx                              0x000000e4
#define NFP_ME_IndLMAddr1BytIdx                              0x000000e8
#define NFP_ME_ActLMAddr1BytIdx                              0x000000ec
#define   NFP_ME_LMAddrBytIdx_LocalMemoryAddr(_x)            (((_x) & 0x3ff) << 2)
#define   NFP_ME_LMAddrBytIdx_LocalMemoryAddr_of(_x)         (((_x) >> 2) & 0x3ff)
#define   NFP_ME_LMAddrBytIdx_ByteNumber(_x)                 (((_x) & 0x3) << 0)
#define   NFP_ME_LMAddrBytIdx_ByteNumber_of(_x)              (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: IndPredCC - Indirect Predicate CC select
    Bits:  3:0  PredCCSel(rw) - Encoded value for selecting the Predicate Condition Code; encoded values select Condition Codes as shown below
*/
#define NFP_ME_IndPredCC                                     0x000000b0
#define   NFP_ME_IndPredCC_PredCCSel(_x)                     (((_x) & 0xf) << 0)
#define   NFP_ME_IndPredCC_PredCCSel_of(_x)                  (((_x) >> 0) & 0xf)
#define     NFP_ME_IndPredCC_PredCCSel_PEQ                   (0)
#define     NFP_ME_IndPredCC_PredCCSel_PNE                   (1)
#define     NFP_ME_IndPredCC_PredCCSel_PMI                   (2)
#define     NFP_ME_IndPredCC_PredCCSel_PPL                   (3)
#define     NFP_ME_IndPredCC_PredCCSel_PCS                   (4)
#define     NFP_ME_IndPredCC_PredCCSel_PCC                   (5)
#define     NFP_ME_IndPredCC_PredCCSel_PVS                   (6)
#define     NFP_ME_IndPredCC_PredCCSel_PVC                   (7)
#define     NFP_ME_IndPredCC_PredCCSel_PGE                   (8)
#define     NFP_ME_IndPredCC_PredCCSel_PLT                   (9)
#define     NFP_ME_IndPredCC_PredCCSel_PLE                   (10)
#define     NFP_ME_IndPredCC_PredCCSel_PGT                   (11)
/*----------------------------------------------------------------
  Register: TimestampLow - Timestamp is 64 bits. It counts up by one every sixteen cycles
    Bits: 31:0  CountLower(rw) - Current 64-bit count value (lower 32 bits).
*/
#define NFP_ME_TimestampLow                                  0x000000c0
#define   NFP_ME_TimestampLow_CountLower(_x)                 (((_x) & 0xffffffff) << 0)
#define   NFP_ME_TimestampLow_CountLower_of(_x)              (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: TimestampHgh - Timestamp is 64 bits. It counts up by one every sixteen cycles
    Bits: 31:0  CountUpper(rw) - Current 64-bit count value (upper 32 bits).
*/
#define NFP_ME_TimestampHgh                                  0x000000c4
#define   NFP_ME_TimestampHgh_CountUpper(_x)                 (((_x) & 0xffffffff) << 0)
#define   NFP_ME_TimestampHgh_CountUpper_of(_x)              (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: XfrAndBytIdx - This register is used when Transfer registers are accessed via indexed mode
    Bits:  9:2  XferIndex(rw) - XferIndex: Transfer Index Register
    Bits:  1:0  ByteIndex(rw) - ByteIndex - Byte Index Register
*/
#define NFP_ME_XfrAndBytIdx                                  0x000000f4
#define   NFP_ME_XfrAndBytIdx_XferIndex(_x)                  (((_x) & 0xff) << 2)
#define   NFP_ME_XfrAndBytIdx_XferIndex_of(_x)               (((_x) >> 2) & 0xff)
#define   NFP_ME_XfrAndBytIdx_ByteIndex(_x)                  (((_x) & 0x3) << 0)
#define   NFP_ME_XfrAndBytIdx_ByteIndex_of(_x)               (((_x) >> 0) & 0x3)
/*----------------------------------------------------------------
  Register: NxtNghbrSgl - Signal a Context in Next Neighbor
    Bits:  7    ThisContext(wo) - Controls whether or not the context field of this register is used in selecting the context that is signaled in the next neighbor microengine.
    Bits:  6:3  SignalNo(wo) - Signal to set in the next neighbor microengine.
    Bits:  2:0  Context(wo) - Context to signal in the next neighbor microengine. This field is only used if ThisContext is not asserted.
*/
#define NFP_ME_NxtNghbrSgl                                   0x00000100
#define   NFP_ME_NxtNghbrSgl_ThisContext                     BIT(7)
#define     NFP_ME_NxtNghbrSgl_ThisContext_USECTX            (0 << 7)
#define     NFP_ME_NxtNghbrSgl_ThisContext_SAMECTX           BIT(7)
#define   NFP_ME_NxtNghbrSgl_SignalNo(_x)                    (((_x) & 0xf) << 3)
#define   NFP_ME_NxtNghbrSgl_SignalNo_of(_x)                 (((_x) >> 3) & 0xf)
#define   NFP_ME_NxtNghbrSgl_Context(_x)                     (((_x) & 0x7) << 0)
#define   NFP_ME_NxtNghbrSgl_Context_of(_x)                  (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: PrvNghbrSgl - Signal a Context in Previous Neighbor
    Bits:  7    ThisContext(wo) - Controls whether or not the context field of this register is used in selecting the context that is signaled in the previous neighbor microengine.
    Bits:  6:3  SignalNo(wo) - Signal to set in the previous neighbor microengine.
    Bits:  2:0  Context(wo) - Context to signal in the previous neighbor microengine. This field is only used if ThisContext is not asserted.
*/
#define NFP_ME_PrvNghbrSgl                                   0x00000104
#define   NFP_ME_PrvNghbrSgl_ThisContext                     BIT(7)
#define     NFP_ME_PrvNghbrSgl_ThisContext_USECTX            (0 << 7)
#define     NFP_ME_PrvNghbrSgl_ThisContext_SAMECTX           BIT(7)
#define   NFP_ME_PrvNghbrSgl_SignalNo(_x)                    (((_x) & 0xf) << 3)
#define   NFP_ME_PrvNghbrSgl_SignalNo_of(_x)                 (((_x) >> 3) & 0xf)
#define   NFP_ME_PrvNghbrSgl_Context(_x)                     (((_x) & 0x7) << 0)
#define   NFP_ME_PrvNghbrSgl_Context_of(_x)                  (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: SameMESignal - Signal another Context in same Microengine.
    Bits:  7    NextContext(wo) - Controls whether or not the Context field of this register is used in selecting the context that is signaled in the microengine.
    Bits:  6:3  SignalNo(wo) - Signal to set in microengine.
    Bits:  2:0  Context(wo) - Context to signal in the microengine. This field is only used if NextContext field is not asserted.
*/
#define NFP_ME_SameMESignal                                  0x00000108
#define   NFP_ME_SameMESignal_NextContext                    BIT(7)
#define     NFP_ME_SameMESignal_NextContext_USECTX           (0 << 7)
#define     NFP_ME_SameMESignal_NextContext_NEXT             BIT(7)
#define   NFP_ME_SameMESignal_SignalNo(_x)                   (((_x) & 0xf) << 3)
#define   NFP_ME_SameMESignal_SignalNo_of(_x)                (((_x) >> 3) & 0xf)
#define   NFP_ME_SameMESignal_Context(_x)                    (((_x) & 0x7) << 0)
#define   NFP_ME_SameMESignal_Context_of(_x)                 (((_x) >> 0) & 0x7)
/*----------------------------------------------------------------
  Register: CRCRemainder - Result of the CRC operation after a crc instruction.
    Bits: 31:0  CRCRemainder(rw) - Input operand and result of the CRC instruction.
*/
#define NFP_ME_CRCRemainder                                  0x00000140
#define   NFP_ME_CRCRemainder_CRCRemainder(_x)               (((_x) & 0xffffffff) << 0)
#define   NFP_ME_CRCRemainder_CRCRemainder_of(_x)            (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: ProfileCnt - The profile count is used for code profiling and tuning
    Bits: 15:0  Count(rw) - Advances by one every cycle.
*/
#define NFP_ME_ProfileCnt                                    0x00000144
#define   NFP_ME_ProfileCnt_Count(_x)                        (((_x) & 0xffff) << 0)
#define   NFP_ME_ProfileCnt_Count_of(_x)                     (((_x) >> 0) & 0xffff)
/*----------------------------------------------------------------
  Register: PseudoRndNum - Random number generator
    Bits: 31:0  Number(rw) - Pseudo Random Number. This field must be non-zero to generate a pseudo random number.
*/
#define NFP_ME_PseudoRndNum                                  0x00000148
#define   NFP_ME_PseudoRndNum_Number(_x)                     (((_x) & 0xffffffff) << 0)
#define   NFP_ME_PseudoRndNum_Number_of(_x)                  (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: MiscControl - Miscellaneous Control Register
    Bits: 30    ReqOnCsEccError(rw) - This bit enables the ME to generate an external Event request (ATTN asserts) on CS ECC correctable error.
    Bits: 26    Interrupt(rw) - This bit supports the External Interrupt functionality.
    Bits: 25    ECCCorrGate(rw) - Correction on Control Store ECC errors. This bit is used to enable testing that the microengine responds correctly to hard ECC errors.
    Bits: 24    ParityEnable(rw) - Parity checking on register files.
    Bits: 23    ForceBadParity(rw) - Force incorrect parity value on register file when registers are written. This bit is used for manufacturing test and should be left as 0 for normal operation.
    Bits: 12    UstorECCCrctEn(rw) - UstoreECCCorrectEnable: Enables the microengine to correct correctable control store ECC errors. Note that ECC checking must be enabled in CtxEnables for this to have any meaning. Also note that when ECC error correcting is enabled, local CSR shadow rule is in effect. This means that any time a local CSR is changed by program control, it must not be read until the write-to-use latency is met. Specifically, software must not count on being able to user the value prior to the write in the write-to-use latency shadow. This applies to all local CSRs except LMAddr, XferIndex, NNPut, anf NNGet. For those registers the HW will guarantee that the old value can be used in the write-to-use latency shadow.
    Bits:  4    ThrdPrtyAdrMode(rw) - Selects between 32-bit and 40-bit addressing for the 3rd Party command instruction.
    Bits:  3    LegacyIndRef(rw) - Selects between Normal and Legacy Indirect Reference Modes.
    Bits:  2    ShareUstore(rw) - Enables Shared Control Store between the two MEs in an ME group.
    Bits:  1    LMRegionPull(rw) - Selects between the lower and upper halves of the 1K LM address space for CPP Pull transactions.
    Bits:  0    LMRegionPush(rw) - Selects between the lower and upper halves of the 1K LM address space for CPP Push transactions.
*/
#define NFP_ME_MiscControl                                   0x00000160
#define   NFP_ME_MiscControl_ReqOnCsEccError                 BIT(30)
#define     NFP_ME_MiscControl_ReqOnCsEccError_NO_REQUEST    (0 << 30)
#define     NFP_ME_MiscControl_ReqOnCsEccError_REQUEST       BIT(30)
#define   NFP_ME_MiscControl_Interrupt                       BIT(26)
#define     NFP_ME_MiscControl_Interrupt_NOT_INTERRUPTED     (0 << 26)
#define     NFP_ME_MiscControl_Interrupt_INTERRUPTED         BIT(26)
#define   NFP_ME_MiscControl_ECCCorrGate                     BIT(25)
#define     NFP_ME_MiscControl_ECCCorrGate_DISABLE           (0 << 25)
#define     NFP_ME_MiscControl_ECCCorrGate_ENABLE            BIT(25)
#define   NFP_ME_MiscControl_ParityEnable                    BIT(24)
#define     NFP_ME_MiscControl_ParityEnable_DISABLE          (0 << 24)
#define     NFP_ME_MiscControl_ParityEnable_ENABLE           BIT(24)
#define   NFP_ME_MiscControl_ForceBadParity                  BIT(23)
#define     NFP_ME_MiscControl_ForceBadParity_DISABLE        (0 << 23)
#define     NFP_ME_MiscControl_ForceBadParity_ENABLE         BIT(23)
#define   NFP_ME_MiscControl_UstorECCCrctEn                  BIT(12)
#define     NFP_ME_MiscControl_UstorECCCrctEn_DISABLE        (0 << 12)
#define     NFP_ME_MiscControl_UstorECCCrctEn_ENABLE         BIT(12)
#define   NFP_ME_MiscControl_ThrdPrtyAdrMode                 BIT(4)
#define     NFP_ME_MiscControl_ThrdPrtyAdrMode_40BIT         (0 << 4)
#define     NFP_ME_MiscControl_ThrdPrtyAdrMode_32BIT         BIT(4)
#define   NFP_ME_MiscControl_LegacyIndRef                    BIT(3)
#define     NFP_ME_MiscControl_LegacyIndRef_NORMAL           (0 << 3)
#define     NFP_ME_MiscControl_LegacyIndRef_LEGACY           BIT(3)
#define   NFP_ME_MiscControl_ShareUstore                     BIT(2)
#define     NFP_ME_MiscControl_ShareUstore_NOT_SHARED        (0 << 2)
#define     NFP_ME_MiscControl_ShareUstore_SHARED            BIT(2)
#define   NFP_ME_MiscControl_LMRegionPull                    BIT(1)
#define     NFP_ME_MiscControl_LMRegionPull_LOWER_HALF       (0 << 1)
#define     NFP_ME_MiscControl_LMRegionPull_UPPER_HALF       BIT(1)
#define   NFP_ME_MiscControl_LMRegionPush                    BIT(0)
#define     NFP_ME_MiscControl_LMRegionPush_LOWER_HALF       (0 << 0)
#define     NFP_ME_MiscControl_LMRegionPush_UPPER_HALF       BIT(0)
/*----------------------------------------------------------------
  Register: PcBreakpoint0Mask - Mask register associated with PC Breakpoint 0
  Register: PcBreakpoint1Mask - Mask register associated with PC Breakpoint 1
    Bits: 13:0  PcMask(rw) - Masks the PC (program counter) bits which should not be considered for comparison against the value in the PcBrkpnt field of the PcBreakpoint CSR.
*/
#define NFP_ME_PcBreakpoint0Mask                             0x00000164
#define NFP_ME_PcBreakpoint1Mask                             0x00000168
#define   NFP_ME_PcBreakpointMask_PcMask(_x)                 (((_x) & 0x3fff) << 0)
#define   NFP_ME_PcBreakpointMask_PcMask_of(_x)              (((_x) >> 0) & 0x3fff)
/*----------------------------------------------------------------
  Register: Mailbox0 - Mailbox Register 0
  Register: Mailbox1 - Mailbox Register 1
  Register: Mailbox2 - Mailbox Register 2
  Register: Mailbox3 - Mailbox Register 3
    Bits: 31:0  Mailbox(rw) - Mailbox Data value defined by the application.
*/
#define NFP_ME_Mailbox0                                      0x00000170
#define NFP_ME_Mailbox1                                      0x00000174
#define NFP_ME_Mailbox2                                      0x00000178
#define NFP_ME_Mailbox3                                      0x0000017c
#define   NFP_ME_Mailbox_Mailbox(_x)                         (((_x) & 0xffffffff) << 0)
#define   NFP_ME_Mailbox_Mailbox_of(_x)                      (((_x) >> 0) & 0xffffffff)
/*----------------------------------------------------------------
  Register: CmdIndirectRef0 - Command Indirect Reference Register Type 0
    Bits: 29:24 Island(rw) - Override value for the CPP command's island field.
    Bits: 23:20 Master(rw) - Various overriding definitions depending on the value of the OVE_MASTER field in the previous ALU.
    Bits: 19:16 SignalMaster(rw) - Override value for the CPP command's signal master field.
    Bits: 15:13 SignalCtx(rw) - Override value for the CPP command's signal context field.
    Bits: 12:9  SignalNum(rw) - Override value for the CPP command's signal number field.
    Bits:  7:0  ByteMask(rw) - Override value for the CPP command's byte mask field.
*/
#define NFP_ME_CmdIndirectRef0                               0x00000190
#define   NFP_ME_CmdIndirectRef0_Island(_x)                  (((_x) & 0x3f) << 24)
#define   NFP_ME_CmdIndirectRef0_Island_of(_x)               (((_x) >> 24) & 0x3f)
#define   NFP_ME_CmdIndirectRef0_Master(_x)                  (((_x) & 0xf) << 20)
#define   NFP_ME_CmdIndirectRef0_Master_of(_x)               (((_x) >> 20) & 0xf)
#define   NFP_ME_CmdIndirectRef0_SignalMaster(_x)            (((_x) & 0xf) << 16)
#define   NFP_ME_CmdIndirectRef0_SignalMaster_of(_x)         (((_x) >> 16) & 0xf)
#define   NFP_ME_CmdIndirectRef0_SignalCtx(_x)               (((_x) & 0x7) << 13)
#define   NFP_ME_CmdIndirectRef0_SignalCtx_of(_x)            (((_x) >> 13) & 0x7)
#define   NFP_ME_CmdIndirectRef0_SignalNum(_x)               (((_x) & 0xf) << 9)
#define   NFP_ME_CmdIndirectRef0_SignalNum_of(_x)            (((_x) >> 9) & 0xf)
#define   NFP_ME_CmdIndirectRef0_ByteMask(_x)                (((_x) & 0xff) << 0)
#define   NFP_ME_CmdIndirectRef0_ByteMask_of(_x)             (((_x) >> 0) & 0xff)

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_ME_H */
