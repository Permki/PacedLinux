/**
 * Copyright (C) 2008-2017 Netronome Systems, Inc.  All rights reserved.
 *
 * @file nfp_elf.c
 *
 */

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <errno.h>
#include <inttypes.h>
#include <nfp-common/nfp_platform.h>
#include <nfp-common/ns_log.h>
#include <nfp-common/nfp_elf.h>

/* SHT_NOTE sections are not byte-swapped on load. Such sections are only
 * processed once on buffer input.
 */

#if (!defined(_FILE_OFFSET_BITS) || (_FILE_OFFSET_BITS != 64))
#error "This file requires _FILE_OFFSET_BITS=64."
#endif

#ifndef _LARGEFILE_SOURCE
#error "This file requires _LARGEFILE_SOURCE."
#endif

/* ==== Convenience macros ==== */

#define NFP_ELF_USTORE_ECC_POLY_WORDCNT 7
#define NFP_ELF_USTORE_WORDSIZE 45

#define NFP_ELF_UNUSED(_x_) (void)(_x_)

#define NFP_ELF_NUM64_CAN_BE_NUM32(c64)                        \
	((((uint64_t)(c64) > UINT64_C(0xFFFFffff00000000))) || \
	 (((uint64_t)(c64) < UINT64_C(0x100000000))))

#define NFP_ELF_EXPR_STACK_SIZE 64

#define NFP_ELF_SEC_ENT_CNT(ctx, idx)                                         \
	(((ctx)->shdrs[idx].sh_entsize) ?                                     \
		 ((ctx)->shdrs[idx].sh_size / (ctx)->shdrs[idx].sh_entsize) : \
		 0)

#define NFP_ELF_BITS_SET(word, val, mask, shift) \
	(((word) & ~((mask) << (shift))) | (((val) & (mask)) << (shift)))
#define NFP_ELF_BITS_GET(word, mask, shift) (((word) >> (shift)) & (mask))

#define NSE_CALLOC_OOM(_dst_, _n_, _sz_)                            \
	do {                                                        \
		if (((_sz_) == 0) || ((_n_) == 0)) {                \
			_dst_ = NULL;                               \
			break;                                      \
		}                                                   \
		_dst_ = calloc((_n_), (_sz_));                      \
		if ((_dst_) == NULL) {                              \
			err = errno;                                \
			NS_LOG(NS_LOG_LVL_FATAL, "Out of memory."); \
			goto error_cleanup;                         \
		}                                                   \
	} while (0)

#define NSE_MALLOC_OOM(_dst_, _sz_)                                 \
	do {                                                        \
		if ((_sz_) == 0) {                                  \
			_dst_ = NULL;                               \
			break;                                      \
		}                                                   \
		_dst_ = malloc((_sz_));                             \
		if ((_dst_) == NULL) {                              \
			err = errno;                                \
			NS_LOG(NS_LOG_LVL_FATAL, "Out of memory."); \
			goto error_cleanup;                         \
		}                                                   \
	} while (0)

#define NSE_FREAD(_dst_, _sz_, _cnt_, _f_)                            \
	do {                                                          \
		size_t rcnt;                                          \
		rcnt = fread((_dst_), (_sz_), (_cnt_), (_f_));        \
		if (rcnt != (_cnt_)) {                                \
			err = errno;                                  \
			if (err == 0)                                 \
				err = EIO;                            \
			NS_LOG(NS_LOG_LVL_FATAL, "File read error."); \
			goto error_cleanup;                           \
		}                                                     \
	} while (0)

#define NSE_FWRITE(_data_, _sz_, _cnt_, _f_)                           \
	do {                                                           \
		size_t rcnt;                                           \
		rcnt = fwrite((_data_), (_sz_), (_cnt_), (_f_));       \
		if (rcnt != (_cnt_)) {                                 \
			err = errno;                                   \
			if (err == 0)                                  \
				err = EIO;                             \
			NS_LOG(NS_LOG_LVL_FATAL, "File write error."); \
			goto error_cleanup;                            \
		}                                                      \
	} while (0)

#define NSE_FSEEK(_f_, _pos_, _whence_)                               \
	do {                                                          \
		if (nfp_fseek64((_f_), (_pos_), (_whence_)) != 0) {   \
			err = EIO;                                    \
			NS_LOG(NS_LOG_LVL_FATAL, "File seek error."); \
			goto error_cleanup;                           \
		}                                                     \
	} while (0)

static const char *_nfp_elf_name_not_found = "<no name>";

