#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "LCD_init.c"
#include "toBits.c"
int main() {
    // initialize LCD to home with set cursor
    init(); 
    // clears display sets to first position

    /* Grab string to write to LCD */ 
    while(1) {
        int fd; // fifo refrence 
        char * myfifo = "/tmp/myfifo";
        char s[16]; 
        mkfifo(myfifo, 0666); // create the FIFO (named pipe), 0666 specifices permissions 
        fd = open(myfifo, O_RDONLY);
        read(fd, s, sizeof(s));
        close(fd);
        /* remove the FIFO */
        unlink(myfifo);

        int i;
        int * p; 
        for(i = 0; i < 16; i++) {
            p = toBits(s[i]);
            command(1,0,p[7],p[6],p[5],p[4],p[3],p[2],p[1],p[0]);
        }
        // reset cursor to home position
        command(0,0,0,0,0,0,0,0,1,0);  
    }
}