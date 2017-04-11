/* 
474library.c
UW EE/CSE 474 SPRING 2017 - ECKER
Michael W, Joshua S, Alex F
This library contains several functions to
make code more concise and clear
*/

#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep


// Takes in same parameters as fprintf, and will flush
// the values immediately afterwords 
void writeToFile(FILE *location, char *type, char *value) {
	fprintf(location, type, value);
	fflush(location); 
}

// takes in location and r/w as a parameter
// not exit until such a location exists
FILE* waitForFileInit(char *location, char *rw, int waitTime) {
	while(!fopen(location, rw)) {
		usleep(waitTime);
	}
	return fopen(location, rw);
}