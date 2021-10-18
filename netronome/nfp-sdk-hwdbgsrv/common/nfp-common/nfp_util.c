/*
 * Copyright (C) 2012-2014,  Netronome Systems, Inc.  All rights reserved.
 *
 * @file          nfp_resid.c
 * @brief         Functions/macros for resource identification,
 *                like microengine IDs.
 *
 */

#include <nfp-common/nfp_platform.h>
#include <nfp-common/nfp_util.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#if defined(__APPLE__) || defined(__OSX__)
#include <mach-o/dyld.h>
#include <sys/param.h>
#endif

#if defined(__FreeBSD__)
#include <sys/types.h>
#include <sys/sysctl.h>
#include <errno.h>
#endif

#ifdef _WIN32
#  define _UTL_PATH_SEP_C '\\'
#  define _UTL_PATH_SEP_S "\\"
#  define _UTL_PATH_LIST_SEP_C ';'

/* Accept both path separators on Windows */
#  define _UTL_IS_SEP(_c_) (((_c_) == '/') || ((_c_) == '\\'))
#else
#  define _UTL_PATH_SEP_C '/'
#  define _UTL_PATH_SEP_S "/"
#  define _UTL_PATH_LIST_SEP_C ':'
#  define _UTL_IS_SEP(_c_) ((_c_) == '/')
#endif

#ifdef _MSC_VER
#  define _uli_str2ull (unsigned long long)_strtoui64
#  define _uli_strdup _strdup
#else
#  define _uli_str2ull strtoull
#  define _uli_strdup strdup
#endif

const char *nfp_util_basename_c(const char *path)
{
	const char *c = NULL;

	if (!path)
		return NULL;

#ifdef _WIN32
	{
		const char *cf = NULL, *cb = NULL;

		cf = strrchr(path, '/');
		cb = strrchr(path, '\\');
		if (cf > cb)
			c = cf;
		else
			c = cb;
	}
#else
	c = strrchr(path, _UTL_PATH_SEP_C);
#endif

	if (c)
		c++;
	else
		c = path;

	return c;
}

/* Wanted something that will only clean up the path and not */
/* resolve links or check for existence. */
int nfp_util_normalize_abspath(char *path)
{
	char *c, *mark;
	char *new_path, *new_mark, *new_start;
	size_t path_sz;

	path_sz = strlen(path) + 1;
	new_path = (char *)malloc(path_sz + 16); /* arbitrary 16 bytes for UNC path safety */
	new_mark = new_path;
	new_start = new_path;

	mark = path;

#ifdef _WIN32
	/* Windows has some funny path ideas. */

	/* Check if it's UNC. */
	/* \\server\some/path */
	/* \\?\UNC\server\some\path */
	/* \\?\C:\File */
	if (strncmp(mark, "\\\\", 2) == 0) {
		strcpy(new_mark, "\\\\");
		new_mark += 2;
		while (*mark == '\\')
			mark++; /* might have been double escaped */

		if (*mark == '?') {
			*new_mark++ = '?';
			mark++;

			while (*mark == '\\')
				mark++; /* might have been double escaped */
			*new_mark++ = '\\';
			if (strncmp(mark, "UNC\\", 4) == 0) {
				strcpy(new_mark, "UNC\\");
				new_mark += 4;
				mark += 4;
				while (*mark == '\\')
					mark++; /* might have been double escaped */
			}
		}
	} else if (_UTL_IS_SEP(*mark)) {
		/* Skip past any preceding separators */
		for (; (*mark) && (_UTL_IS_SEP(*mark)); mark++)
			;

		if (*mark == '\0') {
			*new_mark++ = _UTL_PATH_SEP_C;
			goto success_cleanup; /* strange, but OK */
		}
		if (mark > path)
			mark--;
		if (!_UTL_IS_SEP(*mark))
			goto error_cleanup;
	} else {
		if (*mark == '.')
			goto cleanup;
		if (isalpha(*mark) && (mark[1] != ':'))
			goto cleanup;
	}

	/* First go past anything preceding the first separator */
	/* which would be 'C:' (or other drive, or server name) */
	for (; (*mark) && !_UTL_IS_SEP(*mark); mark++)
		*new_mark++ = *mark;

	if (*mark == '\0') {
		*new_mark++ = _UTL_PATH_SEP_C;
		goto success_cleanup; /* strange, but OK */
	}

	new_start = new_mark;
#else
	if (!_UTL_IS_SEP(*mark))
		goto cleanup;
	/* Skip past any preceding separators */
	for (; (*mark) && (_UTL_IS_SEP(*mark)); mark++)
		;

	if (*mark == '\0') {
		*new_mark++ = _UTL_PATH_SEP_C;
		goto success_cleanup; /* strange, but OK */
	}
	if (mark > path)
		mark--;
	if (!_UTL_IS_SEP(*mark))
		goto error_cleanup;
#endif

	while (*mark) {
		/* Step over one or more separators first. */
		for (; (*mark) && _UTL_IS_SEP(*mark); mark++)
			;
		if (*mark == '\0')
			break;

		/* Find next separator */
		for (c = mark; (*c) && !_UTL_IS_SEP(*c); c++)
			;

		if ((c - mark) == 0)
			goto error_cleanup;

		if (((c - mark) == 1) && (mark[0] == '.')) {
			mark = c;
			continue;
		}

		if (((c - mark) == 2) && (mark[0] == '.') && (mark[1] == '.')) {
			/* Go up one level */
			if ((new_mark - 1) > new_start)
				new_mark--;
			while ((new_mark > new_start) && (!_UTL_IS_SEP(*new_mark)))
				new_mark--;
			/* new_mark++; */
			mark = c;
			continue;
		}

		*new_mark++ = _UTL_PATH_SEP_C;
		strncpy(new_mark, mark, (c - mark));
		new_mark += (c - mark);

		mark = c;
	}

success_cleanup:
	*new_mark++ = '\0';
	memcpy(path, new_path, (new_mark - new_path));
cleanup:
	free(new_path);
	return 1;

error_cleanup:
	if (new_path)
		free(new_path);
	return 0;
}

