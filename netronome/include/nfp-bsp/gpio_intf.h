/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file gpio_intf.h
 *
 */


#ifndef __GPIO_INTERFACE_H__
#define __GPIO_INTERFACE_H__
/******************************************************************************/

struct gpio_intf {

    void *priv;

    int (*set_gpio)     (struct gpio_intf *, u32 pin, u32 val);
    int (*set_gpio_dir) (struct gpio_intf *, u32 pin, u32 output, u32 val);
    int (*get_gpio)     (struct gpio_intf *, u32 *val_bits);
    int (*get_gpio_dir) (struct gpio_intf *, u32 *dir_bits);
    int (*count)        (struct gpio_intf *);
};

/******************************************************************************/
#endif  //__GPIO_INTERFACE_H__

