/* Music.C, uses GPIO pins to drive LEDS to counter up to 7 and binary
as well drive pwm to output series of musical notes*/ 

#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep

#define GPIO_PIN_0 45      // LED pins
#define GPIO_PIN_1 69
#define GPIO_PIN_2 66

int main() {

	// Creates the File pointers to create the gpio file system,
	// set the direction of the GPIO, and change the data stored there.
	FILE *sys, *pwmSet, *dir, *val;

	// Specifies the file that the pointer will be used for (w = //write)
	sys = fopen("/sys/class/gpio/export", "w");
	fseek(sys, 0, SEEK_SET);
	// init two folders for PWM pins
   	pwmSet = fopen("/sys/devices/bone_capemgr.9/slots", "w"); 
	if(!pwmSet) printf("pwmSet Broke"); 
	fprintf(pwmSet, "%s", "am33xx_pwm");
	fflush(pwmSet); 
	fprintf(pwmSet, "%s", "bone_pwm_P9_14");
	fflush(pwmSet);
	fprintf(pwmSet, "%s", "bone_pwm_P8_19"); 
	fflush(pwmSet); 

	// init specific GPIO pin folders 
	fprintf(sys, "%d", GPIO_PIN_0);
	fflush(sys);
	fprintf(sys, "%d", GPIO_PIN_1);
	fflush(sys);
	fprintf(sys, "%d", GPIO_PIN_2); 
	fflush(sys);

	//Set the gpios to output
	FILE *dir1, *dir2, *val1, *val2, *musRun;
	dir = fopen("/sys/class/gpio/gpio45/direction", "w");
	fseek(dir, 0, SEEK_SET);
	fprintf(dir, "%s", "out");
	fflush(dir);
	dir1 = fopen("/sys/class/gpio/gpio69/direction", "w");
	fseek(dir1, 0, SEEK_SET);
	fprintf(dir1, "%s", "out");
	fflush(dir1);
	dir2 = fopen("/sys/class/gpio/gpio66/direction", "w");
	fseek(dir2, 0, SEEK_SET);
	fprintf(dir2, "%s", "out");
	fflush(dir2);

	//Opens the file that controls if the pin is high or low
	val = fopen("/sys/class/gpio/gpio45/value", "w");
	fseek(val, 0, SEEK_SET);	
	val1 = fopen("/sys/class/gpio/gpio69/value", "w");
	fseek(val1, 0, SEEK_SET);
	val2 = fopen("/sys/class/gpio/gpio66/value", "w");
	fseek(val2, 0, SEEK_SET); 

	// due to time constraints of creating PWM folders, a delay
	// must occur in order to give time for the folders to generate
	usleep(300000);
	
	// set period pointer 
	musRun = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/period", "w");
	if(!musRun) printf("musRun broke\n");
	// set duty pointer 
	FILE *musDuty = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/duty", "w");
	if(!musDuty) printf("musDuty broke\n");
	fprintf(musDuty, "%d", 100000);
	fflush(musDuty); 
	// set run/enable pointer 
	FILE *musEnable = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/run", "w");
	if(!musEnable) printf("musEnable broke\n"); 
	fprintf(musEnable, "%d", 1); 
	fflush(musEnable); 
	
	// used for loop logic 
	int count = 0; 
	int count2 = 0; 
	//Blinks the LED and steps through notes 
	while(1) {
		switch(count) {
			case 0:
				// A4 
				fprintf(musRun, "%d", 3822629);
				fprintf(val, "%d", 0);
				fprintf(val1, "%d", 0);
				fprintf(val2, "%d", 0);
				break;
			case 1:
				fprintf(musRun, "%d", 3405299);
				fprintf(val, "%d", 1);
				fprintf(val1, "%d", 0);
				fprintf(val2, "%d", 0);
				break; 
			case 2:
				fprintf(musRun, "%d", 3214090);
				fprintf(val, "%d", 0);
				fprintf(val1, "%d", 1);
				fprintf(val2, "%d", 0);
				break;	
			case 3:
				fprintf(musRun, "%d", 2873442);
				fprintf(val, "%d", 1);
				fprintf(val1, "%d", 1);
				fprintf(val2, "%d", 0);
				break;
			case 4:
				fprintf(musRun, "%d", 2702775);
				fprintf(val, "%d", 0);
				fprintf(val1, "%d", 0);
				fprintf(val2, "%d", 1); 
				break;
			case 5:
				fprintf(musRun, "%d", 2551020);
				fprintf(val, "%d", 1);
				fprintf(val1, "%d", 0);
				fprintf(val2, "%d", 1); 
				break;
			case 6:
				fprintf(musRun, "%d", 2145186);
				fprintf(val, "%d", 0);
				fprintf(val1, "%d", 1);
				fprintf(val2, "%d", 1); 
				break;
			case 7:	
				fprintf(musRun, "%d", 1911132);
				fprintf(val, "%d", 1);
				fprintf(val1, "%d", 1);
				fprintf(val2, "%d", 1); 
				break; 
		}


		fflush(musRun); 
		fflush(val);
		fflush(val1);
		fflush(val2);
		if(count2 == 0) {
      			usleep(300000);
		} else if (count2 == 1) {
			usleep(150000);
		} else {
			usleep(75000);
		} 
		if(7 == count) {
			count = 0;
			count2++;
		} else {
			count++;
		}
		if(count2 == 6) {
			break;
		} 
	}

   // close all files
	fprintf(musEnable, "%d", 0);
	fprintf(val, "%d", 0);
	fprintf(val1, "%d", 0);
	fprintf(val2, "%d", 0); 
	fflush(musRun); 
	fflush(val);
	fflush(val1);
	fflush(val2);
	fclose(sys);
	fclose(dir);
	fclose(dir1);
	fclose(dir2); 
	fclose(val);
	fclose(val1);
	fclose(val2);
	fclose(pwmSet);
	fclose(musRun);
	fclose(musEnable);
	fclose(musDuty); 
	return 0;
}
