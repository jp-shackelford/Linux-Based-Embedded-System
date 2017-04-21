# Makefile for GPIO User Space

CC=arm-linux-gnueabihf-gcc
SOURCES= osc.c front.c ggb.c back.c 
EXECUTABLE = o f g b 

lcd: $(SOURCES) 
	$(CC) -o o osc.c
	$(CC) -o f front.c
	$(CC) -o g ggb.c
	$(CC) -o b back.c 

.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLE)
