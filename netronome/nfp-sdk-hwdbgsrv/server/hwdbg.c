/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include <nfp6000/nfp_me.h>
/*#include <nfp3800/nfp_me.h>*/

#include <nfp_nffw.h>

#include "dbgsrv.h"
#include "brreg.h"

#ifdef SDK_SIM
/* 4min timeout for the sim - sigh */
#define STEP_TIMEOUT_MS (60000 * 4)
#else
/* step API times out after 5s */
#define STEP_TIMEOUT_MS (5000)
#endif

#define MAX2(x,y) ((x) > (y) ? (x) : (y))

/* determine the meid of the paired me for SCS */
#define SCSBUDDY(m) ((m)%2 == 0 ? (m) + 1 : (m) - 1)

/* XXX - FIX ME
 * temporary define - remove when could include nfp3800/nfp_me.h
 */
#define NFP3800_ME_CTX_ENABLES              0x00000018
#define NFP3800_ME_MISC_CONTROL             0x00000160
#define NFP3800_ME_MISC_CONTROL_QUAD_USTORE (1 << 6)

/* ucode instruction for ctx_arb[bpt] */
#define INSTR_CTXARB_BPT 0x00E000020000LL

#define USTORE_ECC_WORDSIZE            45
#define USTORE_ECC_PARITY_BITS         7

#define STOP_STATE_RUNNING 0
#define STOP_STATE_STOPPED 1
#define STOP_STATE_ATBREAK 2

/* The size of one ME's side-routine block, which is the same regardless of
 * the ME's SCS mode.
 *
 * The number of defers can be 0, 1, 2 or 3. The final unconditional branch
 * is placed after the last defer. So the minimum size is 2, the maximum is 5.
 * .0 instruction to run
 * .1 defer[0]
 * .2 defer[1]
 * .3 defer[2]
 * .4 br[resume_addr]
 */
#define ROUTINE_INSTR_CNT   5

static const uint64_t ustore_ecc_polynomials[USTORE_ECC_PARITY_BITS] = {
    UINT64_C(0x0ff800007fff),
    UINT64_C(0x11f801ff801f),
    UINT64_C(0x1e387e0781e1),
    UINT64_C(0x17cb8e388e22),
    UINT64_C(0x1af5b2c93244),
    UINT64_C(0x1f56d5525488),
    UINT64_C(0x0daf69a46910)
};

static int ustore_ecc_parity(uint64_t value)
{
    int n, parity = 0;
    for (n = 0; n < USTORE_ECC_WORDSIZE; n++, value >>= 1)
        parity ^= (value & 1);
    return parity;
}

static void ustore_ecc_calculate(uint64_t *uword)
{
    uint64_t word = (*uword);
    uint8_t ecc = 0;
    size_t n;

    word &= (UINT64_C(1) << USTORE_ECC_WORDSIZE) - 1;

    for (n = 0; n < USTORE_ECC_PARITY_BITS; n++) {
        ecc |= ustore_ecc_parity(ustore_ecc_polynomials[n] & word) << n;
    }

    *uword = ( (((uint64_t)ecc) << USTORE_ECC_WORDSIZE) | word );
}

/********** Support functions ***********/

struct hwdbg_me_data *hwdbg_get_me_data(struct hwdbg_handle *hd, int meid)
{
    int i;
    for (i = 0; i < hd->me_count; i++) {
        if (hd->me_data[i].meid != meid)
            continue;
        if (hd->me_data[i].disabled)
            break;
        return &hd->me_data[i];
    }
    return NULL;
}

/* helper function for mecsr read */
static int mecsr_read(struct hwdbg_handle *hd,
                      struct hwdbg_me_data *me,
                      int addr, uint32_t *val)
{
    int ret;
    ret = nfp_mecsr_read(hd->nfp, me->meid, -1, addr, val);
    if (ret < 0) {
        NS_LOG(NS_LOG_LVL_WARN,
               "Failed to access mecsr %s:%x (%s), disabling debug for %s",
               me->meid_str, addr, strerror(errno), me->meid_str);
        me->disabled = 1;
    }

    return ret;
}

static int meindcsr_read(struct hwdbg_handle *hd,
                         struct hwdbg_me_data *me,
                         int ctx, int addr, uint32_t *val)
{
    int ret;
    ret = nfp_mecsr_read(hd->nfp, me->meid, ctx, addr, val);
    if (ret < 0) {
        NS_LOG(NS_LOG_LVL_WARN,
               "Failed to access mecsr %s:%x (%s), disabling debug for %s",
               me->meid_str, addr, strerror(errno), me->meid_str);
        me->disabled = 1;
    }

    return ret;
}

/* helper function for mecsr write */
static int meindcsr_write(struct hwdbg_handle *hd,
                          struct hwdbg_me_data *me,
                          int ctx, int addr, uint32_t val)
{
    int ret;

    ret = nfp_mecsr_write(hd->nfp, me->meid, ctx, addr, val);
    if (ret < 0) {
        NS_LOG(NS_LOG_LVL_WARN,
               "Failed to access mecsr %s:%x (%s), disabling debug for %s",
               me->meid_str, addr, strerror(errno), me->meid_str);
        me->disabled = 1;
    }
    return ret;
}

static int mecsr_write(struct hwdbg_handle *hd,
                       struct hwdbg_me_data *me,
                       int addr, uint32_t val)
{
    int ret;

    ret = nfp_mecsr_write(hd->nfp, me->meid, -1, addr, val);
    if (ret < 0) {
        NS_LOG(NS_LOG_LVL_WARN,
               "Failed to access mecsr %s:%x (%s), disabling debug for %s",
               me->meid_str, addr, strerror(errno), me->meid_str);
        me->disabled = 1;
    }
    return ret;
}

/* scs helpers */
static int is_scs_enabled(struct hwdbg_handle *hd, struct hwdbg_me_data *me)
{
    uint32_t val32;
    if (mecsr_read(hd, me, NFP_ME_MISC_CONTROL, &val32) < 0)
        return 0;

    return val32 & NFP_ME_MISC_CONTROL_SHARE_USTORE;
}

/* qcs helpers */
static int is_qcs_enabled(struct hwdbg_handle *hd, struct hwdbg_me_data *me)
{
    uint32_t val32;
    if (mecsr_read(hd, me, NFP3800_ME_MISC_CONTROL, &val32) < 0)
        return 0;

    return val32 & NFP3800_ME_MISC_CONTROL_QUAD_USTORE;
}

/* is the scs intr @ pc on the me */
static int scs_instr_on_me(int meid, int pc)
{
    int pc_odd = pc % 2, me_odd = meid % 2;

    return !(me_odd ^ pc_odd);
}

/* is the qcs intr @ pc on the me */
/*static int qcs_instr_on_me(int meid, int pc)
{
    int pc_me = pc % 4 , me = meid % 4;

    return ( ((me ^ pc_me) == 0) ? 1 : 0);
}*/

/* ustore access helpers */

static int short_stop_me(struct hwdbg_handle *hd,
                  struct hwdbg_me_data *me,
                  uint32_t *ctxen_val);

/* store the unmodified codestore for an ME - done on demand */
static int init_cs(struct hwdbg_handle *hd, struct hwdbg_me_data *me)
{
    int l = CSLEN * sizeof(uint64_t);
    uint32_t val32 = 0;
    int ret = 0;

    if (me->base_cs) /* already set up */
        return 0;

    me->base_cs = malloc(l);
    if (!me->base_cs) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to allocate codestore memory for %s",
               me->meid_str);
        return -1;
    }

    if (me->scsenabled) {
        /* we have to read the ustore with SCS off */
        if (mecsr_read(hd, me, NFP_ME_MISC_CONTROL, &val32) < 0) {
            free(me->base_cs);
            me->base_cs = NULL;
            return -1;
        }
        if (mecsr_write(hd, me,
                        NFP_ME_MISC_CONTROL,
                        val32 & ~NFP_ME_MISC_CONTROL_SHARE_USTORE) < 0) {
            free(me->base_cs);
            me->base_cs = NULL;
            return -1;
        }

        if (mecsr_read(hd, me->scs_me_data, NFP_ME_MISC_CONTROL, &val32) < 0) {
            free(me->scs_me_data->base_cs);
            me->scs_me_data->base_cs = NULL;
            return -1;
        }
        if (mecsr_write(hd, me->scs_me_data,
                        NFP_ME_MISC_CONTROL,
                        val32 & ~NFP_ME_MISC_CONTROL_SHARE_USTORE) < 0) {
            free(me->scs_me_data->base_cs);
            me->scs_me_data->base_cs = NULL;
            return -1;
        }
    }

    if (me->qcsenabled) {
        /* we have to read the ustore with QCS off */
        if (mecsr_read(hd, me, NFP3800_ME_MISC_CONTROL, &val32) < 0) {
            free(me->base_cs);
            me->base_cs = NULL;
            return -1;
        }
        if (mecsr_write(hd, me,
                        NFP3800_ME_MISC_CONTROL,
                        val32 & ~NFP3800_ME_MISC_CONTROL_QUAD_USTORE) < 0) {
            free(me->base_cs);
            me->base_cs = NULL;
            return -1;
        }
    }

    if (nfp_ustore_read(hd->nfp, me->meid, me->base_cs, l, 0) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to read ustore for %s (%s)",
               me->meid_str, strerror(errno));
        free(me->base_cs);
        me->base_cs = NULL;
        me->disabled = 1;
        ret = -1; /* make sure we write back misc control if needed */
    }

    if (me->scsenabled) {
        /* write back the original */
        if (mecsr_write(hd, me, NFP_ME_MISC_CONTROL, val32) < 0) {
            free(me->base_cs);
            me->base_cs = NULL;
            return -1;
        }
        if (mecsr_write(hd, me->scs_me_data, NFP_ME_MISC_CONTROL, val32) < 0) {
            free(me->scs_me_data->base_cs);
            me->scs_me_data->base_cs = NULL;
            return -1;
        }
    }

    if (me->qcsenabled) {
        /* write back the original */
        if (mecsr_write(hd, me, NFP3800_ME_MISC_CONTROL, val32) < 0) {
            free(me->base_cs);
            me->base_cs = NULL;
            return -1;
        }
    }

    return ret;
}

static int init_base_cs(struct hwdbg_handle *hd, struct hwdbg_me_data *me)
{
    int ret, i = 0;

    ret = init_cs(hd, me);
    if (ret < 0)
        return -1;

    /* set up the scs and qcs ustore too if needed */
    if (!me->scsenabled)
        return 0;
    else if (!me->qcsenabled)
        return 0;

    if (me->scsenabled) {
        me = me->scs_me_data;
        return init_cs(hd, me);
    } else if (me->qcsenabled) {
        for (i = 0; i < QCSMENUM; i++) {
            me = me->qcs_me_data[i];
            ret = init_cs(hd, me);
            if (ret < 0)
                return -1;
        }
    }

    return 0;
}

static int write_active_scsustore(struct hwdbg_handle *hd,
                                  struct hwdbg_me_data *me,
                                  int pc,
                                  uint64_t val)
{
    uint32_t ctxen_val = 0, scsctxen_val = 0;
    int restore_ctxen = 0, scsrestore_ctxen = 0;
    struct hwdbg_me_data *scsme = NULL;
    struct hwdbg_me_data *tmp = me;

    scsme = me->scs_me_data;

    /* use the other if needed */
    if (!scs_instr_on_me(me->meid, pc)) {
        me = scsme;
        scsme = tmp;
    }
    pc /= 2;

    if (pc >= CSLEN) {
        NS_LOG(NS_LOG_LVL_ERROR, "PC %d exceeds %d", pc, CSLEN);
        return -1;
    }

    /* In SCS mode, we now stop both MEs regardless of which ME's code word
     * we want to access. This is slower, but much safer.
     */
    restore_ctxen = short_stop_me(hd, me, &ctxen_val);
    if (restore_ctxen < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to stop ME %s for ustore access",
               me->meid_str);
        return -1;
    }

    if (scsme) {
        scsrestore_ctxen = short_stop_me(hd, scsme, &scsctxen_val);
        if (scsrestore_ctxen < 0) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "Failed to stop SCS ME %s for ustore access",
                   scsme->meid_str);
            return -1;
        }
    }

    /* we need to save the original ustore before changing so we can recover */
    if (!me->base_cs) {
        if (init_base_cs(hd, me) < 0)
            return -1;
    }

    if (nfp_ustore_write(hd->nfp, me->meid, &val, 8, pc * 8) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to write ustore for %s (%s)",
               me->meid_str, strerror(errno));
        return -1;
    }

    if (restore_ctxen) {
        if (mecsr_write(hd, me, NFP_ME_CTX_ENABLES, ctxen_val) < 0)
            return -1;
    }
    if (scsrestore_ctxen) {
        if (mecsr_write(hd, scsme, NFP_ME_CTX_ENABLES, scsctxen_val) < 0)
            return -1;
    }

    return 0;
}

static int write_active_qcsustore(struct hwdbg_handle *hd,
                                  struct hwdbg_me_data *me,
                                  int pc,
                                  uint64_t val)
{
    uint32_t ctxen_val = 0, qcsctxen_val[QCSMENUM];
    int restore_ctxen = 0, qcsrestore_ctxen[QCSMENUM];
    struct hwdbg_me_data *qcsme[QCSMENUM];
    int i = 0;

    for (i = 0; i < QCSMENUM; i++) {
        qcsctxen_val[i] = 0;
        qcsrestore_ctxen[i] = 0;
        qcsme[i] = NULL;
    }

    pc /= 4;

    if (pc >= CSLEN) {
        NS_LOG(NS_LOG_LVL_ERROR, "PC %d exceeds %d", pc, CSLEN);
        return -1;
    }

    /* In QCS mode, we now stop all four MEs regardless of which ME's code word
     * we want to access. This is slower, but much safer.
     */
    restore_ctxen = short_stop_me(hd, me, &ctxen_val);
    if (restore_ctxen < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to stop ME %s for ustore access",
               me->meid_str);
        return -1;
    }

    for (i = 0; i < QCSMENUM; i++) {
        qcsme[i] = me->qcs_me_data[i];
        if (qcsme[i]) {
            qcsrestore_ctxen[i] = short_stop_me(hd, qcsme[i], &(qcsctxen_val[i]));
            if (qcsrestore_ctxen[i] < 0) {
                NS_LOG(NS_LOG_LVL_ERROR,
                    "Failed to stop QCS ME %s for ustore access",
                    qcsme[i]->meid_str);
                return -1;
            }
        }
    }

    /* we need to save the original ustore before changing so we can recover */
    if (!me->base_cs) {
        if (init_base_cs(hd, me) < 0)
            return -1;
    }

    if (nfp_ustore_write(hd->nfp, me->meid, &val, 8, pc * 8) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to write ustore for %s (%s)",
               me->meid_str, strerror(errno));
        return -1;
    }

    if (restore_ctxen) {
        if (mecsr_write(hd, me, NFP3800_ME_CTX_ENABLES, ctxen_val) < 0)
            return -1;

        for (i = 0; i < QCSMENUM; i++) {
            if (qcsrestore_ctxen[i]) {
                if (mecsr_write(hd, qcsme[i], NFP3800_ME_CTX_ENABLES, qcsctxen_val[i]) < 0)
                    return -1;
            }
        }
    }

    return 0;
}

