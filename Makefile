
KERNEL :=linux-hwe-5.8-5.8.0
USER := $(whoami)

insert:
	cp ModifiedFiles/tcp_cubic_paced.c $(KERNEL)/net/ipv4/tcp_cubic_paced.c 
	cp ModifiedFiles/tcp.h $(KERNEL)/include/net/tcp.h
	cp ModifiedFiles/tcp_output.c $(KERNEL)/net/ipv4/tcp_output.c

bashload:
	cp .bashrc ${HOME}/.bashrc
	echo "source ~/.bashrc"

bashsave:
	cp /home/$(USER)/.bashrc .bashrc

tools:
	sudo apt-get install build-essential libncurses-dev bison flex libssl-dev libelf-dev

sources:
	sudo emacs /etc/apt/sources.list

language:
	gsettings set org.gnome.desktop.input-sources sources "[('xkb', 'no+mac')]"
#P8cBENz5YNgdB7w
#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
#P8cBENz5YNgdB7w
