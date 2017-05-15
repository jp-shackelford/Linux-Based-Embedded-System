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
void writeToStream(FILE *stream, char *format, char *value) {
    fprintf(stream, format, value);
    fflush(stream);
}

// returns a pointer to value of a GPIO 
FILE* initGPIO(int gpioNumber) {
    FILE *exportP = getStream(GPIO_EXPORT_PATH, "direction");
    char gpioNumString[PATH_BUF];
    sprintf(gpioNumString, "%d", gpioNumber);
    writeToStream(exportP, "%d", gpioNumString);
    FILE* currentP; 
    char tempPath[PATH_BUF];
    sprintf(tempPath, "%s%d%s", "/sys/class/gpio/gpio", gpioNumber, "/direction");
    writeToStream(currentP, "%s", "out");
    sprintf(tempPath, "%s%d%s", "/sys/class/gpio/gpio", gpioNumber, "/value");
    return getStream(tempPath, "w"); 
} 

// returns a pointer to duty cycle of PWM
// takes int as a parameter
// 0 for EHRPWM1A, 1 for EHRPWM2A
FILE* initPWM(int pwm) {
	FILE* pwmSet = getStream("/sys/devices/bone_capemgr.9/slots", "w"); 
	writeToStream(pwmSet, "%s", "am33xx_pwm");
	writeToStream(pwmSet, "%s", "bone_pwm_P9_14");
    writeToStream(pwmSet, "%s", "bome_pwm_p8_19"); 
    char path[PATH_BUF];
    // if path == 1, use EHRPWM2A
    if(pwm) {
        sprintf(path, "%s", "/sys/devices/ocp.3/pwm_test_P8_19.15/");
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
	writeToStream(enable, "%d", "0"); 
    return duty; 
}