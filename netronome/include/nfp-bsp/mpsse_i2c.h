/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file mpsse_i2c.h
 *
 */


#ifndef _MPSSE_I2C_H_
#define _MPSSE_I2C_H_

/******************************************************************************/
#include "mpsse.h"

struct i2c_intf * nfp6_svp_i2c_init (u32 ser_num, u32 freq_hz, u32 buf_size);
struct i2c_intf * mpsse_i2c_init    (u32 vid, u32 pid, u32 dev_id, u32 port,
                                     u32 frequency_hz, u32 buffer_size,
                                     struct gpio_intf **);
void              mpsse_i2c_deinit  (struct i2c_intf * mi);

/******************************************************************************/

#endif //  _MPSSE_I2C_H_
/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 expandtab: */
