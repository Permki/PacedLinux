/*
 * Copyright (C) 2012-2014,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_resid.c
 * @brief         Functions/macros for resource identification,
 *                like microengine IDs.
 *
 */

#include <nfp-common/nfp_platform.h>
#include <nfp-common/nfp_resid.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

#define NRI_NFP3200_CLUSTER_MAXCNT 5
#define NRI_NFP3200_ME_MAXCNT 8
#define NRI_NFP3200_CTX_MAXCNT 8
#define NRI_NFP6000_ISLAND_MAXCNT 64
#define NRI_NFP6000_ME_MAXCNT 12
#define NRI_NFP6000_CTX_MAXCNT 8

struct nri_iid_alias {
	const char *alias;
	int iid;
};

static const char nri_str_iid[] = "i";
static const size_t nri_str_iid_len = sizeof(nri_str_iid) - 1;
static const char nri_str_cl[] = "cl";
static const size_t nri_str_cl_len = sizeof(nri_str_cl) - 1;
static const char nri_str_me[] = "me";
static const size_t nri_str_me_len = sizeof(nri_str_me) - 1;
static const char nri_str_ctx[] = "ctx";
static const size_t nri_str_ctx_len = sizeof(nri_str_ctx) - 1;
static const char nri_str_tg[] = "tg";
static const size_t nri_str_tg_len = sizeof(nri_str_tg) - 1;

static struct nri_iid_alias nri_nfp6000_iid_alias_map[] = {
	{"arm", 1},

	{"pcie0", 4},
	{"pcie1", 5},
	{"pcie2", 6},
	{"pcie3", 7},

	{"nbi0", 8},
	{"nbi1", 9},

	{"crypto0", 12},
	{"crypto1", 13},

	{"emem0", 24},
	{"emem1", 25},
	{"emem2", 26},

	{"imem0", 28},
	{"imem1", 29},

	{"mei0", 32},
	{"mei1", 33},
	{"mei2", 34},
	{"mei3", 35},
	{"mei4", 36},
	{"mei5", 37},
	{"mei6", 38},

	{"ila0", 48},
	{"ila1", 49},

	{NULL, -1}
};

int nfp3200_idstr2meid(const char *s, const char **endptr)
{
	int cl;
	int menum;
	const char *c;

	if (endptr)
		*endptr = s;

	if (!s)
		return -1;

	cl = nfp3200_idstr2cluster(s, &c);
	if ((cl == -1) || (*c == '\0'))
		return -1;

	menum = nfp3200_idstr2menum(c, &c);
	if (menum == -1)
		return -1;

	if (endptr)
		*endptr = c;

	return NFP3200_MEID(cl, menum);
}

int nfp3200_idstr2cluster(const char *s, const char **endptr)
{
	unsigned long val;
	char *c;

	if (endptr)
		*endptr = s;

	if (!s)
		return -1;

	if (strncmp(s, nri_str_cl, nri_str_cl_len) != 0)
		return -1;
	s += nri_str_cl_len;
	val = strtoul(s, &c, 10);
	if (s == c)
		return -1;
	if (*c == '.')
		c++;
	else if (*c != '\0')
		return -1;

	if (val >= NRI_NFP3200_CLUSTER_MAXCNT)
		return -1;

	if (endptr)
		*endptr = c;

	return (int)val;
}

int nfp3200_idstr2menum(const char *s, const char **endptr)
{
	unsigned long val;
	char *c;

	if (endptr)
		*endptr = s;

	if (!s)
		return -1;

	if (strncmp(s, nri_str_me, nri_str_me_len) != 0)
		return -1;
	s += nri_str_me_len;
	val = strtoul(s, &c, 10);
	if (*c == '.')
		c++;
	else if (*c != '\0')
		return -1;

	if (val >= NRI_NFP3200_ME_MAXCNT)
		return -1;

	if (endptr)
		*endptr = c;

	return (int)val;
}

