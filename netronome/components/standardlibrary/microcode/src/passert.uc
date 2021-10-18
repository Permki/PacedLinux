/*
 * Copyright (C) 2005-2013 Netronome Systems, Inc.  All rights reserved.
 */

#ifndef _PASSERT_UC_
#define _PASSERT_UC_

#ifndef NFP_LIB_ANY_NFAS_VERSION
    #if (!defined(__NFP_TOOL_NFAS) ||                       \
        (__NFP_TOOL_NFAS < NFP_SW_VERSION(5, 0, 0, 0)) ||   \
        (__NFP_TOOL_NFAS > NFP_SW_VERSION(6, 1, 0, 255)))
        #error "This standard library is not supported for the version of the SDK currently in use."
    #endif
#endif

/** @file passert.uc Preprocessor Assert Macros
 * @addtogroup passert Preprocessor Assert Macros
 * @{
 *
 * @name Preprocessor Assert Macros
 * @{
 *
 */


#macro passert(EXPRESSION)

    #ifdef _PASSERT_EXPRESSION
        #error "passert: _PASSERT_EXPRESSION is already defined" _PASSERT_EXPRESSION
    #endif

    #define_eval _PASSERT_EXPRESSION '(EXPRESSION)'

    #if (is_ct_const(_PASSERT_EXPRESSION))

        #if (_PASSERT_EXPRESSION == 0)

            #error "ASSERTION FAILED:" _PASSERT_EXPRESSION

        #endif

    #elif (is_rt_const(_PASSERT_EXPRESSION))

        .assert _PASSERT_EXPRESSION

    #else

        #error "passert: Unsupported expression:" _PASSERT_EXPRESSION

    #endif

    #undef _PASSERT_EXPRESSION

#endm


#macro passert(EXPRESSION, CHECK)

    #ifdef _PASSERT_EXPRESSION
        #error "passert: _PASSERT_EXPRESSION is already defined" _PASSERT_EXPRESSION
    #endif

    #if (streq(CHECK, "POWER_OF_2"))

        #define_eval _PASSERT_EXPRESSION '(((EXPRESSION) & ((EXPRESSION) - 1)) == 0)'

    #else

        #error "passert: Unknown check" CHECK

    #endif

    #if (is_ct_const(_PASSERT_EXPRESSION))

        #if (_PASSERT_EXPRESSION == 0)

            #error "ASSERTION FAILED:" _PASSERT_EXPRESSION

        #endif

    #elif (is_rt_const(_PASSERT_EXPRESSION))

        .assert _PASSERT_EXPRESSION

    #else

        #error "passert: Unsupported expression:" _PASSERT_EXPRESSION

    #endif

    #undef _PASSERT_EXPRESSION

#endm


#macro passert(EXPRESSION, CHECK, PAR1)

    #ifdef _PASSERT_EXPRESSION
        #error "passert: _PASSERT_EXPRESSION is already defined" _PASSERT_EXPRESSION
    #endif

    #if (streq(CHECK, "MULTIPLE_OF"))

        #define_eval _PASSERT_EXPRESSION '(((EXPRESSION) % (PAR1)) == 0)'

    #elif (streq(CHECK, "MINIMUM"))

        #define_eval _PASSERT_EXPRESSION '((EXPRESSION) >= (PAR1))'

    #elif (streq(CHECK, "MAXIMUM"))

        #define_eval _PASSERT_EXPRESSION '((EXPRESSION) <= (PAR1))'

    #elif (streq(CHECK, "EQ"))

        #define_eval _PASSERT_EXPRESSION '((EXPRESSION) == (PAR1))'

    #elif (streq(CHECK, "NE"))

        #define_eval _PASSERT_EXPRESSION '((EXPRESSION) != (PAR1))'

    #elif (streq(CHECK, "GE"))

        #define_eval _PASSERT_EXPRESSION '((EXPRESSION) >= (PAR1))'

    #elif (streq(CHECK, "LT"))

        #define_eval _PASSERT_EXPRESSION '((EXPRESSION) < (PAR1))'

    #elif (streq(CHECK, "GT"))

        #define_eval _PASSERT_EXPRESSION '((EXPRESSION) > (PAR1))'

    #elif (streq(CHECK, "LE"))

        #define_eval _PASSERT_EXPRESSION '((EXPRESSION) <= (PAR1))'

    #elif (streq(CHECK, "STREQ"))

        #define_eval _PASSERT_EXPRESSION '(streq(EXPRESSION, PAR1) == 1)'

    #elif (streq(CHECK, "STRNEQ"))

        #define_eval _PASSERT_EXPRESSION '(streq(EXPRESSION, PAR1) == 0)'

    #elif (streq(CHECK, "STRSTR"))

        #define_eval _PASSERT_EXPRESSION '(strstr(PAR1, EXPRESSION) > 0)'

    #else

        #error "passert: Unknown check" CHECK

    #endif

    #if (is_ct_const(_PASSERT_EXPRESSION))

        #if (_PASSERT_EXPRESSION == 0)

            #error "ASSERTION FAILED:" _PASSERT_EXPRESSION

        #endif

    #elif (is_rt_const(_PASSERT_EXPRESSION))

        .assert _PASSERT_EXPRESSION

    #else

        #error "passert: Unsupported expression:" _PASSERT_EXPRESSION

    #endif

    #undef _PASSERT_EXPRESSION

#endm


#macro passert(EXPRESSION, CHECK, PAR1, PAR2)

    #ifdef _PASSERT_EXPRESSION
        #error "passert: _PASSERT_EXPRESSION is already defined" _PASSERT_EXPRESSION
    #endif

    #if (streq(CHECK, "IS_IN_RANGE"))

        #define_eval _PASSERT_EXPRESSION '(((EXPRESSION) >= (PAR1)) && ((EXPRESSION) <= (PAR2)))'

    #else

        #error "passert: Unknown check" CHECK

    #endif

    #if (is_ct_const(_PASSERT_EXPRESSION))

        #if (_PASSERT_EXPRESSION == 0)

            #error "ASSERTION FAILED:" _PASSERT_EXPRESSION

        #endif

    #elif (is_rt_const(_PASSERT_EXPRESSION))

        .assert _PASSERT_EXPRESSION

    #else

        #error "passert: Unsupported expression:" _PASSERT_EXPRESSION

    #endif

    #undef _PASSERT_EXPRESSION

#endm


/** @}
 * @}
 */

#endif /* _PASSERT_UC_ */
