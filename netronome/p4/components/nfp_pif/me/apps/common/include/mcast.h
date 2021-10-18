/* Copyright (C) 2016,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __MCAST_H__
#define __MCAST_H__

#ifndef MCAST_GROUP_MAX
    #define MCAST_GROUP_MAX         16      // max number of multicast groups
#endif

#ifndef MCAST_PORTS_MAX
    #define MCAST_PORTS_MAX         16      // max number of multicast ports per group
#endif

/*
 * Used for multicast processing
 * Also use rid in cloning actions to indicate the egress_instance.
 */
struct replication_information {
    union {
        struct {
            int16_t group;
            int16_t rid;
        };
        uint32_t __raw;
    };
};


/** mcast_table_init
 *
 * Initialise multicast table with -1 (no elements in the group).
 */
void mcast_table_init(void);


/** mcast_grp_load
 *
 * Load multicast group information from external memory to local CLS.
 * Creates a local CLS copy of the multicast group. This is valid until multicast
 * processing is complete.
 *
 * @param grp_id             Multicast group id to save.
 * @return                   First multicast group element and next element.
 *                           -1 if list is empty or group id out of range.
 */
int32_t mcast_grp_load(uint32_t grp_id, uint32_t grp_idx);


/** mcast_grp_retrieve_element
 *
 * Retrieve next two multicast elements from our local copy in CLS.
 *
 * @param grp_id             Multicast group id to retrieve from
 * @param element            Index of the element to return.
 * @return                   Current multicast group element and next element.
 *                           -1 if end of the list reached or group id out of range.
 */
int32_t mcast_grp_retrieve_element(uint32_t grp_id, uint32_t element);

int16_t process_multicast(struct replication_information mcast);

#endif /* __MCAST_H__ */
