/* Copyright (C) 2015-2016,  Netronome Systems, Inc.  All rights reserved. */

#include <nfp/mem_atomic.h>
#include <nfp/mem_ring.h>
#include <nfp/me.h>
#include <nfp/cls.h>
#include "pif_common.h"
#include "pkt_clone.h"

#ifdef PIF_DEBUG
    __export __emem_n(0) uint64_t pif_act_stats[PIF_ACTION_ID_MAX + 1];
#endif

extern __nnr uint32_t calc_fld_bmsk;

#define BITRANGE(var, width, offset) \
    (((var) >> (offset)) & ((1 << (width)) - 1))


static int pif_action_exec_drop_act(__lmem uint32_t *_pif_parrep, __xread uint32_t *_pif_actdatabuf, unsigned _pif_debug)
{
    int _pif_return = PIF_RETURN_FORWARD;
    __xread struct pif_action_actiondata_drop_act *_pif_act_data = (__xread struct pif_action_actiondata_drop_act *)_pif_actdatabuf;
    __lmem struct pif_parrep_ctldata *_pif_ctldata = (__lmem struct pif_parrep_ctldata *)(_pif_parrep + PIF_PARREP_CTLDATA_OFF_LW);
#ifdef PIF_DEBUG
    if (_pif_debug & PIF_ACTION_OPDATA_DBGFLAG_BREAK) {
        /* copy the table number and rule number into mailboxes */
        unsigned int temp0, temp1;
        temp0 = local_csr_read(local_csr_mailbox_2);
        temp1 = local_csr_read(local_csr_mailbox_3);
        local_csr_write(local_csr_mailbox_2, _pif_act_data->__pif_rule_no);
        local_csr_write(local_csr_mailbox_3, _pif_act_data->__pif_table_no);
#if SIMULATION == 1
        __asm { /* add nops so mailboxes have time to propagate */
        nop;
        nop;
        nop;
        nop;
        nop;
        nop;
        nop;
        nop;
        }
#endif
        __debug_label("pif_table_hit_drop_act");
        local_csr_write(local_csr_mailbox_2, temp0);
        local_csr_write(local_csr_mailbox_3, temp1);
    }
#endif
#ifdef PIF_DEBUG
    __debug_label("pif_action_state_drop_act");
#endif


    {
        /* drop() */

        /* primitive body */
        _pif_return = PIF_RETURN_DROP;
    }
    return _pif_return;
}

extern __forceinline int pif_action_exec_op(__lmem uint32_t *parrep, __xread uint32_t *_actdata)
{
    __xread union pif_action_opdata *opdata = (__xread union pif_action_opdata *) _actdata;
    int ret = -1;

    if (opdata->action_id > PIF_ACTION_ID_MAX) {
        /* FIXME: TODO: account for bad action id */
        return -1;
    }

    PIF_DEBUG_SET_STATE(PIF_DEBUG_STATE_ACTION, opdata->action_id);
    switch (opdata->action_id) {
    case PIF_ACTION_ID_drop_act:
        ret = pif_action_exec_drop_act(parrep, _actdata + PIF_ACTION_OPDATA_LW, opdata->dbg_flags);
        break;
    }
#ifdef PIF_DEBUG
        mem_incr64((__mem __addr40 uint64_t *)(pif_act_stats + opdata->action_id));
#endif

    return ret;
}
