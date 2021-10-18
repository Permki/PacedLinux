/*
 * Copyright (C) 2014, Netronome Systems, Inc.
 * All right reserved.
 *
 */

#ifndef NFP_SPI_H
#define NFP_SPI_H

/* API is part of libnfp */
#include "_nfp_apiex.h"

struct nfp_spi;

/**
 * Acquire a handle to one of the NFP SPI busses
 * @ingroup nfp6000-only
 *
 * @param       nfp     NFP Device
 * @param       bus     SPI Bus (0..3)
 */
NFP_API
struct nfp_spi *nfp_spi_acquire(struct nfp_device *nfp, int bus);

/**
 * Release the handle to a NFP SPI bus
 * @ingroup nfp6000-only
 *
 * @param       spi     NFP SPI bus
 */
NFP_API
void nfp_spi_release(struct nfp_spi *spi);

/**
 * Set the SPI bus width
 * @ingroup nfp6000-only
 *
 * @param       spi     NFP SPI bus
 * @param       width   SPI Bus Width (0, 1 (default), 2 bit, or 4 bit)
 */
NFP_API
int nfp_spi_width_set(struct nfp_spi *spi, int width);

/**
 * Set the clock rate of the NFP SPI bus
 * @ingroup nfp6000-only
 *
 * @param       spi     NFP SPI bus
 * @param       hz      Desired SPI clock rate (0 = default speed)
 */
NFP_API
int nfp_spi_speed_set(struct nfp_spi *spi, unsigned int hz);

/**
 * Get the clock rate of the NFP SPI bus
 * @ingroup nfp6000-only
 *
 * @param       spi     NFP SPI bus
 * @param       hz      SPI clock rate pointer
 */
NFP_API
int nfp_spi_speed_get(struct nfp_spi *spi, unsigned int *hz);

/**
 * Set the SPI mode
 * @ingroup nfp6000-only
 *
 * @param       spi     NFP SPI bus
 * @param       mode    SPI CPHA/CPOL mode (-1, 0, 1, 2, or 3)
 *
 * Use mode of '-1' for the default for this bus.
 */

#define NFP_SPI_MC_DATAINOUTLEADTRAIL_FLAG BIT(2)
#define NFP_SPI_MC_DATAINOUTLEADTRAIL_OFFS (3)

NFP_API
int nfp_spi_mode_set(struct nfp_spi *spi, int mode);

/**
 * Get the SPI mode
 * @ingroup nfp6000-only
 *
 * @param       spi     NFP SPI bus
 * @param       mode    SPI CPHA/CPOL mode pointer
 */
NFP_API
int nfp_spi_mode_get(struct nfp_spi *spi, int *mode);

/**
 * Locks bus in preparation for nfp6000_spi_transact call(s)
 * @ingroup nfp6000-only
 *
 * @param       spi                      SPI Bus
 */
NFP_API
int nfp6000_spi_get_lock(struct nfp_spi *spi);

/**
 * Release bus after nfp6000_spi_transact call(s)
 * @ingroup nfp6000-only
 *
 * @param       spi                      SPI Bus
 */
NFP_API
void nfp6000_spi_release_lock(struct nfp_spi *spi);

/**
 * Perform an arbitrary SPI transaction'
 * @ingroup nfp6000-only
 *
 * @param       spi                      SPI Bus
 * @param       cs                       SPI Chip select (0..3)
 * @param       tx                       TX buffer
 * @param       tx_bit_cnt               TX buffer size in bits
 * @param       rx                       RX buffer
 * @param       rx_bit_cnt               RX buffer size in bits
 * @param       mdio_data_drive_disable  MDIO compatibility flag
 */
#define CS_SELECT BIT(0)
#define CS_DESELECT BIT(1)
NFP_API
int nfp6000_spi_transact(struct nfp_spi *spi,
			 int cs,
			 int cs_action,
			 const void *tx,
			 uint32_t tx_bit_cnt,
			 void *rx,
			 uint32_t rx_bit_cnt,
			 int mdio_data_drive_disable);

/**
 * Perform a trivial SPI read
 * @ingroup nfp6000-only
 *
 * @param       spi     SPI Bus
 * @param       cs      SPI Chip select (0..3)
 * @param       cmd_len Number of bytes in the command
 * @param       cmd     SPI command
 * @param       res_len Number of bytes of response
 * @param       res     SPI response
 */
NFP_API
int nfp_spi_read(struct nfp_spi *spi,
		 int cs,
		 unsigned int cmd_len,
		 const void *cmd,
		 unsigned int res_len,
		 void *res);

/**
 * Perform a trivial SPI write
 * @ingroup nfp6000-only
 *
 * @param       spi     SPI Bus
 * @param       cs      SPI Chip select (0..3)
 * @param       cmd_len Number of bytes in the command
 * @param       cmd     SPI command
 * @param       dat_len Number of bytes of write data
 * @param       dat     SPI write data
 */
NFP_API
int nfp_spi_write(struct nfp_spi *spi,
		  int cs,
		  unsigned int cmd_len,
		  const void *cmd,
		  unsigned int dat_len,
		  const void *dat);

/**
 * Perform a low level SPI CSR read
 * @ingroup nfp6000-only
 *
 * @param       spi     SPI Bus
 * @param       csr     Register to access
 * @param       val     Register data
 */
NFP_API
int nfp6000_spi_csr_readl(struct nfp_spi *spi, uint32_t csr, uint32_t *val);

/**
 * Perform a low level SPI CSR write
 * @ingroup nfp6000-only
 *
 * @param       spi     SPI Bus
 * @param       csr     Register to access
 * @param       val     Register data
 */
NFP_API
int nfp6000_spi_csr_writel(struct nfp_spi *spi, uint32_t csr, uint32_t val);

#endif /* NFP_SPI_H */
