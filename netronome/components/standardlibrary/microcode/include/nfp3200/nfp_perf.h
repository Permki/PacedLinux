/*
 * Copyright (C) 2012  Netronome Systems, Inc.  All rights reserved.
 *
 * @file        nfp3200/nfp_perf.h
 * @brief       Register definitions for Performance Analyzer
 *
 * Disclaimer: this file is provided without any warranty
 * as part of an early-access program, and the content is
 * bound to change before the final release.
 */
#ifndef __NFP_SDK__NFP3200_NFP_PERF_H__
#define __NFP_SDK__NFP3200_NFP_PERF_H__

/* HGID: nfp3200/perf.desc = 30d128ef7055 */
/* Register Type: PAConfig */
#define NFP_PERF_CONFIG                0x0000
#define   NFP_PERF_CONFIG_TEST_MODE                     (0x1 << 31)
#define   NFP_PERF_CONFIG_TEST_MODE_bf                  0, 31, 31
#define   NFP_PERF_CONFIG_TEST_MODE_mask                (0x1)
#define   NFP_PERF_CONFIG_TEST_MODE_bit                 (31)
#define   NFP_PERF_CONFIG_SRM_WRITE                     (0x1 << 30)
#define   NFP_PERF_CONFIG_SRM_WRITE_bf                  0, 30, 30
#define   NFP_PERF_CONFIG_SRM_WRITE_mask                (0x1)
#define   NFP_PERF_CONFIG_SRM_WRITE_bit                 (30)
#define   NFP_PERF_CONFIG_EVENT_SOURCE_1(_x)            (((_x) & 0x3) << 20)
#define   NFP_PERF_CONFIG_EVENT_SOURCE_1_mask           (0x3)
#define   NFP_PERF_CONFIG_EVENT_SOURCE_1_shift          (20)
#define     NFP_PERF_CONFIG_EVENT_SOURCE_1_NONE         (0)
#define     NFP_PERF_CONFIG_EVENT_SOURCE_1_FIFO_NOT_EMPTY (1)
#define     NFP_PERF_CONFIG_EVENT_SOURCE_1_FIFO_FULL    (2)
#define     NFP_PERF_CONFIG_EVENT_SOURCE_1_TOGGLE_EXT_TRIGGER (3)
#define   NFP_PERF_CONFIG_CAPTURE_METHOD_1(_x)          (((_x) & 0x3) << 18)
#define   NFP_PERF_CONFIG_CAPTURE_METHOD_1_mask         (0x3)
#define   NFP_PERF_CONFIG_CAPTURE_METHOD_1_shift        (18)
#define     NFP_PERF_CONFIG_CAPTURE_METHOD_1_32BIT      (0)
#define     NFP_PERF_CONFIG_CAPTURE_METHOD_1_32BIT_TIMESTAMPED (1)
#define     NFP_PERF_CONFIG_CAPTURE_METHOD_1_64BIT      (2)
#define     NFP_PERF_CONFIG_CAPTURE_METHOD_1_96BIT_TIMESTAMPED (3)
#define   NFP_PERF_CONFIG_EXCL_FIFO_1                   (0x1 << 17)
#define   NFP_PERF_CONFIG_EXCL_FIFO_1_bf                0, 17, 17
#define   NFP_PERF_CONFIG_EXCL_FIFO_1_mask              (0x1)
#define   NFP_PERF_CONFIG_EXCL_FIFO_1_bit               (17)
#define   NFP_PERF_CONFIG_JOURNAL_1                     (0x1 << 16)
#define   NFP_PERF_CONFIG_JOURNAL_1_bf                  0, 16, 16
#define   NFP_PERF_CONFIG_JOURNAL_1_mask                (0x1)
#define   NFP_PERF_CONFIG_JOURNAL_1_bit                 (16)
#define   NFP_PERF_CONFIG_EVENT_SOURCE_0(_x)            (((_x) & 0x3) << 8)
#define   NFP_PERF_CONFIG_EVENT_SOURCE_0_mask           (0x3)
#define   NFP_PERF_CONFIG_EVENT_SOURCE_0_shift          (8)
#define     NFP_PERF_CONFIG_EVENT_SOURCE_0_NONE         (0)
#define     NFP_PERF_CONFIG_EVENT_SOURCE_0_FIFO_NOT_EMPTY (1)
#define     NFP_PERF_CONFIG_EVENT_SOURCE_0_FIFO_FULL    (2)
#define     NFP_PERF_CONFIG_EVENT_SOURCE_0_TOGGLE_EXT_TRIGGER (3)
#define   NFP_PERF_CONFIG_CAPTURE_METHOD_0(_x)          (((_x) & 0x3) << 6)
#define   NFP_PERF_CONFIG_CAPTURE_METHOD_0_mask         (0x3)
#define   NFP_PERF_CONFIG_CAPTURE_METHOD_0_shift        (6)
#define     NFP_PERF_CONFIG_CAPTURE_METHOD_0_32BIT      (0)
#define     NFP_PERF_CONFIG_CAPTURE_METHOD_0_32BIT_TIMESTAMPED (1)
#define     NFP_PERF_CONFIG_CAPTURE_METHOD_0_64BIT      (2)
#define     NFP_PERF_CONFIG_CAPTURE_METHOD_0_96BIT_TIMESTAMPED (3)
#define   NFP_PERF_CONFIG_EXCL_FIFO_0                   (0x1 << 5)
#define   NFP_PERF_CONFIG_EXCL_FIFO_0_bf                0, 5, 5
#define   NFP_PERF_CONFIG_EXCL_FIFO_0_mask              (0x1)
#define   NFP_PERF_CONFIG_EXCL_FIFO_0_bit               (5)
#define   NFP_PERF_CONFIG_JOURNAL_0                     (0x1 << 4)
#define   NFP_PERF_CONFIG_JOURNAL_0_bf                  0, 4, 4
#define   NFP_PERF_CONFIG_JOURNAL_0_mask                (0x1)
#define   NFP_PERF_CONFIG_JOURNAL_0_bit                 (4)
#define     NFP_PERF_CONFIG_JOURNAL_0_STOPONFULL        (0x0)
#define     NFP_PERF_CONFIG_JOURNAL_0_JOURNAL           (0x10)
#define   NFP_PERF_CONFIG_SRAM_TRWM(_x)                 (((_x) & 0x7) << 1)
#define   NFP_PERF_CONFIG_SRAM_TRWM_mask                (0x7)
#define   NFP_PERF_CONFIG_SRAM_TRWM_shift               (1)
#define   NFP_PERF_CONFIG_ACTIVE                        (0x1)
#define   NFP_PERF_CONFIG_ACTIVE_bf                     0, 0, 0
#define   NFP_PERF_CONFIG_ACTIVE_mask                   (0x1)
#define   NFP_PERF_CONFIG_ACTIVE_bit                    (0)
/* Register Type: PAStatus */
#define NFP_PERF_STATUS                0x0004
#define   NFP_PERF_STATUS_TEST_MODE                     (0x1 << 31)
#define   NFP_PERF_STATUS_TEST_MODE_bf                  0, 31, 31
#define   NFP_PERF_STATUS_TEST_MODE_mask                (0x1)
#define   NFP_PERF_STATUS_TEST_MODE_bit                 (31)
#define   NFP_PERF_STATUS_STAGE_1_bf                    0, 24, 23
#define   NFP_PERF_STATUS_STAGE_1_mask                  (0x3)
#define   NFP_PERF_STATUS_STAGE_1_shift                 (23)
#define   NFP_PERF_STATUS_VALID_1                       (0x1 << 22)
#define   NFP_PERF_STATUS_VALID_1_bf                    0, 22, 22
#define   NFP_PERF_STATUS_VALID_1_mask                  (0x1)
#define   NFP_PERF_STATUS_VALID_1_bit                   (22)
#define   NFP_PERF_STATUS_EVENT_SOURCE_1_bf             0, 21, 20
#define   NFP_PERF_STATUS_EVENT_SOURCE_1_mask           (0x3)
#define   NFP_PERF_STATUS_EVENT_SOURCE_1_shift          (20)
#define     NFP_PERF_STATUS_EVENT_SOURCE_1_NONE         (0)
#define     NFP_PERF_STATUS_EVENT_SOURCE_1_FIFO_NOT_EMPTY (1)
#define     NFP_PERF_STATUS_EVENT_SOURCE_1_FIFO_FULL    (2)
#define     NFP_PERF_STATUS_EVENT_SOURCE_1_TOGGLE_EXT_TRIGGER (3)
#define   NFP_PERF_STATUS_CAPTURE_METHOD_1_bf           0, 19, 18
#define   NFP_PERF_STATUS_CAPTURE_METHOD_1_mask         (0x3)
#define   NFP_PERF_STATUS_CAPTURE_METHOD_1_shift        (18)
#define     NFP_PERF_STATUS_CAPTURE_METHOD_1_32BIT      (0)
#define     NFP_PERF_STATUS_CAPTURE_METHOD_1_32BIT_TIMESTAMPED (1)
#define     NFP_PERF_STATUS_CAPTURE_METHOD_1_64BIT      (2)
#define     NFP_PERF_STATUS_CAPTURE_METHOD_1_96BIT_TIMESTAMP (3)
#define   NFP_PERF_STATUS_EXCL_FIFO_1                   (0x1 << 17)
#define   NFP_PERF_STATUS_EXCL_FIFO_1_bf                0, 17, 17
#define   NFP_PERF_STATUS_EXCL_FIFO_1_mask              (0x1)
#define   NFP_PERF_STATUS_EXCL_FIFO_1_bit               (17)
#define   NFP_PERF_STATUS_JOURNAL_1                     (0x1 << 16)
#define   NFP_PERF_STATUS_JOURNAL_1_bf                  0, 16, 16
#define   NFP_PERF_STATUS_JOURNAL_1_mask                (0x1)
#define   NFP_PERF_STATUS_JOURNAL_1_bit                 (16)
#define   NFP_PERF_STATUS_STAGE_0_bf                    0, 12, 11
#define   NFP_PERF_STATUS_STAGE_0_mask                  (0x3)
#define   NFP_PERF_STATUS_STAGE_0_shift                 (11)
#define   NFP_PERF_STATUS_VALID_0                       (0x1 << 10)
#define   NFP_PERF_STATUS_VALID_0_bf                    0, 10, 10
#define   NFP_PERF_STATUS_VALID_0_mask                  (0x1)
#define   NFP_PERF_STATUS_VALID_0_bit                   (10)
#define   NFP_PERF_STATUS_EVENT_SOURCE_0_bf             0, 9, 8
#define   NFP_PERF_STATUS_EVENT_SOURCE_0_mask           (0x3)
#define   NFP_PERF_STATUS_EVENT_SOURCE_0_shift          (8)
#define     NFP_PERF_STATUS_EVENT_SOURCE_0_NONE         (0)
#define     NFP_PERF_STATUS_EVENT_SOURCE_0_FIFO_NOT_EMPTY (1)
#define     NFP_PERF_STATUS_EVENT_SOURCE_0_FIFO_FULL    (2)
#define     NFP_PERF_STATUS_EVENT_SOURCE_0_TOGGLE_EXT_TRIGGER (3)
#define   NFP_PERF_STATUS_CAPTURE_SOURCE_0_bf           0, 7, 6
#define   NFP_PERF_STATUS_CAPTURE_SOURCE_0_mask         (0x3)
#define   NFP_PERF_STATUS_CAPTURE_SOURCE_0_shift        (6)
#define     NFP_PERF_STATUS_CAPTURE_SOURCE_0_32BIT      (0)
#define     NFP_PERF_STATUS_CAPTURE_SOURCE_0_32BIT_TIMESTAMPED (1)
#define     NFP_PERF_STATUS_CAPTURE_SOURCE_0_64BIT      (2)
#define     NFP_PERF_STATUS_CAPTURE_SOURCE_0_96BIT_TIMESTAMPED (3)
#define   NFP_PERF_STATUS_EXCL_FIFO_0                   (0x1 << 5)
#define   NFP_PERF_STATUS_EXCL_FIFO_0_bf                0, 5, 5
#define   NFP_PERF_STATUS_EXCL_FIFO_0_mask              (0x1)
#define   NFP_PERF_STATUS_EXCL_FIFO_0_bit               (5)
#define   NFP_PERF_STATUS_JOURNAL_0                     (0x1 << 4)
#define   NFP_PERF_STATUS_JOURNAL_0_bf                  0, 4, 4
#define   NFP_PERF_STATUS_JOURNAL_0_mask                (0x1)
#define   NFP_PERF_STATUS_JOURNAL_0_bit                 (4)
#define     NFP_PERF_STATUS_JOURNAL_0_STOPONFULL        (0x0)
#define     NFP_PERF_STATUS_JOURNAL_0_JOURNAL           (0x10)
#define   NFP_PERF_STATUS_SRAM_TRWM_bf                  0, 3, 1
#define   NFP_PERF_STATUS_SRAM_TRWM_mask                (0x7)
#define   NFP_PERF_STATUS_SRAM_TRWM_shift               (1)
#define   NFP_PERF_STATUS_ACTIVE                        (0x1)
#define   NFP_PERF_STATUS_ACTIVE_bf                     0, 0, 0
#define   NFP_PERF_STATUS_ACTIVE_mask                   (0x1)
#define   NFP_PERF_STATUS_ACTIVE_bit                    (0)
/* Register Type: PATimer */
#define NFP_PERF_TIMER                 0x0008
#define   NFP_PERF_TIMER_VALUE_bf                       0, 31, 0
#define   NFP_PERF_TIMER_VALUE_mask                     (0xffffffff)
#define   NFP_PERF_TIMER_VALUE_shift                    (0)
/* Register Type: PAFifoControl */
#define NFP_PERF_FIFO_CONTROL0         0x0010
#define NFP_PERF_FIFO_CONTROL1         0x0018
#define   NFP_PERF_FIFO_CONTROL_OVERFLOW                (0x1 << 26)
#define   NFP_PERF_FIFO_CONTROL_OVERFLOW_bf             0, 26, 26
#define   NFP_PERF_FIFO_CONTROL_OVERFLOW_mask           (0x1)
#define   NFP_PERF_FIFO_CONTROL_OVERFLOW_bit            (26)
#define   NFP_PERF_FIFO_CONTROL_FULL                    (0x1 << 25)
#define   NFP_PERF_FIFO_CONTROL_FULL_bf                 0, 25, 25
#define   NFP_PERF_FIFO_CONTROL_FULL_mask               (0x1)
#define   NFP_PERF_FIFO_CONTROL_FULL_bit                (25)
#define   NFP_PERF_FIFO_CONTROL_EMPTY                   (0x1 << 24)
#define   NFP_PERF_FIFO_CONTROL_EMPTY_bf                0, 24, 24
#define   NFP_PERF_FIFO_CONTROL_EMPTY_mask              (0x1)
#define   NFP_PERF_FIFO_CONTROL_EMPTY_bit               (24)
#define   NFP_PERF_FIFO_CONTROL_WRITEPTR(_x)            (((_x) & 0xfff) << 12)
#define   NFP_PERF_FIFO_CONTROL_WRITEPTR_bf             0, 23, 12
#define   NFP_PERF_FIFO_CONTROL_WRITEPTR_mask           (0xfff)
#define   NFP_PERF_FIFO_CONTROL_WRITEPTR_shift          (12)
#define   NFP_PERF_FIFO_CONTROL_READPTR(_x)             ((_x) & 0xfff)
#define   NFP_PERF_FIFO_CONTROL_READPTR_bf              0, 11, 0
#define   NFP_PERF_FIFO_CONTROL_READPTR_mask            (0xfff)
#define   NFP_PERF_FIFO_CONTROL_READPTR_shift           (0)
/* Register Type: PAFifoData */
#define NFP_PERF_FIFO_DATA0            0x0014
#define NFP_PERF_FIFO_DATA1            0x001c
#define   NFP_PERF_FIFO_DATA_DATA_bf                    0, 31, 0
#define   NFP_PERF_FIFO_DATA_DATA_mask                  (0xffffffff)
#define   NFP_PERF_FIFO_DATA_DATA_shift                 (0)
/* Register Type: PATriggerControl */
#define NFP_PERF_TRIGGER_CONTROL_BOTH  0x0020
#define NFP_PERF_TRIGGER0_CONTROL      0x0100
#define NFP_PERF_TRIGGER1_CONTROL      0x0180
#define   NFP_TRIGGER_CONTROL_TIMEOUT(_x)               (((_x) & 0xfffff) << 12)
#define   NFP_TRIGGER_CONTROL_TIMEOUT_mask              (0xfffff)
#define   NFP_TRIGGER_CONTROL_TIMEOUT_shift             (12)
#define   NFP_TRIGGER_CONTROL_TRIGGER_STATE(_x)         (((_x) & 0xf) << 8)
#define   NFP_TRIGGER_CONTROL_TRIGGER_STATE_mask        (0xf)
#define   NFP_TRIGGER_CONTROL_TRIGGER_STATE_shift       (8)
#define   NFP_TRIGGER_CONTROL_CMD(_x)                   ((_x) & 0xf)
#define   NFP_TRIGGER_CONTROL_CMD_mask                  (0xf)
#define   NFP_TRIGGER_CONTROL_CMD_shift                 (0)
#define     NFP_TRIGGER_CONTROL_CMD_RESET               (1)
#define     NFP_TRIGGER_CONTROL_CMD_START               (2)
#define     NFP_TRIGGER_CONTROL_CMD_PAUSE               (3)
#define     NFP_TRIGGER_CONTROL_CMD_RESTART             (4)
#define     NFP_TRIGGER_CONTROL_CMD_HALT                (5)
/* Register Type: PAMaskCompare */
#define NFP_PERF_MASK_COMPARE(_x)      (0x80 + (0x4 * ((_x) & 0xf)))
#define   NFP_PERF_MASK_COMPARE_SELECT(_x)              (((_x) & 0xf) << 28)
#define   NFP_PERF_MASK_COMPARE_SELECT_bf               0, 31, 28
#define   NFP_PERF_MASK_COMPARE_SELECT_mask             (0xf)
#define   NFP_PERF_MASK_COMPARE_SELECT_shift            (28)
#define   NFP_PERF_MASK_COMPARE_MASK(_x)                (((_x) & 0xff) << 8)
#define   NFP_PERF_MASK_COMPARE_MASK_bf                 0, 15, 8
#define   NFP_PERF_MASK_COMPARE_MASK_mask               (0xff)
#define   NFP_PERF_MASK_COMPARE_MASK_shift              (8)
#define   NFP_PERF_MASK_COMPARE_VALUE(_x)               ((_x) & 0xff)
#define   NFP_PERF_MASK_COMPARE_VALUE_bf                0, 7, 0
#define   NFP_PERF_MASK_COMPARE_VALUE_mask              (0xff)
#define   NFP_PERF_MASK_COMPARE_VALUE_shift             (0)
/* Register Type: PATriggerStatus */
#define NFP_PERF_TRIGGER0_STATUS       0x0100
#define NFP_PERF_TRIGGER1_STATUS       0x0180
#define   NFP_PERF_TRIGGER_STATUS_TIMEOUT_bf            0, 31, 20
#define   NFP_PERF_TRIGGER_STATUS_TIMEOUT_mask          (0xfff)
#define   NFP_PERF_TRIGGER_STATUS_TIMEOUT_shift         (20)
#define   NFP_PERF_TRIGGER_STATUS_EXT_PENDING_IN        (0x1 << 17)
#define   NFP_PERF_TRIGGER_STATUS_EXT_PENDING_IN_bf     0, 17, 17
#define   NFP_PERF_TRIGGER_STATUS_EXT_PENDING_IN_mask   (0x1)
#define   NFP_PERF_TRIGGER_STATUS_EXT_PENDING_IN_bit    (17)
#define   NFP_PERF_TRIGGER_STATUS_EXT_PENDING_OUT       (0x1 << 16)
#define   NFP_PERF_TRIGGER_STATUS_EXT_PENDING_OUT_bf    0, 16, 16
#define   NFP_PERF_TRIGGER_STATUS_EXT_PENDING_OUT_mask  (0x1)
#define   NFP_PERF_TRIGGER_STATUS_EXT_PENDING_OUT_bit   (16)
#define   NFP_PERF_TRIGGER_STATUS_STATE_bf              0, 11, 8
#define   NFP_PERF_TRIGGER_STATUS_STATE_mask            (0xf)
#define   NFP_PERF_TRIGGER_STATUS_STATE_shift           (8)
#define   NFP_PERF_TRIGGER_STATUS_FSM_bf                0, 7, 4
#define   NFP_PERF_TRIGGER_STATUS_FSM_mask              (0xf)
#define   NFP_PERF_TRIGGER_STATUS_FSM_shift             (4)
#define     NFP_PERF_TRIGGER_STATUS_FSM_IDLE            (0)
#define     NFP_PERF_TRIGGER_STATUS_FSM_RUNNING         (1)
#define     NFP_PERF_TRIGGER_STATUS_FSM_PAUSE           (3)
#define     NFP_PERF_TRIGGER_STATUS_FSM_HALTED          (4)
#define     NFP_PERF_TRIGGER_STATUS_FSM_DONE            (5)
/* Register Type: PATriggerCounterRestart */
#define NFP_PERF_TRIGGER0_COUNTER_RESTART0 0x0110
#define NFP_PERF_TRIGGER0_COUNTER_RESTART1 0x0114
#define NFP_PERF_TRIGGER1_COUNTER_RESTART0 0x0190
#define NFP_PERF_TRIGGER1_COUNTER_RESTART1 0x0194
#define   NFP_TRIGGER_COUNTER_RESTART_VALUE(_x)         (_x)
#define   NFP_TRIGGER_COUNTER_RESTART_VALUE_bf          0, 31, 0
#define   NFP_TRIGGER_COUNTER_RESTART_VALUE_mask        (0xffffffff)
#define   NFP_TRIGGER_COUNTER_RESTART_VALUE_shift       (0)
/* Register Type: PATriggerCounter */
#define NFP_PERF_TRIGGER0_COUNTER0     0x0118
#define NFP_PERF_TRIGGER0_COUNTER1     0x011c
#define NFP_PERF_TRIGGER1_COUNTER0     0x0198
#define NFP_PERF_TRIGGER1_COUNTER1     0x019c
#define   NFP_TRIGGER_COUNTER_VALUE_bf                  0, 31, 0
#define   NFP_TRIGGER_COUNTER_VALUE_mask                (0xffffffff)
#define   NFP_TRIGGER_COUNTER_VALUE_shift               (0)
/* Register Type: PATriggerPerformanceCounter */
#define NFP_PERF_TRIGGER0_PERF_COUNTER(_x) (0x120 + (0x4 * ((_x) & 0x3)))
#define NFP_PERF_TRIGGER1_PERF_COUNTER(_x) (0x1a0 + (0x4 * ((_x) & 0x3)))
#define   NFP_TRIGGER_PERF_COUNTER_VALUE(_x)            (_x)
#define   NFP_TRIGGER_PERF_COUNTER_VALUE_bf             0, 31, 0
#define   NFP_TRIGGER_PERF_COUNTER_VALUE_mask           (0xffffffff)
#define   NFP_TRIGGER_PERF_COUNTER_VALUE_shift          (0)
/* Register Type: PATriggerStateCompare */
#define NFP_PERF_TRIGGER0_STATE_COMPARE(_x) (0x140 + (0x8 * ((_x) & 0x7)))
#define NFP_PERF_TRIGGER1_STATE_COMPARE(_x) (0x1c0 + (0x8 * ((_x) & 0x7)))
#define   NFP_TRIGGER_STATE_COMPARE_CAPTURE(_x)         (((_x) & 0x1f) << 16)
#define   NFP_TRIGGER_STATE_COMPARE_CAPTURE_bf          0, 20, 16
#define   NFP_TRIGGER_STATE_COMPARE_CAPTURE_mask        (0x1f)
#define   NFP_TRIGGER_STATE_COMPARE_CAPTURE_shift       (16)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_DATA_31_0 (0)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_DATA_63_32 (1)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_DATA_95_64 (2)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_TIMESTAMP (3)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_CHANGED_DATA_31_0 (4)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_CHANGED_DATA_63_32 (5)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_CHANGED_DATA_95_64 (6)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_CHANGED_TIMESTAMP (7)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_COUNTER_0_ADD (8)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_COUNTER_1_ADD (9)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_COUNTER_2_ADD (10)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_COUNTER_3_ADD (11)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_COUNTER_0_ADD_COUNTER_1_INC (12)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_COUNTER_2_ADD_COUNTER_3_INC (13)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_STATISTICS (14)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_COUNTER_MASKED_INC (15)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_COUNTER_0_INC (24)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_COUNTER_1_INC (25)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_COUNTER_2_INC (26)
#define     NFP_TRIGGER_STATE_COMPARE_CAPTURE_COUNTER_3_INC (27)
#define   NFP_TRIGGER_STATE_COMPARE_COUNTER             (0x1 << 14)
#define   NFP_TRIGGER_STATE_COMPARE_COUNTER_bf          0, 14, 14
#define   NFP_TRIGGER_STATE_COMPARE_COUNTER_mask        (0x1)
#define   NFP_TRIGGER_STATE_COMPARE_COUNTER_bit         (14)
#define   NFP_TRIGGER_STATE_COMPARE_MATCH               (0x1 << 13)
#define   NFP_TRIGGER_STATE_COMPARE_MATCH_bf            0, 13, 13
#define   NFP_TRIGGER_STATE_COMPARE_MATCH_mask          (0x1)
#define   NFP_TRIGGER_STATE_COMPARE_MATCH_bit           (13)
#define     NFP_TRIGGER_STATE_COMPARE_MATCH_ANY         (0x0)
#define     NFP_TRIGGER_STATE_COMPARE_MATCH_ALL         (0x2000)
#define   NFP_TRIGGER_STATE_COMPARE_SELECT              (0x1 << 12)
#define   NFP_TRIGGER_STATE_COMPARE_SELECT_bf           0, 12, 12
#define   NFP_TRIGGER_STATE_COMPARE_SELECT_mask         (0x1)
#define   NFP_TRIGGER_STATE_COMPARE_SELECT_bit          (12)
#define     NFP_TRIGGER_STATE_COMPARE_SELECT_ALL        (0x0)
#define     NFP_TRIGGER_STATE_COMPARE_SELECT_TOP8       (0x1000)
#define   NFP_TRIGGER_STATE_COMPARE_MASK(_x)            ((_x) & 0xfff)
#define   NFP_TRIGGER_STATE_COMPARE_MASK_bf             0, 11, 0
#define   NFP_TRIGGER_STATE_COMPARE_MASK_mask           (0xfff)
#define   NFP_TRIGGER_STATE_COMPARE_MASK_shift          (0)
/* Register Type: PATriggerStateTransition */
#define NFP_PERF_TRIGGER0_STATE_TRANSITION(_x) (0x144 + (0x8 * ((_x) & 0x7)))
#define NFP_PERF_TRIGGER1_STATE_TRANSITION(_x) (0x1c4 + (0x8 * ((_x) & 0x7)))
#define   NFP_TRIGGER_STATE_TRANSITION_1_DO(_x)         (((_x) & 0x3) << 28)
#define   NFP_TRIGGER_STATE_TRANSITION_1_DO_bf          0, 29, 28
#define   NFP_TRIGGER_STATE_TRANSITION_1_DO_mask        (0x3)
#define   NFP_TRIGGER_STATE_TRANSITION_1_DO_shift       (28)
#define     NFP_TRIGGER_STATE_TRANSITION_1_DO_NOTHING   (0)
#define     NFP_TRIGGER_STATE_TRANSITION_1_DO_EXTERNAL_TRIGGER (1)
#define     NFP_TRIGGER_STATE_TRANSITION_1_DO_PASS      (2)
#define     NFP_TRIGGER_STATE_TRANSITION_1_DO_CAPTURE   (3)
#define   NFP_TRIGGER_STATE_TRANSITION_1_COUNTER_ACTION(_x) (((_x) & 0x3) << 26)
#define   NFP_TRIGGER_STATE_TRANSITION_1_COUNTER_ACTION_bf 0, 27, 26
#define   NFP_TRIGGER_STATE_TRANSITION_1_COUNTER_ACTION_mask (0x3)
#define   NFP_TRIGGER_STATE_TRANSITION_1_COUNTER_ACTION_shift (26)
#define     NFP_TRIGGER_STATE_TRANSITION_1_COUNTER_ACTION_HOLD (0)
#define     NFP_TRIGGER_STATE_TRANSITION_1_COUNTER_ACTION_RESTART (1)
#define     NFP_TRIGGER_STATE_TRANSITION_1_COUNTER_ACTION_DEC (2)
#define     NFP_TRIGGER_STATE_TRANSITION_1_COUNTER_ACTION_INC (3)
#define   NFP_TRIGGER_STATE_TRANSITION_1_WHEN(_x)       (((_x) & 0x3) << 24)
#define   NFP_TRIGGER_STATE_TRANSITION_1_WHEN_bf        0, 25, 24
#define   NFP_TRIGGER_STATE_TRANSITION_1_WHEN_mask      (0x3)
#define   NFP_TRIGGER_STATE_TRANSITION_1_WHEN_shift     (24)
#define     NFP_TRIGGER_STATE_TRANSITION_1_WHEN_TRUE    (0)
#define     NFP_TRIGGER_STATE_TRANSITION_1_WHEN_EXTERNAL_TRIGGER (1)
#define     NFP_TRIGGER_STATE_TRANSITION_1_WHEN_COUNTER_IS_ZERO (2)
#define     NFP_TRIGGER_STATE_TRANSITION_1_WHEN_COUNTER_IS_NONZERO (3)
#define   NFP_TRIGGER_STATE_TRANSITION_1_MASK(_x)       (((_x) & 0xf) << 20)
#define   NFP_TRIGGER_STATE_TRANSITION_1_MASK_bf        0, 23, 20
#define   NFP_TRIGGER_STATE_TRANSITION_1_MASK_mask      (0xf)
#define   NFP_TRIGGER_STATE_TRANSITION_1_MASK_shift     (20)
#define   NFP_TRIGGER_STATE_TRANSITION_1_MATCH          (0x1 << 19)
#define   NFP_TRIGGER_STATE_TRANSITION_1_MATCH_bf       0, 19, 19
#define   NFP_TRIGGER_STATE_TRANSITION_1_MATCH_mask     (0x1)
#define   NFP_TRIGGER_STATE_TRANSITION_1_MATCH_bit      (19)
#define     NFP_TRIGGER_STATE_TRANSITION_1_MATCH_ANY    (0x0)
#define     NFP_TRIGGER_STATE_TRANSITION_1_MATCH_ALL    (0x80000)
#define   NFP_TRIGGER_STATE_TRANSITION_1_NEXT_STATE(_x) (((_x) & 0x7) << 16)
#define   NFP_TRIGGER_STATE_TRANSITION_1_NEXT_STATE_bf  0, 18, 16
#define   NFP_TRIGGER_STATE_TRANSITION_1_NEXT_STATE_mask (0x7)
#define   NFP_TRIGGER_STATE_TRANSITION_1_NEXT_STATE_shift (16)
#define     NFP_TRIGGER_STATE_TRANSITION_1_NEXT_STATE_RESERVED (0)
#define     NFP_TRIGGER_STATE_TRANSITION_1_NEXT_STATE_ADD1 (1)
#define     NFP_TRIGGER_STATE_TRANSITION_1_NEXT_STATE_ADD2 (2)
#define     NFP_TRIGGER_STATE_TRANSITION_1_NEXT_STATE_FINISH (3)
#define     NFP_TRIGGER_STATE_TRANSITION_1_NEXT_STATE_STAY (4)
#define     NFP_TRIGGER_STATE_TRANSITION_1_NEXT_STATE_SUB3 (5)
#define     NFP_TRIGGER_STATE_TRANSITION_1_NEXT_STATE_SUB2 (6)
#define     NFP_TRIGGER_STATE_TRANSITION_1_NEXT_STATE_SUB1 (7)
#define   NFP_TRIGGER_STATE_TRANSITION_0_DO(_x)         (((_x) & 0x3) << 12)
#define   NFP_TRIGGER_STATE_TRANSITION_0_DO_bf          0, 13, 12
#define   NFP_TRIGGER_STATE_TRANSITION_0_DO_mask        (0x3)
#define   NFP_TRIGGER_STATE_TRANSITION_0_DO_shift       (12)
#define     NFP_TRIGGER_STATE_TRANSITION_0_DO_NOTHING   (0)
#define     NFP_TRIGGER_STATE_TRANSITION_0_DO_EXTERNAL_TRIGGER (1)
#define     NFP_TRIGGER_STATE_TRANSITION_0_DO_PASS      (2)
#define     NFP_TRIGGER_STATE_TRANSITION_0_DO_CAPTURE   (3)
#define   NFP_TRIGGER_STATE_TRANSITION_0_COUNTER_ACTION(_x) (((_x) & 0x3) << 10)
#define   NFP_TRIGGER_STATE_TRANSITION_0_COUNTER_ACTION_bf 0, 11, 10
#define   NFP_TRIGGER_STATE_TRANSITION_0_COUNTER_ACTION_mask (0x3)
#define   NFP_TRIGGER_STATE_TRANSITION_0_COUNTER_ACTION_shift (10)
#define     NFP_TRIGGER_STATE_TRANSITION_0_COUNTER_ACTION_HOLD (0)
#define     NFP_TRIGGER_STATE_TRANSITION_0_COUNTER_ACTION_RESTART (1)
#define     NFP_TRIGGER_STATE_TRANSITION_0_COUNTER_ACTION_DEC (2)
#define     NFP_TRIGGER_STATE_TRANSITION_0_COUNTER_ACTION_INC (3)
#define   NFP_TRIGGER_STATE_TRANSITION_0_WHEN(_x)       (((_x) & 0x3) << 8)
#define   NFP_TRIGGER_STATE_TRANSITION_0_WHEN_bf        0, 9, 8
#define   NFP_TRIGGER_STATE_TRANSITION_0_WHEN_mask      (0x3)
#define   NFP_TRIGGER_STATE_TRANSITION_0_WHEN_shift     (8)
#define     NFP_TRIGGER_STATE_TRANSITION_0_WHEN_TRUE    (0)
#define     NFP_TRIGGER_STATE_TRANSITION_0_WHEN_EXTERNAL_TRIGGER (1)
#define     NFP_TRIGGER_STATE_TRANSITION_0_WHEN_COUNTER_IS_ZERO (2)
#define     NFP_TRIGGER_STATE_TRANSITION_0_WHEN_COUNTER_IS_NONZERO (3)
#define   NFP_TRIGGER_STATE_TRANSITION_0_MASK(_x)       (((_x) & 0xf) << 4)
#define   NFP_TRIGGER_STATE_TRANSITION_0_MASK_bf        0, 7, 4
#define   NFP_TRIGGER_STATE_TRANSITION_0_MASK_mask      (0xf)
#define   NFP_TRIGGER_STATE_TRANSITION_0_MASK_shift     (4)
#define   NFP_TRIGGER_STATE_TRANSITION_0_MATCH          (0x1 << 3)
#define   NFP_TRIGGER_STATE_TRANSITION_0_MATCH_bf       0, 3, 3
#define   NFP_TRIGGER_STATE_TRANSITION_0_MATCH_mask     (0x1)
#define   NFP_TRIGGER_STATE_TRANSITION_0_MATCH_bit      (3)
#define     NFP_TRIGGER_STATE_TRANSITION_0_MATCH_ANY    (0x0)
#define     NFP_TRIGGER_STATE_TRANSITION_0_MATCH_ALL    (0x8)
#define   NFP_TRIGGER_STATE_TRANSITION_0_NEXT_STATE(_x) ((_x) & 0x7)
#define   NFP_TRIGGER_STATE_TRANSITION_0_NEXT_STATE_bf  0, 2, 0
#define   NFP_TRIGGER_STATE_TRANSITION_0_NEXT_STATE_mask (0x7)
#define   NFP_TRIGGER_STATE_TRANSITION_0_NEXT_STATE_shift (0)
#define     NFP_TRIGGER_STATE_TRANSITION_0_NEXT_STATE_RESERVED (0)
#define     NFP_TRIGGER_STATE_TRANSITION_0_NEXT_STATE_ADD1 (1)
#define     NFP_TRIGGER_STATE_TRANSITION_0_NEXT_STATE_ADD2 (2)
#define     NFP_TRIGGER_STATE_TRANSITION_0_NEXT_STATE_FINISH (3)
#define     NFP_TRIGGER_STATE_TRANSITION_0_NEXT_STATE_STAY (4)
#define     NFP_TRIGGER_STATE_TRANSITION_0_NEXT_STATE_SUB3 (5)
#define     NFP_TRIGGER_STATE_TRANSITION_0_NEXT_STATE_SUB2 (6)
#define     NFP_TRIGGER_STATE_TRANSITION_0_NEXT_STATE_SUB1 (7)

/* Performance analyzer XPB target */
#define NFP_XPB_PERF    NFP_XPB_DEST(31, 6)

#endif /* __NFP_SDK__NFP3200_NFP_PERF_H__ */
