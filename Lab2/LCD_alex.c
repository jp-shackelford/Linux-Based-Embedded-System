// LCD.C	

#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep
#include "LCD.h"


FILE *sys, *dir; // Files for pin access and direction
// Create global files for each pin used on the LCD display
FILE *rsVal, *rwVal, *enVal, *db7Val, *db6Val, *db5Val, 
	 *db4Val, *db3Val, *db2Val, *db1Val, *db0Val;

int main() {
	
	init(); // Initialize LCD display


   // close all files
	fclose(sys);
	fclose(dir);
	fclose(rsVal);
	fclose(rwVal); 
	fclose(db7Val);
	fclose(db6Val);
	fclose(db5Val);
	fclose(db4Val);
	fclose(db3Val);
	fclose(db2Val);
	fclose(db1Val);
	fclose(db0Val);
	
	return 0;
}



int init() {

	// Array of each GPIO pin number
	int[11] GPIO = {RS, RW, E, DBO, DB1, DB2, DB3, DB4, DB5, DB6, DB7};

	sys = fopen("/sys/class/gpio/export", "w");
	fseek(sys, 0, SEEK_SET);
	
	
	int i; 
	char dirStr[36], valStr[36]; 

	// Generate folders for each GPIO pin being used and set all directions to out
	for(i = 0; i < 11; i++) {
		
		fprintf(sys, "%d", GPIO[i]);
		fflush(sys);  	

		sprintf(dirStr,"%s%d%s","/sys/class/gpio/gpio",GPIO[i],"/direction");
		sprintf(valStr,"%s%d%s","/sys/class/gpio/gpio",GPIO[i],"/value");

		dir = fopen(dirStr, "w");
		while(!dir) {} // Wait in loop until dir gets initialized
		fseek(dir, 0, SEEK_SET);
		fprintf(dir, "%s", "out");
		fflush(dir);

	}

	// Files for putting values onto the GPIO pins
	rsVal = fopen(RS_VALUE, "w");
	fseek(rsVal, 0, SEEK_SET);
	rwVal = fopen(RW_VALUE, "w");
	fseek(rwVal, 0, SEEK_SET);
	enVal = fopen(E_VALUE, "w");
	fseek(enVal, 0, SEEK_SET);
	db7Val = fopen(DB7_VALUE, "w");
	fseek(db7Val, 0, SEEK_SET);
	db6Val = fopen(DB6_VALUE, "w");
	fseek(db6Val, 0, SEEK_SET);
	db5Val = fopen(DB5_VALUE, "w");
	fseek(db5Val, 0, SEEK_SET);
	db4Val = fopen(DB4_VALUE, "w");
	fseek(db4Val, 0, SEEK_SET);
	db3Val = fopen(DB3_VALUE, "w");
	fseek(db3Val, 0, SEEK_SET);
	db2Val = fopen(DB2_VALUE, "w");
	fseek(db2Val, 0, SEEK_SET);
	db1Val = fopen(DB1_VALUE, "w");
	fseek(db1Val, 0, SEEK_SET);
	db0Val = fopen(DB0_VALUE, "w");
	fseek(db0Val, 0, SEEK_SET);

	// fprintf(val, "%d", 1);
	// fflush(val)

	/* INITIALIZE LCD DISPLAY */
	usleep(30000); // Wait >15ms
	command(0,0,0,0,1,1,0,0,0,0);
	usleep(5000); // Wait >4.1ms
	command(0,0,0,0,1,1,0,0,0,0);
	usleep(150); // Wait > 100us
	command(0,0,0,0,1,1,0,0,0,0);
	command(0,0,0,0,1,1,N,F,0,0);
	command(0,0,0,0,0,0,1,0,0,0);
	command(0,0,0,0,0,0,0,0,0,1);
	command(0,0,0,0,0,0,0,1,ID,S);
	command(0,0,0,0,0,0,1,1,C,B);

	return 0;
}

void command(int rs,int rw,int d7,int d6,int d5,int d4,int d3,int d2,int d1,int d0) {
	
	// Write in values for each pin
	fprintf(rsVal, "%d", rs);
	fflush(rsVal);
	fprintf(rwVal, "%d", rw);
	fflush(rwVal);
	fprintf(db7Val, "%d", d7);
	fflush(db7Val);	
	fprintf(db6Val, "%d", d6);
	fflush(db6Val);
	fprintf(db5Val, "%d", d5);
	fflush(db5Val);
	fprintf(db4Val, "%d", d4);
	fflush(db4Val);
	fprintf(db3Val, "%d", d3);
	fflush(db3Val);
	fprintf(db2Val, "%d", d2);
	fflush(db2Val);
	fprintf(db1Val, "%d", d1);
	fflush(db1Val);
	fprintf(db0Val, "%d", d0);
	fflush(db0Val);

	// Strobe the enable
	enVal = 1;
	usleep(10);
	enVal = 0;
}


