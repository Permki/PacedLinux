/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __IPV6_H__
#define __IPV6_H__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file ipv6.h Microengine IPv6 Defines
 * @addtogroup ipv6 Microengine IPv6
 * @{
 *
 * @name Microengine IPv6 Defines
 * @{
 *
 */

/*
 * Per RFC 2460
 * IPv6 Header Format:
 *
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |Version| Traffic Class |           Flow Label                  |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |         Payload Length        |  Next Header  |   Hop Limit   |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                         Source Address                        +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +                      Destination Address                      +
 *  |                                                               |
 *  +                                                               +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */


#define IPV6_SUCCESS    0x0
#define IPV6_FAILURE    0x1

#define IXP_IPV6_SUCCESS   IPV6_SUCCESS
#define IP_SOURCE      1
#define IP_DESTINATION     2

#ifndef IP_DEST
    #define IP_DEST        IP_DESTINATION
#endif


#define IPV6_DROP_BASE                  0x80
#define IPV6_SOURCE_ADDR_MULTICAST      (IPV6_DROP_BASE + 2)
#define IPV6_DEST_ADDR_ZERO_OR_ONE      (IPV6_DROP_BASE + 3)


/*  IXP2XXX Mapping
 *
 *  Byte 0       1        2        3        4       5         6       7
 * |--------|--------|--------|--------|--------|--------|--------|--------|
 *
 *  Long Word        0                                   1
 * |-----------------------------------|-----------------------------------|
 *
 *  Quad Word
 * |-----------------------------------------------------------------------|
 *
 *
 *  -----------------------------------------------------------------------
 * |Ver| Tfc Cls|     Flow  Label      | Payload Length  | NextHdr| HopLmt |
 * |-----------------------------------------------------------------------|
 * |                          Source IPv6 Addr                             |
 * |                                                                       |
 * |-----------------------------------------------------------------------|
 * |                       Destination IPv6 Addr                           |
 * |                                                                       |
 * |-----------------------------------------------------------------------|
 * |                                                                       |
 * |                                 DATA                                  |
 * |                                .......                                |
 *
 *
 */

#define IPV6_VERSION_TRAFFIC_CLASS_HI4         0, 1      /**< 4 bit version and
                                                          * MSB 4 bits of
                                                          * traffic class
                                                          * offset 0, 1 byte
                                                          */
#define IPV6_TRAFFIC_CLASS_LO4_FLOW_LABEL_HI4  1, 1      /**< 4 LSB bits of
                                                          * traffic class and
                                                          * 4 MSB bits of flow
                                                          * label
                                                          * offset 1, 1 byte
                                                          */
#define IPV6_FLOW_LABEL_LO16                   2, 2      /**< 16 LSB bits of bits
                                                          * of flow label
                                                          * offset 2, 2 bytes
                                                          */
#define IPV6_BYTES_4_7                         4, 4


#define IPV6_PAYLOAD_LENGTH                    4, 2      /**< Payload length
                                                          * offset 4, 2 bytes
                                                          */
#define IPV6_NEXT_HEADER                       6, 1      /**< Next Header
                                                          * offset 6, 1 bytes
                                                          */
#define IPV6_HOP_LIMIT                         7, 1      /**< Hop Limit
                                                          * offset 7, 1 bytes
                                                          */
#define IPV6_SRC_ADDR                          8, 16     /**< Source Address
                                                          * offset 8, 16 bytes
                                                          */
#define IPV6_DST_ADDR                         24, 16     /**< Destination Address
                                                          * offset 24, 16 bytes
                                                          */
#define IPV6_SRC_ADDR_127_120                  8, 1      /**< Source Address
                                                          * bits [127..112]
                                                          * offset  8, 1 byte
                                                          */
#define IPV6_SRC_ADDR_127_96                   8, 4      /**< Source Address
                                                          * bits [127..96]
                                                          * offset  8, 4 bytes
                                                          */
#define IPV6_SRC_ADDR_95_64                   12, 4      /**< Source Address
                                                          * bits [ 95..64]
                                                          * offset 12, 4 bytes
                                                          */
#define IPV6_SRC_ADDR_63_32                   16, 4      /**< Source Address
                                                          * bits [ 63..32]
                                                          * offset 16, 4 bytes
                                                          */
#define IPV6_SRC_ADDR_31_0                    20, 4      /**< Source Address
                                                          * bits [ 31.. 0]
                                                          * offset 20, 4 bytes
                                                          */
#define IPV6_DST_ADDR_127_96                  24, 4      /**< Dst Address
                                                          * bits [127..96]
                                                          * offset 24, 4 bytes
                                                          */
#define IPV6_DST_ADDR_95_64                   28, 4      /**< Dst Address
                                                          * bits [ 95..64]
                                                          * offset 28, 4 bytes
                                                          */
#define IPV6_DST_ADDR_63_32                   32, 4      /**< Dst Address
                                                          * bits [ 63..32]
                                                          * offset 32, 4 bytes
                                                          */
#define IPV6_DST_ADDR_31_0                    36, 4      /**< Dst Address
                                                          * bits [ 31.. 0]
                                                          */

#define IPV6_SOURCE_PORT                      40         /**< Source port
                                                          * offset 40, 2 byte
                                                          */
#define IPV6_DESTINATION_PORT                 42         /**< Destination port
                                                          * offset 42, 2 byte
                                                          */

//
// Starting byte and count for IPv6 header fields.  These are used primarily
// by microC.

#define IPV6_VERSION_TRAFFIC_CLASS_FLOW_LABEL_START     0
#define IPV6_VERSION_TRAFFIC_CLASS_FLOW_LABEL_COUNT     4

#define IPV6_VERSION_TRAFFIC_CLASS_HI4_START            0
#define IPV6_VERSION_TRAFFIC_CLASS_HI4_COUNT            1

#define IPV6_TRAFFIC_CLASS_LO4_FLOW_LABEL_HI4_START     1
#define IPV6_TRAFFIC_CLASS_LO4_FLOW_LABEL_HI4_COUNT     1

#define IPV6_FLOW_LABEL_LO16_START                      2
#define IPV6_FLOW_LABEL_LO16_COUNT                      2

#define IPV6_BYTES_4_7_START                            4
#define IPV6_BYTES_4_7_COUNT                            4

#define IPV6_PAYLOAD_LENGTH_START                       4
#define IPV6_PAYLOAD_LENGTH_COUNT                       2

#define IPV6_NEXT_HEADER_START                          6
#define IPV6_NEXT_HEADER_COUNT                          1

#define IPV6_HOP_LIMIT_OFFSET                           7
#define IPV6_HOP_LIMIT_LENGTH                           1

#define IPV6_SRC_ADDR_START                             8
#define IPV6_SRC_ADDR_COUNT                             16

#define IPV6_SRC_ADDR_127_96_START                      8
#define IPV6_SRC_ADDR_127_96_COUNT                      4

#define IPV6_SRC_ADDR_95_64_START                       12
#define IPV6_SRC_ADDR_95_64_COUNT                       4

#define IPV6_SRC_ADDR_63_32_START                       16
#define IPV6_SRC_ADDR_63_32_COUNT                       4

#define IPV6_SRC_ADDR_31_0_START                        20
#define IPV6_SRC_ADDR_31_0_COUNT                        4

#define IPV6_DST_ADDR_START                             24
#define IPV6_DST_ADDR_COUNT                             16

#define IPV6_DST_ADDR_127_96_START                      24
#define IPV6_DST_ADDR_127_96_COUNT                      4

#define IPV6_DST_ADDR_95_64_START                       28
#define IPV6_DST_ADDR_95_64_COUNT                       4

#define IPV6_DST_ADDR_63_32_START                       32
#define IPV6_DST_ADDR_63_32_COUNT                       4

#define IPV6_DST_ADDR_31_0_START                        36
#define IPV6_DST_ADDR_31_0_COUNT                        4


/*  TRIE_DEPTH codes for LPM */
#define TRIE15  1

//IPV6 extension header values
#define IPV6_EXTN_HOP_BY_HOP 0
#define IPV6_EXTN_ROUTING 43
#define IPV6_EXTN_FRAGMENT 44
#define IPV6_EXTN_DESTINATION 60
#define IPV6_EXTN_AUTHENTICATION 51
#define IPV6_EXTN_ENCAPSULATION 50


/** @}
 * @}
 */

#endif /* __IPV6_H__ */
