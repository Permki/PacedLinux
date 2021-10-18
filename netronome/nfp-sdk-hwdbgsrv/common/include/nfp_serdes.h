/*
 * Copyright (C) 2013 Netronome Systems, Inc. All rights reserved.
 * Author: Jason McMullan <jason.mcmullan@netronome.com>
 *
 */

#ifndef NFP_SERDES_H
#define NFP_SERDES_H

#include <nfp-common/nfp_platform.h>

#ifdef __KERNEL__
#define NO_NFP_API_EXPORT
#endif

/* API is part of libnfp */
#include "_nfp_apiex.h"

struct nfp_serdes;

struct nfp_serdes_register {
    const char *regname, *regdesc;
    int page, regs;
    struct nfp_serdes_csr {
        int reg, start, end;
    } reg[5];
    const char *regval[32];
};

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
int nfp_serdes_readb(struct nfp_serdes *serdes, int unit, int page, int reg, uint8_t *value);

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
int nfp_serdes_writeb(struct nfp_serdes *serdes, int unit, int page, int reg, uint8_t value);

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
int nfp_serdes_writebm(struct nfp_serdes *serdes, int unit, int page, int reg, uint8_t mask, uint8_t value);

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
int nfp_serdes_waitbm(struct nfp_serdes *serdes, int unit, int page, int reg, uint8_t mask, uint8_t value, int timeout_us);

/**
 * Get the first named register in a serdes unit on a specific page
 * @ingroup nfp6000-only
 *
 * @param       serdes  NFP Serdes handle
 * @param       unit    Serdes unit number
 * @param       page    Serdes register page (0 or 4)
 * @param       regname Serdes register name (optional, can be NULL)
 * @return              Serdes register description handle, or NULL and errno
 */
NFP_API
const struct nfp_serdes_register *nfp_serdes_register_lookup(struct nfp_serdes *serdes, int unit, int page, const char *regname);

/**
 * Get the next named register
 * @ingroup nfp6000-only
 *
 * @param       reg     Serdes register description handle
 * @return              Serdes register description handle, or NULL on completion
 */
NFP_API
const struct nfp_serdes_register *nfp_serdes_register_next(const struct nfp_serdes_register *reg);

/**
 * Read serdes register by register description handle
 * @ingroup nfp6000-only
 *
 * @param       serdes  NFP Serdes handle
 * @param       unit    Serdes unit number
 * @param       page    Serdes register page
 * @param       reg     Serdes register description handle
 * @param       value   Pointer to read value to
 * @return              0 on success, -1 and errno on failure
 */
NFP_API
int nfp_serdes_register_read(struct nfp_serdes *serdes, int unit, int page, const struct nfp_serdes_register *reg, uint32_t *value);

/** 
 * Write serdes register by address
 * @ingroup nfp6000-only
 *
 * @param       serdes  NFP Serdes handle
 * @param       unit    Serdes unit number
 * @param       page    Serdes register page
 * @param       reg     Serdes register description handle
 * @param       value   Value to write
 * @return              0 on success, -1 and errno on failure
 */
NFP_API
int nfp_serdes_register_write(struct nfp_serdes *serdes, int unit, int page, const struct nfp_serdes_register *reg, uint32_t value);

/**
 * Get the name of serdes register from a register description handle
 * @ingroup nfp6000-only
 *
 * @param       reg     Serdes register description handle
 * @return              Name of the register
 */
NFP_API
const char *nfp_serdes_register_name(const struct nfp_serdes_register *reg);

/**
 * Get any extra description for the register description handle
 * @ingroup nfp6000-only
 *
 * @param       reg     Serdes register description handle
 * @return              Description of the register
 */
NFP_API
const char *nfp_serdes_register_description(const struct nfp_serdes_register *reg);

/**
 * Get the bit width of the register from a register description handle
 * @ingroup nfp6000-only
 *
 * @param       reg     Serdes register description handle
 * @return              Bit width of the register
 */
NFP_API
int nfp_serdes_register_width(const struct nfp_serdes_register *reg);

#endif /* NFP_SERDES_H */
