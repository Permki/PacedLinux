/**
 * Copyright (C) 2013-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_nbi_tm.h
 *
 *
 */

#ifndef __NFP_NBI_TM_H__
#define __NFP_NBI_TM_H__

#include <stdbool.h>
#include <stdint.h>

#include <nfp.h>
#include <nfp_nbi.h>

/**highest qid */
#define MAX_QID 1023
/**maximum queue descriptors */
#define MAX_Q_DESC 16383
/**MAX_MINIPKT_CREDITS */
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
/**highest supported shaper rate*/
#define MAX_SHP_RATE 12000
/**highest sequencer id */
#define MAX_SEQ_ID 31
/**uninitialized value */
#define NFP_NBI_TM_UNINITIALIZED -12345
#define NFP_NBI_TM_NUM_PORT_WEIGHTS 8


/**
 * Struct used for writing or reading the global traffic manager configuration
 */
struct nfp_nbi_tm_gbl_trf_mngr_cfg {
  /** mac out of band flow control enable : [0-1]*/
    int32_t mac_oobfc;
  /** configure l0 channel map : [0-4]*/
    int32_t l0_chan_map;
  /** enable sequencer 0 : [0-1]*/
    int32_t seq0_enable;
  /** enable global scheduling : [0-1]*/
    int32_t schd_enable;
  /** enable global shaping : [0-1]*/
    int32_t shp_enable;
  /** configure channel level to be 1 or 0 : [0-1] */
    int32_t chan_lvl_sel;
  /** configure the number of sequencers : [0-6]*/
    int32_t num_seq;
  /** enable or disable level 1 of scheduling : [0-1]*/
    int32_t l1_input_sel;
};


/**
 * Struct used for reading channel xoff information for all channels */
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
    int32_t drp_rate_range_A0;
  /** drop rate for queue drpth range A1  : [0-7]*/
    int32_t drp_rate_range_A1;
  /** drop rate for queue drpth range A2 : [0-7] */
    int32_t drp_rate_range_A2;
   /** drop rate for queue drpth range A3 : [0-7] */
    int32_t drp_rate_range_A3;
  /** drop rate for queue drpth range B0 : [0-7] */
    int32_t drp_rate_range_B0;
  /** drop rate for queue drpth range B1 : [0-7] */
    int32_t drp_rate_range_B1;
   /** drop rate for queue drpth range B2 : [0-7] */
    int32_t drp_rate_range_B2;
  /** drop rate for queue drpth range B3 : [0-7] */
    int32_t drp_rate_range_B3;
};

/**
 * Struct for writing or reading the parameters of a queue identified by qid
 */
struct nfp_nbi_tm_q_params_cfg {
  /** queue enable/disable : [0-1]*/
    int32_t q_enable;
  /** drop configration : [0-1]*/
    int32_t q_drop_enable;
  /** random early detection configuration : [0-1]*/
    int32_t q_red_enable;
  /** queue size : [0x3-0xe]*/
    int32_t q_size;
  /** dma threshold : [0-7]*/
    int32_t q_dma_thresh;
  /** drop range : [0-1]*/
    int32_t q_drop_range;
};

/**
 * NFP NBI TM Buffer List Queue configuration
 * Buffer List Queue specifies the start,end and size of a buffer
 * list with respect to buffer descriptor memory.
 */
struct nfp_nbi_tm_blq_config {
    /** Buffer List Queue Number : [0-3]*/
    int32_t blq;
    /** Start of buffer list*/
    int32_t head;
    /** End of buffer list*/
    int32_t tail;
    /** Size of Queue*/
    int32_t size;
};

/**
 * NFP NBI TM Buffer List Queue Event configuration
 */
struct nfp_nbi_tm_blq_event_config {
    /** Buffer List Queue Number : [0-3]*/
    int32_t blq;
    /** Enable events*/
    int32_t blq_event_status_enable;
    /** Enable event when blq transitions from full to not full*/
    int32_t blq_event_not_empty_enable;
    /** BLQ Threshold [0-3] to generate event when 16,32,64,128 buffers are read*/
    int32_t blq_event_threshold;
};

/**
 * NFP NBI TM Buffer List Queue Status
 */
