#include<stdio.h>
#include<stdlib.h>
#include<string.h>

unsigned int_to_bin (unsigned k) {
	return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_bin(k / 2)));
}

int* toBits(char key) {

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

	return a;
}

// given a character, will write to cursor on screen
void writechar(char c) {
    int * p = toBits(c);
    int i;
        for(i = 0; i < 16; i++) {
            command(1,0,p[7],p[6],p[5],p[4],p[3],p[2],p[1],p[0]); 
        }
    close(p);
}

int test() {
	char key;
	int *a;
	scanf(" %c", &key);
	
	a = toBits(key);

        int j;
        for(j = 0; j < 8; j++) {
                printf("%d", a[j]);
        }
        printf("\n"); 
    return 0; 
}
