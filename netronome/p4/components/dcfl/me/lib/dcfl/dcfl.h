/*
 * Copyright (C) 2013 Netronome Systems, Inc.  All rights reserved.
 *
 * @file        dcfl.h
 * @brief       DCFL ME library header file.
 */

#ifndef _DCFL_H_
#define _DCFL_H_

#if defined(__NFP_LANG_MICROC)
#include <stdint.h>
#include <types.h>
#ifdef DCFL_PIPELINED
#include <nfp/mem_ring.h>
#endif
#endif

#define DCFL_LKP_OVERFLOW_CNTR      dcfl_lkp_overflow_counter
#define DCFL_CROSS_OVERFLOW_CNTR    dcfl_cross_overflow_counter
#define DCFL_MAX_LABEL              0xFFFF
#define DCFL_MAX_LABELS_PER_LKP     32

/* For dcfl_in_me app */
#define DCFL_READ_BUF_LW            16
#define DCFL_WRITE_BUF_LW           16
#define DCFL_LABELSET_LW            16

/* Magic numbers for NFP6xxx */
#define DCFL_NUM_ISLANDS            7
#define DCFL_MAX_APP_ME_PER_ISLAND  12
#define DCFL_MAX_APP_CTX_PER_ME     8

/*
 * Rings and queues
 */
#define DCFL_KEYQ_NAME              dcfl_request_ring
#define DCFL_KEYQ_ENTRIES           131072
/* 1<<17 allows 131072/DCFL_KEY_WORKQ_LW=16K requests to be tested */

#define DCFL_ANQ_NAME               dcfl_an_ring
/* power of 2 higher than MAX_FIELDS=40: */
#define DCFL_ANQ_ENTRIES            DCFL_KEYQ_ENTRIES * 64

#define DCFL_RESJRNL_NAME           dcfl_results_jrnl
#define DCFL_TESTRES_RING_SIZE_LW   DCFL_KEYQ_ENTRIES * 16

#define DCFL_HIT_COUNTER_NAME       dcfl_hit_counter
#define DCFL_HIT_COUNTER_SIZE       64

#define DCFL_DBGJRNL_NAME   dcfl_dbgjrnl
#define DCFL_DBGJRNL_QNUM   20
#define DCFL_DBGJRNL_SIZE   2048
#ifdef DCFL_DEBUG_HITS
#define DCFL_DBGJRNL_BUFLEN 4
#else
#define DCFL_DBGJRNL_BUFLEN 2
#endif

#define DCFL_MAX_TEST_REQUESTS      128
#define DCFL_KEYS_NAME              dcfl_keys
#define DCFL_KEY_SIZE_LW            64
#define DCFL_KEYS_SIZE_LW           (DCFL_MAX_TEST_REQUESTS * DCFL_KEY_SIZE_LW)

#define DCFL_RESULTS_SIZE_LW        (DCFL_MAX_APP_CTX_PER_ME        \
                                    * DCFL_MAX_APP_ME_PER_ISLAND    \
                                    * DCFL_NUM_ISLANDS              \
                                    * DCFL_MAX_LABELS_PER_LKP / 2)
#if defined(__NFP_LANG_MICROC)

#define DCFL_KEY_WORKQ_LW   8
#define DCFL_AN_WORKQ_LW    13

#define DCFL_STAGE_SIG      15

/* Bits to mask on thread_info structure to get interthread_signal value */
#define DCFL_ITHREAD_MASK           0xFF001FFC

struct dcfl_thread_info {
    union {
        __packed struct {
            uint32_t isl_id     : 8;
            uint32_t test_ct_num:11;
            uint32_t me         : 4;
            uint32_t ctx        : 3;
            uint32_t sig        : 4;
            uint32_t reserved   : 2;
        };
        uint32_t value;
    };
};

struct dcfl_key_workq_entry {
    union {
        __packed struct {
            struct dcfl_thread_info thread_info;
            uint64_t key_addr;
            uint64_t result_addr;
            uint32_t size;
            __addr40 __imem uint8_t *cfg_ptr;
        };
        uint32_t values[DCFL_KEY_WORKQ_LW];
    };
};

