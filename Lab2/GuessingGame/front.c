#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "BB_Library.c"
int main() {
    char s[1024]; 
    while(1) {
    	printf("enter 16 characters to get printed:\n"); 
    	//Grab 16 characters from user 
    	fgets(s, sizeof(s)/sizeof(s[0]), stdin);
    	int fd;
        char * myfifo = "/tmp/myfifo";
        /* create the FIFO (named pipe) */
        mkfifo(myfifo, 0666);
        /* write "Hi" to the FIFO */
        fd = open(myfifo, O_WRONLY); 
        write(fd, s, sizeof(s[0]) * 1024);
        close(fd);
        /* remove the FIFO */
        unlink(myfifo);
    }
    init_command(); 
    command(0,0,0,0,0,0,0,0,1,0);
    return 0;
}