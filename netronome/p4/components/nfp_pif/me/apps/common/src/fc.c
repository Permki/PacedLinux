/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved. */


#include <assert.h>
#include <nfp.h>
#include <stdint.h>

#include <flow_cache_global_c.h>
#include <flow_cache_c.h>
#include <flow_cache_lock_c.h>


__asm .alloc_mem _FC_EMU_0_BUCKETS_BASE emem0+_FC_BUCKETS_ADDR global _FC_BUCKETS_ALLOC_SIZE 256 addr40

__asm .alloc_mem _FC_EMU_0_ENTRIES_BASE emem0+_FC_ENTRIES_ADDR global _FC_ENTRIES_ALLOC_SIZE 256

__asm .alloc_mem _FC_EMU_0_PAYLOADS_BASE emem0+_FC_PAYLOADS_ADDR global _FC_PAYLOADS_ALLOC_SIZE 256

#if (FC_LB_EMU > 1)
__asm .alloc_mem _FC_EMU_1_BUCKETS_BASE emem1+_FC_BUCKETS_ADDR global _FC_BUCKETS_ALLOC_SIZE 256 addr40

__asm .alloc_mem _FC_EMU_1_ENTRIES_BASE emem1+_FC_ENTRIES_ADDR global _FC_ENTRIES_ALLOC_SIZE 256

__asm .alloc_mem _FC_EMU_1_PAYLOADS_BASE emem1+_FC_PAYLOADS_ADDR global _FC_PAYLOADS_ALLOC_SIZE 256

#if (FC_LB_EMU > 2)
__asm .alloc_mem _FC_EMU_2_BUCKETS_BASE emem2+_FC_BUCKETS_ADDR global _FC_BUCKETS_ALLOC_SIZE 256 addr40

__asm .alloc_mem _FC_EMU_2_ENTRIES_BASE emem2+_FC_ENTRIES_ADDR global _FC_ENTRIES_ALLOC_SIZE 256

__asm .alloc_mem _FC_EMU_2_PAYLOADS_BASE emem2+_FC_PAYLOADS_ADDR global _FC_PAYLOADS_ALLOC_SIZE 256

#endif /* FC_LB_EMU > 2 */
#endif /* FC_LB_EMU > 1 */



extern __forceinline void fc_do_release(__gpr fc_lock_t in_lock)
{
    fc_lock_release(in_lock,
                    (__mem __addr40 unsigned int *)__link_sym("_FC_EMU_0_BUCKETS_BASE")
#if FC_LB_EMU > 1
                    ,(__mem __addr40 unsigned int *)__link_sym("_FC_EMU_1_BUCKETS_BASE")
#if FC_LB_EMU > 2
                    ,(__mem __addr40 unsigned int *)__link_sym("_FC_EMU_2_BUCKETS_BASE")
#endif
#endif
                    );
}

extern __forceinline fc_lookup_result_t fc_do_lookup(__lmem uint32_t *flowkey,
                                unsigned int flowkey_len,
                                unsigned int pktlen,
                                unsigned int version,
                                unsigned int timeout)
{
    volatile __gpr fc_lookup_result_t result;

    result = fc_exact_lookup_and_learn(
                flowkey,
                flowkey_len,
                pktlen,
                version,
                timeout,
                (__mem __addr40 unsigned int *)__link_sym("_FC_EMU_0_BUCKETS_BASE"),
#if FC_LB_EMU > 1
                (__mem __addr40 unsigned int *)__link_sym("_FC_EMU_1_BUCKETS_BASE"),
#if FC_LB_EMU > 2
                (__mem __addr40 unsigned int *)__link_sym("_FC_EMU_2_BUCKETS_BASE"),
#endif
#endif
                (__mem __addr40 unsigned int *)__link_sym("_FC_EMU_0_ENTRIES_BASE"),
#if FC_LB_EMU > 1
                (__mem __addr40 unsigned int *)__link_sym("_FC_EMU_1_ENTRIES_BASE"),
#if FC_LB_EMU > 2
                (__mem __addr40 unsigned int *)__link_sym("_FC_EMU_2_ENTRIES_BASE"),
#endif
#endif
                (__mem __addr40 unsigned int *)__link_sym("_FC_EMU_0_PAYLOADS_BASE")
#if FC_LB_EMU > 1
                ,(__mem __addr40 unsigned int *)__link_sym("_FC_EMU_1_PAYLOADS_BASE")
#if FC_LB_EMU > 2
                ,(__mem __addr40 unsigned int *)__link_sym("_FC_EMU_2_PAYLOADS_BASE")
#endif
#endif
                );
    return result;
}

/*******************************************************************/

extern __forceinline uint32_t fc_get_version(void)
{
    return local_csr_read(local_csr_mailbox3);
}
