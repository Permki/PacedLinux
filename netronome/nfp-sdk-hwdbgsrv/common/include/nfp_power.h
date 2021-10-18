/*
 * Copyright (C) 2014,2017 Netronome Systems, Inc.
 * All right reserved.
 *
 */

#ifndef __NFP_POWER_H__
#define __NFP_POWER_H__

#include <nfp-common/nfp_platform.h>

#ifdef __KERNEL__
#define NO_NFP_API_EXPORT
#endif

/* API is part of libnfp */
#include "_nfp_apiex.h"

/**
 * NFP Device Power States
 *
 * NFP_DEVICE_STATE_P0          Clocked, reset released
 * NFP_DEVICE_STATE_P1          Not clocked, reset released
 * NFP_DEVICE_STATE_P2          Clocked, held in reset
 * NFP_DEVICE_STATE_P3          No clocking, held in reset
 *
 * NOTE: Transitioning a device from P0 to power state P2
 *       or P3 will imply that all running configuration
 *       of the device will be lost, and the device must
 *       be re-initialized when P0 state is re-entered.
 */
#define NFP_DEVICE_STATE_P0     0
#define NFP_DEVICE_STATE_P1     1
#define NFP_DEVICE_STATE_P2     2
#define NFP_DEVICE_STATE_P3     3

/* Friendly aliases of the above device states
 */
#define NFP_DEVICE_STATE_ON             NFP_DEVICE_STATE_P0
#define NFP_DEVICE_STATE_SUSPEND        NFP_DEVICE_STATE_P1
#define NFP_DEVICE_STATE_RESET          NFP_DEVICE_STATE_P2
#define NFP_DEVICE_STATE_OFF            NFP_DEVICE_STATE_P3

#define NFP_TH_DEVICE(island, unit)     ((((island) & 0x3f) << 8) | ((unit) & 0xf))
#define NFP_TH_DEVICE_ISLAND_of(x)      (((x) >> 8) & 0x3f)
#define NFP_TH_DEVICE_UNIT_of(x)        (((x) >> 0) & 0x0f)

/* Aliases to simplify the transition to new names.
 */
#define NFP6000_DEVICE NFP_TH_DEVICE
#define NFP6000_DEVICE_ISLAND_of NFP_TH_DEVICE_ISLAND_of
#define NFP6000_DEVICE_UNIT_of NFP_TH_DEVICE_UNIT_of

/**
 * NFP6000 specific subdevice identifiers
 */
#define NFP6000_DEVICE_ARM(dev, unit)   NFP_TH_DEVICE((dev) + 1, unit)
#define     NFP6000_DEVICE_ARM_CORE  0
#define     NFP6000_DEVICE_ARM_ARM   1
#define     NFP6000_DEVICE_ARM_GSK   2
#define     NFP6000_DEVICE_ARM_PRH   3
#define     NFP6000_DEVICE_ARM_MEG0  4
#define     NFP6000_DEVICE_ARM_MEG1  5
#define NFP6000_DEVICE_PCI(dev, unit)    NFP_TH_DEVICE((dev) + 4, unit)
#define     NFP6000_DEVICE_PCI_CORE  0
#define     NFP6000_DEVICE_PCI_PCI   1
#define     NFP6000_DEVICE_PCI_MEG0  2
#define     NFP6000_DEVICE_PCI_MEG1  3
#define     NFP6000_DEVICE_PCI_EXT   4
#define NFP6000_DEVICE_NBI(dev, unit)    NFP_TH_DEVICE((dev) + 8, unit)
#define     NFP6000_DEVICE_NBI_CORE  0
#define     NFP6000_DEVICE_NBI_MAC4  4
#define     NFP6000_DEVICE_NBI_MAC5  5
#define NFP6000_DEVICE_CRP(dev, unit)    NFP_TH_DEVICE((dev) + 12, unit)
#define     NFP6000_DEVICE_CRP_CORE  0
#define     NFP6000_DEVICE_CRP_CRP   1
#define     NFP6000_DEVICE_CRP_MEG0  2
#define     NFP6000_DEVICE_CRP_MEG1  3
#define NFP6000_DEVICE_EMU(dev, unit)    NFP_TH_DEVICE((dev) + 24, unit)
#define     NFP6000_DEVICE_EMU_CORE  0
#define     NFP6000_DEVICE_EMU_QUE   1
#define     NFP6000_DEVICE_EMU_LUP   2
#define     NFP6000_DEVICE_EMU_DAL   3
#define     NFP6000_DEVICE_EMU_EXT   4
#define     NFP6000_DEVICE_EMU_DDR0  5
#define     NFP6000_DEVICE_EMU_DDR1  6
#define NFP6000_DEVICE_IMU(dev, unit)    NFP_TH_DEVICE((dev) + 28, unit)
#define     NFP6000_DEVICE_IMU_CORE  0
#define     NFP6000_DEVICE_IMU_STS   1
#define     NFP6000_DEVICE_IMU_LBL   2
#define     NFP6000_DEVICE_IMU_CLU   3
#define     NFP6000_DEVICE_IMU_NLU   4
#define NFP6000_DEVICE_FPC(dev, unit)    NFP_TH_DEVICE((dev) + 32, unit)
#define     NFP6000_DEVICE_FPC_CORE  0
#define     NFP6000_DEVICE_FPC_MEG0  1
#define     NFP6000_DEVICE_FPC_MEG1  2
#define     NFP6000_DEVICE_FPC_MEG2  3
#define     NFP6000_DEVICE_FPC_MEG3  4
#define     NFP6000_DEVICE_FPC_MEG4  5
#define     NFP6000_DEVICE_FPC_MEG5  6
#define NFP6000_DEVICE_ILA(dev, unit)    NFP_TH_DEVICE((dev) + 48, unit)
#define     NFP6000_DEVICE_ILA_CORE  0
#define     NFP6000_DEVICE_ILA_ILA   1
#define     NFP6000_DEVICE_ILA_MEG0  2
#define     NFP6000_DEVICE_ILA_MEG1  3

/**
 * NFP3800 specific subdevice identifiers
 */
#define NFP3800_DEVICE_ARM(dev, unit)   NFP_TH_DEVICE((dev) + 1, unit)
#define     NFP3800_DEVICE_ARM_CORE  0
#define     NFP3800_DEVICE_ARM_ARM   1
#define     NFP3800_DEVICE_ARM_GSK   2
#define     NFP3800_DEVICE_ARM_PRH   3
#define     NFP3800_DEVICE_ARM_MEG0  4
#define NFP3800_DEVICE_PCI(dev, unit)    NFP_TH_DEVICE((dev) + 4, unit)
#define     NFP3800_DEVICE_PCI_CORE  0
#define     NFP3800_DEVICE_PCI_PCI   1
#define     NFP3800_DEVICE_PCI_MEG0  2
#define     NFP3800_DEVICE_PCI_MEG1  3
#define     NFP3800_DEVICE_PCI_EXT   4
#define NFP3800_DEVICE_NBI(dev, unit)    NFP_TH_DEVICE((dev) + 8, unit)
#define     NFP3800_DEVICE_NBI_CORE  0
#define     NFP3800_DEVICE_NBI_MAC4  4
#define NFP3800_DEVICE_CRP(dev, unit)    NFP_TH_DEVICE((dev) + 12, unit)
#define     NFP3800_DEVICE_CRP_CORE  0
#define     NFP3800_DEVICE_CRP_CRP   1
#define     NFP3800_DEVICE_CRP_MEG0  2
#define     NFP3800_DEVICE_CRP_MEG1  3
#define NFP3800_DEVICE_EMU(dev, unit)    NFP_TH_DEVICE((dev) + 24, unit)
#define     NFP3800_DEVICE_EMU_CORE  0
#define     NFP3800_DEVICE_EMU_QUE   1
#define     NFP3800_DEVICE_EMU_LUP   2
#define     NFP3800_DEVICE_EMU_DAL   3 /* TODO: Review this, NFPBSP-3143 */
#define     NFP3800_DEVICE_EMU_EXT   4
#define     NFP3800_DEVICE_EMU_DDR0  5 /* TODO: Review this, NFPBSP-3143 */
#define     NFP3800_DEVICE_EMU_DDR1  6
#define NFP3800_DEVICE_FPC(dev, unit)    NFP_TH_DEVICE((dev) + 32, unit)
#define     NFP3800_DEVICE_FPC_CORE  0
#define     NFP3800_DEVICE_FPC_MEG0  1
#define     NFP3800_DEVICE_FPC_MEG1  2
#define     NFP3800_DEVICE_FPC_MEG2  3

