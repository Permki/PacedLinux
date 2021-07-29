
insert:
	cp ModifiedFiles/tcp_cubic_paced.c linux-hwe-5.8-5.8.0v2/net/ipv4/tcp_cubic_paced.c 
	cp ModifiedFiles/tcp.h linux-hwe-5.8-5.8.0v2/include/net/tcp.h
	cp ModifiedFiles/tcp_output.c linux-hwe-5.8-5.8.0v2/net/ipv4/tcp_output.c
	cp ModifiedFiles/skbuff.h linux-hwe-5.8-5.8.0v2/include/linux/skbuff.h

	cp ModifiedFiles/tcp_cubic_paced.c linux-hwe-5.8-5.8.0/net/ipv4/tcp_cubic_paced.c
	cp ModifiedFiles/tcp.h linux-hwe-5.8-5.8.0/include/net/tcp.h
	cp ModifiedFiles/tcp_output.c linux-hwe-5.8-5.8.0/net/ipv4/tcp_output.c
	cp ModifiedFiles/skbuff.h linux-hwe-5.8-5.8.0/include/linux/skbuff.h

bashload:
	cp .bashrc /home/permagnekirkhus/.bashrc

bashsave:
	cp /home/permagnekirkhus/.bashrc .bashrc

tools:
	sudo apt-get install build-essential libncurses-dev bison flex libssl-dev libelf-dev
