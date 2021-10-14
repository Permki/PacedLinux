header_type eth_hdr {
    fields {
    dst : 48;
    src : 48;
    etype : 16;
    }
}



header_type xlan_hdr {
    fields {
        tag : 32;
        etype : 16;
        }
    }

header eth_hdr eth;

header xlan_hdr xlan;

#define XLAN_ETYPE 0x9999

parser start {
    return eth_parse;
}



parser eth_parse {
    extract(eth);
    return select(eth.etype) {
        XLAN_ETYPE: xlan_parse;
        default: ingress;
        }

}

parser xlan_parse {
    extract(xlan);
    return ingress;
}



control ingress {
    apply(encap_tbl);
}

table encap_tbl{
    reads {
        standard_metadata.ingress_port : exact;
        }
    actions {
        encap_act;
        drop_act;
        }
}

action drop_act(){
    drop();
}

counter encap_counter{
    type : packets;
    instance_count : 1;
}

counter decap_counter{
    type : bytes;
    instance_count : 1;
}

//primitive_action filter_func();

action encap_act(prt, tag) {
   // filter_func();
    modify_field(standard_metadata.egress_spec, prt);
    xlan_encap(tag);
    count(encap_counter, 0);
}



action xlan_encap(tag) {
    add_header(xlan);
    modify_field(xlan.tag, tag);
    modify_field(xlan.etype, eth.etype);
    modify_field(eth.etype, XLAN_ETYPE);
}