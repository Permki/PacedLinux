/*
 * Copyright (C) 2013 Netronome Systems, Inc. All rights reserved.
 * Author: Jason McMullan <jason.mcmullan@netronome.com>
 *
 */

#ifndef NFP_SERDES_H
#define NFP_SERDES_H

#include <nfp-common/nfp_platform.h>
#include <nfp_serdes_regs.h>

#ifdef __KERNEL__
#define NO_NFP_API_EXPORT
#endif

/* API is part of libnfp */
#include "_nfp_apiex.h"

struct nfp_serdes;

/**
 * Open a handle to the serdes units on an island
 * @ingroup nfp6000-only
 *
 * @param       nfp     NFP Device handle
 * @param       island  NFP island id
 * @return              NFP Serdes handle
 */
NFP_API
struct nfp_serdes *nfp_serdes_open(struct nfp_device *nfp, int island);

/**
 * Close the handle to an island's serdes units
 * @ingroup nfp6000-only
 *
 * @param       serdes  NFP Serdes handle
 */
NFP_API
void nfp_serdes_close(struct nfp_serdes *serdes);

/**
 * Get the name of a serdes handle
 * @ingroup nfp6000-only
 *
 * @param       serdes  NFP Serdes handle
 * @return              Name of the serdes group
 */
NFP_API
const char *nfp_serdes_name(struct nfp_serdes *serdes);

/**
 * Get the number of serdes units in the handle
 * @ingroup nfp6000-only
 *
 * @param       serdes  NFP Serdes handle
 * @return              Number of serdes units in the serdes group
 */
NFP_API
int nfp_serdes_units(struct nfp_serdes *serdes);

/**
 * Get the name of a serdes unit in an island
 * @ingroup nfp6000-only
 *
 * @param       serdes  NFP Serdes handle
 * @param       unit    NFP serdes unit number
 * @return              Name of a serdes unit
 */
NFP_API
const char *nfp_serdes_unit_name(struct nfp_serdes *serdes, int unit);

/**
 * Read serdes register by address
 * @ingroup nfp6000-only
 *
 * @param       serdes  NFP Serdes handle
 * @param       unit    Serdes unit number
 * @param       page    Serdes register page
 * @param       reg     Serdes register address
 * @param       value   Pointer to read value to
 * @return              0 on success, -1 and errno on failure
 */
NFP_API
int nfp_serdes_readb(struct nfp_serdes *serdes,
		     int unit,
		     int page,
		     int reg,
		     uint8_t *value);

/**
 * Write serdes register by address
 * @ingroup nfp6000-only
 *
 * @param       serdes  NFP Serdes handle
 * @param       unit    Serdes unit number
 * @param       page    Serdes register page
 * @param       reg     Serdes register address
 * @param       value   Value to write
 * @return              0 on success, -1 and errno on failure
 */
NFP_API
int nfp_serdes_writeb(struct nfp_serdes *serdes,
		      int unit,
		      int page,
		      int reg,
		      uint8_t value);

/**
 * Write serdes register by address
 * @ingroup nfp6000-only
 *
 * @param       serdes  NFP Serdes handle
 * @param       unit    Serdes unit number
 * @param       page    Serdes register page
 * @param       reg     Serdes register address
 * @param       mask    Mask of bits to change
 * @param       value   Value to write
 * @return              0 on success, -1 and errno on failure
 */
NFP_API
int nfp_serdes_writebm(struct nfp_serdes *serdes,
		       int unit,
		       int page,
		       int reg,
		       uint8_t mask,
		       uint8_t value);

/**
 * Wait on serdes register by address
 * @ingroup nfp6000-only
 *
 * @param       serdes  NFP Serdes handle
 * @param       unit    Serdes unit number
 * @param       page    Serdes register page
 * @param       reg     Serdes register address
 * @param       mask    Mask of bits to check
 * @param       value   Value to check against
 * @param   timeout_us  Timeout (< 0 for forever)
 * @return              0 on success, -1 and errno on failure
 */
NFP_API
int nfp_serdes_waitbm(struct nfp_serdes *serdes,
		      int unit,
		      int page,
		      int reg,
		      uint8_t mask,
		      uint8_t value,
		      int timeout_us);

/**
 * Get the sub-register/component count for a register
 * @ingroup nfp6000-only
 *
 * @param       id      Serdes register id
 * @param       count   sub-register/component count
 * @return              0 on success, -1 and errno on failure
 * completion
 */
NFP_API
int nfp_serdes_register_component_count(enum nfp_serdes_reg id, int *count);

/**
 * Get the bit width of the register from a register description handle
 * @ingroup nfp6000-only
 *
 * @param       id      Serdes register id
 * @return              Bit width of the register
 */
NFP_API
int nfp_serdes_register_width(enum nfp_serdes_reg id);

/**
 * Get the parameters of a sub-register/component for a register
 * @ingroup nfp6000-only
 *
 * @param       id      Serdes register id
 * @param       comp    sub-register/component index
 * @param       reg     sub-register address
 * @param       start   sub-register start bit
 * @param       end     sub-register end bit
 * @return              0 on success, -1 and errno on failure
 * completion
 */
NFP_API
int nfp_serdes_register_component_info(enum nfp_serdes_reg id, int comp,
				       int *subreg, int *start, int *end);

/**
 * Get the page of the specified register
 * @ingroup nfp6000-only
 *
 * @param       id      Serdes register id
 * @return              (page) >= 0 on success, -1 and errno on failure
 * completion
 */
NFP_API
int nfp_serdes_register_page(enum nfp_serdes_reg id);

/**
 * Get serdes register count
 * @ingroup nfp6000-only
 *
 * @return              (count) >= 0 on success, -1 and errno on failure
 * completion
 */
NFP_API
int nfp_serdes_register_count(void);

/**
 * Read serdes register by register description handle
 * @ingroup nfp6000-only
 *
 * @param       serdes  NFP Serdes handle
 * @param       unit    Serdes unit number
 * @param       page    Serdes register page
 * @param       id      Serdes register id
 * @param       value   Pointer to read value to
 * @return              0 on success, -1 and errno on failure
 */
NFP_API
int nfp_serdes_register_read(struct nfp_serdes *serdes, int unit, int page,
			     enum nfp_serdes_reg id, uint32_t *value);

/**
 * Write serdes register by address
 * @ingroup nfp6000-only
 *
 * @param       serdes  NFP Serdes handle
 * @param       unit    Serdes unit number
 * @param       page    Serdes register page
 * @param       id      Serdes register id
 * @param       value   Value to write
 * @return              0 on success, -1 and errno on failure
 */
NFP_API
int nfp_serdes_register_write(struct nfp_serdes *serdes, int unit, int page,
			      enum nfp_serdes_reg id, uint32_t value);

#endif /* NFP_SERDES_H */
