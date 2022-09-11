/*
 *	Zach Tang cs3377.005 zxt200009
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){
	srand(time(NULL));
	
	int r = rand() % 100 + 1;

	printf("Ok, I am ready!\n");

	int counter = 1;
	while(1){
		int guess;
		printf("Enter your guess: ");

		scanf("%d", &guess);
		
		if(guess < r){
			printf("Go higher!\n");
		} else if(guess > r){
			printf("Go lower!\n");
		} else{
			printf("You got it in %d tries!\n", counter);		
			break;
		}
		
		counter += 1;
	}
	
	return 0;
}
