import socket

s = socket.socket(socket.AF_PACKET, socket.SOCK_RAW)
s.bind(("enp5s0np0", 0))

ethernet  = b'\x00\x0c\x29\xd3\xbe\xd6' # MAC Address Destination
ethernet += b'\x00\x0c\x29\xe0\xc4\xaf' # MAC Address Source
ethernet += b'\x08\x00'                 # Protocol-Type: IPv4

ip_header  = b'\x45\x00\x00\x28'  # Version, IHL, Type of Service | Total Length
ip_header += b'\xab\xcd\x00\x00'  # Identification | Flags, Fragment Offset
ip_header += b'\x40\x06\xa6\xec'  # TTL, Protocol | Header Checksum
ip_header += b'\x0a\x0a\x0a\x02'  # Source Address
ip_header += b'\x0a\x0a\x0a\x01'  # Destination Address

tcp_header  = b'\x30\x39\x00\x50' # Source Port | Destination Port
tcp_header += b'\x01\x00\x00\x00' # Sequence Number
tcp_header += b'\x00\x00\x00\x00' # Acknowledgement Number
tcp_header += b'\x70\x01\x02\x03' # Data Offset, Reserved, Flags | Window Size
tcp_header += b'\xe6\x32\x00\x00' # Checksum | Urgent Pointer

packet = ethernet + ip_header + tcp_header

for x in range(3):
    s.send(packet)
