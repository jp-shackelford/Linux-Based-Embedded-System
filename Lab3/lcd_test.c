#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#define LCD_DIR "/dev/lcd"

int main(void)  {
	int size_bu,fd;
	char write_buf[100];
	fd = open(LCD_DIR, O_RDWR);
	if(fd < 0) {
		printf("File %s cannot be opened\n", LCD_DIR);	
		exit(1);
	}
	strcpy(write_buf, "1001001100");
	write(fd,write_buf, sizeof(write_buf));
	printf("done did the write to device file\n");
	close(fd);
	return 0;
}
