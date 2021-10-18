/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file spi_intf.h
 *
 */


#ifndef __SPI_INTERFACE_H__
#define __SPI_INTERFACE_H__
/******************************************************************************/

struct spi_intf {

    /* if cs is negative no action will be taken to set-up/change cs signals */
    int (*txrx) (struct spi_intf *, int cs,
                                u8 *tx, u32 tx_bitcnt, u8 *rx, u32 rx_bitcnt);
};

/******************************************************************************/
#endif // __SPI_INTERFACE_H__

