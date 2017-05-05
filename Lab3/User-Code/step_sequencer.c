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
	static char timingBuffer[18] = "2222222222222222\n"; // Default is .2s for each

	int state = 0;
	while (state != 10) {
		// user input state
		if(state == 0) {
			printf("Choose a selection on the LCD!\n");
			cursorState(0); 	
			writeString("Input|q|w|t|0-6\n");	
			// Receive input from player	
			fgets(inputBuffer,18,stdin);
			//printf("got : %s\n", inputBuffer);
			setCursor(0);
			cursorState(0); 
			strcat(inputBuffer, "\n");
			if(inputBuffer[0] == 'q' || inputBuffer[0] == 'Q') {
				state = 10; 		
			} else if(inputBuffer[0] == 'w' || inputBuffer[0] == 'W') {
				state = 3;
			} else if(inputBuffer[0] == 't' || inputBuffer[0] == 'T') {
				state = 4;
			} else {
				state = 2; // Default is playback stage	
			}
		
		// playback state
		} else if(state == 2) {
			home(); 
			writeString(inputBuffer);
			setCursor(0);
			cursorState(1); // sets to blink
			printf("%s", inputBuffer);
			int i;
			for(i=0; i<16; i++) { // Know that we have exactly 16 notes
				cursorState(1);
				switch(inputBuffer[i]) {
					case '0': play(0, timingBuffer[i]);
						break;
					case '1': play(C, timingBuffer[i]);
						break;
					case '2': play(D, timingBuffer[i]);
						break;
					case '3': play(E, timingBuffer[i]);
						break;
					case '4': play(F, timingBuffer[i]);
						break;
					case '5': play(G, timingBuffer[i]);
						break;
					case '6': play(A, timingBuffer[i]); 
						break;
				}
				shiftRight();
			}
			state = 0; // Sends back to select stage
		
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
				if(writeIn[0] == 'r' || writeIn[0] == 'R' || writeIn[0] == '\n') {
					if (current != 15) {					
						shiftRight();
						current++;			
					}	
				} else if(writeIn[0] == 'l' || writeIn[0] == 'L') {
					if(current != 0) {				
						shiftLeft();
						current--; 		
					} else {
						state = 0;
					}
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
				} else if(writeIn[0] == 'i' || writeIn[0] == 'I') {
					state = 0;				
				} else if(writeIn[0] == 'q'|| writeIn[0] == 'Q') {
					state = 10;				
				} else if(writeIn[0] == 'p'|| writeIn[0] == 'P') {
					state = 2;		
				} else if(writeIn[0] == 't'|| writeIn[0] == 'T') {
 					state = 4;
				} else {
					state = 0; 				
				}
			}
		
		// Time edit stage
		} else if (state == 4) {
			strcpy(timingBuffer, "2222222222222222\n");
			setCursor(0);
			writeString(timingBuffer);
			setCursor(0);
			int current = 0;
			cursorState(3);
			
			while (state == 4) {	
			
				char writeIn[3];
				fgets(writeIn,3,stdin);
				
				if(writeIn[0] == 'r' || writeIn[0] == 'R' || writeIn[0] == '\n') {
					if (current != 15) {					
						shiftRight();
						current++;			
					}	
				} else if(writeIn[0] == 'l' || writeIn[0] == 'L') {
					if(current != 0) {				
						shiftLeft();
						current--; 		
					} else {
						state = 0; // return home
					}
				} else if(  writeIn[0] == '0' ||
								writeIn[0] == '1' ||
								writeIn[0] == '2' ||
								writeIn[0] == '3' ||
								writeIn[0] == '4' ||
								writeIn[0] == '5' ||
								writeIn[0] == '6' ||
								writeIn[0] == '7' ||
								writeIn[0] == '8' ||
								writeIn[0] == '9') {
					timingBuffer[current] = writeIn[0]; 
					writechar(timingBuffer[current]);	
					current++; 		
				} else if(writeIn[0] == 'i' || writeIn[0] == 'I') {
					state = 0;				
				} else if(writeIn[0] == 'q'|| writeIn[0] == 'Q') {
					state = 10;				
				} else if(writeIn[0] == 'p'|| writeIn[0] == 'P') {
					state = 2;		
				} else if(writeIn[0] == 'w'|| writeIn[0] == 'W') {
 					state = 3;
				} else {
					state = 0; 				
				}
			}	
		} 
		fprintf(musEnable, "%d", 0); 
		fflush(musEnable);
		usleep(200000);
	}
	
	// End-of-program cleanup
	clear_and_home();
	cursorState(0);	
	close(fd);
}

// initializes PWM for this project
// its enable, duty, and period are set
// in global file pointers in the header 
void init_pwm() {
	FILE* pwmSet = getStream("/sys/devices/bone_capemgr.9/slots", "w"); 
	if(!pwmSet) {
		printf("pwmSet Broke\n");
		exit(1);
	} 
	fprintf(pwmSet, "%s", "am33xx_pwm");
	fflush(pwmSet); 
	fprintf(pwmSet, "%s", "bone_pwm_P9_14");
	fflush(pwmSet);
	usleep(10000); 
	musPeriod = getStream("/sys/devices/ocp.3/pwm_test_P9_14.15/period", "w");
	if(!musPeriod) printf("musPeriod broke\n");
	// set duty pointer 
	musDuty =  getStream("/sys/devices/ocp.3/pwm_test_P9_14.15/duty", "w");
	if(!musDuty) printf("musDuty broke\n");
	fprintf(musDuty, "%d", 100000);
	fflush(musDuty); 
	// set run/enable pointer 
	musEnable = getStream("/sys/devices/ocp.3/pwm_test_P9_14.15/run", "w");
	if(!musEnable) printf("musEnable broke\n"); 
	fprintf(musEnable, "%d", 0); 
	fflush(musEnable); 
}

// plays a note for a period of time on the PWM
// takes int (period) and char time as parameters 
void play(int period, char time) {
	int t = 0;
	
	// Convert numerical char to int
	if (time == '0') t=0;
	else if (time == '1') t=1;
	else if (time == '2') t=2;
	else if (time == '3') t=3;
	else if (time == '4') t=4;
	else if (time == '5') t=5;
	else if (time == '6') t=6;
	else if (time == '7') t=7;
	else if (time == '8') t=8;
	else if (time == '9') t=9;
	else t=0; // Default is 0 sec	
	t *= 100000; // convert to 1/10 sec

	if (period == 0) { // Play no note
		fprintf(musEnable, "%d", 0); 
		fflush(musEnable);
		usleep(t);
	} else {
		fprintf(musPeriod, "%d", period);
		fflush(musPeriod);
		fprintf(musEnable, "%d", 1); 
		fflush(musEnable);
		usleep(t);
	}	
	
}

FILE* getStream(char *path, char *mode) {
        while(!fopen(path, mode)) {
                usleep(1);
        }
        return fopen(path, mode);
}


