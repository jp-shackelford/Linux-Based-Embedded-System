/*	blinky.c
 * Adrian Prananda Lincong Li
 * 30 / Sept / 2015
 *
 *	Blinks and external LED from gpio0[8] = x70 pin 27
 */

#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep

#define GPIO_PIN 45      // LED pin

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
	fprintf(sys, "%d", GPIO_PIN);
	
	// Clears the FILE stream for sys file object.
	fflush(sys);

	//Set the gpio to output
	dir = fopen("/sys/class/gpio/gpio45/direction", "w");
	fseek(dir, 0, SEEK_SET);
	fprintf(dir, "%s", "out");
	fflush(dir);

	//Opens the file that controls if the pin is high or low
	val = fopen("/sys/class/gpio/gpio45/value", "w");
	fseek(val, 0, SEEK_SET);

	//Blinks the LED
	while(1) {
		fprintf(val, "%d", 1);
		fflush(val);
      // delays for given length of time in microseconds.
      usleep(50000);		
      fprintf(val, "%d", 0);
		fflush(val);
      usleep(50000);
	}

   // close all files
	fclose(sys);
	fclose(dir);
	fclose(val);
	return 0;
}
