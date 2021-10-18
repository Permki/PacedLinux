/*
 * Copyright (C) 2012-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * NFP Register data and lookup API. While types and enums are used
 * elsewhere for all NFP families, only NFP-6xxx chips are currently
 * supported by the register data.
 *
 */

#ifndef __NFP_REGDATA_H__
#define __NFP_REGDATA_H__

#include <stdint.h>
#include <string.h>
#include <nfp-common/nfp_platform.h>
#include <nfp-common/nfp_resid.h>
#include <nfp-common/nfp_chipdata.h>

#include "_nfp_common_apiex.h"

__BEGIN_DECLS

/* API flags */
/* For lookups, ignore the case of normal names */
#define NFP_REGDATA_FLG_IGN_CASE_FULLNAME BIT(0)
/* For lookups, ignore the case of altnames, which is upper case */
#define NFP_REGDATA_FLG_IGN_CASE_ALTNAME BIT(1)
/* For matches, don't include normal names */
#define NFP_REGDATA_MATCH_FLG_SKIP_FULLNAME BIT(2)
/* For matches, don't include altnames */
#define NFP_REGDATA_MATCH_FLG_SKIP_ALTNAME BIT(3)
/* For matches, don't include bit fields of a matched register */
#define NFP_REGDATA_MATCH_FLG_SKIP_BITFIELDS BIT(4)
/* For lookups, don't allow local-scope lookups.
 * This is mainly intended for use in the SDK toolchain
 * where local-island lookups cannot yet resolve to
 * an absolute island
 */
#define NFP_REGDATA_FLG_FQN_ONLY BIT(5)
/* For lookups, set nfp_regdata_node.name */
#define NFP_REGDATA_FLG_GET_FULLNAME BIT(6)
/* For lookups, set nfp_regdata_node.altname */
#define NFP_REGDATA_FLG_GET_ALTNAME BIT(7)
/* For lookups, set nfp_regdata_node.description */
#define NFP_REGDATA_FLG_GET_DESC BIT(8)
/* For matches, return all repeating entries. This can be
 * thousands of nodes for some maps.
 */
#define NFP_REGDATA_MATCH_FLG_REPEATS_ALL BIT(9)
/* TODO: print "cls:Rings.RingBase[0-15]"    */
#define NFP_REGDATA_MATCH_FLG_REPEATS_RANGED BIT(10)
/* Allow partial matching of last part of lookup string.
 * Not implemented, not necessary yet.
 */
#define NFP_REGDATA_MATCH_FLG_PARTIAL BIT(11)
/* For lookups, set nfp_regdata_node.fqn */
#define NFP_REGDATA_FLG_GET_FQN BIT(12)
/* For matches, don't return matches of the next level,
 * only match up to the given string. This is useful
 * for expanding {} specifiers.
 */
#define NFP_REGDATA_MATCH_FLG_NO_DESCEND BIT(13)
/* For lookups, don't require an exact match of the registers.
 * Used for looking up secret reg maps by offset
 */
#define NFP_REGDATA_FLG_OFFSET_LOOKUP BIT(14)
/* For lookups, set all string fields. */
#define NFP_REGDATA_FLG_GET_NAMES                                     \
	(NFP_REGDATA_FLG_GET_FULLNAME | NFP_REGDATA_FLG_GET_ALTNAME | \
	 NFP_REGDATA_FLG_GET_DESC)
/* CTX_ANY means a CSR is an ME indirect CSR, but without identifying
 * any specific context's CSR.
 */
