/**
 * Copyright (C) 2013-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * nfp nsp serial api
 *
 */
#ifndef __NFP_NSPTERM_H__
#define __NFP_NSPTERM_H__

int nspterm_server(struct nfp_device *nfp,
		   char *rx,
		   uint32_t *rxsz,
		   char *tx,
		   uint32_t *txsz);

int nspterm_available(struct nfp_device *nfp);

int nspterm_client(struct nfp_device *nfp,
		   char *rx,
		   uint32_t *rxsz,
		   char *tx,
		   uint32_t *txsz);

int nspterm_srv_peekc(void *data);

int nspterm_srv_readc(void *data);

int nspterm_srv_writec(void *data, char c);

#endif /* __NFP_NSPTERM_H__ */
