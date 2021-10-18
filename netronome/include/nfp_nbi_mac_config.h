/**
 * Copyright (C) 2013-2017 Netronome Systems, Inc.  All rights reserved.
 * Author: Tony Egan <tony.egan@netronome.com>
 *
 * @file nfp_nbi_mac_config.h
 * nfp6000 MAC configuration structures and functions
 *
 */

#ifndef __NFP_NBI_MAC_CONFIG_H__
#define __NFP_NBI_MAC_CONFIG_H__

#include <stdio.h>

#include "nfp_nbi_mac_misc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * MAC Channel configuration.
 *
 * The nfp_nbi_mac_chan_config structure is used to configure global MAC
 * settings and the parameters relating to channels. The MAC supports
 * up to 128 channels in the interface to the NBI. All active Ethernet
 * and Interlaken ports must be mapped to unique channels as part of
 * initialization (see nfp_nbi_mac_eth_write_portchan,
 * nfp_nbi_mac_ilk_write_portchan).  Each channel has it's own settings
 * for pause watermark and command prepend.
 *
 * The global settings oobfc_0_511_ena and oobfc_512_1023_ena relate
 * to Traffic Manager out-of-band flow control, a feature that
 * requires external hardware support.  The full_config and split_mem
 * parameters are used to control the behavior of macinit.
 *
 */
struct nfp_nbi_mac_chan_config {
	/**
	 * Mask to enable/disable command prepend on egress channels 0-31
	 */
	unsigned int egress_cmd_prepend_mask31_0;
	/**
	 * Mask to enable/disable command prepend on egress channels 32-63
	 */
	unsigned int egress_cmd_prepend_mask63_32;
	/**
	 * Mask to enable/disable command prepend on egress channels 64-95
	 */
	unsigned int egress_cmd_prepend_mask95_64;
	/**
	 * Mask to enable/disable command prepend on egress channels 96-127
	 */
	unsigned int egress_cmd_prepend_mask127_96;
	/**
	 * Watermark at which pause frames will be sent for channels 0-31
	 */
	int pause_wm00_31[32];
	/**
	 * Watermark at which pause frames will be sent for channels 32-63
	 */
	int pause_wm32_63[32];
	/**
	 * Watermark at which pause frames will be sent for channels 64-95
	 */
	int pause_wm64_95[32];
	/**
	 * Watermark at which pause frames will be sent for channels 96-127
	 */
	int pause_wm96_127[32];
	/**
	 * Enable/disable out-of-band flow control for Traffic Manager queues
	 * 0-511
	 */
	int oobfc_0_511_ena;
	/**
	 * Enable/disable out-of-band flow control for Traffic Manager queues
	 * 511-1023
	 */
	int oobfc_512_1023_ena;
	/**
	 * Configure both core pairs in the MAC
	 */
	int full_config;
	/**
	 * Divide the packet memory between the two core pairs
	 */
	int split_mem;
};

/**
 * MAC Ethernet port configuration
 *
 * The nfp_nbi_mac_enet_config structure specifies the parameters for
 * configuring a MAC Ethernet port.  These parameters affect all
 * channels mapped to the port.
 *
 */