#define NFP_REGDATA_CTX_ANY -2
/* CTX_NA means a CSR is an ME CSR, but not an indirect CSR. */
#define NFP_REGDATA_CTX_NA -1
#define NFP_REGDATA_CTX_0 0
#define NFP_REGDATA_CTX_1 1
#define NFP_REGDATA_CTX_2 2
#define NFP_REGDATA_CTX_3 3
#define NFP_REGDATA_CTX_4 4
#define NFP_REGDATA_CTX_5 5
#define NFP_REGDATA_CTX_6 6
#define NFP_REGDATA_CTX_7 7
#define NFP_REGDATA_IID_NA -1
#define NFP_REGDATA_IID_LOCAL 0
#define NFP_REGDATA_MENUM_LOCAL -2
#define NFP_REGDATA_MENUM_NA -1
#define NFP_REGDATA_DISCOVER_RET_STOP 0
#define NFP_REGDATA_DISCOVER_RET_ABORT 1
#define NFP_REGDATA_DISCOVER_RET_OWN 2
#define NFP_REGDATA_DISCOVER_RET_FREE 3
#define NFP_REGDATA_MECSR_LATENCY_NA (INT8_MIN)
#define NFP_REGDATA_FILE_NAME_TOP "top.json"
#define NFP_REGDATA_PATH_ENVVAR "NFP_REGDATA_PATH"

enum nfp_regfield_access {
	NFP_REGFIELD_ACCESS_NONE = 0,
	/* Read Only, Read value then Clear */
	NFP_REGFIELD_ACCESS_RC = 1,
	/* Read value then Clear, Write */
	NFP_REGFIELD_ACCESS_RCW = 2,
	/* Read Only */
	NFP_REGFIELD_ACCESS_RO = 3,
	/* Read, Write */
	NFP_REGFIELD_ACCESS_RW = 4,
	/* Read, Write 0 to Clear */
	NFP_REGFIELD_ACCESS_RW0C = 5,
	/* Read, Write 1 to Clear */
	NFP_REGFIELD_ACCESS_RW1C = 6,
	/* Read, Write 1 to Set */
	NFP_REGFIELD_ACCESS_RW1S = 7,
	/* Read, Write any value to Clear */
	NFP_REGFIELD_ACCESS_RWC = 8,
	/* Read, Write any value to Latch internal value */
	NFP_REGFIELD_ACCESS_RWL = 9,
	/* Write Only, Write 0 to Clear */
	NFP_REGFIELD_ACCESS_W0C = 10,
	/* Write Only, Write 1 to Clear */
	NFP_REGFIELD_ACCESS_W1C = 11,
	/* Write Only, Write any value to Clear */
	NFP_REGFIELD_ACCESS_WC = 12,
	/* Write Only */
	NFP_REGFIELD_ACCESS_WO = 13
};

enum nfp_reg_type {
	NFP_REG_TYPE_NONE = 0,
	NFP_REG_TYPE_CPP = 1,
	NFP_REG_TYPE_MECSR = 2,
	NFP_REG_TYPE_MEREG = 3,
	NFP_REG_TYPE_XPB = 4
};

enum nfp_mereg_type {
	/* Based on original nfp.h enum */
	NFP_MEREG_TYPE_NONE = 0x7F,
	NFP_MEREG_TYPE_GPRA = 0,
	NFP_MEREG_TYPE_GPRB = 1,
	NFP_MEREG_TYPE_XFER_IN = 2,
	NFP_MEREG_TYPE_XFER_OUT = 3,
	NFP_MEREG_TYPE_NN = 4
};

enum nfp_regdata_node_type {
	NFP_REGDATA_NODE_TYPE_NONE = 0,
	NFP_REGDATA_NODE_TYPE_TARGET = 1,
	NFP_REGDATA_NODE_TYPE_ADDRMAP = 2,
	NFP_REGDATA_NODE_TYPE_MULTIREG = 3,
	NFP_REGDATA_NODE_TYPE_REG = 4,
	NFP_REGDATA_NODE_TYPE_BITF = 5
};

/* Opaque data handle */
struct nfp_regdata;

struct nfp_reg_bitf {
	const char *name;
	const char *description;
	const char *altname;
	uint32_t msb;
	uint32_t lsb;
	enum nfp_regfield_access access_mode;
};