int nfp3200_idstr2ctxnum(const char *s, const char **endptr)
{
	unsigned long val;
	char *c;

	if (endptr)
		*endptr = s;

	if (!s)
		return -1;

	if (strncmp(s, nri_str_ctx, nri_str_ctx_len) != 0)
		return -1;
	s += nri_str_ctx_len;
	val = strtoul(s, &c, 10);
	if (*c == '.')
		c++;
	else if (*c != '\0')
		return -1;

	if (val >= NRI_NFP3200_CTX_MAXCNT)
		return -1;

	if (endptr)
		*endptr = c;

	return (int)val;
}

int nfp3200_idstr2megrp(const char *s, const char **endptr)
{
	unsigned long val;
	char *c;

	if (endptr)
		*endptr = s;

	if (!s)
		return -1;

	if (strncmp(s, nri_str_tg, nri_str_tg_len) != 0)
		return -1;
	s += nri_str_tg_len;
	val = strtoul(s, &c, 10);
	if (*c == '.')
		c++;
	else if (*c != '\0')
		return -1;

	if (val >= (NRI_NFP3200_ME_MAXCNT / 2))
		return -1;

	if (endptr)
		*endptr = c;

	return (int)val;
}

const char *nfp3200_meid2str(char *s, int meid)
{
	if ((!s) || (meid < 0))
		return NULL;

	snprintf(s, NFP_MEID_STR_SZ, "%s%d.%s%d", nri_str_cl, NFP3200_MEID_CLUSTER_of(meid),
		 nri_str_me, NFP3200_MEID_MENUM_of(meid));

	return s;
}

const char *nfp3200_cluster2str(char *s, int cluster_num)
{
	if ((!s) || (cluster_num < 0))
		return NULL;

	snprintf(s, NFP_MEID_STR_SZ, "%s%d", nri_str_cl, cluster_num);

	return s;
}

const char *nfp3200_menum2str(char *s, int menum)
{
	if ((!s) || (menum < 0))
		return NULL;

	snprintf(s, NFP_MEID_STR_SZ, "%s%d", nri_str_me, menum);

	return s;
}

const char *nfp3200_ctxnum2str(char *s, int ctxnum)
{
	if ((!s) || (ctxnum < 0))
		return NULL;

	snprintf(s, NFP_MEID_STR_SZ, "%s%d", nri_str_ctx, ctxnum);

	return s;
}

const char *nfp3200_megrp2str(char *s, int megrp)
{
	if ((!s) || (megrp < 0))
		return NULL;

	snprintf(s, NFP_MEID_STR_SZ, "%s%d", nri_str_tg, megrp);

	return s;
}

int nfp6000_idstr2meid(const char *s, const char **endptr)
{
	int iid;
	int menum;
	const char *c;

	if (endptr)
		*endptr = s;

	if (!s)
		return -1;

	iid = nfp6000_idstr2island(s, &c);
	if ((iid == -1) || (*c == '\0'))
		return -1;

	menum = nfp6000_idstr2menum(c, &c);
	if (menum == -1)
		return -1;

	if (endptr)
		*endptr = c;

	return NFP6000_MEID(iid, menum);
}

int nfp6000_idstr2island(const char *s, const char **endptr)
{
	unsigned long val;
	char *c;
	struct nri_iid_alias *alias;
	size_t slen;

	if (endptr)
		*endptr = s;

	if (!s)
		return -1;

	slen = strlen(s);

	/* First check for alias matches. If none, assume iX[.meY] format */
	for (alias = nri_nfp6000_iid_alias_map; alias->alias; alias++) {
		size_t alen = strlen(alias->alias);

		if (slen < alen)
			continue;

		if (strncmp(s, alias->alias, alen) == 0) {
			if ((s[alen] == '.') || (s[alen] == '\0')) {
				if (s[alen] == '.')
					alen++;

				if (endptr)
					*endptr = &s[alen];
				return alias->iid;
			}
		}
	}

	if (strncmp(s, nri_str_iid, nri_str_iid_len) != 0)
		return -1;
	s += nri_str_iid_len;
	val = strtoul(s, &c, 10);
	if (*c == '.')
		c++;
	else if (*c != '\0')
		return -1;

	if (val >= NRI_NFP6000_ISLAND_MAXCNT)
		return -1;

	if (endptr)
		*endptr = c;

	return (int)val;
}

