/*
 * Copyright (C) 2018 Netronome Systems, Inc. All rights reserved.
 * Author: Tony Egan <tony.egan@netronome.com>
 *
 */

#ifndef __NFP_NBI_MAC_PORT_CFG_H__
#define __NFP_NBI_MAC_PORT_CFG_H__

#define PORT_CFG_PORT_ANY 0x3d
#define PORT_CFG_PORT_ALL 0x3e
#define PORT_CFG_PORT_EOF 0x3f
#define PORT_CFG_FRAME_LENGTH 10240
#define PORT_CFG_CHAN_PAUSE_QUANTA 0xffff
#define PORT_CFG_CHAN_PAUSE_THRESH 0x5555
#define PORT_CFG_DQDWRR_WT_10G 256
#define PORT_CFG_DQDWRR_WT_40G 1024
#define PORT_CFG_DQDWRR_WT_100G 2560
#define PORT_CFG_DEFAULT_MRU 9216

enum {  PORT_CFG_ATTR_CHAN = 1,
	PORT_CFG_ATTR_PCPMAP30,
	PORT_CFG_ATTR_PCPMAP74,
	PORT_CFG_ATTR_PCPMAPCP,
	PORT_CFG_ATTR_PREPEND_IG,
	PORT_CFG_ATTR_PREPEND_EG,
	PORT_CFG_ATTR_ENA1,
	PORT_CFG_ATTR_ENA2,
	PORT_CFG_ATTR_PWM,
	PORT_CFG_ATTR_HWM,
	PORT_CFG_ATTR_WMCP,
	PORT_CFG_ATTR_CRAN,
	PORT_CFG_ATTR_SERDES,
	PORT_CFG_ATTR_NBI
};

#define PORT_CFG_PORT(_x) (((_x) & 0x3f) << 26)
#define PORT_CFG_PORT_of(_x) (((_x) >> 26) & 0x3f)
#define PORT_CFG_ATTR(_x) (((_x) & 0x3f) << 20)
#define PORT_CFG_ATTR_of(_x) (((_x) >> 20) & 0x3f)
#define PORT_CFG_MASK(_x) (((_x) & 0x3ff) << 10)
#define PORT_CFG_MASK_of(_x) (((_x) >> 10) & 0x3ff)
#define PORT_CFG_DATA(_x) (((_x) & 0x3ff) << 0)
#define PORT_CFG_DATA_of(_x) (((_x) >> 0) & 0x3ff)
#define PORT_CFG_VALUE_of(_x) (PORT_CFG_MASK_of(_x) & PORT_CFG_DATA_of(_x))

#define PORT_CFG_CHAN_BASE(_x) (((_x) & 0x3f) << 0)
#define PORT_CFG_CHAN_BASE_of(_x) (((_x) >> 0) & 0x3f)
#define PORT_CFG_CHAN_UNTAG(_x) (((_x) & 0x3f) << 6)
#define PORT_CFG_CHAN_UNTAG_of(_x) (((_x) >> 6) & 0x3f)
#define PORT_CFG_CHAN_NUM(_x) (((_x) & 0xf) << 12)
#define PORT_CFG_CHAN_NUM_of(_x) (((_x) >> 12) & 0xf)
#define PORT_CFG_CHAN_PCP_PORT0(_x) (((_x) & 0x1) << 19)
#define PORT_CFG_CHAN_PCP_PORT0_of(_x) (((_x) >> 19) & 0x1)

#define PORT_CFG_PCPMAP_NUM(_x) (((_x) & 0xf) << 12)
#define PORT_CFG_PCPMAP_NUM_of(_x) (((_x) >> 12) & 0xf)
#define PORT_CFG_PCPMAP(_x, _c) (((_x) & 0x7) << ((_c % 4) * 3))
#define PORT_CFG_PCPMAP_of(_x, _c) (((_x) >> ((_c % 4) * 3) & 0x7))
#define PORT_CFG_PCPMAP_PORT(_x) (((_x) & 0x3f) << 0)
#define PORT_CFG_PCPMAP_PORT_of(_x) (((_x) >> 0) & 0x3f)

