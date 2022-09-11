/*
 * zxt200009 CS3377.005 Zach Tang
 * randompwd.c
 */


#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

const int NUM_ARGS = 6;
enum ch{LOWER, UPPER, DIGITS, SPECIAL};


// args <minLen> <maxLen> <minLower> <minUpper> <minDigits> <minSpecial>
int main(int argc, char*argv[]){
		
	if(argc - 1 > NUM_ARGS){
		printf("Too many arguments. Expected %d arguments\n", NUM_ARGS);
		return 0;
	} else if(argc - 1 < NUM_ARGS){
		printf("Too few arguments. Expected %d arguments\n", NUM_ARGS);
		return 0;
	}

	srand(time(NULL) + getpid());
	
	const int minLen = atoi(argv[1]),
			maxLen = atoi(argv[2]);

	char password[maxLen + 1];

	//printf("%d %d %d %d %d %d", minLen, maxLen, minLower, minUpper, minDigits, minSpecial);

	int len = rand() % (maxLen - minLen) + minLen; 
	
	char specialChars[11] = {'!', '@', '#', '$', '%', '^', '&', '*', '+', ':', ';'};

	int minimums[4] = {atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6])};
	int counters[4] = {0};
	int i = 0;
	
	while(i < len){
		int choice = rand() % 4; // between lower case, upper case, digits, and special
		
		// check if character count for each type has met criteria
		if(counters[choice] >= minimums[choice]){
			for(int j = 0; j < 4; j++){
				if(counters[j] < minimums[j]){
					choice = j;
					break;
				}
			}
		}

		counters[choice] += 1;

		char c;
		switch(choice){
			case LOWER:
				c = rand() % 26 + 97; // a - z = [97, 122]
				break;
			case UPPER:
				c = rand() % 26 + 65; // A - Z = [65, 90]
				break;
			case DIGITS:
				c = rand() % 10 + 48; // 0 - 9 = [48, 57]
				break;
			case SPECIAL:
				c = specialChars[rand() % 11]; 
		}


		password[i] = c;
		i += 1;
	}
	password[i] = '\0';

	printf("%s\n", password);


	return 0;
}
