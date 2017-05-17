#include "MotorLibrary.c"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

FILE* adc; 
char analog_path[1024];

int main() {

	// intialize analog inputs 
	printf("initializing folders...\n");
    FILE* slots = getStream("/sys/devices/bone_capemgr.9/slots", "w");
    writeToStream(slots, "%s", "cape-bone-iio");
	FILE* fp;
	// this is inline bash /usr/bin/find is the program while the rest are its arguments
	// "r" is for the pointer
    fp = popen("/usr/bin/find /sys/devices/ocp.3/ -name \"helper*\" -type d", "r");
    fgets(analog_path, sizeof(analog_path), fp);
    usleep(1000);
    // remove newline 
    strtok(analog_path, "\n");
    // add in the file we're looking for 
    strcat(analog_path, "/AIN0");
    fclose(fp);

	// read value from adc 
	adc = fopen(analog_path, "r");
	char value[1024]; 
	while(1) {
		fgets(value, 1024, adc);
		int adc_mvolt = atoi(value);
		printf("adc_mvolt: %d\n", adc_mvolt);
	}

	fclose(adc); 
}