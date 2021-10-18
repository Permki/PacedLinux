/**
 * Copyright (C) 2013-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_nbi_tm.h
 *
 *
 */

#ifndef __NFP_NBI_TM_H__
#define __NFP_NBI_TM_H__

#include <stdint.h>

#include <nfp.h>
#include <nfp_nbi.h>

/**highest qid */
#define MAX_QID 1023
/**max minipkt credits */
#define MAX_MINIPKT_CRDTS 512
/**highest channel id */
#define MAX_CHAN_ID 127
/**highest scheduler id */
#define MAX_SCHED_ID 144
/**l0 scheduler highest port number */
#define MAX_L0_SCHED_PORT 127
/**l1 highest scheduler id */
#define MAX_L1_SCHED_ID 143
/**l2 highest scheduler id */
#define MAX_L2_SCHED_ID 127
/**highest shaper id */
#define MAX_SHP_ID 144
/**l1 ext highest shaper id **/
#define MAX_L1EXT_SHP_ID 15
/**highest supported shaper rate*/
#define MAX_SHP_RATE 12000
#define NFP_NBI_TM_NUM_PORT_WEIGHTS 8
/**Max drop precedence level**/
#define TM_MAX_DROP_PRECEDENCE 7

/**
 * Struct used for writing or reading the global traffic manager configuration
 */
struct nfp_nbi_tm_gbl_trf_mngr_cfg {
	/** TM free pointer no return : [0-1]*/
	uint8_t tm_freeptr_no_ret;
	/** DWRR ignore flow control : [0-1]*/
	uint8_t dwrr_ignore_fc;
	/** packet descriptor linked list enable : [0-1]*/
	uint8_t pkt_desc_ll_enable;
	/** mac out of band flow control enable : [0-1]*/
	uint8_t mac_oobfc;
	/** configure l0 channel map : [0-4]*/
	uint8_t l0_chan_map;
	/** enable sequencer 0 : [0-1]*/
	uint8_t seq0_enable;
	/** enable global scheduling : [0-1]*/
	uint8_t schd_enable;
	/** enable global shaping : [0-1]*/
	uint8_t shp_enable;
	/** configure channel level to be 1 or 0 : [0-1] */
	uint8_t chan_lvl_sel;
	/** configure the number of sequencers : [0-6]*/
	uint8_t num_seq;
	/** enable or disable level 1 of scheduling : [0-1]*/
	uint8_t l1_input_sel;
	/** Shaper reduce rate enable **/
	uint8_t shaper_reduce_rate;
	/** Shaper reduce three fourth enable **/
	uint8_t shaper_reduce_three_fourth;
	/** dummy field to force ARM and X86 to align **/
	uint8_t d1;
	/** dummy field to force ARM and X86 to align **/
	uint8_t d2;
	/** dummy field to force ARM and X86 to align **/
	uint8_t d3;
};

/**
 * Struct used for reading channel xoff information for all channels
 */
struct mac_chl_xoff {
	/** channel xoff information for channels 63-0  */
	uint64_t chl_63_0;
	/** channel xoff information for channels 127-64 */
	uint64_t chl_127_64;
};

/**
 * Struct used for writing or reading the global traffic manager drop rate
 * configuration
 */
struct nfp_nbi_tm_gbl_drop_rate_cfg {
	/** drop rate for queue drpth range A0 : [0-7] */
	uint8_t drp_rate_range_A0;
	/** drop rate for queue drpth range A1  : [0-7]*/
	uint8_t drp_rate_range_A1;
	/** drop rate for queue drpth range A2 : [0-7] */
	uint8_t drp_rate_range_A2;
	/** drop rate for queue drpth range A3 : [0-7] */
	uint8_t drp_rate_range_A3;
	/** drop rate for queue drpth range B0 : [0-7] */
	uint8_t drp_rate_range_B0;
	/** drop rate for queue drpth range B1 : [0-7] */
	uint8_t drp_rate_range_B1;
	/** drop rate for queue drpth range B2 : [0-7] */
	uint8_t drp_rate_range_B2;
	/** drop rate for queue drpth range B3 : [0-7] */
	uint8_t drp_rate_range_B3;
};

/**
 * Struct used for writing or reading the global traffic manager drop precedence
 * profile configuration
 */
struct nfp_nbi_tm_drop_profile_cfg {
	/** drop rate for queue depth range 0 : [0-7] */
	int8_t drp_rate_range_0;
	/** drop rate for queue depth range 1  : [0-7]*/
	int8_t drp_rate_range_1;
	/** drop rate for queue depth range 2 : [0-7] */
	int8_t drp_rate_range_2;
	/** drop rate for queue depth range 3 : [0-7] */
	int8_t drp_rate_range_3;
};

/**
 * Struct for writing or reading the parameters of a queue identified by qid
 */
