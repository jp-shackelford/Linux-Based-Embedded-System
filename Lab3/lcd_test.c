#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#define LCD_DIR "/dev/lcd"

int main(void)  {
	int size_bu,fd;
	char write_buf[100];
	fd = open(LCD_DIR, O_RDWR);
	if(fd < 0) {
		printf("File %s cannot be opened\n", LCD_DIR);	
		exit(1);
	}
	// set what will be written to the file
	strcpy(write_buf, "1001001100");
	write(fd, write_buf, 10); 
	usleep(1000000);
	//clear();
	return 0;
}



char* getInput() {
	static char input[256];
	int i;
	printf("Enter: ");
	fgets(input, 256, stdin);
	printf("\n");
	return input;
}
