# We need to be told where the BSP headers and libraries are installed

# First check the default location, this may get overridden
ifndef BSPROOT
BSPROOT_DEFAULT=/opt/netronome
DIREXISTS=$(shell test -d $(BSPROOT_DEFAULT) && echo exists)
ifneq ($(DIREXISTS),)
BSP_INC=$(BSPROOT_DEFAULT)/include
BSP_LIB=$(BSPROOT_DEFAULT)/lib
endif
endif

# We can be told where they are individually
ifdef NFP_CFLAGS
ifdef NFP_LIBS
BSP_INC=$(NFP_CFLAGS)
BSP_LIB=$(NFP_LIBS)
endif
endif

# or we can be told where an installation is
ifdef BSPROOT
BSP_INC=$(BSPROOT)/include
BSP_LIB=$(BSPROOT)/lib
endif

ifndef BSP_INC
$(error BSP_INC is undefined; remember to set either NFP_CFLAGS or BSPROOT)
endif

ifndef BSP_LIB
$(error BSP_LIB is undefined; remember to set either NFP_LIBS or BSPROOT)
endif

$(info Note: using BSP libraries directory $(BSP_LIB) and include directory $(BSP_INC))

# Executable name
DBGSRVBIN = nfp-sdk-hwdbgsrv

# C files and flags
C_FILES = ipc/_nfp.c \
          ipc/_nfp_internal.c \
          ipc/_nfp_explicit.c \
          ipc/_nfp_cpp_internal.c \
          ipc/_nfp_bulk.c \
          ipc/_nfp_cpp.c \
          ipc/_nfp_nffw.c \
          ipc/_nfp_hwdbg.c \
          ipc/_nfp_power.c \
          logging.c \
          main.c \
          dbgsrv.c \
          hwdbg.c \
          hwdbg_mgmt.c \
          brreg.c \
          ipc/ipc_support.c \
          ipc/ipc_client.c \
          ipc/nfp_hwdbg.c \
          ipc/nfp.c \
          ../common/ns_ipc/src/ns_error.c \
          ../common/ns_ipc/src/ns_ipc_common.c \
          ../common/ns_ipc/src/ns_ipc_buf.c \
          ../common/ns_ipc/src/ns_ipc_server.c \
          version.c

C_INC = -I$(BSP_INC) \
        -I. \
        -I./ipc \
        -I../common/ns_ipc/include \
        -I../common/include

C_OBJ_FILES = $(addprefix ,$(C_FILES:.c=.o))
C_FLAGS = -g -Wall -Werror -fPIC
ifneq ($(SDK_SIM),)
C_FLAGS += -DSDK_SIM
endif
CC=gcc

# Linker files and flags
LD_FLAGS = -Wl,-rpath=$(BSP_LIB)
LD_LIBS = -lpthread -lrt -lnfp -lnfp_common

ifneq ("$(wildcard $(BSP_LIB)/libnfp_nffw.so)","")
LD_LIBS += -lnfp_nffw
endif

ifneq ("$(wildcard $(BSP_LIB)/libnfp_nbi.so)","")
LD_LIBS += -lnfp_nbi
endif

ifneq ($(SDK_SIM),)
LD_LIBS += -lnfp_sal
endif
LD_LIBPATHS = -L$(BSP_LIB)

$(DBGSRVBIN): $(C_OBJ_FILES)
	$(CC) -o $@ $^ $(LD_FLAGS) $(LD_LIBS) $(LD_LIBPATHS) 

%.o: %.c
	$(CC) $(C_INC) $(C_FLAGS) -c -o $@ $<

clean:
	rm -f $(DBGSRVBIN) $(C_OBJ_FILES)
