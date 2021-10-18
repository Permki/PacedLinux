/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include <nfp.h>
#include <nfp6000/nfp_me.h>
#include "dbgsrv.h"
#include "brreg.h"

static int getdatareg(struct nfp_device *nfp, int meid, int ctx,
                      int regtype, int regaddr, uint32_t *regdata)
{
    uint32_t val32;
    uint32_t a;

    /* TODO : these all need to be checked */
    switch(regtype) {
    case IXP_GPA_REL:
        a = nfp_mereg_gprA_ctx(regaddr, ctx);
        break;
    case IXP_GPB_REL:
        a = nfp_mereg_gprB_ctx(regaddr, ctx);
        break;
    case IXP_GPA_ABS:
        a = nfp_mereg_gprA(regaddr);
        break;
    case IXP_GPB_ABS:
        a = nfp_mereg_gprB(regaddr);
        break;
    case IXP_DR_RD_REL:
    case IXP_SR_RD_REL:
        a = nfp_mereg_xfer_in_ctx(regaddr, ctx);
        break;
    case IXP_DR_WR_REL:
    case IXP_SR_WR_REL:
        a = nfp_mereg_xfer_out_ctx(regaddr, ctx);
        break;
    case IXP_DR_RD_ABS:
    case IXP_SR_RD_ABS:
        a = nfp_mereg_xfer_in(regaddr);
        break;
    case IXP_DR_WR_ABS:
    case IXP_SR_WR_ABS:
        a = nfp_mereg_xfer_out(regaddr);
        break;
    case IXP_NEIGH_REL:
        a = nfp_mereg_nn_ctx(regaddr, ctx);
        break;
    case IXP_NEIGH_INDX:
        a = nfp_mereg_nn(regaddr);
        break;
    case IXP_SR_ABS:
    case IXP_DR_ABS:
        /* read the T_INDEX CSR and "or" with the regAddr to get the xferAddr */
        if (nfp_mecsr_read(nfp, meid, -1, NFP_ME_T_INDEX, &val32)) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to read T_INDEX when stepping branch (%s)",
                   strerror(errno));
            return -1;
        }
        a = nfp_mereg_xfer_in(regaddr | NFP_ME_T_INDEX_INDEX_of(val32));
        break;
    default :
        NS_LOG(NS_LOG_LVL_ERROR,
               "invalid REGTYPE when stepping branch (%d)", regtype);
        return -1;
    }
    if (nfp_me_register_read(nfp, meid, a, regdata) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "me register read failed for regtype %d:%d "
               "while stepping branch (%s)",
               regtype, regaddr, strerror(errno));
        return -1;
    }
    return 0;
}

int brreg_get_offset(struct nfp_device *nfp, int meid, int ctx,
                     int regtype, int regaddr, uint32_t *regdata)
{
    uint32_t addr, val;
    int ret = 0;

    /* XXX: FIXME, LMEM2/3? */
    if (regtype == IXP_LMEM0 || regtype == IXP_LMEM1) {
        if (regtype == IXP_LMEM0)
            addr = NFP_ME_ACTIVE_LM_ADDR_0;
        else
            addr = NFP_ME_ACTIVE_LM_ADDR_1;

        ret = nfp_mecsr_read(nfp, meid, -1, addr, &val);
        if (ret < 0) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to access LM address when stepping branch (%s)",
                   strerror(errno));
            return -1;
        }
        /* XXX: FIXME: BUG: regdata is not being set! */
        *regdata = NFP_ME_LMADDR_ADDR_of(val);
    } else {
        ret = getdatareg(nfp, meid, ctx,
                         regtype, regaddr, regdata);
        if (ret < 0) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to access register when stepping branch (%s)",
                   strerror(errno));
            return -1;
        }
    }

    return 0;
}
