Catamaran (SDN + NFM) picoengine firmware

/**
 * Packet Metadata Format
 * Bit    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
 * -----\ 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * Word  +-----------+-------------------+---+---------------------------+
 *    0  | CTM Number| Packet Number     |Bls|     Packet Length         |
 *       +-+---+-----+-------------------+---+---------------------------+
 *    1  |S|Rsv|                   MU Pointer                            |
 *       +-+---+-------------------------+---+-----+-----+-+-----+-+-----+
 *    2  |      Sequence Number          |NFP|  R  | Seq |P|MType|V| BP  |
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    3  |        Reserved         |E|TLD|  OL4  |  OL3  |   OL2   |  R  |
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    4  |     Port      |    HP-Off1    |    HP-Off0    |F|M|T| Reserved|
 *       |               |               |               |r|c|D|         |
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    5  |P|I|P|S|Tag|I|T|     O-Off     |       LIF+Mode / Match        |
 *       |E|E|W|p|Cnt|T|S|               |                               |
 *       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *    Word 0
 *      Bls      Buffer list
 *
 *    Word 1
 *      S        1 if packet is split between CTM and MU memory buffers
 *      Rsv      Reserved
 *
 *    Word 2
 *      NFP      Target NFP (ignore)
 *      R        Reserved
 *      Seq      Sequencer
 *      P        Protocol error; error if 1
 *      MType    Metadata type
 *                 0      = NBI 0
 *                 1      = NBI 1
 *                 others = reserved
 *      V        Valid metadata flag; error if 0
 *      BP       (NBI DMA) buffer pool
 *
 *    Word 3
 *      E        Early termination; type information may be incomplete if 1
 *      TLD      Tunnel layer depth
 *                 0 = no tunnels detected
 *                 1 = one tunnel detected (e.g. GRE, VXLAN)
 *      OL4      Outer L4 encapsulation type
 *                 0      = Other/unknown outer L4 type
 *                 2      = UDP
 *                 3      = TCP
 *                 5      = Non-first fragment
 *                 13     = GRE
 *                 14     = UDP-VXLAN
 *                 others = reserved
 *      OL3      Outer L3 encapsulation type
 *                 0      = Other/unknown outer L3 type
 *                 1      = ARP
 *                 2      = MPLS
 *                 3      = FCoE
 *                 4      = IPv4
 *                 5      = IPv6
 *                 others = reserved
 *      OL2      Outer L2 encapsulation type
 *                 0      = Other/unknown outer L2 type
 *                 4      = Ethernet II (ETH)
 *                 6      = ETH-PPPoE
 *                 others = reserved
 *      R        Reserved
 *
 *    Word 4
 *      Port     Port (or channel) number
 *      HP-Off1  Header parse offset #1
 *                 ETH-IPv4-TCP:                TCP payload offset
 *                 ETH-IPv4-UDP:                UDP payload offset
 *                 ETH-IPv4-UDP-VXLAN:          VXLAN payload offset
 *                 Others:                      0
 *      HP-Off0  Header parse offset #0
 *                 ETH-ARP:                     ARP header offset
 *                 ETH-FCoE:                    FCoE header offset
 *                 ETH-IPv4-GRE:                GRE payload offset
 *                 ETH-IPv4-Non-first-fragment: IPv4 payload offset
 *                 ETH-IPv4-Other:              IPv4 payload offset
 *                 ETH-IPv4-TCP:                TCP header offset
 *                 ETH-IPv4-UDP:                UDP header offset
 *                 ETH-IPv4-UDP-VXLAN:          UDP header offset
 *                 ETH-IPv6:                    IPv6 payload offset
 *                 ETH-MPLS:                    MPLS stack payload offset
 *                 ETH-Other:                   Ethernet payload offset
 *                 ETH-PPPoE:                   PPPoE payload offset
 *                 Unknown packet types:        Possible L2/L3 header offset
 *      Fr       Fragment flag (IPv4 only)
 *      Mc       Multicast flag (ETH-MPLS only)
 *      TD       TCP SYN/RST detected flag (IPv4-TCP only)
 *
 *    Word 5
 *      PE       Packet error (error if 1)
 *      IE       Interface error (configuration error if 1)
 *      PW       Packet warning (possible header error if 1)
 *      Sp       Special handling (possible special header present if 1)
 *      Tag Cnt  VLAN tag count
 *                 0 = No VLAN tags detected
 *                 1 = 1 VLAN tag detected
 *                 2 = 2 VLAN tags detected
 *                 3 = 3+ VLAN tags detected
 *      IT       Initial TCAM tag flag (default is 0)
 *      TS       Table set indicator (used for synchronization purposes)
 *      O-Off    Outer offset
 *                 ETH-ARP:                     Ethernet header offset
 *                 ETH-FCoE:                    Ethernet header offset
 *                 ETH-IPv4-GRE:                IPv4 header offset
 *                 ETH-IPv4-Non-first-fragment: IPv4 header offset
 *                 ETH-IPv4-Other:              IPv4 header offset
 *                 ETH-IPv4-TCP:                IPv4 header offset
 *                 ETH-IPv4-UDP:                IPv4 header offset
 *                 ETH-IPv4-UDP-VXLAN:          IPv4 header offset
 *                 ETH-IPv6:                    IPv4 header offset
 *                 ETH-MPLS:                    MPLS label stack offset
 *                 ETH-Other:                   Ethernet header offset
 *                 ETH-PPPoE:                   Ethernet header offset
 *                 Unknown packet types:        Start-of-packet offset
 *      LIF+Mode/Match
 *        If LIF+mode mode is enabled for the port:
 *            Bits [15:12] - LIF Mode
 *            Bits [11:10] - Reserved
 *            Bits [ 9: 0] - LIF ID (no LIF found if 0; interface error is set)
 *        If MAC DA matching is enabled for the port:
 *            Bits [15: 0] - MAC DA match ID (no match found if 0)
 *        If port is unconfigured:
 *            Bits [15: 0] - 0xffff (interface error is set)
 */

