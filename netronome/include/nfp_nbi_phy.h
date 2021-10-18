/**
 * Copyright (C) 2013-2017 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_nbi_phy.h
 *
 */

#ifndef __NFP_NBI_PHY_H__
#define __NFP_NBI_PHY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <nfp.h>
#include <nfp_cpp.h>
#include <nfp_serdes.h>

#include "nfp_nbi.h"
#include "nfp_nbi_phy_init.h"
#include "nfp_nbi_phy_config.h"

/**
 * Enable PMA Rx-Tx loopback at the Serdes receiver
 */
#define NFP_NBI_MAC_LOOP_LINEPMA1  0x10
/**
 * Enable PMA Tx-Rx loopback at the Serdes Serializer
 */
#define NFP_NBI_MAC_LOOP_SYSPMA1   0x20
/**
 * Enable PMA Tx-Rx loopback at the Serdes Tx driver
 */
#define NFP_NBI_MAC_LOOP_SYSPMA2   0x40
/**
 * Enable PMA Parallel Rx-Tx loopback at the Serdes data port
 */
#define NFP_NBI_MAC_LOOP_LINEPMA2  0x80

/**
 * Initialize one serdes (4 lanes)
 * @ingroup nfp6000-only
 *
 * Executes the sequence which writes the Tx & Rx tuning values to the
 * specified serdes.
 *
 *  @param serdes Serdes handle
 *  @param params selection parameters
 *
 *  @return zero on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_init_group(struct nfp_serdes *serdes,
			   struct nfp_nbi_phy_params *params);

/**
 * Initialize RX adaptation
 * @ingroup nfp6000-only
 *
 * Executes the sequence which initializes Rx adaptation on the
 * specified serdes.
 *
 *  @param serdes Serdes handle
 *  @param params selection parameters
 *
 *  @return zero on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_rxadapt(struct nfp_serdes *serdes,
			struct nfp_nbi_phy_params *params);

/**
 * Start RX adaptation
 * @ingroup nfp6000-only
 *
 * Executes the sequence which initiates Rx adaptation on the
 * specified serdes lane.
 *
 *  @param serdes Serdes handle
 *  @param params selection parameters
 *
 *  @return zero on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_rxadapt_start(struct nfp_serdes *serdes,
			      struct nfp_nbi_phy_params *params);

/**
 * Check if RX adaptation finished
 * @ingroup nfp6000-only
 *
 * Determine if Rx Adaptation is finished
 *  @param serdes Serdes handle
 *  @param params selection parameters
 *
 *  @return zero on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_rxadapt_done(struct nfp_serdes *serdes,
			     struct nfp_nbi_phy_params *params);

/**
 * Rervert to static rx tuning values
 * @ingroup nfp6000-only
 *
 *  @param serdes Serdes handle
 *  @param params selection parameters
 *
 */
NFP_NBI_API
void nfp_nbi_phy_use_static(struct nfp_serdes *serdes,
			    struct nfp_nbi_phy_params *params);

