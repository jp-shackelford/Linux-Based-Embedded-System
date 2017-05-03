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
	strcpy(write_buf, "H");
	write(fd,write_buf, (size_t)sizeof(write_buf), 0);
	printf("size of write buff = %d\n", sizeof(write_buf));
	usleep(1000000);
	strcpy(write_buf, "Z");
	write(fd,write_buf, (size_t)sizeof(write_buf), 0);
	printf("size of write buff = %d\n", sizeof(write_buf));
	close(fd);
	return 0;
}
