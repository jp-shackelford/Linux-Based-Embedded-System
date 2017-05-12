#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <unistd.h>
#include <fcntl.h>  
#include <time.h>
#include <signal.h>

#define SAMPLE_SIZE 10
#define SAMPLE_TIME 1000 // 1000µs = 1ms; 10 samples = .01s
#define THRESHOLD 1000
#define CLOCKID CLOCK_REALTIME // Settable, realtime system clock
#define SIG SIGRTMIN


static void
       handler(int sig, siginfo_t *si, void *uc)
       {
           /* Note: calling printf() from a signal handler is not
              strictly correct, since printf() is not async-signal-safe;
              see signal(7) */

           printf("Caught signal %d\n", sig);
           //print_siginfo(si);
           signal(sig, SIG_IGN);
       }
       
       
       
int main() {
	timer_t timerid;
   	struct sigevent sev;
    struct itimerspec its;
    sigset_t mask;
    struct sigaction sa;

    //system("echo BB-ADC > /sys/devices/bone_capemgr.9/slots");

    /* Establish handler for timer signal */
    printf("Establishing handler for signal %d\n", SIG);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIG, &sa, NULL) == -1) {
        printf("sigaction");
        exit(1);
    }

    /* Create the timer */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCKID, &sev, &timerid) == -1) {
        printf("timer_create");
        exit(1);
    }
    printf("timer ID is 0x%lx\n", (long) timerid);

    /* Start the timer */
    its.it_value.tv_sec = 1000000000;
    its.it_value.tv_nsec = 10000000;
   	printf("%d\n", its.it_value.tv_nsec);
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;
    
	// Collect samples
		int i;
        for (i=0; i<SAMPLE_SIZE; i++) {
			timer_settime(timerid, 0, &its, NULL); // Reset timer to 0
			printf("%d\n", &its);
           	printf("%d\n", timer_gettime(timerid, &its));
            while (timer_gettime(timerid, its.it_value.tv_nsec) > 0) {
            	printf("%d\n", timer_gettime(timerid, &its));
            } // While timer hasn't expired
       	}
    return 0;
}

