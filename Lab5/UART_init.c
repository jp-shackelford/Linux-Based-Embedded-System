#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "TankHeader.h"
#include "MotorLibrary.c" 

int shots = 5;
Motor * m0;
Motor * m1;

void reload() {
	int i=0;
	for (i=1; i<5; i++) {
		sleep(1);
		printf("%s", ". ");
	}
	sleep(1);
	printf("%s\n", ".");
}

int readButton(char press) {
	if (press == 'q') { // To quit program
		printf("%s\n", "quitting");
		exit(EXIT_SUCCESS);
		return QUIT;
	} else if (press == 'w' || press == 'W') {
		return FORWARD;
	} else if (press == 'a' || press == 'A') {
		return TURN_LEFT;
	} else if (press == 's' || press == 'S') {
		return BACKWARD;
	} else if (press == 'd' || press == 'D') {
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
		if (shots < CAP && shots != CAP) {
			printf("%s\n", "Reloading");
			reload();
			shots = 5;
			printf("%s\n", "Done reloading");
		}
		return NO_ACTION;
	} else if (press == 'b' || press == 'B') {
		return BRAKE;
	} else {
		return NO_ACTION;
	}
}


int main() {
	int fd = open("/dev/ttyO1", O_RDWR);
	char receive[2];
	char buf[50];
	size_t nbytes;
	ssize_t bytes_written;
	
	struct termios options;
	tcgetattr(fd, &options);
	
	// Set up serial comm options
	options.c_cflag = B115200 | CS8 | CREAD | CLOCAL; // 115200 Baud, 8 data bits
	options.c_iflag = IGNPAR | ICRNL; // Ignore parity bits
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);
	
	// Set up drive motors
    	m0 = malloc(sizeof(Motor));
    	motorinit(m0, 115, 49, 1, 112, 1);
    	m1 = malloc(sizeof(Motor));
    	motorinit(m1, 60,  48, 0, 112, 1); 

	strcpy(buf, "Done initializing\n");
	write(fd, buf, strlen(buf));
	tcflush(fd, TCOFLUSH); // Flush output buffer

	int prevAction = 0;
	while (1) {
		
		tcflush(fd, TCIFLUSH); // Flush input buffer before each read
		printf("I'm preparing to receive: \n");
		read(fd, receive, 1); // Read only first char
		printf("%s\n", receive);
		int action = readButton(receive[0]);
		if (action > 0) { // Didn't get NO_ACTION or QUIT, so interpret
			if (action == FORWARD && prevAction == BACKWARD) {
				brake_full(m0,m1);
			} else if (action == BACKWARD && prevAction == FORWARD) {
				brake_full(m0,m1);
			} else if (action == LEFT && prevAction == RIGHT){
				brake_full(m0,m1);
			} else if (action == RIGHT && prevAction == LEFT){
				brake_full(m0,m1);
			} else {
				// Find action to perfrom
				switch(action) {
					case FORWARD :
						forward(m0,m1);
						break;
					case BACKWARD :
						back(m0,m1);
						break;
					case LEFT :
						left(m0, m1, -8);
						break;
					case RIGHT : 
						right(m0, m1, -8);
						break;
					case FIRE :
						printf("Will fire here...\n");
						break;
					case BRAKE :
						brake_full(m0,m1);
						break;
						
				}
				// Print action to the console
				printf("%s%d\n", "Will do ", action);
			}
		}
		prevAction = action; // Store action
	}
}

