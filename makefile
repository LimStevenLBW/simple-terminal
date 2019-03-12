CC=gcc

CFLAGS = -g -c -Wall
#CFLAGS = -ansi -c -Wall -pedantic

A = TermEmu.o

Aprog: $A
	$(CC) -o Aprog $A

TermEmu.o: TermEmu.c
	$(CC) -c TermEmu.c

clean:
	rm *.o