static const struct nfp_elf_impexpr_field _nfp3200_sdk_elf_impexpr_field_map[] = {
	{ 1, NFP_ELF_R_NFP3200_W32LE, 32, 0, '=', {31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_SRC8_A, 64, 0, '=', {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_SRC8_B, 64, 0, '=', {17, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_IMMED8_I, 64, 0, '=', {27, 20, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_SC, 64, 0, '=', {9, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 2, NFP_ELF_R_NFP3200_IMMED_LO16_I_A, 64, 0, '=', {27, 20, 8, 7, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 2, NFP_ELF_R_NFP3200_IMMED_LO16_I_B, 64, 0, '=', {27, 20, 8, 17, 10, 0, 0, 0, 0, 0, 0, 0} },
	{ 2, NFP_ELF_R_NFP3200_IMMED_HI16_I_A, 64, 0, '=', {27, 20, 24, 7, 0, 16, 0, 0, 0, 0, 0, 0} },
	{ 2, NFP_ELF_R_NFP3200_IMMED_HI16_I_B, 64, 0, '=', {27, 20, 24, 17, 10, 16, 0, 0, 0, 0, 0, 0} },
	{ 2, NFP_ELF_R_NFP3200_SRC7_B, 64, 0, '=', {17, 16, 5, 14, 10, 0, 0, 0, 0, 0, 0, 0} },
	{ 2, NFP_ELF_R_NFP3200_SRC7_A, 64, 0, '=', {7, 6, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 3, NFP_ELF_R_NFP3200_SRC8_I_B, 64, 0, '=', {18, 18, 7, 17, 16, 5, 14, 10, 0, 0, 0, 0} },
	{ 3, NFP_ELF_R_NFP3200_SRC8_I_A, 64, 0, '=', {18, 18, 7, 7, 6, 5, 4, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_W64LE, 64, 0, '=', {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_W64BE, 64, 1, '=', {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_W32LE_AND, 32, 0, '&', {31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_W32LE_OR, 32, 0, '|', {31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_W32BE_AND, 32, 1, '&', {31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_W32BE_OR, 32, 1, '|', {31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_W64LE_AND, 64, 0, '&', {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_W64LE_OR, 64, 0, '|', {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_W64BE_AND, 64, 1, '&', {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP3200_W64BE_OR, 64, 1, '|', {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }
};

static const size_t _nfp3200_sdk_elf_impexpr_field_map_cnt = ARRAY_SIZE(_nfp3200_sdk_elf_impexpr_field_map);

static const struct nfp_elf_impexpr_field _nfp6000_sdk_elf_impexpr_field_map[] = {
	{ 1, NFP_ELF_R_NFP6000_W32LE, 32, 0, '=', {31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_W32BE, 32, 1, '=', {31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_SRC8_A, 64, 0, '=', {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_SRC8_B, 64, 0, '=', {17, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_IMMED8_I, 64, 0, '=', {27, 20, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_SC, 64, 0, '=', {9, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 2, NFP_ELF_R_NFP6000_IMMED_LO16_I_A, 64, 0, '=', {27, 20, 8, 7, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 2, NFP_ELF_R_NFP6000_IMMED_LO16_I_B, 64, 0, '=', {27, 20, 8, 17, 10, 0, 0, 0, 0, 0, 0, 0} },
	{ 2, NFP_ELF_R_NFP6000_IMMED_HI16_I_A, 64, 0, '=', {27, 20, 24, 7, 0, 16, 0, 0, 0, 0, 0, 0} },
	{ 2, NFP_ELF_R_NFP6000_IMMED_HI16_I_B, 64, 0, '=', {27, 20, 24, 17, 10, 16, 0, 0, 0, 0, 0, 0} },
	{ 2, NFP_ELF_R_NFP6000_SRC7_B, 64, 0, '=', {17, 16, 5, 14, 10, 0, 0, 0, 0, 0, 0, 0} },
	{ 2, NFP_ELF_R_NFP6000_SRC7_A, 64, 0, '=', {7, 6, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 3, NFP_ELF_R_NFP6000_SRC8_I_B, 64, 0, '=', {18, 18, 7, 17, 16, 5, 14, 10, 0, 0, 0, 0} },
	{ 3, NFP_ELF_R_NFP6000_SRC8_I_A, 64, 0, '=', {18, 18, 7, 7, 6, 5, 4, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_W64LE, 64, 0, '=', {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_W64BE, 64, 1, '=', {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 0, NFP_ELF_R_NFP6000_SH_INFO, 32, 0, '=', {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_W32_29_24, 32, 0, '=', {29, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_W32LE_AND, 32, 0, '&', {31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_W32LE_OR, 32, 0, '|', {31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_W32BE_AND, 32, 1, '&', {31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_W32BE_OR, 32, 1, '|', {31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_W64LE_AND, 64, 0, '&', {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_W64LE_OR, 64, 0, '|', {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_W64BE_AND, 64, 1, '&', {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
	{ 1, NFP_ELF_R_NFP6000_W64BE_OR, 64, 1, '|', {63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }
};

static const size_t _nfp6000_sdk_elf_impexpr_field_map_cnt = ARRAY_SIZE(_nfp6000_sdk_elf_impexpr_field_map);

enum _nfp_elf_token_type_e {
	_NFP_ELF_TOK_NONE = 0,
	_NFP_ELF_TOK_VALUE,
	_NFP_ELF_TOK_STRING,
	_NFP_ELF_TOK_ADD,
	_NFP_ELF_TOK_SUB,
	_NFP_ELF_TOK_MUL,
	_NFP_ELF_TOK_DIV,
	_NFP_ELF_TOK_MOD,
	_NFP_ELF_TOK_NEG,
	_NFP_ELF_TOK_BITAND,
	_NFP_ELF_TOK_BITOR,
	_NFP_ELF_TOK_BITXOR,
	_NFP_ELF_TOK_BITINV,
	_NFP_ELF_TOK_AND,
	_NFP_ELF_TOK_OR,
	_NFP_ELF_TOK_NOT,
	_NFP_ELF_TOK_EQ,
	_NFP_ELF_TOK_NEQ,
	_NFP_ELF_TOK_LT,
	_NFP_ELF_TOK_GT,
	_NFP_ELF_TOK_LTE,
	_NFP_ELF_TOK_GTE,
	_NFP_ELF_TOK_LSHF,
	_NFP_ELF_TOK_RSHF,
	_NFP_ELF_TOK_TRIARY,
	_NFP_ELF_TOK_IMPVAR,
	_NFP_ELF_TOK_MEMVAR
};

typedef enum _nfp_elf_token_type_e _nfp_elf_token_type;

struct _nfp_elf_expr_token_s {
	const char *str;
	_nfp_elf_token_type type;
	size_t pop;
	size_t push;
};

typedef struct _nfp_elf_expr_token_s _nfp_elf_expr_token;

static const _nfp_elf_expr_token _nfp_elf_tokens[] = {
	{"+", _NFP_ELF_TOK_ADD, 2, 1},     {"-", _NFP_ELF_TOK_SUB, 2, 1},
	{"*", _NFP_ELF_TOK_MUL, 2, 1},     {"/", _NFP_ELF_TOK_DIV, 2, 1},
	{"%", _NFP_ELF_TOK_MOD, 2, 1},     {"-u", _NFP_ELF_TOK_NEG, 1, 1},
	{"&", _NFP_ELF_TOK_BITAND, 2, 1},  {"|", _NFP_ELF_TOK_BITOR, 2, 1},
	{"^", _NFP_ELF_TOK_BITXOR, 2, 1},  {"~", _NFP_ELF_TOK_BITINV, 1, 1},
	{"&&", _NFP_ELF_TOK_AND, 2, 1},    {"||", _NFP_ELF_TOK_OR, 2, 1},
	{"!", _NFP_ELF_TOK_NOT, 1, 1},     {"==", _NFP_ELF_TOK_EQ, 2, 1},
	{"!=", _NFP_ELF_TOK_NEQ, 2, 1},    {"<", _NFP_ELF_TOK_LT, 2, 1},
	{">", _NFP_ELF_TOK_GT, 2, 1},      {"<=", _NFP_ELF_TOK_LTE, 2, 1},
	{">=", _NFP_ELF_TOK_GTE, 2, 1},    {"<<", _NFP_ELF_TOK_LSHF, 2, 1},
	{">>", _NFP_ELF_TOK_RSHF, 2, 1},   {"?", _NFP_ELF_TOK_TRIARY, 3, 1},
	{"&i", _NFP_ELF_TOK_IMPVAR, 0, 1}, {"&v", _NFP_ELF_TOK_MEMVAR, 0, 1}
};

static const _nfp_elf_expr_token _nfp_elf_expr_token_value = {NULL, _NFP_ELF_TOK_VALUE, 0, 1};

static const _nfp_elf_expr_token _nfp_elf_expr_token_string = {NULL, _NFP_ELF_TOK_STRING, 0, 0};

static uint64_t _nfp_elf_ecc_polynomials[NFP_ELF_USTORE_ECC_POLY_WORDCNT] = {
	UINT64_C(0x0ff800007fff), UINT64_C(0x11f801ff801f), UINT64_C(0x1e387e0781e1),
	UINT64_C(0x17cb8e388e22), UINT64_C(0x1af5b2c93244), UINT64_C(0x1f56d5525488),
	UINT64_C(0x0daf69a46910)};

static const struct nfp_elf_impexpr_field *_nfp_elf_find_ief(int chip_family, uint32_t wtype);

static int _nfp_elf_patch_word(uint64_t *eword,
			       const struct nfp_elf_impexpr_field *ief,
			       uint64_t wval,
			       uint64_t offset);

static const _nfp_elf_expr_token *_nfp_elf_get_token_type(const char *tokstr, uint64_t *val)
{
	size_t idx;
	char *endc = NULL;

	if (!tokstr)
		return NULL;
	/* Don't check on val here. */

	if (isdigit(tokstr[0])) {
		if (!val)
			return NULL;
		*val = nfp_strtou64(tokstr, &endc, 0);
		return &_nfp_elf_expr_token_value;
	}

	if (isalpha(tokstr[0]) || (tokstr[0] == '_'))
		return &_nfp_elf_expr_token_string;

	for (idx = 0; idx < (sizeof(_nfp_elf_tokens) / sizeof(_nfp_elf_expr_token)); idx++) {
		if (strcmp(_nfp_elf_tokens[idx].str, tokstr) == 0)
			return &_nfp_elf_tokens[idx];
	}

	return NULL;
}

static int _nfp_elf_eval_expr(struct nfp_elf *ectx,
			      uint64_t *result,
			      char *expr,
			      int allow_undefined_sym)
{
	struct nfp_elf_Elf64_Sym *sym = NULL;
	int64_t val = 0;
	int64_t stack[NFP_ELF_EXPR_STACK_SIZE];
	size_t sn = 0;
	char tmpc = 0;
	char *s = expr;
	char *tok = expr;
	const _nfp_elf_expr_token *toktype = NULL;
	_nfp_elf_token_type prevtoktype = _NFP_ELF_TOK_NONE;
	int err = EINVAL;

	if ((!ectx) || (!result) || (!expr)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	while ((*s) && isspace(*s))
		s++;
	if (!*s) {
		NS_LOG(NS_LOG_LVL_FATAL, "Empty expression string.");
		goto error_cleanup;
	}

	tok = s;
	while (tok) {
		while ((*s) && !isspace(*s))
			s++;
		if (*s) {
			tmpc = *s;
			*s = '\0';
		} else {
			s = NULL;
		}

		toktype = _nfp_elf_get_token_type(tok, (uint64_t *)&val);
		if (!toktype) {
			NS_LOG(NS_LOG_LVL_FATAL, "Invalid token type: \"%s\"", tok);
			goto error_cleanup;
		}
		if (toktype->pop > sn) {
			err = ERANGE;
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Bad expression, pop more than stack size after token \"%s\"", tok);
			goto error_cleanup;
		}

		sn -= toktype->pop;
		if ((toktype->push + sn) >= NFP_ELF_EXPR_STACK_SIZE) {
			err = ERANGE;
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Bad expression, push more than maximum stack size after token \"%s\"",
			       tok);
			goto error_cleanup;
		}

		switch (toktype->type) {
		case _NFP_ELF_TOK_ADD:
			stack[sn] = stack[sn] + stack[sn + 1];
			break;
		case _NFP_ELF_TOK_SUB:
			stack[sn] = stack[sn] - stack[sn + 1];
			break;
		case _NFP_ELF_TOK_MUL:
			stack[sn] = stack[sn] * stack[sn + 1];
			break;
		case _NFP_ELF_TOK_DIV:
			if (stack[sn + 1] == 0) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad expression, divide by zero.");
				goto error_cleanup;
			}
			stack[sn] = stack[sn] / stack[sn + 1];
			break;
		case _NFP_ELF_TOK_MOD:
			if (stack[sn + 1] == 0) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad expression, modulo by zero.");
				goto error_cleanup;
			}
			stack[sn] = stack[sn] % stack[sn + 1];
			break;
		case _NFP_ELF_TOK_NEG:
			stack[sn] = -stack[sn];
			break;
		case _NFP_ELF_TOK_BITAND:
			stack[sn] = stack[sn] & stack[sn + 1];
			break;
		case _NFP_ELF_TOK_BITOR:
			stack[sn] = stack[sn] | stack[sn + 1];
			break;
		case _NFP_ELF_TOK_BITXOR:
			stack[sn] = stack[sn] ^ stack[sn + 1];
			break;
		case _NFP_ELF_TOK_BITINV:
			stack[sn] = ~stack[sn];
			break;
		case _NFP_ELF_TOK_AND:
			stack[sn] = stack[sn] && stack[sn + 1];
			break;
		case _NFP_ELF_TOK_OR:
			stack[sn] = stack[sn] || stack[sn + 1];
			break;
		case _NFP_ELF_TOK_NOT:
			stack[sn] = !(stack[sn]);
			break;
		case _NFP_ELF_TOK_EQ:
			stack[sn] = (stack[sn] == stack[sn + 1]) ? 1 : 0;
			break;
		case _NFP_ELF_TOK_NEQ:
			stack[sn] = (stack[sn] != stack[sn + 1]) ? 1 : 0;
			break;
		case _NFP_ELF_TOK_LT:
			stack[sn] = (stack[sn] < stack[sn + 1]) ? 1 : 0;
			break;
		case _NFP_ELF_TOK_GT:
			stack[sn] = (stack[sn] > stack[sn + 1]) ? 1 : 0;
			break;
		case _NFP_ELF_TOK_LTE:
			stack[sn] = (stack[sn] <= stack[sn + 1]) ? 1 : 0;
			break;
		case _NFP_ELF_TOK_GTE:
			stack[sn] = (stack[sn] >= stack[sn + 1]) ? 1 : 0;
			break;
		case _NFP_ELF_TOK_LSHF:
			/* Always eval as 64-bit. */
			stack[sn] = stack[sn] << (stack[sn + 1] & 63);
			break;
		case _NFP_ELF_TOK_RSHF:
			stack[sn] = stack[sn] >> (stack[sn + 1] & 63);
			break;
		case _NFP_ELF_TOK_TRIARY:
			stack[sn] = stack[sn] ? stack[sn + 1] : stack[sn + 2];
			break;
		case _NFP_ELF_TOK_VALUE:
			stack[sn] = val;
			break;
		case _NFP_ELF_TOK_STRING:
			if (sym) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad expression, unresolved symbol name being replaced.");
				goto error_cleanup;
			}
			sym = nfp_elf_find_sym(ectx, tok);
			if (!sym) {
				NS_LOG(NS_LOG_LVL_FATAL, "Bad expression, symbol \"%s\" not found.",
				       tok);
				goto error_cleanup;
			}
			break;
		case _NFP_ELF_TOK_IMPVAR:
		case _NFP_ELF_TOK_MEMVAR:
			if ((prevtoktype != _NFP_ELF_TOK_STRING) || (!sym)) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Bad expression, cannot evaluate \"%s\", previous token not a symbol.",
				       tok);
				goto error_cleanup;
			}
			if ((toktype->type == _NFP_ELF_TOK_IMPVAR) &&
			    (!nfp_elf_import_var_is_defined(ectx, sym))) {
				err = ENOENT;
				if (!allow_undefined_sym)
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Bad expression, import variable \"%s\" not defined.",
					       nfp_elf_sym_name(ectx, sym));
				goto error_cleanup;
			}
			stack[sn] = sym->st_value;
			sym = NULL;
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL, "Invalid token type: \"%s\"", tok);
			goto error_cleanup;
		}

		prevtoktype = toktype->type;

		sn += toktype->push;

		if (!s) {
			tok = NULL;
		} else {
			*s = tmpc;

			while ((*s) && isspace(*s))
				s++;
			if (!*s)
				tok = NULL;
			else
				tok = s;
		}
	}

	if ((sym) || (sn != 1)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Bad expression, not fully evaluated.");
		goto error_cleanup;
	}

	*result = stack[0];
	return 0;

error_cleanup:
	if ((s) && (tmpc != 0))
		*s = tmpc;
	return -err;
}

struct nfp_elf *nfp_elf_from_file(const char *filename)
{
	FILE *f = NULL;
	struct nfp_elf *ectx = NULL;
	int err = EINVAL;
	nfp_off64_t bufsz;
	uint8_t *buf = NULL;

	if (!filename) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	f = fopen(filename, "rb");
	if (!f) {
		err = errno;
		NS_LOG(NS_LOG_LVL_FATAL, "Could not open file \"%s\" for reading.", filename);
		goto error_cleanup;
	}

	NSE_FSEEK(f, 0, SEEK_END);
	bufsz = nfp_ftell64(f);
	NSE_FSEEK(f, 0, SEEK_SET);
	if ((uint64_t)bufsz > SIZE_MAX) {
		err = EFBIG;
		NS_LOG(NS_LOG_LVL_FATAL, "File too large.");
		goto error_cleanup;
	}

	if (bufsz == 0) {
		err = EIO;
		NS_LOG(NS_LOG_LVL_FATAL, "File is empty.");
		goto error_cleanup;
	}

	NSE_MALLOC_OOM(buf, (size_t)bufsz);
	NSE_FREAD(buf, 1, (size_t)bufsz, f);

	ectx = nfp_elf_mutable_buf(buf, (size_t)bufsz);
	if (!ectx)
		goto error_cleanup;
	ectx->_buf = buf;
	ectx->_bufsz = bufsz;
	fclose(f);
	return ectx;

error_cleanup:
	if (f)
		fclose(f);
	if (buf)
		free(buf);

	return NFP_ERRPTR(err);
}

struct nfp_elf *nfp_elf_from_buf(const void *buf, size_t buf_len)
{
	struct nfp_elf *ectx = NULL;
	int err = EINVAL;
	uint8_t *mbuf = NULL;

	if (!buf || !buf_len) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	NSE_MALLOC_OOM(mbuf, buf_len);
	memcpy(mbuf, buf, buf_len);
	ectx = nfp_elf_mutable_buf(mbuf, buf_len);
	if (!ectx)
		goto error_cleanup;
	ectx->_buf = mbuf;
	ectx->_bufsz = buf_len;
	return ectx;

error_cleanup:
	if (mbuf)
		free(mbuf);

	return NFP_ERRPTR(err);
}

struct nfp_elf *nfp_elf_mutable_buf(void *buf, size_t buf_len)
{
	struct nfp_elf *ectx = NULL;
	uint8_t *buf8 = (uint8_t *)buf;
	uint8_t *pu8;
	int err = EINVAL;
	struct nfp_elf_Elf64_Shdr *sec = NULL;
	size_t idx;
	uint64_t _u64;

	if (!buf) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	NSE_CALLOC_OOM(ectx, 1, sizeof(struct nfp_elf));
	ectx->rev_min = -1;
	ectx->rev_max = -1;
	ectx->mip_sh_off = UINT64_MAX;

	if (buf_len < NFP_ELF_EI_NIDENT) {
		NS_LOG(NS_LOG_LVL_FATAL, "ELF data too short.");
		goto error_cleanup;
	}

	pu8 = buf8;
	if ((pu8[NFP_ELF_EI_MAG0] != NFP_ELF_ELFMAG0) ||
	    (pu8[NFP_ELF_EI_MAG1] != NFP_ELF_ELFMAG1) ||
	    (pu8[NFP_ELF_EI_MAG2] != NFP_ELF_ELFMAG2) ||
	    (pu8[NFP_ELF_EI_MAG3] != NFP_ELF_ELFMAG3) ||
	    (pu8[NFP_ELF_EI_VERSION] != NFP_ELF_EV_CURRENT) ||
	    (pu8[NFP_ELF_EI_DATA] != NFP_ELF_ELFDATA2LSB)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Not a valid ELF file.");
		goto error_cleanup;
	}

	switch (pu8[NFP_ELF_EI_CLASS]) {
	case NFP_ELF_ELFCLASS64:
		break;
	case NFP_ELF_ELFCLASS32:
		NS_LOG(NS_LOG_LVL_FATAL, "ELF32 NFFW files are no longer supported.");
		goto error_cleanup;
	default:
		NS_LOG(NS_LOG_LVL_FATAL, "Unknown ELF class.");
		goto error_cleanup;
	}

	/* ==== Read file headers ==== */
	pu8 = buf8;
	if (buf_len < sizeof(struct nfp_elf_Elf64_Ehdr)) {
		NS_LOG(NS_LOG_LVL_FATAL, "ELF data too short.");
		goto error_cleanup;
	}

	ectx->ehdr = (struct nfp_elf_Elf64_Ehdr *)pu8;
#if (NFP_HOST_ENDIAN == BIG_ENDIAN)
	{
		ectx->ehdr->e_type = NFP_LETOH16(ectx->ehdr->e_type);
		ectx->ehdr->e_machine = NFP_LETOH16(ectx->ehdr->e_machine);
		ectx->ehdr->e_version = NFP_LETOH32(ectx->ehdr->e_version);
		ectx->ehdr->e_entry = NFP_LETOH64(ectx->ehdr->e_entry);
		ectx->ehdr->e_phoff = NFP_LETOH64(ectx->ehdr->e_phoff);
		ectx->ehdr->e_shoff = NFP_LETOH64(ectx->ehdr->e_shoff);
		ectx->ehdr->e_flags = NFP_LETOH32(ectx->ehdr->e_flags);
		ectx->ehdr->e_ehsize = NFP_LETOH16(ectx->ehdr->e_ehsize);
		ectx->ehdr->e_phentsize = NFP_LETOH16(ectx->ehdr->e_phentsize);
		ectx->ehdr->e_phnum = NFP_LETOH16(ectx->ehdr->e_phnum);
		ectx->ehdr->e_shentsize = NFP_LETOH16(ectx->ehdr->e_shentsize);
		ectx->ehdr->e_shnum = NFP_LETOH16(ectx->ehdr->e_shnum);
		ectx->ehdr->e_shstrndx = NFP_LETOH16(ectx->ehdr->e_shstrndx);
	}
#endif
	switch (ectx->ehdr->e_machine) {
	case NFP_ELF_EM_NFP:
		ectx->family = (ectx->ehdr->e_flags >> NFP_ELF_EF_NFP_FAMILY_LSB) &
			NFP_ELF_EF_NFP_FAMILY_MASK;
		break;
	case NFP_ELF_EM_NFP3200:
		ectx->family = NFP_CHIP_FAMILY_NFP3200;
		break;
	case NFP_ELF_EM_NFP6000:
		ectx->family = NFP_CHIP_FAMILY_NFP6000;
		break;
	default:
		break;
	}

	if (!ectx->family) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid ELF machine type.");
		goto error_cleanup;
	}

	if (((ectx->ehdr->e_type != NFP_ELF_ET_EXEC) &&
	     (ectx->ehdr->e_type != NFP_ELF_ET_REL) &&
	     (ectx->ehdr->e_type != NFP_ELF_ET_NFP_PARTIAL_EXEC) &&
	     (ectx->ehdr->e_type != NFP_ELF_ET_NFP_PARTIAL_REL)) ||
	    (ectx->ehdr->e_version != NFP_ELF_EV_CURRENT) ||
	    (ectx->ehdr->e_ehsize != sizeof(struct nfp_elf_Elf64_Ehdr)) ||
	    (ectx->ehdr->e_shentsize != sizeof(struct nfp_elf_Elf64_Shdr))) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid ELF file header.");
		goto error_cleanup;
	}

	if (ectx->ehdr->e_shoff < ectx->ehdr->e_ehsize) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid ELF header content.");
		goto error_cleanup;
	}

	if (ectx->ehdr->e_shstrndx >= ectx->ehdr->e_shnum) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid ELF header content.");
		goto error_cleanup;
	}

	/* ==== Read section headers ==== */
	if (buf_len < (ectx->ehdr->e_shoff +
		       ((size_t)ectx->ehdr->e_shnum * sizeof(struct nfp_elf_Elf64_Shdr)))) {
		NS_LOG(NS_LOG_LVL_FATAL, "ELF data too short.");
		goto error_cleanup;
	}
	pu8 = buf8 + ectx->ehdr->e_shoff;
	NSE_CALLOC_OOM(ectx->shdrs, ectx->ehdr->e_shnum, sizeof(*ectx->shdrs));
	memcpy(ectx->shdrs, pu8, ectx->ehdr->e_shnum * sizeof(*ectx->shdrs));
	ectx->shdrs_cnt = ectx->ehdr->e_shnum;

	NSE_CALLOC_OOM(ectx->shdrs_data, (size_t)ectx->ehdr->e_shnum, sizeof(void *));
	NSE_CALLOC_OOM(ectx->shdrs_host_endian, (size_t)ectx->ehdr->e_shnum,
		       sizeof(ectx->shdrs_host_endian[0]));

#if (NFP_HOST_ENDIAN == BIG_ENDIAN)
	for (idx = 0, sec = ectx->shdrs; idx < ectx->shdrs_cnt; idx++, sec++) {
		sec->sh_name = NFP_LETOH32(sec->sh_name);
		sec->sh_type = NFP_LETOH32(sec->sh_type);
		sec->sh_flags = NFP_LETOH64(sec->sh_flags);
		sec->sh_addr = NFP_LETOH64(sec->sh_addr);
		sec->sh_offset = NFP_LETOH64(sec->sh_offset);
		sec->sh_size = NFP_LETOH64(sec->sh_size);
		sec->sh_link = NFP_LETOH32(sec->sh_link);
		sec->sh_info = NFP_LETOH32(sec->sh_info);
		sec->sh_addralign = NFP_LETOH64(sec->sh_addralign);
		sec->sh_entsize = NFP_LETOH64(sec->sh_entsize);
	}
#endif

	/* Now do a complete range sanity check */
	_u64 = sizeof(struct nfp_elf_Elf64_Ehdr);
	_u64 += sizeof(struct nfp_elf_Elf64_Shdr) * ectx->shdrs_cnt;
	_u64 += (NFP_ELF_FILE_ALIGN - _u64) & (NFP_ELF_FILE_ALIGN - 1);
	/* Don't count last section with alignment */
	for (idx = 0, sec = ectx->shdrs; (idx + 1) < ectx->shdrs_cnt; idx++, sec++) {
		if ((sec->sh_type == NFP_ELF_SHT_NOBITS) || (sec->sh_type == NFP_ELF_SHT_NULL))
			continue;
		_u64 += sec->sh_size;
		_u64 += (NFP_ELF_FILE_ALIGN - _u64) & (NFP_ELF_FILE_ALIGN - 1);
	}
	_u64 += sec->sh_size;
	if (_u64 > buf_len) {
		NS_LOG(NS_LOG_LVL_FATAL, "ELF data too short.");
		goto error_cleanup;
	}

	for (idx = 0, sec = ectx->shdrs; idx < ectx->shdrs_cnt; idx++, sec++) {
		if ((sec->sh_entsize != 0) && ((sec->sh_size % sec->sh_entsize) != 0)) {
			NS_LOG(NS_LOG_LVL_FATAL, "Invalid ELF section header, index %" PRIuSZ ".",
			       idx);
			goto error_cleanup;
		}

		switch (sec->sh_type) {
		case NFP_ELF_SHT_REL:
			if (sec->sh_entsize != sizeof(struct nfp_elf_Elf64_Rel)) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid ELF section header, index %" PRIuSZ ".",
				       idx);
				goto error_cleanup;
			}
			break;
		case NFP_ELF_SHT_SYMTAB:
			if (sec->sh_entsize != sizeof(struct nfp_elf_Elf64_Sym)) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid ELF section header, index %" PRIuSZ ".",
				       idx);
				goto error_cleanup;
			}
			break;
		case NFP_ELF_SHT_NFP_MECONFIG:
			if (nfp_elf_family(ectx) != NFP_CHIP_FAMILY_NFP3200) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Section only supported for NFP-32xx NFFW files.");
				goto error_cleanup;
			}

			if (sec->sh_entsize != sizeof(struct nfp_elf_Elf_meconfig)) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid ELF section header, index %" PRIuSZ ".",
				       idx);
				goto error_cleanup;
			}
			break;
		case NFP_ELF_SHT_NFP_INITREG:
			if (nfp_elf_arch(ectx) != NFP_CHIP_ARCH_TH) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Section not supported for target arch.");
				goto error_cleanup;
			}

			if (sec->sh_entsize != sizeof(struct nfp_elf_Elf_initRegEntry)) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid ELF section header, index %" PRIuSZ ".",
				       idx);
				goto error_cleanup;
			}
			break;
		default:
			break;
		}

		switch (sec->sh_type) {
		case NFP_ELF_SHT_REL: {
			if ((sec->sh_size == 0) || (sec->sh_size > UINT32_MAX)) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid ELF section header, index %" PRIuSZ ".", idx);
				goto error_cleanup;
			}

			pu8 = buf8 + sec->sh_offset;
			ectx->shdrs_data[idx] = pu8;

#if (NFP_HOST_ENDIAN == BIG_ENDIAN)
			{
				struct nfp_elf_Elf64_Rel *rel64 = NULL;
				size_t entcnt = 0;
				size_t relidx = 0;

				entcnt = (size_t)NFP_ELF_SEC_ENT_CNT(ectx, idx);
				rel64 = (struct nfp_elf_Elf64_Rel *)pu8;

				for (relidx = 0, rel64 = ectx->shdrs_data[idx];
				     relidx < entcnt; relidx++, rel64++) {
					rel64->r_info = NFP_LETOH64(rel64->r_info);
					rel64->r_offset = NFP_LETOH64(rel64->r_offset);
				}
			}
#endif

			ectx->shdrs_host_endian[idx] = 1;
		} break;
		case NFP_ELF_SHT_NOTE: {
			if ((sec->sh_size == 0) || (sec->sh_size > UINT32_MAX)) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid ELF section header, index %" PRIuSZ ".", idx);
				goto error_cleanup;
			}

			pu8 = buf8 + sec->sh_offset;
			ectx->shdrs_data[idx] = pu8;
			ectx->shdrs_host_endian[idx] = 0;

			/* Extract notes that we recognise */
			{
				struct nfp_elf_Elf64_Nhdr *nhdr = (struct nfp_elf_Elf64_Nhdr *)pu8;

				while ((uint8_t *)nhdr < (pu8 + sec->sh_size)) {
					uint32_t nnamesz = NFP_LETOH32(nhdr->n_namesz);
					uint32_t ndescsz = NFP_LETOH32(nhdr->n_descsz);
					uint32_t ntype = NFP_LETOH32(nhdr->n_type);
					const char *nname =
						(const char *)((uint8_t *)nhdr + sizeof(*nhdr));
					uint32_t *descword =
						(uint32_t *)((uint8_t *)nhdr + sizeof(*nhdr) +
							     ((nnamesz + UINT32_C(3)) &
							      ~UINT32_C(3)));

					if (strncmp(nname, NFP_ELT_NOTE_NAME_NFP,
						    NFP_ELT_NOTE_NAME_NFP_SZ) == 0) {
						switch (ntype) {
						case NFP_ELF_NT_NFP_REVS:
							if (ndescsz != 8) {
								NS_LOG(NS_LOG_LVL_FATAL,
								       "Invalid ELF NOTE descsz in section %" PRIuSZ ".",
								       idx);
								goto error_cleanup;
							}
							ectx->rev_min =
								(int)NFP_LETOH32(descword[0]);
							ectx->rev_max =
								(int)NFP_LETOH32(descword[1]);
							break;
						case NFP_ELF_NT_NFP_MIP_LOCATION:
							if (ndescsz != 12) {
								NS_LOG(NS_LOG_LVL_FATAL,
								       "Invalid ELF NOTE descsz in section %" PRIuSZ ".",
								       idx);
								goto error_cleanup;
							}
							ectx->mip_shndx = NFP_LETOH32(descword[0]);
							if (ectx->mip_shndx) {
								if (ectx->mip_shndx < ectx->shdrs_cnt) {
									ectx->mip_sh_off =
										NFP_LETOH32(descword[1]) |
										((uint64_t)NFP_LETOH32(
											 descword[2]) << 32);
								} else {
									NS_LOG(NS_LOG_LVL_FATAL,
									       "Invalid ELF NOTE mip_shndx in section %" PRIuSZ ".",
									       idx);
									goto error_cleanup;
								}
							} else {
								ectx->mip_sh_off = 0;
							}
							break;
						default:
							break;
						}
					} else if (strncmp(nname, NFP_ELT_NOTE_NAME_NFP_USER,
							   NFP_ELT_NOTE_NAME_NFP_USER_SZ) == 0) {
						switch (ntype) {
						case NFP_ELF_NT_NFP_USER:
							ectx->user_note_cnt++;
							break;
						default:
							break;
						}
					}

					nhdr = (struct nfp_elf_Elf64_Nhdr *)((uint8_t *)descword +
									     ((ndescsz +
									       UINT32_C(3)) &
									      ~UINT32_C(3)));
				}

				if (ectx->user_note_cnt) {
					struct nfp_elf_user_note *unote;

					NSE_CALLOC_OOM(ectx->user_notes, ectx->user_note_cnt,
						       sizeof(*ectx->user_notes));

					nhdr = (struct nfp_elf_Elf64_Nhdr *)pu8;
					unote = ectx->user_notes;
					while ((uint8_t *)nhdr < (pu8 + sec->sh_size)) {
						uint32_t nnamesz = NFP_LETOH32(nhdr->n_namesz);
						uint32_t ndescsz = NFP_LETOH32(nhdr->n_descsz);
						uint32_t ntype = NFP_LETOH32(nhdr->n_type);
						const char *nname = (const char *)((uint8_t *)nhdr +
										   sizeof(*nhdr));
						uint8_t *desc =
							(uint8_t *)nhdr + sizeof(*nhdr) +
							((nnamesz + UINT32_C(3)) & ~UINT32_C(3));

						if (strncmp(nname, NFP_ELT_NOTE_NAME_NFP_USER,
							    NFP_ELT_NOTE_NAME_NFP_USER_SZ) == 0) {
							size_t nsz;

							switch (ntype) {
							case NFP_ELF_NT_NFP_USER:
								unote->name = (const char *)desc;
								nsz = strlen(unote->name) + 1;
								while (nsz % 4)
									nsz++;
								if (nsz > ndescsz) {
									NS_LOG(NS_LOG_LVL_FATAL,
									       "Invalid ELF USER NOTE descsz in section %" PRIuSZ ".",
									       idx);
									goto error_cleanup;
								}
								unote->data_sz =
									ndescsz - (uint32_t)nsz;
								if (unote->data_sz)
									unote->data = desc + nsz;
								unote++;
								break;
							default:
								break;
							}
						}

						nhdr = (struct nfp_elf_Elf64_Nhdr
								*)(desc + ((ndescsz + UINT32_C(3)) &
									   ~UINT32_C(3)));
					}
				}
			}
		} break;
		case NFP_ELF_SHT_NFP_MECONFIG: {
			size_t entcnt = 0;

			if ((sec->sh_size == 0) || (sec->sh_size > UINT32_MAX)) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid ELF section header, index %" PRIuSZ ".", idx);
				goto error_cleanup;
			}

			pu8 = buf8 + sec->sh_offset;
			ectx->shdrs_data[idx] = pu8;
			entcnt = (size_t)NFP_ELF_SEC_ENT_CNT(ectx, idx);
			ectx->meconfs = (struct nfp_elf_Elf_meconfig *)pu8;
			ectx->meconfs_cnt = entcnt;

#if (NFP_HOST_ENDIAN == BIG_ENDIAN)
			{
				struct nfp_elf_Elf_meconfig *meconf = NULL;
				size_t meidx = 0;

				for (meidx = 0, meconf = ectx->shdrs_data[idx]; meidx < entcnt;
				     meidx++, meconf++) {
					meconf->ctx_enables = NFP_LETOH32(meconf->ctx_enables);
					meconf->entry = NFP_LETOH32(meconf->entry);
					meconf->misc_control = NFP_LETOH32(meconf->misc_control);
					meconf->reserved = NFP_LETOH32(meconf->reserved);
				}
			}
#endif
			ectx->shdrs_host_endian[idx] = 1;
		} break;
		case NFP_ELF_SHT_NFP_INITREG: {
			if ((sec->sh_size == 0) || (sec->sh_size > UINT32_MAX)) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid ELF section header, index %" PRIuSZ ".", idx);
				goto error_cleanup;
			}

			pu8 = buf8 + sec->sh_offset;
			ectx->shdrs_data[idx] = pu8;

#if (NFP_HOST_ENDIAN == BIG_ENDIAN)
			{
				size_t ireidx = 0;
				size_t entcnt = 0;
				struct nfp_elf_Elf_initRegEntry *ire = NULL;

				entcnt = (size_t)NFP_ELF_SEC_ENT_CNT(ectx, idx);
				ire = (struct nfp_elf_Elf_initRegEntry *)pu8;

				for (ireidx = 0, ire = ectx->shdrs_data[idx]; ireidx < entcnt;
				     ireidx++, ire++) {
					ire->w0 = NFP_LETOH32(ire->w0);
					ire->cpp_offset_lo = NFP_LETOH32(ire->cpp_offset_lo);
					ire->val = NFP_LETOH32(ire->val);
					ire->mask = NFP_LETOH32(ire->mask);
				}
			}
#endif
			ectx->shdrs_host_endian[idx] = 1;
		} break;
		case NFP_ELF_SHT_SYMTAB: {
			if ((sec->sh_size == 0) || (sec->sh_size > UINT32_MAX)) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid ELF section header, index %" PRIuSZ ".", idx);
				goto error_cleanup;
			}

			pu8 = buf8 + sec->sh_offset;
			ectx->shdrs_data[idx] = pu8;

#if (NFP_HOST_ENDIAN == BIG_ENDIAN)
			{
				size_t symidx = 0;
				size_t entcnt;
				struct nfp_elf_Elf64_Sym *sym64 = NULL;

				entcnt = (size_t)NFP_ELF_SEC_ENT_CNT(ectx, idx);
				sym64 = (struct nfp_elf_Elf64_Sym *)pu8;

				for (symidx = 0, sym64 = ectx->shdrs_data[idx];
				     symidx < entcnt; symidx++, sym64++) {
					sym64->st_name = NFP_LETOH32(sym64->st_name);
					sym64->st_value = NFP_LETOH64(sym64->st_value);
					sym64->st_size = NFP_LETOH64(sym64->st_size);
					sym64->st_shndx = NFP_LETOH16(sym64->st_shndx);
				}
			}
#endif

			ectx->shdrs_host_endian[ectx->shdr_idx_symtab] = 1;
		} break;
		case NFP_ELF_SHT_NOBITS:
		case NFP_ELF_SHT_NULL:
			break;
		default:
			if ((sec->sh_offset > 0) && (sec->sh_size > 0)) {
				/* Limit sections to 4GiB, because they won't need to be this large
				 * and this ensures we can handle the file on 32-bit hosts without
				 * unexpected problems.
				 */
				if (sec->sh_size > UINT32_MAX) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid ELF section header, index %" PRIuSZ ".",
					       idx);
					goto error_cleanup;
				}

				pu8 = buf8 + sec->sh_offset;
				ectx->shdrs_data[idx] = pu8;
				ectx->shdrs_host_endian[idx] = 0;
			}
			break;
		}
	}

	/* ==== Read shstrtab ==== */
	sec = &ectx->shdrs[ectx->ehdr->e_shstrndx];
	if (sec->sh_type != NFP_ELF_SHT_STRTAB) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid ELF shstrtab.");
		goto error_cleanup;
	}

	ectx->shstrtab = ectx->shdrs_data[ectx->ehdr->e_shstrndx];
	ectx->shstrtab_sz = (size_t)sec->sh_size;

	/* ==== Read first symtab if any, assuming it's the primary or only one ==== */
	for (idx = 0, sec = ectx->shdrs; idx < ectx->shdrs_cnt; idx++, sec++) {
		if (sec->sh_type == NFP_ELF_SHT_SYMTAB)
			break;
	}

	if ((idx < ectx->shdrs_cnt) && (sec->sh_type == NFP_ELF_SHT_SYMTAB)) {
		ectx->shdr_idx_symtab = idx;
		ectx->syms = ectx->shdrs_data[idx];
		ectx->syms_cnt = (size_t)NFP_ELF_SEC_ENT_CNT(ectx, idx);

		/* Load symtab's strtab */
		idx = sec->sh_link;

		if ((idx == NFP_ELF_SHN_UNDEF) || (idx >= ectx->shdrs_cnt)) {
			NS_LOG(NS_LOG_LVL_FATAL, "ELF symtab has no strtab.");
			goto error_cleanup;
		}

		sec = &ectx->shdrs[sec->sh_link];
		if (sec->sh_type != NFP_ELF_SHT_STRTAB) {
			NS_LOG(NS_LOG_LVL_FATAL, "ELF symtab has no strtab.");
			goto error_cleanup;
		}

		if ((sec->sh_size == 0) || (sec->sh_size > UINT32_MAX)) {
			NS_LOG(NS_LOG_LVL_FATAL, "ELF symtab has invalid strtab.");
			goto error_cleanup;
		}

		ectx->symstrtab = ectx->shdrs_data[idx];
		ectx->symstrtab_sz = (size_t)sec->sh_size;
	}

	/* Populate the fw_mip struct if we have a .note for it */
	if (ectx->mip_shndx) {
		const struct nfp_mip_v0 *mip0;
		const struct nfp_mip_v1 *mip1;
		const struct nfp_mip_v2 *mip2;

		sec = &ectx->shdrs[ectx->mip_shndx];
		pu8 = buf8 + sec->sh_offset + ectx->mip_sh_off;
		mip0 = (const struct nfp_mip_v0 *)pu8;
		mip1 = (const struct nfp_mip_v1 *)pu8;
		mip2 = (const struct nfp_mip_v2 *)pu8;

		if (NFP_LETOH32(mip0->signature) != NFP_MIP_SIGNATURE) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "MIP note pointer does not point to valid signature.");
			goto error_cleanup;
		}

		ectx->fw_mip.shndx = ectx->mip_shndx;
		ectx->fw_mip.sh_offset = ectx->mip_sh_off;
		ectx->fw_mip.mip_ver = NFP_LETOH32(mip0->mip_version);

		switch (ectx->fw_mip.mip_ver) {
		case 1:
			ectx->fw_mip.fw_version = NFP_LETOH32(mip1->version);
			ectx->fw_mip.fw_buildnum = NFP_LETOH32(mip1->buildnum);
			ectx->fw_mip.fw_buildtime = NFP_LETOH32(mip1->buildtime);
			strncpy(ectx->fw_mip.fw_name, mip1->name, 16);
			break;
		case 2:
			ectx->fw_mip.fw_version = NFP_LETOH32(mip2->fw_version);
			ectx->fw_mip.fw_buildnum = NFP_LETOH32(mip2->fw_buildnum);
			ectx->fw_mip.fw_buildtime = NFP_LETOH32(mip2->fw_buildtime);
			strncpy(ectx->fw_mip.fw_name, mip2->fw_name, 16);
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "MIP note pointer does not point to recognised version.");
			goto error_cleanup;
		}
	}

	ectx->_buf = buf;
	ectx->_bufsz = 0; /* Don't free */

	return ectx;

error_cleanup:
	if (ectx)
		nfp_elf_free(ectx);

	return NFP_ERRPTR(err);
}

size_t nfp_elf_to_buf(struct nfp_elf *ectx, void **buf_out)
{
	void *buf = NULL;
	size_t buf_sz;
	uint64_t _u64;
	int err = EINVAL;
	struct nfp_elf_Elf64_Shdr *sec = NULL;
	size_t idx;
	uint8_t *pu8;

	if ((!ectx) || (!buf_out)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	_u64 = sizeof(struct nfp_elf_Elf64_Ehdr);
	_u64 += sizeof(struct nfp_elf_Elf64_Shdr) * ectx->shdrs_cnt;
	_u64 += (NFP_ELF_FILE_ALIGN - _u64) & (NFP_ELF_FILE_ALIGN - 1);
	/* Don't count last section with alignment */
	for (idx = 0, sec = ectx->shdrs; (idx + 1) < ectx->shdrs_cnt; idx++, sec++) {
		if ((sec->sh_type == NFP_ELF_SHT_NOBITS) || (sec->sh_type == NFP_ELF_SHT_NULL))
			continue;
		_u64 += sec->sh_size;
		_u64 += (NFP_ELF_FILE_ALIGN - _u64) & (NFP_ELF_FILE_ALIGN - 1);
	}
	_u64 += sec->sh_size;

	if (_u64 > SIZE_MAX) {
		err = ERANGE;
		NS_LOG(NS_LOG_LVL_FATAL, "Total ELF buffer size too large.");
		goto error_cleanup;
	}
	buf_sz = (size_t)_u64;

	NSE_CALLOC_OOM(buf, buf_sz, 1);

	pu8 = (uint8_t *)buf;
	{
		struct nfp_elf_Elf64_Ehdr *ehdr64 = (struct nfp_elf_Elf64_Ehdr *)pu8;

		memcpy(ehdr64->e_ident, ectx->ehdr->e_ident, NFP_ELF_EI_NIDENT);
		ehdr64->e_type = NFP_HTOLE16(ectx->ehdr->e_type);
		ehdr64->e_machine = NFP_HTOLE16(ectx->ehdr->e_machine);
		ehdr64->e_version = NFP_HTOLE32(ectx->ehdr->e_version);
		ehdr64->e_entry = NFP_HTOLE64(ectx->ehdr->e_entry);
		ehdr64->e_phoff = NFP_HTOLE64(ectx->ehdr->e_phoff);
		ehdr64->e_shoff = NFP_HTOLE64(ectx->ehdr->e_shoff);
		ehdr64->e_flags = NFP_HTOLE32(ectx->ehdr->e_flags);
		ehdr64->e_ehsize = NFP_HTOLE16(ectx->ehdr->e_ehsize);
		ehdr64->e_phentsize = NFP_HTOLE16(ectx->ehdr->e_phentsize);
		ehdr64->e_phnum = NFP_HTOLE16(ectx->ehdr->e_phnum);
		ehdr64->e_shentsize = NFP_HTOLE16(ectx->ehdr->e_shentsize);
		ehdr64->e_shnum = NFP_HTOLE16(ectx->ehdr->e_shnum);
		ehdr64->e_shstrndx = NFP_HTOLE16(ectx->ehdr->e_shstrndx);
	}

	pu8 = (uint8_t *)buf + ectx->ehdr->e_shoff;
	{
		struct nfp_elf_Elf64_Shdr *shdr64 = (struct nfp_elf_Elf64_Shdr *)pu8;

		for (idx = 0; idx < ectx->shdrs_cnt; idx++, shdr64++) {
			shdr64->sh_name = NFP_HTOLE32(ectx->shdrs[idx].sh_name);
			shdr64->sh_type = NFP_HTOLE32(ectx->shdrs[idx].sh_type);
			shdr64->sh_flags = NFP_HTOLE64(ectx->shdrs[idx].sh_flags);
			shdr64->sh_addr = NFP_HTOLE64(ectx->shdrs[idx].sh_addr);
			shdr64->sh_offset = NFP_HTOLE64(ectx->shdrs[idx].sh_offset);
			shdr64->sh_size = NFP_HTOLE64(ectx->shdrs[idx].sh_size);
			shdr64->sh_link = NFP_HTOLE32(ectx->shdrs[idx].sh_link);
			shdr64->sh_info = NFP_HTOLE32(ectx->shdrs[idx].sh_info);
			shdr64->sh_addralign = NFP_HTOLE64(ectx->shdrs[idx].sh_addralign);
			shdr64->sh_entsize = NFP_HTOLE64(ectx->shdrs[idx].sh_entsize);
		}
	}

	for (idx = 0, sec = ectx->shdrs; idx < ectx->shdrs_cnt; idx++, sec++) {
		size_t entcnt = (size_t)NFP_ELF_SEC_ENT_CNT(ectx, idx);

		pu8 = (uint8_t *)buf + sec->sh_offset;

		switch (sec->sh_type) {
		case NFP_ELF_SHT_REL: {
			struct nfp_elf_Elf64_Rel *rel = ectx->shdrs_data[idx];

			{
				size_t entidx;
				struct nfp_elf_Elf64_Rel *rel64 = (struct nfp_elf_Elf64_Rel *)pu8;

				for (entidx = 0; entidx < entcnt; entidx++, rel64++, rel++) {
					rel64->r_offset = NFP_HTOLE64(rel->r_offset);
					rel64->r_info = NFP_HTOLE64(rel->r_info);
				}
			}
		} break;
		case NFP_ELF_SHT_NFP_MECONFIG: {
			memcpy(pu8, ectx->shdrs_data[idx],
			       (sizeof(struct nfp_elf_Elf_meconfig) * entcnt));

#if (NFP_HOST_ENDIAN == BIG_ENDIAN)
			{
				struct nfp_elf_Elf_meconfig *meconf =
					(struct nfp_elf_Elf_meconfig *)pu8;
				size_t meidx = 0;

				for (meidx = 0; meidx < entcnt; meidx++, meconf++) {
					meconf->ctx_enables = NFP_HTOLE32(meconf->ctx_enables);
					meconf->entry = NFP_HTOLE32(meconf->entry);
					meconf->misc_control = NFP_HTOLE32(meconf->misc_control);
					meconf->reserved = NFP_HTOLE32(meconf->reserved);
				}
			}
#endif
		} break;
		case NFP_ELF_SHT_NFP_INITREG: {
			memcpy(pu8, ectx->shdrs_data[idx],
			       (sizeof(struct nfp_elf_Elf_initRegEntry) * entcnt));

#if (NFP_HOST_ENDIAN == BIG_ENDIAN)
			{
				struct nfp_elf_Elf_initRegEntry *ire =
					(struct nfp_elf_Elf_initRegEntry *)pu8;
				size_t ireidx = 0;

				for (ireidx = 0; ireidx < entcnt; ireidx++, ire++) {
					ire->w0 = NFP_HTOLE32(ire->w0);
					ire->cpp_offset_lo = NFP_HTOLE32(ire->cpp_offset_lo);
					ire->val = NFP_HTOLE32(ire->val);
					ire->mask = NFP_HTOLE32(ire->mask);
				}
			}
#endif
		} break;
		case NFP_ELF_SHT_SYMTAB: {
			struct nfp_elf_Elf64_Sym *sym = ectx->shdrs_data[idx];

			{
				size_t entidx;
				struct nfp_elf_Elf64_Sym *sym64 = (struct nfp_elf_Elf64_Sym *)pu8;

				for (entidx = 0; entidx < entcnt; entidx++, sym64++, sym++) {
					sym64->st_info = sym->st_info;
					sym64->st_name = NFP_HTOLE32(sym->st_name);
					sym64->st_other = sym->st_other;
					sym64->st_shndx = NFP_HTOLE16(sym->st_shndx);
					sym64->st_size = NFP_HTOLE64(sym->st_size);
					sym64->st_value = NFP_HTOLE64(sym->st_value);
				}
			}
		} break;
		case NFP_ELF_SHT_NOBITS:
		case NFP_ELF_SHT_NULL:
			break;
		default:
			if ((sec->sh_offset > 0) && (sec->sh_size > 0))
				memcpy(pu8, ectx->shdrs_data[idx], (size_t)sec->sh_size);
			break;
		}
	}

	*buf_out = buf;
	return buf_sz;

error_cleanup:
	if (buf)
		free(buf);

	errno = err;
	return 0;
}

size_t nfp_elf_borrow_buf(struct nfp_elf *ectx, void **buf_out)
{
	int err = EINVAL;

	if ((!ectx) || (!buf_out)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	*buf_out = ectx->_buf;
	return ectx->_bufsz;

error_cleanup:
	errno = err;
	return 0;
}

int nfp_elf_to_file(struct nfp_elf *ectx, const char *filename)
{
	int err = EINVAL;
	void *buf = NULL;
	size_t buf_sz;
	FILE *f = NULL;

	buf_sz = nfp_elf_to_buf(ectx, &buf);
	if (buf_sz == 0)
		goto error_cleanup;

	f = fopen(filename, "wb");
	if (!f) {
		err = errno;
		NS_LOG(NS_LOG_LVL_FATAL, "Could not open file \"%s\" for writing.", filename);
		goto error_cleanup;
	}

	if (fwrite(buf, 1, buf_sz, f) != buf_sz) {
		err = errno;
		NS_LOG(NS_LOG_LVL_FATAL, "Could not write file \"%s\" (%" PRIuSZ " bytes).",
		       filename, buf_sz);
		goto error_cleanup;
	}

	free(buf);
	fclose(f);
	return 0;

error_cleanup:
	if (buf)
		free(buf);
	if (f)
		fclose(f);
	return NFP_ERRNO(err);
}

void nfp_elf_free(struct nfp_elf *ectx)
{
	if (!ectx)
		return;

	if (ectx->shdrs)
		free(ectx->shdrs);
	if (ectx->shdrs_data)
		free(ectx->shdrs_data);
	if (ectx->shdrs_host_endian)
		free(ectx->shdrs_host_endian);

	if (ectx->_buf && ectx->_bufsz)
		free(ectx->_buf);

	free(ectx);
}

int nfp_elf_strip_debug(struct nfp_elf *ectx)
{
	int err = EINVAL;
	struct nfp_elf_Elf64_Shdr *sec = NULL;
	struct nfp_elf_Elf64_Sym *sym = NULL;
	size_t secidx, dbgidx, symidx;
	uint64_t dbgoffset = 0;
	uint64_t dbgsize = 0;
	uint64_t glbdiff = 0;

	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	for (dbgidx = 0, sec = ectx->shdrs; dbgidx < ectx->shdrs_cnt; dbgidx++, sec++) {
		if (sec->sh_type == NFP_ELF_SHT_UOF_DEBUG)
			break;
	}

	if (dbgidx == ectx->shdrs_cnt)
		return 0; /* Had no debug data */

	if (ectx->dbgdata) {
		free(ectx->dbgdata);
		ectx->dbgdata = NULL;
	}

	if (ectx->shdrs_data[dbgidx]) {
		dbgoffset = sec->sh_offset;
		dbgsize = sec->sh_size;
		dbgsize += (NFP_ELF_FILE_ALIGN - dbgsize) & (NFP_ELF_FILE_ALIGN - 1);
		free(ectx->shdrs_data[dbgidx]);
		ectx->shdrs_data[dbgidx] = NULL;
	}

	/* Now remove the section header. If it's the last section, we only remove it and reduce
	 * the section count. If not, we need to re-pickle some secion index pointers.
	 */
	if (dbgidx != (ectx->shdrs_cnt - 1)) {
		memmove(&ectx->shdrs[dbgidx], &ectx->shdrs[dbgidx + 1],
			sizeof(ectx->shdrs[dbgidx]) * (ectx->shdrs_cnt - 1 - dbgidx));
		memmove(&ectx->shdrs_data[dbgidx], &ectx->shdrs_data[dbgidx + 1],
			sizeof(ectx->shdrs_data[dbgidx]) * (ectx->shdrs_cnt - 1 - dbgidx));
		ectx->shdrs_cnt--;
		ectx->ehdr->e_shnum--;

		if (ectx->mip_shndx > dbgidx)
			ectx->mip_shndx--;
		if (ectx->shdr_idx_symtab > dbgidx)
			ectx->shdr_idx_symtab--;

		glbdiff = ectx->ehdr->e_shentsize;
		glbdiff += (NFP_ELF_FILE_ALIGN - glbdiff) & (NFP_ELF_FILE_ALIGN - 1);
		dbgoffset -= glbdiff;

		if ((size_t)ectx->ehdr->e_shstrndx > dbgidx)
			ectx->ehdr->e_shstrndx--;
		if (nfp_elf_family(ectx) == NFP_CHIP_FAMILY_NFP3200) {
			uint32_t meconfidx =
				ectx->ehdr->e_flags >> NFP_ELF_EF_NFP3200_MECONFIG_IDX_LSB;
			meconfidx &= NFP_ELF_EF_NFP3200_MECONFIG_IDX_MASK;
			if (meconfidx > dbgidx) {
				meconfidx--;
				ectx->ehdr->e_flags &= ~(NFP_ELF_EF_NFP3200_MECONFIG_IDX_MASK
							<< NFP_ELF_EF_NFP3200_MECONFIG_IDX_LSB);
				ectx->ehdr->e_flags |=
					(meconfidx << NFP_ELF_EF_NFP3200_MECONFIG_IDX_LSB);
			}
		}

		for (secidx = 0, sec = ectx->shdrs; secidx < ectx->shdrs_cnt; secidx++, sec++) {
			if (sec->sh_offset >= glbdiff)
				sec->sh_offset -= glbdiff;
			if (sec->sh_offset >= (dbgoffset + dbgsize))
				sec->sh_offset -= dbgsize;

			switch (sec->sh_type) {
			case NFP_ELF_SHT_SYMTAB:
				if ((size_t)sec->sh_link > dbgidx)
					sec->sh_link--;
				for (symidx = 0, sym = ectx->syms; symidx < ectx->syms_cnt;
				     symidx++, sym++) {
					if (((size_t)sym->st_shndx > dbgidx) &&
					    (sym->st_shndx < NFP_ELF_SHN_LORESERVE))
						sym->st_shndx--;
				}
				break;
			case NFP_ELF_SHT_REL:
				if ((size_t)sec->sh_info > dbgidx)
					sec->sh_link--;
				if ((size_t)sec->sh_info > dbgidx)
					sec->sh_info--;
				break;
			case NFP_ELF_SHT_NOTE:
				/* Go through all the notes to find the MIP locator note and update
				 * it.
				 */
				{
					struct nfp_elf_Elf64_Nhdr *nhdr =
						(struct nfp_elf_Elf64_Nhdr *)
							ectx->shdrs_data[secidx];
					int done = 0;

					while ((uint8_t *)nhdr <
					       ((uint8_t *)ectx->shdrs_data[secidx] +
						sec->sh_size)) {
						uint32_t nnamesz = NFP_LETOH32(nhdr->n_namesz);
						uint32_t ndescsz = NFP_LETOH32(nhdr->n_descsz);
						uint32_t ntype = NFP_LETOH32(nhdr->n_type);
						const char *nname = (const char *)((uint8_t *)nhdr +
										   sizeof(*nhdr));
						uint32_t *descword =
							(uint32_t *)((uint8_t *)nhdr +
								     sizeof(*nhdr) +
								     ((nnamesz + UINT32_C(3)) &
								      ~UINT32_C(3)));

						if ((nnamesz == NFP_ELT_NOTE_NAME_NFP_SZ) &&
						    (strncmp(nname, NFP_ELT_NOTE_NAME_NFP,
							     nnamesz) == 0)) {
							switch (ntype) {
							case NFP_ELF_NT_NFP_MIP_LOCATION:
								descword[0] = NFP_HTOLE32(
									(uint32_t)ectx->mip_shndx);
								done = 1;
								break;
							default:
								break;
							}
						}

						if (done)
							break;

						nhdr = (struct nfp_elf_Elf64_Nhdr
								*)((uint8_t *)descword +
								   ((ndescsz + UINT32_C(3)) &
								    ~UINT32_C(3)));
					}
				}
				break;
			default:
				break;
			}
		}
	} else {
		ectx->shdrs_cnt--;
	}

	return 0;

error_cleanup:
	return NFP_ERRNO(err);
}

int nfp_elf_is_elf64(struct nfp_elf *ectx)
{
	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return 0;
	}

	return 1;
}

int nfp_elf_is_exec(struct nfp_elf *ectx)
{
	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return 0;
	}

	return (ectx->ehdr->e_type == NFP_ELF_ET_EXEC) ||
	       (ectx->ehdr->e_type == NFP_ELF_ET_NFP_PARTIAL_EXEC);
}

int nfp_elf_rev_min(struct nfp_elf *ectx)
{
	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return -1;
	}

	return ectx->rev_min;
}

int nfp_elf_rev_max(struct nfp_elf *ectx)
{
	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return -1;
	}

	return ectx->rev_max;
}

int nfp_elf_mip_note(struct nfp_elf *ectx, size_t *mip_shndx, uint64_t *mip_sh_offset)
{
	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return -1;
	}

	if (mip_shndx)
		*mip_shndx = ectx->mip_shndx;
	if (mip_sh_offset)
		*mip_sh_offset = ectx->mip_sh_off;
	return 0;
}

const struct nfp_elf_fw_mip *nfp_elf_mip(struct nfp_elf *ectx)
{
	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return NULL;
	}

	if (!ectx->fw_mip.shndx)
		return NULL;
	return &ectx->fw_mip;
}

int nfp_elf_arch(struct nfp_elf *ectx)
{
	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return -1;
	}

	return NFP_FAMILY_ARCH(ectx->family);
}

int nfp_elf_family(struct nfp_elf *ectx)
{
	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return -1;
	}

	return ectx->family;
}

struct nfp_elf_Elf64_Ehdr *nfp_elf_filehdr(struct nfp_elf *ectx)
{
	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return NFP_ERRPTR(EINVAL);
	}

	return ectx->ehdr;
}

