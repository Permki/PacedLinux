/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file i2c_intf.h
 *
 */


#ifndef __I2C_INTERFACE_H__
#define __I2C_INTERFACE_H__
/******************************************************************************/

struct i2c_intf {

    void *priv;

    int (*sendbyte)    (struct i2c_intf *, u8 dev, u8 byte);
    int (*getbyte)     (struct i2c_intf *, u8 dev, u8 *byte);
    int (*read_addr8)  (struct i2c_intf *, u8 dev, u8 addr, u8 *buf, u32 cnt);
    int (*read_addr16) (struct i2c_intf *, u8 dev, u16 addr, u8 *buf, u32 cnt);
    int (*write_addr8) (struct i2c_intf *, u8 dev, u8 addr, u8 *buf, u32 cnt);
    int (*write_addr16)(struct i2c_intf *, u8 dev, u16 addr, u8 *buf, u32 cnt);
    int (*txrx)        (struct i2c_intf *, u8 dev, u8 *tx, u32 tx_sz,
                                              u8 *rx, u32 rx_sz, u32 nostop);
};

/******************************************************************************/
#endif // __I2C_INTERFACE_H__
