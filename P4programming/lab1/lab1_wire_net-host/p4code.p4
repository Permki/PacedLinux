header_type eth_hdr {
    fields {
    dst : 48;
    src : 48;
    etype : 16;
    }
}

header eth_hdr eth;

parser start {
    return eth_parse;
}

parser eth_parse {
    extract(eth);
    return ingress;
}

control ingress {
    apply(encap_tbl);
}

table encap_tbl{
   actions {
        drop_act;
    }
}

action drop_act(){
    drop();
}
