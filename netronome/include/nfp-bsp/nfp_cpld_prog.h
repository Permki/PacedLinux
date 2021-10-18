/**
 * Copyright (C) 2008-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_cpld_prog.h
 *
 */

#ifndef _NFP_CPLD_PROG_H_
#define _NFP_CPLD_PROG_H_

/******************************************************************************/
int nfp_cpld_usb_update(char *amda, char *jedfile);
int nfp_cpld_update(int nfpid, char *jedfile, int phymod);
/******************************************************************************/

#endif /* _NFP_CPLD_PROG_H_ */
