#include <stdio.h>
#include "BB_Library.c"

void main () { 
	FILE* thing = getStream("./testfile.txt", "w");  
	//writeToStream(thing, "%s", "test");
	writeToStream(thing, "%s", "why"); 
} 