FILE *nfp_util_data_fopen(const char *filename, const char *category)
{
	char buff[NFP_PATH_MAX];
	int err;
	const char *search;
	FILE *out;

	if (!filename)
		return NFP_ERRPTR(EINVAL);

	/* Check the current directory */

	/* Open as a binary stream */
	out = fopen(filename, "rb");
	if (!out && filename[0] != _UTL_PATH_SEP_C) {
		/* Not found in current directory, try elsewhere */
		search = getenv("NETRONOME_DIR");
		if (search) {
			if (!category)
				category = ".";

			err = snprintf(buff, sizeof(buff), "%s/share/%s/%s", search, category,
				       filename);
			if (err < sizeof(buff)) {
				/* Open as a binary stream */
				out = fopen(buff, "rb");
			} else {
				errno = EINVAL;
			}
		}
	}

	return out;
}

char *nfp_util_bin_dir()
{
	char *bin_dir = NULL;

#if defined(EMBEDDED)
	{
		ssize_t cnt;

		cnt = 1;
		bin_dir = malloc(cnt);
		if (!bin_dir)
			goto error;

		bin_dir[0] = 0;
	}
#elif defined(__linux__)
	{
		ssize_t cnt;
		ssize_t sz;

		sz = FILENAME_MAX;
		bin_dir = malloc(sz);
		if (!bin_dir)
			goto error;

		cnt = readlink("/proc/self/exe", bin_dir, sz);
		if (cnt < 0)
			goto error;

		while (cnt == sz) {
			free(bin_dir);
			sz *= 2;
			if (sz >= NFP_PATH_MAX)
				goto error;
			bin_dir = malloc(sz);
			if (!bin_dir)
				goto error;
			cnt = readlink("/proc/self/exe", bin_dir, sz);
			if (cnt < 0)
				goto error;
		}

		if (cnt == 0)
			goto error;

		/* Now zero out the filename part */
		cnt--;
		while ((cnt > 0) && (bin_dir[cnt] != '/')) {
			bin_dir[cnt] = '\0';
			cnt--;
		}
	}
#elif defined(_WIN32)
	{
		size_t cnt;
		size_t sz;

		sz = FILENAME_MAX;
		bin_dir = malloc(sz);
		if (!bin_dir)
			goto error;

		cnt = GetModuleFileName(NULL, bin_dir, sz);

		while (cnt == sz) {
			free(bin_dir);
			sz *= 2;
			if (sz >= NFP_PATH_MAX)
				goto error;
			bin_dir = malloc(sz);
			if (!bin_dir)
				goto error;
			cnt = GetModuleFileName(NULL, bin_dir, sz);
		}

		if (cnt == 0)
			goto error;

		/* Now zero out the filename part */
		if (!!strchr(bin_dir, '\\')) {
			while ((cnt > 0) && (bin_dir[cnt] != '\\')) {
				bin_dir[cnt] = '\0';
				cnt--;
			}
		}
	}
#elif defined(__APPLE__) || defined(__OSX__)
	{
		ssize_t cnt;
		char *s;
		uint32_t sz32;

		sz32 = PATH_MAX;
		bin_dir = malloc(sz32);
		if (!bin_dir)
			goto error;

		if (_NSGetExecutablePath(bin_dir, &sz32) != 0) {
			free(bin_dir);
			bin_dir = malloc(sz32);
			if (_NSGetExecutablePath(bin_dir, &sz32) != 0)
				goto error;
		}
		sz32 *= 2;
		s = malloc(sz32);
		if (!s)
			goto error;
		if (!realpath(bin_dir, s)) {
			free(s);
			goto error;
		}
		free(bin_dir);
		bin_dir = s;
		cnt = strlen(bin_dir);
		while ((cnt > 0) && (bin_dir[cnt] != '/')) {
			bin_dir[cnt] = '\0';
			cnt--;
		}
	}
#elif defined(__FreeBSD__)
	{
		ssize_t cnt;
		int mib[4];

		mib[0] = CTL_KERN;
		mib[1] = KERN_PROC;
		mib[2] = KERN_PROC_PATHNAME;
		mib[3] = -1;

		sz = 1024;
		bin_dir = malloc(sz);
		if (!bin_dir)
			goto error;
		cnt = sz;
		while (sysctl(mib, 4, bin_dir, &cnt, NULL, 0) != 0) {
			free(bin_dir);
			sz *= 2;
			if (sz >= NFP_PATH_MAX)
				goto error;
			bin_dir = malloc(sz);
			if (!bin_dir)
				goto error;
			cnt = sz;
		}

		cnt--;
		while ((cnt > 0) && (bin_dir[cnt] != '/')) {
			bin_dir[cnt] = '\0';
			cnt--;
		}
	}
#else
#  error "Unsupported platform."
#endif

	return bin_dir;

error:
	if (bin_dir)
		free(bin_dir);
	bin_dir = NULL;

	return NULL;
}

