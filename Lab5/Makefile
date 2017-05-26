CC=gcc
SOURCES=UART_init.c 
EXECUTABLE=tank

all: tank sensor

tank: UART_init.c
	$(CC) -o $@ $^
sensor: hit_sensor.c
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLE)
