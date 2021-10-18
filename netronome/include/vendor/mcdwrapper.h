/**
 * Copyright (C) 2016 Netronome Systems, Inc.  All rights reserved.
 *
 * @file mcdwrapper.h
 * Wrapper functions for the Marvell mcd PHY library.
 *
 */
#ifndef MCDWRAPPER_H
#define MCDWRAPPER_H

#define MCDDS_SIZE 280

typedef struct {
	struct nfp_device *nfp;
	uint8_t mcdds[MCDDS_SIZE];
	int port;
} mcd_host_context_s;

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

#define MCD_MOD_ANEG BIT(4)
#define MCD_MOD_FFEC BIT(6)
#define MCD_MOD_RSFEC BIT(7)
#define MCD_MOD_SPEED_MASK (7)
#define MCD_MOD_1G (0x0)
#define MCD_MOD_10G (0x3)
#define MCD_MOD_25G (0x4)
#define MCD_MOD_40G (0x5)
#define MCD_MOD_50G (0x6)
#define MCD_PCS_LINK BIT(2)
#define MCD_BPA_10KR BIT(3)
#define MCD_BPA_FFEC BIT(4)
#define MCD_BPA_RSFEC BIT(7)
#define MCD_BPA_25CR BIT(13)
#define MCD_CPA_25CR BIT(5)
#define MCD_ADV1_PF  BIT(10)

typedef struct {
	uint16_t mode_select;
	uint16_t pcs_status;
	uint16_t bpaneg_status;
	uint16_t caneg_status;
	uint16_t advertise1;
	uint16_t linkpartner1;
} mcd_port_status_s;

typedef struct {
	uint32_t rs4_corrected;
	uint32_t rs4_uncorrected;
	uint32_t rs4_symbol;
	uint16_t rs4_BIP;
	uint32_t tbr_corrected;
	uint32_t tbr_uncorrected;
	uint32_t br_corrected;
	uint32_t br_uncorrected;
	uint32_t rs_corrected;
	uint32_t rs_uncorrected;
	uint32_t rs_symbol;
} mcd_fec_counters_s;

typedef enum {
	NFP_MCD_NONE,
	NFP_MCD_P10LN,
	NFP_MCD_P10LF,
	NFP_MCD_P10KN,
	NFP_MCD_P10KF,
	NFP_MCD_P25CN,
	NFP_MCD_P25CF,
	NFP_MCD_P25CR,
	NFP_MCD_P25BN,
	NFP_MCD_P25BF,
	NFP_MCD_P25BR,
	NFP_MCD_P25LN,
	NFP_MCD_P25LF,
	NFP_MCD_P25LR,
	NFP_MCD_P1X,
	NFP_MXD_P50MN,
	NFP_MXD_P50MF,
	NFP_MXD_P50MR,
	NFP_MXD_P50BN,
	NFP_MXD_P50BF,
	NFP_MXD_P50BR,
	NFP_MXD_P40LN
} NFP_MCD_MODES;

int mcd_init(struct nfp_device *nfp, int port, NFP_MCD_MODES mode,
	     int firmware);
int mcd_link_propagation(struct nfp_device *nfp, int port, int propagation);
int mcd_set_mode(mcd_host_context_s *mcdhc, NFP_MCD_MODES mode);
int mcd_read_mode(struct nfp_device *nfp, int port, NFP_MCD_MODES *mode);
const char *mcd_mode_description(NFP_MCD_MODES mode);
int mcd_read_status(struct nfp_device *nfp, int port,
		    mcd_port_status_s *status);
int mcd_read_rmfifo_drops(struct nfp_device *nfp, int port, uint16_t *drops,
			  uint16_t *errs);
int mcd_ifup(struct nfp_device *nfp, int port);
int mcd_ifdown(struct nfp_device *nfp, int port);
int mcd_fec_counters(struct nfp_device *nfp, int port,
		     mcd_fec_counters_s *counters);
int mcd_search_mode(struct nfp_device *nfp, int port, int first);

int mcd_open(struct nfp_device *nfp, mcd_host_context_s *mcdhc, int port);

int mcd_set_loopback(mcd_host_context_s *mcdhc, uint16_t portside,
		     uint16_t lanes, uint16_t loopback, uint16_t enable);
int mcd_get_loopback(mcd_host_context_s *mcdhc, uint16_t portside,
		     uint16_t lanes, uint16_t loopback, uint16_t *enable);

int mcd_get_rev(mcd_host_context_s *mcdhc);

int mcd_serdes_reset(mcd_host_context_s *mcdhc, uint16_t portside, uint16_t lanes);

int mcd_set_txval(mcd_host_context_s *mcdhc, uint16_t portside, uint16_t lanes,
		  int16_t prec, int16_t att, int16_t postc, uint16_t reset);

int mcd_get_txval(mcd_host_context_s *mcdhc, uint16_t portside,
		  uint16_t lanes);

int mcd_set_prbs(mcd_host_context_s *mcdhc, uint16_t portside, uint16_t lanes);
int mcd_txrx_prbs_en(mcd_host_context_s *mcdhc, uint16_t portside,
		     uint16_t lanes, uint16_t enable);
int mcd_get_prbs_counters(mcd_host_context_s *mcdhc, uint16_t portside,
			  uint16_t lanes);
int mcd_fiber_sfp28_wa(struct nfp_device *nfp, int port);

/*Serdes Library Calls*/
int mcds_get_eye(mcd_host_context_s *mcdhc, uint16_t portside, uint16_t lanes,
		 uint16_t dbg_lvl);

int mcds_get_rxval(mcd_host_context_s *mcdhc, uint16_t portside,
		   uint16_t lanes);

int mcds_get_eye_metric(mcd_host_context_s *mcdhc, uint16_t portside,
			uint16_t lanes);

int mcds_get_serdes_dump(mcd_host_context_s *mcdhc, uint16_t portside,
			 uint16_t lanes);

int mcds_get_txval(mcd_host_context_s *mcdhc, uint16_t portside,
		   uint16_t lanes);

int mcds_set_txval(mcd_host_context_s *mcdhc, uint16_t portside,
		   uint16_t lanes, int16_t prec, int16_t att, int16_t postc,
		   int16_t slew);

int mcds_set_txprbs(mcd_host_context_s *mcdhc, uint16_t portside,
		    uint16_t lanes);

int mcds_get_txprbs(mcd_host_context_s *mcdhc, uint16_t portside,
		    uint16_t lanes);

int mcds_set_rxprbs(mcd_host_context_s *mcdhc, uint16_t portside,
		    uint16_t lanes);

int mcds_get_rxprbs(mcd_host_context_s *mcdhc, uint16_t portside,
		    uint16_t lanes);

int mcds_injecterr(mcd_host_context_s *mcdhc, uint16_t portside,
		   uint16_t lanes);

int mcds_get_errcnt(mcd_host_context_s *mcdhc, uint16_t portside,
		    uint16_t lanes);

int mcds_set_lpbk(mcd_host_context_s *mcdhc, uint16_t portside, uint16_t lanes,
		  uint16_t lpbk);

int mcds_reset(mcd_host_context_s *mcdhc, uint16_t portside, uint16_t lanes);

#endif