static int _utl_digit2val(char digit, int base)
{
	switch (digit) {
	case '0':
	case '1':
		if (base == 2)
			return (int)digit - '0';
	/* fall through */
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
		if (base == 8)
			return (int)digit - '0';
	/* fall through */
	case '8':
	case '9':
		if (base == 16)
			return (int)digit - '0';
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		if (base == 16)
			return ((int)digit - 'a') + 10;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		if (base == 16)
			return ((int)digit - 'A') + 10;
	default:
		break;
	}
	return -1;
}

static char _utl_val2digit(int val, int base)
{
	switch (val) {
	case 0:
	case 1:
		if (base == 2)
			return (char)(val + '0');
	/* fall through */
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		if (base == 8)
			return (char)(val + '0');
	/* fall through */
	case 8:
	case 9:
		if (base == 16)
			return (char)(val + '0');
	case 0xa:
	case 0xb:
	case 0xc:
	case 0xd:
	case 0xe:
	case 0xf:
		if (base == 16)
			return (char)((val - 10) + 'a');
	default:
		break;
	}
	return '?';
}

int nfp_util_str2lrgint(const char *s, uint32_t **vals, size_t *word_cnt)
{
	int err = EINVAL;
	uint32_t *valarr = NULL;
	size_t val_sz = 0;
	size_t idx;
	char *c;
	unsigned long long ull;
	int base = 0;

	if ((!s) || (!vals) || (!word_cnt))
		goto error_cleanup;

	/* Negative values are not supported for simplicity */
	if ((*s == '-') || (*s == '\0'))
		goto error_cleanup;

	/* Determine base and skip prefix chars. strtoull does this, */
	/* but if we do it here we support binrary via strtoull and */
	/* also have it ready for the actual large ints. */
	if (*s != '0') {
		base = 10;
	} else {
		s++;
		switch (*s) {
		case 'x':
		case 'X':
			base = 16;
			s++;
			if (*s == '\0')
				goto error_cleanup;
			break;
		case 'b':
		case 'B':
			base = 2;
			s++;
			if (*s == '\0')
				goto error_cleanup;
			break;
		default:
			base = 8;
			break;
		}
	}

	/* First try to eval it with strtoull. */
	/* This will cover most cases, including 64-bit decimal */
	errno = 0;
	ull = _uli_str2ull(s, &c, base);
	err = errno;
	if ((*c == '\0') && (err == 0)) {
		unsigned long long tull = ull;

		val_sz = 0;
		while (tull) {
			tull >>= 32; /* Very important to be unsigned here */
			val_sz++;
		}
		if (val_sz == 0)
			val_sz = 1;

		valarr = calloc(val_sz, sizeof(valarr[0]));
		if (!valarr) {
			err = ENOMEM;
			goto error_cleanup;
		}

		for (idx = 0; idx < val_sz; idx++)
			valarr[idx] = (ull >> (idx * 32)) & 0xFFFFffff;
	} else {
		size_t digit_cnt = 0;
		const char *cc;
		int lsb;

		err = EINVAL;

		/* Skip leading zeroes and underscores. */
		while ((*s == '0') || (*s == '_'))
			s++;

		/* Count useful digits */
		for (cc = s; (*cc); cc++)
			if (*cc != '_')
				digit_cnt++;
		cc--; /* cc now points to last char */

		switch (base) {
		case 16:
			val_sz = (digit_cnt + 7) / 8;
			valarr = calloc(val_sz, sizeof(valarr[0]));
			if (!valarr) {
				err = ENOMEM;
				goto error_cleanup;
			}
			lsb = 0;

			while (cc >= s) {
				int v;

				if (*cc == '_') {
					cc--;
					continue;
				}
				idx = lsb / 32;
				v = _utl_digit2val(*cc, base);
				if (v == -1)
					goto error_cleanup;
				valarr[idx] |= (v << (lsb % 32));
				/* printf("idx %d, lsb %d, cc %c, v 0x%x\n", (int)idx, lsb, *cc,
				 * valarr[idx]);
				 */
				lsb += 4;
				cc--;
			}

			break;
		case 8:
			val_sz = ((digit_cnt - 1) * 3);
			switch (*s) {
			case '0':
			case '1':
				val_sz += 1;
				break;
			case '2':
			case '3':
				val_sz += 2;
				break;
			default:
				val_sz += 3;
				break;
			}
			val_sz = (val_sz + 31) / 32; /* from bits to words */

			valarr = calloc(val_sz, sizeof(valarr[0]));
			if (!valarr) {
				err = ENOMEM;
				goto error_cleanup;
			}
			lsb = 0;

			while (cc >= s) {
				int v;
				int wlsb = lsb % 32;

				if (*cc == '_') {
					cc--;
					continue;
				}
				idx = lsb / 32;
				v = _utl_digit2val(*cc, base);
				if (v == -1)
					goto error_cleanup;
				if (((wlsb + 3) > 32) && ((idx + 1) < val_sz)) {
					/* Rely on 32-bit overflow truncating shifted value */
					valarr[idx] |= (uint32_t)(v << wlsb);
					valarr[idx + 1] |= (uint32_t)(v >> (32 - wlsb));
				} else {
					valarr[idx] |= (uint32_t)(v << wlsb);
				}
				/* printf("idx %d, lsb %d, cc %c, v 0x%x\n", (int)idx, lsb, *cc,
				 * valarr[idx]);
				 */
				lsb += 3;
				cc--;
			}

			break;
		case 2:
			val_sz = (digit_cnt + 31) / 32;
			valarr = calloc(val_sz, sizeof(valarr[0]));
			if (!valarr) {
				err = ENOMEM;
				goto error_cleanup;
			}
			lsb = 0;

			while (cc >= s) {
				int v;

				if (*cc == '_') {
					cc--;
					continue;
				}
				idx = lsb / 32;
				v = _utl_digit2val(*cc, base);
				if (v == -1)
					goto error_cleanup;
				valarr[idx] |= (v << (lsb % 32));
				/* printf("idx %d, lsb %d, cc %c, v 0x%x\n", (int)idx, lsb, *cc,
				 * valarr[idx]);
				 */
				lsb += 1;
				cc--;
			}
			break;
		default:
			goto error_cleanup;
		}
	}

	*vals = valarr;
	*word_cnt = val_sz;

	return 0;

error_cleanup:

	if (valarr)
		free(valarr);
	return -err;
}

