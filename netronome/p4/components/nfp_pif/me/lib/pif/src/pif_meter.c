#include <stdint.h>
#include <nfp.h>
#include <nfp/mem_atomic.h>
#include <nfp/me.h>

#include "pif_common.h"

/*
 * Defines
 */

#define RFC_2698 0
#define RFC_2697 1

/*
 * Meter operation - it would be nice to move this into flowenv
 */

__intrinsic void pif_mem_meter(__xrw uint32_t *mval, __emem void *addr, int colour, int rfc)
{
    SIGNAL_PAIR meter_sig;
    uint32_t high_addr, low_addr;

    /* no checks on colour and rfc */
    high_addr = (((uint64_t) addr) >> 32) << 24;
    low_addr = ((uint64_t) addr) & 0xffffffff;

#define RFC_OFF 0
#define COLOUR_OFF 1
    low_addr |= (rfc << RFC_OFF) | (colour << COLOUR_OFF);

    __asm {
        mem[meter, *mval, high_addr, << 8, low_addr], sig_done[meter_sig];
    }
    __wait_for_all(&meter_sig);
}

/* this is a fairly wacky function which does a 32bit multiple
 * but sets the result to 0 on overflow, v0 and v1 are expected
 * to be non-zero
 */
__intrinsic uint32_t mult32_oflow0(uint32_t v0, uint32_t v1)
{
    uint32_t result;
    uint32_t result_hi;

    __asm {
        mul_step[v0, v1], 32x32_start
        mul_step[v0, v1], 32x32_step1
        mul_step[v0, v1], 32x32_step2
        mul_step[v0, v1], 32x32_step3
        mul_step[v0, v1], 32x32_step4
        mul_step[result,--], 32x32_last
        mul_step[result_hi, --], 32x32_last2
    }

    if (result_hi > 0)
        return 0;

    return result;
}

/* update meter timers */
void update_meter_timers(__mem __addr40 struct pif_meter_data *meter)
{
    uint64_t ctime = me_tsc_read();
    uint32_t ctime_upper, ptime_upper, ctime_lower, ptime_lower, tdelta;
    uint8_t shift;
    __xread struct pif_meter_data mdata_rd;
    __xwrite struct pif_meter_data_wr mdata_wr;

    mem_read_atomic(&mdata_rd, (__mem __addr40 void *)meter, sizeof(mdata_rd));

    shift = PIF_METER_SHIFT_of(mdata_rd.config);

    /* apply the time resolution shift */

    ctime >>= shift;

    ctime_upper = (ctime >> 32) & 0xffffff;
    ctime_lower = ctime & 0xffffffff;

    if (shift != PIF_METER_CFGSHIFT_of(mdata_rd.last_update_high_shft)) {
        /* make sure meter is restarted after a config change */
        ptime_upper = 0;
        ptime_lower = 0;
    } else {
        ptime_upper = PIF_METER_UPTIME_of(mdata_rd.last_update_high_shft);
        ptime_lower = mdata_rd.last_update_low;
    }

    if (ctime_upper < ptime_upper)
        goto no_change; /* jump back in time */

    if (ctime_upper == ptime_upper && ctime_lower <= ptime_lower)
        goto no_change; /* jump back in time, or no time passed */

    if (ctime_upper - ptime_upper > 1) {
        /* the upper bit changed multiple times:
         * so reset the buckets and the timer
         */
        mdata_wr.bucket_P = mdata_rd.PBS;
        mdata_wr.bucket_C = mdata_rd.PBS;
        mdata_wr.last_update_high_shft = PIF_METER_UPWORD(ctime_upper, shift);
        mdata_wr.last_update_low = ctime >> 32;

        mem_write_atomic(&mdata_wr, (__mem __addr40 void *)meter, sizeof(mdata_wr));
        return;
    }

    /* check if we wrapped */
    if (ctime_lower <= ptime_lower)
        tdelta = ptime_lower - ctime_lower;
    else
        tdelta = ctime_lower - ptime_lower;

    {
        uint32_t Pdelta, Cdelta;
        uint32_t newP, newC;

        Pdelta = mult32_oflow0(tdelta, mdata_rd.PIR);
        if (Pdelta == 0) { /* mult overflowed */
            mdata_wr.bucket_P = mdata_rd.PBS;
        } else {
            newP = mdata_rd.bucket_P + Pdelta;
            /* either add oveflow or exceed max bkt size */
            if (newP < Pdelta || newP > mdata_rd.PBS)
                newP = mdata_rd.PBS;
            mdata_wr.bucket_P = newP;
        }

        /* NOTE: we only support a single rate config
         *       so P values copied into C bucket
         */
        Cdelta = mult32_oflow0(tdelta, mdata_rd.PIR);
        if (Cdelta == 0) { /* mult overflowed */
            mdata_wr.bucket_C = mdata_rd.PBS;
        } else {
            newC = mdata_rd.bucket_C + Cdelta;
            /* either add oveflow or exceed max bkt size */
            if (newC < Cdelta || newC > mdata_rd.PBS)
                newC = mdata_rd.PBS;
            mdata_wr.bucket_C = newC;
        }

        mdata_wr.last_update_high_shft = PIF_METER_UPWORD(ctime_upper, shift);
        mdata_wr.last_update_low = ctime_lower;

        mem_write_atomic(&mdata_wr, (__mem __addr40 void *)meter, sizeof(mdata_wr));
    }

    return;
no_change:
    {
        __xwrite uint32_t val;
        __mem __addr40 uint8_t *tmr_addr;
        /* TODO: could we use a bit clear immed? */

        val = PIF_METER_UPWORD(ptime_upper, shift);

        tmr_addr = ((__mem __addr40 uint8_t *)meter) +
                   offsetof(struct pif_meter_data, last_update_high_shft);
        mem_write_atomic(&val,
                         (__mem __addr40 void *)tmr_addr,
                         sizeof(uint32_t));
    }
}

/*
 * meter entry point
 */

int pif_meter_execute(__mem __addr40 struct pif_meter_data *meter,
                      int colour_in,
                      int tick)
{
    __xrw uint32_t xval32;
    __mem __addr40 uint8_t *offset;

    /* try to get exclusive access for doing PIR/CIR updates */

    /* do a test + set on the msb of timestamp */
    xval32 = 1 << 31;

    offset = ((__mem __addr40 uint8_t *)meter) +
             offsetof(struct pif_meter_data, last_update_high_shft);

    mem_test_set(&xval32,
                 (__mem __addr40 void *)offset,
                 sizeof(xval32));

    /* check the original test value */
    if ((xval32 >> 31) == 0) {
        /* update meter timers */
        update_meter_timers(meter);
    }

    xval32 = tick;

    offset = ((__mem __addr40 uint8_t *)meter) +
             offsetof(struct pif_meter_data, bucket_P);
    /* TODO: should RFC be configurable per meter? */
    pif_mem_meter(&xval32, (__mem __addr40 void *)offset, colour_in, RFC_2698);

    return xval32; /* return colour result of mem_meter */
}
