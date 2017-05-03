# Makefile for GPIO User Space

CC=arm-linux-gnueabihf-gcc
SOURCES= music.c
EXECUTABLE = music

music: $(SOURCES)
	$(CC) -o music music.c
	 
.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLE)