/**
 *  Load the serdes tuning values from a file
 * @ingroup nfp6000-only
 *
 *  Tuning values are maintained in a JSON file.
 *  @param[out] phy_config PHY configuration to load into
 *  @param fp  file handle
 *
 *  @return zero on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_load_config(struct nfp_nbi_phy_config *phy_config, FILE *fp);

/**
 * Write PHY loopback modes for lanes specified by lanemask
 * @ingroup nfp6000-only
 *
 * Each SerDes lane supports two system side loopbacks and two line-side
 * loopbacks.
 *
 * These loopbacks may be enabled/disabled in various combinations for
 * testing.  The loopback codes are defined in nfp_nbi_phy.h and can be
 * ORed together to enable multiple loopbacks on a per-lane basis.
 *
 * @param nbi NBI device
 * @param lanemask 24-bit value - each bit selects a lane, 23-0
 * @param mode loopback mode
 * @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_write_loopback_mode(struct nfp_nbi_dev *nbi, uint32_t lanemask,
				    uint32_t mode);

/**
 * Read status of PHY loopbacks for a single lane
 * @ingroup nfp6000-only
 *
 * Each SerDes lane supports two system side loopbacks and two line-side
 * loopbacks.
 *
 * These loopbacks may be enabled/disabled in various combinations for
 * testing.  The loopback codes are defined in nfp_nbi_phy.h and can be
 * ORed together to enable multiple loopbacks on a per-lane basis.
 *
 * The return value will be the result of ORing the states of the four
 * loopbacks for the specified lane.
 *
 * @param nbi NBI device
 * @param lane integer lane number in range 0..23
 * @return loopback state on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_phy_read_loopback_mode(struct nfp_nbi_dev *nbi, int lane);

/// @cond
struct krstate {
	uint32_t eye;        /* current eye */
	int besteye;         /* best eye */
	int step;            /* algorithm step */
	int stepcnt;         /* step count */
	int bstep[3];        /* step count */
	int hyd;             /* ethernet core */
	int seg;             /* ethernet segment */
	int group;           /* serdes group */
	int lane;            /* serdes lane */
	int cmd[3];          /* command sent to LP inc, dec, hold C_1(P1), C0(LN), C1(M1) */
	int uprecd[3];       /* update received from LP inc, dec, hold C_1(P1), C0(LN), C1(M1) */
	int exp[3];          /* expect C_1(P1), C0(LN), C1(M1) */
	int ldcoeff[3];      /* local coeff: min, max, updated, not-updated */
	int lpcoeff[3];      /* partner coeff: min, max, updated, not-updated */
	int prelpcoeff[3];   /* previous partner coeff */
	int posn[3];         /* current C_1, C0, C1 coordinates */
	int reg[3];          /* local TXDRV_LEVNP1, TXDRV_LEVN, TXDRV_LEVNM1 cached values */
	int bestposn[3];     /* best eye C_1, C0, C1 coordinates phase 2 */
	int ldstate;         /* local training state */
	int lpstate;         /* partner training state */
	int lprr;            /* LP receiver ready count (300 frames) */
	int coutcnt;         /* Number of updates sent to LP */
	int cincnt;          /* Number of updates received from LP */
	int lock;            /* frame lock */
	int done;            /* complete */
	struct timeval tstart;      /* start time */
};

/*
 * coefficient update commands [ctl|m1|levn|p1] aka |c1|c0|c-1|
 * c1=x-axis (left/right), c0=z-axis (fwd/back) c_1=y-axis (up/down)
 * hold=0 inc=1, dec=2
 */
typedef enum crcmd {
	c_hold    = 0x00000000,
	c_up      = 0x00000001,
	c_down    = 0x00000002,
	c_right   = 0x00000010,
	c_left    = 0x00000020,
	c_rtup    = 0x00000011,
	c_lftup   = 0x00000021,
	c_rtdn    = 0x00000012,
	c_lftdn   = 0x00000022,
	c_fwd     = 0x00000004,
	c_back    = 0x00000008,
	c_init    = 0x00001000,
	c_pset    = 0x00002000,
	c_rxadapt = 0x10000000,
	c_eye     = 0x20000000,
	c_best    = 0x30000000,
	c_end     = 0x40000000,
	c_limit   = 0x50000000
} crcmd_t;

#define NFP_NBI_MAC_KR_BPAXIS_1(kr) (abs(kr->bestposn[0] - kr->posn[0]))
#define NFP_NBI_MAC_KR_BPAXIS0(kr) (abs(kr->bestposn[1] - kr->posn[1]))
#define NFP_NBI_MAC_KR_BPAXIS1(kr) (abs(kr->bestposn[2] - kr->posn[2]))
#define NFP_NBI_MAC_KR_BPAXIS(kr) (abs(abs(kr->bestposn[0] - kr->posn[0]) - \
				       abs(kr->bestposn[2] - kr->posn[2])))
#define NFP_NBI_MAC_KR_BPDIAG(kr) (abs(kr->bestposn[0] - kr->posn[0]) - \
				   NFP_NBI_MAC_KR_BPAXIS(kr))
#define NFP_NBI_MAC_KR_BPSTEPS(kr) (NFP_NBI_MAC_KR_BPAXIS(kr) + \
				    NFP_NBI_MAC_KR_BPDIAG(kr))

#define NFP_NBI_MAC_KR_COST(kr) (abs(kr->bestposn[0] - kr->posn[0]) + \
				 abs(kr->bestposn[2] - kr->posn[2]))

enum {NFP_NBI_PHY_FIBER, NFP_NBI_PHY_DAC}; /* PHY module type */

/* loop counters */
#define NFP_NBI_MAC_KR_NINIT 17         /* 17 */
#define NFP_NBI_MAC_KR_NCOARSE 3	/* 5 */
#define NFP_NBI_MAC_KR_NRESC1 1
#define NFP_NBI_MAC_KR_NRESC_1 1
#define NFP_NBI_MAC_KR_NFINEC_1 7
#define NFP_NBI_MAC_KR_NFINEC1 7
#define NFP_NBI_MAC_KR_EYE_ITER 10      /* 25 iterations */
#define NFP_NBI_MAC_KR_EYE_TIME 400     /* timeout * 5us */
#define NFP_NBI_MAC_KR_RXADAPT_TIME 150 /* timeout */
#define NFP_NBI_MAC_KR_LASTCALL 50000   /* time to exit uS */

