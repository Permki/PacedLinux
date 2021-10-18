/*
 * Copyright (C) 2009-2010 Netronome Systems, Inc.  All rights reserved.
 */

///////////////////////////////////////////////////////////////////////////////
//
//      File Name: hardware.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __HARDWARE_H__
#define __HARDWARE_H__

/*
 * This file is used in MicroC source files.  As the compiler does not support
 * the IS_IXPTYPE() macro, we use the unadorned IXP2800 macro.
 */

#ifdef IXP2800
#include <ixp2800_hardware.h>
#else
#include <nfp3200_hardware.h>
#endif

#endif /* __HARDWARE_H__ */
