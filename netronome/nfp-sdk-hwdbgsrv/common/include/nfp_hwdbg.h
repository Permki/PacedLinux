/*
 * Copyright (C) 2014,  Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __NFP_HWDBG_H__
#define __NFP_HWDBG_H__

#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/cdefs.h>

/* Import/Export symbols for MSC and set visibility for GCC */
#if defined(NO_NFP_HWDBG_API_EXPORT)
    #define NFP_HWDBG_API
#elif defined(_MSC_VER)
    #ifdef NFP_HWDBG_API_EXPORT
        #define NFP_HWDBG_API __declspec(dllexport)
    #else
        #define NFP_HWDBG_API __declspec(dllimport)
    #endif
#else
    #if __GNUC__ >= 4
        #define NFP_HWDBG_API __attribute__ ((visibility ("default")))
    #else
        #define NFP_HWDBG_API
    #endif
#endif


/** Opaque NFP hardware debug handle */
struct nfp_hwdbg;


__BEGIN_DECLS;

#ifdef _WIN32
    #define __PACKED__
    #pragma pack(push, 1)
#else
    #define __PACKED__ __attribute__ ((__packed__))
#endif

/** Structure containing information about a break */
struct nfp_hwdbg_break_data {
    /** If greater or equal to zero, the user-assigned code breakpoint ID,
      * if less than zero, an ID automatically assigned to a software breakpoint
      */
    int64_t breakid;
    /** The MicroEngine ID on which the break occurred */
    uint16_t meid;
    /** The MicroEngine context on which the break occurred */
    uint8_t ctx;
    /** The program counter at which the break occurred */
    uint16_t pc;
} __PACKED__;

/** State of an ME */
enum NFP_HWDBG_ME_STATUS {
    NFP_HWDBG_ME_STATUS_ACTIVE = 0,    /** Active context is running */
    NFP_HWDBG_ME_STATUS_ATBREAK = 1,   /** ME is stopped at a break */
    NFP_HWDBG_ME_STATUS_INACTIVE = 2,  /** ME has no contexts running */
    NFP_HWDBG_ME_STATUS_DISABLED = 3,  /** Debugging the ME has been disabled */
};

/** Structure containing the run state for all contexts of an ME */
struct nfp_hwdbg_me_status_data {
    /** State of ME */
    uint8_t state;

    /** CTX and PC of running, stopped or inactive ME */
    uint8_t ctx;
    uint16_t pc;

    /** Context enables */
    uint8_t ctx_enables;
    /* TODO: TBD */
    uint32_t enables;

    /** Other ME state */
    uint32_t timestamp_low;
    uint32_t timestamp_high;
    uint32_t profile_cnt;

    uint16_t ctxpc[8];
} __PACKED__;

#ifdef _WIN32
    #pragma pack(pop)
#endif

/** @file nfp_hwdbg.h
 *  @addtogroup nfhal NFHAL
 *  @{
 */

/** @name Functions for interacting with hardware debug features with NFP devices.
 *  @{
 */

/** Event callback function pointer type */

/** Event payload structure for ME group stop */
struct nfp_hwdbg_event_pl_groupstop {
    int meid;
};

/** Event payload structure for NFP_HWDBG_EVENT_TYPE_SHUTDOWN */
struct nfp_hwdbg_event_pl_shutdown {
    int arg; /* usage TBD */
};

/** Event payload structure for NFP_HWDBG_EVENT_TYPE_RESET */
struct nfp_hwdbg_event_pl_reset {
    int type; /* RESET one of NFP_HWDBG_SRV_RESET_TYPES */
};

/** Types of hardware debug events */
enum NFP_HWDBG_EVENT_TYPES {
    /** Invalid type */
    NFP_HWDBG_EVENT_TYPE_INVALID = 0,
    /** Break event - struct nfp_hwdbg_break_data */
    NFP_HWDBG_EVENT_TYPE_BREAK = 1,
    /** ME within break group stop - nfp_hwdbg_event_pl_groupstop */
    NFP_HWDBG_EVENT_TYPE_GROUPSTOP = 2,
    /** Server is shutting down - nfp_hwdbg_event_pl_shutdown */
    NFP_HWDBG_EVENT_TYPE_SHUTDOWN = 3,
    /** Server data has been reset, generally indicates a firmware load */
    NFP_HWDBG_EVENT_TYPE_RESET = 4,
    /** end of enum indicator */
    NFP_HWDBG_EVENT_TYPE_LAST,
};

/** Types of hardware debug reset actions */
enum NFP_HWDBG_SRV_RESET_TYPES {
    /** Invalid type */
    NFP_HWDBG_SRV_RESET_TYPE_INVALID = 0,
    /** Soft reset, keep existing code breakpoints */
    NFP_HWDBG_SRV_RESET_TYPE_SOFTRESET = 1,
    /** Hard reset, remove all code breakpoints */
    NFP_HWDBG_SRV_RESET_TYPE_HARDRESET = 2,
    /** Loader reset, put the server in wait state
     *  this is required for co-ordinating loading
     *  while debug server is running
     */
    NFP_HWDBG_SRV_RESET_TYPE_LOADER = 3,
};

