#include <stdlib.h>
#include <stdio.h>

int guessingGame(int left) {

	int correct = 0;
	int guess = 0;

	int num = (rand() % 100) + 1; // Gives random number from 1-100

	//printf("%d\n", num);
	printf("\n");
	printf("%s%d%s\n", "I'm thinking of a number from 1 to 100. You have ", left, " guesses..." );

	while (correct==0 && left>0) {
		
		scanf("%d", &guess);
		left--;

		if (guess == num) {
			printf("%s\n", "Correct! You win!");
			printf("\n");
			correct = 1;
			break;
		} else {
			if (left == 0) {
				printf("\n");
				printf("%s%d\n","Game over! The correct answer was ", num);
				break;
			}

			printf("%s", "Incorrect.");
			if (guess > num) {
				printf("%s", "Try lower...        ");
			} else {
				printf("%s", "Try higher...       ");
			}
			if (left == 1) {
				printf("%s\n","Last guess!");
			} else if (left > 0) {
				printf("%d%s\n", left, " guesses left");
			} 
		}
	}

	return correct;

}

int totalGuesses = 6;

int main() {

	srand(time(NULL));

	char again[20] = "n";
	int games = 0;
	int won = 0;
	
	while (1) {
		
		games++;
		if (guessingGame(totalGuesses)) {
			won++;
		}
		
		printf("%s%d%s%d%s\n", "You've won ", won, " games out of ", games, ".");

		printf("%s\n", "Play again? (y/n)");
		while (1) {
			
			scanf("%s", &again);
			
			if (again[0] == 'n' || again[0] == 'N') {
				printf("\n");
				printf("%s\n", "Thanks for playing :)");
				printf("%s\n", "      --Group 6");
				printf("\n");
				return 0;

			} else if (again[0] == 'y' || again[0] == 'Y') {
				break;

			} else {
				printf("%s\n", "Invalid input. Please enter a valid response.");
			}
		}

	}

	return 0;
}