__BEGIN_DECLS;

struct nfp_device;

static nfp_inline unsigned int nfp6000_power_subdev_megrp(int iid, int megroup)
{
	switch (iid) {
	case 1:
		if (megroup < 2)
			return NFP6000_DEVICE_ARM(iid - 1,
						  NFP6000_DEVICE_ARM_MEG0 + megroup);
		break;
	case 4:
	case 5:
	case 6:
	case 7:
		if (megroup < 2)
			return NFP6000_DEVICE_PCI(iid - 4,
						  NFP6000_DEVICE_PCI_MEG0 + megroup);
		break;
	case 12:
	case 13:
		if (megroup < 2)
			return NFP6000_DEVICE_CRP(iid - 12,
						  NFP6000_DEVICE_CRP_MEG0 + megroup);
		break;
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
		if (megroup < 6)
			return NFP6000_DEVICE_FPC(iid - 32,
						  NFP6000_DEVICE_FPC_MEG0 + megroup);
		break;
	case 48:
	case 49:
		if (megroup < 2)
			return NFP6000_DEVICE_ILA(iid - 48,
						  NFP6000_DEVICE_ILA_MEG0 + megroup);
		break;
	default:
		break;
	}

	return 0;
}

static nfp_inline unsigned int nfp3800_power_subdev_megrp(int iid, int megroup)
{
	switch (iid) {
	case 1:
		if (megroup < 1)
			return NFP3800_DEVICE_ARM(iid - 1,
						  NFP3800_DEVICE_ARM_MEG0 + megroup);
		break;
	case 4:
		if (megroup < 2)
			return NFP3800_DEVICE_PCI(iid - 4,
						  NFP3800_DEVICE_PCI_MEG0 + megroup);
		break;
	case 12:
		if (megroup < 2)
			return NFP3800_DEVICE_CRP(iid - 12,
						  NFP3800_DEVICE_CRP_MEG0 + megroup);
		break;
	case 32:
	case 33:
	case 34:
		if (megroup < 3)
			return NFP3800_DEVICE_FPC(iid - 32,
						  NFP3800_DEVICE_FPC_MEG0 + megroup);
		break;
	default:
		break;
	}

	return 0;
}

/**
 * Get current device state
 *
 * @param dev           NFP device
 * @param subdevice     NFP subdevice
 * @param state         Power state
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_power_get(struct nfp_device *dev, unsigned int subdevice, int *state);

/**
 * Set device power state
 *
 * @param dev           NFP device
 * @param subdevice     NFP subdevice
 * @param state         Power state
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_power_set(struct nfp_device *dev, unsigned int subdevice, int state);

/**
 * Perform a soft-reset of the NFP, if possible
 *
 * @param dev           NFP device
 *
 * @return 0 on success, or -1 on error (and set errno accordingly).
 */
NFP_API
int nfp_reset_soft(struct nfp_device *dev);

/** Check if an island is powered on (thus available).
 *
 * For @p iid <= 0, the return value is always 1.
 *
 * A helper function to know when we expect an island to respond to a
 * CPP command. It checks only one subdevice in an island to minimize the
 * number of CPP reads (useful for simulation), with the exception of NBI
 * islands.
 *
 * For a given device handle instance, once an island is seen as
 * powered-on the state is cached to prevent further CPP reads. If it was not
 * seen as powered-on yet, it will be checked again.
 *
 * Return value does not differentiate between an error or a power-off island
 * to keep things simple. In both cases, errno most likely is ENODEV.
 *
 * @param               nfp Device
 * @param               iid Island ID
 * @return              0 on error or if island is off, 1 if island is on.
 */
NFP_API
int nfp_island_powered(struct nfp_device *nfp, int iid);

__END_DECLS;

#endif /* __NFP_POWER_H__ */