size_t nfp_elf_sections(struct nfp_elf *ectx, struct nfp_elf_Elf64_Shdr **shdrs)
{
	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		errno = EINVAL;
		return 0;
	}

	if (shdrs)
		*shdrs = ectx->shdrs;
	return ectx->shdrs_cnt;
}

size_t nfp_elf_symbols(struct nfp_elf *ectx, struct nfp_elf_Elf64_Sym **syms)
{
	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		errno = EINVAL;
		return 0;
	}

	if (syms)
		*syms = ectx->syms;
	return ectx->syms_cnt;
}

int nfp_elf_sec_is_text(struct nfp_elf *ectx, size_t secidx)
{
	struct nfp_elf_Elf64_Shdr *sec;

	if (!ectx)
		return 0;
	if (secidx >= ectx->shdrs_cnt)
		return 0;

	sec = &ectx->shdrs[secidx];
	if ((sec->sh_type != NFP_ELF_SHT_PROGBITS) ||
	    ((sec->sh_flags & NFP_ELF_SHF_ALLOC) != NFP_ELF_SHF_ALLOC) ||
	    ((sec->sh_flags & NFP_ELF_SHF_EXECINSTR) != NFP_ELF_SHF_EXECINSTR) ||
	    (NFP_ELF_GET_SHI_MEMTYPE(nfp_elf_family(ectx), sec->sh_info) !=
	     NFP_ELF_SHI_NFP_MEMTYPE_USTORE))
		return 0;

	if ((sec->sh_flags & NFP_ELF_SHF_NFP_INIT_SECTION) == NFP_ELF_SHF_NFP_INIT_SECTION)
		return 0;

	return 1;
}