/** Types of hardware debug break flushes */
enum NFP_HWDBG_FLUSH_TYPES {
    /** Invalid type */
    NFP_HWDBG_FLUSH_TYPE_INVALID = 0,
    /** Flush breaks that allocated by provided handle */
    NFP_HWDBG_FLUSH_TYPE_CLIENT = 1,
    /** Flush all breaks */
    NFP_HWDBG_FLUSH_TYPE_ALL = 2,
};

/* Macros for manipulating debug info */

/** Instruction is ctx_arb[kill] */
#define NFP_HWDBG_DBGINFO_CTXARBKILL     (1 << 0)
/** A breakpoint is allowed at a location; i.e not in a defer slot */
#define NFP_HWDBG_DBGINFO_BRKPTALLOWED   (1 << 1)
/** Instruction was ctx_arb[bpt] */
#define NFP_HWDBG_DBGINFO_SWBRKPT        (1 << 2)
#define NFP_HWDBG_DBGINFO_DEFERCNT(x)    (((x) & 0xf) << 4)
#define NFP_HWDBG_DBGINFO_DEFERCNT_of(x) (((x) >> 4) & 0xf)
/** Branch address; -1 if no branch applies */
#define NFP_HWDBG_DBGINFO_BRADDR(x)      (((x) & 0xffff) <<  8)
#define NFP_HWDBG_DBGINFO_BRADDR_of(x)   ((int16_t)(((x) >> 8) & 0xffff))
/** Branch register types; types match debug data */
#define NFP_HWDBG_DBGINFO_REGTYPE(x)     (((x) & 0xff)   << 24)
#define NFP_HWDBG_DBGINFO_REGTYPE_of(x)  (((x) >> 24) & 0xff)
/** Branch register address */
#define NFP_HWDBG_DBGINFO_REGADDR(x)     (((uint64_t)((x) & 0xffff)) << 32)
#define NFP_HWDBG_DBGINFO_REGADDR_of(x)  (((x) >> 32) & 0xffff)

/**
 * Open NFP hardware debug handle.
 *
 * @param devnum        NFP device number
 *
 * @return device struct pointer, or NULL on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
struct nfp_hwdbg * nfp_hwdbg_open(unsigned int devnum);

/**
 * Free up resources and close NFP hardware debug handle.
 *
 * @param hwdbg           NFP hardware debug handle
 */
NFP_HWDBG_API
void nfp_hwdbg_close(struct nfp_hwdbg *hwdbg);

/**
 * Determine the number of attached clients
 *
 * @param hwdbg           NFP hardware debug handle
 *
 * @return the number of connected client, or -1 on failure
 */
NFP_HWDBG_API
int nfp_hwdbg_client_count(struct nfp_hwdbg *hwdbg);

/**
 * Reset all debug data within the server
 *
 * @param hwdbg           NFP hardware debug handle
 * @param resettype       Reset type, refer to NFP_HWDBG_SRV_RESET_TYPES
 *
 * This function is used to clear local state within the debug server
 * between ME code loads. It is possible to reset this state while
 * keeping all existing code breakpoints by setting @a resettype to
 * NFP_HWDBG_SRV_RESET_TYPE_SOFTRESET.
 *
 */
NFP_HWDBG_API
void nfp_hwdbg_srv_reset(struct nfp_hwdbg *hwdbg, int resettype);