struct nfp_reg_id {
	int16_t iid;
	int8_t ctx;
	uint16_t bit_len;
	int8_t cpp_tgt;
	int8_t cpp_rd_action;
	int8_t cpp_rd_token;
	int8_t cpp_wr_action;
	int8_t cpp_wr_token;
	uint64_t cpp_offset;
};

/** This is general purpose lookup information that provides
 * all information available on a lookup string. It can describe
 * a top level lookup target, an address map, a register or
 * a bit field of a register.
 */
struct nfp_regdata_node {
	/* Fully qualified name is tgt:map.map.reg */
	char *fqn;

	/* NFP_REGDATA_FLG_GET_* enables these strings and the caller must free
	 * with nfp_regdata_node_free
	 */
	char *name;
	char *altname;
	char *description;

	/* This is always the write-offset, which is almost always the same as
	 * the read offset.
	 */
	uint64_t offset;

	/* The first and last byte offsets occupied by registers in a map or
	 * its nested maps. In most cases this will be 0 to the last byte of
	 * the last register in a memory-like map and thus the size of such
	 * a map can be obtained by (reg_byte_last - reg_byte_first + 1).
	 * Note that 'reg_byte_first' and 'offset' may not be the same if
	 * the register in the map do not start from 0.
	 */
	uint64_t reg_byte_first;
	uint64_t reg_byte_last;
	int offset_iid_lsb; /* -1 if iid is not part of offset */
	int offset_me_lsb;  /* -1 is microengine number is not part of offset */

	/* NFP_REGDATA_IID_NA if not applicable, island ID if lookup referred to
	 * specific island. If set, IID may still not be part of offset. Check
	 * offset_iid_lsb. Will be NFP_REGDATA_IID_LOCAL if the lookup was for
	 * a local island (i0).
	 */
	int iid;

	/* NFP_REGDATA_MENUM_NA if not applicable, menum within island otherwise.
	 * Will be NFP_REGDATA_MENUM_LOCAL if the lookup was for a local ME.
	 */
	int menum;

	/* NFP_REGDATA_CTX_NA if not applicable */
	int ctx;

	int8_t cpp_tgt; /* -1 if not applicable */
	int8_t cpp_rd_action;
	int8_t cpp_rd_token;
	int8_t cpp_wr_action;
	int8_t cpp_wr_token;
	uint64_t cpp_rd_offset;

	enum nfp_regdata_node_type type;

	/* Set to register bit size if type is a regiser or bitfield. If type is
	 * a map and all the map entries are registers with the same bit size,
	 * this will also be set to that bit size.
	 */
	uint32_t bit_len;

	/* These are valid if type is bitfield or register. */
	/* Register level access info, derived from all bit fields. */
	uint8_t is_readable;
	uint8_t is_writable;
	uint8_t is_sim_backdoorable;
	size_t field_cnt;
	const struct nfp_reg_bitf *fields;
	/* These are valid if type is bitfield. */
	const struct nfp_reg_bitf *matched_field;

	enum nfp_reg_type reg_type;
	union {
		struct {
			int dummy;
		} cpp;
		struct {
			int8_t latency_rd;  /* NFP_REGDATA_MECSR_LATENCY_NA for NA */
			int8_t latency_wr;  /* NFP_REGDATA_MECSR_LATENCY_NA for NA */
			int8_t latency_use; /* NFP_REGDATA_MECSR_LATENCY_NA for NA */
			uint16_t offset;    /* CSR offset with ME CSR map. */
		} mecsr;
		struct {
			enum nfp_mereg_type mereg_type;
			int reg_idx; /* Register number in register bank */
		} mereg;
	} data;
};

/** Load register data.
 *
 * @param chip          A ChipData chip handle.
 * @param revision      A single revision of the chip to load register maps
 *                      for.
 * @param regdata_path  If NULL, embedded data will be used. If non-NULL,
 *                      a valid JSON register map directory should exist
 *                      at this path and will be loaded instead.
 * @return              An opaque handle to the loaded register data, NULL
 *                      on error.
 */
