/*
 * Copyright (C) 2013, Netronome Systems, Inc.  All rights reserved.
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef NFP6000_UART_H
#define NFP6000_UART_H


/* HGID: 60515d2e075a */

#ifndef NFP6000_LONGNAMES

/*----------------------------------------------------------------
  Register: UartDataReg_DivisorLow - Control the baud rate (only if DivAccess bit set)
    Bits:  7:0  DivLow(rw) - Least significant eight bits of the baud rate divisor
*/
#define NFP_UART_UartDataReg_DIVISORLOW                      0x00000000
#define   NFP_UART_UARTDATAREG_DIVISORLOW_DIVLOW(_x)         (((_x) & 0xff) << 0)
#define   NFP_UART_UARTDATAREG_DIVISORLOW_DIVLOW_of(_x)      (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: UartDataReg_TxHolding - Hold the transmit data (only if DivAccess bit clear)
    Bits:  7:0  TxByte(wo) - Data byte to be transmitted or placed in the transmit FIFO
*/
#define NFP_UART_UartDataReg_TXHOLDING                       0x00000000
#define   NFP_UART_UARTDATAREG_TXHOLDING_TXBYTE(_x)          (((_x) & 0xff) << 0)
#define   NFP_UART_UARTDATAREG_TXHOLDING_TXBYTE_of(_x)       (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: UartDataReg_RxBuffer - Buffer for the receive data (only if DivAccess bit clear)
    Bits:  7:0  RxByte(ro) - Data byte received from the Rx FIFO
*/
#define NFP_UART_UartDataReg_RXBUFFER                        0x00000000
#define   NFP_UART_UARTDATAREG_RXBUFFER_RXBYTE(_x)           (((_x) & 0xff) << 0)
#define   NFP_UART_UARTDATAREG_RXBUFFER_RXBYTE_of(_x)        (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: UartIntEnDivHi_UartInterruptEnable - Interrupt enable
    Bits:  6    Enable(rw) - UART Unit Enable
    Bits:  5    NRZEn(rw) - NRZ coding Enable
    Bits:  4    RXTimeout(rw) - Receiver time out Interrupt Enable
    Bits:  2    LineStatus(rw) - Receiver Line Status Interrupt Enable
    Bits:  1    TxDataReq(rw) - Transmit Data request Interrupt Enable
    Bits:  0    RxDataValid(rw) - Receiver Data Available Interrupt Enable
*/
#define NFP_UART_UartIntEnDivHi_UARTINTERRUPTENABLE          0x00000004
#define   NFP_UART_UARTINTENDIVHI_UARTINTERRUPTENABLE_ENABLE BIT(6)
#define   NFP_UART_UARTINTENDIVHI_UARTINTERRUPTENABLE_NRZEN  BIT(5)
#define   NFP_UART_UARTINTENDIVHI_UARTINTERRUPTENABLE_RXTIMEOUT BIT(4)
#define   NFP_UART_UARTINTENDIVHI_UARTINTERRUPTENABLE_LINESTATUS BIT(2)
#define   NFP_UART_UARTINTENDIVHI_UARTINTERRUPTENABLE_TXDATAREQ BIT(1)
#define   NFP_UART_UARTINTENDIVHI_UARTINTERRUPTENABLE_RXDATAVALID BIT(0)
/*----------------------------------------------------------------
  Register: UartIntEnDivHi_DivisorHigh - Control the baud rate (only if DivAccess bit set)
    Bits:  7:0  DivHigh(rw) - Most significant eight bits of the baud rate divisor
*/
#define NFP_UART_UartIntEnDivHi_DIVISORHIGH                  0x00000004
#define   NFP_UART_UARTINTENDIVHI_DIVISORHIGH_DIVHIGH(_x)    (((_x) & 0xff) << 0)
#define   NFP_UART_UARTINTENDIVHI_DIVISORHIGH_DIVHIGH_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: UartFifoCtrlIntSts_FifoControl - Controls the FIFO
    Bits:  7:6  IntLevel(wo) - Interrupt trigger level. When the number of bytes in the receiver FIFO equals the interrupt trigger level programmed into this field and the Received Data Available Interrupt is enabled (via UART_IER), an interrupt is generated and appropriate bits are set in the UART_IIR.
    Bits:  2    TxFifoReset(wo) - Write a 1 to reset the transmitter FIFO; any byte currently being transmitted will complete
    Bits:  1    RxFifoReset(wo) - Write a 1 to reset the receive FIFO; any byte being received will be added to the FIFO when it completes
    Bits:  0    FifoEn(wo) - Transmit and Receive FIFO enable; write 1 to enable the FIFOs, and 0 to disable them. This bit must be set for other bits to have any effect in the write. Writing zero clears both transmit and receive FIFOs.