int nfp_elf_sec_is_init_text(struct nfp_elf *ectx, size_t secidx)
{
	struct nfp_elf_Elf64_Shdr *sec;

	if (!ectx)
		return 0;
	if (secidx >= ectx->shdrs_cnt)
		return 0;

	sec = &ectx->shdrs[secidx];
	if ((sec->sh_type != NFP_ELF_SHT_PROGBITS) ||
	    ((sec->sh_flags & NFP_ELF_SHF_ALLOC) != NFP_ELF_SHF_ALLOC) ||
	    ((sec->sh_flags & NFP_ELF_SHF_EXECINSTR) != NFP_ELF_SHF_EXECINSTR) ||
	    (NFP_ELF_GET_SHI_MEMTYPE(nfp_elf_family(ectx), sec->sh_info) !=
	     NFP_ELF_SHI_NFP_MEMTYPE_USTORE))
		return 0;

	if ((sec->sh_flags & NFP_ELF_SHF_NFP_INIT_SECTION) == 0)
		return 0;

	return 1;
}

int nfp_elf_sec_has_reloc(struct nfp_elf *ectx, size_t secidx)
{
	size_t relsecidx;
	struct nfp_elf_Elf64_Shdr *relsec;
	int err = EINVAL;

	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	if (secidx >= ectx->shdrs_cnt) {
		err = ENOENT;
		NS_LOG(NS_LOG_LVL_FATAL,
		       "Section index %" PRIuSZ " is out of range (%" PRIuSZ " sections).", secidx,
		       ectx->shdrs_cnt);
		goto error_cleanup;
	}

	for (relsecidx = 0, relsec = ectx->shdrs; relsecidx < ectx->shdrs_cnt;
	     relsecidx++, relsec++) {
		if (relsecidx == secidx)
			continue;
		if (relsec->sh_type != NFP_ELF_SHT_REL)
			continue;

		if (relsec->sh_link != ectx->shdr_idx_symtab) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "No symtab for relocation section %" PRIuSZ " (%s).", relsecidx,
			       nfp_elf_sec_name(ectx, relsecidx));
			goto error_cleanup;
		}

		if (relsec->sh_info == secidx)
			return 1;
	}

	return 0;