struct nfp_nbi_tm_blq_status {
    /** Buffer List Queue Number : [0-3]*/
    int32_t blq;
    /** Start of buffer list*/
    int32_t head;
    /** End of buffer list*/
    int32_t tail;
    /** Size of Queue*/
    int32_t size;
    /** Counts number of BLQ Full Events dropped*/
    int32_t blq_evnt_full_cnt;
    /**
    * Number of buffer descriptors resident in the buffer descriptor
    * register FIFO
    */
    int32_t blq_bdfifo_entry_cnt;
    /**
    * The number of entries in the Buffer List Queue calcuate by
    * hardware
    */
    int32_t blq_entry_cnt;
    /**The Queue is full when set*/
    int32_t blq_full;
    /**The queue is empty when set*/
    int32_t blq_empty;

};

/**
 * Struct for reading the status of a queue identified by qid
 */
struct nfp_nbi_tm_q_st {
  /** indicates if the queue is full or not */
    int32_t q_full;
  /** the number of current descriptors in a queue */
    int32_t q_level;
};

/**
 * Struct for reading the queue descriptor memeory head and tail pointers for a
 * queue identified by qid.
*/
struct nfp_nbi_tm_q_desc_pntrs {
  /** head pointer of the queue */
    int32_t head_p;
  /** tail pointer of the queue */
    int32_t tail_p;
};


/**
 * Struct for reading or writing the configuration for the shaper given shaper
id
 */
struct nfp_nbi_tm_shp_cfg {
  /** shaper rate : [0x1-0x2710]*/
    int32_t rate;
  /** shaper threshold : [0-7]*/
    int32_t thrhld;
  /** shaper maximum overshoot : [0-7]*/
    int32_t maxovrsht;
  /** rate_adjust shaper rate adjust : [0-511]*/
    int32_t rateadj;
};


/**
 * Read the current status of the shaper with the given shaper id
 */
struct nfp_nbi_tm_shp_st {
  /** indicates if the shaper is open or closed */
    int32_t shp_open;
   /** the level of the shaper */
    int32_t shp_lvl;
};


/**
 * Struct for configuring l0 scheluder
 */
struct nfp_nbi_tm_l0_sched_cfg {
/** scheduler mode of operation : [0,1,2,4,6]*/
    int32_t sched_mode;
/** dwrr weights for ports */
    uint32_t dwrr_wgt[MAX_L0_SCHED_PORT + 1];
/** dwrr deficits for ports : [0]*/
    uint32_t dwrr_dft[MAX_L0_SCHED_PORT + 1];

};

/**
 *@ Struct to be populated with the dwrr port values
 */
struct nfp_nbi_tm_sched_cfg {
/** scheduler mode of operation : [0,1,2,4,6]*/
    int32_t sched_mode;
/** scheduler port weights : [0-0x7fffff]*/
    uint32_t dwrr_wgt[NFP_NBI_TM_NUM_PORT_WEIGHTS];
/** scheduler port deficits : [0]*/
    uint32_t dwrr_dft[NFP_NBI_TM_NUM_PORT_WEIGHTS];
};

/**
 * Struct used for writing or reading the minipacket credit configuration
 */
struct nfp_nbi_tm_minipkt_crdts_cfg {
  /** minipacket flow control mode : [0-1]*/
    int32_t minipkt_fc_mode;
  /** free pool credit limit : [0-511]*/
    int32_t fp_crdt_lim;
  /** channel credit limit : [0-511]*/
    int32_t crdt_lim;
};

struct nfp_nbi_tm_cfg {
    struct nfp_nbi_tm_gbl_trf_mngr_cfg tm_gbl_cfg;
    struct nfp_nbi_tm_q_params_cfg qparams_cfg[MAX_QID + 1];
    struct nfp_nbi_tm_minipkt_crdts_cfg minipkt_cfg;
    struct nfp_nbi_tm_shp_cfg shpcfg[MAX_SHP_ID + 1];
    struct nfp_nbi_tm_sched_cfg schedcfg[MAX_SCHED_ID + 1];
    struct nfp_nbi_tm_l0_sched_cfg l0_schedcfg;
    struct nfp_nbi_tm_blq_config blq_config[4];
};

/**
 * Function to initialize the traffic manager from a configuration
 * @ingroup nfp6000-only
 *
 *@param       nbi             NBI device
 *@param       file            configuration file
 *@return                       <0 on error
 */
NFP_NBI_API int nfp_nbi_tm_init(struct nfp_nbi_dev *nbi, const char *file);

/**
 * Function to load the traffic manager configuration from a file
 * @ingroup nfp6000-only
 *
 *@param       cfg             configuration struct
 *@param       fp              configuration file pointer
 *@return                       <0 on error
 */
NFP_NBI_API int nfp_nbi_tm_load_config(struct nfp_nbi_tm_cfg *cfg,
				       FILE * fp);