NFP_COMMON_API
struct nfp_regdata *nfp_regdata_load(const struct nfp_chipdata_chip *chip,
				     int revision,
				     const char *regdata_path);

/** Get information about the loaded data.
 *
 * This function exists purely for diagnostic purposes. The returned string
 * is a concatenation of all lines in the '_comment' array in top.json,
 * separated and ended by '\n'.
 *
 * @param regdata       Handle to register data as returned by
 *                      @c nfp_regdata_load
 * @return              NULL on error, empty string if there is no comment
 *                      or a comment string.
 */
NFP_COMMON_API
const char *nfp_regdata_comment(struct nfp_regdata *regdata);

/** Free previously loaded register data.
 *
 * @param regdata       Handle to register data as returned by
 *                      @c nfp_regdata_load
 */
NFP_COMMON_API
void nfp_regdata_free(struct nfp_regdata *regdata);

/** Free the strings and other data in a node, if allocated.
 *
 * While a no function in this API allocates a new struct nfp_regdata_node,
 * some functions do allocate new data for things like names in the node.
 * While the caller may choose to call free() on those, this function
 * provides a convenient way to free the data.
 */
NFP_COMMON_API
void nfp_regdata_node_free_data(struct nfp_regdata_node *node);

/** Get the NFP family of the register data.
 *
 * @param regdata       Handle to register data
 * @return Chip family.
 */
NFP_COMMON_API
int nfp_regdata_family(struct nfp_regdata *regdata);

/** Perform a hierarchical lookup for a register, bit field or address map.
 *
 * The format of @p name, the lookup string, is:
 * "tgt:map.map.reg.field".
 *
 * If the lookup fails, the content of @p node is undefined.
 *
 * @param regdata       Handle to register data
 * @param name          The lookup string, which is the fully qualified name
 *                      to a target, map, register or bit field.
 * @param flags         Any combination of NFP_REGDATA_FLG_* flags.
 * @param node          Pointer to a struct nfp_regdata_node variable which
 *                      will be populated with lookup results.
 * @return              0 on success, -1 on error.
 */
NFP_COMMON_API
int nfp_regdata_lookup(struct nfp_regdata *regdata,
		       const char *name,
		       unsigned int flags,
		       struct nfp_regdata_node *node);

/** A local-scope lookup similar to @c nfp_regdata_lookup
 *
 * This function is mainly used by the SDK toolchain to lookup
 * nodes that are not yet fully resolved, such as local to the island
 * or local to the microengine.
 *
 * @param regdata       Handle to register data
 * @param name          The lookup string, which is the fully qualified name
 *                      to a target, map, register or bit field.
 * @param flags         Any combination of NFP_REGDATA_FLG_* flags.
 * @param node          Pointer to a struct nfp_regdata_node variable which
 *                      will be populated with lookup results.
 * @param local_iid     Local Island ID. When larger than 0, this is used to
 *                      fully resolve a lookup for the given island with
 *                      a lookup string that is local-scoped.
 *                      When 0 or negative, the lookup node will not point
 *                      to a fully resolved node, but to a local-scoped node.
 * @param local_menum   Similar to @p local_iid, but for local microengine
 *                      scope.
 * @return              0 on success, -1 on error.
 */
NFP_COMMON_API
int nfp_regdata_lookup_local(struct nfp_regdata *regdata,
			     const char *name,
			     unsigned int flags,
			     struct nfp_regdata_node *node,
			     int local_iid,
			     int local_menum);