/* write to the active code store */
static int write_active_ustore(struct hwdbg_handle *hd,
                               struct hwdbg_me_data *me,
                               int pc,
                               uint64_t val)
{
    uint32_t ctxen_val = 0;
    int restore_ctxen = 0;

    ustore_ecc_calculate(&val);

    if (me->scsenabled)
        return write_active_scsustore(hd, me, pc, val);

    else if (me->qcsenabled)
        return write_active_qcsustore(hd, me, pc, val);

    if (pc >= CSLEN) {
        NS_LOG(NS_LOG_LVL_ERROR, "PC %d exceeds %d", pc, CSLEN);
        return -1;
    }

    /* We now stop the ME we want to access. */
    restore_ctxen = short_stop_me(hd, me, &ctxen_val);
    if (restore_ctxen < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to stop ME %s for ustore access",
               me->meid_str);
        return -1;
    }

    /* we need to save the original ustore before changing so we can recover */
    if (!me->base_cs) {
        if (init_base_cs(hd, me) < 0)
            return -1;
    }

    if (nfp_ustore_write(hd->nfp, me->meid, &val, 8, pc * 8) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to write ustore for %s (%s)",
               me->meid_str, strerror(errno));
        return -1;
    }

    if (restore_ctxen) {
        if (mecsr_write(hd, me, NFP_ME_CTX_ENABLES, ctxen_val) < 0)
            return -1;
    }

    return 0;
}

static int read_active_scsustore(struct hwdbg_handle *hd,
                                 struct hwdbg_me_data *me,
                                 int pc,
                                 uint64_t *val)
{
    uint32_t ctxen_val = 0, scsctxen_val = 0;
    int restore_ctxen = 0, scsrestore_ctxen = 0;
    struct hwdbg_me_data *scsme = NULL;
    uint32_t scsmisc_val = 0;
    uint32_t misc_val = 0;
    uint64_t eccmask;
    int ret = 0;

    struct hwdbg_me_data *tmp = me;
    scsme = me->scs_me_data;

    /* use the other if needed */
    if (!scs_instr_on_me(me->meid, pc)) {
        me = scsme;
        scsme = tmp;
    }
    pc /= 2;

    if (pc >= CSLEN) {
        NS_LOG(NS_LOG_LVL_ERROR, "PC %d exceeds %d", pc, CSLEN);
        return -1;
    }

    /* In SCS mode, we now stop both MEs regardless of which ME's code word
     * we want to access. This is slower, but much safer.
     */
    restore_ctxen = short_stop_me(hd, me, &ctxen_val);
    if (restore_ctxen < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to stop ME %s for ustore access",
               me->meid_str);
        return -1;
    }

    if (scsme) {
        scsrestore_ctxen = short_stop_me(hd, scsme, &scsctxen_val);
        if (scsrestore_ctxen < 0) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "Failed to stop SCS ME %s for ustore access",
                   scsme->meid_str);
            return -1;
        }
    }

    /* we have to read the ustore with SCS off */
    /* and we turn it off for both SCS MEs to be safe */
    if (mecsr_read(hd, me, NFP_ME_MISC_CONTROL, &misc_val) < 0) {
        return -1;
    }
    if (mecsr_write(hd, me,
                    NFP_ME_MISC_CONTROL,
                    misc_val & ~NFP_ME_MISC_CONTROL_SHARE_USTORE) < 0) {
        return -1;
    }
    
    if (scsme) {
        if (mecsr_read(hd, scsme, NFP_ME_MISC_CONTROL, &scsmisc_val) < 0) {
            return -1;
        }
        if (mecsr_write(hd, scsme,
                        NFP_ME_MISC_CONTROL,
                        scsmisc_val & ~NFP_ME_MISC_CONTROL_SHARE_USTORE) < 0) {
            return -1;
        
        }
    }

    if (nfp_ustore_read(hd->nfp, me->meid, val, 8, pc * 8) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to read ustore for %s (%s)",
               me->meid_str, strerror(errno));
        ret = -1;
    }

    if (ret < 0)
        return ret;

    /* write back the original */
    if (mecsr_write(hd, me, NFP_ME_MISC_CONTROL, misc_val) < 0)
        return -1;
    if (mecsr_write(hd, scsme, NFP_ME_MISC_CONTROL, scsmisc_val) < 0)
        return -1;

    if (restore_ctxen) {
        if (mecsr_write(hd, me, NFP_ME_CTX_ENABLES, ctxen_val) < 0)
            return -1;
    }
    if (scsrestore_ctxen) {
        if (mecsr_write(hd, scsme, NFP_ME_CTX_ENABLES, scsctxen_val) < 0)
            return -1;
    }

    /* mask out ECC */
    eccmask = ~((-1LL) << USTORE_ECC_WORDSIZE);
    *val = (*val) & eccmask;
    return 0;
}

static int read_active_qcsustore(struct hwdbg_handle *hd,
                                 struct hwdbg_me_data *me,
                                 int pc,
                                 uint64_t *val)
{
    uint32_t ctxen_val = 0, qcsctxen_val[QCSMENUM];
    int restore_ctxen = 0, qcsrestore_ctxen[QCSMENUM];
    struct hwdbg_me_data *qcsme[QCSMENUM];
    uint32_t qcsmisc_val[QCSMENUM];
    uint32_t misc_val = 0;
    uint64_t eccmask;
    int ret = 0, i = 0;

    for (i = 0; i < QCSMENUM; i++) {
        qcsctxen_val[i] = 0;
        qcsrestore_ctxen[i] = 0;
        qcsmisc_val[i] = 0;
        qcsme[i] = NULL;
    }

    pc /= 4;

    if (pc >= CSLEN) {
        NS_LOG(NS_LOG_LVL_ERROR, "PC %d exceeds %d", pc, CSLEN);
        return -1;
    }

    /* In QCS mode, we now stop all four MEs regardless of which ME's code word
     * we want to access. This is slower, but much safer.
     */
    restore_ctxen = short_stop_me(hd, me, &ctxen_val);
    if (restore_ctxen < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to stop ME %s for ustore access",
               me->meid_str);
        return -1;
    }


    for (i = 0; i < QCSMENUM; i++) {
        qcsme[i] = me->qcs_me_data[i];
        qcsrestore_ctxen[i] = short_stop_me(hd, qcsme[i], &(qcsctxen_val[i]));
        if (qcsrestore_ctxen[i] < 0) {
            NS_LOG(NS_LOG_LVL_ERROR,
                "Failed to stop SCS ME %s for ustore access",
                qcsme[i]->meid_str);
            return -1;
        }
    }

    /* we have to read the ustore with QCS off */
    /* and we turn it off for all QCS MEs to be safe */
    if (mecsr_read(hd, me, NFP3800_ME_MISC_CONTROL, &misc_val) < 0) {
        return -1;
    }
    if (mecsr_write(hd, me,
                    NFP3800_ME_MISC_CONTROL,
                    misc_val & ~NFP3800_ME_MISC_CONTROL_QUAD_USTORE) < 0) {
        return -1;
    }
    
    for (i = 0; i < QCSMENUM; i++) {
        qcsme[i] = me->qcs_me_data[i];
        if (qcsme[i]) {
            if (mecsr_read(hd, qcsme[i], NFP3800_ME_MISC_CONTROL, &qcsmisc_val[i]) < 0) {
                return -1;
            }
            if (mecsr_write(hd, qcsme[i],
                            NFP3800_ME_MISC_CONTROL,
                            qcsmisc_val[i] & ~NFP3800_ME_MISC_CONTROL_QUAD_USTORE) < 0) {
                return -1;
            }
        
        }
    }

    if (nfp_ustore_read(hd->nfp, me->meid, val, 8, pc * 8) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to read ustore for %s (%s)",
               me->meid_str, strerror(errno));
        ret = -1;
    }

    if (ret < 0)
        return ret;

    /* write back the original */
    if (mecsr_write(hd, me, NFP3800_ME_MISC_CONTROL, misc_val) < 0)
        return -1;

    if (restore_ctxen) {
        if (mecsr_write(hd, me, NFP3800_ME_CTX_ENABLES, ctxen_val) < 0)
            return -1;
    }

    for (i = 0; i < QCSMENUM; i++) {
        if (qcsme[i]) {
            if (mecsr_write(hd, qcsme[i], NFP3800_ME_MISC_CONTROL, qcsmisc_val[i]) < 0)
                return -1;

            if (restore_ctxen) {
                if (qcsrestore_ctxen[i]) {
                    if (mecsr_write(hd, qcsme[i], NFP3800_ME_CTX_ENABLES, qcsctxen_val[i]) < 0)
                        return -1;
                }
            }
        }
    }

    /* mask out ECC */
    eccmask = ~((-1LL) << USTORE_ECC_WORDSIZE);
    *val = (*val) & eccmask;
    return 0;
}

/* get a codestore value from the real store */
static int read_active_ustore(struct hwdbg_handle *hd,
                              struct hwdbg_me_data *me,
                              int pc,
                              uint64_t *val)
{
    uint32_t ctxen_val = 0;
    int restore_ctxen = 0;
    uint64_t eccmask;
    int ret = 0;

    /* First do SCS and QCS error checks */
    if (me->scsenabled)
        return read_active_scsustore(hd, me, pc, val);

    else if (me->qcsenabled)
        return read_active_qcsustore(hd, me, pc, val);

    if (pc >= CSLEN) {
        NS_LOG(NS_LOG_LVL_ERROR, "PC %d exceeds %d", pc, CSLEN);
        return -1;
    }

    /* Then start meddling */

    /* We now stop both MEs regardless of which ME's code word
     * we want to access. This is slower, but much safer.
     */
    restore_ctxen = short_stop_me(hd, me, &ctxen_val);
    if (restore_ctxen < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to stop ME %s for ustore access",
               me->meid_str);
        return -1;
    }

    if (nfp_ustore_read(hd->nfp, me->meid, val, 8, pc * 8) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to read ustore for %s (%s)",
               me->meid_str, strerror(errno));
        ret = -1;
    }

    if (ret < 0)
        return ret;

    if (restore_ctxen) {
        if (mecsr_write(hd, me, NFP_ME_CTX_ENABLES, ctxen_val) < 0)
            return -1;
    }

    /* mask out ECC */
    eccmask = ~((-1LL) << USTORE_ECC_WORDSIZE);
    *val = (*val) & eccmask;
    return 0;
}

/* get a codestore value from the original/base store */
static int get_base_ustore(struct hwdbg_handle *hd,
                           struct hwdbg_me_data *me,
                           int pc,
                           uint64_t *val)
{
    uint64_t eccmask;
    if (!me->base_cs) {
        if (init_base_cs(hd, me) < 0)
            return -1;
    }

    if (me->scsenabled) {
        int pcodd = pc % 2, meodd = me->meid % 2;
        if (pcodd ^ meodd) {
            /* if one is odd and the other even we use the scs meid */
            me = me->scs_me_data;
        }
        pc /= 2;
    }

    if (me->qcsenabled) {
        /* PC = 0 --> ME0 
         * PC = 1 --> ME1
         * PC = 2 --> ME2
         * PC = 3 --> ME3
         */
        int pc_index = (pc % 4), me_index = me->meid % 4;
        /* if PC and ME indices are equal then we are done */
        if (pc_index ^ me_index) {
            /* if PC index is zero regardless of ME index we use QCS ME data zero */
            if (pc_index == 0)
                me = me->qcs_me_data[0];
            /* if ME index is zero and PC index is not zero,
             * depend on PC index we use QCS ME data 
             */
            else if (me_index == 0)
                me = me->qcs_me_data[pc_index-1];
            /* if PC and ME indices are not zero and are not equal,
             * PC index is either 1, 2, or 3
             */
            else if (pc_index != me_index) {
                /* If PC index is 1, regardless of ME index we use QCS ME data one */
                if (pc_index == 1)
                    me = me->qcs_me_data[1];
                /* If PC index is 2, ME index is either 1 or 3, depend on ME index we use QCS ME data */
                else if (pc_index == 2) {
                    if (me_index == 1)
                        me = me->qcs_me_data[1];
                    else if (me_index == 3)
                        me = me->qcs_me_data[2];
                /* If PC index is 3, regardless of ME index we use QCS ME data two */
                } else if (pc_index == 3)
                    me = me->qcs_me_data[2];
            }
        }
        pc /= 4;
    }

    if (pc >= CSLEN) {
        NS_LOG(NS_LOG_LVL_ERROR, "PC %d exceeds %d", pc, CSLEN);
        return -1;
    }

    /* mask out ECC */
    eccmask = ~((-1LL) << USTORE_ECC_WORDSIZE);
    *val = me->base_cs[pc] & eccmask;
    return 0;
}

/* was the base code store originally a ctx_arb[bpt]? */
static int base_was_swbpt(struct hwdbg_handle *hd,
                          struct hwdbg_me_data *me,
                          int pc)
{
    uint64_t val;
    if (get_base_ustore(hd, me, pc, &val) < 0)
        return 0;

    if (val == INSTR_CTXARB_BPT)
        return 1;

    return 0;
}

#ifdef SDK_SIM

#include <nfp_sal.h>

static void wait_mecycles(struct hwdbg_handle *hd,
                          struct hwdbg_me_data *me,
                          int cycles)
{
    uint64_t tstart = 0, tcur = 0;
    int n = 0;

    tstart = nfp_sal_cycle_count(hd->nfp);

    while (1) {
        tcur = nfp_sal_cycle_count(hd->nfp);
        if (tcur - tstart >= cycles)
            return;
        usleep(1000);
        n++;

        /* worst case of 1s (1000 loops) per cycle */
        if (n >= (1000 * cycles))
            return;
    }
}
#endif


/******* breakpoint management, add/restore code and resume ********/

int hwdbg_me_running(struct hwdbg_handle *hd, int meid)
{
    struct hwdbg_me_data *me;
    /* already locked */

    if (meid <= 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Invalid meid %x for running check", meid);
        return 0;
    }

    me = hwdbg_get_me_data(hd, meid);
    if (!me) {
        char meid_str[NFP_MEID_STR_SZ];
        nfp6000_meid2str(meid_str, meid);

        NS_LOG(NS_LOG_LVL_ERROR,
               "Could not find meid %s for running check", meid_str);
        return 0;
    }

    if (me->stopped == STOP_STATE_RUNNING)
        return 1;

    return 0;
}

