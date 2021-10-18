/*
 * Copyright (C) 2012-2014 Netronome Systems, Inc.  All rights reserved.
 *
 * NFP ChipData API for descriptions and iterators of chip
 * resources. While heavily used by the SDK toolchain, the API
 * is also used on hardware to obtain information about the available
 * islands and microengines of an open nfp_device.
 *
 */

#ifndef __NFP_CHIPDATA_H__
#define __NFP_CHIPDATA_H__

#include <stdint.h>
#include <string.h>
#include <nfp-common/nfp_platform.h>
#include <nfp-common/nfp_resid.h>

#include "_nfp_common_apiex.h"

__BEGIN_DECLS

/** This matches NFP_CPP_MODEL_INVALID in nfp_cpp.h
 */
#define NFP_CHIPDATA_CPP_MODEL_INVALID 0xffffffff
#define NFP_CHIPDATA_FILE_NAME_CHIPS "nfp_userchips.json"
#define NFP_CHIPDATA_INCLUDE_PATH_ENVVAR "NFP_CHIPDATA_INCLUDE"
#define NFP_CHIPDATA_OVR_PATH_ENVVAR "NFP_CHIPDATA_PATH"
#define NFP_CHIPDATA_CHIP_ID_ENVVAR "NFP_CHIP_ID"
#define NFP_CHIPDATA_STRING_MAX_SZ 2048
#define NFP_CHIPDATA_MEMID_STR_SZ (NFP_MEID_STR_SZ + 8)

enum nfp_chipdata_mu_type {
	NFP_CHIPDATA_MU_TYPE_NONE = 0,
	NFP_CHIPDATA_MU_TYPE_CTM = 1,
	NFP_CHIPDATA_MU_TYPE_IMEM = 2,
	/* EMEM and DDR are viewed as the same on NFP-32xx. DDR is invalid for NFP-6xxx */
	NFP_CHIPDATA_MU_TYPE_EMEM = 3,
	NFP_CHIPDATA_MU_TYPE_DDR = 4
};

enum nfp_chipdata_mem_type {
	NFP_CHIPDATA_MEM_TYPE_NONE = 0,
	NFP_CHIPDATA_MEM_TYPE_CTM = 1,
	NFP_CHIPDATA_MEM_TYPE_IMEM = 2,
	/* EMEM and DDR are viewed as the same on NFP-32xx. DDR is invalid for NFP-6xxx */
	NFP_CHIPDATA_MEM_TYPE_EMEM = 3,
	NFP_CHIPDATA_MEM_TYPE_DDR = 4,
	/* NFP-6xxx VQDR is not handled as a new memory type. Instead,
	 * QDR0,1,2,3 are handled as global single-island memories for
	 * behaviour similar to NFP-32xx real and virtual QDR.
	 */
	/* NFP_CHIPDATA_MEM_TYPE_VQDR = 5, */
	NFP_CHIPDATA_MEM_TYPE_QDR0 = 6,
	NFP_CHIPDATA_MEM_TYPE_QDR1 = 7,
	NFP_CHIPDATA_MEM_TYPE_QDR2 = 8,
	NFP_CHIPDATA_MEM_TYPE_QDR3 = 9,
	NFP_CHIPDATA_MEM_TYPE_CLS = 10,
	/* GS is invalid for NFP-6xxx */
	NFP_CHIPDATA_MEM_TYPE_GS = 11,
	NFP_CHIPDATA_MEM_TYPE_LM = 12, /* Microengine local memory */
	NFP_CHIPDATA_MEM_TYPE_USTORE = 13,
	NFP_CHIPDATA_MEM_TYPE_CRYPTO = 14, /* Crypto SRAM buffer */
	NFP_CHIPDATA_MEM_TYPE_ILA = 15,
	NFP_CHIPDATA_MEM_TYPE_EMEM_CACHE = 16
};

enum nfp_chipdata_mem_class {
	NFP_CHIPDATA_MEM_CLASS_NONE = 0,
	NFP_CHIPDATA_MEM_CLASS_MU = 1,
	NFP_CHIPDATA_MEM_CLASS_QDR = 2,
	NFP_CHIPDATA_MEM_CLASS_CLS = 3,
	NFP_CHIPDATA_MEM_CLASS_GS = 4,
	NFP_CHIPDATA_MEM_CLASS_LM = 5,
	NFP_CHIPDATA_MEM_CLASS_USTORE = 6,
	NFP_CHIPDATA_MEM_CLASS_CRYPTO = 7,
	NFP_CHIPDATA_MEM_CLASS_ILA = 8
};

enum nfp_chipdata_isld_type {
	NFP_CHIPDATA_ISLD_TYPE_NONE = 0,
	NFP_CHIPDATA_ISLD_TYPE_ARM = 1,
	NFP_CHIPDATA_ISLD_TYPE_PCIE = 3,
	NFP_CHIPDATA_ISLD_TYPE_NBI = 4,
	NFP_CHIPDATA_ISLD_TYPE_CRYPTO = 5,
	NFP_CHIPDATA_ISLD_TYPE_MU_ENG_EXT = 6,
	NFP_CHIPDATA_ISLD_TYPE_MU_ENG_INT = 7,
	NFP_CHIPDATA_ISLD_TYPE_MECL = 8, /* 12 ME cluster island */
	NFP_CHIPDATA_ISLD_TYPE_ILA = 9   /* Interlaken */
};

enum nfp_chipdata_alloc_policy {
	NFP_CHIPDATA_ALLOC_POLICY_INVALID = 0,
	NFP_CHIPDATA_ALLOC_POLICY_SPREAD = 1,
	NFP_CHIPDATA_ALLOC_POLICY_EXHAUST = 2,
};

#define NFP_CHIPDATA_ALLOC_POLICY_DEFAULT (NFP_CHIPDATA_ALLOC_POLICY_SPREAD)

/* Opaque structures */
struct nfp_chipdata;
struct nfp_chipdata_chip;

/*
 * The following structures are not opaque because marking them opaque
 * requires a dozen extra access function wrappers to be defined, adds
 * tons of unnecessary code to the library, and in the end makes the
 * resulting client code more difficult to read.
 *
 * Any users of these structs should keep in mind that they may change
 * in future versions.
 */

