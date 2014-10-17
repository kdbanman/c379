
# Kirby Banman
# 1173895

# CMPUT 379 EA1
# Assignment 2

CC=gcc -Wall -Wextra

all: server_f server_p

server_f: server_f.c server_f.h server_conf.o server_util.o log_monitor.o
	$(CC) -o server_f server_f.c server_conf.o server_util.o log_monitor.o

server_p: server_p.c server_p.h server_conf.o server_util.o log_monitor.o
	$(CC) -o server_p server_p.c server_conf.o server_util.o log_monitor.o

server_conf.o: server_conf.c server_conf.h
	$(CC) -o server_conf.o -c server_conf.c

server_util.o: server_util.c server_util.h
	$(CC) -o server_util.o -c server_util.c

log_monitor.o: log_monitor.c log_monitor.h
	$(CC) -o log_monitor.o -c log_monitor.c

clean:
	rm -rf *.o server_f server_p
