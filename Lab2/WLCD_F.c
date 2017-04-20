#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main() {
	printf("enter 16 characters to get printed:\n"); 
	//Grab 16 characters from user 
	char s[16];
	scanf("%s", s);
	int fd;
    char * myfifo = "/tmp/myfifo";
    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);
    /* write "Hi" to the FIFO */
    fd = open(myfifo, O_WRONLY);
    write(fd, s, sizeof(s));
    close(fd);
    /* remove the FIFO */
    unlink(myfifo);
    return 0;
}