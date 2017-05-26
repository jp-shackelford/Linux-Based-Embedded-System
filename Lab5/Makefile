CC=gcc
EXECUTABLE=tank sensor

all: tank sensor

tank: UART_init.c
	$(CC) -o $@ $^
sensor: hit_sensor.c
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLE)
