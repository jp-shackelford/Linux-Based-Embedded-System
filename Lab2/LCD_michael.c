// LCD.C	

#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep
#include "LCD.h"
#include "BB_Library.c" 

FILE *sys, *dir; // Files for pin access and direction
// Create global files for each pin used on the LCD display
FILE *rsVal, *rwVal, *enVal, *db7Val, *db6Val, *db5Val, 
	 *db4Val, *db3Val, *db2Val, *db1Val, *db0Val;

int init() {

	// Array of each GPIO pin number
	int GPIO[11] = {RS, RW, E, DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7};
	sys = getStream("/sys/class/gpio/export", "w"); // check BB_Library for implementation 
	fseek(sys, 0, SEEK_SET);
	int i; 
	char dirStr[36], valStr[36]; 
	// Generate folders for each GPIO pin being used and set all directions to out
	for(i = 0; i < 11; i++) {
		
		writeToStream(sys, "%d", GPIO[i]);  	

		sprintf(dirStr,"%s%d%s","/sys/class/gpio/gpio",GPIO[i],"/direction");

		dir = getStream(dirStr, "w");
		while(!dir) {} // Wait in loop until dir gets initialized
		fseek(dir, 0, SEEK_SET);
		fprintf(dir, "%s", "out");
		fflush(dir);

	}

	/* INITIALIZE LCD DISPLAY */
	init_command(); 
	usleep(30000); // Wait >15ms
	command(0,0,0,0,1,1,0,0,0,0);
	usleep(5000); // Wait >4.1ms
	command(0,0,0,0,1,1,0,0,0,0);
	usleep(150); // Wait > 100us
	command(0,0,0,0,1,1,0,0,0,0);
	command(0,0,0,0,1,1,0,0,0,0); // 1-line display, 5x7 font
	command(0,0,0,0,0,0,1,0,0,0);
	command(0,0,0,0,0,0,0,0,0,1);
	command(0,0,0,0,0,0,0,1,1,0); // Increment mode, no shift
	command(0,0,0,0,0,0,1,1,1,0); // Cursor on, blink off
	return 0;
}


int main() {
	
	init(); // Initialize LCD display
   	// close all open streams 
	usleep(1000000000); 
	return 0;
}







