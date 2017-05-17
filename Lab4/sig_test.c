#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sighandler(int);

int main()
{
   int count = 0;

   //signal(SIGINT, sighandler);

   while(1) 
   {
      printf("Going to sleep for a second...\n");
      count++;
	   if(count == 5) {
	   	raise(SIGINT);
	   }      
      sleep(1); 
   }
   return(0);
}

void sighandler(int signum)
{
   printf("Caught signal %d, coming out...\n", signum);
   exit(1);
}