error_cleanup:
	return NFP_ERRNO(err);
}

int nfp_elf_resolve(struct nfp_elf *ectx, int allow_undefined_sym)
{
	int err = EINVAL;
	struct nfp_elf_Elf64_Sym *sym = NULL;
	size_t idx;
	int still_has_unresolved_expr = 0;
	struct nfp_elf_Elf64_Shdr *relsec = NULL;
	struct nfp_elf_Elf64_Shdr *modsec = NULL;
	struct nfp_elf_Elf64_Rel *rel = NULL;
	size_t relcnt = 0;
	size_t relsecidx = 0;
	size_t modsecidx = 0;
	size_t relidx = 0;
	uint64_t val64 = 0;
	uint64_t *val64p = 0;
	uint32_t *val32p = 0;
	uint16_t *val16p = 0;
	uint8_t *val8p = 0;

	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	for (idx = 0, sym = ectx->syms; idx < ectx->syms_cnt; idx++, sym++) {
		if (NFP_ELF_ELF_ST_TYPE(sym->st_info) != NFP_ELF_STT_NFP_EXPR)
			continue;

		if (sym->st_shndx != NFP_ELF_SHN_UNDEF)
			continue;

		err = -_nfp_elf_eval_expr(ectx, &sym->st_value, &ectx->symstrtab[sym->st_name],
					  allow_undefined_sym);

		if (err != 0) {
			if (allow_undefined_sym && (err == ENOENT)) {
				still_has_unresolved_expr = 1;
			} else {
				NS_LOG(NS_LOG_LVL_FATAL,
				       " Evaluation of expression symbol %" PRIuSZ " failed.", idx);
				goto error_cleanup;
			}
		}
	}

	if (still_has_unresolved_expr) {
		if (ectx->ehdr->e_type == NFP_ELF_ET_EXEC)
			ectx->ehdr->e_type = NFP_ELF_ET_REL;
		else if (ectx->ehdr->e_type == NFP_ELF_ET_NFP_PARTIAL_EXEC)
			ectx->ehdr->e_type = NFP_ELF_ET_NFP_PARTIAL_REL;
		return 1;
	}

	/* Check for possible relocations on sh_info and other pre-loaded sections. */
	for (relsecidx = 0, relsec = ectx->shdrs; relsecidx < ectx->shdrs_cnt;
	     relsecidx++, relsec++) {
		uint8_t *moddata = NULL;
		uint8_t endian_host = 0;

		if (relsec->sh_type != NFP_ELF_SHT_REL)
			continue;

		if (relsec->sh_link != ectx->shdr_idx_symtab) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "No symtab for relocation section %" PRIuSZ " (%s).", relsecidx,
			       nfp_elf_sec_name(ectx, relsecidx));
			goto error_cleanup;
		}

		if (relsec->sh_info >= ectx->shdrs_cnt) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Invalid target section for relocation section %" PRIuSZ " (%s).",
			       relsecidx, nfp_elf_sec_name(ectx, relsecidx));
			goto error_cleanup;
		}

		modsecidx = relsec->sh_info;
		modsec = &ectx->shdrs[modsecidx];
		moddata = (uint8_t *)ectx->shdrs_data[modsecidx];
		endian_host = ectx->shdrs_host_endian[modsecidx];

		if ((relsec->sh_size / relsec->sh_entsize) > UINT32_MAX) {
			NS_LOG(NS_LOG_LVL_FATAL, "Too many entries in section %" PRIuSZ " (%s).",
			       relsecidx, nfp_elf_sec_name(ectx, relsecidx));
			goto error_cleanup;
		}

		relcnt = (size_t)(relsec->sh_size / relsec->sh_entsize);
		rel = ectx->shdrs_data[relsecidx];
		if (!rel) {
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Internal error, no data for section %" PRIuSZ " (%s).", relsecidx,
			       nfp_elf_sec_name(ectx, relsecidx));
			goto error_cleanup;
		}

		for (relidx = 0; relidx < relcnt; relidx++, rel++) {
			const struct nfp_elf_impexpr_field *ief = NULL;

			ief = _nfp_elf_find_ief(nfp_elf_family(ectx),
						NFP_ELF_ELF64_R_TYPE(rel->r_info));
			if (!ief) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Invalid relocation type for entry %" PRIuSZ
				       " in relocation section %" PRIuSZ " (%s).",
				       relidx, relsecidx, nfp_elf_sec_name(ectx, relsecidx));
				goto error_cleanup;
			}

			if (NFP_ELF_ELF64_R_SYM(rel->r_info) >= ectx->syms_cnt) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Section %" PRIuSZ
				       " (%s) has invalid relocation, index %" PRIuSZ ".",
				       relsecidx, nfp_elf_sec_name(ectx, relsecidx), relidx);
				goto error_cleanup;
			}
			sym = &ectx->syms[NFP_ELF_ELF64_R_SYM(rel->r_info)];

			if (ief->count > 0) {
				if (!moddata)
					continue;

				if ((rel->r_offset + (ief->word_size / 8)) > modsec->sh_size) {
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Invalid relocation offset in relocation section %" PRIuSZ " (%s).",
					       relsecidx, nfp_elf_sec_name(ectx, relsecidx));
					goto error_cleanup;
				}
				switch (ief->word_size) {
				case 8:
					val8p = (uint8_t *)(moddata + (ptrdiff_t)rel->r_offset);
					val64 = *val8p;
					break;
				case 16:
					val16p = (uint16_t *)(moddata + (ptrdiff_t)rel->r_offset);
					if (endian_host) {
						val64 = *val16p;
					} else {
						if (ief->big_endian)
							val64 = NFP_BETOH16(*val16p);
						else
							val64 = NFP_LETOH16(*val16p);
					}
					break;
				case 32:
					val32p = (uint32_t *)(moddata + (ptrdiff_t)rel->r_offset);
					if (endian_host) {
						val64 = *val32p;
					} else {
						if (ief->big_endian)
							val64 = NFP_BETOH32(*val32p);
						else
							val64 = NFP_LETOH32(*val32p);
					}
					break;
				case 64:
					val64p = (uint64_t *)(moddata + (ptrdiff_t)rel->r_offset);
					if (endian_host) {
						val64 = *val64p;
					} else {
						if (ief->big_endian)
							val64 = NFP_BETOH64(*val64p);
						else
							val64 = NFP_LETOH64(*val64p);
					}
					break;
				default:
					NS_LOG(NS_LOG_LVL_FATAL,
					       "Internal error, bad relocation type word size");
					goto error_cleanup;
				}
			} else {
				switch (ief->r_type) {
				case NFP_ELF_R_NFP6000_SH_INFO:
					val64 = modsec->sh_info;
					break;
				default:
					NS_LOG(NS_LOG_LVL_FATAL, "Invalid relocation type.");
					goto error_cleanup;
				}
			}

			err = _nfp_elf_patch_word(&val64, ief, sym->st_value, rel->r_offset);
			if (err == -ERANGE) {
				NS_LOG(NS_LOG_LVL_FATAL,
				       "Relocation expression overflowed, section %" PRIuSZ
				       " (%s), entry %" PRIuSZ ", symbol \"%s\"",
				       relsecidx, nfp_elf_sec_name(ectx, relsecidx), relidx,
				       nfp_elf_sym_name(ectx, sym));
				goto error_cleanup;
			} else if (err != 0) {
				NS_LOG(NS_LOG_LVL_FATAL, "Error patching at address 0x%" PRIX64
							 " of section %" PRIu32 " (%s): ",
				       rel->r_offset, modsecidx, nfp_elf_sec_name(ectx, modsecidx));
				goto error_cleanup;
			}

			if (ief->count > 0) {
				if (nfp_elf_sec_is_text(ectx, modsecidx) ||
				    nfp_elf_sec_is_init_text(ectx, modsecidx))
					nfp_elf_ustore_ecc_calculate_and_set_bits(ectx, &val64);

				switch (ief->word_size) {
				case 8:
					*val8p = (uint8_t)(val64 & 0xFF);
					break;
				case 16:
					if (endian_host) {
						*val16p = (uint16_t)val64;
					} else {
						if (ief->big_endian)
							*val16p = (uint16_t)NFP_HTOBE16(val64);
						else
							*val16p = (uint16_t)NFP_HTOLE16(val64);
					}
					break;
				case 32:
					if (endian_host) {
						*val32p = (uint32_t)val64;
					} else {
						if (ief->big_endian)
							*val32p = (uint32_t)NFP_HTOBE32(val64);
						else
							*val32p = (uint32_t)NFP_HTOLE32(val64);
					}
					break;
				case 64:
					if (endian_host) {
						*val64p = val64;
					} else {
						if (ief->big_endian)
							*val64p = NFP_HTOBE64(val64);
						else
							*val64p = NFP_HTOLE64(val64);
					}
					break;
				default:
					goto error_cleanup;
				}
			} else {
				switch (ief->r_type) {
				case NFP_ELF_R_NFP6000_SH_INFO:
					modsec->sh_info = (uint32_t)val64;
					break;
				default:
					goto error_cleanup;
				}
			}
		}
	}

	if (ectx->ehdr->e_type == NFP_ELF_ET_REL)
		ectx->ehdr->e_type = NFP_ELF_ET_EXEC;
	else if (ectx->ehdr->e_type == NFP_ELF_ET_NFP_PARTIAL_REL)
		ectx->ehdr->e_type = NFP_ELF_ET_NFP_PARTIAL_EXEC;
	return 0;

