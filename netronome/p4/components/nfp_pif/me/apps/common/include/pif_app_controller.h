
/* Copyright (C) 2017,  Netronome Systems, Inc.  All rights reserved. */


struct pif_ctrl_header {
    union {
        struct {
            int32_t placeholder_1; /* TODO not sure what the header should be */
            int32_t placeholder_2;
        };
        uint32_t __raw[2];
    };
};

/* TODO Not sure what these should be */
#define CPU_IN_PORT     4
#define CPU_OUT_PORT    4

/* Remove the controller header from the packet */
int decap_controller_pkt(PIF_PKT_INFO_TYPE struct pif_pkt_info
                                        *pktinfo);

/* Add controller header to the packet */
void encap_controller_pkt(PIF_PKT_INFO_TYPE struct pif_pkt_info
                                        *pktinfo);