struct nfp_nbi_mac_enet_config {
	/**
	 * Port number
	 */
	int port;
	/**
	 * Hardware MAC address
	 */
	int mac_addr;
	/**
	 * Port speed [1G, 10G, 40G, 100G]
	 */
	int lane_mode;
	/**
	 * Enabled/disabled state
	 */
	int enabled;
	/**
	 * Base channel [0-64] assigned to the port
	 */
	int channel_base;
	/**
	 * Number of channels assigned to the port - max 8 for Ethernet
	 * corresponding to 8 PCP classes.
	 */
	int num_channels;
	/**
	 * Channel remap assignments, mapping 8 classes into fewer channels
	 */
	int pcp_remap[8];
	/**
	 * Channel used for untagged packets when in PFC mode.
	 */
	int untagged_chan;
	/**
	 * Enable/disable Priority Flow Control (PFC) mode
	 */
	int pfc_mode;
	/**
	 * PFC time quanta sent in any pause frames for the associated class
	 */
	int pause_quanta[8];
	/**
	 * PFC pause refresh thresholds for each class
	 */
	int pause_thresh[8];
	/**
	 * Watermark at which pause frames (XOFF) will be sent for all channels
	 * on the port
	 */
	int highwater_mark;
	/**
	 * Exceeding the high watermark plus this drop delta will result in
	 * packet drops
	 */
	int drop_delta;
	/**
	 * Specifies the DSA tagging mode on ingress for the port
	 */
	int ingress_dsa_tag_mode;
	/**
	 * Specifies the DSA tagging mode on egress for the port
	 */
	int egress_dsa_tag_mode;
	/**
	 * Number of bytes reserved for switch support prepend on ingress
	 */
	int ingress_parser_skip_bytes;
	/**
	 * Number of bytes reserved for switch support prepend on egress
	 */
	int egress_parser_skip_bytes;
	/**
	 * Enable/disable prepending the parse result to packets on ingress
	 */
	int ingress_parse_prepend_en;
	/**
	 * Enable/disable prepending the timestamp to packets on ingress
	 */
	int ingress_timestamp_prepend_en;
	/**
	 * Maximum frame length.  Frames exceeding this will be dropped.
	 */
	int frame_length;
	/**
	 * DWRR weight assigned to the port for dequeuing packets through the
	 * NBI.
	 */
	int ig_dq_dwrr_weight;
	/**
	 * Enable/disable round robin dequeuing of this port's channels on
	 * egress
	 */
	int egress_port_rr;
	/**
	 * Enable/disable checking of payload length against the frame's
	 * Length/Type field
	 */
	int length_check;
	/**
	 * Enable/disable forwarding of the CRC field of received frames to the
	 * application.
	 */
	int crc_fwd;
	/**
	 * Enable/disable forwarding of received pause frames to the
	 * application.
	 */
	int pause_fwd;
	/**
	 * Ignore/obey received pause frames.  PFC mode only,
	 */
	int pause_ignore;
	/**
	 * Enable/disable PFC compatible mode when in Link Pause Mode
	 */
	int pause_pfc_comp;
	/**
	 * Enable/disable system side (Tx to Rx) loopback at the PCS
	 */
	int pcs_loopback;
	/**
	 * Enable/disable promiscuous mode - when disabled, frames are dropped
	 * if destination address doesn't match port's MAC address.
	 */
	int promis_en;
	/**
	 * Enable/disable WAN mode of operation - only used in 10G.
	 */
	int wan_mode;
	/**
	 * Set the module type for tuning purposes.
	 */
	char phy_module[24];
	/// @cond
	int sgmii_ena;
	int pad_en; /* implemented? */
	int kx_autoneg_enable;
	int kx_autoneg_link_training;
	int kx_autoneg_nonce;
	int kx_fec_ability;
	int kx_fec_requested;
	int link_timer;
	/// @endcond
};

/**
 * MAC Interlaken transmit module configuration
 */
struct nfp_nbi_mac_ilk_tx_config {
	/**
	 * Maximum burst size
	 */
	int burst_max;
	/**
	 * Minimum burst size
	 */
	int burst_min;
	/**
	 * Short burst size
	 */
	int burst_short;
	/**
	 * 12-bit mask to disable/enable lanes
	 */
	unsigned int lane_disable;
	/**
	 * Metaframe period
	 */
	int metaframe_period;
	/**
	 * Enable/disable out-of-band band flow control
	 */
	int outband;

	/// @cond
	int mult_ch;
	int mult_ext;
	int mult_fc;
	int scram_dis;
	int skip_dis;
	int inband;
	/// @endcond
};

/**
 * MAC Interlaken receive module configuration
 *
 */
