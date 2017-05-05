kefile for GPIO User Space

CC=arm-linux-gnueabihf-gcc
SOURCES= step_sequencer.c
EXECUTABLE = step

step: $(SOURCES)
        $(CC) -o step step_sequencer.c

.PHONY: clean

clean:
        rm -f *.o *~ $(EXECUTABLE)