/**
 * Specify a list of MicroEngines that will debuggable
 *
 * @param hwdbg         NFP hardware debug handle
 * @param meids         array of MEs to that will be enabled
 * @param len           length of breakmeids array
 *
 * All MEs not specified in the meids array will having debugging disabled.
 * This function must be called while the debugger is detached ie when
 * a NFP_HWDBG_SRV_RESET_TYPE_LOADER reset in in place. If debugging is
 * enabled the call will fail with EBUSY.
 *
 * Note that at start up no ME exclusions will take place,
 * unless specified from the command line. The enabled list will apply until
 * it is changed again by another function call
 *
 * If @a len is 0, all MicroEngines that are loaded will be enabled.
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_set_enabled_mes(struct nfp_hwdbg *hwdbg, int *meids, int len);

/**
 * Get the status of the one or many MicroEngines
 *
 * @param hwdbg         NFP hardware debug handle
 * @param meids         Input array of MicroEngine IDs
 * @param status        Output array of MicroEngine statuses
 * @param len           Length of input and output arrays
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_me_status(struct nfp_hwdbg *hwdbg, int *meids, struct nfp_hwdbg_me_status_data *status, int len);

/**
 * Stop a MicroEngine
 *
 * @param hwdbg         NFP hardware debug handle
 * @param meid          MicroEngine ID
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_me_stop(struct nfp_hwdbg *hwdbg, int meid);

/**
 * Run or resume a MicroEngine
 *
 * @param hwdbg         NFP hardware debug handle
 * @param meid          MicroEngine ID
 * @param resume        Resume if Microengine has uncleared breaks
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_me_run(struct nfp_hwdbg *hwdbg, int meid, int resume);

/**
 * Step a MicroEngine
 *
 * @param hwdbg         NFP hardware debug handle
 * @param meid          MicroEngine ID
 * @param ctx           Context to step, -1 for active context
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_me_step(struct nfp_hwdbg *hwdbg, int meid, int ctx);

/**
 * Add an ME break group
 *
 * @param hwdbg         NFP hardware debug handle
 * @param meid          MicroEngine ID to add break group * @param breakmeids    Array of MEs to stop when a break on meid is hit
 * @param len           length of breakmeids array
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_brkgrp_add(struct nfp_hwdbg *hwdbg, int meid, int *breakmeids, int len);

/**
 * Delete an ME break group
 *
 * @param hwdbg         NFP hardware debug handle
 * @param meid          MicroEngine ID to add break group to
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_brkgrp_del(struct nfp_hwdbg *hwdbg, int meid);

/**
 * Add a code breakpoint
 *
 * @param hwdbg         NFP hardware debug handle
 * @param breakid       User-assigned BreakID
 * @param meid          MicroEngine ID
 * @param pc            Program counter value
 * @param ctxmask       Context mask, 0 will enable break on all contexts
 *
 * Use this function to add a code breakpoint on a specific program counter
 * value. The break can apply to one or many contexts. The user must allocate
 * the break an ID which is used for further references to that break. Note that
 * negative BreakIDs are reserved for soft breaks and can't be used.
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_brk_add_code(struct nfp_hwdbg *hwdbg, int64_t breakid, int meid, int pc, unsigned int ctxmask);

/**
 * Delete a code breakpoint
 *
 * @param hwdbg         NFP hardware debug handle
 * @param breakid       User-assigned BreakID
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_brk_del(struct nfp_hwdbg *hwdbg, int64_t breakid);

/**
 * Flush code breakpoints
 *
 * @param hwdbg         NFP hardware debug handle
 * @param flushtype     Type of flush, see NFP_HWDBG_FLUSH_TYPES
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_brk_flush(struct nfp_hwdbg *hwdbg, int flushtype);

/**
 * Check for outstanding breaks
 *
 * @param hwdbg         NFP hardware debug handle
 * @param data          Return array for outstanding break data
 * @param len           Maximum return array length
 * @param cnt           Number of break entries returned or,
 *                      if @a len is zero, the number of entries available
 *
 * @return 0 on success, 1 more breaks are available,
 *  or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_brk_check(struct nfp_hwdbg *hwdbg, struct nfp_hwdbg_break_data *data, int len, int *cnt);

/**
 * Clear an outstanding break
 *
 * @param hwdbg         NFP hardware debug handle
 * @param breakid       BreakID to clear (Can be software or code break)
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_brk_clear(struct nfp_hwdbg *hwdbg, int64_t breakid);

/**
 * Load debug info for a loaded ME
 *
 * @param hwdbg         NFP hardware debug handle
 * @param meid          MicroEngine ID to load info for
 * @param data          Debug data array
 * @param len           Debug data array length
 * @param off           PC offset at which debud info starts
 *
 * Load debug info for an ME; debug info is loaded as an array of encoded
 * uint64_ts. Each element in the array corresponds to a PC offset. Refer
 * to the NFP_HWDBG_DBGINFO_ macros for encoding format.
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_load_dbginfo(struct nfp_hwdbg *hwdbg, int meid, uint64_t *data, int len, int off);

/**
 * Register a hardware debug event handler
 *
 * @param hwdbg         NFP hardware debug handle
 * @param event_cb      Event handler callback function
 *
 * @return 0 on success, or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_subs_events_all(struct nfp_hwdbg *hwdbg, void (*event_cb)(struct nfp_hwdbg *hwdbg, uint32_t ev_type, void *ev_data, uint32_t ev_data_len));

/**
 * Retrieve the remote hardware debug server version string
 *
 * @param hwdbg       NFP hardware debug handle
 * @param buf         Return buffer, contains NULL-terminated string on
 *                    success
 * @param len         Size of @a buf in bytes
 *
 * Retrieve the remote server version string. Note that if @a len is less than
 * or equal to zero, the remote server string length will be returned without
 * @a buf being populated.
 *
 * @returns string length including NULL termination characte on success,
 *  or -1 on failure (and sets errno accordingly).
 */
NFP_HWDBG_API
int nfp_hwdbg_server_version(struct nfp_hwdbg *hwdbg, char *buf, int len);

/**
 * Retrieve and check the RPC version numbers from the client libraries
 * and remote server
 *
 * @param cln_ver           Client library version
 * @param cln_compat        Client library compatibility version
 * @param srv_ver           RPC server version
 * @param srv_compat        RPC server compatibility version
 *
 * @returns -1 on error and 0 on success, and 1 if not-compatible
 */
NFP_HWDBG_API
int nfp_hwdbg_rpc_version(int *cln_ver, int *cln_compat, int *srv_ver, int *srv_compat);

/** @}
 * @}
 */

__END_DECLS;

#endif /* !__NFP_HWDBG_H__ */
