#include "lcd_user.h"

int main () {
	
	int fd;
	char* str[100];
	int length = 0;

	// Open a new LCD module
    fd = open(DEV_FILE, O_WRONLY);

    if (fd < 0) {
        printf("File %s cannot be opened\n", DEV_FILE);
        return -1;
    }

    while (1) {
    	str = getInput();
    	length = strlen(str);
    	write(fd, str, length);
    }

    close(fd); // Close the module

    return 0;
}

char* getInput() {
    static char inputBuffer[256];
    printf("Enter: ");
    fgets(inputBuffer, 256, stdin);
    printf("\n");

    return inputBuffer;
}