int nfp_util_lrgint2str(char **s, const uint32_t *vals, size_t word_cnt, int base, int group_cnt)
{
	char *new_str = NULL;
	int err = EINVAL;
	uint32_t _u32;
	int bit_len = 0;
	size_t val_idx, digit_cnt, digit_idx, str_idx;
	int lsb;

	if ((!s) || (!vals) || (word_cnt == 0))
		goto error_cleanup;

	/* Start from highest word and find first non-zero */
	for (val_idx = word_cnt - 1; val_idx > 0; val_idx--) {
		if (vals[val_idx] != 0)
			break;
	}
	_u32 = vals[val_idx];
	while (_u32) {
		bit_len++;
		_u32 >>= 1;
	}
	if (val_idx > 0)
		bit_len += val_idx * sizeof(vals[0]) * 8;

	if (bit_len == 0) {
		*s = _uli_strdup("0");
		return 0;
	}

	switch (base) {
	case 16:
		if (group_cnt < 0)
			group_cnt = 16; /* 64-bit */
		digit_cnt = (bit_len + 3) / 4;
		if (group_cnt != 0)
			str_idx = digit_cnt + ((digit_cnt + (group_cnt - 1)) / group_cnt) - 1;
		else
			str_idx = digit_cnt;
		new_str = malloc(str_idx + 1);
		new_str[str_idx--] = '\0';

		for (digit_idx = 0; digit_idx < digit_cnt; digit_idx++) {
			lsb = (digit_idx * 4);

			if (group_cnt < 0) {
				if (((lsb % 64) == 0) && (lsb > 0))
					new_str[str_idx--] = '_';
			} else if (group_cnt > 0) {
				if (((digit_idx % group_cnt) == 0) && (digit_idx > 0))
					new_str[str_idx--] = '_';
			}

			val_idx = (lsb / 32);
			_u32 = (vals[val_idx] >> (lsb % 32)) & 0xF;
			new_str[str_idx--] = _utl_val2digit((int)_u32, base);
		}

		break;
	case 8:
		if (group_cnt < 0)
			group_cnt = 21; /* 63-bit */
		digit_cnt = (bit_len + 2) / 3;
		if (group_cnt != 0)
			str_idx = digit_cnt + ((digit_cnt + (group_cnt - 1)) / group_cnt) - 1;
		else
			str_idx = digit_cnt;
		new_str = malloc(str_idx + 1);
		new_str[str_idx--] = '\0';

		for (digit_idx = 0; digit_idx < digit_cnt; digit_idx++) {
			int wlsb;

			lsb = (digit_idx * 3);
			if (group_cnt < 0) {
				/* Group by digits sine 3 bits don't really align nicely */
				/* into power of 2 presentation */
				if (((digit_idx % 21) == 0) && (digit_idx > 0))
					new_str[str_idx--] = '_';
			} else if (group_cnt > 0) {
				if (((digit_idx % group_cnt) == 0) && (digit_idx > 0))
					new_str[str_idx--] = '_';
			}

			wlsb = lsb % 32;
			val_idx = (lsb / 32);
			if (((wlsb + 3) > 32) && ((val_idx + 1) < word_cnt)) {
				_u32 = (vals[val_idx] >> wlsb) & 0x7;
				_u32 |= (vals[val_idx + 1] << (32 - wlsb)) & 0x7;
			} else {
				_u32 = (vals[val_idx] >> wlsb) & 0x7;
			}
			new_str[str_idx--] = _utl_val2digit((int)_u32, base);
		}

		break;
	case 2:
		if (group_cnt < 0)
			group_cnt = 64; /* 64-bit */
		digit_cnt = bit_len;
		if (group_cnt != 0)
			str_idx = digit_cnt + ((digit_cnt + (group_cnt - 1)) / group_cnt) - 1;
		else
			str_idx = digit_cnt;
		new_str = malloc(str_idx + 1);
		new_str[str_idx--] = '\0';

		for (digit_idx = 0; digit_idx < digit_cnt; digit_idx++) {
			if (group_cnt < 0) {
				if (((digit_idx % 64) == 0) && (digit_idx > 0))
					new_str[str_idx--] = '_';
			} else if (group_cnt > 0) {
				if (((digit_idx % group_cnt) == 0) && (digit_idx > 0))
					new_str[str_idx--] = '_';
			}

			lsb = digit_idx;
			val_idx = (lsb / 32);
			_u32 = (vals[val_idx] >> (lsb % 32)) & 0x1;
			new_str[str_idx--] = _utl_val2digit((int)_u32, base);
		}

		break;
	case 10:
		/* Assume the maximum digit count for 64-bit decimal */
		/* and use str_idx as temp variable. */
		/* 18 446 744 073 709 551 615 -> 20 */
		{
			unsigned long long ull = 0;

			for (val_idx = 0; val_idx < word_cnt; val_idx++) {
				if ((val_idx * sizeof(vals[0])) > sizeof(ull))
					break;
				ull |= ((unsigned long long)vals[val_idx])
				       << (val_idx * sizeof(vals[0]) * 8);
			}
			new_str = malloc(20 + 1);
			snprintf(new_str, 20 + 1, "%llu", ull);
			new_str[20 + 1 - 1] = 0;
		}
		break;
	default:
		break;
	}

	*s = new_str;
	return 0;
error_cleanup:

	if (new_str)
		free(new_str);
	return -err;
}

