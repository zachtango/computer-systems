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
#define MAX_ARGS 15
#define MAX_COMMANDS 100

int children[MAX_COMMANDS][2];
char *commands[MAX_COMMANDS];
int numCommands;

void pipeChildren(){
	for(int i = 0; i < numCommands; i++){
		pipe(children[i]);
	}
}

void closePipes(){
	for(int i = 0; i < numCommands; i++){
		close(children[i][0]);
		close(children[i][1]);
	}

}

// runs a simple command
// cmdname arg1 arg2 arg3 ...
void runCommand(char *command) {

	//fprintf(stderr, "%s\n", command);
	char* args[MAX_ARGS];

	//split and assemble the arguments and invoke execvp()
	//use strtok(..)
	int i = 0;
	char *token = strtok(command, "\n ");
	while(token){
		args[i] = token;
		token = strtok(NULL, "\n "); // continue detokenizing
		
		i += 1;
	}

	args[i] = NULL;
	/*
	for(int j = 0; j < i; j++){
		fprintf(stderr, "test:%s", args[j]);
	}*/

	execvp(args[0], args);

	perror("execvp");

	exit(0);
}

//cmd0 | cmd1 | cmd2 | cmd3 | cmd4 
void child(int i) {
	//rewire pipes to 0 and 1 
	//close unnecessary pipes

	dup2(children[i][1], STDOUT_FILENO);
	
	if(i > 0)
		dup2(children[i - 1][0], STDIN_FILENO);

	closePipes();

	//run ith command
	runCommand(commands[i]);
}

void twoWayChild(int i){
	dup2(children[i][0], STDIN_FILENO);
	dup2(children[i + 2][1], STDOUT_FILENO);
	
	closePipes();

	runCommand(commands[i]);
}


void processLine(char *line) {

	char *pipePtr = strchr(line, '|');
	char *equalPtr = strchr(line, '=');

	if (pipePtr) { //not NULL
		// command has several sub-commands connected with pipes
		// setup commands array
		// setup pipes array
		// create children --> invoke child(i) in a loop
		//cmd0 | cmd1 | cmd2 | cmd3 | cmd4 

		int i = 0;
		
		char *token = strtok(line, "|");

		while(token){	
			commands[i] = token;
			token = strtok(NULL, "|"); // continue detokenizing
			
			i += 1;
		}
		
		numCommands = i;
		
		pipeChildren();

		for(i = 0; i < numCommands; i++){
			if(fork() == 0){
				child(i);
				exit(0);
			}
		}

		dup2(children[numCommands - 1][0], STDIN_FILENO);

		closePipes();
		
		char buffer[MAXLEN];
		while (fgets(buffer, MAXLEN, stdin) != NULL) {
       		fprintf(stderr, "%s", buffer);
      	}

	} else if (equalPtr) {
		// command has = operator, so 2 commands --> 2 processes
		int i = 0;
		char *token = strtok(line, "=");
		while(token){	
			commands[i] = token;
			token = strtok(NULL, "="); // continue detokenizing
			
			i += 1;
		}
		
		int parent[2];
		int toChild[2];

		pipe(parent);
		pipe(toChild);
		
		// redir output of process 1 to input of process 0

		if(fork() == 0){
			dup2(toChild[0], STDIN_FILENO);
			dup2(parent[1], STDOUT_FILENO);

			close(parent[0]);
			close(parent[1]);
			close(toChild[0]);
			close(toChild[1]);

			runCommand(commands[1]);

		} else{
			dup2(parent[0], STDIN_FILENO);
			dup2(toChild[1], STDOUT_FILENO);
			
			close(parent[0]);
			close(parent[1]);
			close(toChild[0]);
			close(toChild[1]);
			
			runCommand(commands[0]);
		}
		
		fprintf(stderr, "test\n");
		
	} else 
		//it is a simple command, no pipe or = character
		runCommand(line);

	exit(0);
}

int main() {
	// https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
	// load up all the quotes from quotes.txt
	
	char *quotes[MAXQUOTES];
	FILE *fp;
	fp = fopen("quotes.txt", "r");
	char buffer[MAXLEN];
	int numQuotes = 0;

	while(fgets(buffer, MAXLEN, fp)) {
		quotes[numQuotes] = malloc(strlen(buffer) + 1);
		strcpy(quotes[numQuotes], buffer);

		numQuotes += 1;
	}

	fclose(fp);

	fprintf(stderr, "%d quotes were read.\n", numQuotes);
	// infinite loop to serve the customer
	while (1) {
		//output a random quote to stderr
		fputs(quotes[rand() % numQuotes], stderr);
		fprintf(stderr, "# ");
		//get the user input
		fgets(buffer, MAXLEN, stdin);

		//spawn a child for taking care of it
		if (fork() == 0) 
			processLine(buffer);
		
		//wait the child to finish the job!
		int x=0;
		wait(&x);
	}

	return 0;
}
