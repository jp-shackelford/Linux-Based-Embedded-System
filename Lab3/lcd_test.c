#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#define LCD_DIR "/dev/lcd"

static int fd; 

int init_lcd_library(void)  {
	int size_bu; 
	char write_buf[100];
	fd = open(LCD_DIR, O_RDWR);
	if(fd < 0) {
		printf("File %s cannot be opened\n", LCD_DIR);	
		exit(1);
	}
	return 0; 
}



static void command(char* data) {
	if(strlen(data) == 10) {
		char buff[10];
		strcpy(buff, data);
		write(fd, buff, 10);
		//usleep(100); 
	} else {
		printf("bad data:%s\n", data);	
	}
}

// helper function for toBits
static unsigned int_to_bin (unsigned k) {
	return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_bin(k / 2)));
}

// parses a charter to its ascii equivalent, returns
// and string of length 8 with ascii representation
static char* toBits(char key) {
   unsigned k;
   int bit;
   char exit_key = 'q';
	static int a[8];	
   k = int_to_bin((unsigned)key); // get key's ascii code
   //Parses the key's ascii code, diplaying one bit at a time
	//while( k != NULL && key != exit_key) {
	int i;
	for(i = 0; i < 8; i++) {
		bit = (k % 2);
      k = k / 10;
		a[i] = bit;
	}
	static char c[8];
	int j = 7;
	for(i = 0; i < 8; i++) {
		if(1 == a[i]) {
			c[j] = '1';		
		} else {
			c[j] = '0'; 		
		}
		j--; 
	}
	return c; 
}

// writes a character to current cursor location
void writechar(char x) {
	// pointer to beginning of ascii array
	char c[11] = "10";
	char* p = toBits(x);
	strcat(c, p);
	command(c); 
}

// clears screen sets cursor to top let
void clear_and_home() {
	command("0000000001");
	usleep(16000);
} 

void home() {
	command("0000000010");
}

// writes bottom 16 characters or till newline of a string
// to the top 16 places on LCD dot matrix 
void writeString(char* s) {
	clear_and_home(); 
	int i = 0;
	while(s[i] != '\n') { 
		writechar(s[i]);
		i++; 
	}
}

int main() {
	init_lcd_library(); 
	clear_and_home();
	writeString("is this real?\n");
	home();
	return 0;
}
