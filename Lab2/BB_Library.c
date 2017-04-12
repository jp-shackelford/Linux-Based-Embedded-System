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
#define rsValp "/sys/class/gpio48/value"
#define rValp "/sys/class/gpio49/value"
#define eValp "/sys/class/gpio117/value"
#define db0Valp "/sys/class/gpio115/value"
#define db1Valp "/sys/class/gpio20/value"
#define db2Valp "/sys/class/gpio60/value"
#define db3Valp "/sys/class/gpio112/value"
#define db4Valp "/sys/class/gpio66/value"
#define db5Valp "/sys/class/gpio69/value"
#define db6Valp "/sys/class/gpio45/value"
#define db7Valp "/sys/class/gpio47/value"

  FILE *a[10];
  FILE *eVal = getStream(eValp, "w"); 
  int c = 1;   
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

  void init_command() {
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
	c = 0; 
  }

  // command function for LCD. 
  int command(int b[10]) {
	if(c) {
		printf("%s", "Need to Initialize Command First"); 
		return 0; 	
	}
	// Write in values for each pin
	int i;
	for(i = 0; i < 11; i++) {
		writeToStream(a[i], "%d", b[i]);	
	}

	// Strobe the enable
	eVal = 1;
	usleep(10);
	eVal = 0;

	return 1;
 }
