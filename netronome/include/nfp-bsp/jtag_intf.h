/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file jtag_intf.h
 *
 */


#ifndef __JTAG_INTF_H__
#define __JTAG_INTF_H__
/******************************************************************************/

//#define JTAG_DEBUG

struct jtag_tap_info {
    char *desc;
    u32 chain_index;
    u32 idcode;
    u32 ir_sz;
    u32 bs_sz;
#ifdef JTAG_DEBUG
    char * (*ir_desc)(u8 *, u32 sz);
#endif
};

struct jtag_intf {

    int (*probe)          (struct jtag_intf *,
                                int (*probe_cb)(void *, struct jtag_tap_info *),
                                void *probe_ctx);
    int (*set_current_tap)(struct jtag_intf *, u32 tap_id);
    int (*txrx)           (struct jtag_intf *, u8 *ir, u32 ir_sz,
                                           u8 *tdi, u8 *tdo, u32 bit_cnt);
    struct jtag_tap_info * (*tap_info)(struct jtag_intf *, u32 tap);
};


/******************************************************************************/
#endif // __JTAG_INTF_H__

