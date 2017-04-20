#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "BB_Library.c"
int main() {
    // initialize LCD to home with set cursor
    init(); 
    // clears display sets to first position

    /* Grab string to write to LCD */ 
   while(1) {
        int fd; // fifo refrence 
        char * myfifo = "/tmp/myfifo";
        char s[17]; 
        mkfifo(myfifo, 0666); // create the FIFO (named pipe), 0666 specifices permissions 
        fd = open(myfifo, O_RDONLY);
        read(fd, s, sizeof(s[0]) * 17);
        close(fd);
        printf("%s", s); 
        /* remove the FIFO */
        unlink(myfifo);
        clear_and_home();
        int i;
        for (i = 0; i < 17; i++){
            if(s[i] == '\n') {
                break; 
            } else {
                writechar(s[i]);
            } 
        }   
   }
}