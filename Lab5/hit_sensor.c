#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "BB_Library.c"

#define THRESHOLD 300
#define IR_RECEIVER "/sys/devices/ocp.3/helper.15/AIN0"
#define WAIT_TIME 4

FILE* adc1;
char analog_path[1024];
struct sigaction sa;
int readADC();


int main() {
	FILE* slots = getStream("/sys/devices/bone_capemgr.9/slots", "w");
	writeToStream(slots, "%s", "cape-bone-iio");
	
	FILE* fp;
	fp = popen("/usr/bin/find /sys/devices/ocp.3/ -name \"helper*\" -type d", "r");
	fgets(analog_path, sizeof(analog_path), fp);
	usleep(1000);

	strtok(analog_path, "\n");
	strcat(analog_path, "/AIN");

	fclose(fp);

	pid_t pid;
	key_t MyKey;
	pid_t *ShmPTR;

	MyKey   = ftok(".", 's');        
    pid_t ShmID   = shmget(MyKey, sizeof(pid_t), 0666);
    
    //printf("Sensor ShmID is %d\n", ShmID);
    
    ShmPTR  = (pid_t *) shmat(ShmID, NULL, 0);
    pid = *ShmPTR;                
    shmdt(ShmPTR);
	
    printf("Sensor PID is %d\n", pid);
	
    while(1) {
    	// If reading is ever past threshold
    	if (readADC() < THRESHOLD) {
    		//printf("%s\n", "above threshold");
    		kill(pid, SIGUSR1); // Send signal to other process
    		while (readADC() < THRESHOLD);
    		//printf("%s\n", "all clear. Sleeping now");
    		sleep(WAIT_TIME); // Can't get hit again within 2 sec of being hit
    		//printf("%s\n", "done sleeping");
    	}
    }

    return 0;
}

// Reads the value on an analog pin and returns it as an int
int readADC() {  
    int fd;
    char buf[64]; 
    char val[4];     //holds up to 4 digits for ADC value  
      
    fd = open(IR_RECEIVER, O_RDONLY);
    if (fd < 0) {  
        printf("Error: Can't open ADC\n");
        exit(1);
    }
      
    read(fd, &val, 4);     //read ADC val (up to 4 digits 0-1799)  
    close(fd);     //close file and stop reading  
      
    return atoi(val);     //returns an integer value (rather than ascii)  
}
