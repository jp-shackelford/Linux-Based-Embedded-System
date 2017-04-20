#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "LCD_init.c"
int main() {
    // initialize LCD to home with set cursor
    init(); 
    // clears display sets to first position

    /* Grab string to write to LCD */ 
    int fd; // fifo refrence 
    char * myfifo = "/tmp/myfifo";
    char s[20]; 
    mkfifo(myfifo, 0666); // create the FIFO (named pipe), 0666 specifices permissions 
    fd = open(myfifo, O_RDONLY);
    read(fd, s, sizeof(s));
    close(fd);
    /* remove the FIFO */
    unlink(myfifo);

}