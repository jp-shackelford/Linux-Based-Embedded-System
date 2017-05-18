#include "MotorLibrary.c"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#define HALTF   0
#define FORWARD 1
#define HALTB   2
#define BACK    3
#define MOV_THRESH 600
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
Motor * m0;
Motor * m1;
int state = 0; 

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
        if(state == HALTB || state == HALTF) {
            brake_full(m0, m1);
        } else if(state == FORWARD) {
            forward(m0, m1);
        } else {
            back(m0, m1);
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
   timer.it_value.tv_usec = 100000;
   /* ... and every 250 msec after that. */
   timer.it_interval.tv_sec = 0;
   timer.it_interval.tv_usec = 100000;
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

  int back = atoi(fvalue); // front
  int front  = atoi(bvalue); // back
  int bottom  = atoi(uvalue); // bottom
  int top = atoi(lvalue); // top
  printf("Front %d Back %d Bottom %d Top %d\n",
           front, back, bottom, top);

  // If something is close to the top sensor or the bottom sensor moves out of normal range then stop
  switch(state) {
    case HALTF: if(top < 950) {
                    state = HALTF;
                } else if(top >= 950 && front < MOV_THRESH 
                          && bottom > 700) {
                    state = FORWARD;
                } else {
                    state = HALTF;
                }
                break;
    case FORWARD: if(front < MOV_THRESH && bottom > 700) {
                    state = FORWARD;
                  } else {
                    state = HALTB;
                  }
                  break;
    case HALTB: if(top < 950) {
                    state = HALTB;
                } else if(top >= 950 && back < MOV_THRESH
                          && bottom > 700) {
                    state = BACK;
                } else {
                    state = HALTB;
                }
                break;
    case BACK: if(back < MOV_THRESH & bottom > 700) {
                    state = BACK;
                } else {
                    state = HALTF;
                }
                break;
  }           
  printf("State %d\n", state); 

  fclose(adc0); 
  fclose(adc2);
  fclose(adc4);
  fclose(adc6);
  //sigprocmask(SIG_UNBLOCK, &mask, NULL); // Unblock signals
}