static int add_codebp(struct hwdbg_handle *hd,
                      struct hwdbg_me_data *me,
                      int pc)
{
    NS_LOG(NS_LOG_LVL_DEBUG,
           "adding code BP %s:pc%d", me->meid_str, pc);
    return write_active_ustore(hd, me, pc, INSTR_CTXARB_BPT);
}

static int restore_codebp(struct hwdbg_handle *hd,
                          struct hwdbg_me_data *me,
                          int pc)
{
    uint64_t instr;
    NS_LOG(NS_LOG_LVL_DEBUG, "remove code BP %s:pc%d", me->meid_str, pc);

    /* if we delete a code breakpoint that we are stopped at,
     * we should move the pc from the resume routine back
     * to the original instruction
     * This is needed to support clean shutdowns, otherwise
     * the current pc will reflect as being off the code store
     * after starting again.
     */
    if (me->stopped == STOP_STATE_ATBREAK && me->resume_pc == pc) {
        meindcsr_write(hd, me, me->resume_ctx,
                       NFP_ME_INDIRECT_CTX_STS, me->resume_pc);
        me->routine_primed = 0;
    }

    if (get_base_ustore(hd, me, pc, &instr) < 0)
        return -1;

    return write_active_ustore(hd, me, pc, instr);
}

#if 0
/* this code does not work - a0 doesn't let you modify ustore
 * when interrupted; keeping it around for possibly b0
 */

/* resuming from a code breakpoint involves the following:
 * - load orignal instruction in code store
 * - make sure next PC and CTX will point to instruction
 * - set up PC Breakpoint1 so only one instruction will be run
 * - run ME
 * - clear PC breakpoint
 * - restore ctx_arb[bpt]
 * - run ME
 */
static int resume_codebp(struct hwdbg_handle *hd,
                         struct hwdbg_me_data *me)
{
    uint32_t val32;
    int ctxmask = 0xff;
    int pc = me->resume_pc, ctx = me->resume_ctx;
    int i, n;

    /* add a PC break which match on all PC changes on all contexts */
    NS_LOG(NS_LOG_LVL_DEBUG,
           "resuming code BP me%x:ctx%d:pc%d", me->meid, ctx, pc);

    me->active_codebp = 0;
    if (mecsr_write(hd, me, NFP_ME_PC_BREAKPOINT_1,
                        NFP_ME_PCBREAKPOINT_CNTL(0xff)) < 0)
        return -1;
    if (mecsr_write(hd, me, NFP_ME_PC_BREAKPOINT_1MASK, 0x3fff) < 0)
        return -1;
    NS_LOG(NS_LOG_LVL_EXTRA, "set breakpoint");

    /* enable all contexts */
    if (mecsr_read(hd, me, NFP_ME_CTX_ENABLES, &val32) < 0)
        return -1;

    if (val32 & NFP_ME_CTX_ENABLES_IN_USE) /* if 4 in use */
        ctxmask &= 0x55;
    val32 |= NFP_ME_CTX_ENABLES_CONTEXTS(ctxmask);
    if (mecsr_write(hd, me, NFP_ME_CTX_ENABLES, val32) < 0)
        return -1;
    NS_LOG(NS_LOG_LVL_EXTRA, "enabled contexts");

    /* poll PCBP1 -- this should stop the ME immediately
     * after the base intruction is run */
    NS_LOG(NS_LOG_LVL_EXTRA, "Waiting for one instruction to execute");
    for (i = 0; i < 2; i++) { /* instruction is executed after two PCB hits */
        n = 100; /* we poll for a PCB hit 100 times */
        while (n--) {
            if (mecsr_read(hd, me, NFP_ME_PC_BREAKPOINT_STATUS, &val32) < 0)
                return -1;
            NS_LOG(NS_LOG_LVL_HEAVY, "waiting for PCBREAKPOINT", val32);
            if (val32 & NFP_ME_PC_BREAKPOINT_STATUS_STATUS1) /* PCB happened */
                break;
            if (n == 0) {
                NS_LOG(NS_LOG_LVL_ERROR,
                       "failed to resume from code breakpoint, PCB never hit");
                /* disable pcb1 */
                mecsr_write(hd, me, NFP_ME_PC_BREAKPOINT_1, 0x0);
                return -1;
            }
#ifdef SDK_SIM
            /* XXX wait a while for the bpt to clear */
            wait_mecycles(hd, me, 10);
#endif
            usleep(100); /* TODO: tune this delay */
        }
#ifdef SDK_SIM
        /* XXX wait a while for the me to enter the idle state */
        wait_mecycles(hd, me, 10);
#endif
        NS_LOG(NS_LOG_LVL_EXTRA, "PCB1 hit");
        if (i == 1) {
            if (mecsr_write(hd, me, NFP_ME_PC_BREAKPOINT_1, 0x0) < 0)
                return -1;
            /* Restore software breakpoint */
            if (write_active_ustore(hd, me, pc, INSTR_CTXARB_BPT) < 0)
                return -1;
            NS_LOG(NS_LOG_LVL_EXTRA, "restored codebp");
        }
        /* acknowledge PCBP1 */
        /* disable breaks on PCB1 */
        if (mecsr_read(hd, me, NFP_ME_PC_BREAKPOINT_STATUS, &val32) < 0)
            return -1;
        val32 |= NFP_ME_PC_BREAKPOINT_STATUS_STATUS1;
        if (mecsr_write(hd, me, NFP_ME_PC_BREAKPOINT_STATUS, val32) < 0)
            return -1;
        NS_LOG(NS_LOG_LVL_EXTRA, "cleared pcb");
#ifdef SDK_SIM
        /* XXX wait a while for the bpt to clear */
        wait_mecycles(hd, me, 10);
#endif
        /* enable contexts */
        if (mecsr_read(hd, me, NFP_ME_CTX_ENABLES, &val32) < 0)
            return -1;
        if (val32 & NFP_ME_CTX_ENABLES_IN_USE) /* if 4 in use */
            ctxmask &= 0x55;
        val32 |= NFP_ME_CTX_ENABLES_CONTEXTS(ctxmask);
        if (mecsr_write(hd, me, NFP_ME_CTX_ENABLES, val32) < 0)
            return -1;
        NS_LOG(NS_LOG_LVL_EXTRA, "enabled contexts again");
    }
    /* we should be running at this point */
    me->stopped = STOP_STATE_RUNNING;
    return 0;
}
#endif

/* get the status 'running' status of an ME */
static int load_me_status(struct hwdbg_handle *hd, struct hwdbg_me_data *me)
{
    uint32_t val32;

    if (mecsr_read(hd, me, NFP_ME_CTX_ENABLES, &val32) < 0)
        return -1;

    if (NFP_ME_CTX_ENABLES_CONTEXTS_of(val32) != 0)
        me->stopped = STOP_STATE_RUNNING;
    else
        me->stopped = STOP_STATE_STOPPED;

    return 0;
}

/* resume a stopped ME */
static int resume_me(struct hwdbg_handle *hd,
                     struct hwdbg_me_data *me)
{
    uint32_t val32;
    int ctxmask = 0xff;

    NS_LOG(NS_LOG_LVL_DEBUG, "resuming %s", me->meid_str);
    /* enable all contexts */
    if (mecsr_read(hd, me, NFP_ME_CTX_ENABLES, &val32) < 0)
        return -1;
    if (val32 & NFP_ME_CTX_ENABLES_IN_USE) /* if 4 in use */
        ctxmask &= 0x55;
    val32 |= NFP_ME_CTX_ENABLES_CONTEXTS(ctxmask);
    if (mecsr_write(hd, me, NFP_ME_CTX_ENABLES, val32) < 0)
        return -1;
    NS_LOG(NS_LOG_LVL_EXTRA, "enabled contexts");
    me->stopped = STOP_STATE_RUNNING;
    return 0;
}

/* stop an ME */
static int stop_me(struct hwdbg_handle *hd,
                   struct hwdbg_me_data *me)
{
    uint32_t val32, readback;
    int ctxmask = 0xff;
    int loops = 100;
    int ret = 1; /* default is timeout */
    int ctxen_rb_ok = 0;

    NS_LOG(NS_LOG_LVL_DEBUG, "stopping %s", me->meid_str);
    /* enable all contexts */
    if (mecsr_read(hd, me, NFP_ME_CTX_ENABLES, &val32) < 0)
        return -1;

    /* prevent clearing bkpt bit */
    val32 &= ~(NFP_ME_CTX_ENABLES_BREAKPOINT);
    /* clear the ctx bits */
    val32 &= ~NFP_ME_CTX_ENABLES_CONTEXTS(ctxmask);
    if (mecsr_write(hd, me, NFP_ME_CTX_ENABLES, val32) < 0)
        return -1;

    do {
        if (!ctxen_rb_ok) {
            if (mecsr_read(hd, me, NFP_ME_CTX_ENABLES, &readback) < 0)
                return -1;
            if (NFP_ME_CTX_ENABLES_CONTEXTS_of(readback) ==
                NFP_ME_CTX_ENABLES_CONTEXTS_of(val32)) {
                ctxen_rb_ok = 1;
            }
        }
        if (ctxen_rb_ok){
            /* get the active ctx and pc */
            if (mecsr_read(hd, me, NFP_ME_ACTIVE_CTX_STS, &val32))
                return -1;

            if (!(val32 & NFP_ME_ACTIVE_CTX_STS_AB0)) {
                ret = 0;
                break;
            }
        }
        usleep(100); /* TODO: tune loops and delay */
    } while (loops--);

    if (!ret) {
#ifdef SDK_SIM
        /* XXX wait a while for the pipe to clear */
        wait_mecycles(hd, me, 5);
#endif
        /* if the me was stopped, keep the stopped state */
        NS_LOG(NS_LOG_LVL_DEBUG, "stop done");
        me->stopped = STOP_STATE_STOPPED;
        /* store the active ctx and pc */
        if (mecsr_read(hd, me, NFP_ME_ACTIVE_CTX_STS, &val32) < 0)
            me->resume_ctx = -1;
        else
            me->resume_ctx = NFP_ME_ACTIVE_CTX_STS_ACNO_of(val32);

        if (meindcsr_read(hd, me, me->resume_ctx,
                          NFP_ME_INDIRECT_CTX_STS, &val32) < 0)
            me->resume_pc = -1;
        else
            me->resume_pc = NFP_ME_INDIRECT_CTX_STS_CTX_PC_of(val32); // FIX ME -- for 3800 this should be NFP3800_ME_INDIRECT_CTX_STS_CTX_PC_of
    } else
        NS_LOG(NS_LOG_LVL_WARN, "failed to stop %s", me->meid_str);

#if 0
    if (ret) {
    /* TODO: we can try desperate ctx_arb[bpt] injection here */
        ret = flood_bpt_stop();
    }
#endif

    return ret;
}

/* stop an ME for a quick task, like ustore access
 * where we are likely within some other task
 * and don't want to mess up states
 *
 * Returns -1 on error, 0 if the ME was already stopped and 1 if we
 * stopped it now. ctxen_val is only valid if 1 is returned
 */
static int short_stop_me(struct hwdbg_handle *hd,
                         struct hwdbg_me_data *me,
                         uint32_t *ctxen_val)
{
    uint32_t val32, readback;
    int ctxmask = 0xff;
    int loops = 100;
    int ctxen_rb_ok = 0;
    int was_stopped;

    NS_LOG(NS_LOG_LVL_DEBUG, "stopping %s", me->meid_str);
    /* enable all contexts */
    if (mecsr_read(hd, me, NFP_ME_CTX_ENABLES, &val32) < 0)
        return -1;

    was_stopped = NFP_ME_CTX_ENABLES_CONTEXTS_of(val32) == 0;

    if (!was_stopped) {
        /* prevent clearing bkpt bit */
        val32 &= ~(NFP_ME_CTX_ENABLES_BREAKPOINT);
        *ctxen_val = val32;
        /* clear the ctx bits */
        val32 &= ~NFP_ME_CTX_ENABLES_CONTEXTS(ctxmask);
        if (mecsr_write(hd, me, NFP_ME_CTX_ENABLES, val32) < 0)
            return -1;
    }

    do {
        if (!ctxen_rb_ok) {
            if (mecsr_read(hd, me, NFP_ME_CTX_ENABLES, &readback) < 0)
                return -1;
            if (NFP_ME_CTX_ENABLES_CONTEXTS_of(readback) ==
                NFP_ME_CTX_ENABLES_CONTEXTS_of(val32)) {
                ctxen_rb_ok = 1;
            }
        }
        if (ctxen_rb_ok){
            /* get the active ctx and pc */
            if (mecsr_read(hd, me, NFP_ME_ACTIVE_CTX_STS, &val32))
                return -1;

            if (!(val32 & NFP_ME_ACTIVE_CTX_STS_AB0)) {
                loops = -1;
                break;
            }
        }
        usleep(100); /* TODO: tune loops and delay */
    } while (loops--);

    if (loops == -1) {
#ifdef SDK_SIM
        /* XXX wait a while for the pipe to clear */
        wait_mecycles(hd, me, 5);
#endif
        /* if the me was stopped, keep the stopped state */
        NS_LOG(NS_LOG_LVL_DEBUG, "short stop done");
    } else
        NS_LOG(NS_LOG_LVL_DEBUG, "failed to stop %s", me->meid_str);

    return !was_stopped;
}

/******* Check if a software breakpoint occurred ********/

/* detect a breakpoint */
static int check_swbreak(struct hwdbg_handle *hd,
                         struct hwdbg_me_data *me,
                         int *pc, int *ctx)
{
    uint32_t val32, ctxens;

    /* XXX: dbgsrv47 looked at resets here */

    if (mecsr_read(hd, me, NFP_ME_CTX_ENABLES, &val32) < 0)
        return -1;

    if (!(val32 & NFP_ME_CTX_ENABLES_BREAKPOINT))
        return 0; /* no break occurred */

    if (NFP_ME_CTX_ENABLES_CONTEXTS_of(val32))
        return 0; /* contexts still running */

    ctxens = val32;

#ifdef SDK_SIM
    /* XXX the simulator needs to wait a minimum of 2 cycles */
    wait_mecycles(hd, me, 2);
#endif

    /* get the active ctx and pc */
    if (mecsr_read(hd, me, NFP_ME_ACTIVE_CTX_STS, &val32))
        return -1;

    if (val32 & NFP_ME_ACTIVE_CTX_STS_AB0)
        return 0;  /* ignore if ctx is running */

    /* at this point we must tag the me as stopped on a break */
    me->stopped = STOP_STATE_ATBREAK;

    *ctx = NFP_ME_ACTIVE_CTX_STS_ACNO_of(val32);

    if (meindcsr_read(hd, me, *ctx, NFP_ME_INDIRECT_CTX_STS, &val32) < 0)
        return -1;

    /* store the pc to where the break happened rather than the next PC */
    *pc = NFP_ME_INDIRECT_CTX_STS_CTX_PC_of(val32) - 1; // FIX ME -- for 3800 this should be NFP3800_ME_INDIRECT_CTX_STS_CTX_PC_of

    /* acknowledge break, so we don't detect it again */
    mecsr_write(hd,
                me,
                NFP_ME_CTX_ENABLES,
                ctxens | NFP_ME_CTX_ENABLES_BREAKPOINT);

#ifdef SDK_SIM
    /* XXX the simulator needs to wait a min 5 cycles before taking
     *     action after ack'ing the sw brkpoint */
    wait_mecycles(hd, me, 5);
#endif

    return 1; /* BP - detected */
}

