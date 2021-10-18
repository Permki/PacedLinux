#ifndef __MAC_CMDCFG_ARB_H__
#define __MAC_CMDCFG_ARB_H__

#define ARB_CLS_RING_NUM   4
#define ARB_CLS_RING_BASE  0x1000
#define ARB_CLS_RING_SIZE  64
#define ARB_CLS_ISLAND     1

#define ARB_CLS_BASE_ADDR  (ARB_CLS_ISLAND <<34)
#define ARB_CLS_BASE_ADDR39_32  0x04
#define ARB_CLS_BASE_ADDR_Hi32  0x04000000

#define ARB_CODE_ETH_CMD_CFG_NOOP                 0
#define ARB_CODE_ETH_CMD_CFG_RECACHE              1
#define ARB_CODE_ETH_CMD_CFG_ENABLE_RX            2
#define ARB_CODE_ETH_CMD_CFG_ENABLE_TX            3
#define ARB_CODE_ETH_CMD_CFG_DISABLE_RX           4
#define ARB_CODE_ETH_CMD_CFG_DISABLE_TX           5
#define ARB_CODE_ETH_CMD_CFG_ENABLE_FLUSH         6
#define ARB_CODE_ETH_CMD_CFG_DISABLE_FLUSH        7

#define ARB_CMD(x)         (((x) & 0xff) << 0)
#define ARB_CMD_off(x)     (((x) >>0) & 0xff)
#define ARB_PORT(x)        (((x) & 0x7f) << 8)
#define ARB_PORT_off(x)    (((x) >>8) & 0x7f)
#define ARB_NBI(x)         (((x) & 0x1) << 11)
#define ARB_NBI_off(x)     (((x) >>11) & 0x1)
#define ARB_RECACHE(x)     (((x) & 0x1) << 16)
#define ARB_RECACHE_off(x) (((x) >>16) & 0x1)
#define ARB_SOURCE(x)      (((x) & 0xff) << 24)
#define ARB_SOURCE_off(x)  (((x) >>24) & 0xff)

/* an arbitrary number to insert as the arbiter source value */
#define ARB_SOURCE_APP 4

#endif /* __MAC_CMDCFG_ARB_H__ */