#define PORT_CFG_PREPEND_IG_SKIP(_x) (((_x) & 0xf) << 0)
#define PORT_CFG_PREPEND_IG_SKIP_of(_x) (((_x) >> 0) & 0xf)
#define PORT_CFG_PREPEND_IG_DSA(_x) (((_x) & 0x3) << 4)
#define PORT_CFG_PREPEND_IG_DSA_of(_x) (((_x) >> 4) & 0x3)
#define PORT_CFG_PREPEND_IG_TIME BIT(6)
#define PORT_CFG_PREPEND_IG_PARSE BIT(7)

#define PORT_CFG_PREPEND_EG_SKIP(_x) (((_x) & 0xf) << 0)
#define PORT_CFG_PREPEND_EG_SKIP_of(_x) (((_x) >> 0) & 0xf)
#define PORT_CFG_PREPEND_EG_DSA(_x) (((_x) & 0x3) << 4)
#define PORT_CFG_PREPEND_EG_DSA_of(_x) (((_x) >> 4) & 0x3)
#define PORT_CFG_PREPEND_EG_CMD(_x) (((_x) & 0xff) << 6)
#define PORT_CFG_PREPEND_EG_CMD_of(_x) (((_x) >> 6) & 0xff)

#define PORT_CFG_ENA1_PROMIS BIT(0)
#define PORT_CFG_ENA1_CRC_FWD BIT(1)
#define PORT_CFG_ENA1_TX_PAD BIT(2)
#define PORT_CFG_ENA1_LENCHK_DISA BIT(3)
#define PORT_CFG_ENA1_PFC_MODE BIT(4)
#define PORT_CFG_ENA1_PAUSE_PFC_COMP BIT(5)
#define PORT_CFG_ENA1_EGRR BIT(6)
#define PORT_CFG_ENA1_SGMII BIT(7)
#define PORT_CFG_ENA1_PCSLOOP BIT(8)

#define PORT_CFG_ENA1_PROMIS_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(0) ? 1 : 0)
#define PORT_CFG_ENA1_CRC_FWD_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(1) ? 1 : 0)
#define PORT_CFG_ENA1_TX_PAD_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(2) ? 1 : 0)
#define PORT_CFG_ENA1_LENCHK_DISA_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(3) ? 1 : 0)
#define PORT_CFG_ENA1_PFC_MODE_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(4) ? 1 : 0)
#define PORT_CFG_ENA1_PAUSE_PFC_COMP_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(5) ? 1 : 0)
#define PORT_CFG_ENA1_EGRR_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(6) ? 1 : 0)
#define PORT_CFG_ENA1_SGMII_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(7) ? 1 : 0)
#define PORT_CFG_ENA1_PCSLOOP_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(8) ? 1 : 0)

#define PORT_CFG_ENA2_WAN_MODE BIT(0)
#define PORT_CFG_ENA2_PAUSE_FWD BIT(1)
#define PORT_CFG_ENA2_PAUSE_IGNORE BIT(2)
#define PORT_CFG_ENA2_ADDR_INS BIT(3)
#define PORT_CFG_ENA2_RX_CMDFRM BIT(4)
#define PORT_CFG_ENA2_RX_ERRDISC BIT(5)

#define PORT_CFG_ENA2_WAN_MODE_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(0) ? 1 : 0)
#define PORT_CFG_ENA2_PAUSE_FWD_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(1) ? 1 : 0)
#define PORT_CFG_ENA2_PAUSE_IGNORE_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(2) ? 1 : 0)
#define PORT_CFG_ENA2_ADDR_INS_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(3) ? 1 : 0)
#define PORT_CFG_ENA2_RX_CMDFRM_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(4) ? 1 : 0)
#define PORT_CFG_ENA2_RX_ERRDISC_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(5) ? 1 : 0)