struct nfp_chipdata_memory {
	int cpp_tgt; /* -1 if NA (ME local mem) */
	enum nfp_chipdata_mem_type type;
	size_t name; /* See nfp_chipdata_str */

	/* An absolute resource base (e.g. QDR channel offsets) */
	uint64_t base;

	/* Base in resoure from which to start allocating symbols for the chip
	 * family as a whole.
	 */
	uint64_t alloc_base;

	uint64_t size_max;
};

struct nfp_chipdata_alloc {
	enum nfp_chipdata_mem_type type;
	enum nfp_chipdata_alloc_policy policy;
	size_t islands_count;
	const int *islands;
	size_t targets_count;
	const int *targets;
	uint64_t base; /* This is relative to nfp_chipdata_memory.base */
	uint64_t size_max;
};

struct nfp_chipdata_alloc_type {
	size_t name; /* See nfp_chipdata_str */
	int local_alloc;
	size_t entries_count;
	const struct nfp_chipdata_alloc *entries;
};

/** This struct is used for both IMB and NBI
 *  translation config, but for NBI the
 *  only valid cpp_tgt is for the MU and the
 *  addr40 field must be 1.
 */
struct nfp_chipdata_cppat_entry_arch_th {
	uint8_t from_iid;
	uint8_t cpp_tgt;
	uint8_t isld0;
	uint8_t isld1;
	uint8_t addr40;
	uint8_t mode;
};

#define nfp_chipdata_cppat_entry_nfp6000 nfp_chipdata_cppat_entry_arch_th

union nfp_chipdata_cppat_entry {
	/* nfp3200 does not have this */
	struct nfp_chipdata_cppat_entry_arch_th nfp6000;
	struct nfp_chipdata_cppat_entry_arch_th arch_th;
};

/** Load chip data.
 *
 * Load all chip definition files found in user specified paths
 * or default embedded data.
 *
 * By default data is loaded from the library itself rather than
 * external JSON files. The path to external files can be set by the
 * @p chipdata_path parameter or the environment variable
 * NFP_CHIPDATA_PATH.
 *
 * User files with the name nfp_userchips.json
 * are searched for in the @p include_paths parameter paths, then the
 * environment variable NFP_CHIPDATA_INCLUDE
 * is searched in the same manner. Finally, the default chipdata is loaded.
 *
 * Any chip that is defined with the same ID as an already defined
 * chip will be silently ignored. For example, say a default chip with ID
 * "NFP-1234" is defined in the Netronome provided files. A user can
 * then decide to refine this chip to tweak any of its attributes by
 * defining it in a user JSON file and adding a search path to that file.
 *
 * @param include_paths     NULL, or a list of directories separated by ":" on
 *                          Linux and ";" on Windows.
 * @param chipdata_path     NULL to use the default embedded data or
 *                          the path specified in the environment variable.
 *                          Otherwise, a non-NULL absolute path to a chip data
 *                          directory which will take precedence.
 *
 * @return                  An opaque handle to the loaded data.
 */
NFP_COMMON_API
struct nfp_chipdata *nfp_chipdata_load(const char *include_paths,
				       const char *chipdata_path);

/** Get string for chipdata strtab
 *
 * Returns a pointer to the string in the chipdata strtab at the given offset.
 * The value passed as the offset should only be a value that was returned by
 * one of the other API calls.
 *
 * @param data      Chip data as returned by @c nfp_chipdata_load
 * @param offset    Starting character offset.
 * @return          String pointer (&strtab[offset]), NULL on error.
 */
NFP_COMMON_API
const char *nfp_chipdata_str(const struct nfp_chipdata *data, size_t offset);

/** Get information about the loaded data.
 *
 * This function exists purely for diagnostic purposes. The returned string
 * is a concatenation of all lines in the '_comment' array in chips.json,
 * separated and ended by '\n'.
 *
 * @param data      Chip data as returned by @c nfp_chipdata_load
 * @return          NULL on error, empty string if there is no comment
 *                  or a comment string.
 */
NFP_COMMON_API
const char *nfp_chipdata_comment(const struct nfp_chipdata *data);

/** Returns the chip data path from which the data was loaded or
 * the default path if embedded data was loaded instead and
 * @p default_path is non-zero.
 *
 * If @p data is NULL, NULL will be returned.
 *
 * @return  Path to chip data directory.
 */
NFP_COMMON_API
const char *nfp_chipdata_path(const struct nfp_chipdata *data, int default_path);

/** Returns the relative chip data path.
 *
 * The path is relative to the executable directory and is usually
 * bin/../nfp_chipdata. It is mainly used internally.
 *
 * @return  Relative path to chip data directory.
 */
NFP_COMMON_API
const char *nfp_chipdata_rel_path();

/** Returns the string representation of the family type.
 * @param family    Chip family as defined in nfp_resid.
 * @return          Family name, NULL if family not recognised.
 */
NFP_COMMON_API
const char *nfp_chipdata_family2str(int family);

/** Returns the the family type for the given string.
 * @param family    Chip family as defined in nfp_resid.
 * @return          Family name, NULL if family not recognised.
 */
NFP_COMMON_API
int nfp_chipdata_str2family(const char *family_str);

/** Select a specific chip from the chip data and get a handle to it.
 *
 * The chip is selected by string ID and a case insensitive match will
 * return a handle for that chip.
 *
 * @param data      Chip data as returned by @c nfp_chipdata_load
 * @param chip_id   A string that should match one of the defined
 *                  chips in the chip data. If NULL, the environment variable
 *                  as defined by NFP_CHIPDATA_CHIP_ID_ENVVAR is checked.
 * @return          Handle to matching chip if found, NULL if none is found or
 *                  any parameter was NULL.
 */
NFP_COMMON_API
const struct nfp_chipdata_chip *nfp_chipdata_select(const struct nfp_chipdata *data,
						    const char *chip_id);

/** Check if the chip is part of a board.
 *
 * A chip that is part of a board will have more specific memory sizes and
 * always have only a single revision. A board can have one or more chips
 * and the chip is referenced by "board:num", where num is 0 or more.
 *
 * @param chip      Chip handle as returned by @c nfp_chipdata_select.
 *
 * @return 1 if the chip is board-specific, 0 if not.
 */
