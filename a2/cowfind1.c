#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXLEN 1000000
char input[MAXLEN];

int main() {
	scanf("%s", input);

	/*
	 	brute force

		find all )) AFTER encountering a (( for every (( in input --> same as finding every pair

		O(n^2)
		
	*/

	int counter = 0;
	int i = 0;
	while(i + 1 < MAXLEN && input[i + 1] != '\0'){
		if(input[i] == '(' && input[i + 1] == '('){
			int j = i + 2;

			while(j + 1 < MAXLEN && input[j] != '\0' && input[j + 1] != '\0'){
				if(input[j] == ')' && input[j + 1] == ')'){
					counter += 1;
				}

				j += 1;
			}
		}

		i += 1;
	}
	
	printf("%d\n", counter);
}