*/
#define NFP_UART_UartFifoCtrlIntSts_FIFOCONTROL              0x00000008
#define   NFP_UART_UARTFIFOCTRLINTSTS_FIFOCONTROL_INTLEVEL(_x) (((_x) & 0x3) << 6)
#define   NFP_UART_UARTFIFOCTRLINTSTS_FIFOCONTROL_INTLEVEL_of(_x) (((_x) >> 6) & 0x3)
#define     NFP_UART_UARTFIFOCTRLINTSTS_FIFOCONTROL_INTLEVEL_B1 (0)
#define     NFP_UART_UARTFIFOCTRLINTSTS_FIFOCONTROL_INTLEVEL_B8 (1)
#define     NFP_UART_UARTFIFOCTRLINTSTS_FIFOCONTROL_INTLEVEL_B16 (2)
#define     NFP_UART_UARTFIFOCTRLINTSTS_FIFOCONTROL_INTLEVEL_B32 (3)
#define   NFP_UART_UARTFIFOCTRLINTSTS_FIFOCONTROL_TXFIFORESET BIT(2)
#define   NFP_UART_UARTFIFOCTRLINTSTS_FIFOCONTROL_RXFIFORESET BIT(1)
#define   NFP_UART_UARTFIFOCTRLINTSTS_FIFOCONTROL_FIFOEN     BIT(0)
/*----------------------------------------------------------------
  Register: UartFifoCtrlIntSts_InterruptStatus - Interrupt Identification
    Bits:  7:6  FifoMode(ro) - FIFO Mode Enable Status
    Bits:  3    Timeout(ro) - Time Out Detected
    Bits:  2:1  Source(ro) - Encoded interrupt source
    Bits:  0    Pending(ro) - Clear if interrupt pending
*/
#define NFP_UART_UartFifoCtrlIntSts_INTERRUPTSTATUS          0x00000008
#define   NFP_UART_UARTFIFOCTRLINTSTS_INTERRUPTSTATUS_FIFOMODE(_x) (((_x) & 0x3) << 6)
#define   NFP_UART_UARTFIFOCTRLINTSTS_INTERRUPTSTATUS_FIFOMODE_of(_x) (((_x) >> 6) & 0x3)
#define     NFP_UART_UARTFIFOCTRLINTSTS_INTERRUPTSTATUS_FIFOMODE_NFS (0)
#define     NFP_UART_UARTFIFOCTRLINTSTS_INTERRUPTSTATUS_FIFOMODE_RSVD1 (1)
#define     NFP_UART_UARTFIFOCTRLINTSTS_INTERRUPTSTATUS_FIFOMODE_RSVD2 (2)
#define     NFP_UART_UARTFIFOCTRLINTSTS_INTERRUPTSTATUS_FIFOMODE_FS (3)
#define   NFP_UART_UARTFIFOCTRLINTSTS_INTERRUPTSTATUS_TIMEOUT BIT(3)
#define   NFP_UART_UARTFIFOCTRLINTSTS_INTERRUPTSTATUS_SOURCE(_x) (((_x) & 0x3) << 1)
#define   NFP_UART_UARTFIFOCTRLINTSTS_INTERRUPTSTATUS_SOURCE_of(_x) (((_x) >> 1) & 0x3)
#define   NFP_UART_UARTFIFOCTRLINTSTS_INTERRUPTSTATUS_PENDING BIT(0)
/*----------------------------------------------------------------
  Register: LineControl - Control the transmission line data format
    Bits:  7    DivAccess(rw) - Divisor register access - set to permit access to the baud rate divisor
    Bits:  6    SetBreak(rw) - Set Break
    Bits:  5    StickyPar(rw) - Sticky Parity
    Bits:  4    EvenPar(rw) - Even Parity Select
    Bits:  3    ParityEn(rw) - Parity Enable
    Bits:  2    StopBits(rw) - Stop Bits
    Bits:  1:0  DataLen(rw) - Word Length Select
*/
#define NFP_UART_LINECONTROL                                 0x0000000c
#define   NFP_UART_LINECONTROL_DIVACCESS                     BIT(7)
#define   NFP_UART_LINECONTROL_SETBREAK                      BIT(6)
#define   NFP_UART_LINECONTROL_STICKYPAR                     BIT(5)
#define   NFP_UART_LINECONTROL_EVENPAR                       BIT(4)
#define   NFP_UART_LINECONTROL_PARITYEN                      BIT(3)
#define   NFP_UART_LINECONTROL_STOPBITS                      BIT(2)
#define     NFP_UART_LINECONTROL_STOPBITS_S1                 (0 << 2)
#define     NFP_UART_LINECONTROL_STOPBITS_S2                 BIT(2)
#define   NFP_UART_LINECONTROL_DATALEN(_x)                   (((_x) & 0x3) << 0)
#define   NFP_UART_LINECONTROL_DATALEN_of(_x)                (((_x) >> 0) & 0x3)
#define     NFP_UART_LINECONTROL_DATALEN_B5                  (0)
#define     NFP_UART_LINECONTROL_DATALEN_B6                  (1)
#define     NFP_UART_LINECONTROL_DATALEN_B7                  (2)
#define     NFP_UART_LINECONTROL_DATALEN_B8                  (3)
/*----------------------------------------------------------------
  Register: LineStatus - Stores the status of the previous transaction
    Bits:  7    FifoError(ro) - Fifo Error Status
    Bits:  6    TxEmpty(ro) - Transmitter Empty
    Bits:  5    TxDataReq(ro) - Transmitter Data Request
    Bits:  4    BrkInt(ro) - Break Interrupt
    Bits:  3    FrameErr(ro) - Framing Error
    Bits:  2    ParErr(ro) - Parity Error
    Bits:  1    OverrunErr(ro) - Overrun Error
    Bits:  0    DataRdy(ro) - Data Ready
*/
#define NFP_UART_LINESTATUS                                  0x00000014
#define   NFP_UART_LINESTATUS_FIFOERROR                      BIT(7)
#define   NFP_UART_LINESTATUS_TXEMPTY                        BIT(6)
#define   NFP_UART_LINESTATUS_TXDATAREQ                      BIT(5)
#define   NFP_UART_LINESTATUS_BRKINT                         BIT(4)
#define   NFP_UART_LINESTATUS_FRAMEERR                       BIT(3)
#define   NFP_UART_LINESTATUS_PARERR                         BIT(2)
#define   NFP_UART_LINESTATUS_OVERRUNERR                     BIT(1)
#define   NFP_UART_LINESTATUS_DATARDY                        BIT(0)