struct nfp_nbi_tm_q_params_cfg {
	/** queue enable/disable : [0-1]*/
	uint16_t q_enable : 1;
	/** drop configration : [0-1]*/
	uint16_t q_drop_enable : 1;
	/** random early detection configuration : [0-1]*/
	uint16_t q_red_enable : 1;
	/** queue size : [0x3-0xe]*/
	uint16_t q_size : 4;
	/** dma threshold : [0-7]*/
	uint16_t q_dma_thresh : 3;
	/** drop range : [0-1]*/
	uint16_t q_drop_range : 1;
	/** drop precedence */
	uint16_t q_drop_precedence : 1;
	/** queue owner; pcie or nbi */
	uint16_t q_owner : 1;
};

struct nfp_nbi_tm_q_params_cfg_bin {
	/** used in the binary flash file to pack smaller */
	uint16_t data;
};

/**
 * NFP NBI TM Buffer List Queue configuration
 * Buffer List Queue specifies the start,end and size of a buffer
 * list with respect to buffer descriptor memory.
 */
struct nfp_nbi_tm_blq_config {
	/** Start of buffer list*/
	uint32_t head;
	/** End of buffer list*/
	uint32_t tail;
	/** Size of Queue*/
	uint32_t size;
	/** Buffer List Queue Number : [0-3]*/
	uint8_t blq;
	/** dummy field to force ARM and X86 to align **/
	uint8_t d1;
	/** dummy field to force ARM and X86 to align **/
	uint8_t d2;
	/** dummy field to force ARM and X86 to align **/
	uint8_t d3;
};

/**
 * NFP NBI TM Buffer List Queue Event configuration
 */
struct nfp_nbi_tm_blq_event_config {
	/** Buffer List Queue Number : [0-3]*/
	uint8_t blq;
	/** Enable events*/
	uint8_t blq_event_status_enable;
	/** Enable event when blq transitions from full to not full */
	uint8_t blq_event_not_empty_enable;
	/** BLQ Threshold [0-3] to generate event when 16,32,64,128
	 * buffers are read
	 */
	uint8_t blq_event_threshold;
};

/**
 * NFP NBI TM Buffer List Queue Status
 */
struct nfp_nbi_tm_blq_status {
	/** Start of buffer list*/
	uint32_t head;
	/** End of buffer list*/
	uint32_t tail;
	/** Size of Queue*/
	uint32_t size;
	/** Counts number of BLQ Full Events dropped*/
	uint32_t blq_evnt_full_cnt;
	/**
	 * Number of buffer descriptors resident in the buffer descriptor
	 * register FIFO
	 */
	uint32_t blq_bdfifo_entry_cnt;
	/**
	 * The number of entries in the Buffer List Queue calcuate by
	 * hardware
	 */
	uint32_t blq_entry_cnt;
	/**The Queue is full when set*/
	uint8_t blq_full;
	/**The queue is empty when set*/
	uint8_t blq_empty;
	/** Buffer List Queue Number : [0-3]*/
	uint8_t blq;
	/** dummy field to force ARM and X86 to align **/
	uint8_t d1;
};

/**
 * Struct for reading the status of a queue identified by qid
 */
struct nfp_nbi_tm_q_st {
	/** the number of current descriptors in a queue */
	uint16_t q_level;
	/** indicates if the queue is full or not */
	uint8_t q_full;
};

/**
 * Struct for reading the queue descriptor memeory head and tail pointers for a
 * queue identified by qid.
 */
struct nfp_nbi_tm_q_desc_pntrs {
	/** head pointer of the queue */
	uint32_t head_p;
	/** tail pointer of the queue */
	uint32_t tail_p;
};

/**
 * Struct for reading or writing the configuration for the shaper given shaper
id
 */
struct nfp_nbi_tm_shp_cfg {
	/** shaper rate : [0x1-0x2710]*/
	uint16_t rate;
	/** rate_adjust shaper rate adjust : [0-511]*/
	uint16_t rateadj;
	/** shaper threshold : [0-7]*/
	uint8_t thrhld;
	/** shaper maximum overshoot : [0-7]*/
	uint8_t maxovrsht;
	/** index number/not used indicator. 0xff is unused. **/
	uint8_t index;
	/** dummy field to force ARM and X86 to align **/
	uint8_t d1;
};

/**
 * Read the current status of the shaper with the given shaper id
 */
struct nfp_nbi_tm_shp_st {
	/** the level of the shaper */
	uint32_t shp_lvl;
	/** indicates if the shaper is open or closed */
	uint8_t shp_open;
};

/**
 * Struct for configuring l0 scheluder
 */
