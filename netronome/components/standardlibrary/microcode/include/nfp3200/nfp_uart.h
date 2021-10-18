/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_uart.h
 * @brief       Register definitions for UART
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_UART_H__
#define __NFP_SDK__NFP3200_NFP_UART_H__

/* HGID: nfp3200/uart.desc = c05f20d390a0 */
/* Register Type: UartTxHolding */
#define NFP_UART_THR                   0x0000
#define   NFP_UART_THR_BYTE(_x)                         ((_x) & 0xff)
#define   NFP_UART_THR_BYTE_mask                        (0xff)
#define   NFP_UART_THR_BYTE_shift                       (0)
/* Register Type: UartRxBuffer */
#define NFP_UART_RBR                   0x0000
#define   NFP_UART_RBR_BYTE_bf                          0, 7, 0
#define   NFP_UART_RBR_BYTE_mask                        (0xff)
#define   NFP_UART_RBR_BYTE_shift                       (0)
/* Register Type: UartDivisorLow */
#define NFP_UART_DLL                   0x0000
#define   NFP_UART_DLL_LB(_x)                           ((_x) & 0xff)
#define   NFP_UART_DLL_LB_bf                            0, 7, 0
#define   NFP_UART_DLL_LB_mask                          (0xff)
#define   NFP_UART_DLL_LB_shift                         (0)
/* Register Type: UartInterruptEnable */
#define NFP_UART_IER                   0x0004
#define   NFP_UART_IER_UNIT                             (0x1 << 6)
#define   NFP_UART_IER_UNIT_bf                          0, 6, 6
#define   NFP_UART_IER_UNIT_mask                        (0x1)
#define   NFP_UART_IER_UNIT_bit                         (6)
#define   NFP_UART_IER_NRZ                              (0x1 << 5)
#define   NFP_UART_IER_NRZ_bf                           0, 5, 5
#define   NFP_UART_IER_NRZ_mask                         (0x1)
#define   NFP_UART_IER_NRZ_bit                          (5)
#define   NFP_UART_IER_RX_TIMEOUT                       (0x1 << 4)
#define   NFP_UART_IER_RX_TIMEOUT_bf                    0, 4, 4
#define   NFP_UART_IER_RX_TIMEOUT_mask                  (0x1)
#define   NFP_UART_IER_RX_TIMEOUT_bit                   (4)
#define   NFP_UART_IER_RLSI                             (0x1 << 2)
#define   NFP_UART_IER_RLSI_bf                          0, 2, 2
#define   NFP_UART_IER_RLSI_mask                        (0x1)
#define   NFP_UART_IER_RLSI_bit                         (2)
#define   NFP_UART_IER_THRI                             (0x1 << 1)
#define   NFP_UART_IER_THRI_bf                          0, 1, 1
#define   NFP_UART_IER_THRI_mask                        (0x1)
#define   NFP_UART_IER_THRI_bit                         (1)
#define   NFP_UART_IER_RDI                              (0x1)
#define   NFP_UART_IER_RDI_bf                           0, 0, 0
#define   NFP_UART_IER_RDI_mask                         (0x1)
#define   NFP_UART_IER_RDI_bit                          (0)
/* Register Type: UartDivisorHigh */
#define NFP_UART_DLM                   0x0004
#define   NFP_UART_DLM_HB(_x)                           ((_x) & 0xff)
#define   NFP_UART_DLM_HB_bf                            0, 7, 0
#define   NFP_UART_DLM_HB_mask                          (0xff)
#define   NFP_UART_DLM_HB_shift                         (0)
/* Register Type: UartInterruptStatus */
#define NFP_UART_ISR                   0x0008
#define   NFP_UART_ISR_FIFO_MOD_EN_bf                   0, 7, 6
#define   NFP_UART_ISR_FIFO_MOD_EN_mask                 (0x3)
#define   NFP_UART_ISR_FIFO_MOD_EN_shift                (6)
#define     NFP_UART_ISR_FIFO_MOD_EN_NON_FIFO           (0)
#define     NFP_UART_ISR_FIFO_MOD_EN_FIFO               (3)
#define   NFP_UART_ISR_TIMEOUT                          (0x1 << 3)
#define   NFP_UART_ISR_TIMEOUT_bf                       0, 3, 3
#define   NFP_UART_ISR_TIMEOUT_mask                     (0x1)
#define   NFP_UART_ISR_TIMEOUT_bit                      (3)
#define   NFP_UART_ISR_SOURCE_bf                        0, 2, 1
#define   NFP_UART_ISR_SOURCE_mask                      (0x3)
#define   NFP_UART_ISR_SOURCE_shift                     (1)
#define     NFP_UART_ISR_SOURCE_NOT_USED                (0)
#define     NFP_UART_ISR_SOURCE_THRI                    (1)
#define     NFP_UART_ISR_SOURCE_RDI                     (2)
#define     NFP_UART_ISR_SOURCE_RLSI                    (3)
#define   NFP_UART_ISR_NO_INT                           (0x1)
#define   NFP_UART_ISR_NO_INT_bf                        0, 0, 0
#define   NFP_UART_ISR_NO_INT_mask                      (0x1)
#define   NFP_UART_ISR_NO_INT_bit                       (0)
/* Register Type: UartFifoControl */
#define NFP_UART_FCR                   0x0008
#define   NFP_UART_FCR_R_TRIGGER(_x)                    (((_x) & 0x3) << 6)
#define   NFP_UART_FCR_R_TRIGGER_mask                   (0x3)
#define   NFP_UART_FCR_R_TRIGGER_shift                  (6)
#define     NFP_UART_FCR_R_TRIGGER_1                    (0)
#define     NFP_UART_FCR_R_TRIGGER_8                    (1)
#define     NFP_UART_FCR_R_TRIGGER_16                   (2)
#define     NFP_UART_FCR_R_TRIGGER_32                   (3)
#define   NFP_UART_FCR_CLEAR_XMIT                       (0x1 << 2)
#define   NFP_UART_FCR_CLEAR_XMIT_bf                    0, 2, 2
#define   NFP_UART_FCR_CLEAR_XMIT_mask                  (0x1)
#define   NFP_UART_FCR_CLEAR_XMIT_bit                   (2)
#define   NFP_UART_FCR_CLEAR_RCVR                       (0x1 << 1)
#define   NFP_UART_FCR_CLEAR_RCVR_bf                    0, 1, 1
#define   NFP_UART_FCR_CLEAR_RCVR_mask                  (0x1)
#define   NFP_UART_FCR_CLEAR_RCVR_bit                   (1)
#define   NFP_UART_FCR_ENABLE_FIFO                      (0x1)
#define   NFP_UART_FCR_ENABLE_FIFO_bf                   0, 0, 0
#define   NFP_UART_FCR_ENABLE_FIFO_mask                 (0x1)
#define   NFP_UART_FCR_ENABLE_FIFO_bit                  (0)
/* Register Type: UartLineControl */
#define NFP_UART_LCR                   0x000c
#define   NFP_UART_LCR_DLAB                             (0x1 << 7)
#define   NFP_UART_LCR_DLAB_bf                          0, 7, 7
#define   NFP_UART_LCR_DLAB_mask                        (0x1)
#define   NFP_UART_LCR_DLAB_bit                         (7)
#define   NFP_UART_LCR_SBC                              (0x1 << 6)
#define   NFP_UART_LCR_SBC_bf                           0, 6, 6
#define   NFP_UART_LCR_SBC_mask                         (0x1)
#define   NFP_UART_LCR_SBC_bit                          (6)
#define   NFP_UART_LCR_SPAR                             (0x1 << 5)
#define   NFP_UART_LCR_SPAR_bf                          0, 5, 5
#define   NFP_UART_LCR_SPAR_mask                        (0x1)
#define   NFP_UART_LCR_SPAR_bit                         (5)
#define   NFP_UART_LCR_EPAR                             (0x1 << 4)
#define   NFP_UART_LCR_EPAR_bf                          0, 4, 4
#define   NFP_UART_LCR_EPAR_mask                        (0x1)
#define   NFP_UART_LCR_EPAR_bit                         (4)
#define   NFP_UART_LCR_PARITY                           (0x1 << 3)
#define   NFP_UART_LCR_PARITY_bf                        0, 3, 3
#define   NFP_UART_LCR_PARITY_mask                      (0x1)
#define   NFP_UART_LCR_PARITY_bit                       (3)
#define   NFP_UART_LCR_STOP                             (0x1 << 2)
#define   NFP_UART_LCR_STOP_bf                          0, 2, 2
#define   NFP_UART_LCR_STOP_mask                        (0x1)
#define   NFP_UART_LCR_STOP_bit                         (2)
#define     NFP_UART_LCR_STOP_1                         (0x0)
#define     NFP_UART_LCR_STOP_2                         (0x4)
#define   NFP_UART_LCR_WLEN(_x)                         ((_x) & 0x3)
#define   NFP_UART_LCR_WLEN_bf                          0, 1, 0
#define   NFP_UART_LCR_WLEN_mask                        (0x3)
#define   NFP_UART_LCR_WLEN_shift                       (0)
#define     NFP_UART_LCR_WLEN_5                         (0)
#define     NFP_UART_LCR_WLEN_6                         (1)
#define     NFP_UART_LCR_WLEN_7                         (2)
#define     NFP_UART_LCR_WLEN_8                         (3)
/* Register Type: UartLineStatus */
#define NFP_UART_LSR                   0x0014
#define   NFP_UART_LSR_FIFO_ERR                         (0x1 << 7)
#define   NFP_UART_LSR_FIFO_ERR_bf                      0, 7, 7
#define   NFP_UART_LSR_FIFO_ERR_mask                    (0x1)
#define   NFP_UART_LSR_FIFO_ERR_bit                     (7)
#define   NFP_UART_LSR_TEMT                             (0x1 << 6)
#define   NFP_UART_LSR_TEMT_bf                          0, 6, 6
#define   NFP_UART_LSR_TEMT_mask                        (0x1)
#define   NFP_UART_LSR_TEMT_bit                         (6)
#define   NFP_UART_LSR_THRE                             (0x1 << 5)
#define   NFP_UART_LSR_THRE_bf                          0, 5, 5
#define   NFP_UART_LSR_THRE_mask                        (0x1)
#define   NFP_UART_LSR_THRE_bit                         (5)
#define   NFP_UART_LSR_BI                               (0x1 << 4)
#define   NFP_UART_LSR_BI_bf                            0, 4, 4
#define   NFP_UART_LSR_BI_mask                          (0x1)
#define   NFP_UART_LSR_BI_bit                           (4)
#define   NFP_UART_LSR_FE                               (0x1 << 3)
#define   NFP_UART_LSR_FE_bf                            0, 3, 3
#define   NFP_UART_LSR_FE_mask                          (0x1)
#define   NFP_UART_LSR_FE_bit                           (3)
#define   NFP_UART_LSR_PE                               (0x1 << 2)
#define   NFP_UART_LSR_PE_bf                            0, 2, 2
#define   NFP_UART_LSR_PE_mask                          (0x1)
#define   NFP_UART_LSR_PE_bit                           (2)
#define   NFP_UART_LSR_OE                               (0x1 << 1)
#define   NFP_UART_LSR_OE_bf                            0, 1, 1
#define   NFP_UART_LSR_OE_mask                          (0x1)
#define   NFP_UART_LSR_OE_bit                           (1)
#define   NFP_UART_LSR_DR                               (0x1)
#define   NFP_UART_LSR_DR_bf                            0, 0, 0
#define   NFP_UART_LSR_DR_mask                          (0x1)
#define   NFP_UART_LSR_DR_bit                           (0)

#endif /* __NFP_SDK__NFP3200_NFP_UART_H__ */
