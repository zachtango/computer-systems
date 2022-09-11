/*
 *	Zach Tang zxt200009 CS3377.005 compstr.c
 *
 */


#include <stdio.h>
#include <ctype.h>

#define MAX_LEN 100000


int main(){
	char str1[MAX_LEN], str2[MAX_LEN];

	printf("Enter string #1: ");
	fgets(str1, MAX_LEN, stdin);
	
	printf("Enter string #2: ");
	fgets(str2, MAX_LEN, stdin);
	
	int i = 0, j = 0;

	int same = 1;
	while(1){
		while(str1[i] != '\0' && !isalpha(str1[i])){
			i += 1;
		}
		
		while(str2[j] != '\0' && !isalpha(str2[j])){
			j += 1;
		}
	
		if(str1[i] == '\0' && str2[j] == '\0'){
			break;
		}

		if(tolower(str1[i]) != tolower(str2[j])){
			same = 0;
			break;
		}

		i += 1, j += 1;
	}

	if(same){
		printf("Both strings are same!\n");
	} else{
		printf("Both strings are different!\n");
	}

	return 0;
}