struct nfp_nbi_tm_l0_sched_cfg {
	/** dwrr weights for ports */
	uint32_t dwrr_wgt[MAX_L0_SCHED_PORT + 1];
	/** dwrr deficits for ports : [0]*/
	uint32_t dwrr_dft[MAX_L0_SCHED_PORT + 1];
	/** scheduler mode of operation : [0-7] */
	uint8_t sched_mode;
	/** strict priority mode : [0,1,2,3]*/
	uint8_t sp_mode;
	/** dwrr enable **/
	uint8_t dwrr_enable;
	/** dummy field to force ARM and X86 to align **/
	uint8_t d1;
};

/**
 *@ Struct to be populated with the dwrr port values
 */
struct nfp_nbi_tm_sched_cfg {
	/** scheduler port weights : [0-0x7fffff]*/
	uint32_t dwrr_wgt[NFP_NBI_TM_NUM_PORT_WEIGHTS];
	/** scheduler port deficits : [0]*/
	uint32_t dwrr_dft[NFP_NBI_TM_NUM_PORT_WEIGHTS];
	/** scheduler mode of operation : [0-7]*/
	uint8_t sched_mode;
	/** strict priority mode : [0,1,2,3]*/
	uint8_t sp_mode;
	/** dwrr enable **/
	uint8_t dwrr_enable;
	/** l1 extention scheduler **/
	int8_t l1ext_enable;
	/** l1 extension dwrr enable **/
	int8_t l1ext_dwrr_enable;
	/** l1 extension strict priority mode : [0,1,2,3]*/
	uint8_t l1ext_sp_mode;
	/** index number/not used indicator. 0xff is unused. **/
	uint8_t index;
	/** dummy field to force ARM and X86 to align **/
	uint8_t d1;
};

/**
 * Struct used for writing or reading the minipacket credit configuration
 */
struct nfp_nbi_tm_minipkt_crdts_cfg {
	/** free pool credit limit : [0-511]*/
	uint16_t fp_crdt_lim;
	/** channel credit limit : [0-511]*/
	uint16_t crdt_lim;
	/** minipacket flow control mode : [0-1]*/
	uint8_t minipkt_fc_mode;
	/** per channel credit limit enable */
	int8_t per_chl_crdt_lim;
	/** dummy field to force ARM and X86 to align **/
	uint8_t d1;
	/** dummy field to force ARM and X86 to align **/
	uint8_t d2;
};

struct nfp_nbi_tm_cfg {
	struct nfp_nbi_tm_gbl_trf_mngr_cfg tm_gbl_cfg;
	struct nfp_nbi_tm_q_params_cfg qparams_cfg[MAX_QID + 1];
	struct nfp_nbi_tm_minipkt_crdts_cfg minipkt_cfg;
	struct nfp_nbi_tm_shp_cfg shpcfg[MAX_SHP_ID + 1];
	struct nfp_nbi_tm_shp_cfg l1ext_shpcfg[MAX_L1EXT_SHP_ID + 1];
	struct nfp_nbi_tm_sched_cfg schedcfg[MAX_SCHED_ID + 1];
	struct nfp_nbi_tm_l0_sched_cfg l0_schedcfg;
	struct nfp_nbi_tm_blq_config blq_config[4];
	struct nfp_nbi_tm_drop_profile_cfg drp_profile_cfg[TM_MAX_DROP_PRECEDENCE + 1];
};

#define NFP_NBI_TM_CODE_VERSION 3
#define NFP_NBI_TM_NAMESIZE 32
struct nfp_nbi_tm_emit_bin {
	char name[NFP_NBI_TM_NAMESIZE];
	int32_t code_ver;
	uint32_t crc;
	uint32_t size;
	uint8_t tm_shp_cfg_count;
	uint8_t tm_sched_cfg_count;
	uint8_t tm_l1ext_shp_cfg_count;
	uint8_t d1;
};

