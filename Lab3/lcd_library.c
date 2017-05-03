#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#define LCD_DIR "/dev/lcd"

// directory for kernel space
static int fd; 

// intializes library for use
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


// standard  command for LCD, takes string of
// data of length 10, will print to screen
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

// shifts cursor right
void shiftRight() {
	command("0000010100");
}

// shifts cursor left
void shiftLeft() {
	command("0000010000");
}

//sets cursor position from 0 to 15
// takes int position as a parameter 
void setCursor(int pos) {
	command("0000000010"); // Set cursor to home position
	int curr = 0;
	while (pos != curr && curr<65) {
		if (curr==15) {
			curr = 40;
		} else {
			shiftRight();
			curr++;
		}
	}
}

// call to this function will turn cursor on/of
// 0 is off, 1 is on, 2 is off and blink, 3 is on and blink
void cursorState(int x) {
	switch(x) {
		case 0: command("0000001100");
			break;
		case 1: command("0000001110");
			break;
		case 2: command("0000001101");
			break;
		case 3: command("0000001111");
			break;
	}
}



// clears screen sets cursor to top left
void clear_and_home() {
	command("0000000011");
	usleep(16000);
} 

// sets cursor back to position 0 
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

/* used for testing purposes 
int main() {
	init_lcd_library(); 
	clear_and_home();
	writeString("is this real?\n");
	home(); 
	usleep(1000000);
	shiftRight();
	usleep(1000000); 
	shiftLeft();
	home();
	setCursor(11);
	usleep(1000000);
	cursorState(0);
	usleep(1000000);
	cursorState(1);
	usleep(1000000);
	cursorState(2);
	return 0;
} */ 