error_cleanup:
	return NFP_ERRNO(err);
}

struct nfp_elf_Elf64_Sym *nfp_elf_find_sym(struct nfp_elf *ectx, const char *name)
{
	struct nfp_elf_Elf64_Sym *sym = NULL;
	size_t idx;

	if (!ectx)
		return NULL;
	if (!name)
		return NULL;

	for (idx = 0, sym = ectx->syms; idx < ectx->syms_cnt; idx++, sym++) {
		if (strcmp(&ectx->symstrtab[sym->st_name], name) == 0)
			return sym;
	}

	return NULL;
}

int nfp_elf_sym_is_import_var(struct nfp_elf *ectx, const struct nfp_elf_Elf64_Sym *sym)
{
	NFP_ELF_UNUSED(ectx);

	if (!sym)
		return 0;

	return ((sym->st_name > 0) && (NFP_ELF_ELF_ST_TYPE(sym->st_info) == NFP_ELF_STT_NOTYPE) &&
		((sym->st_shndx == NFP_ELF_SHN_UNDEF) || (sym->st_shndx == NFP_ELF_SHN_ABS)));
}

int nfp_elf_import_var_is_defined(struct nfp_elf *ectx, const struct nfp_elf_Elf64_Sym *sym)
{
	NFP_ELF_UNUSED(ectx);

	if (!sym)
		return 0;

	return ((sym->st_name > 0) && (NFP_ELF_ELF_ST_TYPE(sym->st_info) == NFP_ELF_STT_NOTYPE) &&
		(sym->st_shndx == NFP_ELF_SHN_ABS));
}

