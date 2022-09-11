/*
 *	Zach Tang CS 3377.005 zxt200009
 *
 */

#include <stdio.h>



int main(){
	/*
		think of an umber between 1 and 100
		the computer will ask a series of questions and determine that number
		based on your answers


		ask higher or lower
			binary search
	*/
	

	int left = 1, right = 100;
	while(1){
		int guess = (left + right) / 2;
		printf("How about %d (<, =, >)? ", guess);
		
		char c;
		scanf("%c", &c);

		if(c == '<'){
			right = guess - 1;

		} else if(c == '='){
			printf("It was a good game!\n");
			break;
		} else if(c == '>'){
			left = guess + 1;
		}

	}


	return 0;
}