int nfp6000_idstr2menum(const char *s, const char **endptr)
{
	unsigned long val;
	char *c;

	if (endptr)
		*endptr = s;

	if (!s)
		return -1;

	if (strncmp(s, nri_str_me, nri_str_me_len) != 0)
		return -1;
	s += nri_str_me_len;
	val = strtoul(s, &c, 10);
	if (*c == '.')
		c++;
	else if (*c != '\0')
		return -1;

	if (val >= NRI_NFP6000_ME_MAXCNT)
		return -1;

	if (endptr)
		*endptr = c;

	return (int)val;
}

int nfp6000_idstr2ctxnum(const char *s, const char **endptr)
{
	return nfp3200_idstr2ctxnum(s, endptr);
}

int nfp6000_idstr2megrp(const char *s, const char **endptr)
{
	unsigned long val;
	char *c;

	if (endptr)
		*endptr = s;

	if (!s)
		return -1;

	if (strncmp(s, nri_str_tg, nri_str_tg_len) != 0)
		return -1;
	s += nri_str_tg_len;
	val = strtoul(s, &c, 10);
	if (*c == '.')
		c++;
	else if (*c != '\0')
		return -1;

	if (val >= (NRI_NFP6000_ME_MAXCNT / 2))
		return -1;

	if (endptr)
		*endptr = c;

	return (int)val;
}

const char *nfp6000_meid2str(char *s, int meid)
{
	if ((!s) || (meid < 0))
		return NULL;

	if (NFP6000_MEID_MENUM_of(meid) < 0)
		snprintf(s, NFP_MEID_STR_SZ, "%s%d", nri_str_iid, NFP6000_MEID_ISLAND_of(meid));
	else
		snprintf(s, NFP_MEID_STR_SZ, "%s%d.%s%d", nri_str_iid, NFP6000_MEID_ISLAND_of(meid),
			 nri_str_me, NFP6000_MEID_MENUM_of(meid));

	return s;
}

const char *nfp6000_meid2altstr(char *s, int meid)
{
	struct nri_iid_alias *alias;

	if ((!s) || (meid < 0))
		return NULL;

	/* First check for alias matches.  If none, use iX[.meY] format */
	for (alias = nri_nfp6000_iid_alias_map; alias->alias; alias++) {
		if (NFP6000_MEID_ISLAND_of(meid) == alias->iid)
			break;
	}

	if (alias->alias) {
		if (NFP6000_MEID_MENUM_of(meid) < 0)
			snprintf(s, NFP_MEID_STR_SZ, "%s", alias->alias);
		else
			snprintf(s, NFP_MEID_STR_SZ, "%s.%s%d", alias->alias, nri_str_me,
				 NFP6000_MEID_MENUM_of(meid));
	} else {
		return nfp6000_meid2str(s, meid);
	}

	return s;
}

const char *nfp6000_island2str(char *s, int island_id)
{
	if ((!s) || (island_id < 0))
		return NULL;

	snprintf(s, NFP_MEID_STR_SZ, "%s%d", nri_str_iid, island_id);

	return s;
}

const char *nfp6000_island2altstr(char *s, int island_id)
{
	struct nri_iid_alias *alias;

	if ((!s) || (island_id < 0))
		return NULL;

	for (alias = nri_nfp6000_iid_alias_map; alias->alias; alias++) {
		if (island_id == alias->iid)
			break;
	}

	if (alias->alias)
		snprintf(s, NFP_MEID_STR_SZ, "%s", alias->alias);
	else
		return nfp6000_island2str(s, island_id);

	return s;
}

const char *nfp6000_menum2str(char *s, int menum)
{
	return nfp3200_menum2str(s, menum);
}

const char *nfp6000_ctxnum2str(char *s, int ctxnum)
{
	return nfp3200_ctxnum2str(s, ctxnum);
}

const char *nfp6000_megrp2str(char *s, int menum)
{
	return nfp3200_megrp2str(s, menum);
}

int nfp_idstr2meid(int chip_family, const char *s, const char **endptr)
{
	if (endptr)
		*endptr = NULL;
	if (NFP_FAMILY_IS_ARCH_TH(chip_family))
		return nfp6000_idstr2meid(s, endptr);
	else if (NFP_FAMILY_IS_ARCH_YD(chip_family))
		return nfp3200_idstr2meid(s, endptr);
	else
		return -1;
}

