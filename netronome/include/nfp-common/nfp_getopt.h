/*
 * Copyright (C) 2010, Netronome Systems, Inc.  All rights reserved.
 *
 */

#ifndef VERSION_H
#define VERSION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <nfp.h>
#include <nfp-common/nfp_platform.h>
#include <stdlib.h>
#include <errno.h>

#ifndef PACKAGE_EXTRAVERSION
#define PACKAGE_EXTRAVERSION ""
#endif

/* Common option parsing and error messages
 */
#define NFP_OPTION_x(var, x)                                                        \
	do {                                                                        \
		char *_cp = optarg;                                                 \
		var = x(optarg, &_cp, 0);                                           \
		if ((_cp == optarg) || (*_cp != 0)) {                               \
			fprintf(stderr, "%s: -%c: Argument '%s' is not a number\n", \
				argv[0], c, optarg);                                \
			return EXIT_FAILURE;                                        \
		}                                                                   \
	} while (0)

#define NFP_OPTION_L(var) NFP_OPTION_x(var, strtol)

#define NFP_OPTION_UL(var) NFP_OPTION_x(var, strtoul)

#define NFP_OPTION_LL(var) NFP_OPTION_x(var, nfp_strtoi64)

#define NFP_OPTION_ULL(var) NFP_OPTION_x(var, nfp_strtou64)

#define NFP_OPTION_MODE(var)                                                             \
	do {                                                                             \
		if ((var) != 0) {                                                        \
			fprintf(stderr, "%s: Conflicting options: -%c & -%c\n", argv[0], \
				(var), c);                                               \
			return EXIT_FAILURE;                                             \
		}                                                                        \
		var = c;                                                                 \
	} while (0)

#define NFP_OPTION_RANGE_x(var, min_val, max_val, x)                                    \
	do {                                                                            \
		if (((var) < (min_val)) || ((var) > (max_val))) {                       \
			fprintf(stderr,                                                 \
				"%s: Option -%c must be in the range %" x "..%" x "\n", \
				argv[0], c, min_val, max_val);                          \
			return EXIT_FAILURE;                                            \
		}                                                                       \
	} while (0)

#define NFP_OPTION_RANGE(var, min_val, max_val) \
	NFP_OPTION_RANGE_x(var, min_val, max_val, "d")

#define NFP_OPTION_HEXRANGE(var, min_val, max_val) \
	NFP_OPTION_RANGE_x(var, min_val, max_val, "x")

#define NFP_OPTION_ERROR(program)                                                      \
	fprintf(stderr,                                                                \
		"%s: Invalid command line\nUse `%s --help' for more information.\n\n", \
		program, program)

#define NFP_DEVICE_ERROR(program, nfp)                                                           \
	fprintf(stderr,                                                                          \
		"%s: Failed to open NFP device %d (%s)\n"                                        \
		"Please check that:\n"                                                               \
		"\t-lspci -d 19ee: shows atleast one Netronome device\n"                         \
		"\t-the nfp device number is correct\n"                                              \
		"\t-the user has read and write permissions to the Netronome device\n"                 \
		"\t-the nfp.ko module is loaded\n"                                                   \
		"\t-the nfp_dev_cpp option is enabled (please try modinfo nfp to see all params)\n", \
		program, nfp, strerror(errno));

/* Common 'usage' strings
 * Comment starts on column 31
 * Use spaces, not tabs.
 */

/*
 * --help
 */

#define HELP_HELP "  -h, --help                  show this help message and exit\n"

#define HELP_OPTION                                                              \
	{ /* .name = */                                                          \
		"help", /* .has_arg = */ 0, /* .flag = */ NULL, /* .val = */ 'h' \
	}

#define HELP_FLAG "h"

static void usage(const char *program);

#define HELP_CASE(program)          \
	case 'h':                   \
		usage(program);     \
		exit(EXIT_SUCCESS); \
		break;

/*
 * --nfp=NUM
 */

#define NFP_HELP "  -n NUM, --nfp=NUM           select NFP device\n"

#define NFP_OPTION                                                              \
	{ /* .name = */                                                         \
		"nfp", /* .has_arg = */ 1, /* .flag = */ NULL, /* .val = */ 'n' \
	}

#define NFP_FLAG "n:"

#define NFP_CASE(nfp_dev)               \
	case 'n':                       \
		NFP_OPTION_UL(nfp_dev); \
		break;

/*
 * --nfp_dbdf=D:B:D.F
 */

#define NFP_DBDF_HELP \
	"  -Z D:B:D.F, --nfp_dbdf=D:B:D.F select NFP PCIE Domain:Bus:Dev.Fn\n"

