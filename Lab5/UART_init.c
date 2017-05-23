#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "TankHeader.h"

int shots = 5;

int readButton(int press) {
	if (press == 'q') { // if ESC
		printf("%s\n", "quitting");
		exit(EXIT_SUCCESS);
		return QUIT;
	} else if (press == 'w' || press == 'W') {
		printf("%s\n", "UP");
		return FORWARD;
	} else if (press == 'a' || press == 'A') {
		printf("%s\n", "LEFT");
		return TURN_LEFT;
	} else if (press == 's' || press == 'S') {
		printf("%s\n", "DOWN");
		return BACKWARD;
	} else if (press == 'd' || press == 'D') {
		printf("%s\n", "RIGHT");
		return TURN_RIGHT;
	} else if (press == 'f' || press == 'F') {
		if (shots > 0) {
			shots--;
			printf("%s%d\n", "FIRE!            ", shots);
			return FIRE;
		} else {
			printf("%s\n", "out of ammo! Must reload");
			return NO_ACTION;
		}
	} else if (press == 'r' || press == 'R') {
		if (shots < CAP) {
			printf("%s\n", "Reloading");
			sleep(5);
			shots = 5;
			printf("%s\n", "Done reloading");
		}
		return NO_ACTION;
	} else {
		return NO_ACTION;
	}
}


int main() {
	int fd = open("/dev/ttyO1", O_RDWR);
	printf("%d\n", fd);
	char receive[2];
	char buf[20];
	size_t nbytes;
	ssize_t bytes_written;
	
	struct termios options;
	tcgetattr(fd, &options);
	
	// Set up serial comm options
	options.c_cflag = B115200 | CS8 | CREAD | CLOCAL; // 115200 Baud, 8 data bits
	options.c_iflag = IGNPAR | ICRNL; // Ignore parity bits
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);
	
	strcpy(buf, "Hello there!\n");
	write(fd, buf, strlen(buf));
	
	sleep(2);
	strcpy(buf, "Bye for now\n");
	write(fd, buf, strlen(buf));
	
	printf("%s\n", "gonna try reading here...");
	while (1) {
		tcflush(fd, TCIFLUSH);
		read(fd, receive, 1); // Read only first char
		printf("%s\n", receive);
	}
}

