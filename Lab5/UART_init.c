#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
	int fd = open("/dev/ttyO1", O_RDWR);
	printf("%d\n", fd);
	char receive[10];
	char buf[20];
	size_t nbytes;
	ssize_t bytes_written;
	
	struct termios options;
	tcgetattr(fd, &options);
	
	// Set up serial comm options
	options.c_cflag = B9600 | CS8 | CREAD | CLOCAL; // 9600 Baud, 8 data bits
	options.c_iflag = IGNPAR | ICRNL; // Ignore parity bits
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);
	
	strcpy(buf, "Hello there!\n");
	write(fd, buf, strlen(buf));
	
	sleep(5);
	strcpy(buf, "Bye for now\n");
	write(fd, buf, strlen(buf));
	
	printf("%s\n", "gonna try reading here...");
	while (1) {
		read(fd, receive, sizeof(receive));
		printf("%s\n", receive);
		sleep(1);
	}
}