int nfp_idstr2island(int chip_family, const char *s, const char **endptr)
{
	if (endptr)
		*endptr = NULL;
	if (NFP_FAMILY_IS_ARCH_TH(chip_family))
		return nfp6000_idstr2island(s, endptr);
	else if (NFP_FAMILY_IS_ARCH_YD(chip_family))
		return nfp3200_idstr2cluster(s, endptr);
	else
		return -1;
}

int nfp_idstr2megrp(int chip_family, const char *s, const char **endptr)
{
	if (endptr)
		*endptr = NULL;
	if (NFP_FAMILY_IS_NFP6000(chip_family))
		return nfp6000_idstr2megrp(s, endptr);
	else if (NFP_FAMILY_IS_NFP3200(chip_family))
		return nfp3200_idstr2megrp(s, endptr);
	else
		return -1;
}

int nfp_idstr2menum(int chip_family, const char *s, const char **endptr)
{
	if (endptr)
		*endptr = NULL;
	if (NFP_FAMILY_IS_ARCH_TH(chip_family))
		return nfp6000_idstr2menum(s, endptr);
	else if (NFP_FAMILY_IS_ARCH_YD(chip_family))
		return nfp3200_idstr2menum(s, endptr);
	else
		return -1;
}

int nfp_idstr2ctxnum(int chip_family, const char *s, const char **endptr)
{
	if (endptr)
		*endptr = NULL;
	if (NFP_FAMILY_IS_ARCH_TH(chip_family))
		return nfp6000_idstr2ctxnum(s, endptr);
	else if (NFP_FAMILY_IS_ARCH_YD(chip_family))
		return nfp3200_idstr2ctxnum(s, endptr);
	else
		return -1;
}

const char *nfp_meid2str(int chip_family, char *s, int meid)
{
	if (NFP_FAMILY_IS_ARCH_TH(chip_family))
		return nfp6000_meid2str(s, meid);
	else if (NFP_FAMILY_IS_ARCH_YD(chip_family))
		return nfp3200_meid2str(s, meid);
	else
		return NULL;
}

const char *nfp_meid2altstr(int chip_family, char *s, int meid)
{
	if (NFP_FAMILY_IS_ARCH_TH(chip_family))
		return nfp6000_meid2altstr(s, meid);
	else if (NFP_FAMILY_IS_ARCH_YD(chip_family))
		return nfp3200_meid2str(s, meid);
	else
		return NULL;
}

const char *nfp_island2str(int chip_family, char *s, int island_id)
{
	if (NFP_FAMILY_IS_ARCH_TH(chip_family))
		return nfp6000_island2str(s, island_id);
	else if (NFP_FAMILY_IS_ARCH_YD(chip_family))
		return nfp3200_cluster2str(s, island_id);
	else
		return NULL;
}

const char *nfp_island2altstr(int chip_family, char *s, int island_id)
{
	if (NFP_FAMILY_IS_ARCH_TH(chip_family))
		return nfp6000_island2altstr(s, island_id);
	else if (NFP_FAMILY_IS_ARCH_YD(chip_family))
		return nfp3200_cluster2str(s, island_id);
	else
		return NULL;
}

const char *nfp_megrp2str(int chip_family, char *s, int megrp)
{
	if (NFP_FAMILY_IS_NFP6000(chip_family))
		return nfp6000_megrp2str(s, megrp);
	else if (NFP_FAMILY_IS_NFP3200(chip_family))
		return nfp3200_megrp2str(s, megrp);
	else
		return NULL;
}

const char *nfp_menum2str(int chip_family, char *s, int menum)
{
	if (NFP_FAMILY_IS_ARCH_TH(chip_family))
		return nfp6000_menum2str(s, menum);
	else if (NFP_FAMILY_IS_ARCH_YD(chip_family))
		return nfp3200_menum2str(s, menum);
	else
		return NULL;
}

const char *nfp_ctxnum2str(int chip_family, char *s, int ctxnum)
{
	if (NFP_FAMILY_IS_ARCH_TH(chip_family))
		return nfp6000_ctxnum2str(s, ctxnum);
	else if (NFP_FAMILY_IS_ARCH_YD(chip_family))
		return nfp3200_ctxnum2str(s, ctxnum);
	else
		return NULL;
}

