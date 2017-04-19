#include<stdio.h>
#include<string.h>

unsigned int_to_bin (unsigned k) {
	return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_bin(k / 2)));
}

void get_bits(unsigned k) {
	int bit;
	do {
		bit = (k % 2);
		k = k / 10;
		printf("%d\n", bit);
	} while(k != NULL);
}

void char_to_bin() {
	char key;
	unsigned k;
	char exit_key = 'q';

	do {
        	scanf(" %c", &key);
        	k = int_to_bin((unsigned)key);
       		//printf("%d \n", k);
		get_bits(k);
	} while(int_to_bin((unsigned)key) != int_to_bin((unsigned)exit_key));
}

int main(int argc, char **argv) {
	char_to_bin();
}
