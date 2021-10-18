/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file machxo2_prog_intf.h
 *
 */


#ifndef __MACHXO2_PROG_INTF_H__
#define __MACHXO2_PROG_INTF_H__
/******************************************************************************/

#define MACH_XO2_CFG_MODE_TRANSPARENT    1
#define MACH_XO2_CFG_MODE_OFFLINE        2

#define MACH_XO2_MEMSPACE_CONFIG         0
#define MACH_XO2_MEMSPACE_UFM            4

#define MACH_XO2_ERASE_SRAM              1
#define MACH_XO2_ERASE_FEATURE           2
#define MACH_XO2_ERASE_CONFIG            4
#define MACH_XO2_ERASE_UFM               8

#define PROGFLAG_OFFLINE                (1 << 0)
#define PROGFLAG_NOREFRESH              (1 << 1)

struct machxo2_prog_intf {

    void *priv;

    int (*interface_type)(struct machxo2_prog_intf *);

    int (*program) (struct machxo2_prog_intf *, char *jed_fname,
                                            u32 idcode, u32 flags);
    int (*read)    (struct machxo2_prog_intf *, u32 offline_mode);
    int (*erase)   (struct machxo2_prog_intf *, u32 idcode, u32 offline_mode);
};

/******************************************************************************/
#endif // __MACHXO2_PROG_INTF_H__