#else /* NFP6000_LONGNAMES */

/*----------------------------------------------------------------
  Register: UartDataReg_DivisorLow - Control the baud rate (only if DivAccess bit set)
    Bits:  7:0  DivLow(rw) - Least significant eight bits of the baud rate divisor
*/
#define NFP_UART_UartDataReg_DivisorLow                      0x00000000
#define   NFP_UART_UartDataReg_DivisorLow_DivLow(_x)         (((_x) & 0xff) << 0)
#define   NFP_UART_UartDataReg_DivisorLow_DivLow_of(_x)      (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: UartDataReg_TxHolding - Hold the transmit data (only if DivAccess bit clear)
    Bits:  7:0  TxByte(wo) - Data byte to be transmitted or placed in the transmit FIFO
*/
#define NFP_UART_UartDataReg_TxHolding                       0x00000000
#define   NFP_UART_UartDataReg_TxHolding_TxByte(_x)          (((_x) & 0xff) << 0)
#define   NFP_UART_UartDataReg_TxHolding_TxByte_of(_x)       (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: UartDataReg_RxBuffer - Buffer for the receive data (only if DivAccess bit clear)
    Bits:  7:0  RxByte(ro) - Data byte received from the Rx FIFO
*/
#define NFP_UART_UartDataReg_RxBuffer                        0x00000000
#define   NFP_UART_UartDataReg_RxBuffer_RxByte(_x)           (((_x) & 0xff) << 0)
#define   NFP_UART_UartDataReg_RxBuffer_RxByte_of(_x)        (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: UartIntEnDivHi_UartInterruptEnable - Interrupt enable
    Bits:  6    Enable(rw) - UART Unit Enable
    Bits:  5    NRZEn(rw) - NRZ coding Enable
    Bits:  4    RXTimeout(rw) - Receiver time out Interrupt Enable
    Bits:  2    LineStatus(rw) - Receiver Line Status Interrupt Enable
    Bits:  1    TxDataReq(rw) - Transmit Data request Interrupt Enable
    Bits:  0    RxDataValid(rw) - Receiver Data Available Interrupt Enable