int nfp_idstr2rev(const char *s)
{
	int v = 0;
	int c;

	if ((!s) || (strlen(s) < 2))
		return -1;

	c = s[0];
	if ((c >= 'A') && (c <= 'P'))
		c = (c - 'A');
	else if ((c >= 'a') && (c <= 'p'))
		c = (c - 'a');
	else
		return -1;

	v = (c << 4);

	c = s[1];
	if ((c >= '0') && (c <= '9'))
		c = c - '0';
	else if ((c >= 'A') && (c <= 'F'))
		c = (c - 'A') + 0xA;
	else if ((c >= 'a') && (c <= 'f'))
		c = (c - 'a') + 0xA;
	else
		return -1;

	v |= c;
	return v;
}

const char *nfp_rev2str(char *s, int rev)
{
	int c;

	c = (rev >> 4) & 0xF;
	s[0] = (char)('A' + c);
	c = rev & 0xF;
	if (c < 0xA)
		s[1] = (char)('0' + c);
	else
		s[1] = (char)('A' + (c - 0xA));

	s[2] = '\0';
	return s;
}

struct cpp_target_info {
	int id;
	const char *name;
	const char *comment;
	int deprecated;
};

const struct cpp_target_info nfp3200_cpp_targets[] = {
	{
		/* .id = */ NFP3200_CPPTGT_MSF0, /* .name = */ "msf0",
		/* .comment = */ "MSF0 (SPI4.2/IL/XAUI/SGMII)",
		/* .deprecated = */
		0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_QDR, /* .name = */ "sram",
		/* .comment = */ "QDR SRAM or VQDR", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_QDR, /* .name = */ "qdr",
		/* .comment = */ "QDR SRAM or VQDR", /* .deprecated = */ 1,
	},
	{
		/* .id = */ NFP3200_CPPTGT_MSF1, /* .name = */ "msf1",
		/* .comment = */ "MSF1 (IL/XAUI/SGMII)", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_HASH, /* .name = */ "hash",
		/* .comment = */ "Hash", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_MU, /* .name = */ "mem",
		/* .comment = */ "Memory Unit or DDR3 DRAM", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_MU, /* .name = */ "dram",
		/* .comment = */ "Memory Unit or DDR3 DRAM", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_MU, /* .name = */ "ddr",
		/* .comment = */ "Memory Unit or DDR3 DRAM", /* .deprecated = */ 1,
	},
	{
		/* .id = */ NFP3200_CPPTGT_GS, /* .name = */ "scratch",
		/* .comment = */ "Global Scratch", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_GS, /* .name = */ "gs",
		/* .comment = */ "Global Scratch", /* .deprecated = */ 1,
	},
	{
		/* .id = */ NFP3200_CPPTGT_PCIE, /* .name = */ "pcie",
		/* .comment = */ "PCIE", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_ARM, /* .name = */ "arm",
		/* .comment = */ "ARM Gasket", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_ARM, /* .name = */ "as",
		/* .comment = */ "ARM Gasket Scratch", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_CRYPTO, /* .name = */ "crypto",
		/* .comment = */ "Crypto", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_CAP, /* .name = */ "cap",
		/* .comment = */ "CAP", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_CT, /* .name = */ "ct",
		/* .comment = */ "Cluster Target", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_CT, /* .name = */ "cluster",
		/* .comment = */ "Cluster Target", /* .deprecated = */ 1,
	},
	{
		/* .id = */ NFP3200_CPPTGT_CLS, /* .name = */ "cls",
		/* .comment = */ "Cluster Local Scratch", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP3200_CPPTGT_CLS, /* .name = */ "ls",
		/* .comment = */ "Cluster Local Scratch", /* .deprecated = */ 1,
	},
	{
		/* .id = */ -1, /* .name = */ NULL,
		/* .comment = */ NULL, /* .deprecated = */ 0,
	},
};