/* Tuning Register limits */
#define NFP_NBI_MAC_KR_MAX_C_1 5        /* TXDRV_LEVNP1 (6)  */
#define NFP_NBI_MAC_KR_MAX_C0 31	/* TXDRV_LEVN (0x1f) */
#define NFP_NBI_MAC_KR_MAX_C1 8		/* TXDRV_LEVNM1 (9)  */

#define NFP_NBI_MAC_KR_MIN_C_1 1        /* TXDRV_LEVNP1      */
#define NFP_NBI_MAC_KR_MIN_C0  11	/* TXDRV_LEVN        */
#define NFP_NBI_MAC_KR_MIN_C1  1	/* TXDRV_LEVNM1      */

/* Preset */
#define NFP_NBI_MAC_KR_PRESET_C_1 NFP_NBI_MAC_KR_MIN_C_1
#define NFP_NBI_MAC_KR_PRESET_C0 NFP_NBI_MAC_KR_MAX_C0
#define NFP_NBI_MAC_KR_PRESET_C1 NFP_NBI_MAC_KR_MIN_C1

/* array indices */
#define NFP_NBI_MAC_KR_C_1 0
#define NFP_NBI_MAC_KR_C0 1
#define NFP_NBI_MAC_KR_C1 2

/* commands */
#define NFP_NBI_MAC_KR_HOLD 0x0
#define NFP_NBI_MAC_KR_INC 0x1
#define NFP_NBI_MAC_KR_DEC 0x2
#define NFP_NBI_MAC_KR_COEFF_DELTA(_x) ((_x == NFP_NBI_MAC_KR_DEC) ? -1 : (_x == NFP_NBI_MAC_KR_INC) ? + 1 : 0)

/* acknowledgements */
#define NFP_NBI_MAC_KR_NOT_UPDATED 0x0
#define NFP_NBI_MAC_KR_UPDATED 0x1
#define NFP_NBI_MAC_KR_MIN 0x2
#define NFP_NBI_MAC_KR_MAX 0x3

/* States */
enum nfp_nbi_mac_kr_states {
	NFP_NBI_MAC_KR_STATE_INIT,
	NFP_NBI_MAC_KR_STATE_FRAMELOCK,
	NFP_NBI_MAC_KR_STATE_HOLD,
	NFP_NBI_MAC_KR_STATE_ACK,
	NFP_NBI_MAC_KR_STATE_UPDATE,
	NFP_NBI_MAC_KR_STATE_CMD,
	NFP_NBI_MAC_KR_STATE_NOT_UPDATED,
	NFP_NBI_MAC_KR_STATE_UPDATED,
	NFP_NBI_MAC_KR_STATE_READY,
	NFP_NBI_MAC_KR_STATE_DONE,
	NFP_NBI_MAC_KR_STATE_TIMEOUT
};

int nfp_nbi_phy_tune_read(struct nfp_device *nfp, struct nfp_nbi_phy_config *pcfg);
int nfp_nbi_phy_kr_reginit(struct nfp_serdes *serdes);
int nfp_nbi_phy_kr_rxadapt(struct nfp_serdes *serdes, int group, int lane);
int nfp_nbi_phy_kr_ldcoeff_up(struct nfp_serdes *serdes, struct krstate *kr);
int nfp_nbi_phy_kr_ldcoeff_init(struct nfp_serdes *serdes, struct krstate *kr);
int nfp_nbi_phy_kr_ldcoeff_preset(struct nfp_serdes *serdes, struct krstate *kr);
int nfp_nbi_phy_kr_eye_read(struct nfp_nbi_dev *nbi, int group, int lane,
			    uint32_t *eye);
void nfp_nbi_phy_kr_print_tx(struct nfp_serdes *serdes, int group, int lane);

int nfp_nbi_phy_reinit_lane(struct nfp_serdes *serdes,
			    struct nfp_nbi_phy_params *params);
int nfp_nbi_phy_write_pcspstate(struct nfp_serdes *serdes, int group, int lane,
				int state);
int nfp_nbi_phy_read_xeye_weight(struct nfp_serdes *serdes, int group,
				 int *xeye_weight);

/// @endcond

#ifdef __cplusplus
}
#endif
#endif
