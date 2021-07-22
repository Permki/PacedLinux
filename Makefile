
insertfiles:
	#cp ModifiedFiles/tcp_cubic_paced.c linux-hwe-5.8-5.8.0/net/ipv4/tcp_cubic_paced.c 
	cp ModifiedFiles/tcp.h linux-hwe-5.8-5.8.0/include/net/tcp.h
	cp ModifiedFiles/tcp_output.c linux-5.8-5.8.0/net/ipv4/tcp_output.c
