#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

//read all the quotes from quotes.txt
//when client asks for a motivational quote, select one randomly and send it out.

#define MAXQUOTES 10000
#define MAXLEN 1000

#define MAX_CHILDREN 1000

int parent[2];
int child[MAX_CHILDREN][2];

//runs a simple command
//cmdname arg1 arg2 arg3 ...
// void runCommand(char *command) {
// 	//split and assemble the arguments and invoke execvp()
// 	//use strtok(..)
// }

// //cmd0 | cmd1 | cmd2 | cmd3 | cmd4 
// void child(int i) {
// 	//rewire pipes to 0 and 1 
// 	//close unnecessary pipes
// 	//run ith command
// 	runCommand(commands[i]);
// }

// void processLine(char *line) {
// 	char *pipePtr = strchr(line, '|');
// 	char *equalPtr = strchr(line, '=');
// 	if (pipePtr) { //not NULL
// 		command has several sub-commands connected with pipes
// 		setup commands array
// 		setup pipes array
// 		create children --> invoke child(i) in a loop
// 		//cmd0 | cmd1 | cmd2 | cmd3 | cmd4 
// 	} else if (equalPtr) {
// 		command has = operator, so 2 commands --> 2 processes
//     } else 
// 		//it is a simple command, no pipe or = character
// 		runCommand(line);

// 	exit(0);
// }

int main() {
	// https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
	// load up all the quotes from quotes.txt
	char quotes[MAXQUOTES][MAXLEN];
	FILE *fp = fopen("quotes.txt", "r");
	char buffer[MAXLEN];
	int numQuotes = 0;
	while(fgets(buffer, MAXLEN, fp)) {
		for(int j = 0; j < MAXLEN; j++)
			quotes[i][j] = buffer[j];

		i += numQuotes;
	}
	fclose(fp);

	// infinite loop to serve the customer
	while (1) {
		//output a random quote to stderr
		fputs(quotes[rand() % numQuotes], stderr);
		fprintf(stderr, "# ");
		//get the user input
		fgets(buffer, MAXLEN, stdin);

		if(buffer[0] == 'q'){
			break;
		}

		// //spawn a child for taking care of it
		// if (fork() == 0) 
		// 	processLine(line);

		// //wait the child to finish the job!
		// int x=0;
		// wait(&x);
	}

	return 0;
}
