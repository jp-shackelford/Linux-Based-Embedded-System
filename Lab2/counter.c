/* LCD.C	
 * 
 * 
 * 
 *
 *	
 */

#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep

#define RS  48
#define RW  49
#define E   117
#define DB0 115
#define DB1 20
#define DB2 60
#define DB3 112
#define DB4 66
#define DB5 69
#define DB6 45
#define DB7 47

int main() {

	FILE *sys, *dir, *val;
	// Array of values for each GPIO used 
	int[] GPIO = {RS, RW, E, DBO, DB1, DB2, DB3, DB4, DB5, DB6, DB7};
	
	// Specifies BB's file to initialize GPIO folders 
	sys = fopen("/sys/class/gpio/export", "w");
	fseek(sys, 0, SEEK_SET);
	
	// Writing to export to generate GPIO folders for each
	// GPIO used
	int i; 
	for(i = 0; i < 11; i++) {
		fprintf(sys, "%d", GPIO[i]);
		fflush(sys);  	
	}

	// creating pointer for each GPIO file
	int j;
	for(j  = 0; j < 11; j++) {
		while(!fopen("/sys/class/gpio" 	
	}
	// prints value inside quotes into given file.
	// %d means it will print some variable as a decimal.
	// GPIO_PIN is the variable refered to by %d.
	fprintf(sys, "%d", GPIO_PIN_0);
	fflush(sys);
	fprintf(sys, "%d", GPIO_PIN_1);
	fflush(sys);
	fprintf(sys, "%d", GPIO_PIN_2); 
	// Clears the FILE stream for sys file object.
	fflush(sys);

	//Set the gpio to output
	dir = fopen("/sys/class/gpio/gpio45/direction", "w");
	fseek(dir, 0, SEEK_SET);
	fprintf(dir, "%s", "out");
	fflush(dir);
	
	FILE *dir1, *dir2, *val1, *val2; 
	
	dir1 = fopen("/sys/class/gpio/gpio69/direction", "w");
	fseek(dir1, 0, SEEK_SET);
	fprintf(dir1, "%s", "out");
	fflush(dir1);
	
	dir2 = fopen("/sys/class/gpio/gpio66/direction", "w");
	fseek(dir2, 0, SEEK_SET);
	fprintf(dir2, "%s", "out");
	fflush(dir2);
	

	//Opens the file that controls if the pin is high or low
	val = fopen("/sys/class/gpio/gpio45/value", "w");
	fseek(val, 0, SEEK_SET);	
	val1 = fopen("/sys/class/gpio/gpio69/value", "w");
	fseek(val1, 0, SEEK_SET);
	val2 = fopen("/sys/class/gpio/gpio66/value", "w");
	fseek(val2, 0, SEEK_SET); 
	
	int count = 0; 

	//Blinks the LED
	while(1) {
		// if odd toggle smallest bit 
		if((count % 2) == 1) {
			fprintf(val, "%d", 1);
		} else {
			fprintf(val, "%d", 0);
		} 
		if((count == 2) || (count == 3) || (count == 6) || (count == 7)) {
			fprintf(val1, "%d", 1);
		} else {
			fprintf(val1, "%d", 0);
		}		
		if(count >= 4) {
			fprintf(val2, "%d", 1); 
		} else {
			fprintf(val2, "%d", 0);
		} 
		fflush(val);
		fflush(val1);
		fflush(val2);
      		usleep(200000);
		if(7 == count) {
			count = 0;
		} else {
			count++;
		}
	}

   // close all files
	fclose(sys);
	fclose(dir);
	fclose(dir1);
	fclose(dir2); 
	fclose(val);
	fclose(val1);
	fclose(val2);
	return 0;
}