int nfp_util_lrgint_get(const uint32_t *vals, int msb, int lsb, uint32_t *out_vals)
{
	int out_lsb = 0;
	int bit_len;
	uint32_t mask;

	if ((!vals) || (!out_vals) || (msb < lsb) || (lsb < 0))
		return -EINVAL;

	while (lsb <= msb) {
		if ((msb / 32) > (lsb / 32))
			bit_len = 32 - (lsb % 32);
		else
			bit_len = msb - lsb + 1;
		if (((out_lsb + bit_len - 1) / 32) > (out_lsb / 32))
			bit_len = 32 - (out_lsb % 32);

		if (bit_len == 32)
			mask = UINT32_C(0xFFFFffff);
		else
			mask = ((UINT32_C(1) << bit_len) - 1);

		if ((out_lsb % 32) == 0)
			out_vals[out_lsb / 32] = (vals[lsb / 32] >> (lsb % 32)) & mask;
		else
			out_vals[out_lsb / 32] |= ((vals[lsb / 32] >> (lsb % 32)) & mask)
						  << (out_lsb % 32);

		out_lsb += bit_len;
		lsb += bit_len;
	}

	return 0;
}

uint64_t nfp_util_lrgint_get64(const uint32_t *vals, int msb, int lsb)
{
	uint32_t v[2];

	if ((msb - lsb) > 63)
		return UINT64_MAX;

	if (nfp_util_lrgint_get(vals, msb, lsb, v) != 0)
		return UINT64_MAX;

	return ((uint64_t)v[1] << 32) | v[0];
}

