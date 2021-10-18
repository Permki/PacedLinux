/* Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved. */

__asm .init_csr xpbm:ExtMu0IsldXpbmMap.Island.ExtMuXpbMap.MuLookupReg0.AlgorithmicTableLocation0 1
__asm .init_csr xpbm:ExtMu0IsldXpbmMap.Island.ExtMuXpbMap.MuLookupReg1.AlgorithmicTableLocation0 1
__asm .init_csr xpbm:ExtMu0IsldXpbmMap.Island.ExtMuXpbMap.MuLookupReg0.AlgorithmicTableLocation1 1
__asm .init_csr xpbm:ExtMu0IsldXpbmMap.Island.ExtMuXpbMap.MuLookupReg1.AlgorithmicTableLocation1 1

#if NUM_EMU > 1
__asm .init_csr xpbm:ExtMu1IsldXpbmMap.Island.ExtMuXpbMap.MuLookupReg0.AlgorithmicTableLocation0 1
__asm .init_csr xpbm:ExtMu1IsldXpbmMap.Island.ExtMuXpbMap.MuLookupReg1.AlgorithmicTableLocation0 1
__asm .init_csr xpbm:ExtMu1IsldXpbmMap.Island.ExtMuXpbMap.MuLookupReg0.AlgorithmicTableLocation1 1
__asm .init_csr xpbm:ExtMu1IsldXpbmMap.Island.ExtMuXpbMap.MuLookupReg1.AlgorithmicTableLocation1 1

#if NUM_EMU > 2
__asm .init_csr xpbm:ExtMu2IsldXpbmMap.Island.ExtMuXpbMap.MuLookupReg0.AlgorithmicTableLocation0 1
__asm .init_csr xpbm:ExtMu2IsldXpbmMap.Island.ExtMuXpbMap.MuLookupReg1.AlgorithmicTableLocation0 1
__asm .init_csr xpbm:ExtMu2IsldXpbmMap.Island.ExtMuXpbMap.MuLookupReg0.AlgorithmicTableLocation1 1
__asm .init_csr xpbm:ExtMu2IsldXpbmMap.Island.ExtMuXpbMap.MuLookupReg1.AlgorithmicTableLocation1 1
#endif /* NUM_EMU > 2 */
#endif /* NUM_EMU > 1 */

__asm .init_csr xpbm:IntMu0IsldXpbmMap.Island.IntMuXpbMap.MuLookupReg0.AlgorithmicTableLocation0 0
__asm .init_csr xpbm:IntMu0IsldXpbmMap.Island.IntMuXpbMap.MuLookupReg1.AlgorithmicTableLocation0 0
__asm .init_csr xpbm:IntMu0IsldXpbmMap.Island.IntMuXpbMap.MuLookupReg0.AlgorithmicTableLocation1 0
__asm .init_csr xpbm:IntMu0IsldXpbmMap.Island.IntMuXpbMap.MuLookupReg1.AlgorithmicTableLocation1 0

#if NUM_IMU > 1
__asm .init_csr xpbm:IntMu1IsldXpbmMap.Island.IntMuXpbMap.MuLookupReg0.AlgorithmicTableLocation0 0
__asm .init_csr xpbm:IntMu1IsldXpbmMap.Island.IntMuXpbMap.MuLookupReg1.AlgorithmicTableLocation0 0
__asm .init_csr xpbm:IntMu1IsldXpbmMap.Island.IntMuXpbMap.MuLookupReg0.AlgorithmicTableLocation1 0
__asm .init_csr xpbm:IntMu1IsldXpbmMap.Island.IntMuXpbMap.MuLookupReg1.AlgorithmicTableLocation1 0
#endif /* NUM_IMU > 1 */
