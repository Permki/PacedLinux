#ifndef __PIF_DEBUG_H__
#define __PIF_DEBUG_H__

#ifdef PIF_DEBUG
#define PIF_DEBUG_EXTRA_PARREP 1
#else
#define PIF_DEBUG_EXTRA_PARREP 0
#endif

extern volatile __lmem uint32_t parrep[PIF_PARREP_LEN_LW + PIF_DEBUG_EXTRA_PARREP];

#define PIF_DEBUG_STATE_WAITING 0
#define PIF_DEBUG_STATE_PARSER 1
#define PIF_DEBUG_STATE_CONTROL 2
#define PIF_DEBUG_STATE_ACTION 3
#define PIF_DEBUG_STATE_DEPARSING 4
#define PIF_DEBUG_STATE_SENDING 5

#define PIF_DEBUG_STATE(stage, node) \
    (((stage & 0xff) << 24) | ((node) & (0xffffff << 0)))

#define PIF_DEBUG_STATE_STAGE_of(state) \
    (((state >> 24) & (0xff))

#define PIF_DEBUG_STATE_NODE_of(state) \
    (((state >> 0) & (0xffffff))

#ifdef PIF_DEBUG

#define PIF_DEBUG_SET_STATE(stage, node) \
    do { \
        parrep[PIF_PARREP_LEN_LW] = PIF_DEBUG_STATE(stage, node); \
    } while (0)

#else
#define PIF_DEBUG_SET_STATE(stage, node)
#endif

#endif /* __PIF_DEBUG_H__ */
