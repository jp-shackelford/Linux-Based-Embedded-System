#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep
#include <errno.h>
#include <stddef.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/stat.h>
#include <unistd.h>
#include "lcd_library.c" 
#include "step_sequencer.h"
// takes prompt, inputbufer and buffersize as parameters
// prompt will display on terminal shell prompting for user input
// inputbufer will be string getting written to
// bufersize is the amount getting written to inputbuffer

int main() {
	init_lcd_library(); 
	init_pwm();
	clear_and_home();
	static char inputBuffer[18];

	
	//getInput("Please Enter 16 numbers between 0 and 6", inputBuffer, 17);
	//printf("You entered: %s\n", inputBuffer);
	/*	
	fprintf(musPeriod, "%d", 2309480);
	fflush(musPeriod);
	fprintf(musEnable, "%d", 1);
	fflush(musEnable); 
	usleep(1000000);
	fprintf(musEnable, "%d", 0);
	fflush(musEnable); 
	*/
	
	int state = 0;
	while (state != 10) {
		// user input state
		if(state == 0) {
			cursorState(0); 	
			home(); 
			writeString("Input|q|w|0-6\n");	
			//getInput("", inputBuffer, 17);
			//printf("%s", inputBuffer);
			//writeString(inputBuffer);		
			fgets(inputBuffer,18,stdin);
			printf("got : %s\n", inputBuffer);
			cursorState(0); 
			strcat(inputBuffer, "\n");
			if(inputBuffer[0] == 'q' || inputBuffer[0] == 'Q') {
				state = 10; 		
			} else if(inputBuffer[0] == 'w' || inputBuffer[0] == 'W') {
				state = 3;
			} else {
				state = 2; 			
			}
		// playback state
		} else if(state == 2) {
			home(); 
			writeString(inputBuffer);
			cursorState(1); // sets to blink
			printf("%s", inputBuffer);
			int i;
			for(i=0; i<16; i++) { // Know that we have exactly 16 notes
				cursorState(1);
				switch(inputBuffer[i]) {
					case '0': play(0);
						break;
					case '1': play(C);
						break;
					case '2': play(D);
						break;
					case '3': play(E);
						break;
					case '4': play(F);
						break;
					case '5': play(G);
						break;
					case '6': play(A); 
						break;
				}
				shiftRight();
			}
			state = 0; 
		// edit stage 		
		} else if(state == 3) {
			strcpy(inputBuffer, "0000000000000000\n");
			setCursor(0);
			writeString(inputBuffer);
			setCursor(0);
			int current = 0;
		   cursorState(3); // on and blinking 
			while(state == 3) {
				if(current == 16) {
					current = 0;
					home(); 
				}
				char writeIn[3];
				fgets(writeIn,3,stdin);
				if(writeIn[0] == 'r' && current != 15) {
					shiftRight();
					current++;				
				} else if(writeIn[0] == 'l' && current != 0) {
					shiftLeft();
					current--; 				
				} else if(  writeIn[0] == '0' ||
								writeIn[0] == '1' ||
								writeIn[0] == '2' ||
								writeIn[0] == '3' ||
								writeIn[0] == '4' ||
								writeIn[0] == '5' ||
								writeIn[0] == '6') {
					inputBuffer[current] = writeIn[0]; 
					writechar(inputBuffer[current]);	
					current++; 			
				} else if(writeIn[0] == 'i') {
					state = 0;				
				} else if(writeIn[0] == 'q') {
					state = 10;				
				} else if(writeIn[0] == 'p') {
					state = 2;				
				} else {
					state = 0; 				
				}
			}
		} 	
		fprintf(musEnable, "%d", 0); 
		fflush(musEnable);
		usleep(200000);
	}
	clear_and_home();
	cursorState(0);	
}
 
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
			//return -1;
		} else {
			printf("Error! Only enter numbers between 0 and 6. Try again\n");
			getInput(prompt, inputBuffer, buffersize);
			//return -1;
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
			//return -1;
		} else {
			inputBuffer[buffersize] = '\0';
			//printf("success\n");
			//return 0;
		}
	}
}

// initializes PWM for this project
// its enable, duty, and period are set
// in global file pointers in the header 
void init_pwm() {
	FILE* pwmSet = fopen("/sys/devices/bone_capemgr.9/slots", "w"); 
	if(!pwmSet) {
		printf("pwmSet Broke\n");
		exit(1);
	} 
	fprintf(pwmSet, "%s", "am33xx_pwm");
	fflush(pwmSet); 
	fprintf(pwmSet, "%s", "bone_pwm_P9_14");
	fflush(pwmSet);
	usleep(10000); 
	musPeriod = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/period", "w");
	if(!musPeriod) printf("musPeriod broke\n");
	// set duty pointer 
	musDuty = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/duty", "w");
	if(!musDuty) printf("musDuty broke\n");
	fprintf(musDuty, "%d", 100000);
	fflush(musDuty); 
	// set run/enable pointer 
	musEnable = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/run", "w");
	if(!musEnable) printf("musEnable broke\n"); 
	fprintf(musEnable, "%d", 0); 
	fflush(musEnable); 
}

void play(int period) {
	if (period == 0) {
		fprintf(musEnable, "%d", 0); 
		fflush(musEnable);
		usleep(200000);
	} else {
		fprintf(musPeriod, "%d", period);
		fflush(musPeriod);
		fprintf(musEnable, "%d", 1); 
		fflush(musEnable);
		usleep(200000);
	}	
	
}


