all: server-all client-all

server-all:
	make -C server

client-all:
	make -C client

help:
	@echo "targets:"
	@echo "server-all - build the debug server"
	@echo "client-all - build client libraries and tools"

clean:
	make -C server clean
	make -C client clean
