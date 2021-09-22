/*
 * Copyright (C) 2015-2018,  Netronome Systems, Inc.  All rights reserved.
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
 * @file          apps/wire/wire_main.c
 * @brief         Main program for ME cut-through wire app
 *
 * This application acts as a "wire" which by default will return packets
 * back out on the port they were received on.
 *
 *
 * It also doubles up as test code for the header extract
 * code. The packet headers are extracted and counts are maintained
 * for different types of packets.
 */

/* Flowenv */
#include <nfp.h>
#include <stdint.h>

#include <pkt/pkt.h>
#include <net/eth.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <nfp/mem_atomic.h>
#include <nfp/mem_bulk.h>
#include <nfp/me.h>
/*
 * Mapping between channel and TM queue
 */
#ifndef NBI
#define NBI 0
#endif

#ifndef PKT_NBI_OFFSET
#define PKT_NBI_OFFSET 128
#endif

 __packed struct pkt_hdr {
	 struct {
		uint32_t mac_timestamp;
		uint32_t mac_prepend;
	};
	struct eth_hdr eth_hdr;
	struct ip4_hdr ipv4_hdr;
	struct tcp_hdr tcp_hdr;
	uint16_t padding;
};



// __declspec(shared ctm) is one copy shared by all threads in an ME, in CTM
// __declspec(shared export ctm) is one copy shared by all MEs in an island in CTM (CTM default scope for 'export' of island)
// __declspec(shared export imem) is one copy shared by all MEs on the chip in IMU (IMU default scope for 'export' of global)
// __declspec(shared scope(island) export cls) struct statistics stats;
// __declspec(shared scope(global) export imem) struct counters counters;

void timing_loop(long nanosecs);


struct pkt_rxed {
	struct nbi_meta_catamaran nbi_meta;
	struct pkt_hdr            pkt_hdr;
};

__mem40 struct pkt_hdr *receive_packet( struct pkt_rxed *pkt_rxed, size_t size )
{
	__xread struct pkt_rxed pkt_rxed_in;
	int island, pnum;
	int pkt_off;
	__mem40 struct pkt_hdr *pkt_hdr;
    
	pkt_nbi_recv(&pkt_rxed_in, sizeof(pkt_rxed->nbi_meta));
	pkt_rxed->nbi_meta = pkt_rxed_in.nbi_meta;

	pkt_off  = PKT_NBI_OFFSET;
	island   = pkt_rxed->nbi_meta.pkt_info.isl;
	pnum     = pkt_rxed->nbi_meta.pkt_info.pnum;
	pkt_hdr  = pkt_ctm_ptr40(island, pnum, pkt_off);

	mem_read32(&(pkt_rxed_in.pkt_hdr), pkt_hdr, sizeof(pkt_rxed_in.pkt_hdr));
	pkt_rxed->pkt_hdr = pkt_rxed_in.pkt_hdr;

	if (pkt_hdr->tcp_hdr.off == 0x7) timing_loop(40000000);

	return pkt_hdr;
}



void send_packet( struct nbi_meta_catamaran *nbi_meta, __mem40 struct pkt_hdr *pkt_hdr )
{
	int island, pnum, plen;
	int pkt_off;
	__gpr struct pkt_ms_info msi;
	__mem40 char *pbuf;
	uint16_t q_dst = 0;

    /* Write the MAC egress CMD and adjust offset and len accordingly */
	pkt_off = PKT_NBI_OFFSET + MAC_PREPEND_BYTES;
	island = nbi_meta->pkt_info.isl;
	pnum   = nbi_meta->pkt_info.pnum;
	pbuf   = pkt_ctm_ptr40(island, pnum, 0);
	plen   = nbi_meta->pkt_info.len - MAC_PREPEND_BYTES;

    /* Set egress tm queue.
     * Set tm_que to mirror pkt to port on which in ingressed. */
	q_dst  = PORT_TO_CHANNEL(nbi_meta->port);

	pkt_mac_egress_cmd_write(pbuf, pkt_off, 1, 1); // Write data to make the packet MAC egress generate L3 and L4 checksums

	msi = pkt_msd_write(pbuf, pkt_off); // Write a packet modification script of NULL
	pkt_nbi_send(island,
		pnum,
		&msi,
		plen,
		NBI,
		q_dst,
		nbi_meta->seqr,
		nbi_meta->seq,
		PKT_CTM_SIZE_256);
}

void timing_loop(long nanosecs){

	long curr_time, sleep_time;
	long last_time =  me_tsc_read();
	long period = nanosecs;
    
	while (1) {
		curr_time = me_tsc_read();
		if (curr_time - last_time >= period) return;
	
		sleep_time = period - (curr_time - last_time);
		if (sleep_time >= 1 << 20) // see documentation for sleep()
			sleep_time = (1 << 20) - 1;

		sleep(sleep_time);
    }
}


int main(void)
{
	struct pkt_rxed pkt_rxed; /* The packet header received by the thread */
	__mem40 struct pkt_hdr *pkt_hdr;    /* The packet in the CTM */

	for (;;) {
		pkt_hdr = receive_packet(&pkt_rxed, sizeof(pkt_rxed));
		send_packet(&pkt_rxed.nbi_meta, pkt_hdr);
    	}

	return 0;
}

/* -*-  Mode:C; c-basic-offset:4; tab-width:4 -*- */