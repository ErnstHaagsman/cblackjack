CC=gcc
LDFLAGS=-lncurses
CFLAGS=-Wall -g

blackjack: blackjack.o cards.o main.o
	$(CC) main.o blackjack.o cards.o -o blackjack $(LDFLAGS)

blackjack.o: blackjack.c blackjack.h cards.h
	$(CC) $(CFLAGS) -c blackjack.c

cards.o: cards.c cards.h
	$(CC) $(CFLAGS) -c cards.c

main.o: main.c cards.h blackjack.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm blackjack blackjack.o cards.o main.o
