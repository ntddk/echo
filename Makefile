all: echod echoc echod-ssh echoc-ssh

echod: echod.c
	gcc echod.c -o echod

echoc: echoc.c
	gcc echoc.c -o echoc

echod-ssh: echod-ssh.c
	gcc echod-ssh.c -o echod-ssh -lssl -lcrypto

echoc-ssh: echoc-ssh.c
	gcc echoc-ssh.c -o echoc-ssh -lssl -lcrypto
