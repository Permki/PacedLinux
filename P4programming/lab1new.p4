header_type eth_hdr{
    fields {
        dst: 48;
        src: 48;
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

action drop_act(){
    drop();
}

table in_tbl {
    actions {
        drop_act;
    }
}

control ingress{
    apply(in_tbl);
}