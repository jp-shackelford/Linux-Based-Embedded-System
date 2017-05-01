#include "lcd_user.h"

int main () {
	
	int fd;

	// Open a new LCD module
    fd = open(DEV_FILE, O_WRONLY);

    if (fd < 0) {
        printf("File %s cannot be opened\n", DEV_FILE);
        return -1;
    }

    while (1) {
    	/* IMPLEMENT USER CODE HERE
    	
    	.

    	.

    	.

    	*/
    }

    close(fd); // Close the module

    return 0;
}

char* getInput() {
    static char inputBuffer[256];
    int i;
    printf("Enter: ");
    fgets(inputBuffer, 256, stdin);
    printf("\n");

    return inputBuffer;
}