struct nfp_nbi_mac_ilk_rx_config {
	/**
	 * Maximum burst size
	 */
	int burst_max;
	/**
	 * Minimum burst size
	 */
	int burst_min;
	/**
	 * Short burst size
	 */
	int burst_short;
	/**
	 * 12-bit mask to disable/enable lanes
	 */
	unsigned int lane_disable;
	/**
	 * Metaframe period
	 */
	int metaframe_period;
	/**
	 * Enable/disable out-of-band band flow control
	 */
	int outband;

	/// @cond
	int mult_ch;
	int mult_ext;
	int mult_fc;
	int scram_dis;
	int skip_dis;
	int inband;
	/// @endcond
};

/**
 * MAC Interlaken core configuration
 */
struct nfp_nbi_mac_ilk_config {
	/**
	 * Interlaken core [0,1]
	 */
	int ilk;
	/**
	 * Number of active Interlaken lanes - determines bandwidth
	 */
	int num_lanes;
	/**
	 * Enabled/disabled state
	 */
	int enabled;
	/**
	 * Base channel [0-127] assigned to the port
	 */
	int channel_base;
	/**
	 * Number of channels assigned to the port - max 64 in memory split
	 * mode, 128 in un-split mode
	 *
	 */
	int num_channels;
	/**
	 * Watermark at which pause frames (XOFF) will be sent for all channels
	 * on the port
	 */
	int highwater_mark;
	/**
	 * Exceeding the high watermark plus this drop delta will result in
	 * packet drops
	 */
	int drop_delta;
	/**
	 * Specifies the DSA tagging mode on ingress for the port
	 */
	int ingress_dsa_tag_mode;
	/**
	 * Specifies the DSA tagging mode on egress for the port
	 */
	int egress_dsa_tag_mode;
	/**
	 * Number of bytes reserved for switch support prepend on ingress
	 */
	int ingress_parser_skip_bytes;
	/**
	 * Number of bytes reserved for switch support prepend on egress
	 */
	int egress_parser_skip_bytes;
	/**
	 * Enable/disable prepending the parse result to packets on ingress
	 */
	int ingress_parse_prepend_en;
	/**
	 * Enable/disable prepending the timestamp to packets on ingress
	 */
	int ingress_timestamp_prepend_en;
	/**
	 * DWRR weight assigned to the port for dequeuing packets through the
	 * NBI.
	 */
	int ig_dq_dwrr_weight;
	/**
	 * Enable/disable segmented mode dequeuing (RR thru channels)
	 * instead of packet mode dequeuing (dwell on channel until EOP)
	 */
	int ig_dq_segmented;
	/**
	 * Enable/disable Interlaken Lookaside mode (Core 1 only).
	 */
	int la_mode;
	/**
	 * Enable Lookaside 2-channel operation - default is 4 channels.
	 */
	int la_2chan;
	/**
	 * Enable/disable line side (Rx to Tx) loopback
	 */
	int loopback_line;
	/**
	 * Enable/disable system side (Tx to Rx) loopback
	 */
	int loopback_sys;
	/**
	 * MAC Interlaken transmit module configuration
	 *
	 */
	struct nfp_nbi_mac_ilk_tx_config tx;
	/**
	 * MAC Interlaken receive module configuration
	 *
	 */
	struct nfp_nbi_mac_ilk_rx_config rx;
	/// @cond
	int outband;
	int packet_mode;
	/// @endcond
};

/**
 * MAC SerDes configuration
 */
struct nfp_nbi_mac_serdes_config {
	/**
	 * Serdes identifier [0-5]
	 */
	int serdes;
	/**
	 * Enabled/disabled state
	 */
	int enabled;
	/**
	 * Bit rate
	 */
	int bit_rate;
	/**
	 * Clock multiplexer setting
	 */
	int clk_muxsel;
	/**
	 * rx adapt x eye weight
	 */
	int xeye_weight;
	/**
	 * rx adapt y eye weight
	 */
	int yeye_weight;
};

/**
 * Full MAC configuration
 */
