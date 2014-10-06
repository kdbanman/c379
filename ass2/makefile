# Kirby Banman
# 1173895

# CMPUT 379 EA1
# Assignment 2

all: server_f server_p

server_f: server_f.c server_f.h server.h
	gcc -Wall -Wextra -o server_f server_f.c

server_p: server_p.c server_p.h server.h
	gcc -Wall -Wextra -o server_p server_p.c

clean:
	rm -rf *.o server_f server_p
