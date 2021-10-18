/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file mpsse_jtag.h
 *
 */


#ifndef _MPSSE_JTAG_H_
#define _MPSSE_JTAG_H_

/******************************************************************************/
#include "mpsse.h"

struct jtag_ctrl_intf* nfp6_svp_jtag_init(u32 ser_num, u32 bus, u32 freq_hz, u32 buf_sz);
struct jtag_ctrl_intf* mpsse_jtag_init   (u32 vid, u32 pid, u32 dev_id, u32 port,
                                           u32 frequency_hz, u32 buffer_size,
                                           struct gpio_intf **);
void                   mpsse_jtag_deinit (struct jtag_ctrl_intf *);

/******************************************************************************/

#endif //  _MPSSE_JTAG_H_
