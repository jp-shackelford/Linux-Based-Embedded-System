/* EE474, ECKER, SPRING17
 * BB Library
 * Contains several functions which eliminate some redunant
 * code commonly type during EE 474 beagle bone labs
 * Declaration notes: #include this file after calling stndio and 
 * any other librarys. 
 */ 


  // Get Stream. Takes a file path, and mode type as parameters
  // will wait for file to be initialized due to BB's timing delays
  // Note: if the path passed in doesn't or will never exist, getStream
  // will loop indefinetly 
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

  FILE *a[10];
  FILE *eVal; 
  int flag = 1; 
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

  void init_lcd() {
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

  // command function for LCD.
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
	usleep(60);
	return 1;
 }

void set_cursor(int p) {
	return 0; 
}
