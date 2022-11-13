If you wish to create virtual ports to send packets from the NIC to itself:

A host with an Agilio-LX or CX Intelligent Server Adapter must support ARI (Alternative
Routing ID) and Virtualization Technology (Intel VT-d, AMD-Vi, IOMMU) for SR-IOV VFs.
Since SR-IOV is optional part of the PCIe specification and seen as a "server-only" feature is
is typically only supported on server motherboard chipsets. Please refer to your intended test
setup's hardware documentation on whether it is supported.If the host system does not support
ARI, then set env DISABLE_NFD=yes in the /etc/init/nfp-sdk6-rte* startup files, and
use the physical ports on the SmartNIC connected to a suitable 10G/40G NIC for packet
ingress/egress.
• To enable SR-IOV and ARI functionality it needs to be enabled in the system BIOS and the operating system.
To enable in the BIOS typically requires Virtualization Technology (Intel VT-d or AMD-Vi) and SR-IOV to be
enabled globally; some BIOS might also require SR-IOV to be enabled on the specific PCIe port being used
(please refer to your hardware manufacturer's documentation for the specifics). In the OS virtualization needs
be enabled in the kernel startup settings using intel_iommu=on or amd_iommu=on.