int nfp_util_lrgint_bitop(uint32_t *vals, int msb, int lsb, char bitop, const uint32_t *in_vals)
{
	int in_lsb = 0;
	int bit_len;
	uint32_t mask, v;

	if ((!vals) || (!in_vals) || (msb < lsb) || (lsb < 0))
		return -EINVAL;

	while (lsb <= msb) {
		if ((msb / 32) > (lsb / 32))
			bit_len = 32 - (lsb % 32);
		else
			bit_len = msb - lsb + 1;
		if (((in_lsb + bit_len - 1) / 32) > (in_lsb / 32))
			bit_len = 32 - (in_lsb % 32);

		if (bit_len == 32)
			mask = UINT32_C(0xFFFFffff);
		else
			mask = ((UINT32_C(1) << bit_len) - 1);

		v = (((in_vals[in_lsb / 32] >> (in_lsb % 32)) & mask) << (lsb % 32));
		switch (bitop) {
		case '|':
			vals[lsb / 32] |= v;
			break;
		case '&':
			vals[lsb / 32] &= v;
			break;
		case '^':
			vals[lsb / 32] ^= v;
			break;
		default:
			break;
		}

		in_lsb += bit_len;
		lsb += bit_len;
	}

	return 0;
}

int nfp_util_lrgint_clr(uint32_t *vals, int msb, int lsb)
{
	/* int in_lsb = 0; */
	int bit_len;
	uint32_t mask;

	if ((!vals) || (msb < lsb) || (lsb < 0))
		return -EINVAL;

	while (lsb <= msb) {
		if ((msb / 32) > (lsb / 32))
			bit_len = 32 - (lsb % 32);
		else
			bit_len = msb - lsb + 1;
		/* if ( ((in_lsb + bit_len - 1) / 32) > (in_lsb / 32) ) */
		/* bit_len = 32 - (in_lsb % 32); */

		if (bit_len == 32)
			mask = UINT32_C(0xFFFFffff);
		else
			mask = ((UINT32_C(1) << bit_len) - 1);

		vals[lsb / 32] &= ~(mask << (lsb % 32));
		/* in_lsb += bit_len; */
		lsb += bit_len;
	}

	return 0;
}

