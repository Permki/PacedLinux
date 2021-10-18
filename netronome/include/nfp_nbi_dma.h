/**
 * Copyright (C) 2013-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_nbi_dma.h
 * nfp6000 NFP NBI DMA API functions
 *
 */

#ifndef __NFP_NBI_DMA_H__
#define __NFP_NBI_DMA_H__

#include <stdint.h>

#include <nfp_nbi.h>

/**
 * NFP NBI DMA configuration
 * Configurations parameters for nfp nbi dma functional aspects ctm
 * polling, credit rate, error enable/disable and push bus select.
 */
struct nfp_nbi_dma_config {
	/**
	 * Controls how CTM polling is performed. When set the CTM
	 * polling logic skips to the next active CTM every poll interval.
	 * When clear the CTM polling logic increments the CTM counter by
	 * one every poll.
	 */
	int ctm_poll_search_enable;
	/**
	 * Selects whether to limit the bandwidth on the four Push Buses.
	 */
	int rate_limit_enable;
	/**
	 * Configures the amount of time between packet allocation poll
	 * commands to a CTM. If CtmPollSearchEna = 0, the polling
	 * interval is defined by the CtmPollIntvl. If the
	 * CtmPollSearchEna = 1, the poll interval is defined by the
	 * following equation: (CtmPollIntvl/64)*(# of active CTMs).
	 */
	int ctm_poll_interval;
	/**
	 * Configures whether the polling function is enabled or disabled
	 */
	int ctm_poll_enable;
	/**
	 * This field is placed in the 3 lsbits of the Packet Allocation
	 * Command Address field. It is used by the CTM to identify which
	 * NBI sent the command.
	 */
	int nbi_number;
	/**
	 * Controls how the hardware responds to a push bus input error for
	 * BLQ writes transactions.
	 */
	int dis_rx_blq_wr_in_err;
	/**
	 * Controls how the hardware responds to a push bus input error for
	 * allocation response transactions.
	 */
	int dis_rx_alloc_in_err;
	/**
	 * Controls how the hardware responds to a push bus input protocol
	 * error for allocation response transactions
	 */
	int dis_rx_push_last_err;
	/**
	 * Controls how the hardware forwards the received NBI Buffer
	 * Memory Read Error to the output data buses.
	 */
	int dis_buf_rd_err;
	/**
	 * Controls how the BLQ logic handles multi-bit ECC errors received
	 * from the Buffer Descriptor RAM.
	 */
	int dis_bd_ram_err;
	/**
	 * Controls how the DMA engine handles multi-bit ECC errors
	 * received from the DMA State RAM.
	 */
	int dis_ds_ram_err;
	/**
	 * Controls how the DMA engine handles multi-bit ECC errors
	 * received from the Buffer Completion RAM.
	 */
	int dis_bc_ram_err;
	/**
	 * Disables a push bus from being selected by the read arbiter when
	 * set. Used to force packet traffic to not use a particular push
	 * bus.
	 */
	int dis_push_bus_select;
	/**
	 * Sets the credit rate for push bus 3. Values are in 4 cycle
	 * increments.
	 */
	int credit_rate0;
	/**
	 * Sets the credit rate for push bus 2. Values are in 4 cycle
	 * increments.
	 */
	int credit_rate1;
	/**
	 * Sets the credit rate for push bus 1. Values are in 4 cycle
	 * increments.
	 */
	int credit_rate2;
	/**
	 * Sets the credit rate for push bus 0. Values are in 4 cycle
	 * increments.
	 */
	int credit_rate3;
	/**
	 * Set lookahead enable/disable with its buffer credit
	 * threshold and outstanding 256B credits threshold. Used by
	 * nfp38xx (ignored by nfp6xxx).
	 */
	int alloc_lookahead_enable;
	int buffer_credit_threshold;
	int outstanding_256B_credits_threshold;
};

/**
 * NFP NBI DMA Buffer Pool configuration
 * Buffer pool specifies which buffers to be used using Buffer Lists
 * and the destination for incoming packets using Buffer Pool Entries
 */
