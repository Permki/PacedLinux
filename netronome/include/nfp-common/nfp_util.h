/*
 * Copyright (C) 2014,2017  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_util.h
 * @brief         General portable utility functions
 *
 */

#ifndef __NFP_UTIL_H__
#define __NFP_UTIL_H__

#include <stdio.h>
#include <nfp-common/nfp_platform.h>

#include "_nfp_common_apiex.h"

__BEGIN_DECLS

/** Get the current executable's directory.
 *
 * The returned path will contain a trailing path separator
 * and must be freed by the caller (free()).
 *
 * @return Return NULL on failure, a new char array on success.
 */
NFP_COMMON_API
char *nfp_util_bin_dir();

/** Return pointer to file name part of path.
 *  utl_path_basename_c("/a/b/c.d") == "c.d"
 *  utl_path_basename_c("abc") == "abc"
 *
 * @return NULL on error, basename pointer on success
 */
NFP_COMMON_API
const char *nfp_util_basename_c(const char *path);

/** Clean up absolute path to have no relative references in the middle.
 * /a/b/c/./d/../e//../../f -> /a/b/f
 *
 * Links are not resolved, no exsitence check is performed.
 * The result will not have a trailing path separator.
 *
 * When no parent directory exists when encountering '..', the
 * path will remain at root (or drive/UNC root on Windows).
 *
 * Repeated separators are replaced with a single separator.
 * On Windows, UNC path prefixes are checked for an accepted, but only
 * backslashes are valid (\\server, \\?\server, \\?\UNC\server) in the prefix.
 *
 * On windows, all forward slashes will be converted to back slashes.
 *
 * @return 1 on success, 0 on failure.
 */
NFP_COMMON_API
int nfp_util_normalize_abspath(char *path);

/** Open a library data file
 *
 * Given a filename, opens the named library configuration file.
 * The file will be opened with O_RDONLY.
 *
 * Use fclose(3) to close the descriptor.
 *
 *  @param filename     Filename to open
 *  @param category     Category of configuration file:
 *                        "nbi" - NBI JSON file
 *
 *  @return             fd (!= NULL) on success, NULL and errno for error
 */
NFP_COMMON_API
FILE *nfp_util_data_fopen(const char *filename, const char *category);

/** Convert an arbitrarily long positive string to a 32-bit integer array.
 *
 *  For decimal (base 10) format strings, only up to unsigned long long
 *  integers are supported. For larger than unsigned long long, bases
 *  16, 8 and 2 are supported (hexadecimal, octal and binary).
 *
 *  Negative integers are not supported.
 *
 *  The supported formats are:
 *  - 01234 = 1234_base8 = 668_base10
 *  - 0x1234 = 1234_base16 = 4660_base10
 *  - 0b11011 = 11011_base2 = 27_base10
 *
 *  For bases 2, 8 and 16, underscores (_) may be inserted at any point
 *  after the initial format specifier to aid readability. For example:
 *  - 0x11223344_55667788_99aabbcc
 *  - 0b1111_1011__1010_1011
 *  - 0777_655_101
 *
 *  @param s            The input string.
 *  @param vals         Will be set to a newly allocated array containing
 *                      the integer values. Caller must use free() to free
 *                      memory when done. Words are least-significant
 *                      first - vals[0] is bits 0 to 31, vals[1] is bits
 *                      32 to 63.
 *  @param word_cnt     The number of 32-bit words in the @p vals array.
 *  @return             0 on success, negative errno value on failure.
 */
NFP_COMMON_API
int nfp_util_str2lrgint(const char *s, uint32_t **vals, size_t *word_cnt);

/** Convert an arbitrarily long positive integer to a string.
 *
 *  For decimal (base 10), only up to unsigned long long integers are
 *  supported and digit grouping is not used.
 *
 *  The output string's digits can be separated into groups with an
 *  underscore separator. The output string is not prefixed with a format
 *  specifier.
 *
 *  @param s            The output string. Caller must use free() to free
 *                      the string.
 *  @param vals         The input value array, least significant word in
 *                      vals[0].
 *  @param word_cnt     Number of 32-bit words in @p vals array.
 *  @param base         Conversion base. Can be 2, 8, 10 or 16.
 *  @param group_cnt    If 0, no underscores are inserted. If -1, digits are
 *                      grouped so that each group is at most 64 bits. For
 *                      @p group_cnt larger than 0, digits are grouped by
 *                      that count, separated by underscores.
 *  @return             0 on success, negative errno value on failure.
 */
NFP_COMMON_API
int nfp_util_lrgint2str(char **s,
			const uint32_t *vals,
			size_t word_cnt,
			int base,
			int group_cnt);

