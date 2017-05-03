#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep
#include <errno.h>
#include <stddef.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/stat.h>
#include <unistd.h>

void getInput (char *prompt, char *inputBuffer, int buffersize) {
	
	printf("%s : ", prompt);
	fgets(inputBuffer, buffersize, stdin);

	int i;
	for(i = 0; i < strlen(inputBuffer); i++) {
		if( inputBuffer[i] == '0' ||
			inputBuffer[i] == '1' ||
			inputBuffer[i] == '2' ||
			inputBuffer[i] == '3' ||
			inputBuffer[i] == '4' ||
			inputBuffer[i] == '5' ||
			inputBuffer[i] == '6') {
			//Let the input pass.
		} else if (inputBuffer[i] == '\n') {
			printf("Error! Your string was too short. Please enter 16 characters\n");
			getInput(prompt, inputBuffer, buffersize);
			break;
		} else {
			printf("Error! Only enter numbers between 0 and 6. Try again\n");
			getInput(prompt, inputBuffer, buffersize);
			break;
		}
	}	

	if(inputBuffer[strlen(inputBuffer)-1] != '\n') {
		int overflow = 0;

		while (fgetc(stdin) != '\n') {
			overflow++;
		}		

		if(overflow > 0) {
			printf("Error! You had %d too many characters. Try again, only input 16.\n", overflow);
			getInput(prompt, inputBuffer, buffersize);
		} else {
			inputBuffer[buffersize] = '\0';
		}
	}
}

int main() {
	static char inputBuffer[17];
	getInput("Please Enter 16 numbers between 0 and 6", inputBuffer, 17);
	printf("You entered: %s\n", inputBuffer);
}