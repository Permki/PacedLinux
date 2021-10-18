/**
 * Copyright (C) 2013-2016 Netronome Systems, Inc.  All rights reserved.
 *
 * nfp nsp lockless flags
 *
 */
#ifndef __NFP_NSPFLAGS_H__
#define __NFP_NSPFLAGS_H__

/* ARM will spin with no resources held, waiting for this flag to clear */
#define NSPFLAG_BUSYWAIT 0
/* reset/clear nsp log */
#define NSPFLAG_CLEARLOG 1
/* NSP will exit returning to nfp-boot/nanotemp */
#define NSPFLAG_EXIT     2

#define NSPFLAG_SET(v, f) ((v) & (1 << (f)))

struct nspflags;

int nfp_nspflags_available(struct nfp_device *nfp);

/* ARM/NSP only routines */
int nfp_nspflags_resource_init(struct nfp_device *nfp);

int nfp_nspflags_read(struct nfp_device *nfp, uint32_t *flags);

int nfp_nsptask_set(struct nfp_device *nfp,
		    const char *task,
		    int index,
		    int spins);

int nfp_nsptask_get_current_index(struct nfp_device *nfp, int *index);

/* client/host control routines */
int nfp_nspflag_set(struct nfp_device *nfp, uint32_t flag);

int nfp_nspflag_clear(struct nfp_device *nfp, uint32_t flag);

int nfp_nspflags_get(struct nfp_device *nfp, uint32_t *flags);

int nfp_nsptask_get(struct nfp_device *nfp, char *task, int sz, int *spins);

#endif /* __NFP_NSPFLAGS_H__ */