#define NFP_DBDF_OPTION                                                              \
	{ /* .name = */                                                              \
		"nfp_dbdf", /* .has_arg = */ 1, /* .flag = */ NULL, /* .val = */ 'Z' \
	}

#define NFP_DBDF_FLAG "Z:"

#define NFP_DBDF_CASE(nfp_dev)                                 \
	case 'Z':                                              \
		nfp_dev = nfp_device_number_from_pcie(optarg); \
		break;
/*
 * --version
 */
#define VERSION_HELP "  -V, --version               show revision of this utility\n"

#define VERSION_OPTION                                                              \
	{ /* .name = */                                                             \
		"version", /* .has_arg = */ 0, /* .flag = */ NULL, /* .val = */ 'V' \
	}

#define VERSION_FLAG "V"

#define VERSION_PRINT(program)                                                   \
	do {                                                                     \
		printf("%s (%s) %s%s\n", program, PACKAGE_NAME, PACKAGE_VERSION, \
		       PACKAGE_EXTRAVERSION);                                    \
	} while (0)

#define VERSION_CASE(program)           \
	case 'V':                       \
		VERSION_PRINT(program); \
		exit(EXIT_SUCCESS);     \
		break;

/*
 * --mac=NUM
 */
#define MAC_HELP "  -m NUM, --mac=NUM           select MAC/NBI [0,1]\n"

#define MAC_OPTION                                                              \
	{ /* .name = */                                                         \
		"mac", /* .has_arg = */ 1, /* .flag = */ NULL, /* .val = */ 'm' \
	}

#define MAC_FLAG "m:"

#define MAC_CASE(nbi_id)                        \
	case 'm':                               \
		NFP_OPTION_UL(nbi_id);          \
		NFP_OPTION_RANGE(nbi_id, 0, 1); \
		break;

/*
 * --parsable
 */
#define PARSABLE_HELP                                                             \
	"  --parsable                   produce output that is script-friendly\n" \
	"                               with a format that should not change,\n"  \
	"                               no other output will be on produced on\n" \
	"                               stdout.\n"                                \
	"  --parsable-format            show all possible parsable output formats\n"

#define PARSABLE_OPTIONS                                \
	{						\
		/* .name = */ "parsable",		\
		/* .has_arg = */ 0,			\
		/* .flag = */ &nfpopt_parsable,		\
		/* .val = */ 1				\
	},						\
	{						\
		/* .name = */ "parsable-format",	\
		/* .has_arg = */ 0,			\
		/* .flag = */ &nfpopt_parsable_format,	\
		/* .val = */ 1				\
	}

/* All options
 */

#define NFP_OPTION_HELP NFP_HELP NFP_DBDF_HELP
#define NFP_COMMAND_HELP VERSION_HELP HELP_HELP

#define NFP_NODEV_GETOPT_OPTIONS     \
	VERSION_OPTION, HELP_OPTION, \
	{                            \
		0, 0, 0, 0           \
	}
#define NFP_GETOPT_OPTIONS NFP_OPTION, NFP_DBDF_OPTION, NFP_NODEV_GETOPT_OPTIONS

#define NFP_NODEV_GETOPT_FLAGS(str) "+" VERSION_FLAG HELP_FLAG str
#define NFP_GETOPT_FLAGS(str) NFP_NODEV_GETOPT_FLAGS(str) NFP_FLAG NFP_DBDF_FLAG

#define NFP_GETOPT_CASE_DEFAULT(program)   \
	case 0: /* Flagged options */      \
		break;                     \
	case ':':                          \
	case '?':                          \
		exit(EXIT_FAILURE);        \
		break;                     \
	default:                           \
		NFP_OPTION_ERROR(program); \
		break;

#define NFP_GETOPT_CASE_DEFAULT_NOLONG(program) \
	case ':':                               \
	case '?':                               \
		exit(EXIT_FAILURE);             \
		break;                          \
	default:                                \
		NFP_OPTION_ERROR(program);      \
		break;

#define NFP_NODEV_GETOPT_CASES(program) \
	VERSION_CASE(program)           \
	HELP_CASE(program)              \
	NFP_GETOPT_CASE_DEFAULT(program)

#define NFP_GETOPT_CASES(program, nfp_dev) \
	NFP_CASE(nfp_dev)                  \
	NFP_DBDF_CASE(nfp_dev)             \
	NFP_NODEV_GETOPT_CASES(program)

#define NFP_GETOPT_CASES_NOLONG(program, nfp_dev) \
	NFP_CASE(nfp_dev)                         \
	NFP_DBDF_CASE(nfp_dev)                    \
	VERSION_CASE(program)                     \
	HELP_CASE(program)                        \
	NFP_GETOPT_CASE_DEFAULT_NOLONG(program)

#endif /* VERSION_H */