struct nfp_nbi_dma_bp_config {
	/** Buffer Pool Number*/
	int bp;
	/** Enable Packet Drops*/
	int drop_enable;
	/** Set CTM offset 0(4B) or 1(8B)*/
	int ctm_offset;
	/** BLQ number for primary list*/
	int primary_buffer_list;
	/** BLQ number for secondary list*/
	int secondary_buffer_list;
	/**
	 * CTM split length to be used for packet dma
	 * 0(256B),1(512B),2(1024B),3(2048B)
	 */
	int ctm_split_length;
	/** BPE number marking the start of chain*/
	int bpe_head;
	/** BPE number marking the end of chain*/
	int bpe_chain_end;
	/** Pointer to array of BPE configs*/
	struct nfp_nbi_dma_bpe_config *bpe_chain;
};

/**
 * NFP NBI DMA Buffer Pool Entry configuration
 * Buffer pool entry specifies the target island for a buffer pool
 * and the credits that monitor buffer usage and number of packets
 * received by target island.
 */
struct nfp_nbi_dma_bpe_config {
	/** Buffer Pool Entry Number */
	int bpe;
	/** CTM target island id*/
	int ctm_target;
	/** Number of packet credits for the target*/
	int packet_credits;
	/** Number of buffer credits for the target*/
	int buffer_credits;
};

/**
 * NFP NBI DMA Buffer List Queue configuration
 * Buffer List Queue specifies the start,end and size of a buffer
 * list with respect to buffer descriptor memory.
 */
struct nfp_nbi_dma_blq_config {
	/** Buffer List Queue Number */
	int blq;
	/** Start of buffer list*/
	int head;
	/** End of buffer list*/
	int tail;
	/** Size of Queue*/
	int size;
	/** Buffer size to be used*/
	int buffer_size;
	/** Pointer to bdsram config*/
	struct nfp_nbi_dma_bdsram_config *bdsram_cfg;
};

/**
 * NFP NBI DMA Buffer List Queue Event configuration
 */
struct nfp_nbi_dma_blq_event_config {
	/** Buffer List Queue Number */
	int blq;
	/** Enable events*/
	int blq_event_status_enable;
	/** Enable event when blq transitions from full to not full*/
	int blq_event_not_full_enable;
	/** BLQ Threshold 0,1,2,3 to generate event when 16,32,64,128 buffers
	 * are read
	 */
	int blq_event_threshold;
};

/**
 * NFP NBI DMA Buffer List Queue Status
 */
struct nfp_nbi_dma_blq_status {
	/** Buffer List Queue Number */
	int blq;
	/** Start of buffer list*/
	int head;
	/** End of buffer list*/
	int tail;
	/** Size of Queue*/
	int size;
	/** Counts number of BLQ Full Events dropped*/
	int blq_evnt_full_cnt;
	/**
	 * Number of buffer descriptors resident in the buffer descriptor
	 * register FIFO
	 */
	int blq_bdfifo_entry_cnt;
	/**
	 * The number of entries in the Buffer List Queue calcuate by
	 * hardware
	 */
	int blq_entry_cnt;
	/**The Queue is full when set*/
	int blq_full;
	/**The queue is empty when set*/
	int blq_empty;
};

/**
 * NFP NBI DMA Buffer Descriptor SRAM configuration
 * BDSRAM holds the confgiguration for buffers to be used for
 * packets.
 */
struct nfp_nbi_dma_bdsram_config {
	/** List of MU base addresses*/
	uint64_t *bc_mu_base_addr;
	/** List of total no. of buffers per mu base addresses specified.
	 * The sum of the entries in bc_total_cnt should be <=4K at all times.
	 */
	uint32_t *bc_total_cnt;
	/**
	 * List of no. of buffer entries in continuum per mu base
	 * addresses specified. When 0 will fill bdsram with total number
	 * of entries for corresponding buffer base. When X will fill
	 * bdsram with X entries of corresponding mu buffers, followed by
	 * the next mu base entry.
	 */
	uint32_t *bc_fill_ratio;
	/** Number of entries in the mu_base_addr array/list*/
	uint32_t bc_entries;
};

/**
 * NFP NBI DMA Counters
 */
struct nfp_nbi_dma_counters {
	/** Received Packet Count*/
	uint64_t pkt_cnt;
	/** Received Byte Count*/
	uint64_t byte_cnt;
	/** Dropped Packet Count*/
	uint64_t drop_pkt_cnt;
	/** Dropped Byte Count*/
	uint64_t drop_byte_cnt;
};

