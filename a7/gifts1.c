#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

//let us assume that there will not be more than 100 players
#define MAXPLAYERS 100
//let us assume that max length for any name is 100 characters
#define MAXLEN 100


//arrays to store the player names and balances
char names[MAXPLAYERS][MAXLEN];
double balances[MAXPLAYERS];
int numPlayers = 0; //set when actual player data is loaded

//search the player names array and return the index if specified name is found
//return -1 otherwise.
int findIndex(char *name) {
        for(int i=0; i<numPlayers; i++)
            if(strcmp(name, names[i]) == 0)
               return i;

        return -1;
}

int main(int argc, char *argv[]) {
	if(argc < 4){
		printf("Expected 2 or more args\n");
		return 0;
	}

	FILE *filep; //fopen("gifts1.txt", "w+");

	bool new = strcmp(argv[1], "new") == 0;
	int argOffset = 1;

	if(new){
		filep = fopen("gifts1.txt", "w");

		for(int i = 0; i < argc - 2; i += 2){
			printf("%s: %.2lf\n", argv[i + 2], atof(argv[i + 3]));
			
			fprintf(filep, "%s %lf\n", argv[i + 2], atof(argv[i + 3]));
		}

	} else{
		filep = fopen("gifts1.txt", "r+");

		int i = 0;
		while(fscanf(filep, "%s %lf\n", names[i], balances + i) != EOF){
			i += 1;
		}

		freopen("gifts1.txt", "w+", filep);

		numPlayers = i;

		char sender[MAXLEN];
		double total = atof(argv[2]);
		double amt = total / (argc - 3);

		strcpy(sender, argv[1]);
		
		balances[findIndex(sender)] -= total;

		for(i = 0; i < argc - 3; i++){
			char receiver[MAXLEN];
			strcpy(receiver, argv[i + 3]);
			
			balances[findIndex(receiver)] += amt;	
		}
		
		for(i = 0; i < numPlayers; i++){
			printf("%s: %.2lf\n", names[i], balances[i]);		
			fprintf(filep, "%s %lf\n", names[i], balances[i]);
		}
	}


	
	fclose(filep);
	return 0;
}