#define PORT_CFG_PWM(_x) (((_x) & 0x7ff) << 0)
#define PORT_CFG_PWM_of(_x) (((_x) >> 0) & 0x7ff)
#define PORT_CFG_PWM_CHAN(_x) (((_x) & 0x7f) << 12)
#define PORT_CFG_PWM_CHAN_of(_x) (((_x) >> 12) & 0x7f)

#define PORT_CFG_HWM(_x) (((_x) & 0x7ff) << 0)
#define PORT_CFG_HWM_of(_x) (((_x) >> 0) & 0x7ff)
#define PORT_CFG_HWM_DROP(_x) (((_x) & 0x1f) << 11)
#define PORT_CFG_HWM_DROP_of(_x) (((_x) >> 11) & 0x1f)

#define PORT_CFG_WMCP_HWM(_x) (((_x) & 0x1) << 19)
#define PORT_CFG_WMCP_HWM_of(_x) (((_x) >> 19) & 0x1)
#define PORT_CFG_WMCP_PORT(_x) (((_x) & 0x3f) << 0)
#define PORT_CFG_WMCP_PORT_of(_x) (((_x) >> 0) & 0x3f)

#define PORT_CFG_CRAN_KX_AUTONEG BIT(0)
#define PORT_CFG_CRAN_KX_LINK_TRN BIT(1)
#define PORT_CFG_CRAN_KX_FEC_ABL BIT(2)
#define PORT_CFG_CRAN_KX_FEC_REQ BIT(3)

#define PORT_CFG_CRAN_KX_AUTONEG_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(0) ? 1 : 0)
#define PORT_CFG_CRAN_KX_LINK_TRN_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(1) ? 1 : 0)
#define PORT_CFG_CRAN_KX_FEC_ABL_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(2) ? 1 : 0)
#define PORT_CFG_CRAN_KX_FEC_REQ_of(_x) (PORT_CFG_VALUE_of(_x) & BIT(3) ? 1 : 0)

#define PORT_CFG_SERDES_ENA(_x) (((_x) & 0x1) << 0)
#define PORT_CFG_SERDES_ENA_of(_x) (((_x) >> 0) & 0x1)
#define PORT_CFG_SERDES_XEYE(_x) (((_x) & 0xf) << 1)
#define PORT_CFG_SERDES_XEYE_of(_x) (((_x) >> 1) & 0xf)
#define PORT_CFG_SERDES_YEYE(_x) (((_x) & 0xf) << 5)
#define PORT_CFG_SERDES_YEYE_of(_x) (((_x) >> 5) & 0xf)
#define PORT_CFG_SERDES_BITRATE(_x) (((_x) & 0x7) << 9)
#define PORT_CFG_SERDES_BITRATE_of(_x) (((_x) >> 9) & 0x7)
#define PORT_CFG_SERDES_NUM(_x) (((_x) & 0xf) << 16)
#define PORT_CFG_SERDES_NUM_of(_x) (((_x) >> 16) & 0xf)

#define PORT_CFG_NBI_NUM(_x) (((_x) & 0x1) << 19)
#define PORT_CFG_NBI_NUM_of(_x) (((_x) >> 19) & 0x1)

int nfp_nbi_mac_apply_port_cfg(struct nfp_device *nfp, struct nfp_nbi_mac_config *mcfg, const uint32_t *pcfg);
int nfp_nbi_mac_phy_cfg(struct nfp_device *nfp, struct nfp_nbi_mac_config *mcfg, const char *phy);
int nfp_nbi_mac_phy_cfg_nbi(struct nfp_device *nfp, struct nfp_nbi_mac_config *mcfg, int nbi);
void nfp_nbi_mac_ncsi_cfg(struct nfp_device *nfp, struct nfp_nbi_mac_config *mcfg);

#endif