/* categories of breakpoints */
/* user ctx_arb[bpt] */
#define BRCAT_SWBPT        (0)
/* code BP ctx_arb[bpt] that hit a matching context */
#define BRCAT_CODEBPT      (1)
/* code BP ctx_arb[bpt] that missed a matching context */
#define BRCAT_CODEBPT_MISS (2)
/* ctx_arb[bpt] for step  */
#define BRCAT_STEPBPT      (3)

/* prime the ME to resume */
static int categorize_swbreak(struct hwdbg_handle *hd,
                              struct hwdbg_me_data *me,
                              int pc, int ctx, int was_step)
{
    int ret = 0;

    /* a step that hit on the desired ctx */
    if (was_step)
        return BRCAT_STEPBPT;

    /* determine if the original instruction was a ctx_arb[bpt] */
    if (base_was_swbpt(hd, me, pc)) {
        ret = BRCAT_SWBPT;
        NS_LOG(NS_LOG_LVL_DEBUG,
               "software BP on %s:pc%d:ctx%d", me->meid_str, pc, ctx);
    } else {
         if (hwdbg_mgmt_codebp_matches(dbgsrv_glob->hwdbg_mgmt,
                                       me->meid, ctx, pc)) {
             NS_LOG(NS_LOG_LVL_DEBUG,
                    "code BP on %s:pc%d:ctx%d", me->meid_str, pc, ctx);
             ret = BRCAT_CODEBPT;
         } else {
             NS_LOG(NS_LOG_LVL_DEBUG,
                    "missed code/step BP on %s:pc%d:ctx%d",
                    me->meid_str, pc, ctx);
             ret = BRCAT_CODEBPT_MISS;
         }
    }

    /* we already know this isn't a real break so dont report */
    if (ret == BRCAT_CODEBPT_MISS)
        return ret;

    /* notify the clients about the break */
    hwdbg_mgmt_handle_swbrk(dbgsrv_glob->hwdbg_mgmt,
                            me->meid, ctx, pc, ret == BRCAT_SWBPT);

    return ret;
}

/* prime the ME to resume for a code break without PCB */
static int prime_swbreak_nopcb(struct hwdbg_handle *hd,
                               struct hwdbg_me_data *me,
                               int pc, int ctx)
{
    uint64_t instr;
    int defers;
    int rtn_len, rtn_pc;
    unsigned int nxt_pc;
    int i;

    if (!me->dbginfo) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Cannot inject codeBP resume routine for %s "
               "without debug info loaded", me->meid_str);
        return -1;
    }

    if (pc >= me->dbginfo_len + me->dbginfo_off || pc < me->dbginfo_off) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Exceeded debug info bounds priming %s to resume at pc %d",
               me->meid_str, pc);
        return -1;
    }

    /* get the number of defers after the instruction */
    defers = NFP_HWDBG_DBGINFO_DEFERCNT_of(me->dbginfo[pc - me->dbginfo_off]);

    /* figure out how long the resume routine will be */
    rtn_len = 1 /* instruction to run */ + defers + 1 /* branch back */;
    if (rtn_len > ROUTINE_INSTR_CNT) {
        NS_LOG(NS_LOG_LVL_ERROR, "Unexpected routine size.");
        return -1;
    }

    rtn_pc = me->routine_addr;
    /* copy the instruction to run and the defers into the end of CS */
    for (i = 0; i < 1 + defers; i++) {
        if (get_base_ustore(hd, me, pc + i, &instr)) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to get instruction for resume routine %s:pc%d",
                   me->meid_str, pc + i);
            return -1;
        }
        if (write_active_ustore(hd, me, rtn_pc + i, instr) < 0) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to write resume routine instruction at %s:pc%d",
                   me->meid_str, rtn_pc + i);
            return -1;
        }
    }
    /* load up an unconditional branch back to the instruction after the last
     * of the defers
     */

    /* the next PC to we jump to after the resume routine */
    nxt_pc = pc + rtn_len - 1;

    /* first load upper half */
    instr = 0x00D8 |
           (((nxt_pc) & 0x2000) >>  5) |
           (((nxt_pc) & 0x1c00) >> 10);
    instr <<= 32;
    /* now the lower half */
    instr |= 0x00008038 | (((nxt_pc) & 0x3ff) << 22);

    /* write in the branch back */
    if (write_active_ustore(hd, me, rtn_pc + i, instr) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "failed to write resume instruction at %s:pc%d",
               me->meid_str, rtn_pc + i);
        return -1;
    }

    /* now set PC to the start of the resume routine */
    if (meindcsr_write(hd, me, ctx, NFP_ME_INDIRECT_CTX_STS, rtn_pc) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "failed to set resume routine pc %d", rtn_pc);
        return -1;
    }

    me->routine_primed = 1;

    NS_LOG(NS_LOG_LVL_DEBUG,
           "loaded resume routine of length %d at %s:pc%d",
           rtn_len, me->meid_str, rtn_pc);

    return 0;
}

/* prime the ME to resume */
static int prime_swbreak(struct hwdbg_handle *hd,
                         struct hwdbg_me_data *me,
                         int pc, int ctx,
                         int brcat)
{
    uint32_t val32;

    /* set next context to run */
    NS_LOG(NS_LOG_LVL_EXTRA, "restored original context %d", ctx);
    mecsr_write(hd, me, NFP_ME_CTX_ARB_CNTL, ctx);

    /* setup wakeup event */
    meindcsr_read(hd, me, ctx,
                  NFP_ME_INDIRECT_CTX_WAKEUP_EVENTS, &val32);
    if (!(val32 & 0xffff)) {
        val32 |= NFP_ME_CTXWKPEVT_VOLUNTARY;
        NS_LOG(NS_LOG_LVL_EXTRA, "setting voluntary wakeup");
        meindcsr_write(hd, me, ctx,
                           NFP_ME_INDIRECT_CTX_WAKEUP_EVENTS, val32);
    }

    /* store this for later */
    me->resume_pc = pc;
    me->resume_ctx = ctx;
    me->routine_primed = 0;
    me->active_codebp = 0;

    if (brcat == BRCAT_SWBPT) {
        /* set PC to instruction after ctx_arb[bpt] */
        meindcsr_write(hd, me, ctx, NFP_ME_INDIRECT_CTX_STS, pc + 1);
        NS_LOG(NS_LOG_LVL_EXTRA, "loaded next pc %d", pc + 1);
        /* make sure the pc points to the next intruction to be run */
        me->resume_pc++;
    } else if (brcat == BRCAT_STEPBPT) { /* step bpt */
        /* set PC to instruction where ctx_arb[bpt] hit */
        meindcsr_write(hd, me, ctx, NFP_ME_INDIRECT_CTX_STS, pc);
        NS_LOG(NS_LOG_LVL_EXTRA, "loaded next pc %d", pc);
    } else {
        if (prime_swbreak_nopcb(hd, me, pc, ctx) < 0)
            return -1;
    }
#if 0
    /* PCB break resume doesn't work for a0 */
    else { /* code break */
        /* load original instruction */
        if (get_base_ustore(hd, me, pc, &instr)) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to read original instruction");
            return -1;
        }
        if (write_active_ustore(hd, me, pc, instr) < 0) {
            NS_LOG(NS_LOG_LVL_ERROR,
                   "failed to restore original instruction");
            return -1;
        }
        NS_LOG(NS_LOG_LVL_EXTRA,
               "restored original instruction %lx", instr);

        /* set PC where ctx_arb[bpt] hit */
        if (meindcsr_write(hd, me, ctx, NFP_ME_INDIRECT_CTX_STS, pc) < 0) {
            NS_LOG(NS_LOG_LVL_ERROR, "failed to restore pc %d", pc);
            return -1;
        }

        NS_LOG(NS_LOG_LVL_EXTRA, "restored original pc %d", pc);

        /* make sure that we take codebp resume action later */
        me->active_codebp = 1;
    }
#endif
    NS_LOG(NS_LOG_LVL_DEBUG,
           "primed %s for resume on %d", me->meid_str, me->resume_pc);

    return 0;
}

/******* step context ********/

/* a list entry for keeping track of the ctx_arb[bpt]'s we added as
 * part of the single step
 */
struct step_bp_entry {
    uint16_t pc; /* location of the injected break */
    uint64_t oldinst; /* substituted instruction */
    struct step_bp_entry *next;
};

static void free_step_bp_list(struct hwdbg_handle *hd,
                              struct hwdbg_me_data *me,
                              struct step_bp_entry *l,
                              int noustorewr)
{
    struct step_bp_entry *e, *en;
    for (e = l; e; e = en) {
        en = e->next;
        if (!noustorewr)
            write_active_ustore(hd, me, e->pc, e->oldinst);
        free(e);
    }
}

static int is_step_bp(struct step_bp_entry *l, int pc)
{
    struct step_bp_entry *e;
    for (e = l; e; e = e->next) {
        if (e->pc == pc)
            return 1;
    }
    return 0;
}

static int add_step_bp(struct hwdbg_handle *hd,
                        struct hwdbg_me_data *me,
                        struct step_bp_entry **l,
                        int pc)
{
    struct step_bp_entry *e;

    e = malloc(sizeof(*e));
    if (!e) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to allocate step break entry");
        return -1;
    }

    e->pc = pc;
    e->next = *l;

    /* Note: we read the active ustore so we can restore the ustore
     *       to its original state after the step is done. Note this isn't
     *       the original (base) ustore as we may copy back a code breakpoint
     *       which isn't in the original(base) store
     */
    if (read_active_ustore(hd, me, pc, &e->oldinst) < 0) {
        free(e);
        return -1;
    }

    if (write_active_ustore(hd, me, pc, INSTR_CTXARB_BPT) < 0) {
        free(e);
        return -1;
    }

    *l = e;
    return 0;
}

#define STEP_RECURSE_MAX 4

/* inject ctx_arb[bpts] into the PC of the next intruction to execute
 * note that we have to inject at branch locations too
 * we need also to take care not to inject them where
 * breakpoints are not allowed
 */
/* this code is based on dbgsrv_srvif.c:insertStep() */
static int add_step_bps(struct hwdbg_handle *hd,
                        struct hwdbg_me_data *me,
                        int pc, int ctx,
                        struct step_bp_entry **bp_list,
                        int recurse,
                        int skipuword)
{
    int16_t nxtpc;
    int off;

    if (!recurse) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Reached recurse limit while adding step for %s:%d",
               me->meid_str, pc);
        return -1;
    }

    off = me->dbginfo_off;

    while (1) {
        if (pc >= me->dbginfo_len + off || pc < off) {
            if (*bp_list) {
                /* a step has been added, which means it was a branch
                 * with no instruction for the no branch case
                 */
                NS_LOG(NS_LOG_LVL_DEBUG,
                       "Detected step at branch at end of codestore; "
                       "if branch isn't taken there may be trouble");
                return 0;
            }
            /* give error when stepping into the ether (beyond loaded code)  */
            NS_LOG(NS_LOG_LVL_ERROR,
                   "Exceeded debug info bounds while stepping %s at pc %d",
                   me->meid_str, pc);
            return -1;
        }

        if (skipuword) {
            /* if the instruction we are adding a break around is ctx_arb[kill]
             * dont try to figure out what the next instruction is */
            if (me->dbginfo[pc - off] & NFP_HWDBG_DBGINFO_CTXARBKILL)
                break;
        }

        if (!skipuword) {
            if (me->dbginfo[pc - off] &
                                NFP_HWDBG_DBGINFO_BRKPTALLOWED) {
                if (add_step_bp(hd, me, bp_list, pc) < 0) {
                    NS_LOG(NS_LOG_LVL_ERROR,
                           "Failed to add step breakpoint at %s:pc%d",
                           me->meid_str, pc);
                    return -1;
                }
                break; /* success */
            }
        }
        /* we now try to add a break to the next instruction */
        nxtpc = NFP_HWDBG_DBGINFO_BRADDR_of(me->dbginfo[pc - off]);
        if (nxtpc >= 0) {
            int regtype, regaddr;
            uint32_t inc = 0;
            /* ok - instruction was a branch, we now have to figure out where
             * the branch will take us */
             regtype = NFP_HWDBG_DBGINFO_REGTYPE_of(me->dbginfo[pc - off]);
             regaddr = NFP_HWDBG_DBGINFO_REGADDR_of(me->dbginfo[pc - off]);

             if (regtype != 0) {
                 if (brreg_get_offset(hd->nfp, me->meid, ctx,
                                      regtype, regaddr, &inc) < 0) {
                     return -1;
                 }
                 nxtpc += inc;
             }
             if (add_step_bps(hd, me, nxtpc, ctx, bp_list, recurse--, 0) < 0)
                 return -1;
             /* success - we have now added bpt(s) at the branch location,
              *           we continue on to try adding a bpt at the next pc
              */
        }
        skipuword = 0;
        pc++;
    }

    return 0;
}

