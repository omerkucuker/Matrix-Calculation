CC=gcc
CFLAGS=-c -Wall

all: matrixcalc

matrixcalc: matrixcalc.c
	$(CC) matrixcalc.c -o matrixcalc -l pthread

clean:
	rm -rf *o main