/**
 * Function to initialize the traffic manager from a configuration
 * @ingroup nfp6000-only
 *
 *@param       nbi             NBI device
 *@param       file            configuration file
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_init(struct nfp_nbi_dev *nbi, const char *file);

/**
 * Function to load the traffic manager configuration from a file
 * @ingroup nfp6000-only
 *
 *@param       cfg             configuration struct
 *@param       fp              configuration file pointer
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_load_config(struct nfp_nbi_tm_cfg *cfg, FILE *fp);

/**
 * Function to initialize the traffic manager from a configuration struct
 * @ingroup nfp6000-only
 *
 *@param       nbi             NBI device
 *@param       cfg             configuration struct
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_cfg(struct nfp_nbi_dev *nbi, const struct nfp_nbi_tm_cfg *cfg);

/**
 * Write global traffic manager configuration
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	tm_gbl_cfg	a struct nfp_nbi_tm_gbl_trf_mngr_cfg object to be
 *populated
 *			        with traffic manager parameters to be configured
 *@return			<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_write_gbl_cfg(
	struct nfp_nbi_dev *nfpnbi,
	const struct nfp_nbi_tm_gbl_trf_mngr_cfg *tm_gbl_cfg);

/**
 *Read global traffic manager configuration
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	tm_gbl_cfg	a struct nfp_nbi_tm_gbl_trf_mngr_cfg object that
 *will be
 *				updated with the current traffic manager config
 *@return			<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_gbl_cfg(struct nfp_nbi_dev *nfpnbi,
			    struct nfp_nbi_tm_gbl_trf_mngr_cfg *tm_gbl_cfg);

/**
 * Configure global traffic manager scheduling
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	enable	Set enable 1 to enable traffic manager global scheduling
 *@return		<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_sched_cfg(struct nfp_nbi_dev *nfpnbi, int enable);

/**
 * Configure mac/traffic manager out of band flow control
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        enable          Set enable 1 to enable out of band flow control
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_oobfc_cfg(struct nfp_nbi_dev *nfpnbi, int enable);

/**
 * Read  mac/traffic manager out of band flow control for qids
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        qid             queue id for oobfc state read : [0-1023]
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_qid_oobfc(struct nfp_nbi_dev *nfpnbi, int qid);

/**
 * Read  mac/traffic manager out of band flow control for channels
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        chl             channel number for oobfc state read : [0-127]
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_chl_xoff(struct nfp_nbi_dev *nfpnbi, int chl);

/**
 * Read  mac channels xoff information for all channels(127-0)
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        chl_xoff        a struct mac_chl_off object
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_chl_xoff_all_chl(struct nfp_nbi_dev *nfpnbi,
				     struct mac_chl_xoff *chl_xoff);

/**
 * Configure traffic manager global drop rates
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	drp_rate_cfg	a struct nfp_nbi_tm_gbl_drop_rate_cfg object to
 *be updated
 *				with drop rate parameters to be configured
 *@return			<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_write_drop_rate_cfg(
	struct nfp_nbi_dev *nfpnbi,
	const struct nfp_nbi_tm_gbl_drop_rate_cfg *drp_rate_cfg);

/**
 * Read traffic manager global drop rate configuration
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	     tm device handle
 *@param	drp_rate_cfg a struct nfp_nbi_tm_gbl_drop_rate_cfg object that
 *will be
 *                           updated with the drop rates configuration
 *@return		     <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_drop_rate_cfg(
	struct nfp_nbi_dev *nfpnbi,
	struct nfp_nbi_tm_gbl_drop_rate_cfg *drp_rate_cfg);

/**
 * Dump global traffic manager configuration
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi              tm device handle
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_gbl_cfg_dump(struct nfp_nbi_dev *nfpnbi);

/**
 * Write the queue paramaters for the queue with the provided qid
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	qid	queue identifier for a queue : [0-1023]
 *@param	qparams a struct nfp_nbi_tm_q_params_cfg object that needs to be
 *populated
 *			with the queue parameters to be configured
 *@return		<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_write_q_cfg(struct nfp_nbi_dev *nfpnbi,
			   int qid,
			   const struct nfp_nbi_tm_q_params_cfg *qparams);

/**
 * Read the queue paramaters for the queue with the provided qid
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	qid	queue identifier for a queue : [0-1023]
 *@param	qparams a struct nfp_nbi_tm_q_params_cfg object that will be
 *updated with
 *			the queue parameters
 *@return		<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_q_cfg(struct nfp_nbi_dev *nfpnbi,
			  int qid,
			  struct nfp_nbi_tm_q_params_cfg *qparams);

/**
 * Read the drop count for the queue with the provided qid
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	qid	queue identifier for a queue : [0-1023]
 *@param	dropcnt the queue drop count is returned through this parameter
 *@param	clear	clear, if set to 1, will clear the drop count register
 *			for a queue (clear on read)
 *@return		<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_q_dropcnt(struct nfp_nbi_dev *nfpnbi,
			      int qid,
			      uint32_t *dropcnt,
			      int clear);

/**
 * Read the queue status for the queue with the provided qid
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	qid	queue identifier for a queue : [0-1023]
 *@param	qst	struct nfp_nbi_tm_q_st object will be updated with the
 *queue status
 *@return		<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_q_status(struct nfp_nbi_dev *nfpnbi,
			     int qid,
			     struct nfp_nbi_tm_q_st *qst);

/**
 * Configure the head tail sram pointers for the queue with the provided qid.
 * @ingroup nfp6000-only
 *
 *The tail pointer will be set to the head pointer.
 *
 *@param	nfpnbi	tm device handle
 *@param	qid	queue identifier for a queue : [0-1023]
 *@param	q_headp the head pointer to be configured for a queue
 *@return		<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_write_headtailsram_cfg(struct nfp_nbi_dev *nfpnbi,
				      int qid,
				      uint32_t q_headp);

/**
 * Read the current head and tail sram pointers for a queue
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	qid	queue identifier for a queue : [0-1023]
 *@param	qpntrs	a nfp_nbi_tm_q_desc_pntrs object that will be updated
 *with the
 *                      head and tail pointers of a queue
 *@return		<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_headtailsram_cfg(struct nfp_nbi_dev *nfpnbi,
				     int qid,
				     struct nfp_nbi_tm_q_desc_pntrs *qpntrs);

/**
 * Dump queue configuration
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        qid             queue identifier for a queue : [0-1023]
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_q_cfg_dump(struct nfp_nbi_dev *nfpnbi, int qid);

/**
 * Dump the current status for a given queue
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        qid             the queue identifier number : [0-1023]
 *@param        clear           clear, if set, will clear the queue drop counter
 *register for the given qid
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_q_status_dump(struct nfp_nbi_dev *nfpnbi, int qid, int clear);

/**
 * Configure the shaper with the given shaper id with the shaper parameters
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi  tm device handle
 *@param	shp_id  the shaper identifier : [0-144]
 *@param	shpcfg	a nfp_nbi_tm_shp_cfg struct object that is to be
 *populated with
 *                      the shaper config parameters
 *@return		<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_write_shp_cfg(struct nfp_nbi_dev *nfpnbi,
			     int shp_id,
			     const struct nfp_nbi_tm_shp_cfg *shpcfg);

/**
 * Configure the l1 ext shaper with the given shaper id with the shaper parameters
 * @ingroup nfp6000-only
 *
 *@param    nfpnbi  tm device handle
 *@param    shp_id  the shaper identifier : [0-14]
 *@param    shpcfg  a nfp_nbi_tm_shp_cfg struct object that is to be populated with
 *                      the shaper config parameters
 *@return       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_write_l1ext_shp_cfg(struct nfp_nbi_dev *nfpnbi, int shp_id,
				   const struct nfp_nbi_tm_shp_cfg *shpcfg);

/**
 * Read the configuration of the shaper with the given shaper id
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	shp_id	the shaper identifier : [0-144]
 *@param	shpcfg	a struct nfp_nbi_tm_shp_cfg object for returning
 *shaper config
 *			parameters
 *@return		<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_shp_cfg(struct nfp_nbi_dev *nfpnbi,
			    int shp_id,
			    struct nfp_nbi_tm_shp_cfg *shpcfg);

/**
 * Read the shaper status of the shaper with the given shaper id
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	shp_id	the shaper identifier : [0-144]
 *@param	shpst	a struct nfp_nbi_tm_shp_st object that will be updated
 *with the
 *			shaper parameters
 *@return		<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_shp_status(struct nfp_nbi_dev *nfpnbi,
			       int shp_id,
			       struct nfp_nbi_tm_shp_st *shpst);

/**
 * Dump the configuration for a given shaper
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi        tm device handle
 *@param        shp_id        the shaper identifier : [0-144]
 *@return                     <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_shp_cfg_dump(struct nfp_nbi_dev *nfpnbi, int shp_id);

/**
 * Dump the current status of a given l1 extension shaper
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi        tm device handle
 *@param        shp_id        shaper identifier number : [0-14]
 *@return                     <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_l1ext_shp_status_dump(struct nfp_nbi_dev *nfpnbi, int shp_id);

/**
 * Dump the configuration for a given l1 extension shaper
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi        tm device handle
 *@param        shp_id        the shaper identifier : [0-14]
 *@return                     <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_l1ext_shp_cfg_dump(struct nfp_nbi_dev *nfpnbi, int shp_id);

/**
 * Dump the current status of a given shaper
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi        tm device handle
 *@param        shp_id        shaper identifier number : [0-144]
 *@return                     <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_shp_status_dump(struct nfp_nbi_dev *nfpnbi, int shp_id);

/**
 * Configure the scheduler mode of operation given the scheduler id
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	sched_id	the scheduler identifier : [0-144]
 *@param	schedcfg	the scheduler mode of operation
 *@return			<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_write_sched_cfg(struct nfp_nbi_dev *nfpnbi,
			       int sched_id,
			       const struct nfp_nbi_tm_sched_cfg *schedcfg);

/**
 * Read the scheduler mode of operation given the scheduler id
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	sched_id	the scheduler identifier : [0-144]
 *@param	schedcfg	the scheduler mode of operation
 *@return			<0 on error
 */

