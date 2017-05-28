#include "TankHeader.h"
#include "MotorLibrary.c" 

int fd;
Motor * m0;
Motor * m1;
FILE * hitLED;
int firedisable;

int readButton(char press) {
	
	if (press == 'q' || press == 'Q') { // To quit program
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
		return FIRE;
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
	
	char message[50];
	
	
	if (sig == SIGUSR1) {
		firedisable = 1;
		//writeToStream(hitLED, "%d", 1);
		strcpy(message, "HIT! Fire is temporarily disabled\n");
		printf(message);
		write(fd, message, strlen(message));
	 	alarm(4); // Set 2 second timer that will disable our fire button
	} else if (sig == SIGALRM) {
		//writeToStream(hitLED, "%d", 0);
		firedisable = 0; // after alarm goes off reenable firing
		strcpy(message, "Fire is re-enabled!\n");
		printf(message);
		write(fd, message, strlen(message));
	} else if (sig == SIGINT) {
		printf("	QUITTING\n");
		raise(SIGKILL);
	}
	
	//printf("%s%d\n", "firedisable (handler): ", firedisable);
	sigprocmask(SIG_UNBLOCK, &mask, NULL); // Unblock all signals
}


int main() {
	fd = open("/dev/ttyO1", O_RDWR);
	char receive[100];
	char buf[50];
	
	/*
	hitLED = initGPIO(66);
	writeToStream(hitLED, "%d", 1);
	printf("wrote to stream\n");
	*/
	
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
	
	//printf("%s%d\n", "ShmID is ", ShmID);
	
    pid_t* ShmPTR  = (pid_t*) shmat(ShmID, NULL, 0);
    *ShmPTR = pid;
    
    printf("%s%d\n", "Tank PID is: ", pid);
	
	// Setup signal handler
    if (signal(SIGUSR1, handler) == -1) {
		printf("SIGUSR1 install error\n");
		raise(SIGKILL);
	}
	if (signal(SIGALRM, handler) == -1) {
		printf("SIGALRM install error\n");
		raise(SIGKILL);
	}
	if (signal(SIGINT, handler) == -1) {
		printf("SIGINT install error\n");
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
	
		char message[100]; // For outgoing messages
	
		// Read the input
		read(fd, receive, sizeof(receive));
		action = readButton(receive[0]); // Only look at first char
		
		if (action > 0) { // Didn't get NO_ACTION or QUIT, so interpret
			
			//printf("%s%d%s%d\n", "My action is: ", action, "	My prevAction was: ", prevAction);
			
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
						//printf("%s%d\n", "firedisable: ", firedisable);
						if (!firedisable)
							printf("   <Will fire here...>   \n");
						else 
							printf("Fire is disabled!\n");
						break;
					case BRAKE :
						brake_full(m0,m1);
						break;
						
				}
				if (action != 5) // Don't care about storing a fire
					prevAction = action; // Store this action
			}
		} else if (action == QUIT) {
			strcpy(message, "Quitting now \n");
			write(fd, message, strlen(message));
			standby(m0);
			standby(m1);
			raise(SIGKILL); // Kill this process
			exit(EXIT_SUCCESS);
		}
	}
}

