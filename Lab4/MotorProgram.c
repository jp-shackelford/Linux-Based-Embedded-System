#include "MotorLibrary.c"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#define FORWARD 0
#define BRAKE   1 

void timer_Init();
void timer_handler();
struct sigaction sa;
struct itimerval timer; 
int pid; 
FILE* adc0;
FILE* adc2;
FILE* adc4;
FILE* adc6;  
char analog_path[1024];
char analog_path0[1024];
char analog_path2[1024];
char analog_path4[1024];
char analog_path6[1024];
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
    strcpy(analog_path0, analog_path);
    strcpy(analog_path2, analog_path);
    strcpy(analog_path4, analog_path);
    strcpy(analog_path6, analog_path);
    strcat(analog_path0, "/AIN0");
    strcat(analog_path2, "/AIN2");
    strcat(analog_path4, "/AIN4");
    strcat(analog_path6, "/AIN6");
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
  
  // read value from adc 0
  adc0 = fopen(analog_path0, "r");
  char fvalue[1024]; 
  fgets(fvalue, 1024, adc0);

  // read value from adc 2 
  adc2 = fopen(analog_path2, "r");
  char bvalue[1024]; 
  fgets(bvalue, 1024, adc2);

   // read value from adc 4
  adc4 = fopen(analog_path4, "r");
  char lvalue[1024]; 
  fgets(lvalue, 1024, adc4);

   // read value from adc 6
  adc6 = fopen(analog_path6, "r");
  char uvalue[1024]; 
  fgets(uvalue, 1024, adc6);

  int zero_mvolt = atoi(fvalue);
  int two_mvolt  = atoi(bvalue);
  int six_mvolt  = atoi(uvalue);
  int four_mvolt = atoi(lvalue);
  printf("Front %d Back %d topside %d UnderSide %d\n", zero_mvolt, two_mvolt, four_mvolt, six_mvolt);
  switch(command) {
    case SPINLEFT:
    case FORWARD: 
    case BACKWARD:
    case SPINRIGHT:
    default: 
        break;
  }
  fclose(adc0); 
  fclose(adc2);
  fclose(adc4);
  fclose(adc6);
  //sigprocmask(SIG_UNBLOCK, &mask, NULL); // Unblock signals
}