struct nfp_nbi_mac_config {
	int nbiid;
	/**
	 * Channel and global configuration
	 */
	struct nfp_nbi_mac_chan_config chan_config;
	/**
	 * Ethernet port configuration
	 */
	struct nfp_nbi_mac_enet_config enet_config[24];
	/**
	 * Interlaken core configuration
	 */
	struct nfp_nbi_mac_ilk_config ilk_config[2];
	/**
	 * Serdes configuration
	 */
	struct nfp_nbi_mac_serdes_config serdes_config[6];
};

/**
 * Initialize MAC configuration structure from NFP registers
 * @ingroup nfp6000-only
 *
 * This function initializes a MAC configuration structure by reading
 * the NFP registers and extracting the default values.
 *
 *
 *  @param nbi NBI handle
 *  @param mcfg MAC configuration structure
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_init_config(struct nfp_nbi_dev *nbi,
			    struct nfp_nbi_mac_config *mcfg);

/**
 * Read MAC channel configuration registers into configuration structure.
 * @ingroup nfp6000-only
 *
 * This function initializes a MAC configuration structure by reading
 * default values from the NFP channel configuration registers.
 *
 *
 *  @param nbi NBI handle
 *  @param ccfg MAC channel configuration structure
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_chan_read_config(struct nfp_nbi_dev *nbi,
				 struct nfp_nbi_mac_chan_config *ccfg);

/**
 * Read MAC Ethernet configuration registers into configuration structure.
 * @ingroup nfp6000-only
 *
 * This function initializes a MAC configuration structure by reading
 * default values from the NFP Ethernet configuration registers for all ports.
 *
 *
 *  @param nbi NBI handle
 *  @param mcfg MAC Ethernet configuration structure
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_enet_read_config(struct nfp_nbi_dev *nbi,
				 struct nfp_nbi_mac_config *mcfg);

/**
 * Read MAC Ethernet port configuration registers into configuration structure.
 * @ingroup nfp6000-only
 *
 * This function initializes a MAC configuration structure by reading
 * default values from the NFP Ethernet configuration registers for a single
 * port.
 *
 *
 *  @param nbi NBI handle
 *  @param ecfg MAC Ethernet port configuration structure
 *  @param port MAC ethernet port: [0-23]
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_enet_read_port_config(struct nfp_nbi_dev *nbi,
				      struct nfp_nbi_mac_enet_config *ecfg,
				      int port);

/**
 * Read MAC Interlaken transmit configuration registers into a
 * configuration structure.
 * @ingroup nfp6000-only
 *
 * This function initializes a MAC configuration structure by reading
 * default values from the NFP Interlaken configuration registers for transmit.
 *
 *
 *  @param nbi NBI handle
 *  @param txcfg MAC Interlaken transmit configuration structure
 *  @param core Interlaken core: [0-1]
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_read_tx_config(struct nfp_nbi_dev *nbi,
				   struct nfp_nbi_mac_ilk_tx_config *txcfg,
				   int core);

/**
 * Read MAC Interlaken receive registers into a configuration structure.
 * @ingroup nfp6000-only
 *
 * This function initializes a MAC configuration structure by reading
 * default values from the NFP Interlaken configuration registers for receive.
 *
 *
 *  @param nbi NBI handle
 *  @param rxcfg MAC Interlaken receive configuration structure
 *  @param core Interlaken core: [0-1]
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_read_rx_config(struct nfp_nbi_dev *nbi,
				   struct nfp_nbi_mac_ilk_rx_config *rxcfg,
				   int core);

/**
 * Read MAC Interlaken configuration registers into a configuration structure.
 * @ingroup nfp6000-only
 *
 * This function initializes a MAC configuration structure by reading
 * default values from the NFP Interlaken configuration registers.
 *
 *
 *  @param nbi NBI handle
 *  @param icfg MAC Interlaken configuration structure
 *  @param core Interlaken core: [0-1]
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_read_config(struct nfp_nbi_dev *nbi,
				struct nfp_nbi_mac_ilk_config *icfg,
				int core);

/**
 * Dump MAC configuration to a file in JSON format.
 * @ingroup nfp6000-only
 *
 * This function outputs a MAC configuration to a specified file in JSON format.
 *
 *
 *  @param fp File pointer
 *  @param mcfg MAC configuration structure
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_dump_config(FILE *fp, struct nfp_nbi_mac_config *mcfg);

/**
 * Dump MAC channel configuration to a file in JSON format.
 * @ingroup nfp6000-only
 *
 * This function outputs a MAC channel configuration to a specified
 * file in JSON format.
 *
 *
 *  @param fp File pointer
 *  @param ccfg MAC channel configuration structure
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_chan_dump_config(FILE *fp,
				 struct nfp_nbi_mac_chan_config *ccfg);

/**
 * Dump MAC configuration for all Ethernet ports to a file in JSON format.
 * @ingroup nfp6000-only
 *
 * This function outputs a MAC Ethernet configuration to a specified
 * file in JSON format.
 *
 *
 *  @param fp File pointer
 *  @param ecfg MAC Ethernet configuration structure
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_enet_dump_config(FILE *fp,
				 struct nfp_nbi_mac_enet_config *ecfg);

/**
 * Dump MAC Ethernet configuration for one Ethernet port to a file in
 * JSON format.
 * @ingroup nfp6000-only
 *
 * This function outputs a MAC Ethernet port configuration to a
 * specified file in JSON format.
 *
 *
 *  @param fp File pointer
 *  @param ecfg MAC Ethernet port configuration structure
 *  @param port MAC ethernet port: [0-23]
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_enet_dump_port_config(FILE *fp,
				      struct nfp_nbi_mac_enet_config *ecfg,
				      int port);

/**
 * Dump MAC Interlaken transmit configuration to a file in JSON format.
 * @ingroup nfp6000-only
 *
 * This function outputs a MAC Interlaken transmit configuration to a
 * specified file in JSON format.
 *
 *
 *  @param fp File pointer
 *  @param txcfg MAC Interlaken transmit configuration structure
 *  @param core Interlaken core: [0-1]
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_dump_tx_config(FILE *fp,
				   struct nfp_nbi_mac_ilk_tx_config *txcfg,
				   int core);

/**
 * Dump MAC Interlaken receive configuration to a file in JSON format.
 * @ingroup nfp6000-only
 *
 * This function outputs a MAC Interlaken receive configuration to a
 * specified file in JSON format.
 *
 *
 *  @param fp File pointer
 *  @param rxcfg MAC Interlaken receive configuration structure
 *  @param core Interlaken core: [0-1]
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_dump_rx_config(FILE *fp,
				   struct nfp_nbi_mac_ilk_rx_config *rxcfg,
				   int core);

/**
 * Dump MAC Interlaken configuration to a file in JSON format.
 * @ingroup nfp6000-only
 *
 * This function outputs a MAC Interlaken configuration to a specified
 * file in JSON format.
 *
 *
 *  @param fp File pointer
 *  @param icfg MAC Interlaken configuration structure
 *  @param core Interlaken core: [0-1]
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_ilk_dump_config(FILE *fp,
				struct nfp_nbi_mac_ilk_config *icfg,
				int core);

/**
 * Dump MAC SerDes block configuration to a file in JSON format.
 * @ingroup nfp6000-only
 *
 * This function outputs a MAC SerDes block configuration to a
 * specified file in JSON format.
 *
 *
 *  @param fp File pointer
 *  @param scfg MAC SerDes configuration structure
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_serdes_dump_block_config(FILE *fp,
					 struct nfp_nbi_mac_serdes_config *scfg);

/**
 * Dump MAC SerDes configuration to a file in JSON format.
 * @ingroup nfp6000-only
 *
 * This function outputs a MAC SerDes configuration to a specified
 * file in JSON format.
 *
 *
 *  @param fp File pointer
 *  @param scfg MAC SerDes configuration structure
 *  @param core Ethernet core: [0-1]
 *
 *  @return 0 on success. Set errno and return -1 on error.
 */
NFP_NBI_API
int nfp_nbi_mac_serdes_dump_config(FILE *fp,
				   struct nfp_nbi_mac_serdes_config *scfg,
				   int core);

#ifdef __cplusplus
}
#endif

#endif
