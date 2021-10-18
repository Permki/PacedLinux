IPCSRC = ../common/ns_ipc/src/ns_ipc_buf.c \
         ../common/ns_ipc/src/ns_ipc_client.c \
         ../common/ns_ipc/src/ns_ipc_common.c

NFPLIB = ./libnfp.so
NFPFILES = lib/nfp.c \
           lib/nfp_client_common.c \
           lib/nfp_local.c \
           lib/nfp_internal.c \
           lib/nfp_bulk.c \
           lib/nfp_power.c \
           lib/nfp_cpp.c \
           lib/nfp_cpp_internal.c \
           lib/nfp_cpp_local.c \
           lib/nfp_explicit_local.c \
           $(IPCSRC)

NFPNFFWLIB = ./libnfp_nffw.so
NFPNFFWFILES = lib/nfp_nffw.c \
               lib/nfp_client_common.c \
               lib/nfp_nffw_local.c \
               lib/nfp_mip.c \
               lib/nfp_syms.c \
               $(IPCSRC)

NFPHWDBGLIB = ./libnfp_hwdbg.so
NFPHWDBGFILES = lib/nfp_hwdbg.c \
                lib/nfp_client_common.c \
                lib/nfp_hwdbg_local.c \
                $(IPCSRC)


NFPCOMMONLIB = ./libnfp_common.so
NFPCOMMONFILES = $(wildcard ../common/nfp-common/*.c)

DBGCMD = dbgcmd
DBGCMDSRC = examples/dbgcmd/dbgcmd.c \
            examples/dbgcmd/gui.c \
            examples/dbgcmd/listfile.c \
            examples/dbgcmd/uof.c

# General Make options
# RPC'ed libs
RPC_INC = -Ilib -I../common/ns_ipc/include -I../common/include
RPC_LIB = -lpthread -lrt -lnfp_common -L.
RPC_FLAGS = -Wall -Werror -fPIC -shared -fvisibility=hidden
RPC_FLAGS += -DNFP_RPC_DEFAULT_URL=\"tcp://localhost:20406\"
# nfp common
CMN_LIB = -lpthread -lrt -lz -ljansson
CMN_INC = -I../common/include -I../common/nfp-common/srcinc
CMN_FLAGS = -Wall -Werror -fPIC -shared -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE
# flags for examples
EXAMPLES_LIB = -lpthread -lncurses -lelf -L. -Wl,-rpath=.
EXAMPLES_INC = -I../common/include -Iexamples/dbgcmd
EXAMPLES_FLAGS = -Wall -Werror -g
CC = gcc

all: $(NFPLIB) $(NFPHWDBGLIB) $(DBGCMD) $(NFPNFFWLIB)

$(NFPHWDBGLIB): $(NFPHWDBGFILES) $(NFPCOMMONLIB)
	$(CC) $(RPC_FLAGS) $(RPC_INC) -o $@ $(NFPHWDBGFILES) $(RPC_LIB)

$(NFPLIB): $(NFPFILES) $(NFPCOMMONLIB)
	$(CC) $(RPC_FLAGS) $(RPC_INC) -o $@ $(NFPFILES) $(RPC_LIB)

$(NFPNFFWLIB): $(NFPNFFWFILES) $(NFPCOMMONLIB)
	$(CC) $(RPC_FLAGS) $(RPC_INC) -o $@ $(NFPNFFWFILES) $(RPC_LIB)

$(NFPCOMMONLIB): $(NFPCOMMONFILES)
	$(CC) $(CMN_FLAGS) $(CMN_INC) -o $@ $^ $(CMN_LIB)

$(DBGCMD): $(DBGCMDSRC) $(NFPLIB) $(NFPHWDBGLIB) $(NFPCOMMONLIB)
	$(CC) $(EXAMPLES_FLAGS) $(EXAMPLES_INC) -o $@ $^ $(EXAMPLES_LIB)

clean:
	rm -f *.so $(DBGCMD)