NFP_NBI_API
int nfp_nbi_dma_cfg_nbidma(struct nfp_nbi_dev *nfpnbi,
			   struct nfp_nbi_dma_config *nbidma_cfg);
/**
 * Configure NFP NBI DMA Buffer Pool control register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param bp_cfg	NFP NBI DMA BP Config structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_dma_cfg_bp(struct nfp_nbi_dev *nfpnbi,
		       struct nfp_nbi_dma_bp_config *bp_cfg);
/**
 * Reset NFP NBI DMA Buffer Pool Entry Chainend Register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_dma_reset_bpechained(struct nfp_nbi_dev *nfpnbi);
/**
 * Configure NFP NBI DMA Buffer Pool Entry register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param bpe_cfg	NFP NBI DMA BPE Config structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_dma_cfg_bpe(struct nfp_nbi_dev *nfpnbi,
			struct nfp_nbi_dma_bpe_config *bpe_cfg);

/**
 * Configure NFP NBI DMA Buffer List Queue Control register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param blq_cfg	NFP NBI DMA BLQ Config structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_dma_cfg_blq(struct nfp_nbi_dev *nfpnbi,
			struct nfp_nbi_dma_blq_config *blq_cfg);
/**
 * Configure NFP NBI DMA Buffer List Queue Event Control register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param blqevnt_cfg	NFP NBI DMA BLQ Event Config structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_dma_cfg_blq_event(struct nfp_nbi_dev *nfpnbi,
			      struct nfp_nbi_dma_blq_event_config *blqevnt_cfg);
/**
 *  Enable/Disable NFP NBI DMA Buffer List Queue Event FIFO full assert
 * @ingroup nfp6000-only
 *
 * @param set		NFP NBI DMA BLQ Event fifo full enable/disable
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_dma_write_blq_event_fifo_full_assert(int set);
/**
 * Read NFP NBI DMA Buffer Pool register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param bp		NFP NBI DMA Buffer Pool number
 * @param data		NFP NBI DMA BP Config structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_dma_read_bp(struct nfp_nbi_dev *nfpnbi,
			int bp,
			struct nfp_nbi_dma_bp_config *data);
/**
 * Read NFP NBI DMA Buffer Pool Entry register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param bpe       NFP NBI DMA Buffer Pool number
 * @param data		NFP NBI DMA BP Config structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_dma_read_bpe(struct nfp_nbi_dev *nfpnbi,
			 int bpe,
			 struct nfp_nbi_dma_bpe_config *data);
/**
 * Read NFP NBI DMA Buffer List Queue register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param blq		NFP NBI DMA Buffer List Queue number
 * @param data		NFP NBI DMA BLQ Config structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_dma_read_blq(struct nfp_nbi_dev *nfpnbi,
			 int blq,
			 struct nfp_nbi_dma_blq_config *data);
/**
 * Read NFP NBI DMA Buffer List Queue Event register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param blq		NFP NBI DMA Buffer List Queue number
 * @param data		NFP NBI DMA BLQ Event Config structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_dma_read_blq_event(struct nfp_nbi_dev *nfpnbi,
			       int blq,
			       struct nfp_nbi_dma_blq_event_config *data);
/**
 * Read NFP NFP NBI DMA Buffer List Queue status
 * @ingroup nfp6000-only
 *
 * @param nfpnbi		NFP NBI device handle
 * @param blq			NFP NBI DMA Buffer List Queue number
 * @param data			NFP NBI DMA BLQ Event Status structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_dma_read_blq_status(struct nfp_nbi_dev *nfpnbi,
				int blq,
				struct nfp_nbi_dma_blq_status *data);
/**
 * Read NFP NBI DMA BDSRAM contents
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param bdsram_start	NFP NBI DMA BDSRAM entry to start from
 * @param count		NFP NBI DMA BDSRAM entries to read
 * @param data		Pointer for NFP NBI DMA BDSRAM content
 *
 * @return number of bytes on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_dma_read_bdsram(struct nfp_nbi_dev *nfpnbi,
			    uint64_t bdsram_start,
			    uint64_t count,
			    uint64_t *data);
/**
 * Read NFP NBI DMA counters
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param data		NFP NBI DMA counters
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_dma_read_counters(struct nfp_nbi_dev *nfpnbi,
			      struct nfp_nbi_dma_counters *data);
#endif
