
# Kirby Banman
# 1173895

# CMPUT 379 EA1
# Assignment 2

CC=gcc -Wall -Wextra

RES1=3\r\nA B C\r\n5 4 9\r\n
PROC1=3\r\nP1 4 4 6 6 20\r\nP2 1 1 8 3 25\r\nP3 1 1 1 0 15\r\n

RES2=4\r\nCPU A B C\r\n3 2 4 1\r\n
PROC2=3\r\nP1 1 2 1 1 10 5\r\nP2 1 1 0 1 12 7\r\nP3 1 0 3 0 14 3\r\n

banker: banker.c banker.h
	$(CC) -o banker banker.c

run: banker
	./banker

test: banker
	echo -e "$(RES1)$(PROC1)" | ./banker
	echo -e "$(RES2)$(PROC2)" | ./banker

clean:
	rm banker
