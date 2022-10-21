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

// use binary data file gifts2.dat to read and store the results.

int main(int argc, char *argv[]) {
	if(argc < 4){
		printf("Expected 3 or more args\n");
		return 0;
	}

	int MAX_BUFFER_SIZE = MAXPLAYERS * (MAXLEN + 10);

	int fd;
	
	bool new = strcmp(argv[1], "new") == 0;
	
	if(new){
		fd = open("gifts2.dat", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
		
		char space = ' ';
		char nl = '\n';
		double bal;
		for(int i = 0; i < argc - 2; i += 2){
			bal = atof(argv[i + 3]);

			printf("%s: %.2lf\n", argv[i + 2], bal); 
			
			write(fd, argv[i + 2], strlen(argv[i + 2]) + 1);
			write(fd, &space, sizeof(char));
			write(fd, argv[i + 3], strlen(argv[i + 3]) + 1);
			write(fd, &nl, sizeof(char));
		}
	} else{
		fd = open("gifts2.dat", O_RDWR);
	
		char buffer[MAX_BUFFER_SIZE + 1];
		buffer[MAX_BUFFER_SIZE] = '\0';

		read(fd, buffer, MAX_BUFFER_SIZE);

		char name[MAXLEN];
		char bal[MAXLEN];
		
		char *c = buffer;
		int n = strlen(c);
		
		int i = 0;
		while( n != 0 ){

			strcpy(name, c);
			strcpy(names[i], name);

			c += n + 2;
			n = strlen(c);

			if(c >= (buffer + MAX_BUFFER_SIZE) ){
				break;
			}
			strcpy(bal, c);
			balances[i] = atof(bal); 

			c += n + 2;
			n = strlen(c);

			i += 1;
		}


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

		lseek(fd, 0, SEEK_SET);
		
		char space = ' ';
		char nl = '\n';

		for(i = 0; i < numPlayers; i++){
			snprintf(bal, 50, "%lf", balances[i]);
			
			printf("%s: %.2lf\n", names[i], balances[i]);

			write(fd, names[i], strlen(names[i]) + 1);
			write(fd, &space, sizeof(char));
			write(fd, bal, strlen(bal) + 1);
			write(fd, &nl, sizeof(char));
		
		}
	}
	
	
	
	
	close(fd);
}
