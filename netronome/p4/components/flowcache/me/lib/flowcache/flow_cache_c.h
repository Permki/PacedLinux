/*
 * Copyright (C) 2014-2016,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        flow_cache_c.h
 * @brief       Prototypes for flow_cache.c
 */

#ifndef _FLOW_CACHE_C_H_
#define _FLOW_CACHE_C_H_

#include <nfp.h>
#include <stdint.h>
#include <stdlib.h>


#include "flow_cache_global_c.h"


/** fc_global_init
 * Initialise rings
 * Once off initialisation
 */
void fc_global_init();


/** fc_me_init
 * Initialisation once per worker ME (which uses the flow cache)
 */
void fc_me_init();

/** fc_exact_lookup_and_learn
 * Performs an exact-match lookup and for the given key with autonomous learning.
 * The address of the corresponding flow context is returned in out_addr[2]. If the entry is not
 * present, then a new uninitialised flow context is created.
 * The key and version must match an existing entry otherwise a new flow context is created.
 * The payload address and lock is returned in fc_lookup_result_t.
 *
 * @param in_key_addr           The key to use for hashing and matching.
 * @param in_key_len            The length of the key in 32-bit words.
 * @param in_pkt_len            The length of the packet to be used for stats.
 * @param in_version            32 bits of version, if the version does not match an existing entry, the existing entry is ignored and new entry is added.
 * @param in_timeout            The active timeout for the entry in seconds, or zero if the entry should not be timed out automatically.
 * @param bucket_base_ptr0      First bucket base pointer
 * @param bucket_base_ptr1      Second bucket base pointer (if two external memories)
 * @param bucket_base_ptr2      Third bucket base pointer (if three external memories)
 * @param entry_base_ptr0       First entry base pointer
 * @param entry_base_ptr1       Second entry base pointer (if two external memories)
 * @param entry_base_ptr2       Third entry base pointer (if three external memories)
 * @param payload_base_ptr0     First payload base pointer
 * @param payload_base_ptr1     Second payload base pointer (if two external memories)
 * @param payload_base_ptr2     Third payload base pointer (if three external memories)
 * @return                      Result of type fc_lookup_result_t
 */
__gpr fc_lookup_result_t fc_exact_lookup_and_learn(
    __lmem uint32_t *in_key_addr,
    __gpr uint32_t in_key_len,
    __gpr uint32_t in_pkt_len,
    __gpr uint32_t in_version,
    __gpr uint32_t in_timeout,
    __mem __addr40 uint32_t *bucket_base_ptr0,
#if (FC_LB_EMU > 1)
    __mem __addr40 uint32_t *bucket_base_ptr1,
#if (FC_LB_EMU > 2)
    __mem __addr40 uint32_t *bucket_base_ptr2,
#endif
#endif

    __mem __addr40 uint32_t *entry_base_ptr0,
#if (FC_LB_EMU > 1)
    __mem __addr40 uint32_t *entry_base_ptr1,
#if (FC_LB_EMU > 2)
    __mem __addr40 uint32_t *entry_base_ptr2,
#endif
#endif

    __mem __addr40 uint32_t *payload_base_ptr0
#if (FC_LB_EMU > 1)
    ,__mem __addr40 uint32_t *payload_base_ptr1
#if (FC_LB_EMU > 2)
    ,__mem __addr40 uint32_t *payload_base_ptr2
#endif
#endif
);


#endif
