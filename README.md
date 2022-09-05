# PacedLinux
Everything necessary for modifying kernel and compiling firmware etc. 
All manuals and resources collected in one place.
Netronome debs are not kept here due to size.


[KernelModifications](https://github.com/Permki/PacedLinux/tree/main/ModifiedFiles) contains the files used for modifying the kernel and the modified version of *TCP Cubic* called *TCP Cubic Paced*\
[NetronomeNIC](https://github.com/Permki/PacedLinux/tree/main/NetronomeNIC) contains the firmware experimentally modified to perform pacing host-side.\
[P4programming](https://github.com/Permki/PacedLinux/tree/main/P4programming) contains various projects and tests made with P4\
[c_packetprocessing](https://github.com/Permki/PacedLinux/tree/main/c_packetprocessing) contains micro-c projects delaying packets receiver side.\
\
See [Netronome NIC-firmware](https://github.com/Netronome/nic-firmware/) for details on compiling NIC firmware if needed. 
For ordinary use of NIC-firmware this is not necessary, but it is necessary for compiling and loading a modified version of the NIC-firmware.\
Compiling NIC-firmware can be done by using the function _compile()_ found in [.bashrc](https://github.com/Permki/PacedLinux/blob/main/.bashrc)\
[tcp_packet.py](https://github.com/Permki/PacedLinux/blob/main/tcp_packet.py) contains a simple script for sending multiple packets. Used for testing pacing with micro-c found in [c_packetprocessing](https://github.com/Permki/PacedLinux/tree/main/c_packetprocessing). Remember to enable TSO (_ethtool -K ethInterface tso on_)
