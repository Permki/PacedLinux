/*
 * Copyright (C) 2014, Netronome Systems, Inc.  All rights reserved.
 */
#ifndef __UOF_DBG_H__
#define __UOF_DBG_H__

#define DBGUOF_OBJID_LEN       8

typedef enum{
    LitleEndian,
    BigEndian
}Ucld_EndianTyp;

typedef enum{
    GlobalScope,
    FileScope,
    FuncScope,
    BlockScope
}Ucld_ScopeType;

typedef enum{
    CType_None,
    CType_Int1,
    CType_Uint1,
    CType_Int2,
    CType_Uint2,
    CType_Int4,
    CType_Uint4,
    CType_Int8,
    CType_Uint8,
    CType_Enum,
    CType_Reg,
    CType_Sig,
    CType_Struct,
    CType_Array,
    CType_ScratchPtr,
    CType_SramPtr, // For Thornham, extract island IDs and channel from addresses based on target's IMB CPPAT Tables
    CType_MuPtr, // For Thornham, extract island IDs from addresses based on target's IMB CPPAT Tables
    CType_RfifoPtr,
    CType_TfifoPtr,
    CType_LmemPtr,
    CType_UmemPtr,
    CType_AbstractPtr,
    CType_ClsPtr, // For Thornham, extract island IDs from addresses based on target's IMB CPPAT Tables
}Ucld_CType;

typedef enum{
    MeReg=0x52,                     /* ME register */
    MeGlobal=0x53,                  /* shared symbol */
    MeCtxLocal=0x54,                /* thread local variables */
    Rfifo,                          /* receive FIFO */
    Tfifo,                          /* transmit FIFO */
    LocalMem,                       /* shared localmemory */
    Optimized,                      /* optimized variable */
    MultiStorage,                   /* stored in multiple storage types */
    ScratchRing,                    /* scratch ring */
    NNring,                            /* NN ring */
    Sram0Ring,                      /* sram0 ring */
    Sram1Ring,                      /* sram1 ring */
    Sram2Ring,                      /* sram2 ring */
    Sram3Ring                       /* sram3 ring */
}Ucld_VarLocType;

typedef enum{
    UdefTool,                       /* unspecified tool type */
    Uca,                            /* assembler */
    Mcpcom                          /* uC compiler */
}Ucld_LstFileToolType;

#define DBG_OBJS    "DBG_OBJS"
#define DBG_IMAG    "DBG_IMAG"
#define DBG_STRT    "DBG_STRT"
#define DBG_SYMB    "DBG_SYMB"
#define DBG_INIT    "DBG_INIT"

#define UOF_DBG_VERSION 1

typedef struct dbg_ObjTable_S{
   unsigned int     numEntries;     /* num of entries in table */
   /* numEntries * sizeof(element) follows */
}dgb_ObjTable_T;

typedef dgb_ObjTable_T dbg_RegTab_T;
typedef dgb_ObjTable_T dbg_LblTab_T;
typedef dgb_ObjTable_T dbg_SymTab_T;
typedef dgb_ObjTable_T dbg_SrcTab_T;
typedef dgb_ObjTable_T dbg_TypTab_T;
typedef dgb_ObjTable_T dbg_ScopeTab_T;

typedef struct uof_objHdr_S{
   unsigned int     cpuType;            /* CPU type */
   unsigned short   minCpuVer;          /* starting CPU version */
   unsigned short   maxCpuVer;          /* ending CPU version */
   short            maxChunks;          /* max chunks in chunk obj */
   short            numChunks;          /* num of actual chunks */
   unsigned int     nn_chain_modes;     /* Bit <1:0> for cluster 0, bits <3:2> for cluster 1, value maps to CtgtNNConfig CSR <Chain:EnableCPP>*/
   unsigned int     uof_dbg_version;    /* Version of debug data format - new SDK may change this format more easily than the main ELF format */
   /* maxChunks of uof_chunkHdr_T immediately follows this header */
}uof_objHdr_T;

