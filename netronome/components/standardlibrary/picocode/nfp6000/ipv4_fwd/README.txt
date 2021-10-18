IPv4 Forwarding picoengine firmware


IPV4 Exception Packets
----------------------
Byte-3      Byte-2      Byte-1      Byte-0
Control Sequence#       SP        Policer BP
Byte-7      Byte-6      Byte-5      Byte-4
DID Result
Byte-11     Byte-10     Byte-9      Byte-8
PVID                    Flags       Pkt Type = 13
Byte-15     Byte-14     Byte-13     Byte-12
L4 Offset   L3 Offset   Eth Offset  Channel#

IPV4 Packets
------------
Byte-3      Byte-2      Byte-1      Byte-0
IPV4 Sequence#          SP        Policer BP
Byte-7      Byte-6      Byte-5      Byte-4
MU Lookup
Byte-11     Byte-10     Byte-9      Byte-8
PVID                    Flags       Pkt Type = 0
Byte-15     Byte-14     Byte-13     Byte-12
DID                     IPv4 Offset Channel#

ARP Packets
-----------
Byte-3      Byte-2      Byte-1      Byte-0
Control Sequence#       SP        Policer BP
Byte-7      Byte-6      Byte-5      Byte-4
DID Result
Byte-11     Byte-10     Byte-9      Byte-8
PVID                    Flags       Pkt Type = 5
Byte-15     Byte-14     Byte-13     Byte-12
Reserved    ARP Offset  Eth Offset  Channel#

Non IPv4 Non Errored Packets/Characterization Fail
--------------------------------------------------
Byte-3      Byte-2      Byte-1      Byte-0
Control Sequence#       SP        Policer BP
Byte-7      Byte-6      Byte-5      Byte-4
DID Result
Byte-11     Byte-10     Byte-9      Byte-8
PVID                    Flags       Pkt Type = 9
Byte-15     Byte-14     Byte-13     Byte-12
L4 Offset   L3 Offset   Eth Offset  Channel#

Errored Packets/Characterization Fail
-------------------------------------
Byte-3      Byte-2      Byte-1      Byte-0
Error Sequence#         SP        Policer BP
Byte-7      Byte-6      Byte-5      Byte-4
Reserved
Byte-11     Byte-10     Byte-9      Byte-8
Reserved                Reserved    Pkt Type = 10
Byte-15     Byte-14     Byte-13     Byte-12
Reserved    Reserved    Reserved    Channel#

Note:
Policer and BP is 4 bits each in Byte-0

