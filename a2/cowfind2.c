#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXLEN 1000000
char input[MAXLEN];

int main() {
	scanf("%s", input);

	/*
		start from end of input

		keep track of number of )) seen

		if (( seen add current number of )) seen to counter
	
		O(n)
	*/

	int j;

	for(int i = 0; i < MAXLEN; i++){
		if(input[i + 1] == '\0'){
			j = i;
			break;
		}
	}

	int counter = 0;
	int seen = 0;
	while(j > 0){
		if(input[j] == ')' && input[j - 1] == ')'){
			seen++;
		} else if(input[j] == '(' && input[j - 1] == '('){
			counter += seen;	
		}
		j -= 1;
	}
	
	
	printf("%d\n", counter);
}
