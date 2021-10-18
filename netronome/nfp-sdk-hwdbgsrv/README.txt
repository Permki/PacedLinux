Debug Server for NFP6xxx

Overview
-------------------------------------------------

Debug Server provide hardware debug features for NFP6xxx. It provides the
following functionality:

*   MicroEngine run control: run, stop and single step
*   Per-context code breakpoints
        - Allow MEs to stop on a given Program Counter (PC)
*   Handling of software breakpoints
        - Allow running and stepping over ctx_arb[bpt]
*   RPC access to BSP APIs (e.g nfp.h, nfp_cpp.h)
        - RPC calls invoke native BSP library calls
*   RPC access to hardware debug APIs (nfp_hwdbg.h)
        - RPC calls implemented in the server
*   Asynchronous event notification to clients
        - Client callback invoked for breaks, etc.

Compiling Debug Server
-------------------------------------------------

To compile the simulator, first ensure the BSPROOT environment variable is set
to the location of a host BSP installation. The BSP installation should have
client libraries in BSPROOT/lib and header files in BSPROOT/include. It is
also possible to set environment variable NFP_LIBS and NFP_CFLAGS to point
to the BSP libraries and include directories respectively. Note that if
no environment variables are set a default BSPROOT of /opt/netronome will
apply, but only if the directory exists.

To compile:

cd server
make


Running Debug Server
-------------------------------------------------

Note that before running Debug Server you should have the microCode you wish to
debug loaded. To start debug server:

cd server
./nfp-sdk-hwdbgsrv

Running the Debug Commander (dbgcmd) client tool
-------------------------------------------------

Debug Commander is an ncurses tool which provides a text-based user interface
for debugging an ME design.

To compile:

cd client/
make

to run:
./dbgcmd -l design.list -e design.elf

Note the list and elf must be generated with debug info enabled,
ie the -g flag for nfas and nfld.

The list file is optional, but is needed to display code and add breaks.
The elf file is optional, but is needed to load debug info; single stepping
will not work without debug info loaded.

The interface is designed to be vim-esque. Type ":h" for help.

Additional packages
-------------------------------------------------

Ensure that the following package(s) are installed on your system:

* libjansson-dev
* libncurses-dev (libncurses5-dev on Debian-based Linux distributions)
* libelf-dev
