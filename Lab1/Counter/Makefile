# Makefile for GPIO User Space

CC=arm-linux-gnueabihf-gcc
SOURCES= blinky.c
EXECUTABLE = blinky

blinky: blinky.c
	$(CC) -o blinky blinky.c

.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLE)