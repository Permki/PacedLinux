/*
 * Copyright (C) 2012-2015,  Netronome Systems, Inc.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file          lib/net/tcp.h
 * @brief         Definitions for TCP header parsing
 *
 * Incomplete, new definitions will be added as/when needed
 */

#ifndef _NET_TCP_H_
#define _NET_TCP_H_

/**
 * Header length definitions
 * @NET_TCP_LEN             Length of TCP header, no options
 * @NET_TCP_LEN32           Length of TCP header, no options (32bit words)
 */
#define NET_TCP_LEN              20
#define WORDSIZE            4
#define NET_TCP_LEN32            (NET_TCP_LEN / WORDSIZE)

/* TCP flag defines */
#define NET_TCP_FLAG_FIN (1 << 0)  /* Finished */
#define NET_TCP_FLAG_SYN (1 << 1)  /* Synchronize */
#define NET_TCP_FLAG_RST (1 << 2)  /* Reset */
#define NET_TCP_FLAG_PSH (1 << 3)  /* Push */
#define NET_TCP_FLAG_ACK (1 << 4)  /* Acknowledgment */
#define NET_TCP_FLAG_URG (1 << 5)  /* Urgent */
#define NET_TCP_FLAG_ECE (1 << 6)  /* ECN-Echo */
#define NET_TCP_FLAG_CWR (1 << 7)  /* Congestion Window Reduced */

#if defined(__NFP_LANG_MICROC)

/* TCP Options defines */
#define TCPOPT_NOP		1	/* Padding */
#define TCPOPT_EOL		0	/* End of options */
#define TCPOPT_MSS		2	/* Segment size negotiating */
#define TCPOPT_WINDOW		3	/* Window scaling */
#define TCPOPT_SACK_PERM        4       /* SACK Permitted */
#define TCPOPT_SACK             5       /* SACK Block */
#define TCPOPT_TIMESTAMP	8	/* Better RTT estimations/PAWS */
#define TCPOPT_MD5SIG		19	/* MD5 Signature (RFC2385) */
#define TCPOPT_MPTCP		30	/* Multipath TCP (RFC6824) */
#define TCPOPT_FASTOPEN		34	/* Fast open (RFC7413) */
#define TCPOPT_EXP		254	/* Experimental */

#define  TCPOPT_PACEOFFLOAD 200

#include <nfp.h>
#include <stdint.h>

/**
 * TCP Header structure
 */
__packed struct tcp_hdr {
    uint16_t sport;                     /** Source port */
    uint16_t dport;                     /** Destination port */

    uint32_t seq;                       /** Sequence number */

    uint32_t ack;                       /** Acknowledgement number */

    uint8_t  off:4;                     /** Data offset */
    uint8_t  x2:4;                      /** (unused) */
    uint8_t  flags;                     /** Flags */
    uint16_t win;                       /** Window */

    uint16_t sum;                       /** Checksum */
    uint16_t urp;                       /** Urgent pointer */
};
#endif /* __NFP_LANG_MICROC */

#endif /* _NET_TCP_H_ */

/* -*-  Mode:C; c-basic-offset:4; tab-width:4 -*- */
