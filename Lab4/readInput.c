 #include <stdlib.h>  
 #include <stdio.h>  
 #include <string.h>  
 #include <unistd.h>
 #include <fcntl.h>  

#define SAMPLE_SIZE 10
#define SAMPLE_TIME 10000 // 10000µs = 10ms; 10 samples = .1s
#define THRESHOLD 

// Reads the value on an analog pin
int readADC(unsigned int pin) {  
    int fd;
    char buf[64]; 
    char val[4];     //holds up to 4 digits for ADC value  
      
    // concatenate directory with analog pin number
    snprintf(buf, 64, "/sys/devices/ocp.3/helper.15/AIN%d", pin); 
      
    fd = open(buf, O_RDONLY);
    if (fd < 0) {  
        printf("Error: Can't open ADC");
        exit(1);
    }
      
    read(fd, &val, 4);     //read ADC ing val (up to 4 digits 0-1799)  
    close(fd);     //close file and stop reading  
      
    return atoi(val);     //returns an integer value (rather than ascii)  
}

// Computes a^b
int power(int a, int b) {
    int i;
    int sum = a;
    for (i = 0; i < b - 1; i++) {
        sum *= a;
    }

    if (b < 1) {
        sum = 1;
    }

    return sum;
}

// Computes the average of a set of data
long average(int * data) {
    int i;
    int sum=0;
    int samples = SAMPLE_SIZE*2;

    if (samples<1) {
        printf("Number of samples must be greater than 0\n");
        exit(1);
    }

    for (i=0; i<samples; i++) {
        sum+=data[i];
    }

    return (long)sum/samples;
}


int main() {
    system("echo BB-ADC > /sys/devices/bone_capemgr.9/slots");

    static int front[2*SAMPLE_SIZE];
    static int back[2*SAMPLE_SIZE];


    int i;
    while (1) {
        for (i=0; i<SAMPLE_SIZE; i++) {
            front[i] = readADC(0);
            front[i+SAMPLE_SIZE] = readADC(2);
            back[i] = readADC(4);
            back[i+SAMPLE_SIZE] = readADC(6);
            usleep(SAMPLE_TIME);
        }

        long avgFront = average(front);
        long avgBack  = average(back);


        /* TODO:
            Interpret data. If avg's are less than the threshold then interrupt motors
        */
    }

    return 0;
}