NFP_COMMON_API
int nfp_chipdata_on_board(const struct nfp_chipdata_chip *chip);

/** Get the chip ID of the chip on the board.
 *
 * @param chip      Chip handle as returned by @c nfp_chipdata_new.
 *
 * @return chip ID, or empty string if none or error.
 */
NFP_COMMON_API
const char *nfp_chipdata_board_chip_id(const struct nfp_chipdata_chip *chip);

/** Create a new, mutable, chip, optionally inherited from another chip.
 *
 * Chips created with this function are not added to the chipdata
 * list, so chip iteration using @c nfp_chipdata_chip_next will not include
 * this new chip.
 *
 * @param data          Chip data as returned by @c nfp_chipdata_load
 * @param chip_family   Family of the new chip.
 * @param chip_rev      Revision of the new chip.
 *                      If @p inherit_chip is not NULL, @p chip_rev must be one
 *                      of the revisions of the parent chip, or -1.
 *                      If it is -1, the highest revision will be selected.
 *                      If @p inherit_chip is NULL, @p chip_rev must be a valid
 *                      revision for the given family.
 * @param new_id        The Chip ID to assign to the new chip.
 * @param inherit_chip  Chip ID of another pre-defined chip to inherit from,
 *                      or NULL for a blank chip.
 * @return              A new chip, or NULL on error. Caller must free
 *                      the chip using @c nfp_chipdata_chip_free
 */
NFP_COMMON_API
struct nfp_chipdata_chip *nfp_chipdata_chip_new(struct nfp_chipdata *data,
						int chip_family,
						int chip_rev,
						const char *new_id,
						const char *inherit_chip);

/** Add an island ID to a mutable chip.
 *
 * If the island ID is already in the chip's list, it will not be added
 * again and it will not be an error.
 *
 * This function returns an error when used for NFP-32xx chips.
 *
 * @param chip      Chip handle as returned by @c nfp_chipdata_new.
 * @param island_id The Island ID to add.
 * @return          0 on success, non-zero on error.
 */
NFP_COMMON_API
int nfp_chipdata_island_add(struct nfp_chipdata_chip *chip, int island_id);

/** Remove an island ID from a mutable chip.
 *
 * If the island ID is not in the chip's list, it will not be an error.
 *
 * This function returns an error when used for NFP-32xx chips.
 *
 * @param chip      Chip handle as returned by @c nfp_chipdata_new.
 * @param island_id The Island ID to remove.
 * @return          0 on success, non-zero on error.
 */
NFP_COMMON_API
int nfp_chipdata_island_rem(struct nfp_chipdata_chip *chip, int island_id);

/** Add a revision to a mutable chip.
 *
 * If the revision is already in the chip's list, it will not be added
 * again and it will not be an error.
 *
 * @param chip      Chip handle as returned by @c nfp_chipdata_new.
 * @param revision The revision to add.
 * @return          0 on success, non-zero on error.
 */
NFP_COMMON_API
int nfp_chipdata_revision_add(struct nfp_chipdata_chip *chip, int revision);

/** Remove a revision from a mutable chip.
 *
 * If the revision is not in the chip's list, it will not be an error.
 *
 * @param chip      Chip handle as returned by @c nfp_chipdata_new.
 * @param revision  The revision to remove.
 * @return          0 on success, non-zero on error.
 */
NFP_COMMON_API
int nfp_chipdata_revision_rem(struct nfp_chipdata_chip *chip, int revision);

/** Set the model ID for a given revision of the given chip.
 *
 * If the chip has no @p revision or @p revision is negative, an error will
 * be returned.
 *
 * Do not modify the revision list after calling this function, the revision
 * and model ID lists will not stay synchronised.
 *
 * @param chip      Chip handle as returned by @c nfp_chipdata_new.
 * @param revision  The revision to set a model ID for.
 * @param model     Model ID
 * @return          0 on success, non-zero on error.
 */
NFP_COMMON_API
int nfp_chipdata_model_set(struct nfp_chipdata_chip *chip, int revision, uint32_t model);

/** Set the number of microengines in a mutable NFP-32xx chip.
 *
 * This function should only be used for NFP-32xx chips.
 *
 * @param chip      Chip handle as returned by @c nfp_chipdata_new.
 * @param me_cnt    The number of microengines (multiple of 8).
 * @return          0 on success, non-zero on error.
 */
NFP_COMMON_API
int nfp_chipdata_mecnt_set(struct nfp_chipdata_chip *chip, size_t me_cnt);

/** Add a CPP Target ID to a mutable NFP-32xx chip.
 *
 * If the CPP Target ID is already in the chip's list, it will not be added
 * again and it will not be an error.
 *
 * This function can only be used with NFP-32xx chips and is mainly intended
 * to define whether a CryptoUnit is present or not.
 *
 * @param chip      Chip handle as returned by @c nfp_chipdata_new.
 * @param cpp_tgt   The CPP Target ID to add.
 * @return          0 on success, non-zero on error.
 */
NFP_COMMON_API
int nfp_chipdata_cpptgt_add(struct nfp_chipdata_chip *chip, int cpp_tgt);

/** Remove a CPP Target ID from a mutable NFP-32xx chip.
 *
 * If the CPP Target ID is not in the chip's list, it will not be an error.
 *
 * This function can only be used with NFP-32xx chips.
 *
 * @param chip      Chip handle as returned by @c nfp_chipdata_new.
 * @param cpp_tgt   The CPP Target ID to remove.
 * @return          0 on success, non-zero on error.
 */
NFP_COMMON_API
int nfp_chipdata_cpptgt_rem(struct nfp_chipdata_chip *chip, int cpp_tgt);

/** Get the name of a CPP Target ID.
 *
 * @param chip      Chip handle as returned by @c nfp_chipdata_new.
 * @param cpp_tgt   The CPP Target ID.
 * @return          Pointer to the name, NULL if the chip family does not
 *                  recognise the CPP Target ID. Note that this is different
 *                  from a chip that knows about, but does not have, the CPP
 *                  target ID.
 */
NFP_COMMON_API
const char *nfp_chipdata_cpptgt2str(const struct nfp_chipdata_chip *chip, int cpp_tgt);