/** Traverse down into address maps to discover register nodes.
 *
 * @param regdata       Handle to register data
 * @param name          The lookup string, which is the fully qualified name
 *                      to a target or map. If NULL or empty, all targets
 *                      will be traversed.
 * @param flags         Same flags as used by @c nfp_regdata_lookup
 * @param usrdata       A pointer to any user data which is simply passed
 *                      to the callback function.
 * @param cb_found_reg  Callback function which is called for each discovered
 *                      register. The callback should return one of:
 *                      - NFP_REGDATA_DISCOVER_RET_STOP - Stop traversal,
 *                        @c nfp_regdata_discover will return with success.
 *                      - NFP_REGDATA_DISCOVER_RET_ABORT - Stop traversal,
 *                        @c nfp_regdata_discover will return with error.
 *                      - NFP_REGDATA_DISCOVER_RET_OWN - The callback took
 *                        ownership of the data in @p cb_found_reg.node and
 *                        @c nfp_regdata_discover should not free it. Note that
 *                        the @p cb_found_reg.node variable cannot be owned
 *                        by the callback.
 *                      - NFP_REGDATA_DISCOVER_RET_FREE - The callback did
 *                        not take ownership of the data in
 *                        @p cb_found_reg.node and @c nfp_regdata_discover will
 *                        free it.
 * @param cb_found_reg.regdata  Same as @p regdata
 * @param cb_found_reg.node     A pointer to a variable owned by
 *                              @c nfp_regdata_discover, the callback function
 *                              should not free @p node.
 * @param cb_found_reg.usrdata  Same as @p usrdata
 * @return 0 on success, -1 on error.
 */
NFP_COMMON_API
int nfp_regdata_discover(struct nfp_regdata *regdata,
			 const char *name,
			 unsigned int flags,
			 void *usrdata,
			 int (*cb_found_reg)(struct nfp_regdata *regdata,
					     struct nfp_regdata_node *node,
					     void *usrdata));

/** Obtain a list of strings that match the next level of the lookup string.
 *
 * @param regdata       Handle to register data
 * @param name          The lookup string. If NULL or empty, all targets
 *                      will be traversed.
 * @param matches       A strtab of matched strings - strings are separated
 *                      by '\0'. The caller must free() this strtab as a whole.
 * @param flags         A combination of NFP_REGDATA_MATCH_FLG_* flags.
 * @return              The number of strings in @p matches
 */
NFP_COMMON_API
ssize_t nfp_regdata_matches(struct nfp_regdata *regdata,
			    const char *name,
			    char **matches,
			    unsigned int flags);

/** Populate a struct nfp_reg_id with a looked up node.
 *
 * @param regdata       Handle to register data
 * @param node          Pointer to a struct nfp_regdata_node variable which
 *                      must describe a register or bit field.
 * @param regid         Pointer to a struct nfp_red_id variable which will
 *                      be populated based on @p node.
 * @return 0 on success, -1 on error.
 */
NFP_COMMON_API
int nfp_regdata_node2regid(struct nfp_regdata *regdata,
			   const struct nfp_regdata_node *node,
			   struct nfp_reg_id *regid);

/** Get the register type of a register ID.
 *
 * @param regdata       Handle to register data
 * @param regid         Pointer to a struct nfp_red_id variable
 * @return              Register type.
 */
NFP_COMMON_API
enum nfp_reg_type nfp_regdata_regid2type(struct nfp_regdata *regdata,
					 const struct nfp_reg_id *regid);

/** Get the access details of a microengine register ID.
 *
 * @param regdata       Handle to register data
 * @param regid         Pointer to a struct nfp_red_id variable
 * @param mereg_type    Will be set to ME Register type
 * @param meid          Will be set to MEID
 * @param reg_idx       Will be set to the absolute register number
 * @return 0 on success, -1 on error.
 */
NFP_COMMON_API
int nfp_regdata_regid2mereg(struct nfp_regdata *regdata,
			    const struct nfp_reg_id *regid,
			    enum nfp_mereg_type *mereg_type,
			    int *meid,
			    uint16_t *reg_idx);

