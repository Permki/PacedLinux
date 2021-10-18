/**
 * Copyright (C) 2017, Netronome Systems, Inc.
 * All rights reserved.
 *
 * nsp rpc stubs
 *
 */

#ifndef NSP_RPC_STUBS_H
#define NSP_RPC_STUBS_H

/*
 * NSP RPC Client stubs
 */

int nrc_test_func(struct nfp_device *nfp, uint8_t *a, uint64_t *b, uint32_t *c);

struct nfp_phymod *nrc_phymod_next(struct nfp_device *nfp, void **ptr);

int nrc_phymod_get_index(struct nfp_phymod *phy, int32_t *index);
int nrc_phymod_get_label(struct nfp_phymod *phy, const char **label);
int nrc_phymod_read_vendor(struct nfp_phymod *phy, char *vendor, uint32_t size);
int nrc_phymod_get_nbi(struct nfp_phymod *phy, int32_t *nbi);
int nrc_phymod_get_port(struct nfp_phymod *phy, int32_t *base, int32_t *lanes);
int nrc_phymod_get_type(struct nfp_phymod *phy, int32_t *type);
int nrc_phymod_indicate_link(struct nfp_phymod *phy, int32_t is_on);
int nrc_phymod_indicate_activity(struct nfp_phymod *phy, int32_t is_on);
int nrc_phymod_read_status(struct nfp_phymod *phy, uint32_t *txstatus, uint32_t *rxstatus);
int nrc_phymod_read_status_los(struct nfp_phymod *phy, uint32_t *txstatus, uint32_t *rxstatus);
int nrc_phymod_read_status_fault(struct nfp_phymod *phy, uint32_t *txstatus, uint32_t *rxstatus);
int nrc_phymod_read_status_optpower(struct nfp_phymod *phy, uint32_t *txstatus, uint32_t *rxstatus);
int nrc_phymod_read_status_optbias(struct nfp_phymod *phy, uint32_t *rxstatus, uint32_t *txstatus);
int nrc_phymod_read_status_voltage(struct nfp_phymod *phy, uint32_t *txstatus, uint32_t *rxstatus);
int nrc_phymod_read_status_temp(struct nfp_phymod *phy, uint32_t *txstatus, uint32_t *rxstatus);
int nrc_phymod_read_lanedisable(struct nfp_phymod *phy, uint32_t *txstatus, uint32_t *rxstatus);
int nrc_phymod_write_lanedisable(struct nfp_phymod *phy, uint32_t txstate, uint32_t rxstate);
int nrc_phymod_read8(struct nfp_phymod *phy, uint32_t addr, uint8_t *data);
int nrc_phymod_write8(struct nfp_phymod *phy, uint32_t addr, uint8_t data);
int nrc_phymod_verify_sff_checkcode(struct nfp_phymod *phy, int32_t *cc_status);
int nrc_phymod_read_oui(struct nfp_phymod *phy, uint32_t *oui);
int nrc_phymod_read_product(struct nfp_phymod *phy, char *product, uint32_t size);
int nrc_phymod_read_serial(struct nfp_phymod *phy, char *serial, uint32_t size);
int nrc_phymod_read_type(struct nfp_phymod *phy, int32_t *type);
int nrc_phymod_read_connector(struct nfp_phymod *phy, int32_t *connector);
int nrc_phymod_read_length(struct nfp_phymod *phy, int32_t *length);
int nrc_phymod_get_active_or_passive(struct nfp_phymod *phy, int32_t *anp);
int nrc_phymod_read_extended_compliance_code(struct nfp_phymod *phy, uint32_t *val);

struct nfp_phymod_eth *nrc_phymod_eth_next(struct nfp_device *nfp, struct nfp_phymod *phy, void **ptr);

int nrc_phymod_eth_get_index(struct nfp_phymod_eth *eth, int32_t *index);
int nrc_phymod_eth_get_phymod(struct nfp_phymod_eth *eth, struct nfp_phymod **phy, int32_t *lane);
int nrc_phymod_eth_get_mac(struct nfp_phymod_eth *eth, const uint8_t **mac);
int nrc_phymod_eth_get_label(struct nfp_phymod_eth *eth, const char **label);
int nrc_phymod_eth_get_nbi(struct nfp_phymod_eth *eth, int32_t *nbi);
int nrc_phymod_eth_get_port(struct nfp_phymod_eth *eth, int32_t *base, int32_t *lanes);
int nrc_phymod_eth_get_speed(struct nfp_phymod_eth *eth, int32_t *speed);
int nrc_phymod_eth_get_fail_to_wire(struct nfp_phymod_eth *eth, const char **label, int32_t *active);
int nrc_phymod_eth_set_fail_to_wire(struct nfp_phymod_eth *eth, int32_t force);
int nrc_phymod_eth_read_disable(struct nfp_phymod_eth *eth, uint32_t *txstatus, uint32_t *rxstatus);
int nrc_phymod_eth_write_disable(struct nfp_phymod_eth *eth, uint32_t txstate, uint32_t rxstate);

#endif /* NSP_RPC_STUBS_H */
