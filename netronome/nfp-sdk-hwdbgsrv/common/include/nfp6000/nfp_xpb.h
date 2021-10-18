/*
 * Copyright (C) 2014 Netronome Systems, Inc. All rights reserved.
 * Author: Jason McMullan <jason.mcmullan@netronome.com>
 *
 */

#ifndef NFP6000_XPB_H
#define NFP6000_XPB_H

/*
 * For use with NFP6000 Databook "XPB Addressing" section
 */
#define NFP_XPB_OVERLAY(island)  (((island) & 0x3f) << 24)

#define NFP_XPB_ISLAND(island)   (NFP_XPB_OVERLAY(island) + 0x60000)

#define NFP_XPB_ISLAND_of(offset) (((offset) >> 24) & 0x3F)

/*
 * For use with NFP6000 Databook "XPB Island and Device IDs" chapter
 */
#define NFP_XPB_DEVICE(island, slave, device) \
				(NFP_XPB_OVERLAY(island) | \
				 (((slave) & 3) << 22) | \
				 (((device) & 0x3f) << 16))

#endif /* NFP6000_XPB_H */