/** Free a chip previously created with @c nfp_chipdata_chip_new
 *
 * @param chip      Chip handle as returned by @c nfp_chipdata_new.
 */
NFP_COMMON_API
void nfp_chipdata_chip_free(struct nfp_chipdata_chip *chip);

/** Get the chip data that this chip was selected from.
 *
 * @param chip      Chip handle.
 * @return          Chip data for this chip.
 */
NFP_COMMON_API
const struct nfp_chipdata *nfp_chipdata_from_chip(const struct nfp_chipdata_chip *chip);

/** Get an the first defined chip.
 *
 * @param data      Chip data as returned by @c nfp_chipdata_load
 * @return          Chip handle to the first chip. NULL if there are none.
 */
NFP_COMMON_API
const struct nfp_chipdata_chip *nfp_chipdata_chip_first(const struct nfp_chipdata *data);

/** Get the next chip after the given chip.
 *
 * @param data      Chip data.
 * @param chip      Current chip handle. If NULL, NULL will be returned.
 * @return          Chip handle to the next chip. NULL if there are none.
 */
NFP_COMMON_API
const struct nfp_chipdata_chip *nfp_chipdata_chip_next(
	const struct nfp_chipdata *data,
	const struct nfp_chipdata_chip *chip);

/** Get the chip's architecture.
 *
 * @param chip      Chip handle.
 * @return          Chip arch as defined in nfp_resid, 0 on error.
 */
NFP_COMMON_API
int nfp_chipdata_chip_arch(const struct nfp_chipdata_chip *chip);

/** Get the chip's family.
 *
 * @param chip      Chip handle.
 * @return          Chip family as defined in nfp_resid, 0 on error.
 */
NFP_COMMON_API
int nfp_chipdata_chip_family(const struct nfp_chipdata_chip *chip);

/** Get the chip's definition file path.
 *
 * @param chip      Chip handle.
 * @return          Path to definition file.
 */
NFP_COMMON_API
const char *nfp_chipdata_chip_file(const struct nfp_chipdata_chip *chip);

/** Get the chip's ID.
 *
 * @param chip      Chip handle.
 * @return          Chip ID string.
 */
NFP_COMMON_API
const char *nfp_chipdata_chip_id(const struct nfp_chipdata_chip *chip);

/** Get the chip's description.
 *
 * @param chip      Chip handle.
 * @return          Chip description string. NULL if there is no description.
 */
NFP_COMMON_API
const char *nfp_chipdata_chip_description(const struct nfp_chipdata_chip *chip);

/** Get the chip's revision list.
 *
 * Revision has the value 0x00 for A0, 0x01 for A1 and 0x10 for B0, etc.
 *
 * @param chip      Chip handle.
 * @param revisions Pointer to int pointer, will be set to the revision array
 *                  if not NULL.
 * @return          Number of revisions. If 0, @p *revisions is undefined
 */
NFP_COMMON_API
size_t nfp_chipdata_chip_revisions(const struct nfp_chipdata_chip *chip,
				   const int **revisions);

/** Check if the chip has the specific revision.
 *
 * @param chip      Chip handle.
 * @param revision  Revision number (e.g. 0x00)
 * @return          1 if the chip has the given revision, 0 if not.
 */
NFP_COMMON_API
int nfp_chipdata_has_revision(const struct nfp_chipdata_chip *chip, int revision);

/** Get the lowest available revision for the given chip.
 *
 * The returned revision is 0x00 for A0, 0x11 for B1 etc.
 *
 * @param chip      Chip handle.
 * @return          Revision number, -1 on error.
 */
NFP_COMMON_API
int nfp_chipdata_revision_min(const struct nfp_chipdata_chip *chip);

/** Get the highest available revision for the given chip.
 *
 * The returned revision is 0x00 for A0, 0x11 for B1 etc.
 *
 * @param chip      Chip handle.
 * @return          Revision number, -1 on error.
 */
NFP_COMMON_API
int nfp_chipdata_revision_max(const struct nfp_chipdata_chip *chip);

/** Get the microengine version for the given chip.
 *
 * Returns one of the values in NFP_CHIP_ME_VERSION_*.
 *
 * @param chip      Chip handle.
 * @return          Microengine version, -1 on error.
 */
NFP_COMMON_API
int nfp_chipdata_me_version(const struct nfp_chipdata_chip *chip);

/** Get the highest available microengine version for the given chip.
 *
 *
 *
 * @param chip      Chip handle.
 * @return          Microengine version, -1 on error.
 */
NFP_COMMON_API
int nfp_chipdata_me_ver_max(const struct nfp_chipdata_chip *chip);

/** Get the model ID for a given revision of the given chip.
 *
 * If the chip has no @p revision or no model ID for @p revision, the return
 * value will be NFP_CHIPDATA_CPP_MODEL_INVALID.
 *
 * @param chip      Chip handle.
 * @param revision  Revision number. If this is -1 and the chip has only one
 *                  revision, then that revision's model ID will be returned.
 * @return Model ID, or NFP_CHIPDATA_CPP_MODEL_INVALID on error or if none.
 */
NFP_COMMON_API
uint32_t nfp_chipdata_model(const struct nfp_chipdata_chip *chip, int revision);

/** Check if the chip has the specific island.
 *
 * For NFP-32xx, this will instead check the existence of a
 * microengine cluster with the number @p island_id.
 *
 * @param chip      Chip handle.
 * @param island_id Island ID (or cluster number for NFP-32xx).
 * @return          1 if island is present, 0 if not.
 */
NFP_COMMON_API
int nfp_chipdata_has_island(const struct nfp_chipdata_chip *chip, int island_id);

/** Get the chip's island ID list.
 *
 * For NFP-32xx, this has no purpose. It will return 0. Use
 * @c nfp_chipdata_island_count to determine how many microengine
 * clusters the NFP-32xx chip has.
 *
 * @param chip      Chip handle.
 * @param revisions Pointer to int pointer, will be set to the island ID array
 *                  if not NULL.
 * @return          Number of Island IDs. If 0, @p *island_ids is undefined
 */
NFP_COMMON_API
size_t nfp_chipdata_island_ids(const struct nfp_chipdata_chip *chip,
			       const int **island_ids);

