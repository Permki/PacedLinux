/*
 * Copyright (C) 2009-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __DL_IPHDR_UC__
#define __DL_IPHDR_UC__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file dl_iphdr.uc IP Header Caching Macros
 * @addtogroup dl_iphdr IP Header Caching
 * @{
 *
 * @name IP Header Caching Macros
 * @{
 *
 */

/** This macro reads the IP header (32 bytes) of the packet from SDRAM and keeps
 * it in a cache (DRAM transfer registers).
 *
// Output:  The $$dl_iphdr# global dram transfer registers will be set with
//          appropriate header data.
//
 * @param buf_handle   Handle of buffer
 * @param req_sig      Signal to use in the I/O (DRAM read) operation
 * @param sig_action   What to do with I/O operation:
 *                     @arg Wait for signal(s) as specified by sig_action
 *                     @arg Do not wait for signal, just return (SIG_NONE)
//      Size:
 */
#define dl_load_iphdr_cache     dl_iphdr_load_cache

#macro dl_iphdr_load_cache(buf_handle, req_sig, sig_action)
.begin

    .reg    sdram_offset, buf_offset

    //  Read 32 bytes of IP Header from SDRAM

    dl_buf_get_data(sdram_offset, buf_handle)
    dl_meta_get_offset(buf_offset)
    dram[read, $$iphdr0, sdram_offset, buf_offset, 4], sig_done[req_sig]    // DRAM reads are in QWORD (8 bytes)

    #if (!streq('sig_action', 'SIG_NONE'))
        ctx_arb[sig_action]
    #endif

.end
#endm


/** This macro writes back the IP header (32 bytes) from the cache to the packet
 * in SDRAM.
 *
 * @param buf_handle   Handle of buffer
 * @param req_sig      Signal to use in the I/O (DRAM read) operation
 * @param sig_action   What to do with I/O operation:
 *                     @arg Wait for signal(s) as specified by sig_action
 *                     @arg Do not wait for signal, just return (SIG_NONE)
 *
 * The $$dl_iphdr# global dram transfer registers will be the set
 * of registers from which data will be written back to SDRAM.
 *
//      Size
 */
#define dl_flush_iphdr_cache        dl_iphdr_flush_cache

#macro dl_iphdr_flush_cache(buf_handle, req_sig, sig_action)
.begin

    .reg    sdram_offset, buf_offset

    //  Write 32 bytes of IP Header from SDRAM

    dl_buf_get_data(sdram_offset, buf_handle)
    dl_meta_get_offset(buf_offset)
    dram[write, $$iphdr0, sdram_offset, buf_offset, 4], sig_done[req_sig]   // DRAM reads are in QWORD (8 bytes)

    #if (!streq('sig_action', 'SIG_NONE'))
        ctx_arb[sig_action]
    #endif

.end
#endm


/** This macro needs to be called before any of the dl_iphdr_*
 * can be called.
 *
 * It allocates a set of DRAM xfer registers to be used
 * for caching. This macro does not generate any code (instructions).
 * It only produces some assembler directives.
 *
 * @instruction_cnt 0
 */
#macro dl_iphdr_cache_init()
    xbuf_alloc($$iphdr, 8, read_write)
#endm

/** This macro needs to be called at the end when dl_iphdr_macros are
 * no longer required.
 *
 * It frees a set of DRAM xfer registers that were
 * allocated using dl_iphdr_cache_init. This macro does not generate
 * any code (instructions). It only produces some assembler directives.
 *
 * @instruction_cnt 0
 */
#macro dl_iphdr_cache_fini()
    xbuf_free($$iphdr)
#endm


/** @}
 * @}
 */

#endif /* __DL_IPHDR_UC__ */
