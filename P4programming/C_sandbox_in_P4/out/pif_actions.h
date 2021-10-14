/* Copyright (C) 2015-2016,  Netronome Systems, Inc.  All rights reserved. */

#ifndef __PIF_ACTIONS_H__
#define __PIF_ACTIONS_H__

/* Warning: generated file - your edits to this file may be lost */

/* Action operation IDs */

#define PIF_ACTION_ID_encap_act 0
#define PIF_ACTION_ID_drop_act 1
#define PIF_ACTION_ID_MAX 1

/* Match action data structure */

__packed struct pif_action_actiondata_encap_act {
    uint32_t tag;
    uint32_t __pif_rule_no;
    uint32_t __pif_table_no;
    uint8_t __pif_padding[2]; /* padding */
    uint16_t prt;
};

__packed struct pif_action_actiondata_drop_act {
    uint32_t __pif_rule_no;
    uint32_t __pif_table_no;
};

#endif /* __PIF_ACTIONS_H__ */
