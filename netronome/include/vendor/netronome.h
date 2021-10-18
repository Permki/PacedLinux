/*
 * Copyright (C) 2016 Netronome Systems, Inc. All rights reserved.
 * Author: David Brunecz <david.brunecz@netronome.com>
 *
 */

#ifndef NFP_VENDOR_LIB_H
#define NFP_VENDOR_LIB_H

#include <nfp.h>

int nfp_cpld_read(struct nfp_device *nfp, uint32_t addr, uint32_t *val);
int nfp_cpld_write(struct nfp_device *nfp, uint32_t addr, uint32_t val);

int nfp_fpga_read(struct nfp_device *nfp, uint32_t addr, uint32_t *val);
int nfp_fpga_write(struct nfp_device *nfp, uint32_t addr, uint32_t val);

int fpga_i2c_read(struct nfp_device *nfp, uint8_t chip,
		  uint32_t addr, size_t alen, uint8_t *buff, size_t buff_len);
int fpga_i2c_write(struct nfp_device *nfp, uint8_t chip,
		   uint32_t addr, size_t alen, const uint8_t *buff, size_t buff_len);
int nfp_phymod_cpld_write(struct nfp_device *nfp, int phymod, uint16_t addr, uint16_t val);
int nfp_phymod_cpld_read(struct nfp_device *nfp, int phymod, uint16_t addr, uint16_t *val);

#endif /* NFP_VENDOR_LIB_H */
