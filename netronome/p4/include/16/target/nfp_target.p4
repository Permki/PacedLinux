#ifndef __NFP_TARGET_16_P4__
#define __NFP_TARGET_16_P4__

/* NFP target port encodings
 * These may be used to encode and decode values of standard_metadata fields
 * ingress_port, egress_spec and egress_port
 */
#define NFP_TARGET_PORT_SPEC_TYPE_PHYSICAL  0
#define NFP_TARGET_PORT_SPEC_TYPE_LOGICAL   1
#define NFP_TARGET_PORT_SPEC_TYPE_MULTICAST 2
#define NFP_TARGET_PORT_SPEC_TYPE_HOST      3

#define NFP_TARGET_PORT_SPEC_HOST_PORT(_vf, _pcie) (_vf)

#define NFP_TARGET_PORT_SPEC_CHAN_of(_pspec) (((_pspec) >> 10) & 0x3f)
#define NFP_TARGET_PORT_SPEC_TYPE_of(_pspec) (((_pspec) >> 8) & 0x3)
#define NFP_TARGET_PORT_SPEC_PORT_of(_pspec) (((_pspec) >> 0) & 0xff)

#define NFP_TARGET_PORT_SPEC(_port, _type, _chan) \
                ((_port & 0xff) | \
                ((_type & 0x3) << 8) | \
                ((_chan & 0x3f) << 10))

#define NFP_TARGET_PORT_SPEC_DROP NFP_TARGET_PORT_SPEC(-1, -1, -1)

/* NFP target values for standard P4 defines */

#define CPU_PORT NFP_TARGET_PORT_SPEC(NFP_TARGET_PORT_SPEC_HOST_PORT(63,0), \
                                      NFP_TARGET_PORT_SPEC_TYPE_HOST, \
                                      0)
#define PORT_CPU CPU_PORT

#define DROP_PORT NFP_TARGET_PORT_SPEC_DROP

#ifdef TARGET_STANDARD_METADATA_T
@metadata @name("standard_metadata")
struct standard_metadata_t {
    bit<16>  ingress_port;
    bit<14>  packet_length;
    bit<16>  egress_spec;
    bit<16>  egress_port;
    bit<10>  egress_instance;
    bit<4>   instance_type;
    bit<32>  clone_spec;
    bit<8>   parser_error_location;
    bit<3>   parser_status;
    bit<1>  checksum_error;
}
#endif

#endif /* __NFP_TARGET_16_P4__ */
