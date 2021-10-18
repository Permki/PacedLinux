/*
 * Copyright (C) 2015, Netronome Systems, Inc.
 * All right reserved.
 *
 */

#ifndef NFP_ECC_H
#define NFP_ECC_H

#include <nfp.h>

struct nfp_ecc_error;

/**
 * Determine if the error has been corrected
 *
 * @parm        e_err         NFP ECC Error handle
 *
 * @return      0 if the error was uncorrected, 1 if was corrected.
 */
int nfp_ecc_error_corrected(const struct nfp_ecc_error *e_err);

/**
 * Get the error handle's error SRAM address
 *
 * @parm        e_err         NFP ECC Error handle
 *
 * @return      SRAM Address of the ECC error
 */
uint64_t nfp_ecc_error_sram_address(const struct nfp_ecc_error *e_err);

/**
 * Get the error handle's error ECC delta
 *
 * @parm        e_err         NFP ECC Error handle
 *
 * @return      ECC Delta of the ECC error
 */
uint8_t nfp_ecc_error_ecc_delta(const struct nfp_ecc_error *e_err);

struct nfp_ecc_monitor;

/**
 * Get the name of the ECC monitor
 *
 * @parm        m       NFP ECC Monitor
 *
 * @return      Name of the NFP ECC Monitor
 */
const char *nfp_ecc_monitor_name(const struct nfp_ecc_monitor *m);

/**
 * Get the enable/disable state of the ECC monitor
 *
 * @parm        m       NFP ECC Monitor
 *
 * @return      > 0 if enabled, 0 if disabled
 */
int nfp_ecc_monitor_enabled(const struct nfp_ecc_monitor *m);

/**
 * Get the XPB address of the ECC monitor
 *
 * @parm        m       NFP ECC Monitor
 *
 * @return      XPB address of the NFP ECC Monitor
 */
uint32_t nfp_ecc_monitor_xpb(const struct nfp_ecc_monitor *m);

/**
 * Get the total count of the corrected errors
 *
 * @param       ecc     NFP ECC handle
 *
 * NOTE: This is only the total since the ECC monitor was last
 *       scanned, not a system lifetime count.
 */
uint32_t nfp_ecc_monitor_corrected(const struct nfp_ecc_monitor *m);

/**
 * Get the total count of the uncorrected errors
 *
 * @param       ecc     NFP ECC handle
 *
 * NOTE: This is only the total since the ECC monitor was last
 *       scanned, not a system lifetime count.
 */
uint32_t nfp_ecc_monitor_uncorrected(const struct nfp_ecc_monitor *m);

/**
 * Get the error montor's first error
 *
 * @parm        e_err         NFP ECC Error handle
 *
 * @return      NFP ECC Monitor handle, or NULL if no error
 */
const struct nfp_ecc_error *nfp_ecc_monitor_error_first(const struct nfp_ecc_monitor *m);

/**
 * Get the error montor's last error
 *
 * @parm        e_err         NFP ECC Error handle
 *
 * @return      NFP ECC Monitor handle, or NULL if no error
 */
const struct nfp_ecc_error *nfp_ecc_monitor_error_last(const struct nfp_ecc_monitor *m);

struct nfp_ecc;

/**
 * Acquire a handle to the NFP ECC monitor
 *
 * @param       nfp     NFP Device handle
 *
 * @return      Non-NULL NFP ECC handle, or NULL (and errno) on error
 */
struct nfp_ecc *nfp_ecc_acquire(struct nfp_device *nfp);

/**
 * Release the NFP ECC handle
 *
 * @param       ecc     NFP ECC handle
 */
void nfp_ecc_release(struct nfp_ecc *ecc);

#define NFP_ECC_FILTER_ENABLED BIT(0)
#define NFP_ECC_FILTER_CORRECTED BIT(1)
#define NFP_ECC_FILTER_UNCORRECTED BIT(2)

/**
 * Set the filtering for the NFP ECC rescan
 *
 * @param      ecc     NFP ECC handle
 * @param      mask    Filtering flag mask (bit set = check match bit)
 * @param      match   Filtering flag match
 */
void nfp_ecc_filter_set(struct nfp_ecc *ecc, uint32_t mask, uint32_t match);

/**
 * Rescan for new NFP ECC errors
 *
 * @param       ecc     NFP ECC handle
 *
 * @return      0 if no errors, > 0 if errors found, -1 and errno if failure
 */
int nfp_ecc_rescan(struct nfp_ecc *ecc);

/**
 * Get the total count of the corrected errors
 *
 * @param       ecc     NFP ECC handle
 *
 * NOTE: This is only the total since the ECC handle was acquired,
 *       not a system lifetime count.
 */
uint64_t nfp_ecc_total_corrected(struct nfp_ecc *ecc);

/**
 * Get the total count of the uncorrected errors
 *
 * @param       ecc     NFP ECC handle
 *
 * NOTE: This is only the total since the ECC handle was acquired,
 *       not a system lifetime count.
 */
uint64_t nfp_ecc_total_uncorrected(struct nfp_ecc *ecc);

/**
 * Iterate over the filtered list of ECC monitors.
 *
 * @param       ecc     NFP ECC handle
 * @param       iter    Pointer to a temporary pointer, void on first call
 *
 * @return      NFP ECC Monitor handle, or NULL if no more entries
 */
const struct nfp_ecc_monitor *nfp_ecc_monitor_next(struct nfp_ecc *ecc, void **iter);

#endif /* NFP_ECC_H */