int nfp_util_lrgint_set(uint32_t *vals, int msb, int lsb)
{
	/* int in_lsb = 0; */
	int bit_len;
	uint32_t mask;

	if ((!vals) || (msb < lsb) || (lsb < 0))
		return -EINVAL;

	while (lsb <= msb) {
		if ((msb / 32) > (lsb / 32))
			bit_len = 32 - (lsb % 32);
		else
			bit_len = msb - lsb + 1;

		if (bit_len == 32)
			mask = UINT32_C(0xFFFFffff);
		else
			mask = ((UINT32_C(1) << bit_len) - 1);

		vals[lsb / 32] |= (mask << (lsb % 32));
		lsb += bit_len;
	}

	return 0;
}

int nfp_util_lrgint_test(uint32_t *vals, int bit)
{
	if ((!vals) || (bit < 0))
		return 0;

	return (vals[bit / 32] >> (bit % 32)) & 1;
}

int nfp_util_lrgint_mask(uint32_t *vals, size_t word_cnt, int msb, int lsb, int inv)
{
	int word_idx;
	int bit_len;
	uint32_t mask;

	if ((!vals) || (msb < lsb) || (lsb < 0))
		return -EINVAL;

	if (!inv) {
		for (word_idx = 0; word_idx < (lsb / 32); word_idx++)
			vals[word_idx] = 0;
		for (word_idx = (msb / 32) + 1; (size_t)word_idx < word_cnt; word_idx++)
			vals[word_idx] = 0;
	}

	while (lsb <= msb) {
		if ((msb / 32) > (lsb / 32))
			bit_len = 32 - (lsb % 32);
		else
			bit_len = msb - lsb + 1;

		if (bit_len == 32)
			mask = UINT32_C(0xFFFFffff);
		else
			mask = ((UINT32_C(1) << bit_len) - 1);

		if (inv)
			vals[lsb / 32] &= ~(mask << (lsb % 32));
		else
			vals[lsb / 32] &= (mask << (lsb % 32));
		lsb += bit_len;
	}

	return 0;
}
