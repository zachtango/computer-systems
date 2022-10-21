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

struct Player {
	char name[MAXLEN];
	double bal;
};

//arrays to store the player names and balances
struct Player players[MAXPLAYERS];
int numPlayers = 0; //set when actual player data is loaded

//search the player names array and return the index if specified name is found
//return -1 otherwise.
int findIndex(char *name) {
        for(int i=0; i<numPlayers; i++)
            if(strcmp(name, players[i].name) == 0)
               return i;

        return -1;
}

// use binary data file gifts2.dat to read and store the results.

int main(int argc, char *argv[]) {
	if(argc < 4){
		printf("Expected 3 or more args\n");
		return 0;
	}

	int MAX_BUFFER_SIZE = MAXPLAYERS * (MAXLEN + 10);
	
	FILE *filep; // = fopen("gifts3.dat", "wb");
	
	bool new = strcmp(argv[1], "new") == 0;
	
	if(new){
		filep = fopen("gifts3.dat", "wb");

		char space = ' ';
		char nl = '\n';
		double bal;
		for(int i = 0; i < argc - 2; i += 2){
			bal = atof(argv[i + 3]);

			printf("%s: %.2lf\n", argv[i + 2], bal);
			
			fwrite(argv[i + 2], 1, strlen(argv[i + 2]) + 1, filep);
			fwrite(&space, 1, sizeof(char), filep);
			fwrite(argv[i + 3], 1, strlen(argv[i + 3]) + 1, filep);
			fwrite(&nl, 1, sizeof(char), filep);
		}
	} else{
		filep = fopen("gifts3.dat", "rb");
	
		char buffer[MAX_BUFFER_SIZE + 1];
		buffer[MAX_BUFFER_SIZE] = '\0';

		fread(buffer, MAX_BUFFER_SIZE, 1, filep);

		char name[MAXLEN];
		char bal[MAXLEN];
		
		char *c = buffer;
		int n = strlen(c);
		
		int i = 0;
		while( n != 0 ){

			strcpy(name, c);
			strcpy(players[i].name, name);

			c += n + 2;
			n = strlen(c);

			if(c >= (buffer + MAX_BUFFER_SIZE) ){
				break;
			}
			strcpy(bal, c);
			players[i].bal = atof(bal); 

			c += n + 2;
			n = strlen(c);

			i += 1;
		}


		numPlayers = i;
		char sender[MAXLEN];
		double total = atof(argv[2]);
		double amt = total / (argc - 3);

		strcpy(sender, argv[1]);
		players[findIndex(sender)].bal -= total;

		for(i = 0; i < argc - 3; i++){
			char receiver[MAXLEN];
			strcpy(receiver, argv[i + 3]);
			players[findIndex(receiver)].bal += amt;
		}

		freopen("gifts3.dat", "wb", filep);
		
		char space = ' ';
		char nl = '\n';

		for(i = 0; i < numPlayers; i++){
			snprintf(bal, 50, "%lf", players[i].bal);
			
			printf("%s: %.2lf\n", players[i].name, players[i].bal);

			fwrite(players[i].name, 1, strlen(players[i].name) + 1, filep);
			fwrite(&space, 1, sizeof(char), filep);
			fwrite(bal, 1, strlen(bal) + 1, filep);
			fwrite(&nl, 1, sizeof(char), filep);
		
		}
	}
	
	
	
	
	fclose(filep);
}
