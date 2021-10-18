/*
 * Copyright (C) 2014 Netronome Systems, Inc. All rights reserved.
 * Author: Jason McMullan <jason.mcmullan@netronome.com>
 *
 */

#ifndef NFP3800_XPB_H
#define NFP3800_XPB_H

/*
 * For use with NFP3800 Databook "XPB Addressing" section
 */
#define NFP3800_XPB_OVERLAY(island)  (((island) & 0x3f) << 24)

#define NFP3800_XPB_ISLAND(island)   (NFP3800_XPB_OVERLAY(island) + 0x60000)

#define NFP3800_XPB_ISLAND_of(offset) (((offset) >> 24) & 0x3F)

/*
 * For use with NFP3800 Databook "XPB Island and Device IDs" chapter
 */
#define NFP3800_XPB_DEVICE(island, slave, device) \
				(NFP3800_XPB_OVERLAY(island) | \
				 (((slave) & 3) << 22) | \
				 (((device) & 0x3f) << 16))

#endif /* NFP3800_XPB_H */