int nfp_elf_import_var_undefine(struct nfp_elf *ectx, struct nfp_elf_Elf64_Sym *sym)
{
	NFP_ELF_UNUSED(ectx);

	if (!sym)
		return NFP_ERRNO(EINVAL);

	sym->st_shndx = NFP_ELF_SHN_UNDEF;

	if (ectx->ehdr->e_type == NFP_ELF_ET_EXEC)
		ectx->ehdr->e_type = NFP_ELF_ET_REL;
	else if (ectx->ehdr->e_type == NFP_ELF_ET_NFP_PARTIAL_EXEC)
		ectx->ehdr->e_type = NFP_ELF_ET_NFP_PARTIAL_REL;
	return 0;
}

int nfp_elf_import_var_set(struct nfp_elf *ectx, struct nfp_elf_Elf64_Sym *sym, uint64_t val)
{
	int err = EINVAL;

	if ((!ectx) || (!sym)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	if (!nfp_elf_sym_is_import_var(ectx, sym)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Symbol \"%s\" not a import variable.",
		       nfp_elf_sym_name(ectx, sym));
		goto error_cleanup;
	}

	sym->st_value = val;
	sym->st_shndx = NFP_ELF_SHN_ABS;

	if (ectx->ehdr->e_type == NFP_ELF_ET_EXEC)
		ectx->ehdr->e_type = NFP_ELF_ET_REL;
	else if (ectx->ehdr->e_type == NFP_ELF_ET_NFP_PARTIAL_EXEC)
		ectx->ehdr->e_type = NFP_ELF_ET_NFP_PARTIAL_REL;

	return 0;
error_cleanup:
	return NFP_ERRNO(err);
}

const struct nfp_elf_impexpr_field *_nfp_elf_find_ief(int chip_family, uint32_t wtype)
{
	size_t idx;

	switch (chip_family) {
	case NFP_CHIP_FAMILY_NFP3200:
		for (idx = 0; idx < _nfp3200_sdk_elf_impexpr_field_map_cnt; idx++)
			if (_nfp3200_sdk_elf_impexpr_field_map[idx].r_type == wtype)
				return &_nfp3200_sdk_elf_impexpr_field_map[idx];
		break;
	case NFP_CHIP_FAMILY_NFP6000:
	case NFP_CHIP_FAMILY_NFP3800:
		for (idx = 0; idx < _nfp6000_sdk_elf_impexpr_field_map_cnt; idx++)
			if (_nfp6000_sdk_elf_impexpr_field_map[idx].r_type == wtype)
				return &_nfp6000_sdk_elf_impexpr_field_map[idx];
		break;
	default:
		break;
	}

	return NULL;
}