static int step_context(struct hwdbg_handle *hd,
                        struct hwdbg_me_data *me,
                        int ctx)
{
    /* list of bps that have been injected as part of the step */
    struct step_bp_entry *steps, *e;
    struct timespec ts;
    /* timer values in milliseconds */
    uint64_t t_start, t_now, t_diff;
    uint32_t val32;
    int cur_ctx;
    int pc;
    int gotbreak, ignorebreak;
    int ret;

    NS_LOG(NS_LOG_LVL_DEBUG, "Stepping context %d", ctx);

    clock_gettime(CLOCK_MONOTONIC, &ts);
    t_start = ts.tv_sec * 1000 + ts.tv_nsec/(1000*1000);

    /* get the active ctx and pc */
    if (mecsr_read(hd, me, NFP_ME_ACTIVE_CTX_STS, &val32))
        return -EIO;

    if (val32 & NFP_ME_ACTIVE_CTX_STS_AB0)
        return -EBUSY;  /* dont step if ctx is running */

    mecsr_read(hd, me, NFP_ME_CTX_ARB_CNTL, &val32);
    cur_ctx = NFP_ME_CTX_ARB_CNTL_NCTX_of(val32);

    /* if we are stepping the current context, latch the context to step */
    if (ctx < 0)
        ctx = cur_ctx;

    /* XXX: need to make sure we dont step a killed context? */

    steps = NULL;

    if (me->routine_primed && cur_ctx == ctx) {
        /* there is a code breakpoint routine primed to run
         * this means the current pc is pointing off the app code
         * space - use the pc that was stored when the code bp hit
         */
         pc = me->resume_pc;
    } else {
        if (meindcsr_read(hd, me, ctx,
                          NFP_ME_INDIRECT_CTX_STS, &val32) < 0)
            return -EIO;

        /* store the pc */
        pc = NFP_ME_INDIRECT_CTX_STS_CTX_PC_of(val32); // FIX ME -- for 3800 this should be NFP3800_ME_INDIRECT_CTX_STS_CTX_PC_of
    }

    NS_LOG(NS_LOG_LVL_DEBUG,
           "Stepping %s at pc %d, ctx %d", me->meid_str, pc, ctx);

    if (add_step_bps(hd, me, pc, ctx, &steps, STEP_RECURSE_MAX, 1) < 0) {
        free_step_bp_list(hd, me, steps, 0);
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to inject breakpoints for step");
        return -EIO;
    }

    /* no steps added - this means we must run freely */
    if (steps == NULL) {
        /* resume me */
#if 0
        /* pcb breaks don't work for a0 */
        if (me->active_codebp)
            ret = resume_codebp(hd, me);
        else
#endif
        ret = resume_me(hd, me);
        return 0;
    }

    NS_LOG(NS_LOG_LVL_DEBUG, "Added swbpt for step on %s", me->meid_str);
    for (e = steps; e; e = e->next)
        NS_LOG(NS_LOG_LVL_DEBUG, "-> pc:%d", e->pc);

    /* Note: self-step case
     * there is a nasty corner case where the step bpt would be over the
     * instruction to be stepped. In this case we cant leave the PC at the
     * current location, we need to point it off the ustore edge, ala code BP
     * routines.
     *
     * e.g
     *  wait_for_sig#:
     *  br_!signal[sync_sig, wait_for_sig#]
     *
     * note: if a resume routine for a code bp is loaded it will work fine
     */
    if (is_step_bp(steps, pc) && !me->routine_primed) {
        if (!me->routine_primed) {
            if (prime_swbreak_nopcb(hd, me, pc, ctx) < 0) {
                free_step_bp_list(hd, me, steps, 0);
                NS_LOG(NS_LOG_LVL_ERROR, "failed to prime self-step BP");
                return -EIO;
            }
            NS_LOG(NS_LOG_LVL_DEBUG, "Stepping over self-step BP");
        }
    }

    /* resume me */
#if 0
    if (me->active_codebp)
        ret = resume_codebp(hd, me);
    else
#endif
    ret = resume_me(hd, me);

    if (ret < 0) {
        free_step_bp_list(hd, me, steps, 0);
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to resume ME");
        return -EIO;
    }

    NS_LOG(NS_LOG_LVL_DEBUG, "Waiting for BP to hit");
    /* wait for break */
    ret = -1;
    while (1) {
        int brpc, brctx, brcat;

        gotbreak = check_swbreak(hd, me, &brpc, &brctx);

        /* keep trying until we hit our on the desired context */
        clock_gettime(CLOCK_MONOTONIC, &ts);
        t_now = ts.tv_sec * 1000 + ts.tv_nsec/(1000*1000);
        t_diff = t_now - t_start;

        if (gotbreak != 1) {
            /* check that the timeout is reached, we add a little time here
             * so that the timeout when stopped code below gets preference
             */
            if (t_diff < STEP_TIMEOUT_MS + 2) {
                usleep(100);
                /* check for a bp again */
                continue;
            }
            if (stop_me(hd, me) < 0) {
                NS_LOG(NS_LOG_LVL_ERROR,
                       "Step timed out; ME could not be stopped!");
                free_step_bp_list(hd, me, steps, 1);
                return -ETIMEDOUT;
            }

            /* free the inject BPs */
            NS_LOG(NS_LOG_LVL_ERROR, "Step timed out while running");
            free_step_bp_list(hd, me, steps, 0);

            /* There is a tiny possibility that the ME stopped at a breakpoint,
             * so we need to handle it just-in-case
             */
            if (check_swbreak(hd, me, &brpc, &brctx) == 1) {
                int brcat;
                /* we stopped on a bp, so we need to handle it now */
                brcat = categorize_swbreak(hd, me, brpc, brctx, 0);
                /* prime the break to be resumed */
                prime_swbreak(hd, me, brpc, brctx, brcat);
                return 0;
            } else {
                /* be sure to clean up after any primed resumes */
                me->routine_primed = 0;
            }

            return -ETIMEDOUT;
        }

        /* check if the break was the desired step BP */
        ignorebreak = 0;
        if (brctx == ctx && is_step_bp(steps, brpc)) {
            NS_LOG(NS_LOG_LVL_DEBUG, "hit step breakpoint on pc %d", brpc);
            ret = 0;
            /* make it so we check for breaks in the self-step case */
            if (brpc != pc) {
                ignorebreak = 1;
            }
        }

        brcat = categorize_swbreak(hd, me, brpc, brctx, ignorebreak);

        if (prime_swbreak(hd, me, brpc, brctx, brcat)) {
            free_step_bp_list(hd, me, steps, 0);
            NS_LOG(NS_LOG_LVL_ERROR, "Failed to prime break resume");
            return -EIO;
        }

        if (brctx != ctx && (brcat == BRCAT_SWBPT || brcat == BRCAT_CODEBPT)) {
            /* if we hit a BP on another context we are done too,
             * BP on this context can only happen if we are stepping over a BP
             */
            NS_LOG(NS_LOG_LVL_DEBUG,
                   "hit breakpoint on pc %d ctx %d while stepping",
                   brpc, brctx);
            ret = 0;
        }

        if (!ret)
            break;

        /* check that the timeout is reached */
        if (t_diff > STEP_TIMEOUT_MS) {
            ret = -1;
            NS_LOG(NS_LOG_LVL_ERROR, "Step timed out while stopped");
            free_step_bp_list(hd, me, steps, 0);
            return -ETIMEDOUT;
        }

        NS_LOG(NS_LOG_LVL_DEBUG,
               "stepped over breakpoint at %d, resuming", pc);
#if 0
        if (me->active_codebp)
            resume_codebp(hd, me);
        else
#endif
        resume_me(hd, me);
        /* try again */
    };

    /* restore the original code */
    free_step_bp_list(hd, me, steps, 0);

    if (ret < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Timed out waiting for swbrk");
        return -ETIMEDOUT;
    }

    return 0;
}


/******** Break check logic *********/

static void do_swbreak(struct hwdbg_handle *hd, struct hwdbg_me_data *me)
{
    int pc, ctx, brcat;
    int i;

    if (me->disabled)
        return;

    if (check_swbreak(hd, me, &pc, &ctx) != 1)
        return;
    /* sw break encountered */
    brcat = categorize_swbreak(hd, me, pc, ctx, 0);
    /* prime the break to be resumed */
    prime_swbreak(hd, me, pc, ctx, brcat);

    /* if the break was a missed codebp, resume the me */
    if (brcat == BRCAT_CODEBPT_MISS) {
#if 0
        if (me->active_codebp)
            resume_codebp(hd, me);
        else
#endif
        resume_me(hd, me);
        return;
    }
    if (!me->brkgrp_meids)
        return;
    /* break all MEs in an associated break group */
    for (i = 0; i < me->brkgrp_len; i++) {
        struct hwdbg_me_data *brkme;
        brkme = hwdbg_get_me_data(hd, me->brkgrp_meids[i]);
        if (!brkme) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "Failed to find MEID %x data for group break",
                   me->brkgrp_meids[i]);
        }
        if (brkme->stopped != STOP_STATE_RUNNING)
            continue;
        if (stop_me(hd, brkme) != 0) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "Failed to stop MEID %x data for group break",
                   me->brkgrp_meids[i]);
        } else {
            struct nfp_hwdbg_event_pl_groupstop gs;
            gs.meid = brkme->meid;
            hwdbg_mgmt_send_event(dbgsrv_glob->hwdbg_mgmt,
                                  NFP_HWDBG_EVENT_TYPE_GROUPSTOP,
                                  &gs,
                                  sizeof(gs));
        }
    }
}

static void do_swbreaks(struct hwdbg_handle *hd)
{
    struct hwdbg_me_data *me;
    int i;

    for (i = 0; i < hd->me_count; i++) {
#ifdef SDK_SIM
        if (hd->stop_thread)
            break;
#endif
        me = &hd->me_data[i];
        if (pthread_mutex_lock(hd->nfp_mtx) < 0) {
            NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire nfp lock");
            return;
        }
        do_swbreak(hd, me);
        pthread_mutex_unlock(hd->nfp_mtx);
    }
}

/******** SW break check thread *********/

static void *swbrk_work(void *arg)
{
    struct hwdbg_handle *hd = arg;

    while (!hd->stop_thread) {
        pthread_mutex_lock(dbgsrv_glob->load_mtx);

        /* only look for breaks if the load is not busy */
        if (!dbgsrv_glob->load_busy)
            do_swbreaks(hd);

        pthread_mutex_unlock(dbgsrv_glob->load_mtx);
        usleep(DBGSRV_DEFAULT_SWBRKPOLL);
    }

    NS_LOG(NS_LOG_LVL_DEBUG, "SW break thread stopped");
    pthread_exit(NULL);
    return NULL;
}

/***** Interaction with hwdbg_mgmt ******/

int hwdbg_stop_me(struct hwdbg_handle *hd, int meid, int suppress)
{
    struct hwdbg_me_data *me;
    int ret;
    /* lock is already held */

    me = hwdbg_get_me_data(hd, meid);
    if (!me) {
        if (suppress)
            return 0;

        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to find ME%x data while stoping ME", meid);
        return -ENOENT;
    }

    if (me->stopped == STOP_STATE_ATBREAK) {
        if (suppress)
            return 0;

        NS_LOG(NS_LOG_LVL_DEBUG,
                "Ignoring stop on %s which is at break", me->meid_str);
        return 0;
    }

    if (me->stopped == STOP_STATE_STOPPED) {
        if (suppress)
            return 0;

        NS_LOG(NS_LOG_LVL_WARN,
                "Attempted to stop an ME that is not running");
        return -EALREADY;
    }

    ret = stop_me(hd, me);

    if (!ret) {
        /* if we stopped on a code / SW break report that, report it
         * and prime the me to run again
         */
        int pc, ctx, brcat;
        if (check_swbreak(hd, me, &pc, &ctx) == 1) {
            /* we stopped on a bp, so we need to handle it now */
            brcat = categorize_swbreak(hd, me, pc, ctx, 0);
            /* prime the break to be resumed */
            prime_swbreak(hd, me, pc, ctx, brcat);
        }
    }

    if (ret < 0)
        return -EIO;

    return ret;
}

int hwdbg_run_me(struct hwdbg_handle *hd, int meid, int suppress)
{
    struct hwdbg_me_data *me;
    int ret;
    /* lock is already held */

    me = hwdbg_get_me_data(hd, meid);
    if (!me) {
        if (suppress)
            return 0;
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to find ME%x data while running ME", meid);
        return -1;
    }

    if (me->stopped == STOP_STATE_RUNNING) {
        if (suppress)
            return 0;
        NS_LOG(NS_LOG_LVL_WARN,
               "Run requested on %s which is not stopped", me->meid_str);
    }

    ret = resume_me(hd, me);

    return ret;
}

int hwdbg_add_codebp(struct hwdbg_handle *hd, int meid, int pc)
{
    struct hwdbg_me_data *me, *scsme;
    struct hwdbg_me_data *qcsme[QCSMENUM];
    int i = 0;

    for (i = 0; i < QCSMENUM; i++)
        qcsme[i] = NULL;

    /* lock is already held */

    me = hwdbg_get_me_data(hd, meid);
    if (!me) {
        NS_LOG(NS_LOG_LVL_WARN, "Failed to find ME%x data for code BP", meid);
        return -ENOENT;
    }

    /* check if me (and scs me, if needed) is stopped */
    if (me->stopped == STOP_STATE_RUNNING) {
        NS_LOG(NS_LOG_LVL_WARN,
               "Attempted to add code break while %s was running",
               me->meid_str);
        return -EBUSY;
    }

    /* if scs is enabled, both MEs need to be stopped */
    if (me->scsenabled) {
        scsme = hwdbg_get_me_data(hd, SCSBUDDY(meid));
        if (!scsme) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "Failed to find %s SCS data for code BP", me->meid_str);
            return -ENOENT;
        }

        if (scsme->stopped == STOP_STATE_RUNNING) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "Attempted to add code break while SCS %s was running",
                   scsme->meid_str);
            return -EBUSY;
        }
    }

    /* if qcs is enabled, all four MEs need to be stopped */
    if (me->qcsenabled) {
        for (i = 0; i < QCSMENUM; i++) {
            qcsme[i] = hwdbg_get_me_data(hd, me->qcs_me_data[i]->meid);
            if (!qcsme[i]) {
                NS_LOG(NS_LOG_LVL_WARN,
                       "Failed to find %s QCS data for code BP", me->meid_str);
                return -ENOENT;
            }
            if (qcsme[i]->stopped == STOP_STATE_RUNNING) {
                NS_LOG(NS_LOG_LVL_WARN,
                       "Attempted to add code break while QCS %s was running",
                    qcsme[i]->meid_str);
                return -EBUSY;
            }
        }
    }

    if (me->dbginfo) {
        if (pc >= me->dbginfo_len - me->dbginfo_off || pc < me->dbginfo_off) {
            NS_LOG(NS_LOG_LVL_WARN, "code BP PC exceed debug info bounds");
            return -EINVAL;
        }
        if (!(me->dbginfo[pc - me->dbginfo_off] &
                        NFP_HWDBG_DBGINFO_BRKPTALLOWED)) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "debug info indicates "
                   "breakpoint cannot be added at pc %d for %s",
                   pc, me->meid_str);
            return -EINVAL;
        }
    } else
        NS_LOG(NS_LOG_LVL_WARN, "Adding breakpoint without debug info");

    return add_codebp(hd, me, pc);
}

