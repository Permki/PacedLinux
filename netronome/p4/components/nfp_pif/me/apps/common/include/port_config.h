#ifndef __PORT_CONFIG_H__
#define __PORT_CONFIG_H__

#ifndef PORT_CONFIG_COUNT
#define PORT_CONFIG_COUNT 256
#endif

#ifndef PORT_CONFIG_TYPES
#define PORT_CONFIG_TYPES 2
#endif

#ifndef PORT_CONFIG_TYPE
#define PORT_CONFIG_TYPE __mem __addr40
#endif

#define PORT_CONFIG_TYPE_VF 0
#define PORT_CONFIG_TYPE_PHY 1

struct port_config {
    union {
        struct {
            unsigned int nic_offload_vlan_rx : 1;
            unsigned int nic_offload_vlan_tx : 1;
            unsigned int nic_offload_checksum_rx : 1;
            unsigned int nic_offload_checksum_tx : 1;
            unsigned int port_paused : 1;
            unsigned int reserved0: 27;
        };
        unsigned int __raw;
    };
};

extern volatile __export PORT_CONFIG_TYPE struct port_config port_configs[PORT_CONFIG_TYPES * PORT_CONFIG_COUNT];

#define PORT_CONFIG_DECLARE volatile __export PORT_CONFIG_TYPE \
    struct port_config port_configs[PORT_CONFIG_TYPES * PORT_CONFIG_COUNT]

__inline struct
port_config port_config_get(int port_type, int subsys, int port_no)
{
    port_no |= subsys << 6;
    return port_configs[(port_type * PORT_CONFIG_COUNT) | port_no];
}

__inline void
port_config_set(int port_type, int subsys, int port_no,
                struct port_config new_val)
{
    port_no |= subsys << 6;
    port_configs[(port_type * PORT_CONFIG_COUNT) | port_no] = new_val;
}

#endif /* __PORT_CONFIG_H__ */
