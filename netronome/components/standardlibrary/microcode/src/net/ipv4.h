/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __IPV4_H__
#define __IPV4_H__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file ipv4.h Microengine IPv4 Defines
 * @addtogroup ipv4 Microengine IPv4
 * @{
 *
 * @name Microengine IPv4 Defines
 * @{
 *
 */

#define IPV4_SUCCESS    0x0
#define IPV4_FAILURE    0x1

#define IXP_IPV4_SUCCESS    IPV4_SUCCESS

/* Cases that cause drop packet */
#define     IPV4_BAD_CHECKSUM       IPV4_FAILURE
#define     IPV4_INVALID_ADDRESS    IPV4_FAILURE
#define     IPV4_DBCAST_ADDRESS     IPV4_FAILURE
#define     IPV4_BAD_TOTAL_LENGTH   IPV4_FAILURE

/* end drop packet */

#define    IPV4_NO_OPTIONS              0x45            /**< version 4, header length 5 longwords */

#define ICMP_PROTOCOL           0x01
#define IGMP_PROTOCOL           0x02
#define TCP_PROTOCOL            0x06
#define UDP_PROTOCOL            0x11


#define SNMP_PORT1              0xA0
#define SNMP_PROTOCOL           SNMP_PORT1
#define SNMP_PORT2              0xA1


/*------------------*/
/*  exception codes.*/
/*------------------*/

/* from header validation */
#define IPV4_EXCP_OPTIONS               0x11
#define IPV4_EXCP_LENGTH_MISMATCH       0x12
#define IPV4_EXCP_BAD_TTL               0x13
#define IPV4_EXCP_MULTICAST             0x14

/* from nexthop info */
#define IPV4_EXCP_FRAG_REQUIRED         0x15
#define IPV4_EXCP_REDIRECT              0x16
#define IPV4_EXCP_DOWN                  0x17
#define IPV4_EXCP_NO_ROUTE              0x18
#define IPV4_EXCP_LOCAL_DELIVERY        0x19
#define IPV4_EXCP_LIMITED_BROADCAST     0x1a

/*----------------------------------------------*/
/*  byte offset, size of IP header fields.      */
/*----------------------------------------------*/
#define IP_VERSION_LENGTH 0, 1          /**< Version/length fields.  offset 0, 1 byte   */
#define IP_TYPE_OF_SERVICE 1, 1         /**< Type of service field.  offset 1, 1 byte   */
#define IP_TOTAL_LENGTH 2, 2            /**< Type of service field.  offset 2, 2 bytes  */
#define IP_IDENTIFICATION 4, 2          /**< Identification field.   offset 4, 2 bytes  */
#define IP_FLAGS_FRAGMENT 6, 2          /**< Flags/fragment fields.  offset 6, 2 bytes  */
#define IP_TIME_TO_LIVE 8, 1            /**< Time to live field.     offset 8, 1 byte   */
#define IP_PROTOCOL 9, 1                /**< Protocol field.         offset 9, 1 byte   */
#define IP_CHECKSUM 10, 2               /**< Checksum field.         offset 10, 2 bytes */
#define IP_SOURCE_ADDRESS 12, 4         /**< Source ip address.      offset 12, 4 bytes */
#define IP_DESTINATION_ADDRESS 16, 4    /**< Destination ip address. offset 16, 4 bytes */
#define IP_DEST_ADDRESS IP_DESTINATION_ADDRESS
#define IP_SOURCE_PORT 20, 2            /**< Source port.            offset 20, 2 bytes */
#define IP_DESTINATION_PORT 22, 2       /**< Destination port.       offset 22, 2 bytes */
#define IP_SRC_DEST_PORT 20, 4          /**< Source & destination port offset 20 , 4 bytes */

/* address types in IPv4 header */
/* These are valid values for WHICH_ADDR operand in ipv4_addr_verify */
#define IP_SRC  0x0
#define IP_DEST 0x1

/* n_tuple Types fro ipv4_n_tuple_extract & ipv4_n_tuple_lookup */

#ifndef FIVE_TUPLE
    #define  FIVE_TUPLE   1
#endif
#ifndef SIX_TUPLE
    #define SIX_TUPLE 2
#endif

#define IPV4_GP_REG 1
#define IPV4_LOCAL_MEM 2
#define IPV4_NOT_GPR_OR_LMEM 3


/** @}
 * @}
 */

#endif /* __IPV4_H__ */