/**
 * Function to initialize the traffic manager from a configuration struct
 * @ingroup nfp6000-only
 *
 *@param       nbi             NBI device
 *@param       cfg             configuration struct
 *@return                       <0 on error
 */
NFP_NBI_API int nfp_nbi_tm_cfg(struct nfp_nbi_dev *nbi,
			       const struct nfp_nbi_tm_cfg *cfg);

/**
 * Write global traffic manager configuration
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	tm_gbl_cfg	a struct nfp_nbi_tm_gbl_trf_mngr_cfg object to be populated
 *			        with traffic manager parameters to be configured
 *@return			<0 on error
 */
NFP_NBI_API int nfp_nbi_tm_write_gbl_cfg(struct nfp_nbi_dev *nfpnbi, const struct
					 nfp_nbi_tm_gbl_trf_mngr_cfg
					 *tm_gbl_cfg);


/**
 *Read global traffic manager configuration
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	tm_gbl_cfg	a struct nfp_nbi_tm_gbl_trf_mngr_cfg object that will be
 *				updated with the current traffic manager config
 *@return			<0 on error
 */
NFP_NBI_API int nfp_nbi_tm_read_gbl_cfg(struct nfp_nbi_dev *nfpnbi, struct nfp_nbi_tm_gbl_trf_mngr_cfg
					*tm_gbl_cfg);


/**
 * Configure global traffic manager scheduling
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	enable	Set enable 1 to enable traffic manager global scheduling
 *@return		<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_sched_cfg(struct nfp_nbi_dev *nfpnbi, bool enable);


/**
 * Configure mac/traffic manager out of band flow control
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        enable          Set enable 1 to enable out of band flow control
 *@return                       <0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_oobfc_cfg(struct nfp_nbi_dev *nfpnbi, bool enable);

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
 *@param	drp_rate_cfg	a struct nfp_nbi_tm_gbl_drop_rate_cfg object to be updated
 *				with drop rate parameters to be configured
 *@return			<0 on error
 */
NFP_NBI_API int nfp_nbi_tm_write_drop_rate_cfg(struct nfp_nbi_dev *nfpnbi, const struct
					       nfp_nbi_tm_gbl_drop_rate_cfg
					       *drp_rate_cfg);


/**
 * Read traffic manager global drop rate configuration
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	     tm device handle
 *@param	drp_rate_cfg a struct nfp_nbi_tm_gbl_drop_rate_cfg object that will be
 *                           updated with the drop rates configuration
 *@return		     <0 on error
 */
NFP_NBI_API int nfp_nbi_tm_read_drop_rate_cfg(struct nfp_nbi_dev *nfpnbi, struct nfp_nbi_tm_gbl_drop_rate_cfg
					      *drp_rate_cfg);



/**
 * Dump global traffic manager configuration
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi              tm device handle
 *@return                       <0 on error
 */
NFP_NBI_API int nfp_nbi_tm_gbl_cfg_dump(struct nfp_nbi_dev *nfpnbi);

/**
 * Write the queue paramaters for the queue with the provided qid
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	qid	queue identifier for a queue : [0-1023]
 *@param	qparams a struct nfp_nbi_tm_q_params_cfg object that needs to be populated
 *			with the queue paramters to be configured
 *@return		<0 on error
 */
NFP_NBI_API int nfp_nbi_tm_write_q_cfg(struct nfp_nbi_dev *nfpnbi, int qid, const struct nfp_nbi_tm_q_params_cfg
				       *qparams);


/**
 * Read the queue paramaters for the queue with the provided qid
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	qid	queue identifier for a queue : [0-1023]
 *@param	qparams a struct nfp_nbi_tm_q_params_cfg object that will be updated with
 *			the queue paramters
 *@return		<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_read_q_cfg(struct nfp_nbi_dev *nfpnbi, int qid,
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
    int nfp_nbi_tm_read_q_dropcnt(struct nfp_nbi_dev *nfpnbi, int qid,
				  uint32_t * dropcnt, bool clear);


/**
 * Read the queue status for the queue with the provided qid
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	qid	queue identifier for a queue : [0-1023]
 *@param	qst	struct nfp_nbi_tm_q_st object will be updated with the queue status
 *@return		<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_read_q_status(struct nfp_nbi_dev *nfpnbi, int qid,
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
					  int qid, uint32_t q_headp);


/**
 * Read the current head and tail sram pointers for a queue
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	qid	queue identifier for a queue : [0-1023]
 *@param	qpntrs	a nfp_nbi_tm_q_desc_pntrs object that will be updated with the
 *                      head and tail pointers of a queue
 *@return		<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_read_headtailsram_cfg(struct nfp_nbi_dev *nfpnbi,
					 int qid,
					 struct nfp_nbi_tm_q_desc_pntrs
					 *qpntrs);


/**
 * Dump queue configuration
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        qid             queue identifier for a queue : [0-1023]
 *@return                       <0 on error
 */