typedef struct dbg_chunkHdr_S{
   char         chunkId[DBGUOF_OBJID_LEN]; /* chunk identifier */
   unsigned int offset;                 /* offset of the chunk in the file */
   unsigned int size;                   /* size of the chunk */
}dbg_chunkHdr_T;                        /* 16 bytes total */


typedef struct dbg_strTab_s{
   unsigned int tableLen;               /* length of table */
   char         *strings;               /* NULL terminated strings */
}dbg_strTab;




typedef struct dbg_InstOprnd_S{
    unsigned int    addr;               /* micro address of instruction */
    int             src1Name;       /* source operand 1 reg name offset in string table */
    int             src1Addr;       /* source operand 1 reg address offset in string table */
    int             src2Name;       /* source operand 2 reg name offset in string table */
    int             src2Addr;       /* source operand 2 reg address offset in string table */
    int             destName;       /* destination reg name offset in string table */
    int             destAddr;       /* destination reg address offset in string table */
    int             xferName;       /* xfer reg name offset in string table */
    int             xferAddr;       /* xfer reg address offset in string table */
    unsigned int    mask;           /* <31:3>unused, <2>I/O indirect, <1>I/O read, <0> I/O write */
    unsigned char   refCount;       /* I/O reference count (1 to 16) */
    unsigned char   deferCount;     /* branch defer count (0-3) */
    unsigned short  reserved1;      /* reserved for future use */
    unsigned int    reserved2;      /* reserved for future use */
}dbg_InstOprnd_T;

#define NFP3200_SDK_UOGDBG_GET_MEID(val)    ((int)NFP3200_MENUM2MEID(val))
#define NFP3200_SDK_UOGDBG_SET_MEID(val)    (NFP3200_MEID2MENUM(val))
#define NFP6000_SDK_UOGDBG_GET_MEID(val)    ((int)NFP6000_MEID(((val) >> 8) & 0xFF, ((val) & 0xFF)))
#define NFP6000_SDK_UOGDBG_SET_MEID(val)    ((NFP6000_MEID_ISLAND(val) << 8) | (NFP6000_MEID_MENUM((val) & 0xFF)))

#define NFP_SDK_UOGDBG_GET_MEID(chip_family, val) \
    (NFP_FAMILY_IS_NFP6000(chip_family) ? NFP6000_SDK_UOGDBG_GET_MEID(val) :     \
    NFP_FAMILY_IS_NFP3200(chip_family) ? NFP3200_SDK_UOGDBG_GET_MEID(val) : -1)
#define NFP_SDK_UOGDBG_SET_MEID(chip_family, val) \
    (NFP_FAMILY_IS_NFP6000(chip_family) ? NFP6000_SDK_UOGDBG_SET_MEID(val) :     \
    NFP_FAMILY_IS_NFP3200(chip_family) ? NFP3200_SDK_UOGDBG_SET_MEID(val) : -1)

typedef struct dbg_Image_S{
   int          lstFileName;        /* list file name str-table offset */
   int meid;                        /* ME ID, access only via NFP_SDK_UOGDBG_*_MEID */
   unsigned int ctxAssigned;        /* bit values of assigned ME contexts */
   unsigned char lstFileCreatedBy;  /* Ucld_LstFileToolType: tool list file was created by */
   unsigned char reserved1;         /* reserved for future use */
   unsigned char ctxMode;           /* number of contexts -- 4 or 8 */
   unsigned char endianMode;        /* endian: little=0, big=1 */
   unsigned int scopeTabOffset;     /* uC scope table offset */
   unsigned int regTabSize;         /* registers table size */
   unsigned int lblTabSize;         /* labels table size */
   unsigned int srcTabSize;         /* source lines table size */
   unsigned int regTabOffset;       /* registers table offset */
   unsigned int lblTabOffset;       /* labels table offset */
   unsigned int srcTabOffset;       /* source lines table offset */
   unsigned int typTabSize;         /* uC type table size */
   unsigned int scopeTabSize;       /* uC scope table size */
   unsigned int typTabOffset;       /* uC type table offset */
   unsigned int instOprndTabSize;   /* instruction operands table size */
   unsigned int instOprndTabOffset; /* instruction operands table offset */
   unsigned int reserved2;          /* reserved for future use */
}dbg_Image_T;


