/* EE474, ECKER, SPRING17
 * BB Library
 * Contains several functions which eliminate some redunant
 * code commonly type during EE 474 beagle bone labs
 * Declaration notes: #include this file after calling stndio and 
 * any other librarys. 
 */ 
#define rsValp "/sys/class/gpio/gpio48/value"
#define rValp "/sys/class/gpio/gpio49/value"
#define eValp "/sys/class/gpio/gpio27/value"
#define db0Valp "/sys/class/gpio/gpio115/value"
#define db1Valp "/sys/class/gpio/gpio20/value"
#define db2Valp "/sys/class/gpio/gpio60/value"
#define db3Valp "/sys/class/gpio/gpio112/value"
#define db4Valp "/sys/class/gpio/gpio66/value"
#define db5Valp "/sys/class/gpio/gpio69/value"
#define db6Valp "/sys/class/gpio/gpio45/value"
#define db7Valp "/sys/class/gpio/gpio47/value"
#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/stat.h>
#include <unistd.h>
#include "LCD.h"

  FILE *a[10];
  FILE *eVal; 
  int flag = 1; 


  // Get Stream. Takes a file path, and mode type as parameters
  // will wait for file to be initialized due to BB's timing delays
  // Note: if the path passed in doesn't or will never exist, getStream
  // will loop indefinetly 
  FILE* getStream(char *path, char *mode) {
	while(!fopen(path, mode)) {
		usleep(1); 	
	}
	return fopen(path, mode);
  } 
  
  // combines fprintf and flush into one function
  // call this to clean up the code a bit 
  void writeToStream(FILE* stream, char *type, char *value) {
  		fprintf(stream, type, value);
		fflush(stream); 
  }

  // helper function called by init, do not call in user cose 
  static void init_lcd() {
  	a[0] = getStream(rsValp, "w");
	a[1] = getStream(rValp, "w");
	a[2] = getStream(db0Valp, "w");
	a[3] = getStream(db1Valp, "w");
	a[4] = getStream(db2Valp, "w");
	a[5] = getStream(db3Valp, "w");
	a[6] = getStream(db4Valp, "w");
	a[7] = getStream(db5Valp, "w");
	a[8] = getStream(db6Valp, "w");
	a[9] = getStream(db7Valp, "w");
	eVal= getStream(eValp, "w");
	writeToStream(eVal, "%d", 0); 
	flag = 0; 
	usleep(30000); // Wait >15ms
	/* int command(int rs, int r, int db0, int db1, int db2, int db3,
  			  int db4, int db5, int db6, int db7) */ 
	// START BOOT UP COMMANDS
	command(0,0,0,0,1,1,0,0,0,0);
	usleep(5000); // Wait >4.1ms
	command(0,0,0,0,1,1,0,0,0,0);
	usleep(5000); // Wait > 100us
	command(0,0,0,0,1,1,0,0,0,0);
	usleep(5000); 
	// END BOOT UP COMMANDS
	command(0,0,0,0,1,1,1,0,0,0); // set 8-bit/2line

  }

  // command function for LCD, writes to individual GPIOS 
  int command(int rs, int r, int db7, int db6, int db5, int db4,
  			  int db3, int db2, int db1, int db0) {
	if(flag) {
		printf("%s", "Need to Initialize Command First"); 
		return 0; 	
	}
	int b[10] = {rs, r, db0, db1, db2, db3, db4, db5, db6, db7}; 
	int i;
	for(i = 0; i < 11; i++) {
		writeToStream(a[i], "%d", b[i]);	
	}
	// Strobe the enable
	writeToStream(eVal, "%d", 1);
	usleep(100); // wait 300 >= ns, 10 us is much greater than 300 so delay is met
	writeToStream(eVal, "%d", 0);
	// wait 60 us for command to settle 
	usleep(1000);
	return 1;
 }

int set_cursor(int p) {
	return 0; 
}

// initialize LCD to default 16x2 grid
// With cursor and blink off 
int init() {
	FILE *sys, *dir; // Files for pin access and direction
	// Create global files for each pin used on the LCD display
	FILE *rsVal, *rwVal, *enVal, *db7Val, *db6Val, *db5Val, 
	 *db4Val, *db3Val, *db2Val, *db1Val, *db0Val;
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
	init_lcd(); 
	command(0,0,0,0,0,0,0,0,0,1); // clear 
	command(0,0,0,0,0,0,1,1,0,0); // Turn on display, cursor, and blink
	command(0,0,0,0,0,0,0,1,1,0); // set mode to increment 
}

// helper function for toBits
static unsigned int_to_bin (unsigned k) {
	return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_bin(k / 2)));
}

// parses a charter to its ascii equivalent, returns
// and array for each bit of charactrs ascii 
int* toBits(char key) {
        unsigned k;
        int bit;
        char exit_key = 'q';
		static int a[8];	
        k = int_to_bin((unsigned)key); // get key's ascii code
       //Parses the key's ascii code, diplaying one bit at a time
	//while( k != NULL && key != exit_key) {
	int i;
	for(i = 0; i < 8; i++) {
		bit = (k % 2);
                k = k / 10;
		a[i] = bit;
	}
	return a;
}

// writes a character to current cursor location
void writechar(char c) {
	// pointer to beginning of ascii array
	int * p;
	p = toBits((char) c);
	int i;
	command(1,0,p[7],p[6],p[5],p[4],p[3],p[2],p[1],p[0]); 
}

// clears screen sets cursor to top let
void clear_and_home() {
	command(0,0,0,0,0,0,0,0,0,1);
	usleep(16000);
} 

void writeString(char* s) {
	clear_and_home(); 
	int i = 0;
	while(s[i] != '\n') {
		writechar(s[i]);
		i++; 
	}
}