/** Get the first island ID.
 *
 * For NFP-32xx 0 is always the first ID.
 *
 * @param chip      Chip handle.
 * @return          First island ID. Can be -1 if there are no islands.
 */
NFP_COMMON_API
int nfp_chipdata_island_id_first(const struct nfp_chipdata_chip *chip);

/** Get the last island ID.
 *
 * @param chip      Chip handle.
 * @return          Last island ID. Can be -1 if there are no islands.
 */
NFP_COMMON_API
int nfp_chipdata_island_id_last(const struct nfp_chipdata_chip *chip);

/** Get the next island ID after the given island ID.
 *
 * For NFP-32xx the next cluster number is returned.
 *
 * @param chip      Chip handle.
 * @param island_id Island ID.
 * @return          Next island ID. -1 if there are no more islands.
 */
NFP_COMMON_API
int nfp_chipdata_island_id_next(const struct nfp_chipdata_chip *chip, int island_id);

/** Get the previous island ID after the given island ID.
 *
 * For NFP-32xx the previous cluster number is returned.
 *
 * @param chip      Chip handle.
 * @param island_id Island ID.
 * @return          Previous island ID. -1 if there are no more islands.
 */
NFP_COMMON_API
int nfp_chipdata_island_id_prev(const struct nfp_chipdata_chip *chip, int island_id);

/** Get the first island ID of given type.
 *
 * For NFP-32xx the type parameter is always NFP_CHIPDATA_ISLD_TYPE_NONE.
 *
 * @param chip          Chip handle.
 * @param island_type   The type of island to find the first ID of. For
 *                      NFP_CHIPDATA_ISLD_TYPE_NONE, this behaves like
 *                      nfp_chipdata_island_id_first.
 * @return              First island ID. Can be -1 if there are no islands.
 */
NFP_COMMON_API
int nfp_chipdata_island_id_type_first(const struct nfp_chipdata_chip *chip,
				      enum nfp_chipdata_isld_type island_type);

/** Get the next island ID of a given type after the given island ID.
 *
 * For NFP-32xx the next cluster number is returned.
 *
 * @param chip      Chip handle.
 * @param island_id Island ID.
 * @param island_type   The type of island to find the first ID of. For
 *                      NFP_CHIPDATA_ISLD_TYPE_NONE, this behaves like
 *                      nfp_chipdata_island_id_next.
 * @return          Next island ID. -1 if there are no more islands.
 */
NFP_COMMON_API
int nfp_chipdata_island_id_type_next(const struct nfp_chipdata_chip *chip,
				     int island_id,
				     enum nfp_chipdata_isld_type island_type);

/** Get the previous island ID of a given type after the given island ID.
 *
 * For NFP-32xx the previous cluster number is returned.
 *
 * @param chip      Chip handle.
 * @param island_id Island ID.
 * @param island_type   The type of island to find the first ID of. For
 *                      NFP_CHIPDATA_ISLD_TYPE_NONE, this behaves like
 *                      nfp_chipdata_island_id_prev.
 * @return          Previous island ID. -1 if there are no more islands.
 */
NFP_COMMON_API
int nfp_chipdata_island_id_type_prev(const struct nfp_chipdata_chip *chip,
				     int island_id,
				     enum nfp_chipdata_isld_type island_type);

/** For a given chip, count all or a specific type of islands.
 *
 * For NFP-32xx, this returns the number of clusters with microengines.
 *
 * @param chip          Chip handle.
 * @param island_type   Chip family specific enum value. If *_NONE, count all
 *                      islands. Otherwise, all islands the chip has of that
 *                      type are counted. The selected chip's family is used
 *                      to interpret @c island_type correctly. For NFP-32xx
 *                      this parameter is ignored.
 * @return              0 if no matching island types or no islands,
 *                      number of matching islands otherwise.
 *
 * Example:
 * @code
 * cnt = nfp_chipdata_island_count(selected_nfp_chip,
 *          NFP_CHIPDATA_ISLD_TYPE_CRYPTO);
 * @endcode
 *
 */
NFP_COMMON_API
size_t nfp_chipdata_island_count(const struct nfp_chipdata_chip *chip,
				 enum nfp_chipdata_isld_type island_type);

/** Determine if the select chip has a cryptography engine.
 *
 * @param chip  Chip handle.
 * @return      1 if chip has cryptography, 0 if not.
 */
NFP_COMMON_API
int nfp_chipdata_has_crypto(const struct nfp_chipdata_chip *chip);

/** Get the name for the given island ID.
 *
 * Does not apply to NFP-32xx. Returns NULL.
 *
 * @param chip          Chip handle.
 * @param island_id     Island ID.
 * @return              Island name string. NULL if island not in chip.
 */
NFP_COMMON_API
const char *nfp_chipdata_island_name(const struct nfp_chipdata_chip *chip, int island_id);

/** Get the name for the given island ID.
 *
 * The return value depends on the select chip's family.
 *
 * Does not apply to NFP-32xx. Returns NFP_CHIPDATA_ISLD_TYPE_NONE.
 *
 * @param chip          Chip handle.
 * @param island_id     Island ID.
 * @return              Island type, 0 if island not in chip.
 */
NFP_COMMON_API
enum nfp_chipdata_isld_type nfp_chipdata_island_type(const struct nfp_chipdata_chip *chip,
						     int island_id);

/** Get the island's CPP target list.
 *
 * Does not apply to NFP-32xx. Returns 0.
 *
 * @param chip          Chip handle.
 * @param cpp_targets   Pointer to int pointer, will be set to the CPP target
 *                      ID array if not NULL.
 * @param island_id     Island ID.
 * @return              Number of CPP targets. If 0, @p *cpp_targets is
 *                      undefined
 */
NFP_COMMON_API
size_t nfp_chipdata_island_cpp_targets(const struct nfp_chipdata_chip *chip,
				       const int **cpp_targets,
				       int island_id);

/** Check if the chip has the specific CPP Target.
 *
 * For NFP-32xx, returns 1 if the chip has the specified CPP target.
 * For NFP-6xxx, returns 1 if any island has the specified CPP target.
 *
 * @param chip          Chip handle.
 * @param cpp_tgt_id    CPP Target ID.
 * @return              1 if CPP target is present, 0 if not.
 */