struct dcfl_an_workq_entry {
    union {
        __packed struct {
            uint32_t field_idx;
            __addr40 __mem uint8_t *key_ptr;
            __addr40 __mem uint8_t *cfg_ptr;
            __addr40 __mem struct dcfl_label_set *labelset_ptr;
            __addr40 __mem uint32_t *nlabels_ptr;
            __addr40 __mem uint32_t *thread_info_ptr;
            uint32_t labelset_idx;
            struct dcfl_thread_info app_thread_info;
        };
        uint32_t values[DCFL_AN_WORKQ_LW];
    };
};

struct dcfl_label_set {
    union {
        struct {
            uint16_t labels[32];
        };
        __packed struct {
            uint32_t priority : 16;
            uint32_t rule_id  : 16;
        } results[DCFL_LABELSET_LW];
        uint32_t values[DCFL_LABELSET_LW];
    };
};

struct dcfl_me_custom_config {
#ifdef DCFL_CUSTOM_TABLE_LW
    uint32_t custom_data[DCFL_CUSTOM_TABLE_LW];
#endif
};

/**
 * Read custom configuration data from the active buffer, and return a pointer
 * to the start of the active buffer.
 *
 * @param table
 * @param custom_cfg
 */
__addr40 __imem uint8_t *dcfl_get_conf(uint32_t table,
    __xread void *custom_cfg);

/**
 * Bubble sort results.
 *
 * Comparing the whole 32 bits means that results are first
 * sorted by priority (increasing), then by rule_id
 * (increasing). */
int dcfl_sort_results(__lmem struct dcfl_label_set *results,
                  uint32_t nresults);

/**
 * Performs a DCFL classification with a classification 'key' over a
 * classification 'table'. It uses local mem storage for the outcome
 * of the internal stages. Results are stored in one of the local mem
 * storage, pointed out by 'labels_id'
 *
 * @param  key         generic classification key
 * @param  table       Classification table where to classify on.
 * @param  labels      1 array of 3 dcfl_label_set structs living
 *                     in local memory.
 * @return labels_id   Id of the dcfl_label_set with the results.
 *                     (if return != 0)
 * @return             Numbers of rules hit with 'key'.
 *
 */
uint32_t dcfl_classify(void *key,
                       uint32_t table,
                       __lmem struct dcfl_label_set *labels,
                       uint32_t *labels_id, __addr40 __imem uint8_t *cfg_ptr);


#ifdef DCFL_PIPELINED
/**
 * Get island, ME nr and context number of this thread, and pack into
 * structure along with the signal that is to be set when DCFL worker
 * completes.
 *
 * @param sig_num   Signal number declared as volatile in this ME, to be
 *                  set with an interthread_signal call.
 */
struct dcfl_thread_info
dcfl_get_thread_info(int sig_num);

/**
 * Do one stage of a DCFL pipeline: do one (or two) lookup(s) and find the
 * crossproduct of their matched labels. Create work on work queue for next
 * stage or assert signal to indicate completion to dcfl_classify(). One or
 * more worker MEs need to call this function in an infinite loop to consume
 * the work
 *
 * @param anq_entry     Struct for an entry of the relevant AN ring
 * @param anq_raddr     Address to the relevant AN ring
 */
void
dcfl_pipeline_stage(struct dcfl_an_workq_entry *anq_entry,
                    mem_ring_addr_t anq_raddr
);

/**
 * Performs a DCFL classification with a classification 'key' over a
 * classification 'table'. Keys and results in DRAM. Lookups and crossproduct
 * are pipelined in a ring identified by anq_raddr. This means that more than
 * one ring can be created, to service specific app threads/MEs.
 *
 * @param  key         generic classification key
 * @param  table       Classification table where to classify on.
 * @param  label_set   Pointer to memory space containing label sets
 * @param  result_idx  Index to the label set that contains final results.
 * @param  cfg_ptr     Pointer to this table's configuration
 * @param  anq_raddr   Address to the relevant AN ring.
 * @return             Number of matches
 *
 */
uint32_t dcfl_classify_pipeline(__addr40 __mem uint8_t *key,
                       uint32_t table,
                       __addr40 __mem struct dcfl_label_set *label_set,
                       __addr40 __mem uint32_t *nlabels,
                       __addr40 __imem uint8_t *cfg_ptr,
                       mem_ring_addr_t anq_raddr
                      );

#endif

#endif

#endif /* _DCFL_H_ */