int hwdbg_del_codebp(struct hwdbg_handle *hd, int meid, int pc)
{
    struct hwdbg_me_data *me, *scsme;
    struct hwdbg_me_data *qcsme[QCSMENUM];
    int ret, i = 0;

    for (i = 0; i < QCSMENUM; i++)
        qcsme[i] = NULL;

    /* lock is already held */
    me = hwdbg_get_me_data(hd, meid);
    if (!me) {
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to find ME%x data when deleting code BP",
               meid);
        return -ENOENT;
    }

    /* check if me (and scs me, if needed) is stopped */
    if (me->stopped == STOP_STATE_RUNNING) {
        NS_LOG(NS_LOG_LVL_WARN,
               "Attempted to delete code break while %s was running",
               me->meid_str);
        return -EBUSY;
    }

    /* if scs is enabled, both MEs need to be stopped */
    if (me->scsenabled) {
        scsme = hwdbg_get_me_data(hd, SCSBUDDY(meid));
        if (!scsme) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "Failed to find %s SCS data for code BP", me->meid_str);
            return -ENOENT;
        }
        if (scsme->stopped == STOP_STATE_RUNNING) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "Attempted to delete code break while SCS %s was running",
                   scsme->meid_str);
            return -EBUSY;
        }
    }

    /* if qcs is enabled, all four MEs need to be stopped */
    if (me->qcsenabled) {
        for (i = 0; i < QCSMENUM; i++) {
            qcsme[i] = hwdbg_get_me_data(hd, me->qcs_me_data[i]->meid);
            if (!qcsme[i]) {
                NS_LOG(NS_LOG_LVL_WARN,
                       "Failed to find %s QCS data for code BP", me->meid_str);
                return -ENOENT;
            }
            if (qcsme[i]->stopped == STOP_STATE_RUNNING) {
                NS_LOG(NS_LOG_LVL_WARN,
                       "Attempted to delete code break while QCS %s was running",
                    qcsme[i]->meid_str);
                return -EBUSY;
            }
        }
    }

    ret = restore_codebp(hd, me, pc);
    return ret;
}

/************* API functions ***************/