NFP_COMMON_API
int nfp_chipdata_has_cpp_target(const struct nfp_chipdata_chip *chip, int cpp_tgt_id);

/** Check if the island has the specific CPP Target.
 *
 * Does not apply to NFP-32xx. Returns 0.
 *
 * @param chip          Chip handle.
 * @param island_id     Island ID.
 * @param cpp_tgt_id    CPP Target ID.
 * @return              1 if CPP target is present, 0 if not.
 */
NFP_COMMON_API
int nfp_chipdata_island_has_cpp_target(const struct nfp_chipdata_chip *chip,
				       int island_id,
				       int cpp_tgt_id);

/** Return the number of microengines in the specific chip.
 *
 * @param chip  Chip handle.
 *
 * @return      Number of microengines, 0 or higher.
 */
NFP_COMMON_API
size_t nfp_chipdata_me_count(const struct nfp_chipdata_chip *chip);

/** Return the number of serdes lines in the specific chip.
 *
 * Returns 0 for NFP-32xx.
 *
 * @param chip  Chip handle.
 *
 * @return      Number of serdes lines, 0 or higher.
 */
NFP_COMMON_API
size_t nfp_chipdata_serdes_count(const struct nfp_chipdata_chip *chip);

/** Return the number of microengines in the specific island.
 *
 * For NFP-32xx, returns 8 regardless of @p island_id.
 *
 * @param chip  Chip handle.
 * @param iid   Island ID to get microengines count of. For island 0
 *              the number of microengines of the island with the most
 *              microengines is returned.
 *
 * @return      Number of microengines, 0 or higher.
 */
NFP_COMMON_API
size_t nfp_chipdata_island_me_count(const struct nfp_chipdata_chip *chip, int island_id);

/** Get the first microengine ID.
 *
 * @param chip      Chip handle.
 * @return          First ME ID. Can be -1.
 */
NFP_COMMON_API
int nfp_chipdata_meid_first(const struct nfp_chipdata_chip *chip);

/** Get the last microengine ID.
 *
 * @param chip      Chip handle.
 * @return          Last ME ID. Can be -1.
 */
NFP_COMMON_API
int nfp_chipdata_meid_last(const struct nfp_chipdata_chip *chip);

/** Get the next microengine ID after the given microengine ID.
 *
 * @param chip      Chip handle.
 * @param meid      ME ID.
 * @return          Next ME ID. -1 if there are no more microengines.
 */
NFP_COMMON_API
int nfp_chipdata_meid_next(const struct nfp_chipdata_chip *chip, int meid);

/** Get the previous microengine ID after the given microengine ID.
 *
 * @param chip      Chip handle.
 * @param meid      ME ID.
 * @return          Previous ME ID. -1 if there are no more microengines.
 */
NFP_COMMON_API
int nfp_chipdata_meid_prev(const struct nfp_chipdata_chip *chip, int meid);

/** Return the number of microengines in the specific island.
 *
 * @param chip  Chip handle.
 * @param meid  Microengine ID. The ID includes the island ID
 *              (or cluster number for NFP-32xx) and the ME number
 *              within the island (or cluster).
 *              See nfp_resid.h.
 *
 * @return      Returns 1 if the ME ID exists in the given chip.
 */
NFP_COMMON_API
int nfp_chipdata_has_me(const struct nfp_chipdata_chip *chip, int meid);

/** Return the memory unit type for the specific island.
 *
 * For NFP-32xx, always returns NFP_CHIPDATA_MU_TYPE_DDR.
 *
 * For NFP-6xxx, one of:
 * NFP_CHIPDATA_MU_TYPE_CTM
 * NFP_CHIPDATA_MU_TYPE_IMEM
 * NFP_CHIPDATA_MU_TYPE_EMEM
 *
 * @param chip  Chip handle.
 * @param iid   Island ID to get MU type of.
 *
 * @return      Memory unit type.
 */
NFP_COMMON_API
enum nfp_chipdata_mu_type nfp_chipdata_island_mu_type(const struct nfp_chipdata_chip *chip,
						      int island_id);

/** Return the memory unit's maximum supported byte size for the
 *  specific island.
 *
 * @param chip      Chip handle.
 * @param island_id Island ID to get MU maximum size of. Ignored for NFP-32xx.
 *
 * @return      Memory unit maximum size in bytes. 0 if island has no
 *              memory unit.
 */
NFP_COMMON_API
uint64_t nfp_chipdata_mu_max_size(const struct nfp_chipdata_chip *chip, int island_id);

/** Return the base address for the given memory type.
 *
 * The memory type is interpreted based on the chip's family.
 *
 * @param chip          Chip handle.
 * @param mem_type      Memory type. The selected chip's family is used
 *                      to interpret this parameter correctly.
 * @param island_id     If not -1 and the island does not have the
 *                      @p mem_type memory type, 0 is returned.
 *                      If -1, no such check is performed. For NFP-32xx
 *                      this parameter is ignored.
 * @return              Memory type's base address.
 *                      0 if error or no matching memory type.
 */
NFP_COMMON_API
uint64_t nfp_chipdata_mem_base(const struct nfp_chipdata_chip *chip,
			       enum nfp_chipdata_mem_type mem_type,
			       int island_id);

/** Return the allocation base address for the given memory type.
 *
 * The memory type is interpreted based on the chip's family.
 *
 * The allocation base address is the address from which the linker
 * will allocate memory symbols. It can be overridden with the linker
 * command line parameters.
 *
 * @param chip          Chip handle.
 * @param mem_type      Memory type. The selected chip's family is used
 *                      to interpret this parameter correctly.
 * @param island_id     If not -1 and the island does not have the
 *                      @p mem_type memory type, 0 is returned.
 *                      If -1, no such check is performed. For NFP-32xx
 *                      this parameter is ignored.
 * @return              Memory type's allocation base address.
 *                      0 if error or no matching memory type.
 */
NFP_COMMON_API
uint64_t nfp_chipdata_mem_alloc_base(const struct nfp_chipdata_chip *chip,
				     enum nfp_chipdata_mem_type mem_type,
				     int island_id);