/** Get the access details of a microengine CSR register ID.
 *
 * @param regdata       Handle to register data
 * @param regid         Pointer to a struct nfp_red_id variable
 * @param meid          Will be set to MEID
 * @param offset        Will be set to the ME CSR byte offset
 * @return 0 on success, -1 on error.
 */
NFP_COMMON_API
int nfp_regdata_regid2mecsr(struct nfp_regdata *regdata,
			    const struct nfp_reg_id *regid,
			    int *meid,
			    uint16_t *offset);

/** Get the name of the lookup target the register can be found in.
 *
 * @param regdata       Handle to register data
 * @param regid         Pointer to a struct nfp_red_id variable
 * @return              Pointer to target name on success, NULL on error.
 */
NFP_COMMON_API
const char *nfp_reg_id_tgtname(struct nfp_regdata *regdata,
			       const struct nfp_reg_id *regid);

/** Compare two lookup nodes.
 *
 * @param node1     The first node
 * @param node2     The second node
 * @param cmp_str   Compare names and descriptions in the nodes.
 * @return          0 if equal,
 *                  -1 if (node1 < node2) and
 *                  1 if (node1 > node2)
 */
NFP_COMMON_API
int nfp_regdata_node_cmp(struct nfp_regdata *regdata,
			 const struct nfp_regdata_node *node1,
			 const struct nfp_regdata_node *node2,
			 int cmp_str);

/** Compare to struct nfp_reg_id variables.
 *
 * @param regid1    The first register ID
 * @param regid1    The second register ID
 * @return          0 if equal,
 *                  -1 if (regid1 < regid2) and
 *                  1 if (regid1 > regid2)
 */
NFP_COMMON_API
int nfp_reg_id_cmp(struct nfp_regdata *regdata,
		   const struct nfp_reg_id *regid1,
		   const struct nfp_reg_id *regid2);

/** Get the number of registers of a given type.
 *
 * This is very static data, but for flexibility this is used by the
 * SDK toolchain.
 *
 * @param regdata       Handle to register data
 * @param mereg_type    Type of microengine register to count.
 * @return Number of registers, or 0 if none or error.
 */
NFP_COMMON_API
size_t nfp_regdata_mereg_cnt(struct nfp_regdata *regdata,
			     enum nfp_mereg_type mereg_type);

/** Find a microengine CSR by name.
 *
 * Similar to @c nfp_regdata_lookup, but the lookup
 * name is simple an ME CSR name, not a hierarchical lookup string.
 *
 * It is mostly used by the SDK toolchain, but can provide a convenient
 * ME CSR name parser for other tools.
 *
 * @param regdata       Handle to register data
 * @param name          Microengine CSR name
 * @param flags         Lookup flags.
 * @param node          Pointer to a struct nfp_regdata_node variable which
 *                      will be populated with lookup results.
 * @return 0 on success, -1 on error.
 */
NFP_COMMON_API
int nfp_regdata_mecsr_lookup(struct nfp_regdata *regdata,
			     const char *name,
			     unsigned int flags,
			     struct nfp_regdata_node *node);

/** Get an array of microengine CSR nodes.
 *
 * Mostly used by the SDK toolchain.
 *
 * @param regdata       Handle to register data
 * @param nodes         Pointer to pointer of struct nfp_regdata_node which will
 *                      be set to a newly allocated array of nodes. The caller
 *                      must call @c nfp_regdata_node_free_data for each node
 *                      and then free() the entire array.
 * @return              Number of entries in @p nodes, -1 on error.
 */
NFP_COMMON_API
ssize_t nfp_regdata_mecsr_list(struct nfp_regdata *regdata,
			       struct nfp_regdata_node **nodes);

/** Find a register node by register ID.
 *
 * Useful for obtaining a fully qualified lookup name for error messages.
 * The caller populates @p regid with the register type and relevant details
 * before calling this function. @c nfp_reg_id.bit_len is ignored.
 *
 * @param regdata       Handle to register data
 * @param regid         Register ID
 * @param flags         Flags to control whether the node strings are
 *                      populated.
 * @param node          Pointer to struct nfp_regdata_node variable that will be
 *                      populated with the lookup results.
 */
