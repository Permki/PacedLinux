/* Copyright (C) 2016,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __PIF_PKT_DUPLICATION_H__
#define __PIF_PKT_DUPLICATION_H__

#include <pif_port_spec.h>

enum pkt_inst_type {

    PIF_PKT_INSTANCE_TYPE_NORMAL = 0,           /* normal packet                */

    /*
     * to ingress
     */
    PIF_PKT_INSTANCE_TYPE_I2I_CLONE,            /* clone ingress to ingress     */
    PIF_PKT_INSTANCE_TYPE_E2I_CLONE,            /* clone egress to ingress      */
    PIF_PKT_INSTANCE_TYPE_INGRESS_RECIRC,       /* recirculate packet           */
    PIF_PKT_INSTANCE_TYPE_RESUBMIT,             /* resubmit packet              */

    /*
     * to egress
     */
    PIF_PKT_INSTANCE_TYPE_I2E_CLONE = 8,            /* clone ingress to egress      */
    PIF_PKT_INSTANCE_TYPE_E2E_CLONE = 9,            /* clone egress to egress       */
    PIF_PKT_INSTANCE_TYPE_REPLICATION = 10          /* multicast packet             */
};

/*
 * used for packet duplication/multicast and cloning
 */
struct pif_pkt_duplication_type {
    union {
        struct {
            uint32_t action     :1;
            uint32_t reserved   :15;
            uint32_t type       :16;
        };
        uint32_t __raw;
    };
};


extern __cls  uint32_t                          parrep_copy[PIF_PARREP_LEN_LW];
extern __cls struct pif_pkt_combined            pif_pkt_info_copy;
extern __nnr struct pif_pkt_duplication_type    pkt_dup_state;


#define PIF_DUP_ACTION_SET(pkt_instance_type) ((1 << 31) | (pkt_instance_type))

#define PIF_DUP_ACTION_CLEAR(pkt_type)   (pkt_type.action = 0x00)

#define PIF_DUP_EGRESS_ACTION(pkt_type)  (pkt_type.type & 0x08)

#endif /* __PIF_PKT_DUPLICATION_H__ */
