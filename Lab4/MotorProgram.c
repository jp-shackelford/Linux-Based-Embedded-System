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
FILE* adc; 
int command = 50; 
Motor * m0;
Motor * m1;

int main() {
    printf("initializing folders...\n");
    FILE* slots = getStream("/sys/devices/bone_capemgr.9/slots", "w");
    writeToStream(slots, "%s", "cape-bone-iio");
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

void timer_handler(int whatever) {
  /*send user defined signal (SIG_USER) to the process with PID
    kill() doesn't neccessarily mean to terminate another process,
    and in this case, it means to send signal "SIG_USER" to the
    process with "pid"
  */
  // read value from adc 
  adc = fopen("/sys/devices/ocp.3/helper.17/AIN0", "r");
  char value[1024]; 
  fgets(value, 1024, adc);
  int adc_mvolt = atoi(value);
  printf("adc_mvolt: %d\n", adc_mvolt); 
  if(adc_mvolt >= 900) {
        command = BRAKE; 
  } else {
        command = FORWARD;
  }
  close(adc); 
}