NFP_NBI_API
int nfp_nbi_tm_read_sched_cfg(struct nfp_nbi_dev *nfpnbi,
			      int sched_id,
			      struct nfp_nbi_tm_sched_cfg *schedcfg);

/**
 * Read the dwrr deficits for the ports of the scheduler with the given
 * scheduler id
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	sched_id	the scheduler identifier (L2 and L1 schedulers only)
 *: [0-144]
 *@param	dft		a struct dwrr_dfct_cfg object that will be
 *				updated with the deficits for the ports of the
 *				scheduler
 *@return			<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_sched_dwrr_cfg_dft(struct nfp_nbi_dev *nfpnbi,
				       int sched_id,
				       uint32_t dft[]);

/**
 * Initialize the dwrr deficits to zero for a given L2/L1 scheduler
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        sched_id        the scheduler identifier (L2 and L1 schedulers
 *only): [0-144]
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_init_sched_dwrr_cfg_dft(struct nfp_nbi_dev *nfpnbi,
				       int sched_id);

/**
 * Configure the dwrr weight for a port of the l0 scheduler
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	 tm device handle
 *@param	l0_schedcfg  a nfp_nbi_tm_l0_sched_cfg object to be populated with
 *the weights and deficits
 *			 port
 *@return		 <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_write_sched_l0_dwrr_cfg(
	struct nfp_nbi_dev *nfpnbi,
	const struct nfp_nbi_tm_l0_sched_cfg *l0_schedcfg);

/**
 * Read the dwrr weight for a port of the l0 scheduler
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param        l0_schedcfg  a nfp_nbi_tm_l0_sched_cfg object to be populated
 *with the weights and deficits
 *@return	<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_sched_l0_dwrr_cfg(
	struct nfp_nbi_dev *nfpnbi,
	struct nfp_nbi_tm_l0_sched_cfg *l0_schedcfg);

/**
 * Dump the configuration for a given scheduler
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        sched_id        scheduler identifier number : [0-144]
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_sched_cfg_dump(struct nfp_nbi_dev *nfpnbi, int sched_id);

/**
 * Read the out of order sequencer count for the sequencer id
 * @ingroup nfp6000-only
 *
 *@param	 nfpnbi		tm device handle
 *@param	 seq_id		the sequencer identifier : [0-31]
 *@param	 ooo_cnt	parameter to be updated with the out of order
 *				count for the given sequencer id
 *@param	 clear		if 1,clears the out of order count register on
 *				a read
 *@return			<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_ooo_cnt(struct nfp_nbi_dev *nfpnbi,
			    int seq_id,
			    uint32_t *ooo_cnt,
			    int clear);

/**
 * Read the reorder activity for the sequencer with the given sequencer id
 * @ingroup nfp6000-only
 *
 *@param	 nfpnbi		tm device handle
 *@param	 seq_id		the sequencer identifier
 *@param	 reord_act	if set, indicates that there was reordering for
 *				the sequencer with the given sequencer id
 *@return			<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_reord_activity(struct nfp_nbi_dev *nfpnbi,
				   int seq_id,
				   int *reord_act);

/**
 * Dump the configuration for a given sequencer
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        seq_id          sequencer identifier number : [0-31]
 *@param        clear           clear, if set, will clear the out of order count
 *for the sequencer
 *                              identified by seq_id
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_seq_dump(struct nfp_nbi_dev *nfpnbi, int seq_id, int clear);

/**
 * Configure the minipacket credits
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	crdt_cfg	a struct nfp_nbi_tm_minipkt_crdts_cfg object to
 be
				populated with the credit config parameters
 *@return			<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_write_minipkt_crdt_cfg(
	struct nfp_nbi_dev *nfpnbi,
	const struct nfp_nbi_tm_minipkt_crdts_cfg *crdt_cfg);

/**
 * Read the minipacket credits configuration
 * @ingroup nfp6000-only
 *
 *@param	 nfpnbi		tm device handle
 *@param	 crdt_cfg	a struct nfp_nbi_tm_minipkt_crdts_cfg object that
 *is to be
 *				updated with the credit configuration parameters
 *@return			<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_minipkt_crdt_cfg(
	struct nfp_nbi_dev *nfpnbi,
	struct nfp_nbi_tm_minipkt_crdts_cfg *crdt_cfg);

/**
 * Read the minipacket channel credits for a given channel
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	chl		the channel number : [0-127]
 *@param	chan_crdts	this parameter will be updated with the channel
 *				credits
 *@return			<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_minipkt_chan_crdts(struct nfp_nbi_dev *nfpnbi,
				       int chl,
				       uint32_t *chan_crdts);

/**
 * Read the minipacket free pool credits for the specified free pool
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	fp_num		the free pool number : [0-1]
 *@param	fp_crdts	this parameter will be updated with the current
				pool credits for the given free pool
 *@return			<0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_read_minipkt_fp_crdts(struct nfp_nbi_dev *nfpnbi,
				     int fp_num,
				     uint32_t *fp_crdts);

/**
 * Dump minipacket credits configuration
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_minipkt_cfg_dump(struct nfp_nbi_dev *nfpnbi);

/**
 * Dump the current minipacket credits for a given channel
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        chl             the channel number : [0-127]
 *@return                       <0 on error
 */