typedef struct dbg_Reg_S{
   unsigned int     name;               /* reg name string-table offset */
   unsigned int     visName;            /* reg visible name string-table offset */
   unsigned short   type;               /* reg type */
   unsigned short   addr;               /* reg address */
   unsigned short   accessMode;         /* uof_RegAccessMode_T: read/write/both/undef */
   unsigned char    visible;            /* register visibility */
   unsigned char    reserved1;          /* reserved for future use */
   unsigned short   refCount;           /* number of contiguous registers allocated */
   unsigned short   reserved2;          /* reserved for future use */
   unsigned int     xoId;               /* xfer order ID */
}dbg_Reg_T;


typedef struct dbg_Label_S{
   int          name;               /* label name string-table offset */
   int          addr;               /* label address */
}dbg_Label_T;

typedef struct dbg_Source_S{
   int              fileName;       /* source file name */
   int              lines;          /* source lines str-table offset */
   unsigned int     lineNum;        /* source line number */
   unsigned int     addr;           /* associated micro address */
   unsigned int     validBkPt;      /* valid breakpoint indicator */
   int              brAddr;         /* branch label address */
   unsigned char    ctxArbKill;     /* this instruction's is a ctx_arb[kill] */
   unsigned char    reserved1;      /* reserved for future use */
   short            regAddr;        /* register address */
   unsigned short   regType;        /* register type */
   unsigned short   deferCount;     /* this instruction's defer count */
   int              funcInsId;      /* function instantiation ID */
}dbg_Source_T;


typedef struct dbg_Type_S{
   int              name;           /* type name string-table offset */
   unsigned short   typeId;         /* id of type -- Ucld_TypeType */
   unsigned short   type;           /* type referenced -- could be itself */
   unsigned int     size;           /* size/bound of the type */
   unsigned int     defOffset;      /* offset to dbg_StructDef_T or dbg_EnumDef_T */
}dbg_Type_T;


/* used for Ucld_TypeType Struct */
typedef struct dbg_StructDef_S{
   unsigned short   numFields;      /* number of fields */
   unsigned short   reserved;       /* reserved for future use */
   unsigned int     fieldOffset;    /* offset to dbg_StructField_T */
}dbg_StructDef_T;

typedef struct dbg_StructField_S{
   int              name;           /* field name string-table offset */
   unsigned int     offset;         /* offset from beg of struct */
   unsigned short   type;           /* field type */
   unsigned char    bitOffset;      /* bitOffset */
   unsigned char    bitSize;        /* bitSize */
}dbg_StructField_T;


/* used for Ucld_TypeType Enum */
typedef struct dbg_EnumDef_S{
   unsigned short   numValues;      /* number of values */
   unsigned short   reserved;       /* reserved for future use */
   unsigned int     valueOffset;    /* offset to dbg_EnumValue_T */
}dbg_EnumDef_T;

typedef struct dbg_EnumValue_S{
   int              name;           /* enum value name string-table offset */
   int              value;          /* enum value */
   unsigned int     reserved;       /* reserved for future use */
}dbg_EnumValue_T;


typedef struct dbg_Scope_S{
   int              name;           /* scope name string-table offset */
   int              interFuncName;  /* for funcScope: internal function name string-table offset */
   int              fileName;       /* file name string-table offset */
   unsigned int     inLinedLine;    /* for funcScope: source line the function was inlined */
   unsigned int        uwordBeg;       /* scope in effect at uword */
   unsigned int        uwordEnd;       /* scope stops at uword */
   unsigned short   lineBeg;        /* scope in effect at source line */
   unsigned short   lineEnd;        /* scope stops at source line */
   unsigned short   type;           /* Ucld_ScopeType -- global, file, funct, ect.. */
   unsigned short   numScopes;      /* number of dbg_Scope_T within this scope */
   unsigned short   numVars;        /* number of variables in this scope */
   unsigned short   numFuncRet;     /* for funcScope: number of function-return locations */
   unsigned int     scopeOffset;    /* offset to dbg_Scope_T within this scope */
   unsigned int     varOffset;      /* offset to dbg_Variable_T within this scope */
   unsigned int     funcRetOffset;  /* func return value offset to dbg_ValueLoc_T */
   int              funcInsId;      /* func instantiation ID */
}dbg_Scope_T;


