#include "TankHeader.h"
#include "MotorLibrary.c" 

int shots = 5;
int fd;
Motor * m0;
Motor * m1;

void reload() {
	int i=0;
	char message[4];
	
	for (i=0; i<shots; i++) {
		strcat(message, " . ");
		write(fd, message, strlen(message));
	}
	
	for (i=shots; i<4; i++) {
		sleep(1);
		strcpy(message, " . ");
		write(fd, message, strlen(message));
	}
	sleep(1);
	strcpy(message, " . \n");
	write(fd, message, strlen(message));
}

int readButton(char press) {
	char message[32];
	
	if (press == 'q') { // To quit program
		strcpy(message, "Quitting now \n");
		write(fd, message, strlen(message));
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
			//shots--;
			//sprintf(message, "%s%d%s\n", "Fire!    ", shots, " shots left. ");
			strcpy(message, "Fire! \n");
			write(fd, message, strlen(message));
			return FIRE;
		} else {
			strcpy(message, "out of ammo! Must reload \n");
			write(fd, message, strlen(message));
			return NO_ACTION;
		}
	} else if (press == 'r' || press == 'R') {
		
		/* SHOTS AND RELOADING CURRENTLY DISABLED */
		strcpy(message, "Reload temporarily disabled \n");
		write(fd, message, strlen(message));
		
		return NO_ACTION;
		if (shots < CAP && shots != CAP) {
			strcpy(message, "Reloading... \n");
			write(fd, message, strlen(message));
			reload(fd);
			shots = 5;
			strcpy(message, "Done reloading \n");
			write(fd, message, strlen(message));
		}
		return NO_ACTION;
	} else if (press == 'b' || press == 'B') {
		return BRAKE;
	} else {
		return NO_ACTION;
	}

}


void handler(int sig) {
	sigset_t mask;
	sigfillset(&mask);
	sigprocmask(SIG_BLOCK, &mask, NULL); // Block all signals so we process one at a time
	
	char message[5];
	strcpy(message, "HIT!\n");
	
	printf(message);
	
	if (sig == SIGUSR1) {
		write(fd, message, strlen(message));
	} 	

	sigprocmask(SIG_UNBLOCK, &mask, NULL); // Unblock all signals
}


int main() {
	fd = open("/dev/ttyO1", O_RDWR);
	char receive[100];
	char buf[50];
	

	// Set up serial comm options
	struct termios options;
	tcgetattr(fd, &options);
	options.c_cflag = B115200 | CS8 | CREAD | CLOCAL; // 115200 Baud, 8 data bits
	options.c_iflag = IGNPAR | ICRNL; // Ignore parity bits
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);


	// Set up process ID and shared memory
	pid_t pid = getpid();
	key_t MyKey;
	MyKey = ftok(".", 's');
	int ShmID   = shmget(MyKey, sizeof(pid_t), IPC_CREAT | 0666);
    pid_t * ShmPTR  = (pid_t *) shmat(ShmID, NULL, 0);
    *ShmPTR = pid;
    
    printf("%s%d\n", "My PID is: ", pid);

    if (signal(SIGUSR1, handler) == -1) {
		printf("SIGUSR1 install error\n");
		raise(SIGKILL);
	}
	

	// Set up drive motors
    m0 = malloc(sizeof(Motor));
    motorinit(m0, 115, 49, 1, 112, 1);
   	m1 = malloc(sizeof(Motor));
    motorinit(m1, 60,  48, 0, 112, 1); 


	strcpy(buf, "Ready!\n");
	write(fd, buf, strlen(buf));
	tcflush(fd, TCOFLUSH); // Flush output buffer
	


	int action = 0;
	int prevAction = 0;
	while (1) {
		// Read the input
		read(fd, receive, sizeof(receive));
		
		action = readButton(receive[0]); // Only look at first char
		
		if (action > 0) { // Didn't get NO_ACTION or QUIT, so interpret
			
			printf("%s%d%s%d\n", "My action is: ", action, "	My prevAction was: ", prevAction);
			
			// If you're going one way and then you try to do the opposite, brake
			if ((action == FORWARD && prevAction == BACKWARD) ||
					(action == BACKWARD && prevAction == FORWARD) ||
					(action == TURN_LEFT && prevAction == TURN_RIGHT) ||
					(action == TURN_RIGHT && prevAction == TURN_LEFT)){
				
				brake_full(m0,m1);
				prevAction = BRAKE; // So this gets stored in prevAction

			} else {
				// Find action to perfrom
				switch(action) {
					case FORWARD :
						forward(m0,m1);
						break;
					case BACKWARD :
						back(m0,m1);
						break;
					case TURN_LEFT :
						left(m0, m1, 8);
						break;
					case TURN_RIGHT : 
						right(m0, m1, 8);
						break;
					case FIRE :
						printf("   <Will fire here...>   \n");
						
						break;
					case BRAKE :
						brake_full(m0,m1);
						break;
						
				}
				prevAction = action; // Store this action
			}
		}
	}
}

