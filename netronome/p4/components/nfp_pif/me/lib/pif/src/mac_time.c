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

#define MAC_NSEC_CSR 0x0840001c // address of nbi0 mac time
/* very rough time taken to fetch mac time
 * worked out from sim from i32.me0, note that this will
 * be higher under load so not perfect, but something at least
 */
#define MAC_FETCH_TIME_NS 55

/* these are used to convert the ME time delta to nsec
 * included are the defaults for me clock of 1000Mhz
 * these should be populated by host based on nfp
 * me clock speed
 */
volatile __export __addr40 __ctm uint32_t mac_time_mult = 16;
volatile __export __addr40 __ctm uint32_t mac_time_rshift = 0;

volatile __export __imem struct mac_time_state mac_time;

__intrinsic void
xpb_read_multiple(__xread unsigned int *xfer, unsigned int addr, int count)
{
    SIGNAL sig;

    __asm ct[xpb_read, *xfer, addr, 0, count/4], ctx_swap[sig]
}

void mac_time_update(void)
{
    __xread unsigned int mactime_xfer[2];
    __xread unsigned int mactime_xfer_again[2];
    __xwrite struct mac_time_state time_reg_xfer;
    __gpr struct mac_time_state time_reg;
    int sub_one_sec;
    uint32_t me_time;

    me_time = me_tsc_read();
    xpb_read_multiple(mactime_xfer,
                      MAC_NSEC_CSR,
                      sizeof(mactime_xfer));

    xpb_read_multiple(mactime_xfer_again,
                      MAC_NSEC_CSR,
                      sizeof(mactime_xfer));

    /* consider this case (if the nsec + sec read is not atomic)
     * nsec       | sec    | descrip
     * ---------------------------------
     * 999999999  |  2     | first read
     * 22         |  2     | second read
     *
     * oops something bad happened
     * in the first read the seconds ticked in the time between the nsec and
     * seconds read; we subtract one from the secs from the original read
     * to get the correct time
     */

    sub_one_sec = 0;
    /* check for inconsistant time - XXX not sure if needed */
    if (mactime_xfer[1] == mactime_xfer_again[1]) {
        if (mactime_xfer[0] > mactime_xfer_again[0]) {
            /* oops, seconds wrapped but the nsec didn't
             * need to subtract one from the orig seconds
             */
             sub_one_sec = 1;
        }
    }

    time_reg.me_time = me_time;
    time_reg.mac_time_ns = mactime_xfer[0];
    time_reg.mac_time_s = mactime_xfer[1];
    time_reg.conv_mult = mac_time_mult;
    time_reg.conv_rshift = mac_time_rshift;

    if (sub_one_sec)
        time_reg.mac_time_s -= 1;

    /* now subtract the time difference between obtaining me_time
     * and the value retrieved from the mac
     */
    if (time_reg.mac_time_ns >= MAC_FETCH_TIME_NS) {
        time_reg.mac_time_ns -= MAC_FETCH_TIME_NS;
    } else {
        time_reg.mac_time_ns = 1 * 1000 * 1000 * 1000 -
                           (MAC_FETCH_TIME_NS - time_reg.mac_time_ns);
        time_reg.mac_time_s -= 1;
    }
    time_reg_xfer = time_reg;

    mem_write64(&time_reg_xfer, (__mem __addr40 void *)&mac_time, sizeof(time_reg_xfer));
}
