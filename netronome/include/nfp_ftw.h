/*
 * Copyright (C) 2018 Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_f2w.h
 * @brief         Headers for Fail-to-Wire
 *
 */
#ifndef __NFP_F2W_H__
#define __NFP_F2W_H__

struct nfp_device;

typedef enum {
	nfp_ftw_port_status_active = 0,  /* Active mode */
	nfp_ftw_port_status_wire,        /* Wire mode */
	nfp_ftw_port_status_unknown,     /* Unknown FTW state */
	nfp_ftw_port_status_not_ftw      /* Port is not a relay FTW bump pair */
} nfp_ftw_port_status;

typedef enum {
	nfp_ftw_watchdog_timer_state_armed = 0, /* Armed */
	nfp_ftw_watchdog_timer_state_disarmed   /* Disarmed */
} nfp_ftw_watchdog_timer_state;

/* Arm the FTW port
 * @port - in: internal port number
 */
int nfp_ftw_arm(int port);

/* Disarm the FTW port
 * @port - in: internal port number
 */
int nfp_ftw_disarm(int port);

/* Set a port to active mode
 * Pkts will be delivered to the NFP
 * @port - in: internal port number
 */
int nfp_ftw_set_active(int port);

/* Set a port to wire mode
 * Pkts will be delivered to the port's relay FTW bump partner
 * @port - in: internal port number
 */
int nfp_ftw_set_wire(int port);

/* Get FTW get port status
 * @port    -  in: internal port number
 * @status  - out: Status of FTW port
 * @partner - out: bump partner port
 */
int nfp_ftw_get_port_status(int port, nfp_ftw_port_status *status, int *partner);

/* Enable the watchdog timer
 * @dev  - in: pointer to nfp descriptor
 * @time - in: time in seconds
 */
int nfp_ftw_arm_watchdog_timer(struct nfp_device *dev, int timeout);

/* Disable the watchdog timer
 * @dev  - in: pointer to nfp descriptor
 */
int nfp_ftw_disarm_watchdog_timer(struct nfp_device *dev);

/* Reset watchdog timer preventing timeout
 * @dev  - in: pointer to nfp descriptor
 */
int nfp_ftw_tickle_watchdog_timer(struct nfp_device *dev);

/* Get the watchdog timer status
 * @dev   - in: pointer to nfp descriptor
 * @state - out: state of watchdog timer
 * @time  - out: time in seconds
 */
int nfp_ftw_get_watchdog_timer_status(struct nfp_device *dev,
				      nfp_ftw_watchdog_timer_state *state,
				      int *timeout);

#endif /* !__NFP_F2W_H__ */
