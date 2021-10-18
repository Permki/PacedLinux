/*
 * Copyright 2017 Netronome, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>

#include <nfp.h>

#include <nfp/me.h>
#include <nfp/mem_bulk.h>

#include <mac_time.h>

volatile __export __imem struct mac_time_state mac_time;

__intrinsic
unsigned long long multiply_32x32(
    unsigned int x,
    unsigned int y
)
{
    unsigned int lo, hi;
    {
        __asm
        {
            mul_step[x, y], 32x32_start
            mul_step[x, y], 32x32_step1
            mul_step[x, y], 32x32_step2
            mul_step[x, y], 32x32_step3
            mul_step[x, y], 32x32_step4
            mul_step[lo, --], 32x32_last
            mul_step[hi, --], 32x32_last2
        }
    }
    return ((unsigned long long)hi << 32) | lo;
}

/* retrieve the current mac time */
extern __forceinline void
mac_time_fetch(__xread struct mac_time_state *mac_time_xfer)
{
    mem_read64(mac_time_xfer, (__mem __addr40 void *)&mac_time, sizeof(*mac_time_xfer));
}

/* calculate a 64-bit time based on current me time and last synced mac time */
struct mac_time_data mac_time_calc(struct mac_time_state mac_time_buf)
{
    struct mac_time_data ret;
    uint32_t me_time = me_tsc_read();
    uint64_t conv_result;
    uint32_t delta_ns;

    /* NOTE: we do things so none of these calculations will overflow! */

    /* get the me time difference since the last mac sync */
    me_time -= mac_time_buf.me_time;
    /* convert to nsec */
    conv_result = multiply_32x32(me_time, mac_time_buf.conv_mult);
    conv_result >>= mac_time_buf.conv_rshift;
    delta_ns = conv_result;

    if (delta_ns + mac_time_buf.mac_time_ns > 1000 * 1000 * 1000) {
        ret.nsec = mac_time_buf.mac_time_ns + delta_ns - 1000 * 1000 * 1000;
        ret.sec = mac_time_buf.mac_time_s + 1;
    } else {
        ret.nsec = mac_time_buf.mac_time_ns + delta_ns;
        ret.sec = mac_time_buf.mac_time_s;
    }
    return ret;
}

#define WRAP_THRESH (100 * 1000 * 1000)

struct mac_time_data
mac_time_calc_pkt(struct mac_time_state mac_time_buf, uint32_t pkt_time_ns)
{
    struct mac_time_data ret;
    int32_t time_diff;
    time_diff = ((int32_t)pkt_time_ns) - ((int32_t)mac_time_buf.mac_time_ns);

    if (time_diff > 0) {
        if (time_diff < WRAP_THRESH) {
            // normal case
            ret.sec = mac_time_buf.mac_time_s;
            ret.nsec = pkt_time_ns;
        } else {
            // weird case, synced mac time is ahead
            // of pkt time, but the nsec of the sync time
            // wrapped
            ret.sec = mac_time_buf.mac_time_s - 1;
            ret.nsec = pkt_time_ns;
        }
    } else {
        if ((-time_diff) > WRAP_THRESH) {
            // normal wrap case, synced mac time is behind
            // and the pkt time has wrapped ns
            ret.sec = mac_time_buf.mac_time_s + 1;
            ret.nsec = pkt_time_ns;
        } else {
            // weird case, synced mac time is ahead
            // of pkt time but nothing wrapped
            ret.sec = mac_time_buf.mac_time_s;
            ret.nsec = pkt_time_ns;
        }
    }

    return ret;
}