/** Return the maximum byte size for the given memory type.
 *
 * The memory type is interpreted based on the chip's family.
 *
 * @param chip          Chip handle.
 * @param mem_type      Memory type. The selected chip's family is used
 *                      to interpret this parameter correctly.
 * @param island_id     If not -1 and the island does not have the
 *                      @p mem_type memory type, 0 is returned.
 *                      If -1, no such check is performed. For NFP-32xx
 *                      this parameter is ignored.
 * @return              Memory type's maximum size in bytes.
 *                      0 if error or no matching memory type.
 */
NFP_COMMON_API
uint64_t nfp_chipdata_mem_max_size(const struct nfp_chipdata_chip *chip,
				   enum nfp_chipdata_mem_type mem_type,
				   int island_id);

/** Check if the island has the specific memory type.
 *
 * Does not apply to NFP-32xx. Returns 0.
 * If this returns 0, @c nfp_chipdata_mem_max_size will also
 * return 0.
 *
 * @param chip          Chip handle.
 * @param mem_type      Memory type. The selected chip's family is used
 *                      to interpret this parameter correctly.
 *                      This excludes microengine memory types.
 * @param island_id     Must be a valid island ID greater than 0.
 * @return              1 if island has the memory type, 0 if not.
 */
NFP_COMMON_API
int nfp_chipdata_island_has_mem_type(const struct nfp_chipdata_chip *chip,
				     enum nfp_chipdata_mem_type mem_type,
				     int island_id);

/** Return the name for the given memory type.
 *
 * The memory type is interpreted based on the chip's family.
 *
 * @param chip          Chip handle.
 * @param mem_type      Memory type. The selected chip's family is used
 *                      to interpret this parameter correctly.
 * @return              Memory type's name.
 *                      NULL if error or no matching memory type.
 */
NFP_COMMON_API
const char *nfp_chipdata_mem_name(const struct nfp_chipdata_chip *chip,
				  enum nfp_chipdata_mem_type mem_type);

/** Return the CPP Target ID for the given memory type.
 *
 * The memory type is interpreted based on the chip's family.
 *
 * @param chip          Chip handle.
 * @param mem_type      Memory type. The selected chip's family is used
 *                      to interpret this parameter correctly.
 * @return              Memory type's CPP Target ID.
 *                      -1 for memory types that are not CPP accessible,
 *                      such as microengine local-mem.
 *                      Also returns -1 on error.
 */
NFP_COMMON_API
int nfp_chipdata_mem_cpp_tgt(const struct nfp_chipdata_chip *chip,
			     enum nfp_chipdata_mem_type mem_type);

/** Get memories for chip.
 *
 * @param chip      Chip handle.
 * @param memories  Returned array of memories.
 *
 * @return          Number of memories. If 0, @p *memories is undefined
 */
NFP_COMMON_API
size_t nfp_chipdata_memories(const struct nfp_chipdata_chip *chip,
			     const struct nfp_chipdata_memory **memories);

/** Get the island's CPP target list.
 *
 * For NFP-32xx, always returns 1 and only
 * NFP_CHIPDATA_MEM_TYPE_CLS for all clusters.
 *
 * @param chip          Chip handle.
 * @param mem_types     Pointer to enum pointer, will be set to the memory
 *                      type array if not NULL.
 * @param island_id     Island ID.
 * @return              Number of memory types. If 0, @p *cpp_targets is
 *                      undefined
 */
NFP_COMMON_API
size_t nfp_chipdata_island_mem_types(const struct nfp_chipdata_chip *chip,
				     const enum nfp_chipdata_mem_type **mem_types,
				     int island_id);

/** Lookup alias for allocation type. Used by NFP SDK toolchain.
 *
 * @param chip        Chip handle.
 * @param name        Name of allocation type.
 * @return            Name of actual allocation type, or @name of no alias.
 *
 */
NFP_COMMON_API
const char *nfp_chipdata_alloc_alias(const struct nfp_chipdata_chip *chip,
				     const char *name);

/** Find matching generic allocation type. Used by NFP SDK toolchain.
 *
 * @param chip        Chip handle.
 * @param alloc_type  Name of allocation type.
 * @return            Allocation type struct.  NULL if alloc type was not
 *                    found.
 */
NFP_COMMON_API
const struct nfp_chipdata_alloc_type *nfp_chipdata_find_alloc_type(
	const struct nfp_chipdata_chip *chip,
	const char *alloc_type);

/** Find matching allocation type. Used by NFP SDK toolchain.
 *
 * @param chip        Chip handle.
 * @param alloc_type  Name of allocation type.
 * @param island      Island ID where type is located.
 * @return            Allocation struct.  NULL if alloc type was not found.
 */
NFP_COMMON_API
const struct nfp_chipdata_alloc *nfp_chipdata_find_alloc(
	const struct nfp_chipdata_chip *chip,
	const char *alloc_type,
	int island_id);

/** Get allocation types for chip. Used by NFP SDK toolchain.
 *
 * @param chip      Chip handle.
 * @param types     Returned array of alloc types.
 *
 * @return          Number of allocation types. If 0, @p *types is undefined
 */
NFP_COMMON_API
size_t nfp_chipdata_alloc_types(const struct nfp_chipdata_chip *chip,
				const struct nfp_chipdata_alloc_type **types);

/** Get CPP Address Translation Tables.
 *
 * Essentially, the values of the TargetAddressModeCfg or NbiMuXlate
 * CSRs in each island that has them:
 * xpb:i32.IMBXpbMap.Target1AddressModeCfg
 * xpb:nbi0.NbiTopXpbMap.NbiTopCSRs.NbiMuXlate
 *
 * The array will be sorted by island then by cpp_tgt.
 *
 * @param chip      Chip handle.
 * @param entries   Returned array of entries.
 *
 * @return          Number of entries. If 0, @p *entries is undefined.
 *                  0 for NFP-32xx.
 */
NFP_COMMON_API
size_t nfp_chipdata_cppat_entries(const struct nfp_chipdata_chip *chip,
				  const union nfp_chipdata_cppat_entry **entries);

/** Get a specific CPPAT Table entry.
 *
 * @param chip_family       Chip family.
 * @param entries           Array of CPPAT entries, can be the array
 *                          returned by nfp_chipdata_cppat_entries
 * @param entry_cnt         Size of entries
 *
 * @return Constant pointer to entry. NULL for NFP-32xx.
 */
