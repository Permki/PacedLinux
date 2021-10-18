/*
 * Copyright (C) 2017, Netronome Systems, Inc.
 * All right reserved.
 *
 *
 */

#ifndef NFP_MEDIA_OVERRIDE_H
#define NFP_MEDIA_OVERRIDE_H

int nfp_media_add_override(struct nfp_device *nfp, int phy, const char *media_val);
int nfp_media_remove_override(struct nfp_device *nfp, int phy, const char *media_val);
int nfp_media_add_aneg_override(struct nfp_device *nfp, int phy, int eth, char aneg_val);
int nfp_media_add_fec_override(struct nfp_device *nfp, int phy, char fec_val);
int nfp_media_media_from_speed(struct nfp_device *nfp, int phy, int speed, const char **media);
int nfp_media_sanity_check_type(struct nfp_device *nfp, int phy, const char *media);
int nfp_media_sanity_check_order(struct nfp_device *nfp, int phy, const char *media);
int nfp_media_set_platform_overrides(struct nfp_device *nfp);

#endif /* NFP_MEDIA_OVERRIDE_H */
