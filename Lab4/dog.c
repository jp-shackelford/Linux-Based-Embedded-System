#include "MotorLibrary.c"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#define FORWARD 0
#define BRAKE   1
#define CIRCLE  2
#define WAG     3

void timer_Init();
void timer_handler();
struct sigaction sa;
struct itimerval timer; 
int pid; 
FILE* adc; 
char analog_path[1024];
int command = 50; 
Motor * m0;
Motor * m1;

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
    
    
    // instantiate motor with control pins 115 and 49
    // pwm pin P8_19 (right side), and STBYpin 112
    printf("initializing motors...\n"); 
    m0 = malloc(sizeof(Motor));
    motorinit(m0, 115, 49, 1, 112, 1);
    m1 = malloc(sizeof(Motor));
    motorinit(m1, 60,  48, 0, 112, 1); 
    int s;
    
    printf("loading up some elbow grease...\n");
    timer_Init();
    while(1) {
        if(command == FORWARD) {
            forward_speed(m0, m1, 8);
        } else if(command == BRAKE) {
            brake_full(m0, m1);
        } else if (command == CIRCLE){
            left(m0, m1, 4);
        } else if (command == WAG) {
            brake_full(m0, m1);
            usleep(25000);
            right(m0, m1, 8);
            usleep(25000);
            left(m0, m1, 8);
            usleep(25000);
            right(m0, m1, 8);
            sleep(1);
            brake_full(m0, m1);
        } else {
            brake_full(m0, m1); 
        }
    }
   	
   	free(m0);
    free(m1); 
}

void timer_Init() {
   memset(&sa, 0, sizeof(sa));
   sa.sa_handler = &timer_handler;
   sigaction (SIGVTALRM, &sa, NULL);
   sigaction (SIGINT, &sa, NULL);

   /* Configure the timer to expire after 250 msec... */
   timer.it_value.tv_sec = 0;
   timer.it_value.tv_usec = 10000;
   /* ... and every 250 msec after that. */
   timer.it_interval.tv_sec = 0;
   timer.it_interval.tv_usec = 10000;
   /* Start a virtual timer. It counts down whenever this process is
     executing. */
   setitimer (ITIMER_VIRTUAL, &timer, NULL);
}

void timer_handler(int signal) {
  /* COMMENTING THIS OUTE FOR NOW 
  sigset_t mask;
  sigfillset(&mask);
  sigdelset(&mask, SIGINT); // Block everyting except sigint (^C)
  sigprocmask(SIG_BLOCK, &mask, NULL);
  
  if (signal == SIGINT) {
  	command = BRAKE;
  } */
  
  // read value from adc 
  adc = fopen(analog_path, "r");
  char value[1024]; 
  fgets(value, 1024, adc);
  int adc_mvolt = atoi(value);
  printf("adc_mvolt: %d\n", adc_mvolt); 
  if(adc_mvolt >= 900) {
    command = WAG; 
  } else if (adc_mvolt >= 200 && adc_mvolt <= 700){
    command = FORWARD;
  } else {
    command = CIRCLE;
  }
  
  fclose(adc); 
  //sigprocmask(SIG_UNBLOCK, &mask, NULL); // Unblock signals
}