NFP_NBI_API int nfp_nbi_tm_q_cfg_dump(struct nfp_nbi_dev *nfpnbi, int qid);


/**
 * Dump the current status for a given queue
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        qid             the queue identifier number : [0-1023]
 *@param        clear           clear, if set, will clear the queue drop counter register for the given qid
 *@return                       <0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_q_status_dump(struct nfp_nbi_dev *nfpnbi, int qid,
				 bool clear);

/**
 * Configure the shaper with the given shaper id with the shaper parameters
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi  tm device handle
 *@param	shp_id  the shaper identifier : [0-144]
 *@param	shpcfg	a nfp_nbi_tm_shp_cfg struct object that is to be populated with
 *                      the shaper config parameters
 *@return		<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_write_shp_cfg(struct nfp_nbi_dev *nfpnbi, int shp_id,
				 const struct nfp_nbi_tm_shp_cfg *shpcfg);


/**
 * Read the configuration of the shaper with the given shaper id
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	shp_id	the shaper identifier : [0-144]
 *@param	shpcfg	a struct nfp_nbi_tm_shp_cfg object for returning shaper config
 *			parameters
 *@return		<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_read_shp_cfg(struct nfp_nbi_dev *nfpnbi, int shp_id,
				struct nfp_nbi_tm_shp_cfg *shpcfg);


/**
 * Read the shaper status of the shaper with the given shaper id
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param	shp_id	the shaper identifier : [0-144]
 *@param	shpst	a struct nfp_nbi_tm_shp_st object that will be updated with the
 *			shaper parameters
 *@return		<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_read_shp_status(struct nfp_nbi_dev *nfpnbi, int shp_id,
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
				   const struct nfp_nbi_tm_sched_cfg
				   *schedcfg);


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
    int nfp_nbi_tm_read_sched_cfg(struct nfp_nbi_dev *nfpnbi, int sched_id,
				  struct nfp_nbi_tm_sched_cfg *schedcfg);


/**
 * Configure the dwrr weights for the ports of the scheduler with the given
 * scheduler id
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	sched_id	the scheduler identifier (L2 and L1 schedulers only) : [0-144]
 *@param	wgt		a struct dwrr_wgt_cfg object
 *@return			<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_write_sched_dwrr_cfg_wgt(struct nfp_nbi_dev *nfpnbi,
					    int sched_id,
					    const uint32_t wgt[]);


/**
 * Read the dwrr weights of the scheduler ports
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	sched_id	the scheduler identifier (L2 and L1 schedulers only) : [0-144]
 *@param	wgt		a struct dwrr_wgt_cfg object
 *@return			<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_read_sched_dwrr_cfg_wgt(struct nfp_nbi_dev *nfpnbi,
					   int sched_id, uint32_t wgt[]);


/**
 * Read the dwrr deficits for the ports of the scheduler with the given
 * scheduler id
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	sched_id	the scheduler identifier (L2 and L1 schedulers only) : [0-144]
 *@param	dft		a struct dwrr_dfct_cfg object that will be
 *				updated with the deficits for the ports of the
 *				scheduler
 *@return			<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_read_sched_dwrr_cfg_dft(struct nfp_nbi_dev *nfpnbi,
					   int sched_id, uint32_t dft[]);


/**
 * Initialize the dwrr deficits to zero for a given L2/L1 scheduler
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        sched_id        the scheduler identifier (L2 and L1 schedulers only): [0-144]
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
 *@param	l0_schedcfg  a nfp_nbi_tm_l0_sched_cfg object to be populated with the weights and deficits
 *			 port
 *@return		 <0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_write_sched_l0_dwrr_cfg(struct nfp_nbi_dev *nfpnbi,
					   const struct
					   nfp_nbi_tm_l0_sched_cfg
					   *l0_schedcfg);


/**
 * Read the dwrr weight for a port of the l0 scheduler
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi	tm device handle
 *@param        l0_schedcfg  a nfp_nbi_tm_l0_sched_cfg object to be populated with the weights and deficits
 *@return	<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_read_sched_l0_dwrr_cfg(struct nfp_nbi_dev *nfpnbi,
					  struct nfp_nbi_tm_l0_sched_cfg
					  *l0_schedcfg);

/**
 * Dump the configuration for a given scheduler
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        sched_id        scheduler identifier number : [0-144]
 *@return                       <0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_sched_cfg_dump(struct nfp_nbi_dev *nfpnbi,
				  int sched_id);



/**
 * Read the out of order sequencer count for the sequencer id
 * @ingroup nfp6000-only
 *
 *@param	 nfpnbi		tm device handle
 *@param	 seq_id		the sequencer identifier : [0-31]
 *@param	 ooo_cnt	paramter to be updated with the out of order
 *				count for the given sequencer id
 *@param	 clear		if 1,clears the out of order count register on
 *				a read
 *@return			<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_read_ooo_cnt(struct nfp_nbi_dev *nfpnbi, int seq_id,
				uint32_t * ooo_cnt, bool clear);


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
				       int seq_id, bool * reord_act);


/**
 * Dump the configuration for a given sequencer
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        seq_id          sequencer identifier number : [0-31]
 *@param        clear           clear, if set, will clear the out of order count for the sequencer
 *                              identified by seq_id
 *@return                       <0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_seq_dump(struct nfp_nbi_dev *nfpnbi, int seq_id,
			    bool clear);


/**
 * Configure the minipacket credits
 * @ingroup nfp6000-only
 *
 *@param	nfpnbi		tm device handle
 *@param	crdt_cfg	a struct nfp_nbi_tm_minipkt_crdts_cfg object to be
				populated with the credit config parameters
 *@return			<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_write_minipkt_crdt_cfg(struct nfp_nbi_dev *nfpnbi,
					  const struct
					  nfp_nbi_tm_minipkt_crdts_cfg
					  *crdt_cfg);


/**
 * Read the minipacket credits configuration
 * @ingroup nfp6000-only
 *
 *@param	 nfpnbi		tm device handle
 *@param	 crdt_cfg	a struct nfp_nbi_tm_minipkt_crdts_cfg object that is to be
 *				updated with the credit configuration parameters
 *@return			<0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_read_minipkt_crdt_cfg(struct nfp_nbi_dev *nfpnbi, struct
					 nfp_nbi_tm_minipkt_crdts_cfg
					 *crdt_cfg);



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
					   int chl, uint32_t * chan_crdts);


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
					 bool fp_num, uint32_t * fp_crdts);


/**
 * Dump minipacket credits configuration
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@return                       <0 on error
 */
