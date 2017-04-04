/*	blinky.c
 * Adrian Prananda Lincong Li
 * 30 / Sept / 2015
 *
 *	Blinks and external LED from gpio0[8] = x70 pin 27
 */

#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep

#define GPIO_PIN_0 45      // LED pins
#define GPIO_PIN_1 69
#define GPIO_PIN_2 66

int main() {

	// Creates the File pointers to create the gpio file system,
	// set the direction of the GPIO, and change the data stored there.
	FILE *sys, *dir, *val;

	// Specifies the file that the pointer will be used for (w = //write)
	sys = fopen("/sys/class/gpio/export", "w");
   
   // int fseek(FILE *stream, long int offset, int whence)
   // 	sets the file position of the stream to the given offset.
   //		Whence has the following constants:
   //			SEEK_SET = Beginning of file
   //			SEEK_CUR = Current position of the file pointer
   //			SEEK_END = End of file
   //
   //		In this instance fseek is not necessarily needed.
	fseek(sys, 0, SEEK_SET);
	
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
