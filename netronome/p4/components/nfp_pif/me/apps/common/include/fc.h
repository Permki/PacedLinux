/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __FC_H__
#define __FC_H__



fc_lookup_result_t fc_do_lookup(__lmem uint32_t *flowkey,
                                unsigned int flowkey_len,
                                unsigned int pktlen,
                                unsigned int version,
                                unsigned int timeout);

void fc_do_release(__gpr fc_lock_t in_lock);

uint32_t fc_get_version(void);



#endif /* __FC_H__ */
