/*
 * Copyright (C) 2008-2011 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef __ETHERNET_H__
#define __ETHERNET_H__

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file ethernet.h Microengine Ethernet Defines
 * @addtogroup ethernet Microengine Ethernet
 * @{
 *
 * @name Microengine Ethernet Defines
 * @{
 *
 */


#define ETHER_SRC_START_ADDR    0x6         //Byte 6 onwards
#define ETHER_DEST_START_ADDR   0x0         //Byte 0
#define ETHER_TYPE_START_ADDR   0xC         //Byte 12 and 13

#define ETHER_BCAST_ADDR1       0xFFFFFFFF  //First 4 bytes from MSB
#define ETHER_BCAST_ADDR2       0xFFFF      //Remaining 2 bytes
#define ETHER_RESERVED_ADDR1    0x0180C200  //First 4 bytes of reserved address from MSB

#define ETHER_SUCCESS           0x0         //Indicates SUCCESS

/* Exception error code */
#define ETHER_NO_EXCEPTION          0x0     //No exception
#define ETHER_INVALID_SRC_ADDRESS   0x90    //Invalid src address. Might be broadcast or multicast
#define ETHER_RESERVED_ADDRESS      0x91    //Invalid dest address.
#define ETHER_BCAST_FAILURE         0x92    //Indicates address is not broadcast
#define ETHER_MCAST_FAILURE         0x93    //Indicates address is not multicast
#define ETHER_BPDU_FAILURE          0x94    //Indicates packet is not bpdu


/** @}
 * @}
 */

#endif /* __ETHERNET_H__ */
