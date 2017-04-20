#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "BB_Library.c"
int main() {
    // initialize LCD to home with set cursor
    init(); 
    writeString("Guess a # 1-10\n"); 
    int ans = rand() % 10; 
    int usr_g; 
    int c = 3; 
    printf("%d\n", ans); 
    while((usr_g != ans) || (c == 0)) {
       scanf("%d", &usr_g);
       if(usr_g == ans) {
        writeString("You Win!\n");
        usleep(5000000);
       } else {
        writeString("Guess Again\n");
       }
   }
   clear_and_home(); 
}