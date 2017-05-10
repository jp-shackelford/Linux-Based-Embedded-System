/**********************************************************************************
*	Description:
*		This module controls the behavior of the H-Bridge chip that runs the
*		wheel motors on our BB Board.
*
*	Inputs:
*		mode: The mode of motor operation:
*			'F' - Forward
*			'R' - Reverse
*			'B'	- Brake
*			'L' - Loose
*
*	Outputs
*		A1: The input to the Ain1 of the H-Bridge
*		A2: The input to the Ain2 of the H-Bridge
*		B1: The input to the Bin1 of the H-Bridge
*		B2:	The input to the Bin2 of the H-Bridge
*
*	Author: Joshua Shackelford
*
*********************************************************************************/

#include "BRIDGE.h"

int motor_control(char mode, int* C0, int* C1);

int main(){
	char modeA;
	char modeB;
	int A0;
	int A1;
	int B0;
	int B1;

	printf("Mode A?(F|R|B|L) ");
	scanf("%s", &modeA);

	printf("Mode B?(F|R|B|L) ");
	scanf("%s", &modeB);

	if(motor_control(modeA, &A0, &A1) && motor_control(modeB, &B0, &B1)) {
		printf("%d\n%d\n%d\n%d\n", A0, A1, B0, B1);
	}
}

int motor_control(char mode, int* C0, int* C1) {

	switch(mode) {
		case('F'):
		case('f'):	*C0 = 0;
					*C1 = 1;
					break;
		case('R'):
		case('r'):	*C0 = 1;
					*C1 = 0;
					break;
		case('B'):
		case('b'):	*C0 = 1;
					*C1 = 1;
					break;
		case('L'):
		case('l'):	*C0 = 0;
					*C1 = 0;
					break;
		default:	printf("Error! Invlaid Input.\n");
					return 0;
					break;
	}
	return 1;
}