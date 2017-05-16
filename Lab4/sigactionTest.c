#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#define CLOCKID CLOCK_REALTIME
#define SIG SIGALRM

void handler(int sig) {
	if (sig==SIG) {
		printf("hi\n");	
	}
}

int main() {
	timer_t timerid;
	struct sigevent sev; // Our signal event
	struct itimerspec its; // Our interval timer
	long long freq_nanosecs;
	sigset_t mask;
	
	// This is where the signal handler is established
	struct sigaction sa; // signal action 	
	sa.sa_flags = SA_SIGINFO;	
	sa.sa_sigaction = handler; // What function should I use to handle this signal
	sigemptyset(&sa.sa_mask); // Set of signals to block (this is an empty set currently)
	sigaction(SIG, &sa, NULL);

	// Block the signal here
	sigemptyset(&mask); // Clear the set of signals to mask
	sigaddset(&mask, SIG); // Add SIGALRM to the set of signals to block
	sigprocmask(SIG_SETMASK, &mask, NULL);


	// Here is where the timer's properties are defined and the timer is created
	sev.sigev_notify = SIGEV_SIGNAL; // Send sigev_signo when timer expires
	sev.sigev_signo = SIG; // What signal will you use to notify me when this happens?
	sev.sigev_value.sival_ptr = &timerid; // What timer am I paying attention to?
	
	timer_create(CLOCKID, &sev, &timerid);
	printf("Timer ID is 0x%lx\n", (long)timerid);
	
	configtime(10000000, its);

	freq_nanosecs = 1000000; // .1 seconds
    its.it_value.tv_sec = freq_nanosecs / 1000000000;
    its.it_value.tv_nsec = freq_nanosecs % 1000000000;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

	timer_settime(timerid, 0, &its, NULL);

	usleep(100000); // Wait for timer to expire and then unmask the signal
	
	// Unblock signal so that our handler will catch it
	sigprocmask(SIG_UNBLOCK, &mask, NULL);


	timer_settime(timerid, 0, &its, NULL);
	usleep(100000);
	

}


