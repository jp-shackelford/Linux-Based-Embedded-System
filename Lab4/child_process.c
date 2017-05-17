#include  <stdio.h>
#include  <sys/types.h>
#include  <signal.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <time.h>

void handler(int sig) {
	sigset_t mask;
	sigfillset(&mask);
	sigprocmask(SIG_BLOCK, &mask, NULL); // Block all signals so we process one at a time
	
	if (sig == SIGUSR1) {
		printf("Got SIGUSR1\n");
	} else if (sig == SIGUSR2) {
		printf("Got SIGUSR2\n");
	}	
	//usleep(100000);
	sigprocmask(SIG_UNBLOCK, &mask, NULL); // Unblock all signals
}


int main() {
	pid_t pid = getpid();
	key_t MyKey;
	
	if (signal(SIGUSR1, handler) == -1) {
		printf("SIGUSR1 install error\n");
		raise(SIGKILL);
	}
	if (signal(SIGUSR2, handler) == -1) {
		printf("SIGUSR2 install error\n");
		raise(SIGKILL);
	}
	

	MyKey = ftok(".", 's');
	int ShmID   = shmget(MyKey, sizeof(pid_t), IPC_CREAT | 0666);
    pid_t * ShmPTR  = (pid_t *) shmat(ShmID, NULL, 0);
    *ShmPTR = pid;

    while(1) {
    	printf("From process %d\n", pid);
    	usleep(1000000);
    	// Run motors here
    }
    return 0;
}
