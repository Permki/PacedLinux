/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved. */

#include <nfp.h>
#include <nfp/mem_bulk.h>
#include <nfp/mem_atomic.h>
#include <nfp6000/nfp_me.h>
#include "pif_common.h"

/***************************************************************************
 * Generic action execution                                                *
 ***************************************************************************/

#ifdef DEBUG_ACTION
__export __emem_n(0) uint32_t pif_act_stats[PIF_ACTION_ID_MAX + 1];
#endif

static __gpr int get_actdata(__xread uint32_t *xfer_buf,
                             __mem __addr40 uint32_t *actbuf,
                             __gpr uint32_t act_len,
                             __gpr uint32_t offset)
{
    SIGNAL sig;
    __gpr uint32_t alen;
    __xread union pif_action_opdata *act_op;

    /* we first need the opcode + a bit of prefetch */
    __mem_read32(xfer_buf,
               actbuf + offset,
               (PIF_ACTION_PREFETCH_LW + PIF_ACTION_OPDATA_LW) * 4,
               64,
               ctx_swap,
               &sig);

    act_op = (__xread union pif_action_opdata *) &xfer_buf[0];
    alen = act_op->actdata_cnt;
    if (alen > PIF_ACTION_PREFETCH_LW) {
        __mem_read32(xfer_buf + PIF_ACTION_PREFETCH_LW + PIF_ACTION_OPDATA_LW,
                     actbuf + offset + PIF_ACTION_PREFETCH_LW + PIF_ACTION_OPDATA_LW,
                     (act_op->actdata_cnt - PIF_ACTION_PREFETCH_LW) * 4,
                     64 - (PIF_ACTION_PREFETCH_LW + PIF_ACTION_OPDATA_LW) * 4,
                     ctx_swap,
                     &sig);
    }

    return PIF_ACTION_OPDATA_LW + alen;
}


int pif_action_execute(__lmem uint32_t *parrep,
                       __mem __addr40 uint32_t *actbuf,
                       __gpr unsigned int actbufoff,
                       __gpr int act_len)
{
    __xread uint32_t xfer_buf[PIF_ACTION_OPDATA_MAX_LW];
    __gpr uint32_t retrieved;
    __gpr uint32_t offset = actbufoff;
    int ret;

    while (act_len > 0) {
        retrieved = get_actdata(xfer_buf, actbuf, act_len, offset);
        offset += retrieved;

        /* check that we didn't get more data than expected
         * protection from malformed entries
         */
        if (retrieved > act_len) {
            return -1;
        }

        act_len -= retrieved;

        /* all accesses to opdata->action_id done in pif_action_exec_op() for performance */
        ret = pif_action_exec_op(parrep, xfer_buf);
        if (ret)
            return ret;
    }
    return 0;
}
