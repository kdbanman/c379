
# Kirby Banman
# 1173895

# CMPUT 379 EA1
# Assignment 2

CC=gcc -Wall -Wextra

RES1=3\nA B C\n5 4 9\n
PROC1=3\nP1 4 4 6 6 20\nP2 1 1 8 3 25\nP3 1 1 1 0 15\n

RES2=4\nCPU A B C\n3 2 4 1\n
PROC2=3\nP1 1 2 1 1 10 5\nP2 1 1 0 1 12 7\nP3 1 0 3 0 14 3\n

banker: banker.c banker.h
	$(CC) -o banker banker.c

run: banker
	./banker

test: banker
	printf "$(RES1)$(PROC1)" | ./banker
	printf "$(RES2)$(PROC2)" | ./banker

clean:
	rm banker
