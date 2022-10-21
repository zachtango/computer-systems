/*
 *	Zach Tang CS 3377.005 zxt200009
 *
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int tochild[2], toparent[2];

int child(){
	/*
		think of an number between 1 and 100
		the computer will ask a series of questions and determine that number
		based on your answers


		ask higher or lower
			binary search
	*/
	

	int left = 1, right = 100;
	while(1){
		int guess = (left + right) / 2;
		
		char guessStr[4];
		sprintf(guessStr, "%d", guess);

		printf("Child wrote %s\n", guessStr);
		write(toparent[1], guessStr, 4);
		//printf("How about %d (<, =, >)? ", guess);

		char response[2];
		read(tochild[0], response, 2);
		printf("Child read %s\n", response);
		
		char c = response[0];
		//scanf("%c", &c);

		if(c == '<'){
			right = guess - 1;

		} else if(c == '='){
			printf("It was a good game parent!\n");
			break;
		} else if(c == '>'){
			left = guess + 1;
		}
	}
	
	return 0;
}


int parent(){
	srand(time(NULL));
	
	int r = rand() % 100 + 1;

	printf("Ok, I am ready!\n");

	int counter = 1;
	while(1){
		char guessStr[4];
	//	printf("Enter your guess: ");
		
		//scanf("%d", &guess);
		read(toparent[0], guessStr, 4);

		int guess = atoi(guessStr);
		printf("Parent read %d\n", guess);

		char c[2] = ".";
		if(guess < r){
			//printf("Go higher!\n");
			c[0] = '>';
		} else if(guess > r){
			//printf("Go lower!\n");
			c[0] = '<';
		} else{
			c[0] = '=';
		}
		
		write(tochild[1], c, 2);
		printf("Parent wrote %s\n", c);

		if(c[0] == '='){	
			printf("Child got it in %d tries!\n", counter);
			break;
		}

		counter += 1;
	}
	
	return 0;
}

int main(){
	pipe(tochild);
	pipe(toparent);
	
	
	if(fork() > 0){
		// parent
		parent();
	} else{
		child();
	}



	return 0;
}




