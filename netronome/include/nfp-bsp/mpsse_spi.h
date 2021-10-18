/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file mpsse_spi.h
 *
 */

#ifndef _MPSSE_SPI_H_
#define _MPSSE_SPI_H_

/******************************************************************************/
#include "mpsse.h"

/* cs_pol_mask: 1 == active high, 0 == active low, active == enable state */
struct spi_intf * mpsse_spi_init    (u32 vid, u32 pid, u32 dev_id, u32 port,
                                     u32 cs_cnt, u32 cs_pol_mask, u32 spimode,
                                     u32 frequency_hz, u32 buffer_size,
                                     struct gpio_intf **);
void              mpsse_spi_deinit  (struct spi_intf * mi);

/******************************************************************************/

#endif //  _MPSSE_SPI_H_
/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 expandtab: */

