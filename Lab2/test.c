#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep
#include "LCD.h"
int main() {
	const char* paths[] = {RS_PATH, RW_PATH};
	printf("%s\n", paths[1]);
	printf("%s\n", "test 1 2?"); 
	return 0; 
}
