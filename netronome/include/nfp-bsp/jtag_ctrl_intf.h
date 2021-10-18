/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file jtag_ctrl_intf.h
 *
 */


#ifndef __JTAG_CONTROLLER_INTERFACE_H__
#define __JTAG_CONTROLLER_INTERFACE_H__
/******************************************************************************/

struct jtag_ctrl_intf {

    /*
     *   tms: bits to clock out
     *   tdo: bit0 will contain din(dout of chain) after tms clocking
     *   tdi: value to present on dout(din of chain) while clocking tms
     *   cnt: bit-count(== clock-cycles) to clock
     */
    int (*clock_tms)(struct jtag_ctrl_intf *ji, u32 *tms,
                u8 *tdo, u32 tdi, u32 cnt);

    /*
     *   tms: value to present on tms at final data bit clock
     *   tdi: bits to clock out to chain
     *   tdo: bits clocked in from chain
     *   cnt: bit-count(== clock-cycles) to clock
     */
    int (*clock_din_dout)(struct jtag_ctrl_intf *ji, u32 tms,
                u8 *tdi, u8 *tdo, u32 cnt);
};

/******************************************************************************/
#endif // __JTAG_CONTROLLER_INTERFACE_H__