NFP_NBI_API
int nfp_nbi_tm_chan_minipkt_crdts_dump(struct nfp_nbi_dev *nfpnbi, int chl);

/**
 * Configure NFP NBI TM Buffer List Queue Event Control register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param blqevnt_cfg	NFP NBI TM BLQ Event Config structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_cfg_blq_event(struct nfp_nbi_dev *nfpnbi,
			     struct nfp_nbi_tm_blq_event_config *blqevnt_cfg);
/**
 * Read NFP NBI TM Buffer List Queue Event register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param data		NFP NBI TM BLQ Event Config structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_read_blq_event(struct nfp_nbi_dev *nfpnbi,
			      struct nfp_nbi_tm_blq_event_config *data);

/**
 * Configure NFP NBI TM Buffer List Queue Control register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param blq_cfg	NFP NBI TM BLQ Config structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_cfg_blq(struct nfp_nbi_dev *nfpnbi,
		       struct nfp_nbi_tm_blq_config *blq_cfg);
/**
 * Read NFP NFP NBI TM Buffer List Queue status
 * @ingroup nfp6000-only
 *
 * @param nfpnbi		NFP NBI device handle
 * @param blq			NFP NBI TM Buffer List Queue number : [0-3]
 * @param data			NFP NBI TM BLQ Event Status structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_read_blq_status(struct nfp_nbi_dev *nfpnbi,
			       int blq,
			       struct nfp_nbi_tm_blq_status *data);

/**
 * Read NFP NBI TM Head/Tail sram contents
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param htsram_start	NFP NBI TM Head/Tail entry to start from
 * @param count		NFP NBI TM Head/Tail entries to read
 * @param data		Pointer for NFP NBI TM sram content
 *
 * @return number of bytes on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_read_htsram(struct nfp_nbi_dev *nfpnbi,
			   uint64_t htsram_start,
			   uint64_t count,
			   uint64_t *data);

/**
 * Read NFP NBI TM BDSRAM contents
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param bdsram_start	NFP NBI TM BDSRAM entry to start from
 * @param count		NFP NBI TM BDSRAM entries to read
 * @param data		Pointer for NFP NBI TM BDSRAM content
 *
 * @return number of bytes on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_read_bdsram(struct nfp_nbi_dev *nfpnbi,
			   uint64_t bdsram_start,
			   uint64_t count,
			   uint64_t *data);

/**
 * Read NFP NBI TM Packet Sram contents
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param pktsram_start	NFP NBI TM PktSram entry to start from
 * @param count		NFP NBI TM packet sram entries to read
 * @param data		Pointer for NFP NBI TM sram content
 *
 * @return number of bytes on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_read_pktsram(struct nfp_nbi_dev *nfpnbi,
			    uint64_t pktsram_start,
			    uint64_t count,
			    uint64_t *data);

/**
 * Read NFP NBI TM Descriptor Sram contents
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param descsram_start	NFP NBI TM descriptor sram entry to start from
 * @param count		NFP NBI TM descriptro sram entries to read
 * @param data		Pointer for NFP NBI TM sram content
 *
 * @return number of bytes on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_read_descsram(struct nfp_nbi_dev *nfpnbi,
			     uint64_t descsram_start,
			     uint64_t count,
			     uint64_t *data);

/**
 * Read NFP NBI TM Reorder buffer Sram contents
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param reorderbuf_start	NFP NBI TM Reorder sram entry to start from
 * @param count		NFP NBI TM Reorder sram entries to read
 * @param data		Pointer for NFP NBI TM sram content
 *
 * @return number of bytes on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_read_reorderbuf(struct nfp_nbi_dev *nfpnbi,
			       uint64_t reorderbuf_start,
			       uint64_t count,
			       uint64_t *data);

/**
 * Load a single NFP NBI TM config from flash by name
 * @ingroup nfp6000-only
 *
 * @param nfp	NFP device handle
 * @param cfg	TM config structure pointer to load into
 * @param name	Name of the config to load
 *
 * @return 0 on success, non-0 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_load_config_flash(struct nfp_device *nfp,
				 struct nfp_nbi_tm_cfg *cfg,
				 const char *name);

/**
 * Choose and configure the TM on all in use NBIs from flash
 * @ingroup nfp6000-only
 *
 * @param nfp	NFP device handle
 *
 * @return 0 on success, non-0 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_choose_config(struct nfp_device *nfp);

/**
 * Initialize linked list for packet descriptors
 * @ingroup nfp6000-only
 *
 * @param nfpnbi        NFP NBI device handle
 * @param tm_gbl_cfg    A type nfp_nbi_tm_gbl_trf_mngr_cfg struct to check if linked
 *                      list mode is enabled.
 * @return              0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_init_linked_list(struct nfp_nbi_dev *nfpnbi,
				const struct nfp_nbi_tm_gbl_trf_mngr_cfg
				*tm_gbl_cfg);

/**
 * Get the maximum index number of queue descriptors for the NBI
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 *
 * @return the maximum index number of descriptors
 */
