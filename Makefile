
KERNEL :=linux-4.15
USER := $(whoami)
MODS := ModifiedFiles

insert:
	cp $(MODS)/tcp.h $(KERNEL)/include/net/tcp.h
	cp $(MODS)/tcp_output.c $(KERNEL)/net/ipv4/tcp_output.c

bashload:
	cp .bashrc ${HOME}/.bashrc

bashsave:
	cp /home/$(USER)/.bashrc .bashrc

tools:
	sudo apt-get install build-essential libncurses-dev bison flex libssl-dev libelf-dev

sources:
	sudo emacs /etc/apt/sources.list

language:
	gsettings set org.gnome.desktop.input-sources sources "[('xkb', 'no+mac')]"

