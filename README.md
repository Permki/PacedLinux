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

I recommend joining the nfp-group hosted on Google. A nice place for discussions and questions regarding nfp-programming. Members of Netronome are also present: [NFP-group](https://groups.google.com/g/open-nfp)

Starting RTE-service: [Troubleshooting RTE service](https://help.netronome.com/support/solutions/articles/36000152964-problems-starting-rte-service)

There may be several issues starting the RTE service. One issue may be that you get a collision with the existing packages like this:
```
dpkg: error processing archive /home/permki/software/nfp-sdk-6-rte-v6.1.0.1-preview-Ubuntu-Release-r2750-2018-10-10-ubuntu.binary/dependencies/nfp-bsp/dpkg//nfp-bsp-dkms_2018.08.17.1104-1_all.deb (--install):
 conflicting packages - not installing nfp-bsp-dkms
```
In that case, do 
```
sudo apt-get remove agilio-nfp-driver-dkms
```
before doing the install. 
Here is the whole install for the RTE (as I have used it):
```
apt-get install libftdi1 libjansson4 build-essential \
linux-headers-`uname -r` dkms git

sudo dpkg -i nfp-sdk_6.1.0.1-preview-3243-2_amd64.deb

wget https://deb.netronome.com/gpg/NetronomePublic.key
apt-key add NetronomePublic.key
add-apt-repository "deb https://deb.netronome.com/apt stable main"
apt-get update
apt-get install agilio-nfp-driver-dkms

reboot

sudo ./sdk6_rte_install.sh install

cat /sys/module/nfp/parameters/nfp_dev_cpp
```
Check to see if the last line returns 1. 

I highly recommend checking out this guide:
[Record of the Unix Wars](http://unixwars.blogspot.com/2019/05/programming-netronome-network-card.html)