NFP_COMMON_API
const union nfp_chipdata_cppat_entry *nfp_chipdata_cppat_entry_find(
	int chip_family,
	const union nfp_chipdata_cppat_entry *entries,
	size_t entry_cnt,
	int from_island,
	int cpp_tgt);

/** Encode island ID bits in address from a given island and CPP target.
 *
 * This function does nothing for NFP-32xx (returns 0).
 *
 * @param entries           Array of CPPAT entries, can be the array
 *                          returned by nfp_chipdata_cppat_entries
 * @param entry_cnt         Size of entries
 * @param from_island   The island from which the command is sent.
 *                      Specifies which island's translator to use for CPPAT.
 * @param cpp_tgt       The CPP Target ID the command is sent to.
 * @param to_island     The destination island.
 * @param addr          A base address in which to set the relevant
 *                      island ID bits to produce an address matching
 *                      the criteria by the other parameters.
 *                      Note that for Memory Unit CPP Targets, the
 *                      address' locality bits need to be set before
 *                      calling this function.
 *
 * @return 0 on success, -ENODEV if no encoding was possible or the chip
 *                       does not have island @p from_island,
 *                       -EINVAL if any parameter was invalid.
 */
NFP_COMMON_API
int nfp_chipdata_cppat_encode(int chip_family,
			      const union nfp_chipdata_cppat_entry *entries,
			      size_t entry_cnt,
			      int from_island,
			      int cpp_tgt,
			      int to_island,
			      uint64_t *addr);

/** Decode the address to determine the destination island ID.
 *
 * This function does nothing for NFP-32xx (returns 0).
 *
 * @param entries           Array of CPPAT entries, can be the array
 *                          returned by nfp_chipdata_cppat_entries
 * @param entry_cnt         Size of entries
 * @param from_island   The island from which the command is sent.
 *                      Specifies which island's translator to use for CPPAT.
 * @param cpp_tgt       The CPP Target ID the command is sent to.
 * @param to_island     Will be set to the destination island.
 * @param addr          A valid address from which to get the relevant
 *                      island ID bits to determine the destination
 *                      island from the specified CPPAT Table entry.
 *                      Note that for Memory Unit CPP Targets, the
 *                      address' locality bits need to be set before
 *                      calling this function.
 *
 * @return 0 on success, -ENODEV if no decoding was possible or the chip does
 *                       not have island @p from_island,
 *                       -EINVAL if any parameter was invalid.
 */
NFP_COMMON_API
int nfp_chipdata_cppat_decode(int chip_family,
			      const union nfp_chipdata_cppat_entry *entries,
			      size_t entry_cnt,
			      int from_island,
			      int cpp_tgt,
			      int *to_island,
			      uint64_t addr);

/** Get the MU Locality LSB used by the translator for a given CPPAT Table
 * entry.
 *
 * This function does nothing for NFP-32xx (returns -1).
 *
 * @param entries           Array of CPPAT entries, can be the array
 *                          returned by nfp_chipdata_cppat_entries
 * @param entry_cnt         Size of entries
 * @param from_island   The island from which the command is sent.
 *                      Specifies which island's translator
 *                      to use for CPPAT.
 *
 * @return      -1 on error, bit number otherwise.
 */
NFP_COMMON_API
int nfp_chipdata_cppat_mu_locality_lsb(int chip_family,
				       const union nfp_chipdata_cppat_entry *entries,
				       size_t entry_cnt,
				       int from_island);

/** Convert nfp_chipdata_mem_type to nfp_chipdata_mem_class.
 *
 * nfp_chipdata_mem_class can be used for more generic memory
 * type info, but generally nfp_chipdata_mem_type is more useful.
 * The reverse conversion is not possible.
 *
 * @param chip          Chip handle.
 * @param mem_type      The memory type to get the memory class of.
 */
NFP_COMMON_API
enum nfp_chipdata_mem_class nfp_chipdata_mem_type2class(
	const struct nfp_chipdata_chip *chip,
	enum nfp_chipdata_mem_type mem_type);

/** Convert nfp_chipdata_mem_type at a specific location to string.
 *
 * @param chip_family   Chip family
 * @param s             Pointer to char buffer of size
 *                      NFP_CHIPDATA_MEMID_STR_SZ.
 *                      The resulting string is output here.
 * @param mem_type      The memory type.
 * @param island_id     Island ID where mem_type resides, if applicable.
 * @param menum         Microengine number within island, zero based, where
 *                      mem_type resides, if applicable.
 * @return              Pointer to "s" on success, NULL on error.
 */
NFP_COMMON_API
const char *nfp_chipdata_mem_type2str(int chip_family,
				      char *s,
				      enum nfp_chipdata_mem_type mem_type,
				      int island_id,
				      int menum);

/** Convert a string to a memory type and location information.
 *
 * @param chip_family   Chip family
 * @param s             Memory type string, for example i33.cls
 * @param endptr        If non-NULL, *endptr will point to the trailing string
 *                      after the memory-id part of the string, which is either
 *                      an empty string or the first character after the
 *                      separating period.
 * @param island_id     Will be set to island_id if applicable, -1 if not.
 * @param menum         Will be set to microengine number withn island if
 *                      applicable, -1 if not.
 * @return              Memory type, NFP_CHIPDATA_MEM_TYPE_NONE on error.
 */
NFP_COMMON_API
enum nfp_chipdata_mem_type nfp_chipdata_idstr2mem_type(int chip_family,
						       const char *s,
						       const char **endptr,
						       int *island_id,
						       int *menum);

/** Get a config value for the given chip and key.
 *
 * Key can be anything that is a contatenation of json objects in the chip
 * definition. For example "i24.emem.size" and values are always strings.
 * The caller needs to convert the value to a specific type if needed.
 *
 * @param chip          Chip handle.
 * @param key           Config key
 * @return              Return value of config, NULL if not set.
 */
NFP_COMMON_API
const char *nfp_chipdata_cfg_val(const struct nfp_chipdata_chip *chip, const char *key);

/** Free previously loaded chip data.
 *
 * @param data      Chip data as returned by @c nfp_chipdata_load
 */
NFP_COMMON_API
void nfp_chipdata_free(struct nfp_chipdata *data);

__END_DECLS
#endif /* __NFP_CHIPDATA_H__ */