*/
#define NFP_UART_UartIntEnDivHi_UartInterruptEnable          0x00000004
#define   NFP_UART_UartIntEnDivHi_UartInterruptEnable_Enable BIT(6)
#define   NFP_UART_UartIntEnDivHi_UartInterruptEnable_NRZEn  BIT(5)
#define   NFP_UART_UartIntEnDivHi_UartInterruptEnable_RXTimeout BIT(4)
#define   NFP_UART_UartIntEnDivHi_UartInterruptEnable_LineStatus BIT(2)
#define   NFP_UART_UartIntEnDivHi_UartInterruptEnable_TxDataReq BIT(1)
#define   NFP_UART_UartIntEnDivHi_UartInterruptEnable_RxDataValid BIT(0)
/*----------------------------------------------------------------
  Register: UartIntEnDivHi_DivisorHigh - Control the baud rate (only if DivAccess bit set)
    Bits:  7:0  DivHigh(rw) - Most significant eight bits of the baud rate divisor
*/
#define NFP_UART_UartIntEnDivHi_DivisorHigh                  0x00000004
#define   NFP_UART_UartIntEnDivHi_DivisorHigh_DivHigh(_x)    (((_x) & 0xff) << 0)
#define   NFP_UART_UartIntEnDivHi_DivisorHigh_DivHigh_of(_x) (((_x) >> 0) & 0xff)
/*----------------------------------------------------------------
  Register: UartFifoCtrlIntSts_FifoControl - Controls the FIFO
    Bits:  7:6  IntLevel(wo) - Interrupt trigger level. When the number of bytes in the receiver FIFO equals the interrupt trigger level programmed into this field and the Received Data Available Interrupt is enabled (via UART_IER), an interrupt is generated and appropriate bits are set in the UART_IIR.
    Bits:  2    TxFifoReset(wo) - Write a 1 to reset the transmitter FIFO; any byte currently being transmitted will complete
    Bits:  1    RxFifoReset(wo) - Write a 1 to reset the receive FIFO; any byte being received will be added to the FIFO when it completes
    Bits:  0    FifoEn(wo) - Transmit and Receive FIFO enable; write 1 to enable the FIFOs, and 0 to disable them. This bit must be set for other bits to have any effect in the write. Writing zero clears both transmit and receive FIFOs.
*/
#define NFP_UART_UartFifoCtrlIntSts_FifoControl              0x00000008
#define   NFP_UART_UartFifoCtrlIntSts_FifoControl_IntLevel(_x) (((_x) & 0x3) << 6)
#define   NFP_UART_UartFifoCtrlIntSts_FifoControl_IntLevel_of(_x) (((_x) >> 6) & 0x3)
#define     NFP_UART_UartFifoCtrlIntSts_FifoControl_IntLevel_B1 (0)
#define     NFP_UART_UartFifoCtrlIntSts_FifoControl_IntLevel_B8 (1)
#define     NFP_UART_UartFifoCtrlIntSts_FifoControl_IntLevel_B16 (2)
#define     NFP_UART_UartFifoCtrlIntSts_FifoControl_IntLevel_B32 (3)
#define   NFP_UART_UartFifoCtrlIntSts_FifoControl_TxFifoReset BIT(2)
#define   NFP_UART_UartFifoCtrlIntSts_FifoControl_RxFifoReset BIT(1)
#define   NFP_UART_UartFifoCtrlIntSts_FifoControl_FifoEn     BIT(0)
/*----------------------------------------------------------------
  Register: UartFifoCtrlIntSts_InterruptStatus - Interrupt Identification
    Bits:  7:6  FifoMode(ro) - FIFO Mode Enable Status
    Bits:  3    Timeout(ro) - Time Out Detected
    Bits:  2:1  Source(ro) - Encoded interrupt source
    Bits:  0    Pending(ro) - Clear if interrupt pending
*/
#define NFP_UART_UartFifoCtrlIntSts_InterruptStatus          0x00000008
#define   NFP_UART_UartFifoCtrlIntSts_InterruptStatus_FifoMode(_x) (((_x) & 0x3) << 6)
#define   NFP_UART_UartFifoCtrlIntSts_InterruptStatus_FifoMode_of(_x) (((_x) >> 6) & 0x3)
#define     NFP_UART_UartFifoCtrlIntSts_InterruptStatus_FifoMode_NFS (0)
#define     NFP_UART_UartFifoCtrlIntSts_InterruptStatus_FifoMode_RSVD1 (1)
#define     NFP_UART_UartFifoCtrlIntSts_InterruptStatus_FifoMode_RSVD2 (2)
#define     NFP_UART_UartFifoCtrlIntSts_InterruptStatus_FifoMode_FS (3)
#define   NFP_UART_UartFifoCtrlIntSts_InterruptStatus_Timeout BIT(3)
#define   NFP_UART_UartFifoCtrlIntSts_InterruptStatus_Source(_x) (((_x) & 0x3) << 1)
#define   NFP_UART_UartFifoCtrlIntSts_InterruptStatus_Source_of(_x) (((_x) >> 1) & 0x3)
#define   NFP_UART_UartFifoCtrlIntSts_InterruptStatus_Pending BIT(0)
/*----------------------------------------------------------------
  Register: LineControl - Control the transmission line data format
    Bits:  7    DivAccess(rw) - Divisor register access - set to permit access to the baud rate divisor
    Bits:  6    SetBreak(rw) - Set Break
    Bits:  5    StickyPar(rw) - Sticky Parity
    Bits:  4    EvenPar(rw) - Even Parity Select
    Bits:  3    ParityEn(rw) - Parity Enable
    Bits:  2    StopBits(rw) - Stop Bits
    Bits:  1:0  DataLen(rw) - Word Length Select
*/
#define NFP_UART_LineControl                                 0x0000000c
#define   NFP_UART_LineControl_DivAccess                     BIT(7)
#define   NFP_UART_LineControl_SetBreak                      BIT(6)
#define   NFP_UART_LineControl_StickyPar                     BIT(5)
#define   NFP_UART_LineControl_EvenPar                       BIT(4)
#define   NFP_UART_LineControl_ParityEn                      BIT(3)
#define   NFP_UART_LineControl_StopBits                      BIT(2)
#define     NFP_UART_LineControl_StopBits_S1                 (0 << 2)
#define     NFP_UART_LineControl_StopBits_S2                 BIT(2)
#define   NFP_UART_LineControl_DataLen(_x)                   (((_x) & 0x3) << 0)
#define   NFP_UART_LineControl_DataLen_of(_x)                (((_x) >> 0) & 0x3)
#define     NFP_UART_LineControl_DataLen_B5                  (0)
#define     NFP_UART_LineControl_DataLen_B6                  (1)
#define     NFP_UART_LineControl_DataLen_B7                  (2)
#define     NFP_UART_LineControl_DataLen_B8                  (3)
/*----------------------------------------------------------------
  Register: LineStatus - Stores the status of the previous transaction
    Bits:  7    FifoError(ro) - Fifo Error Status
    Bits:  6    TxEmpty(ro) - Transmitter Empty
    Bits:  5    TxDataReq(ro) - Transmitter Data Request
    Bits:  4    BrkInt(ro) - Break Interrupt
    Bits:  3    FrameErr(ro) - Framing Error
    Bits:  2    ParErr(ro) - Parity Error
    Bits:  1    OverrunErr(ro) - Overrun Error
    Bits:  0    DataRdy(ro) - Data Ready
*/
#define NFP_UART_LineStatus                                  0x00000014
#define   NFP_UART_LineStatus_FifoError                      BIT(7)
#define   NFP_UART_LineStatus_TxEmpty                        BIT(6)
#define   NFP_UART_LineStatus_TxDataReq                      BIT(5)
#define   NFP_UART_LineStatus_BrkInt                         BIT(4)
#define   NFP_UART_LineStatus_FrameErr                       BIT(3)
#define   NFP_UART_LineStatus_ParErr                         BIT(2)
#define   NFP_UART_LineStatus_OverrunErr                     BIT(1)
#define   NFP_UART_LineStatus_DataRdy                        BIT(0)

#endif /* NFP6000_LONGNAMES */


#endif /* NFP6000_UART_H */