NFP_NBI_API int nfp_nbi_tm_minipkt_cfg_dump(struct nfp_nbi_dev *nfpnbi);


/**
 * Dump the current minipacket credits for a given channel
 * @ingroup nfp6000-only
 *
 *@param        nfpnbi          tm device handle
 *@param        chl             the channel number : [0-127]
 *@return                       <0 on error
 */
NFP_NBI_API
    int nfp_nbi_tm_chan_minipkt_crdts_dump(struct nfp_nbi_dev *nfpnbi,
					   int chl);

/**
 * Configure NFP NBI TM Buffer List Queue Event Control register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param blqevnt_cfg	NFP NBI TM BLQ Event Config structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API int nfp_nbi_tm_cfg_blq_event(struct nfp_nbi_dev *nfpnbi, struct nfp_nbi_tm_blq_event_config
					 *blqevnt_cfg);
/**
 * Read NFP NBI TM Buffer List Queue Event register
 * @ingroup nfp6000-only
 *
 * @param nfpnbi	NFP NBI device handle
 * @param data		NFP NBI TM BLQ Event Config structure
 *
 * @return 0 on success, -1 on failure.
 */
NFP_NBI_API int nfp_nbi_tm_read_blq_event(struct nfp_nbi_dev *nfpnbi, struct nfp_nbi_tm_blq_event_config
					  *data);

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
    int nfp_nbi_tm_read_blq_status(struct nfp_nbi_dev *nfpnbi, int blq,
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
			       uint64_t htsram_start, uint64_t count,
			       uint64_t * data);

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
			       uint64_t bdsram_start, uint64_t count,
			       uint64_t * data);

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
			       uint64_t pktsram_start, uint64_t count,
			       uint64_t * data);

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
			       uint64_t descsram_start, uint64_t count,
			       uint64_t * data);

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
			       uint64_t reorderbuf_start, uint64_t count,
			       uint64_t * data);
#endif