int hwdbg_brkgrp_add(struct hwdbg_handle *hd, int meid, int *meids, int len)
{
    struct hwdbg_me_data *me;

    if (meid <= 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Invalid MEID %x for break group", meid);
        return -EINVAL;
    }

    if (pthread_mutex_lock(hd->nfp_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire nfp lock");
        return -EIO;
    }

    me = hwdbg_get_me_data(hd, meid);
    if (!me) {
        pthread_mutex_unlock(hd->nfp_mtx);
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to find ME%x data for break group");
        return -ENOENT;
    }

    if (me->brkgrp_meids) {
        free(me->brkgrp_meids);
        NS_LOG(NS_LOG_LVL_ERROR,
               "Overriding break group for %s", me->meid_str);
    }

    me->brkgrp_meids = malloc(len * sizeof(int));
    if (!me->brkgrp_meids) {
        pthread_mutex_unlock(hd->nfp_mtx);
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to allocate break group for %s", me->meid_str);
        return -ENOMEM;
    }

    memcpy(me->brkgrp_meids, meids, sizeof(int) * len);
    me->brkgrp_len = len;

    pthread_mutex_unlock(hd->nfp_mtx);
    return 0;
}

int hwdbg_brkgrp_del(struct hwdbg_handle *hd, int meid)
{
    struct hwdbg_me_data *me;

    if (meid <= 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Invalid MEID %x for break group", meid);
        return -EINVAL;
    }

    if (pthread_mutex_lock(hd->nfp_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire nfp lock");
        return -EIO;
    }

    me = hwdbg_get_me_data(hd, meid);
    if (!me) {
        pthread_mutex_unlock(hd->nfp_mtx);
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to find ME%x data for break group", meid);
        return -ENOENT;
    }

    if (me->brkgrp_meids) {
        free(me->brkgrp_meids);
        me->brkgrp_meids = NULL;
    }

    pthread_mutex_unlock(hd->nfp_mtx);
    return 0;
}

int hwdbg_me_status(struct hwdbg_handle *hd, int meid,
                    struct nfp_hwdbg_me_status_data *status)
{
    struct hwdbg_me_data *me;
    uint32_t val32, ctxens, ctxsts, arbctl;
    int i;

    if (meid <= 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Invalid MEID %x for status", meid);
        return -EINVAL;
    }

    if (pthread_mutex_lock(hd->nfp_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire nfp lock");
        return -EIO;
    }

    me = hwdbg_get_me_data(hd, meid);
    if (!me) {
        status->state = NFP_HWDBG_ME_STATUS_DISABLED;
        pthread_mutex_unlock(hd->nfp_mtx);
        return 0;
    }

    /* Read all the me csrs up front -
     * Note that there are a lot of race conditions here
     * blind faith in the consistency of the results here is fool-hardy
     */
    if (mecsr_read(hd, me, NFP_ME_PROFILE_COUNT, &val32)) {
        pthread_mutex_unlock(hd->nfp_mtx);
        return -EIO;
    }
    status->profile_cnt = val32;

    if (mecsr_read(hd, me, NFP_ME_TIMESTAMP_LOW, &val32)) {
        pthread_mutex_unlock(hd->nfp_mtx);
        return -EIO;
    }
    status->timestamp_low = val32;

    if (mecsr_read(hd, me, NFP_ME_TIMESTAMP_HIGH, &val32)) {
        pthread_mutex_unlock(hd->nfp_mtx);
        return -EIO;
    }
    status->timestamp_high = val32;

    if (mecsr_read(hd, me, NFP_ME_CTX_ENABLES, &ctxens)) {
        pthread_mutex_unlock(hd->nfp_mtx);
        return -EIO;
    }
    status->ctx_enables = NFP_ME_CTX_ENABLES_CONTEXTS_of(ctxens);

    if (mecsr_read(hd, me, NFP_ME_ACTIVE_CTX_STS, &ctxsts)) {
        pthread_mutex_unlock(hd->nfp_mtx);
        return -EIO;
    }

    if (me->stopped == STOP_STATE_ATBREAK) {
        /* me is stopped at a break */
        status->state = NFP_HWDBG_ME_STATUS_ATBREAK;
        status->pc = me->resume_pc;
        status->ctx = me->resume_ctx;
    } else if (me->stopped == STOP_STATE_STOPPED) {
        /* me is stopped */
        status->state = NFP_HWDBG_ME_STATUS_INACTIVE;

        if (mecsr_read(hd, me, NFP_ME_CTX_ARB_CNTL, &arbctl)) {
            pthread_mutex_unlock(hd->nfp_mtx);
            return -EIO;
        }

        /* report next context to run */
        status->ctx = NFP_ME_CTX_ARB_CNTL_NCTX_of(arbctl);

        if (meindcsr_read(hd, me, status->ctx,
                          NFP_ME_INDIRECT_CTX_STS, &ctxsts)) {
            pthread_mutex_unlock(hd->nfp_mtx);
            return -EIO;
        }
        status->pc = NFP_ME_INDIRECT_CTX_STS_CTX_PC_of(ctxsts); // FIX ME -- for 3800 this should be NFP3800_ME_INDIRECT_CTX_STS_CTX_PC_of
    } else {
        /* ME is running - we dont bother checking AB0 here */
        status->state = NFP_HWDBG_ME_STATUS_ACTIVE;
        status->pc = NFP_ME_ACTIVE_CTX_STS_ACTXPC_of(ctxsts);
        status->ctx = NFP_ME_ACTIVE_CTX_STS_ACNO_of(ctxsts);
    }

    for (i = 0; i < 8; i++) {
        if (meindcsr_read(hd, me, i,
                          NFP_ME_INDIRECT_CTX_STS, &ctxsts)) {
            pthread_mutex_unlock(hd->nfp_mtx);
            return -EIO;
        }
        status->ctxpc[i] = NFP_ME_INDIRECT_CTX_STS_CTX_PC_of(ctxsts); // FIX ME -- for 3800 this should be NFP3800_ME_INDIRECT_CTX_STS_CTX_PC_of

        if (status->state == NFP_HWDBG_ME_STATUS_ATBREAK) {
            if (status->ctx >= 0 && status->ctx < 8)
                status->ctxpc[status->ctx] = status->pc;
        }
    }

    pthread_mutex_unlock(hd->nfp_mtx);


    NS_LOG(NS_LOG_LVL_DEBUG,
           "Retrieved status for %s: state %d, actctx %d, actpc %d\n"
           "    ctxpc - 0:%d 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d 7:%d",
           me->meid_str, status->state, status->ctx, status->pc,
           status->ctxpc[0], status->ctxpc[1],
           status->ctxpc[2], status->ctxpc[3],
           status->ctxpc[4], status->ctxpc[5],
           status->ctxpc[6], status->ctxpc[7]);

    return 0;
}

int hwdbg_step_me(struct hwdbg_handle *hd, int meid, int ctx)
{
    struct hwdbg_me_data *me, *scsme;
    struct hwdbg_me_data *qcsme[QCSMENUM];
    int resumescs = 0;
    int resumeqcs[QCSMENUM];
    int ret, i = 0;

    if (meid <= 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Invalid MEID %x for step", meid);
        return -EINVAL;
    }

    if (ctx >= 8) {
        NS_LOG(NS_LOG_LVL_ERROR, "Invalid context %x for step", ctx);
        return -EINVAL;
    }

    if (pthread_mutex_lock(hd->nfp_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire nfp lock");
        return -EIO;
    }

    for (i = 0; i < QCSMENUM; i++) {
        qcsme[i] = NULL;
        resumeqcs[i] = 0;
    }

    me = hwdbg_get_me_data(hd, meid);
    if (!me) {
        pthread_mutex_unlock(hd->nfp_mtx);
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to find ME%x data when stepping context", meid);
        return -ENOENT;
    }

    if (me->stopped == STOP_STATE_RUNNING) {
        pthread_mutex_unlock(hd->nfp_mtx);
        NS_LOG(NS_LOG_LVL_WARN, "Cannot step; %s not stopped", me->meid_str);
        return -EBUSY;
    }

#if 0
    /* use this code if we want to enforce that scsme must be stopped
     * before stepping */
    if (me->scsenabled && !scsme->stopped) {
        pthread_mutex_unlock(hd->nfp_mtx);
        NS_LOG(NS_LOG_LVL_WARN,
               "Cannot step an ME while SCS partner is running");
        return -EBUSY;
    }
#endif

    /* automatically stop scsme if required */
    if (me->scsenabled) {
        int pc, ctx, brcat;

        scsme = hwdbg_get_me_data(hd, SCSBUDDY(meid));
        if (!scsme) {
            pthread_mutex_unlock(hd->nfp_mtx);
            NS_LOG(NS_LOG_LVL_WARN,
                   "Failed to find SCS %s data for step", me->meid_str);
            return -ENOENT;
        }

        if (scsme->stopped == STOP_STATE_RUNNING) {
            if (stop_me(hd, scsme) != 0) {
                pthread_mutex_unlock(hd->nfp_mtx);
                NS_LOG(NS_LOG_LVL_WARN, "Failed to stop SCS ME for step");
                return -EBUSY;
            }
            if (check_swbreak(hd, scsme, &pc, &ctx) == 1) {
                /* we stopped on a bp, so we need to handle it now */
                brcat = categorize_swbreak(hd, scsme, pc, ctx, 0);
                /* prime the break to be resumed */
                prime_swbreak(hd, scsme, pc, ctx, brcat);
            }
            resumescs = 1;
        }
    }

    /* automatically stop qcsme if required */
    if (me->qcsenabled) {
        int pc, ctx, brcat;
        for (i = 0; i < QCSMENUM; i++) {
            qcsme[i] = hwdbg_get_me_data(hd, me->qcs_me_data[i]->meid);
            if (!qcsme[i]) {
                pthread_mutex_unlock(hd->nfp_mtx);
                NS_LOG(NS_LOG_LVL_WARN,
                    "Failed to find QCS %s data for step", me->meid_str);
                return -ENOENT;
            }
            if (qcsme[i]->stopped == STOP_STATE_RUNNING) {
                if (stop_me(hd, qcsme[i]) != 0) {
                    pthread_mutex_unlock(hd->nfp_mtx);
                    NS_LOG(NS_LOG_LVL_WARN, "Failed to stop QCS ME for step");
                    return -EBUSY;
                }
                if (check_swbreak(hd, qcsme[i], &pc, &ctx) == 1) {
                    /* we stopped on a bp, so we need to handle it now */
                    brcat = categorize_swbreak(hd, qcsme[i], pc, ctx, 0);
                    /* prime the break to be resumed */
                    prime_swbreak(hd, qcsme[i], pc, ctx, brcat);
                }
                resumeqcs[i] = 1;
            }
        }
#if 0
        /* first find out which ME is stepped */
        if (meid % 4 == 0) {
            for (i = 0; i < QCSMENUM; i++) {
                qcsme[i] = hwdbg_get_me_data(hd, meid+1+i);
                if (!qcsme[i]) {
                    pthread_mutex_unlock(hd->nfp_mtx);
                    NS_LOG(NS_LOG_LVL_WARN,
                        "Failed to find QCS %s data for step", me->meid_str);
                    return -ENOENT;
                }
                if (qcsme[i]->stopped == STOP_STATE_RUNNING) {
                    if (stop_me(hd, qcsme[i]) != 0) {
                        pthread_mutex_unlock(hd->nfp_mtx);
                        NS_LOG(NS_LOG_LVL_WARN, "Failed to stop QCS ME for step");
                        return -EBUSY;
                    }
                    if (check_swbreak(hd, qcsme[i], &pc, &ctx) == 1) {
                        /* we stopped on a bp, so we need to handle it now */
                        brcat = categorize_swbreak(hd, qcsme[i], pc, ctx, 0);
                        /* prime the break to be resumed */
                        prime_swbreak(hd, qcsme[i], pc, ctx, brcat);
                    }
                    resumeqcs[i] = 1;
                }
            }
        } else if (meid %4 == 1) {
            for (i = 1; i < QCSMENUM; i++) {
                qcsme[i] = hwdbg_get_me_data(hd, meid+i);
                if (!qcsme[i]) {
                    pthread_mutex_unlock(hd->nfp_mtx);
                    NS_LOG(NS_LOG_LVL_WARN,
                        "Failed to find QCS %s data for step", me->meid_str);
                    return -ENOENT;
                }
                if (qcsme[i]->stopped == STOP_STATE_RUNNING) {
                    if (stop_me(hd, qcsme[i]) != 0) {
                        pthread_mutex_unlock(hd->nfp_mtx);
                        NS_LOG(NS_LOG_LVL_WARN, "Failed to stop QCS ME for step");
                        return -EBUSY;
                    }
                    if (check_swbreak(hd, qcsme[i], &pc, &ctx) == 1) {
                        /* we stopped on a bp, so we need to handle it now */
                        brcat = categorize_swbreak(hd, qcsme[i], pc, ctx, 0);
                        /* prime the break to be resumed */
                        prime_swbreak(hd, qcsme[i], pc, ctx, brcat);
                    }
                    resumeqcs[i] = 1;
                }
            }
            qcsme[0] = hwdbg_get_me_data(hd, meid-1);
            if (!qcsme[0]) {
                    pthread_mutex_unlock(hd->nfp_mtx);
                    NS_LOG(NS_LOG_LVL_WARN,
                        "Failed to find QCS %s data for step", me->meid_str);
                    return -ENOENT;
            }
            if (qcsme[0]->stopped == STOP_STATE_RUNNING) {
                if (stop_me(hd, qcsme[0]) != 0) {
                    pthread_mutex_unlock(hd->nfp_mtx);
                    NS_LOG(NS_LOG_LVL_WARN, "Failed to stop QCS ME for step");
                    return -EBUSY;
                }
                if (check_swbreak(hd, qcsme[0], &pc, &ctx) == 1) {
                    /* we stopped on a bp, so we need to handle it now */
                    brcat = categorize_swbreak(hd, qcsme[0], pc, ctx, 0);
                    /* prime the break to be resumed */
                    prime_swbreak(hd, qcsme[0], pc, ctx, brcat);
                }
                resumeqcs[0] = 1;
            }
        } else if (meid % 4 == 2) {
            for (i = 2; i > 0; i--) {
                qcsme[2-i] = hwdbg_get_me_data(hd, meid-i);
                if (!qcsme[2-i]) {
                    pthread_mutex_unlock(hd->nfp_mtx);
                    NS_LOG(NS_LOG_LVL_WARN,
                        "Failed to find QCS %s data for step", me->meid_str);
                    return -ENOENT;
                }
                if (qcsme[2-i]->stopped == STOP_STATE_RUNNING) {
                    if (stop_me(hd, qcsme[2-i]) != 0) {
                        pthread_mutex_unlock(hd->nfp_mtx);
                        NS_LOG(NS_LOG_LVL_WARN, "Failed to stop QCS ME for step");
                        return -EBUSY;
                    }
                    if (check_swbreak(hd, qcsme[2-i], &pc, &ctx) == 1) {
                        /* we stopped on a bp, so we need to handle it now */
                        brcat = categorize_swbreak(hd, qcsme[2-i], pc, ctx, 0);
                        /* prime the break to be resumed */
                        prime_swbreak(hd, qcsme[2-i], pc, ctx, brcat);
                    }
                    resumeqcs[2-i] = 1;
                }
            }
            qcsme[2] = hwdbg_get_me_data(hd, meid+1);
            if (!qcsme[2]) {
                pthread_mutex_unlock(hd->nfp_mtx);
                NS_LOG(NS_LOG_LVL_WARN,
                    "Failed to find QCS %s data for step", me->meid_str);
                return -ENOENT;
            }
            if (qcsme[2]->stopped == STOP_STATE_RUNNING) {
                if (stop_me(hd, qcsme[2]) != 0) {
                    pthread_mutex_unlock(hd->nfp_mtx);
                    NS_LOG(NS_LOG_LVL_WARN, "Failed to stop QCS ME for step");
                    return -EBUSY;
                }
                if (check_swbreak(hd, qcsme[2], &pc, &ctx) == 1) {
                    /* we stopped on a bp, so we need to handle it now */
                    brcat = categorize_swbreak(hd, qcsme[2], pc, ctx, 0);
                    /* prime the break to be resumed */
                    prime_swbreak(hd, qcsme[2], pc, ctx, brcat);
                }
                resumeqcs[2] = 1;
            }
        } else if (meid % 4 == 3) {
            for (i = 3; i > 0; i--) {
                qcsme[3-i] = hwdbg_get_me_data(hd, meid-i);
                if (!qcsme[3-i]) {
                    pthread_mutex_unlock(hd->nfp_mtx);
                    NS_LOG(NS_LOG_LVL_WARN,
                        "Failed to find QCS %s data for step", me->meid_str);
                    return -ENOENT;
                }
            }
            if (qcsme[3-i]->stopped == STOP_STATE_RUNNING) {
                if (stop_me(hd, qcsme[3-i]) != 0) {
                    pthread_mutex_unlock(hd->nfp_mtx);
                    NS_LOG(NS_LOG_LVL_WARN, "Failed to stop QCS ME for step");
                    return -EBUSY;
                }
                if (check_swbreak(hd, qcsme[3-i], &pc, &ctx) == 1) {
                    /* we stopped on a bp, so we need to handle it now */
                    brcat = categorize_swbreak(hd, qcsme[3-i], pc, ctx, 0);
                    /* prime the break to be resumed */
                    prime_swbreak(hd, qcsme[3-i], pc, ctx, brcat);
                }
                resumeqcs[3-i] = 1;
            }
        }
#endif
    }

    if (!me->dbginfo) {
        pthread_mutex_unlock(hd->nfp_mtx);
        NS_LOG(NS_LOG_LVL_ERROR,
               "Cannot step %s without debug info loaded", me->meid_str);
        return -ENODATA;
    }

    ret = step_context(hd, me, ctx);

    if (resumescs) /* resume scs me if it was stopped */
        resume_me(hd, scsme);

    for (i = 0; i < QCSMENUM; i++) {
        if (resumeqcs[i]) /* resume qcs me if it was stopped */
            resume_me(hd, qcsme[i]);
    }

    pthread_mutex_unlock(hd->nfp_mtx);
    return ret;
}

int hwdbg_add_dbginfo(struct hwdbg_handle *hd,
                      int meid, uint64_t *data,
                      int len, int off)
{
    struct hwdbg_me_data *me;
    int i = 0;

    if (off < 0 || off >= CSLEN * 2) {
        NS_LOG(NS_LOG_LVL_ERROR, "Invalid offset for dbginfo");
        return -EINVAL;
    }

    if (len <= 0 || off + len > CSLEN * 2) {
        NS_LOG(NS_LOG_LVL_ERROR, "Invalid length for dbginfo");
        return -EINVAL;
    }

    if (!data) {
        NS_LOG(NS_LOG_LVL_ERROR, "Got NULL data for dbginfo");
        return -EINVAL;
    }

    if (pthread_mutex_lock(hd->nfp_mtx) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to acquire nfp lock");
        return -EIO;
    }

    me = hwdbg_get_me_data(hd, meid);
    if (!me) {
        pthread_mutex_unlock(hd->nfp_mtx);
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to find ME%x data when adding debug info", meid);
        return -ENOENT;
    }

    if (me->scsenabled) {
        int raddr = NFP_MIN(me->routine_addr, me->scs_me_data->routine_addr);

        if ((raddr >= 0) && (off + len) >= raddr) {
            NS_LOG(NS_LOG_LVL_WARN, "Not enough space for debugger to handle "
                "breakpoints on SCS MEs %s and %s.", me->meid_str,
                me->scs_me_data->meid_str);
            me->routine_addr = -1;
            me->scs_me_data->routine_addr = -1;
        }
    } else if (me->qcsenabled) {
        for (i = 0; i < QCSMENUM; i++) {
            int raddr = NFP_MIN(me->routine_addr, me->qcs_me_data[i]->routine_addr);

            if ((raddr >= 0) && (off + len) >= raddr) {
                NS_LOG(NS_LOG_LVL_WARN, "Not enough space for debugger to handle "
                    "breakpoints on QCS MEs %s and %s.", me->meid_str,
                    me->qcs_me_data[i]->meid_str);
                me->routine_addr = -1;
                me->qcs_me_data[i]->routine_addr = -1;
            }
        }
    } else if ((me->routine_addr >= 0) && (off + len) >= me->routine_addr) {
        NS_LOG(NS_LOG_LVL_WARN, "Not enough space for debugger to handle "
            "breakpoints on ME %s.", me->meid_str);
        me->routine_addr = -1;
    }

    NS_LOG(NS_LOG_LVL_DEBUG,
           "loading debug info for %s (len = %d, off = %d)",
           me->meid_str, len, off);

    if (me->dbginfo) {
        NS_LOG(NS_LOG_LVL_INFO,
               "Overriding debug info for %s", me->meid_str);
        free(me->dbginfo);
    }
    me->dbginfo = malloc(sizeof(uint64_t) * len);
    if (me->dbginfo == NULL) {
        pthread_mutex_unlock(hd->nfp_mtx);
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to allocate debug info");
        return -ENOMEM;
    }
    me->dbginfo_len = len;
    me->dbginfo_off = off;
    memcpy(me->dbginfo, data, sizeof(uint64_t) * len);

    pthread_mutex_unlock(hd->nfp_mtx);
    return 0;
}

/************* Init + Free **************/

/* return 1 if debugging should be enabled, 0 if not */
static int me_is_enabled(struct hwdbg_handle *hd, int meid)
{
    char buf[NFP_MEID_STR_SZ];
    int fwid;
    int menum;

    /* Grab the firmware ID of the ME in question */
    if (nfp_nffw_info_acquire(hd->nfp) < 0) {
        nfp6000_meid2str(buf, meid);
        NS_LOG(NS_LOG_LVL_ERROR,
               "Failed to acquire nffw info lock for %s",
               buf);
        return 0;
    }

    fwid = nfp_nffw_info_me_fwid(hd->nfp, meid);

    nfp_nffw_info_release(hd->nfp);

    if (fwid < NFFW_FWID_BASE)
        return 0;

    if (dbgsrv_glob->opt_enabled_mes) {
        /* If list is provided from cmd line
         * only allow those to be debugged
         */
        for (menum = 0; dbgsrv_glob->opt_enabled_mes[menum] >= 0; menum++) {
            if (dbgsrv_glob->opt_enabled_mes[menum] == meid)
                return 1;
        }
        return 0;
    }

    return 1;
}

struct hwdbg_handle *hwdbg_init(void)
{
    struct hwdbg_handle *ret;
    struct hwdbg_me_data *me_data;
    int menum = 0, enabledme = 0, qcsmenum = 0;
    int err = 0, meid = 0;

    ret = malloc(sizeof(*ret));
    if (!ret) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to allocate hwdbg handle");
        return NULL;
    }

    ret->stop_thread = 0;
    ret->nfp = dbgsrv_glob->nfp;
    ret->nfp_mtx = dbgsrv_glob->nfp_mtx;

    ret->me_count = nfp_chipdata_me_count(dbgsrv_glob->chip);
    if (ret->me_count == 0) {
        free(ret);
        NS_LOG(NS_LOG_LVL_ERROR, "Chipdata reports no MEs");
        return NULL;
    }

    ret->me_data = malloc(sizeof(struct hwdbg_me_data) * ret->me_count);
    if (!ret->me_data) {
        free(ret);
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to allocate memory for ME data");
        return NULL;
    }

    for (meid = nfp_chipdata_meid_first(dbgsrv_glob->chip);
                meid >= 0;
                meid = nfp_chipdata_meid_next(dbgsrv_glob->chip, meid), menum++) {
        if (menum == ret->me_count) {
            NS_LOG(NS_LOG_LVL_WARN, "BUG: chipdata reported too many MEs");
            break;
        }

        ret->me_data[menum].meid = meid;
        if (!nfp6000_meid2str(ret->me_data[menum].meid_str, meid))
            snprintf(ret->me_data[menum].meid_str, NFP_MEID_STR_SZ, "ME%x", meid);
        ret->me_data[menum].active_codebp = 0;
        ret->me_data[menum].routine_primed = 0;
        ret->me_data[menum].base_cs = NULL;
        ret->me_data[menum].brkgrp_meids = NULL;
        ret->me_data[menum].scs_me_data = NULL;
        for (qcsmenum = 0; qcsmenum < QCSMENUM; qcsmenum++)
            ret->me_data[menum].qcs_me_data[qcsmenum] = NULL;
        ret->me_data[menum].dbginfo = NULL;
        ret->me_data[menum].stopped = STOP_STATE_STOPPED;
        /* just make sure the ME is stopped */

        if (me_is_enabled(ret, meid))
            ret->me_data[menum].disabled = 0;
        else
            ret->me_data[menum].disabled = 1;

        if (!ret->me_data[menum].disabled) {
            if (dbgsrv_glob->opt_stopmes) {
                if (stop_me(ret, &ret->me_data[menum]) < 0) {
                    err = 1;
                    break;
                }
            } else {
                if (load_me_status(ret, &ret->me_data[menum]) < 0) {
                    err = 1;
                    break;
                }
            }
            ret->me_data[menum].scsenabled = is_scs_enabled(ret, &ret->me_data[menum]);
            ret->me_data[menum].qcsenabled = is_qcs_enabled(ret, &ret->me_data[menum]);
            ret->me_data[menum].nopcb = 0;

            if (ret->me_data[menum].scsenabled) {
                // even ME on the first block, odd on the second
                ret->me_data[menum].routine_addr = CSLEN * 2;
                if (ret->me_data[menum].meid & 1)
                    ret->me_data[menum].routine_addr -= ROUTINE_INSTR_CNT;
                else
                    ret->me_data[menum].routine_addr -= (2 * ROUTINE_INSTR_CNT);

            } else if (ret->me_data[menum].qcsenabled) {
                /* First ME on the first block, second ME on the second block,
                 * third ME on the third block, and fourth ME on the fourth block
                 */
                if (ret->me_data[menum].meid % 4 == 0)
                    ret->me_data[menum].routine_addr = CSLEN * 4;
                else if (ret->me_data[menum].meid % 4 == 1)
                    ret->me_data[menum].routine_addr = (CSLEN * 4) - ROUTINE_INSTR_CNT;
                else if (ret->me_data[menum].meid % 4 == 2)
                    ret->me_data[menum].routine_addr = (CSLEN * 4) - (2 * ROUTINE_INSTR_CNT);
                else if (ret->me_data[menum].meid % 4 == 3)
                    ret->me_data[menum].routine_addr = (CSLEN * 4) - (3 * ROUTINE_INSTR_CNT);

            } else {
                ret->me_data[menum].routine_addr = CSLEN - ROUTINE_INSTR_CNT;
            }

            /* TODO: FIXME: check for a0 here */
            if ( (ret->me_data[menum].scsenabled) || (ret->me_data[menum].qcsenabled) )
                ret->me_data[menum].nopcb = 1;
            enabledme++;
        }
    }

    if (err) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to create set up MEs");
        free(ret->me_data);
        free(ret);
        return NULL;
    }

    /* store the SCS buddy and do a sanity check for SCS mode on both */
    for (menum = 0; menum < ret->me_count; menum++) {
        if (ret->me_data[menum].disabled)
            continue;
        if (!ret->me_data[menum].scsenabled)
            continue;

        me_data = hwdbg_get_me_data(ret, SCSBUDDY(ret->me_data[menum].meid));
        if (!me_data) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "could not find SCS pair for %s",
                   ret->me_data[menum].meid_str);
        } else {
            ret->me_data[menum].scs_me_data = me_data;
            if (ret->me_data[menum].scsenabled != me_data->scsenabled) {
                NS_LOG(NS_LOG_LVL_WARN,
                       "detected configuration problem: "
                       "%s and %s do not have the same scs configuration",
                       ret->me_data[menum].meid_str,
                       me_data->meid_str);
            }
        }
    }

    /* store the QCS and do a sanity check for QCS mode on all four */
    for (menum = 0; menum < ret->me_count; menum++) {
        if (ret->me_data[menum].disabled)
            continue;
        if (!ret->me_data[menum].qcsenabled)
            continue;

        if (ret->me_data[menum].meid % 4 == 0) {
            for (qcsmenum = 1; qcsmenum <= QCSMENUM; qcsmenum++) {
                me_data = hwdbg_get_me_data(ret, ret->me_data[menum+qcsmenum].meid);
                if (!me_data) {
                    NS_LOG(NS_LOG_LVL_WARN,
                           "could not find QCS pair for %s",
                           ret->me_data[menum].meid_str);
                } else {
                    ret->me_data[menum].qcs_me_data[qcsmenum-1] = me_data;
                    if (ret->me_data[menum].qcsenabled != me_data->qcsenabled) {
                        NS_LOG(NS_LOG_LVL_WARN,
                               "detected configuration problem: "
                               "%s and %s do not have the same qcs configuration",
                               ret->me_data[menum].meid_str,
                               me_data->meid_str);
                    }
                }
            }
        } else if (ret->me_data[menum].meid % 4 == 1) {
            for (qcsmenum = 1; qcsmenum < QCSMENUM; qcsmenum++) {
                me_data = hwdbg_get_me_data(ret, ret->me_data[menum+qcsmenum].meid);
                if (!me_data) {
                    NS_LOG(NS_LOG_LVL_WARN,
                           "could not find QCS pair for %s",
                           ret->me_data[menum].meid_str);
                } else {
                    ret->me_data[menum].qcs_me_data[qcsmenum] = me_data;
                    if (ret->me_data[menum].qcsenabled != me_data->qcsenabled) {
                        NS_LOG(NS_LOG_LVL_WARN,
                               "detected configuration problem: "
                               "%s and %s do not have the same qcs configuration",
                               ret->me_data[menum].meid_str,
                               me_data->meid_str);
                    }
                }
            }
            me_data = hwdbg_get_me_data(ret, ret->me_data[menum-(qcsmenum-2)].meid);
            if (!me_data) {
                NS_LOG(NS_LOG_LVL_WARN,
                       "could not find QCS pair for %s",
                       ret->me_data[menum].meid_str);
            } else {
                ret->me_data[menum].qcs_me_data[qcsmenum-3] = me_data;
                if (ret->me_data[menum].qcsenabled != me_data->qcsenabled) {
                    NS_LOG(NS_LOG_LVL_WARN,
                           "detected configuration problem: "
                           "%s and %s do not have the same qcs configuration",
                           ret->me_data[menum].meid_str,
                           me_data->meid_str);
                }
            }
        } else if (ret->me_data[menum].meid % 4 == 2) {
            for (qcsmenum = 2; qcsmenum > 0; qcsmenum--) {
                me_data = hwdbg_get_me_data(ret, ret->me_data[menum-qcsmenum].meid);
                if (!me_data) {
                    NS_LOG(NS_LOG_LVL_WARN,
                           "could not find QCS pair for %s",
                           ret->me_data[menum].meid_str);
                } else {
                    ret->me_data[menum].qcs_me_data[2-qcsmenum] = me_data;
                    if (ret->me_data[menum].qcsenabled != me_data->qcsenabled) {
                        NS_LOG(NS_LOG_LVL_WARN,
                               "detected configuration problem: "
                               "%s and %s do not have the same qcs configuration",
                               ret->me_data[menum].meid_str,
                               me_data->meid_str);
                    }
                }
            }
            me_data = hwdbg_get_me_data(ret, ret->me_data[menum+(qcsmenum+1)].meid);
            if (!me_data) {
                NS_LOG(NS_LOG_LVL_WARN,
                       "could not find QCS pair for %s",
                       ret->me_data[menum].meid_str);
            } else {
                ret->me_data[menum].qcs_me_data[qcsmenum+2] = me_data;
                if (ret->me_data[menum].qcsenabled != me_data->qcsenabled) {
                    NS_LOG(NS_LOG_LVL_WARN,
                           "detected configuration problem: "
                           "%s and %s do not have the same qcs configuration",
                           ret->me_data[menum].meid_str,
                           me_data->meid_str);
                }
            }
        } else if (ret->me_data[menum].meid % 4 == 3) {
            for (qcsmenum = 3; qcsmenum > 0; qcsmenum--) {
                me_data = hwdbg_get_me_data(ret, ret->me_data[menum-qcsmenum].meid);
                if (!me_data) {
                    NS_LOG(NS_LOG_LVL_WARN,
                           "could not find QCS pair for %s",
                           ret->me_data[menum].meid_str);
                } else {
                    ret->me_data[menum].qcs_me_data[3-qcsmenum] = me_data;
                    if (ret->me_data[menum].qcsenabled != me_data->qcsenabled) {
                        NS_LOG(NS_LOG_LVL_WARN,
                               "detected configuration problem: "
                               "%s and %s do not have the same qcs configuration",
                               ret->me_data[menum].meid_str,
                               me_data->meid_str);
                    }
                }
            }
        }
    }

    if (enabledme == 0)
        NS_LOG(NS_LOG_LVL_INFO, "No loaded MicroEngines to debug");
    else
        NS_LOG(NS_LOG_LVL_INFO, "Initialized %d ME%s", enabledme, enabledme == 1 ? "" : "s");

    if (pthread_create(&ret->pthread, NULL, &swbrk_work, ret) < 0) {
        NS_LOG(NS_LOG_LVL_ERROR, "Failed to create SW break thread");
        free(ret->me_data);
        free(ret);
        return NULL;
    }

    return ret;
}

