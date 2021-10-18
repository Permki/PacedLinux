#ifndef __GRO_INTERNAL_UC
#define __GRO_INTERNAL_UC

#include <gro.h>

#define GRO_NN_CTX_shf  2
#define GRO_NN_CTX_msk  ((GRO_ROCTX_PER_BLOCK - 1) << GRO_NN_CTX_shf)
#define GRO_NN_NREL_shf 16
#define GRO_NN_NREL_msk (GRO_MAX_REL_PER_MSG - 1)



#endif /* __GRO_INTERNAL_UC */
