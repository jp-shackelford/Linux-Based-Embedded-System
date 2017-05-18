#include  <stdio.h>
#include  <sys/types.h>
#include  <signal.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <stdlib.h>
#include <time.h>


#define THRESHOLD 1200

volatile sig_atomic_t killwhich;

int poll(int which) {
	int data[2];
	if (which) {
		data[0] = readADC(0);
		data[1] = readADC(2);

	} else {
		data[0] = readADC(4);
		data[1] = readADC(6);
	}

	return (data[0]+data[1])/2;
}


void handler(int sig) {
	printf("polling...\n");
	if (poll(0) > THRESHOLD) {
		printf("something in front\n");
		killwhich = 1;
	} else if (poll(1) > THRESHOLD) {
		printf("something in back\n");
		killwhich = 2;
	} else {
		printf("all clear\n");
		killwhich = 0;
	}
}



// Reads the value on an analog pin and returns it as an int
int readADC(unsigned int pin) {  
    int fd;
    char buf[64]; 
    char val[4];     //holds up to 4 digits for ADC value  
      
    // concatenate directory with analog pin number
    sprintf(buf, "%s%d", "/sys/devices/ocp.3/helper.15/AIN", pin);
      
    fd = open(buf, O_RDONLY);
    if (fd < 0) {  
        printf("Error: Can't open ADC\n");
        exit(1);
    }
      
    read(fd, &val, 4);     //read ADC val (up to 4 digits 0-1799)  
    printf("%s\n", &val);
    close(fd);     //close file and stop reading  
      
    return atoi(val);     //returns an integer value (rather than ascii)  
}


int main() {
	pid_t pid;
	key_t MyKey;
	pid_t *ShmPTR;

	MyKey   = ftok(".", 's');        
    pid_t ShmID   = shmget(MyKey, sizeof(pid_t), 0666);
    ShmPTR  = (pid_t *) shmat(ShmID, NULL, 0);
    pid = *ShmPTR;                
    shmdt(ShmPTR);

    printf("My pid is %d\n", pid);
    signal(SIGALRM, handler); // Register with handler

    ualarm(250000, 250000);
    while(1) {
    	if (killwhich == 1) {
			printf("Killing 1\n");
    		kill(pid, SIGUSR1);
    		killwhich = 0;
    	} else if (killwhich == 2) {
			printf("Killing 2\n");
    		kill(pid, SIGUSR2);
    		killwhich = 0;
    	}
    }

    return 0;
}