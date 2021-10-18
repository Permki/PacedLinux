#ifndef __UNROLL_H
#define __UNROLL_H

#include <assert.h>

// a bit heavy handed
#pragma warning(disable:175)

#define UNROLL(BODY, count, MAX) \
    ctassert(__is_ct_const(MAX)); \
    ctassert(MAX > 0); \
    if (MAX == 1) { \
        if (count == 1) { \
            BODY(0) \
        } \
    } \
    else if (MAX == 2) { \
        switch (2 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 3) { \
        switch (3 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 4) { \
        switch (4 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 5) { \
        switch (5 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 6) { \
        switch (6 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 7) { \
        switch (7 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 8) { \
        switch (8 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 9) { \
        switch (9 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 10) { \
        switch (10 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 11) { \
        switch (11 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 12) { \
        switch (12 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 13) { \
        switch (13 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 14) { \
        switch (14 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 15) { \
        switch (15 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 16) { \
        switch (16 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 17) { \
        switch (17 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 18) { \
        switch (18 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 19) { \
        switch (19 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 20) { \
        switch (20 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            case 19: { BODY(19) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 21) { \
        switch (21 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            case 19: { BODY(19) } \
            case 20: { BODY(20) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 22) { \
        switch (22 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            case 19: { BODY(19) } \
            case 20: { BODY(20) } \
            case 21: { BODY(21) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 23) { \
        switch (23 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            case 19: { BODY(19) } \
            case 20: { BODY(20) } \
            case 21: { BODY(21) } \
            case 22: { BODY(22) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 24) { \
        switch (24 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            case 19: { BODY(19) } \
            case 20: { BODY(20) } \
            case 21: { BODY(21) } \
            case 22: { BODY(22) } \
            case 23: { BODY(23) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 25) { \
        switch (25 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            case 19: { BODY(19) } \
            case 20: { BODY(20) } \
            case 21: { BODY(21) } \
            case 22: { BODY(22) } \
            case 23: { BODY(23) } \
            case 24: { BODY(24) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 26) { \
        switch (26 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            case 19: { BODY(19) } \
            case 20: { BODY(20) } \
            case 21: { BODY(21) } \
            case 22: { BODY(22) } \
            case 23: { BODY(23) } \
            case 24: { BODY(24) } \
            case 25: { BODY(25) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 27) { \
        switch (27 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            case 19: { BODY(19) } \
            case 20: { BODY(20) } \
            case 21: { BODY(21) } \
            case 22: { BODY(22) } \
            case 23: { BODY(23) } \
            case 24: { BODY(24) } \
            case 25: { BODY(25) } \
            case 26: { BODY(26) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 28) { \
        switch (28 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            case 19: { BODY(19) } \
            case 20: { BODY(20) } \
            case 21: { BODY(21) } \
            case 22: { BODY(22) } \
            case 23: { BODY(23) } \
            case 24: { BODY(24) } \
            case 25: { BODY(25) } \
            case 26: { BODY(26) } \
            case 27: { BODY(27) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 29) { \
        switch (29 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            case 19: { BODY(19) } \
            case 20: { BODY(20) } \
            case 21: { BODY(21) } \
            case 22: { BODY(22) } \
            case 23: { BODY(23) } \
            case 24: { BODY(24) } \
            case 25: { BODY(25) } \
            case 26: { BODY(26) } \
            case 27: { BODY(27) } \
            case 28: { BODY(28) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 30) { \
        switch (30 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            case 19: { BODY(19) } \
            case 20: { BODY(20) } \
            case 21: { BODY(21) } \
            case 22: { BODY(22) } \
            case 23: { BODY(23) } \
            case 24: { BODY(24) } \
            case 25: { BODY(25) } \
            case 26: { BODY(26) } \
            case 27: { BODY(27) } \
            case 28: { BODY(28) } \
            case 29: { BODY(29) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 31) { \
        switch (31 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            case 19: { BODY(19) } \
            case 20: { BODY(20) } \
            case 21: { BODY(21) } \
            case 22: { BODY(22) } \
            case 23: { BODY(23) } \
            case 24: { BODY(24) } \
            case 25: { BODY(25) } \
            case 26: { BODY(26) } \
            case 27: { BODY(27) } \
            case 28: { BODY(28) } \
            case 29: { BODY(29) } \
            case 30: { BODY(30) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else if (MAX == 32) { \
        switch (32 - count) { \
            case 0: { BODY(0) } \
            case 1: { BODY(1) } \
            case 2: { BODY(2) } \
            case 3: { BODY(3) } \
            case 4: { BODY(4) } \
            case 5: { BODY(5) } \
            case 6: { BODY(6) } \
            case 7: { BODY(7) } \
            case 8: { BODY(8) } \
            case 9: { BODY(9) } \
            case 10: { BODY(10) } \
            case 11: { BODY(11) } \
            case 12: { BODY(12) } \
            case 13: { BODY(13) } \
            case 14: { BODY(14) } \
            case 15: { BODY(15) } \
            case 16: { BODY(16) } \
            case 17: { BODY(17) } \
            case 18: { BODY(18) } \
            case 19: { BODY(19) } \
            case 20: { BODY(20) } \
            case 21: { BODY(21) } \
            case 22: { BODY(22) } \
            case 23: { BODY(23) } \
            case 24: { BODY(24) } \
            case 25: { BODY(25) } \
            case 26: { BODY(26) } \
            case 27: { BODY(27) } \
            case 28: { BODY(28) } \
            case 29: { BODY(29) } \
            case 30: { BODY(30) } \
            case 31: { BODY(31) } \
            default: { __switch_pack(swpack_auto); } \
        } \
    } \
    else { \
        unsigned int i; \
        for (i = 0; i < count; ++i) { BODY(i) } \
    }
 
#define UNROLL_UNBOUNDED(BODY, count) UNROLL(BODY, count, 4294967295)

#endif
