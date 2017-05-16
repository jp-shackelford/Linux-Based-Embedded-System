#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
	switch(sig) {
	case SIGINT :
		printf("bye!\n");
		raise(SIGKILL);
		break;
	case SIGALRM :
		alarm(1);
		printf("tic\n");
		break;
	case SIGUSR1 :
		printf("got some signal\n");
		break;
	}
}


int main(void) {
    signal(SIGALRM, handler); // We will use sigaction() instead of signal()
	signal(SIGINT, handler);
	signal(SIGILL, handler);
	signal(SIGUSR1, handler);
    //ualarm(100000, 100000);
	alarm(1);
    int n = 0;
    while (1) {
		char c = 'c';
		if (c == 'c') {

			raise(SIGUSR1);
			sleep(10);
		} else {
			raise(SIGQUIT);
		}
	}
    return 0;
}