const struct cpp_target_info nfp6000_cpp_targets[] = {
	{
		/* .id = */ NFP6000_CPPTGT_NBI, /* .name = */ "nbi",
		/* .comment = */ "NBI", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP6000_CPPTGT_VQDR, /* .name = */ "sram",
		/* .comment = */ "VQDR", /* .deprecated = */ 1,
	},
	{
		/* .id = */ NFP6000_CPPTGT_ILA, /* .name = */ "ila",
		/* .comment = */ "ILA", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP6000_CPPTGT_MU, /* .name = */ "mu",
		/* .comment = */ "Memory Unit", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP6000_CPPTGT_PCIE, /* .name = */ "pcie",
		/* .comment = */ "PCIE", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP6000_CPPTGT_ARM, /* .name = */ "arm",
		/* .comment = */ "ARM", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP6000_CPPTGT_CRYPTO, /* .name = */ "crypto",
		/* .comment = */ "Crypto", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP6000_CPPTGT_CTXPB, /* .name = */ "ctxpb",
		/* .comment = */ "Cluster Target / XPB", /* .deprecated = */ 0,
	},
	{
		/* .id = */ NFP6000_CPPTGT_CLS, /* .name = */ "cls",
		/* .comment = */ "Cluster Local Scratch", /* .deprecated = */ 0,
	},
	{
		/* .id = */ -1, /* .name = */ NULL,
		/* .comment = */ NULL, /* .deprecated = */ 0,
	},
};

int nfp_str2cpp(int chip_family, const char *tid, uint32_t *cpp_idp, uint64_t *cpp_addrp)
{
	uint32_t dest;
	unsigned long long offset;
	uint64_t tmp[4];
	char *cp, *ncp;
	int island, target, action, token;
	int i;
	const struct cpp_target_info *cpptgts = NULL;

	if (NFP_FAMILY_IS_ARCH_YD(chip_family))
		cpptgts = nfp3200_cpp_targets;
	else if (NFP_FAMILY_IS_ARCH_TH(chip_family))
		cpptgts = nfp6000_cpp_targets;
	else
		return NFP_ERRNO(ENODEV);

	offset = 0;

	cp = strchr(tid, '@');
	if (cp) {
		island = strtoul(tid, &cp, 0);
		if (!cp || *cp != '@' || (island <= 0 || island >= 64))
			return NFP_ERRNO(EINVAL);

		tid = (cp + 1);
	} else {
		island = 0;
	}

	tmp[0] = nfp_strtou64(tid, &cp, 0);
	if (cp == tid) {
		/* Look though the target list */
		for (i = 0; cpptgts[i].name; i++) {
			size_t len = strlen(cpptgts[i].name);

			if (tid[len] != ':' && tid[len] != 0)
				continue;
			if (strncmp(tid, cpptgts[i].name, len) == 0) {
				tmp[0] = cpptgts[i].id;
				cp = (char *)&tid[len];
				break;
			}
		}

		if (!cpptgts[i].name)
			return NFP_ERRNO(EINVAL);
	}

	for (i = 1; (*cp != 0) && (i < 4); i++, cp = ncp) {
		if (*(cp++) != ':')
			return NFP_ERRNO(EINVAL);

		tmp[i] = nfp_strtou64(cp, &ncp, 0);
		if (cp == ncp)
			return NFP_ERRNO(EINVAL);
	}

	if (*cp != 0)
		return NFP_ERRNO(EINVAL);

	switch (i) {
	case 1:
		target = (int)tmp[0];
		action = NFP_CPP_ACTION_RW;
		token = 0;
		offset = 0;
		break;
	case 2:
		target = (int)tmp[0];
		action = NFP_CPP_ACTION_RW;
		token = 0;
		offset = tmp[1];
		break;
	case 3:
		target = (int)tmp[0];
		action = (int)tmp[1];
		token = 0;
		offset = tmp[2];
		break;
	case 4:
		target = (int)tmp[0];
		action = (int)tmp[1];
		token = (int)tmp[2];
		offset = tmp[3];
		break;
	default:
		return NFP_ERRNO(EINVAL);
	}

	if ((target >= 16) || (token >= 4) || (action >= 32 && action != NFP_CPP_ACTION_RW) ||
	    (offset >= (1ULL << 40))) {
		return NFP_ERRNO(EINVAL);
	}

	dest = NFP_CPP_ISLAND_ID(target, action, token, island);

	*cpp_idp = dest;
	*cpp_addrp = offset;

	return 0;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
