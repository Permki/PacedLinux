/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/gpio.h
 * @brief       Register definitions for GPIO
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_GPIO_H__
#define __NFP_SDK__NFP3200_NFP_GPIO_H__

/* HGID: nfp3200/gpio.desc = 9aa6ddb03994 */
/* Register Type: GpioLevels */
#define NFP_GPIO_PLR                   0x0000
#define   NFP_GPIO_PLR_MASK_bf                          0, 11, 0
#define   NFP_GPIO_PLR_MASK_mask                        (0xfff)
#define   NFP_GPIO_PLR_MASK_shift                       (0)
/* Register Type: GpioPinDirection */
#define NFP_GPIO_PDPR                  0x0004
#define   NFP_GPIO_PDPR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_PDPR_MASK_bf                         0, 11, 0
#define   NFP_GPIO_PDPR_MASK_mask                       (0xfff)
#define   NFP_GPIO_PDPR_MASK_shift                      (0)
/* Register Type: GpioPinMakeOutputs */
#define NFP_GPIO_PDSR                  0x0008
#define   NFP_GPIO_PDSR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_PDSR_MASK_mask                       (0xfff)
#define   NFP_GPIO_PDSR_MASK_shift                      (0)
/* Register Type: GpioPinMakeInputs */
#define NFP_GPIO_PDCR                  0x000c
#define   NFP_GPIO_PDCR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_PDCR_MASK_mask                       (0xfff)
#define   NFP_GPIO_PDCR_MASK_shift                      (0)
/* Register Type: GpioSetOutputValues */
#define NFP_GPIO_POPR                  0x0010
#define   NFP_GPIO_POPR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_POPR_MASK_bf                         0, 11, 0
#define   NFP_GPIO_POPR_MASK_mask                       (0xfff)
#define   NFP_GPIO_POPR_MASK_shift                      (0)
/* Register Type: GpioSetOutputsHigh */
#define NFP_GPIO_POSR                  0x0014
#define   NFP_GPIO_POSR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_POSR_MASK_mask                       (0xfff)
#define   NFP_GPIO_POSR_MASK_shift                      (0)
/* Register Type: GpioSetOutputsLow */
#define NFP_GPIO_POCR                  0x0018
#define   NFP_GPIO_POCR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_POCR_MASK_mask                       (0xfff)
#define   NFP_GPIO_POCR_MASK_shift                      (0)
/* Register Type: GpioEdgeDetectRising */
#define NFP_GPIO_REDR                  0x001c
#define   NFP_GPIO_REDR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_REDR_MASK_bf                         0, 11, 0
#define   NFP_GPIO_REDR_MASK_mask                       (0xfff)
#define   NFP_GPIO_REDR_MASK_shift                      (0)
/* Register Type: GpioEdgeDetectFalling */
#define NFP_GPIO_FEDR                  0x0020
#define   NFP_GPIO_FEDR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_FEDR_MASK_bf                         0, 11, 0
#define   NFP_GPIO_FEDR_MASK_mask                       (0xfff)
#define   NFP_GPIO_FEDR_MASK_shift                      (0)
/* Register Type: GpioEdgeDetectStatus */
#define NFP_GPIO_EDSR                  0x0024
#define   NFP_GPIO_EDSR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_EDSR_MASK_bf                         0, 11, 0
#define   NFP_GPIO_EDSR_MASK_mask                       (0xfff)
#define   NFP_GPIO_EDSR_MASK_shift                      (0)
/* Register Type: GpioLevelSensitiveHigh */
#define NFP_GPIO_LSHR                  0x0028
#define   NFP_GPIO_LSHR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_LSHR_MASK_bf                         0, 11, 0
#define   NFP_GPIO_LSHR_MASK_mask                       (0xfff)
#define   NFP_GPIO_LSHR_MASK_shift                      (0)
/* Register Type: GpioLevelSensitiveLow */
#define NFP_GPIO_LSLR                  0x002c
#define   NFP_GPIO_LSLR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_LSLR_MASK_bf                         0, 11, 0
#define   NFP_GPIO_LSLR_MASK_mask                       (0xfff)
#define   NFP_GPIO_LSLR_MASK_shift                      (0)
/* Register Type: GpioLevelSensitiveStatus */
#define NFP_GPIO_LDSR                  0x0030
#define   NFP_GPIO_LDSR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_LDSR_MASK_bf                         0, 11, 0
#define   NFP_GPIO_LDSR_MASK_mask                       (0xfff)
#define   NFP_GPIO_LDSR_MASK_shift                      (0)
/* Register Type: GpioInterruptEnables */
#define NFP_GPIO_INER                  0x0034
#define   NFP_GPIO_INER_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_INER_MASK_bf                         0, 11, 0
#define   NFP_GPIO_INER_MASK_mask                       (0xfff)
#define   NFP_GPIO_INER_MASK_shift                      (0)
/* Register Type: GpioInterruptEnableSet */
#define NFP_GPIO_INSR                  0x0038
#define   NFP_GPIO_INSR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_INSR_MASK_mask                       (0xfff)
#define   NFP_GPIO_INSR_MASK_shift                      (0)
/* Register Type: GpioInterruptEnableReset */
#define NFP_GPIO_INCR                  0x003c
#define   NFP_GPIO_INCR_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_INCR_MASK_mask                       (0xfff)
#define   NFP_GPIO_INCR_MASK_shift                      (0)
/* Register Type: GpioInterruptStatus */
#define NFP_GPIO_INST                  0x0040
#define   NFP_GPIO_INST_MASK(_x)                        ((_x) & 0xfff)
#define   NFP_GPIO_INST_MASK_bf                         0, 11, 0
#define   NFP_GPIO_INST_MASK_mask                       (0xfff)
#define   NFP_GPIO_INST_MASK_shift                      (0)

#endif /* __NFP_SDK__NFP3200_NFP_GPIO_H__ */