typedef struct dbg_ValueLoc_S{
   unsigned int     locId;          /* ixp_RegType_T -- reg, mem, spill, uword */
   int              symbName;       /* symbol name offset to string-table */
   unsigned int     location;       /* memAddr, regNum, uwordAddr, or spill-offset */
   unsigned int     multiplier;     /* spill multiplier */
}dbg_ValueLoc_T;


typedef struct dbg_Variable_S{
   int              name;           /* variable name offset to string-table */
   unsigned short   type;           /* type to refer */
   unsigned char    reserved;       /* reserved for future use */
   unsigned char    locType;        /* location type:- Ucld_VarLocType */
   unsigned int     definedOnLine;  /* line var was defined */
   unsigned int     locOffset;      /* offset to dbg_Sloc_T, dbg_Tloc_T, dbg_RlocTab_T, or dbg_Lmloc_T, dbg_Ring_T */
}dbg_Variable_T;


typedef struct dbg_Ring_S{
   int              symbName;       /* symbolic name offset to string-table */
   unsigned int     memOffset;      /* memory region offset */
   unsigned int     id;             /* scratch/sram ring number */
   unsigned int     size;           /* ring byte size */
}dbg_Ring_T;



typedef struct dbg_Sloc_S{
   int              symbName;       /* symbol name offset to string-table */
   unsigned int     offset;         /* offset from the symbol address */
}dbg_Sloc_T;


typedef struct dbg_Tloc_S{
   int              symbName;       /* symbol name offset to string-table */
   unsigned int     offset;         /* local mem offset */
   unsigned int     multiplier;     /* local mem multiplier */
}dbg_Tloc_T;

typedef struct dbg_Lmloc_S{
   unsigned int     offset;         /* localmemory offset */
}dbg_Lmloc_T;


typedef struct dbg_RlocTab_S{
   unsigned short   numEntries;     /* number of live ranges */
   unsigned short   reserved;       /* reserved for future use */
   /* numEntries of dbg_Liverange_T follows */
}dbg_RlocTab_T;

typedef struct dbg_Liverange_S{
   unsigned int     offset;         /* byte offset from var */
   unsigned int     locId;          /* ixp_RegType_T -- reg, mem, spill, uword */
   unsigned int     regNumOrOffset; /* reg-num, or spill-offset */
   unsigned int     multiplier;     /* spill -- multiplier */
   unsigned int     symName;        /* spill -- symbol name (sram0$tls, localmem$tls, etc..) */
   unsigned short   numRanges;      /* number of ranges */
   unsigned char    ambiguous;      /* the location may or may not contain valid value */
   unsigned char    allocType;      /* shared: MeGlobal, ThreadLocal: MeCtxLocal */
   unsigned int     storageSize;    /* storage byte-size */
   unsigned int     rangeOffset;    /* offset to dbg_Range_T */
}dbg_Liverange_T;

typedef struct dbg_Range_S{
   unsigned int start;
   unsigned int stop;
}dbg_Range_T;

typedef struct dbg_RegInit_S{
    int meid;             /* ME ID, access only via NFP_SDK_UOGDBG_*_MEID */
    unsigned short type;  /* reg type */
    unsigned short index; /* reg number */ /*for xfers, this is always absolute S-type, or simply, xfer-num.*/
    unsigned int   value;
}dbg_RegInit_T;

#endif          // __UOF_DBG_H__
