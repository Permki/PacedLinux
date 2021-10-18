/*
 * Copyright (C) 2010, Netronome Systems, Inc.  All rights reserved.
 *
 * Cayenne CPLD (Nightshade) registers
 */

#ifndef NSCPLD_H
#define NSCPLD_H

#define NSCPLD_VER_REV	0	/* Version/Revision */
#define  NSCPLD_VER_REV_REV_of(x)	(((x) >> 0) & 0xffff)
#define  NSCPLD_VER_REV_MIN_of(x)	(((x) >> 16) & 0xff)
#define  NSCPLD_VER_REV_MAJ_of(x)	(((x) >> 24) & 0xff)

#define NSCPLD_CSR	1	/* Control/Status */
#define  NSCPLD_TCAM_DCM_LOCKED		BIT(0)	/* DCM is locked */
#define  NSCPLD_TCAM_INIT_DONE		BIT(1)	/* Init done */
#define  NSCPLD_NFP_GPIO6_DATA		BIT(2)	/* Data in/out */
#define  NSCPLD_NFP_GPIO6_DIR		BIT(3)	/* Set for output */

#define NSCPLD_RST_CTRL	2	/* Reset Control */
#define  NSCPLD_RST_CTRL_VMON_RESET_N	BIT(0)	/* VMON reset control */
#define  NSCPLD_RST_CTRL_KS8851_RESET_N	BIT(1)	/* KS8851 reset control */
#define  NSCPLD_RST_CTRL_RXAUI_RESET_N	BIT(2)	/* RXAUI reset control */
#define  NSCPLD_RST_CTRL_XGPHY1_RESET_N	BIT(3)	/* XGPHY1 reset control */
#define  NSCPLD_RST_CTRL_XGPHY2_RESET_N	BIT(4)	/* XGPHY2 reset control */
#define  NSCPLD_RST_CTRL_TCAM_RESET_N	BIT(5)	/* TCAM reset control */
#define  NSCPLD_RST_CTRL_FPGA_RESET_N	BIT(6)	/* TCAM Bridge FPGA reset control */

#define NSCPLD_LED_CTRL	3	/* Led Control */
#define  NSCPLD_LED_CTRL_LED_of(x, port, led)	(((x) >> (4 * ((port) & 1) + 2 * ((led) & 1))) & 0x3)
#define  NSCPLD_LED_CTRL_LED(port, led, state)	(((state) & 3) << (4 * ((port) & 1) + 2 * ((led) & 1)))
#define   NSCPLD_LED_OFF	0
#define   NSCPLD_LED_GREEN	1
#define   NSCPLD_LED_YELLOW	2
#define  NSCPLD_LED_CTRL_ENABLE		BIT(8)	/* Set to allow direct control of the LEDs */

#endif /* NSCPLD_H */