NFP_NBI_API
int nfp_nbi_tm_get_max_qdesc(struct nfp_nbi_dev *nfpnbi);

/**
 * Write per channel minipkt credit config
 * @ingroup nfp6000-only
 *
 * @param nfpnbi        NFP NBI device handle
 * @param chan          channel number (range 0-127)
 * @param crdt_lim      per channel credit limit (range 0-512)
 *
 * @return              0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_write_minipkt_per_chl_crdt_cfg(struct nfp_nbi_dev *nfpnbi,
					      int chan, uint32_t crdt_lim);
/**
 * Read per channel minipkt credit config
 * @ingroup nfp6000-only
 *
 * @param nfpnbi        NFP NBI device handle
 * @chan                channel number (range 0-127)
 * @param crdt_lim      the return value for the credit limit
 *
 * @return              0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_read_minipkt_per_chl_crdt_cfg(struct nfp_nbi_dev *nfpnbi,
					     int chan, uint32_t *crdt_lim);
/**
 * Write drop precedence profile configuration
 * @ingroup nfp6000-only
 *
 * @param nfpnbi        NFP NBI device handle
 * @drop_precedence     drop precedence number (range 0-7)
 * @drp_profile         an instance of struct nfp_nbi_tm_gbl_drop_profile_cfg
 *
 * @return              0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_write_drop_precedence_profile(struct nfp_nbi_dev *nfpnbi,
					     int drop_precedence,
					     struct nfp_nbi_tm_drop_profile_cfg *drp_profile);

/**
 * Read drop precedence profile configuration
 * @ingroup nfp6000-only
 *
 * @param nfpnbi        NFP NBI device handle
 * @drop_precedence     drop precedence number (range 0-7)
 * @drp_profile         an instance of struct nfp_nbi_tm_gbl_drop_profile_cfg
 *
 * @return              0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_read_drop_precedence_profile(struct nfp_nbi_dev *nfpnbi,
					    int drop_precedence,
					    struct nfp_nbi_tm_drop_profile_cfg *drp_profile);

/**
 * Write TM egress rate limit configuration
 * @ingroup nfp6000-only
 *
 * @param nfpnbi        NFP NBI device handle
 * @enable              1 = enable limiting TM egress rate, 0 = disable rate limiting
 * @rate_limit          limit egress rate in the range [8, 128] Gbps (0-7)
 *
 * @return              0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_write_cfg_egress_rate_limit(struct nfp_nbi_dev *nfpnbi,
					   int enable, uint32_t rate_limit);

/**
 * Read TM egress rate limit configuration
 * @ingroup nfp6000-only
 *
 * @param nfpnbi        NFP NBI device handle
 * @enable              1 = rate limiting is enabled, 0 =disabled
 * @rate_limit          egress rate is limited in the range [8, 128] Gbps (0-7)
 *
 * @return              0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_read_cfg_egress_rate_limit(struct nfp_nbi_dev *nfpnbi,
					  int enable, uint32_t *rate_limit);

/**
 * Configure TM queue for pcie
 * @ingroup nfp6000-only
 *
 * @param nfpnbi        NFP NBI device handle
 * @enable              1 = pcie queue, 0: NBI queue
 * @qid                 TM queue id. Valid range (0-1023)
 *
 * @return              0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_pcie_q_cfg(struct nfp_nbi_dev *nfpnbi, int qid, int enable);

/**
 * Funtion to see if qowner is pcie or NBI
 * @ingroup nfp6000-only
 *
 * @param nfpnbi        NFP NBI device handle
 * @q_owner             1 = pcie queue, 0: NBI queue
 * @qid                 TM queue id. Valid range (0-1023)
 *
 * @return              0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_get_q_owner(struct nfp_nbi_dev *nfpnbi, int qid, int *q_owner);

/**
 * Funtion to read DSF/CPP command drops due to a full fifo
 * @ingroup nfp6000-only
 *
 * @param nfpnbi        NFP NBI device handle
 * @drop_status         a 32 bit drop status variable
 *
 * @return              0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_read_pcie_cmd_drp_status(struct nfp_nbi_dev *nfpnbi,
					uint32_t *drop_status);

/**
 * Funtion to read DSF/CPP command drops due to a full fifo
 * @ingroup nfp6000-only
 *
 * @param nfpnbi        NFP NBI device handle
 * @drop_status         a 32 bit drop status variable
 *
 * @return              0 on success, -1 on failure.
 */
NFP_NBI_API
int nfp_nbi_tm_read_cmd_drp_status(struct nfp_nbi_dev *nfpnbi,
				   uint32_t *drop_status);

#endif
