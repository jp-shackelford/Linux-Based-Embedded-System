/* EE 474 - ECKER SPRING 2017
   Guessing Game for LCD 
   Plays a simple guessing game with LCD
   will prompt user for input from terminal and repeatedly
   ask for guesses until correct answer is recieved
*/
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
    // write to LCD
    writeString("Guess a # 1-10\n"); 
    // generate correct answer
    int ans = rand() % 10; 
    // declare user guess
    int usr_g; 
    int c = 3; 
    // print answer to terminal for debugging purpose 
    printf("%d\n", ans); 
    // loop game logic 
    while(c > 0) {
       scanf("%d", &usr_g);
       if(usr_g == ans) {
        writeString("You Win!\n");
        usleep(5000000); 
        break; 
       } else {
        writeString("Guess Again\n");
        c--; 
       }
   }
   if(c == 0) {
        writeString("You LOSE\n"); 
        usleep(5000000);
   }
   // reset LCD to clear and home on exit 
   clear_and_home(); 
}