int _nfp_elf_patch_word(uint64_t *eword,
			const struct nfp_elf_impexpr_field *ief,
			uint64_t wval,
			uint64_t offset)
{
	size_t idx = 0;
	uint32_t bh, bl, bs;
	uint64_t val_mask;
	uint64_t val;
	uint64_t used_mask = 0;
	int err = EINVAL;

	if (!eword) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	if (!ief) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid relocation type.");
		goto error_cleanup;
	}

	for (idx = 0; idx < ief->count; idx++) {
		bh = ief->fields[(idx * 3) + 0];
		bl = ief->fields[(idx * 3) + 1];
		bs = ief->fields[(idx * 3) + 2];

		if ((bh - bl + 1) > (sizeof(uint64_t) * 8)) {
			NS_LOG(NS_LOG_LVL_FATAL, "Internal error (import index %" PRIuSZ ").", idx);
			goto error_cleanup;
		}
		if ((bh - bl + bs) >= (sizeof(uint64_t) * 8)) {
			NS_LOG(NS_LOG_LVL_FATAL, "Internal error (import index %" PRIuSZ ").", idx);
			goto error_cleanup;
		}

		if (((bh) >= (sizeof(uint64_t) * 8)) || ((bl) >= (sizeof(uint64_t) * 8)) ||
		    ((bs) >= (sizeof(uint64_t) * 8))) {
			NS_LOG(NS_LOG_LVL_FATAL, "Internal error (import index %" PRIuSZ ").", idx);
			goto error_cleanup;
		}

		if ((bh - bl + 1) == (sizeof(uint64_t) * 8))
			val_mask = UINT64_C(0xFFFFffffFFFFffff);
		else
			val_mask = ((UINT64_C(1) << (bh - bl + 1)) - 1);
		val = (wval >> bs) & val_mask;
		used_mask |= (val_mask) << bs;

		switch (ief->op) {
		case '=':
			*eword = NFP_ELF_BITS_SET(*eword, val, val_mask, bl);
			break;
		case '&':
			*eword &= NFP_ELF_BITS_SET(*eword, val, val_mask, bl);
			break;
		case '|':
			*eword |= NFP_ELF_BITS_SET(*eword, val, val_mask, bl);
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Internal error, bad operand (import index %" PRIuSZ ").",
			       idx);
			goto error_cleanup;
		}
	}

	if (ief->count == 0) {
		bh = ((offset >> 8) & 0x1F);
		bl = (offset & 0x1F);
		bs = 0;

		if ((bh - bl + 1) > (sizeof(uint64_t) * 8)) {
			NS_LOG(NS_LOG_LVL_FATAL, "Internal error (import index %" PRIuSZ ").", idx);
			goto error_cleanup;
		}
		if ((bh - bl + bs) >= (sizeof(uint64_t) * 8)) {
			NS_LOG(NS_LOG_LVL_FATAL, "Internal error (import index %" PRIuSZ ").", idx);
			goto error_cleanup;
		}

		if (((bh) >= (sizeof(uint64_t) * 8)) || ((bl) >= (sizeof(uint64_t) * 8)) ||
		    ((bs) >= (sizeof(uint64_t) * 8))) {
			NS_LOG(NS_LOG_LVL_FATAL, "Internal error (import index %" PRIuSZ ").", idx);
			goto error_cleanup;
		}

		if ((bh - bl + 1) == (sizeof(uint64_t) * 8))
			val_mask = UINT64_C(0xFFFFffffFFFFffff);
		else
			val_mask = ((UINT64_C(1) << (bh - bl + 1)) - 1);
		val = (wval >> bs) & val_mask;
		used_mask |= (val_mask) << bs;

		switch (ief->op) {
		case '=':
			*eword = NFP_ELF_BITS_SET(*eword, val, val_mask, bl);
			break;
		case '&':
			*eword &= NFP_ELF_BITS_SET(*eword, val, val_mask, bl);
			break;
		case '|':
			*eword |= NFP_ELF_BITS_SET(*eword, val, val_mask, bl);
			break;
		default:
			NS_LOG(NS_LOG_LVL_FATAL,
			       "Internal error, bad operand (import index %" PRIuSZ ").",
			       idx);
			goto error_cleanup;
		}
	}

	if ((wval & ~used_mask) != 0)
		return -ERANGE;

	return 0;
error_cleanup:
	return -err;
}

size_t nfp_elf_data(struct nfp_elf *ectx, size_t secidx, void **data)
{
	struct nfp_elf_Elf64_Shdr *sec = NULL;
	int err = EINVAL;

	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		goto error_cleanup;
	}

	if (secidx >= ectx->shdrs_cnt) {
		err = ENOENT;
		NS_LOG(NS_LOG_LVL_FATAL,
		       "Section index %" PRIuSZ " is out of range (%" PRIuSZ " sections).", secidx,
		       ectx->shdrs_cnt);
		goto error_cleanup;
	}

	sec = &ectx->shdrs[secidx];

	if (data)
		*data = ectx->shdrs_data[secidx];

	if (ectx->shdrs_data[secidx])
		return (size_t)sec->sh_size;
	return 0;
error_cleanup:
	errno = err;
	return 0;
}

static int _nfp_elf_ustore_ecc_parity(uint64_t value)
{
	int n, parity = 0;

	for (n = 0; n < NFP_ELF_USTORE_WORDSIZE; n++, value >>= 1)
		parity ^= (value & 1);

	return parity;
}

void nfp_elf_ustore_ecc_calculate_and_set_bits(struct nfp_elf *ectx, uint64_t *eword)
{
	unsigned char ecc = 0;
	int n;

	switch (nfp_elf_family(ectx)) {
	case NFP_CHIP_FAMILY_NFP3200:
	case NFP_CHIP_FAMILY_NFP6000:
	case NFP_CHIP_FAMILY_NFP3800:
		/* FHTXXX Find out if nfp6000 ECC bits are same as nfp3200 */
		*eword &= (UINT64_C(1) << NFP_ELF_USTORE_WORDSIZE) - 1;
		for (n = 0; n < NFP_ELF_USTORE_ECC_POLY_WORDCNT; n++) {
			ecc |= _nfp_elf_ustore_ecc_parity(_nfp_elf_ecc_polynomials[n] & *eword)
			       << n;
		}
		*eword |= ((uint64_t)ecc << NFP_ELF_USTORE_WORDSIZE);
		break;
	default:
		NS_LOG(NS_LOG_LVL_ERROR, "Unsupported chip family.");
		break;
	}
}

struct nfp_elf_Elf_meconfig *nfp_elf_meconf(struct nfp_elf *ectx, int meid)
{
	if ((!ectx) || (meid < 0)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return 0;
	}

	switch (nfp_elf_family(ectx)) {
	case NFP_CHIP_FAMILY_NFP3200:
		if (!ectx->meconfs)
			return NULL;

		if (NFP3200_MEID2MELIN(meid) >= (int)ectx->meconfs_cnt) {
			NS_LOG(NS_LOG_LVL_ERROR,
			       "Microengine out of meconf table range.");
			return NULL;
		}

		return &ectx->meconfs[NFP3200_MEID2MELIN(meid)];
	case NFP_CHIP_FAMILY_NFP6000:
	case NFP_CHIP_FAMILY_NFP3800:
		break;
	default:
		NS_LOG(NS_LOG_LVL_ERROR, "Unsupported chip family.");
		break;
	}

	return NULL;
}

int nfp_elf_me_has_text(struct nfp_elf *ectx, int meid)
{
	struct nfp_elf_Elf64_Shdr *sec;
	size_t idx;

	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return 0;
	}

	for (sec = ectx->shdrs, idx = 0; idx < ectx->shdrs_cnt; idx++, sec++) {
		if (!nfp_elf_sec_is_text(ectx, idx))
			continue;

		if (NFP_ELF_GET_SHI_MEID(nfp_elf_family(ectx), sec->sh_info) == meid)
			return 1;
	}

	return 0;
}

int nfp_elf_me_has_init_text(struct nfp_elf *ectx, int meid)
{
	struct nfp_elf_Elf64_Shdr *sec;
	size_t idx;

	if (!ectx) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return 0;
	}

	for (sec = ectx->shdrs, idx = 0; idx < ectx->shdrs_cnt; idx++, sec++) {
		if (!nfp_elf_sec_is_init_text(ectx, idx))
			continue;

		if (NFP_ELF_GET_SHI_MEID(nfp_elf_family(ectx), sec->sh_info) == meid)
			return 1;
	}

	return 0;
}

const char *nfp_elf_sec_name(struct nfp_elf *ectx, size_t secidx)
{
	struct nfp_elf_Elf64_Shdr *shdr = NULL;

	if ((!ectx) || (secidx >= ectx->shdrs_cnt) || (!ectx->shstrtab))
		return _nfp_elf_name_not_found;

	shdr = &ectx->shdrs[secidx];
	if (shdr->sh_name >= ectx->shstrtab_sz)
		return _nfp_elf_name_not_found;

	return &ectx->shstrtab[shdr->sh_name];
}

const char *nfp_elf_sym_name(struct nfp_elf *ectx, const struct nfp_elf_Elf64_Sym *sym)
{
	if ((!ectx) || (!sym) || (!ectx->symstrtab) || (sym->st_name >= ectx->symstrtab_sz))
		return _nfp_elf_name_not_found;

	return &ectx->symstrtab[sym->st_name];
}

size_t nfp_elf_sec_initreg_cnt(struct nfp_elf *ectx, size_t secidx)
{
	struct nfp_elf_Elf64_Shdr *shdr = NULL;

	if ((!ectx) || (secidx >= ectx->shdrs_cnt))
		return 0;

	shdr = &ectx->shdrs[secidx];

	if (shdr->sh_type != NFP_ELF_SHT_NFP_INITREG)
		return 0;

	return (size_t)NFP_ELF_SEC_ENT_CNT(ectx, secidx);
}

const struct nfp_elf_Elf_initRegEntry *nfp_elf_sec_initreg(struct nfp_elf *ectx,
							   size_t secidx,
							   size_t iridx)
{
	struct nfp_elf_Elf64_Shdr *shdr = NULL;

	if ((!ectx) || (secidx >= ectx->shdrs_cnt))
		return NULL;

	shdr = &ectx->shdrs[secidx];

	if (shdr->sh_type != NFP_ELF_SHT_NFP_INITREG)
		return NULL;

	if (iridx >= (size_t)NFP_ELF_SEC_ENT_CNT(ectx, secidx))
		return NULL;

	return &((const struct nfp_elf_Elf_initRegEntry *)ectx->shdrs_data[secidx])[iridx];
}

size_t nfp_elf_sec_initregs(struct nfp_elf *ectx,
			    size_t secidx,
			    const struct nfp_elf_Elf_initRegEntry **entries)
{
	struct nfp_elf_Elf64_Shdr *shdr = NULL;

	if ((!ectx) || (secidx >= ectx->shdrs_cnt))
		return 0;

	shdr = &ectx->shdrs[secidx];

	if (shdr->sh_type != NFP_ELF_SHT_NFP_INITREG)
		return 0;

	if (entries)
		*entries = (const struct nfp_elf_Elf_initRegEntry *)ectx->shdrs_data[secidx];

	return (size_t)NFP_ELF_SEC_ENT_CNT(ectx, secidx);
}

size_t nfp_elf_impexpr_fields(int chip_family, const struct nfp_elf_impexpr_field **fields)
{
	if (fields)
		*fields = NULL;

	switch (chip_family) {
	case NFP_CHIP_FAMILY_NFP3200:
		if (fields)
			*fields = _nfp3200_sdk_elf_impexpr_field_map;
		return _nfp3200_sdk_elf_impexpr_field_map_cnt;
	case NFP_CHIP_FAMILY_NFP6000:
	case NFP_CHIP_FAMILY_NFP3800:
		if (fields)
			*fields = _nfp6000_sdk_elf_impexpr_field_map;
		return _nfp6000_sdk_elf_impexpr_field_map_cnt;
	default:
		break;
	}

	return 0;
}

int nfp_elf_find_user_note(struct nfp_elf *ectx, const char *name, uint32_t *dsz, const void **data)
{
	size_t idx;

	if ((!ectx) || (!name)) {
		NS_LOG(NS_LOG_LVL_FATAL, "Invalid parameters.");
		return NFP_ERRNO(EINVAL);
	}

	for (idx = 0; idx < ectx->user_note_cnt; idx++) {
		if (strcmp(name, ectx->user_notes[idx].name))
			continue;
		if (dsz)
			*dsz = ectx->user_notes[idx].data_sz;
		if (data)
			*data = ectx->user_notes[idx].data;
		return 0;
	}

	return 1; /* Note not found */
}

size_t nfp_elf_user_notes(struct nfp_elf *ectx, const struct nfp_elf_user_note **user_notes)
{
	if (!ectx)
		return 0;

	if (user_notes)
		*user_notes = ectx->user_notes;

	return ectx->user_note_cnt;
}
