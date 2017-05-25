#include "BB_Library.c"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

FILE* adc1;
char analog_path[1024];
struct sigaction sa;
struct itimerval timer; 
int pid; 

void timer_Init();
void timer_handler(int signal);


int main() {
	FILE* slots = getStream("/sys/devices/bone_capemgr.9/slots", "w");
	writeToStream(slots, "%s", "cape-bone-iio");
	
	FILE* fp;
	fp = popen("/usr/bin/find /sys/devices/ocp.3/ -name \"helper*\" -type d", "r");
	fgets(analog_path, sizeof(analog_path), fp);
	usleep(1000);

	strtok(analog_path, "\n");

	strcat(analog_path, "/AIN1");

	fclose(fp);

	timer_Init();

}

void timer_Init() {
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &timer_handler;
	sigaction (SIGVTALRM, &sa, NULL);
	sigaction (SIGINT, &sa, NULL);

	/* Configure the timer to expire after 250 msec... */
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 100000;
	/* ... and every 250 msec after that. */
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 100000;
	/* Start a virtual timer. It counts down whenever this process is
	executing. */
	setitimer (ITIMER_VIRTUAL, &timer, NULL);
}


void timer_handler(int signal) {

	// read value from adc 1
	adc1 = fopen(analog_path, "r");
	char value[1024];
	fgets(value, 1024, adc1);


	int hit_val = atoi(value); // hit sensor
	printf("Hit Value: %d \n", hit_val);

	if(hit_val > 100) {
		printf("HIT!!\n");
	}

	fclose(adc1);
}
