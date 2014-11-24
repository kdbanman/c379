
# Kirby Banman
# 1173895

# CMPUT 379 EA1
# Assignment 2

CC=gcc -Wall -Wextra
RES=3\r\nA B C\r\n5 4 9\r\n
PROC=3\r\nP1 4 4 6 6 20\r\nP2 1 1 8 3 25\r\nP3 1 1 1 0 15\r\n

banker: banker.c banker.h
	$(CC) -o banker banker.c

run: banker
	./banker

test: banker
	echo -e "$(RES)$(PROC)" | ./banker

clean:
	rm banker