/** Extract a range of bits from a large integer into another large integer.
 *
 *  Performs:
 *  mask = (1 << (msb - lsb + 1)) - 1
 *  out_vals = (vals >> lsb) & mask
 *
 *  The user must ensure that the sizes or the two integer arrays must be
 *  large enough.
 *
 *  @param vals         Integer array to get bit range from. Must have at
 *                      least (msb / 32) words.
 *  @param msb          The most significant bit to extract from.
 *  @param lsb          The least significant bit to extract to.
 *  @param out_vals     Integer array to store extracted bits in. The
 *                      extracted LSB will be stored at bit 0 in this array.
 *  @return             0 on success, -EINVAL for invalid parameters.
 */
NFP_COMMON_API
int nfp_util_lrgint_get(const uint32_t *vals,
			int msb,
			int lsb,
			uint32_t *out_vals);

/** Extract a range of up to 64 bits from a large integer.
 *
 *  Performs:
 *  mask = (1 << (msb - lsb + 1)) - 1
 *  return = (vals >> lsb) & mask
 *
 *  The user must ensure that the sizes or the two integer arrays must be
 *  large enough.
 *
 *  @param vals         Integer array to get bit range from. Must have at
 *                      least (msb / 32) words.
 *  @param msb          The most significant bit to extract from.
 *  @param lsb          The least significant bit to extract to.
 *  @return             The extracted value, UINT64_MAX for invalid parameters
 *                      or if the extracted value is in fact UINT64_MAX.
 */
NFP_COMMON_API
uint64_t nfp_util_lrgint_get64(const uint32_t *vals, int msb, int lsb);

/** Perform bit-wise operation of one large integer into another.
 *
 *  Performs:
 *  mask = (1 << (msb - lsb + 1)) - 1
 *  vals = vals 'op' ((in_vals & mask) << lsb)
 *  where 'op' is an operand.
 *
 *  @param vals         Integer array to OR bit range into. Must have at
 *                      least (msb / 32) words.
 *  @param msb          The most significant bit to extract from.
 *  @param lsb          The least significant bit to extract to.
 *  @param bitop        Operator to use. One of '|', '&', '^'.
 *  @param in_vals      Integer array to get input bits from. Bit 0
 *                      will be set at @p lsb in @p vals.
 *  @return             0 on success, -EINVAL for invalid parameters.
 */
NFP_COMMON_API
int nfp_util_lrgint_bitop(uint32_t *vals,
			  int msb,
			  int lsb,
			  char bitop,
			  const uint32_t *in_vals);

/** Clear a range of bits in a large integer.
 *
 *  Performs:
 *  mask = (1 << (msb - lsb + 1)) - 1
 *  vals = vals & ~(mask << lsb)
 *
 *  @param vals         Integer array to clear bits in. Must have at
 *                      least (msb / 32) words.
 *  @param msb          The most significant bit to extract from.
 *  @param lsb          The least significant bit to extract to.
 *  @return             0 on success, -EINVAL for invalid parameters.
 */
NFP_COMMON_API
int nfp_util_lrgint_clr(uint32_t *vals, int msb, int lsb);

/** Set a range of bits in a large integer.
 *
 *  Performs:
 *  mask = (1 << (msb - lsb + 1)) - 1
 *  vals = vals | (mask << lsb)
 *
 *  @param vals         Integer array to set bits in. Must have at
 *                      least (msb / 32) words.
 *  @param msb          The most significant bit to extract from.
 *  @param lsb          The least significant bit to extract to.
 *  @return             0 on success, -EINVAL for invalid parameters.
 */
NFP_COMMON_API
int nfp_util_lrgint_set(uint32_t *vals, int msb, int lsb);

/** Test if a bit is set in a large integer;
 *
 *  @param vals         Integer array to set bits in. Must have at
 *                      least (msb / 32) words.
 *  @param bit          The bit to test.
 *  @return             1 if set, 0 otherwise.
 */
NFP_COMMON_API
int nfp_util_lrgint_test(uint32_t *vals, int bit);

/** Mask a range of bits in a large integer.
 *
 *  Performs:
 *  mask = (1 << (msb - lsb + 1)) - 1
 *  if (inv)
 *    vals = vals & ~(mask << lsb)
 *  else
 *    vals = vals & (mask << lsb)
 *
 *  @param vals         Integer array to set bits in. Must have at
 *                      least (msb / 32) words.
 *  @param word_cnt     Number of 32-bit words in @p vals array.
 *  @param msb          The most significant bit to extract from.
 *  @param lsb          The least significant bit to extract to.
 *  @param inv          If 0, masked bits are kept. If non-zero, masked bits
 *                      are cleared.
 *  @return             0 on success, -EINVAL for invalid parameters.
 */
NFP_COMMON_API
int nfp_util_lrgint_mask(uint32_t *vals,
			 size_t word_cnt,
			 int msb,
			 int lsb,
			 int inv);

__END_DECLS

#endif /* __NFP_UTIL_H__ */
