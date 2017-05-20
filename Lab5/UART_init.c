#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
	int fd = open("/dev/ttyO1", O_RDWR);
	char receive[100];
	char buf[20];
	size_t nbytes;
	ssize_t bytes_written;
	
	if (fd < 0) {
		printf("Failed to open UART\n");
		exit(0);
	}
	
	struct termios options;
	tcgetattr(fd, &options);
	
	// Set up serial comm options
	options.c_cflag = B115200 | CS8 | CREAD | CLOCAL; // 9600 Baud, 8 data bits
	options.c_iflag = IGNPAR | ICRNL; // Ignore parity bits
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);
	
	strcpy(buf, "testing\n");
	nbytes = strlen(buf);
	
	while (1) {
		bytes_written = write(fd, buf, nbytes);
		printf("%ld\n", bytes_written);
		//read(fd, receive, sizeof(receive));
		printf("%s\n", receive);
		sleep(2);
	}
	close(fd);
}