void hwdbg_reset(struct hwdbg_handle *hd)
{
    struct hwdbg_me_data *me_data;
    int menum;
    int enabledme = 0, qcsmenum = 0;
    /* already locked */

    for (menum = 0; menum < hd->me_count; menum++) {
        if (hd->me_data[menum].base_cs)
            free(hd->me_data[menum].base_cs);
        hd->me_data[menum].base_cs = NULL;
        if (hd->me_data[menum].brkgrp_meids)
            free(hd->me_data[menum].brkgrp_meids);
        hd->me_data[menum].brkgrp_meids = NULL;
        if (hd->me_data[menum].base_cs)
            free(hd->me_data[menum].dbginfo);
        hd->me_data[menum].dbginfo = NULL;
        if (me_is_enabled(hd, hd->me_data[menum].meid))
            hd->me_data[menum].disabled = 0;
        else
            hd->me_data[menum].disabled = 1;

        if (!hd->me_data[menum].disabled)
            enabledme++;

        hd->me_data[menum].active_codebp = 0;
        hd->me_data[menum].routine_primed = 0;
        if (!hd->me_data[menum].disabled) {
            /* just make sure the ME is stopped */
            if (dbgsrv_glob->opt_stopmes)
                stop_me(hd, &hd->me_data[menum]);
            else
                load_me_status(hd, &hd->me_data[menum]);
            hd->me_data[menum].scsenabled = is_scs_enabled(hd, &hd->me_data[menum]);
            hd->me_data[menum].qcsenabled = is_qcs_enabled(hd, &hd->me_data[menum]);

            hd->me_data[menum].nopcb = 0;

            if (hd->me_data[menum].scsenabled) {
                // even ME on the first block, odd on the second
                hd->me_data[menum].routine_addr = CSLEN * 2;
                if (hd->me_data[menum].meid & 1)
                    hd->me_data[menum].routine_addr -= ROUTINE_INSTR_CNT;
                else
                    hd->me_data[menum].routine_addr -= (2 * ROUTINE_INSTR_CNT);

            } else if (hd->me_data[menum].qcsenabled) {
                /* First ME on the first block, second ME on the second block,
                 * third ME on the third block, and fourth ME on the fourth block
                 */
                if (hd->me_data[menum].meid % 4 == 0)
                    hd->me_data[menum].routine_addr = CSLEN * 4;
                else if (hd->me_data[menum].meid % 4 == 1)
                    hd->me_data[menum].routine_addr = (CSLEN * 4) - ROUTINE_INSTR_CNT;
                else if (hd->me_data[menum].meid % 4 == 2)
                    hd->me_data[menum].routine_addr = (CSLEN * 4) - (2 * ROUTINE_INSTR_CNT);
                else if (hd->me_data[menum].meid % 4 == 3)
                    hd->me_data[menum].routine_addr = (CSLEN * 4) - (3 * ROUTINE_INSTR_CNT);

            } else {
                hd->me_data[menum].routine_addr = CSLEN - ROUTINE_INSTR_CNT;
            }

            /* TODO: FIXME: check for a0 here */
            if (hd->me_data[menum].scsenabled)
                hd->me_data[menum].nopcb = 1;
        }
    }
    for (menum = 0; menum < hd->me_count; menum++) {
        if (hd->me_data[menum].disabled)
            continue;
        if (!hd->me_data[menum].scsenabled)
            continue;

        me_data = hwdbg_get_me_data(hd, SCSBUDDY(hd->me_data[menum].meid));
        if (!me_data) {
            NS_LOG(NS_LOG_LVL_WARN,
                   "could not find SCS pair for %s",
                   hd->me_data[menum].meid_str);
        } else {
            hd->me_data[menum].scs_me_data = me_data;
            if (hd->me_data[menum].scsenabled !=
                      hd->me_data[menum].scs_me_data->scsenabled) {
                NS_LOG(NS_LOG_LVL_WARN,
                       "detected configuration problem: "
                       "me%x and me%x do not have the same scs configuration",
                       hd->me_data[menum].meid,
                       hd->me_data[menum].scs_me_data->meid);
            }
        }
    }

    /* store the QCS and do a sanity check for QCS mode on all four */
    for (menum = 0; menum < hd->me_count; menum++) {
        if (hd->me_data[menum].disabled)
            continue;
        if (!hd->me_data[menum].qcsenabled)
            continue;

        if (hd->me_data[menum].meid % 4 == 0) {
            for (qcsmenum = 1; qcsmenum <= QCSMENUM; qcsmenum++) {
                me_data = hwdbg_get_me_data(hd, hd->me_data[menum+qcsmenum].meid);
                if (!me_data) {
                    NS_LOG(NS_LOG_LVL_WARN,
                           "could not find QCS pair for %s",
                           hd->me_data[menum].meid_str);
                } else {
                    hd->me_data[menum].qcs_me_data[qcsmenum-1] = me_data;
                    if (hd->me_data[menum].qcsenabled != me_data->qcsenabled) {
                        NS_LOG(NS_LOG_LVL_WARN,
                               "detected configuration problem: "
                               "%s and %s do not have the same qcs configuration",
                               hd->me_data[menum].meid_str,
                               me_data->meid_str);
                    }
                }
            }
        } else if (hd->me_data[menum].meid % 4 == 1) {
            for (qcsmenum = 1; qcsmenum < QCSMENUM; qcsmenum++) {
                me_data = hwdbg_get_me_data(hd, hd->me_data[menum+qcsmenum].meid);
                if (!me_data) {
                    NS_LOG(NS_LOG_LVL_WARN,
                           "could not find QCS pair for %s",
                           hd->me_data[menum].meid_str);
                } else {
                    hd->me_data[menum].qcs_me_data[qcsmenum] = me_data;
                    if (hd->me_data[menum].qcsenabled != me_data->qcsenabled) {
                        NS_LOG(NS_LOG_LVL_WARN,
                               "detected configuration problem: "
                               "%s and %s do not have the same qcs configuration",
                               hd->me_data[menum].meid_str,
                               me_data->meid_str);
                    }
                }
            }
            me_data = hwdbg_get_me_data(hd, hd->me_data[menum-(qcsmenum-2)].meid);
            if (!me_data) {
                NS_LOG(NS_LOG_LVL_WARN,
                       "could not find QCS pair for %s",
                       hd->me_data[menum].meid_str);
            } else {
                hd->me_data[menum].qcs_me_data[qcsmenum-3] = me_data;
                if (hd->me_data[menum].qcsenabled != me_data->qcsenabled) {
                    NS_LOG(NS_LOG_LVL_WARN,
                           "detected configuration problem: "
                           "%s and %s do not have the same qcs configuration",
                           hd->me_data[menum].meid_str,
                           me_data->meid_str);
                }
            }
        } else if (hd->me_data[menum].meid % 4 == 2) {
            for (qcsmenum = 2; qcsmenum > 0; qcsmenum--) {
                me_data = hwdbg_get_me_data(hd, hd->me_data[menum-qcsmenum].meid);
                if (!me_data) {
                    NS_LOG(NS_LOG_LVL_WARN,
                           "could not find QCS pair for %s",
                           hd->me_data[menum].meid_str);
                } else {
                    hd->me_data[menum].qcs_me_data[2-qcsmenum] = me_data;
                    if (hd->me_data[menum].qcsenabled != me_data->qcsenabled) {
                        NS_LOG(NS_LOG_LVL_WARN,
                               "detected configuration problem: "
                               "%s and %s do not have the same qcs configuration",
                               hd->me_data[menum].meid_str,
                               me_data->meid_str);
                    }
                }
            }
            me_data = hwdbg_get_me_data(hd, hd->me_data[menum+(qcsmenum+1)].meid);
            if (!me_data) {
                NS_LOG(NS_LOG_LVL_WARN,
                       "could not find QCS pair for %s",
                       hd->me_data[menum].meid_str);
            } else {
                hd->me_data[menum].qcs_me_data[qcsmenum+2] = me_data;
                if (hd->me_data[menum].qcsenabled != me_data->qcsenabled) {
                    NS_LOG(NS_LOG_LVL_WARN,
                           "detected configuration problem: "
                           "%s and %s do not have the same qcs configuration",
                           hd->me_data[menum].meid_str,
                           me_data->meid_str);
                }
            }
        } else if (hd->me_data[menum].meid % 4 == 3) {
            for (qcsmenum = 3; qcsmenum > 0; qcsmenum--) {
                me_data = hwdbg_get_me_data(hd, hd->me_data[menum-qcsmenum].meid);
                if (!me_data) {
                    NS_LOG(NS_LOG_LVL_WARN,
                           "could not find QCS pair for %s",
                           hd->me_data[menum].meid_str);
                } else {
                    hd->me_data[menum].qcs_me_data[3-qcsmenum] = me_data;
                    if (hd->me_data[menum].qcsenabled != me_data->qcsenabled) {
                        NS_LOG(NS_LOG_LVL_WARN,
                               "detected configuration problem: "
                               "%s and %s do not have the same qcs configuration",
                               hd->me_data[menum].meid_str,
                               me_data->meid_str);
                    }
                }
            }
        }
    }

    if (enabledme == 0)
        NS_LOG(NS_LOG_LVL_INFO, "No loaded MicroEngines to debug");
    else
        NS_LOG(NS_LOG_LVL_INFO, "Initialized %d ME%s", enabledme, enabledme == 1 ? "" : "s");
}

void hwdbg_free(struct hwdbg_handle *hd)
{
    int menum;
    NS_LOG(NS_LOG_LVL_INFO, "Waiting for SW break thread to stop");
    hd->stop_thread = 1;
    pthread_join(hd->pthread, NULL);

    for (menum = 0; menum < hd->me_count; menum++) {
        if (hd->me_data[menum].base_cs)
            free(hd->me_data[menum].base_cs);
        if (hd->me_data[menum].dbginfo)
            free(hd->me_data[menum].dbginfo);
    }

    free(hd->me_data);
    free(hd);
}
