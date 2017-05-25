CC=gcc
SOURCES=UART_init.c 
EXECUTABLE=tank

tank: $(SOURCES)
	$(CC) -o tank UART_init.c hit_sensor.c

.PHONY: clean

clean:
	rm -f *.o *~ $(EXECUTABLE)