NFP_COMMON_API
int nfp_regdata_find_regid(struct nfp_regdata *regdata,
			   const struct nfp_reg_id *regid,
			   unsigned int flags,
			   struct nfp_regdata_node *node);

/** Find a register node by CPP access details.
 *
 * Similar to @c nfp_regdata_find_regid, but searches specificaly
 * for a CPP type register.
 *
 * @param regdata       Handle to register data
 * @param cpp_tgt       CPP Target ID
 * @param island_id     Island ID
 * @param offset        CSR offset
 * @param flags         Flags to control whether the node strings are
 *                      populated.
 * @param node          Pointer to struct nfp_regdata_node variable that will be
 *                      populated with the lookup results.
 */
NFP_COMMON_API
int nfp_regdata_find_cpp(struct nfp_regdata *regdata,
			 int8_t cpp_tgt,
			 int island_id,
			 uint64_t offset,
			 unsigned int flags,
			 struct nfp_regdata_node *node);

/** Find a register node by XPB access details.
 *
 * Similar to @c nfp_regdata_find_regid, but searches specificaly
 * for an XPB type register.
 *
 * @param regdata       Handle to register data
 * @param offset        CSR offset (32-bit XPB address)
 * @param flags         Flags to control whether the node strings are
 *                      populated.
 * @param node          Pointer to struct nfp_regdata_node variable that will be
 *                      populated with the lookup results.
 */
NFP_COMMON_API
int nfp_regdata_find_xpb(struct nfp_regdata *regdata,
			 uint64_t offset,
			 unsigned int flags,
			 struct nfp_regdata_node *node);

/** Find a register node by ME CSR access details.
 *
 * Similar to @c nfp_regdata_find_regid, but searches specificaly
 * for an ME CSR type register.
 *
 * @param regdata       Handle to register data
 * @param island_id     Island ID. Can be local island.
 * @param menum         Microengine number within island. Can be
 *                      NFP_REGDATA_MENUM_LOCAL
 * @param ctx           Only applies for indirect ME CSRs.
 * @param mecsr_offset  ME CSR offset.
 * @param flags         Flags to control whether the node strings are
 *                      populated.
 * @param node          Pointer to struct nfp_regdata_node variable that will be
 *                      populated with the lookup results.
 */
NFP_COMMON_API
int nfp_regdata_find_mecsr(
	struct nfp_regdata *regdata,
	int island_id, /* Can be local island */
	int menum,     /* Can be NFP_SDK_REGS_MENUM_LOCAL */
	int ctx,       /* Only used for indirect-csr and if positive value */
	uint64_t mecsr_offset,
	unsigned int flags,
	struct nfp_regdata_node *node);

/** Find a register node by ME register access details.
 *
 * Similar to @c nfp_regdata_find_regid, but searches specificaly
 * for an ME register type register.
 *
 * @param regdata       Handle to register data
 * @param island_id     Island ID. Can be local island.
 * @param menum         Microengine number within island. Can be
 *                      NFP_REGDATA_MENUM_LOCAL
 * @param mereg_type    ME register type
 * @param mereg_offset  ME register byte offset.
 * @param flags         Flags to control whether the node strings are
 *                      populated.
 * @param node          Pointer to struct nfp_regdata_node variable that will be
 *                      populated with the lookup results.
 */
NFP_COMMON_API
int nfp_regdata_find_mereg(struct nfp_regdata *regdata,
			   int island_id,
			   int menum,
			   enum nfp_mereg_type mereg_type,
			   uint64_t mereg_offset,
			   unsigned int flags,
			   struct nfp_regdata_node *node);

__END_DECLS
#endif /* __NFP_REGDATA_H__ */
