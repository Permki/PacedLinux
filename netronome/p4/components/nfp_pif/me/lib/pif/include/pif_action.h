/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __PIF_ACTION_H__
#define __PIF_ACTION_H__

#define PIF_ACTION_BACKOFF_SLEEP 256

/* how much data to speculatively grab when retrieving an action */
#define PIF_ACTION_PREFETCH_LW 3
/* the size of the opcode data */
#define PIF_ACTION_OPDATA_LW   1
/* number of 32-bit words of action data allowed per operation */
#define PIF_ACTION_OPDATA_MAX_LW 24

#define PIF_ACTION_OPDATA_ACTION_ID_off 16
#define PIF_ACTION_OPDATA_ACTDATA_CNT_off 0

union pif_action_opdata {
    struct {
        /* lookup index, next state for lookup */
        int reserved0:8;
        /* action ID, effectively opcode */
        int action_id:8;
#define PIF_ACTION_OPDATA_DBGFLAG_BREAK (1 << 0)
        int dbg_flags:8;
        /* how many 32-bit words of data are associate with the action */
        int actdata_cnt:8;
    }  __packed;
    uint32_t val32;
};

/* Execute actions stored in MU memory */
int pif_action_execute(__lmem uint32_t *parrep,
                       __mem __addr40 uint32_t *actdata,
                       __gpr unsigned int actdataoff,
                       __gpr int actdatalen);

/* Execute an action in transfer registers */
int pif_action_exec_op(__lmem uint32_t *parrep,
                       __xread uint32_t *actdata);

#endif /* __PIF_ACTION_H__ */
