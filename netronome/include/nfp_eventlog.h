/*
 * Copyright (C) 2015 Netronome Systems, Inc. All rights reserved.
 * Author: Jason McMullan <jason.mcmullan@netronome.com>
 *
 */

#ifndef NFP_EVENTLOG_H
#define NFP_EVENTLOG_H

/* Event log types (0x00..0x7f) */
#define NFP_EVENTLOG_TEMP_OK    0	/* Data - Temp Celsius */
#define NFP_EVENTLOG_TEMP_WARN  1	/* Data - Temp Celsius */
#define NFP_EVENTLOG_TEMP_CRIT  2	/* Data - Temp Celsius */
#define NFP_EVENTLOG_BOOT       3	/* First reset/boot after temp event */

struct nfp_eventlog;

struct nfp_eventlog *nfp_eventlog_open(struct nfp_fis *fis);
void nfp_eventlog_close(struct nfp_eventlog *ev);

int nfp_eventlog_clear(struct nfp_eventlog *ev);
int nfp_eventlog_append(struct nfp_eventlog *ev, int type, int data);

/* Iterate through the event log. Set 'ptr = NULL' for the first
 * entry.
 */
int nfp_eventlog_next(struct nfp_eventlog *ev, void **ptr, int *type, int *data,
		      uint32_t *uptime_sec);

#endif /* NFP_EVENTLOG_H */
