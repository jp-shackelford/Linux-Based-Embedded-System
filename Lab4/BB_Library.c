#include <stdio.h>      // for File IO and printf
#include <time.h>       // for usleep
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/stat.h>
#include <unistd.h>
// file that when written to initalizes a GPIO folder 
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define PATH_BUF 1024



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
  // note: that one would normally call fprintf to write
  // an int with "%d" and an int value, all inputs to this
  // must be wraped as a string
void writeToStream(FILE *stream, char *format, char *value) {
    if(strcmp(format, "%d") == 0) {
        // cast back into int if writing is of type int
        fprintf(stream, format, atoi(value));
        fflush(stream);
    } else {
        fprintf(stream, format, value);
        fflush(stream);
    }
}

// returns a pointer to value of a GPIO 
// also initializes said gpio 
FILE* initGPIO(int gpioNumber) {
    FILE *exportP = getStream(GPIO_EXPORT_PATH, "w");
    char gpioNumString[PATH_BUF];
    sprintf(gpioNumString, "%d", gpioNumber);
    writeToStream(exportP, "%d", gpioNumString);
    FILE* currentP; 
    char tempPath[PATH_BUF];
    sprintf(tempPath, "%s%d%s", "/sys/class/gpio/gpio", gpioNumber, "/direction");
    currentP = getStream(tempPath, "w");
    writeToStream(currentP, "%s", "out");
    sprintf(tempPath, "%s%d%s", "/sys/class/gpio/gpio", gpioNumber, "/value");
    currentP = getStream(tempPath, "w");
    return getStream(tempPath, "w"); 
} 

// returns a pointer to duty cycle of PWM
// takes int as a parameter
// 0 for EHRPWM1A, 1 for EHRPWM2A
FILE* initPWM(int pwm) {
	FILE* pwmSet = getStream("/sys/devices/bone_capemgr.9/slots", "w"); 
	writeToStream(pwmSet, "%s", "am33xx_pwm");
	writeToStream(pwmSet, "%s", "bone_pwm_P9_14");
    writeToStream(pwmSet, "%s", "bone_pwm_P8_19"); 
    char path[PATH_BUF];
    // if path == 1, use EHRPWM2A
    if(pwm) {
        sprintf(path, "%s", "/sys/devices/ocp.3/pwm_test_P8_19.16/");
    } else { // path == 0 use EHRPWM1A
        sprintf(path, "%s", "/sys/devices/ocp.3/pwm_test_P9_14.15/"); 
    }
    char ppath[PATH_BUF];
    char dpath[PATH_BUF];
    char epath[PATH_BUF]; 
    sprintf(ppath, "%s%s", path, "period");
    sprintf(dpath, "%s%s", path, "duty");
    sprintf(epath, "%s%s", path, "run");
	FILE* period = getStream(ppath, "w");
    writeToStream(period, "%d", "1020408"); //Set Frequency to 980Hz
	FILE* duty =  getStream(dpath, "w");
	writeToStream(duty, "%d", "510204"); // intialize to 50% duty cycle  
	FILE* enable = getStream(epath, "w");
	writeToStream(enable, "%d", "1"); 
    return duty; 
}

// sets duty of a PWM from 0 - 100 % in 12 step increments 
void setDuty(FILE* pwmDuty, int state) {
    switch(state) {
        // 0 % duty cycle (low)
        case 0: writeToStream(pwmDuty, "%d", "0");
                break;
        // 12.5% duty cycle
        case 1: writeToStream(pwmDuty, "%d", "127551");
                break;
        // 25% duty cycle
        case 2: writeToStream(pwmDuty, "%d", "255102");
                break;
        // 37.5% duty cycle
        case 3: writeToStream(pwmDuty, "%d", "382653");
                break;
        // 50% duty cycle
        case 4: writeToStream(pwmDuty, "%d", "510204");
                break;
        // 62.5% duty cycle
        case 5: writeToStream(pwmDuty, "%d", "637755");
                break; 
        // 75% duty cycle
        case 6: writeToStream(pwmDuty, "%d", "765306");
                break;
        // 87.5% duty cycle
        case 7: writeToStream(pwmDuty, "%d", "892857");
                break;
        // 100% duty cycle (high)
        case 8: writeToStream(pwmDuty, "%d", "1020408"); 
                break;
        default: printf("Bad input, between 0-8\n");
                break;
    }
}