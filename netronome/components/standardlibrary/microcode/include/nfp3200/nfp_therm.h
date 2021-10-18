/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_therm.h
 * @brief       Register definitions for Thermal Sensor
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */

#ifndef __NFP_SDK__NFP3200_NFP_THERM_H__
#define __NFP_SDK__NFP3200_NFP_THERM_H__

/* HGID: nfp3200/therm.desc = 1313ce7d94a3 */
/* Register Type: ThermalSensor */
#define NFP_THERM_DYNAMIC              0x0008
#define NFP_THERM_STATIC               0x000c
#define   NFP_THERM_TRIP_IND                            (0x1 << 11)
#define   NFP_THERM_TRIP_IND_bf                         0, 11, 11
#define   NFP_THERM_TRIP_IND_mask                       (0x1)
#define   NFP_THERM_TRIP_IND_bit                        (11)
#define     NFP_THERM_TRIP_IND_XXX                      (0x0)
#define     NFP_THERM_TRIP_IND_ENABLE                   (0x800)
#define   NFP_THERM_EN                                  (0x1 << 10)
#define   NFP_THERM_EN_bf                               0, 10, 10
#define   NFP_THERM_EN_mask                             (0x1)
#define   NFP_THERM_EN_bit                              (10)
#define   NFP_THERM_DIN(_x)                             (((_x) & 0xff) << 2)
#define   NFP_THERM_DIN_bf                              0, 9, 2
#define   NFP_THERM_DIN_mask                            (0xff)
#define   NFP_THERM_DIN_shift                           (2)
#define   NFP_THERM_TEST_CONF(_x)                       ((_x) & 0x3)
#define   NFP_THERM_TEST_CONF_bf                        0, 1, 0
#define   NFP_THERM_TEST_CONF_mask                      (0x3)
#define   NFP_THERM_TEST_CONF_shift                     (0)

#endif /* __NFP_SDK__NFP3200_NFP_THERM_H__ */
