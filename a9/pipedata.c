#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_ARGS 15
#define MAX_BUFFER_LEN 1000000

int main(int argc, char *argv[]){

	if(argc != 3){
		printf("pipedata \"command 1\" \"command 2\"\n");
		return 0;
    }

	char *command1[MAX_ARGS];
	char *command2[MAX_ARGS];

	int i = 0;
	char *token = strtok(argv[1], " ");
	while(token){
		//printf("%s\n", token);
		command1[i] = token;
		token = strtok(NULL, " "); // continue detokenizing
		
		i += 1;
	}
	command1[i] = NULL;
	i = 0;

	token = strtok(argv[2], " ");
	while(token){
		//printf("%s\n", token);
		command2[i] = token;
		token = strtok(NULL, " "); // continue detokenizing

		i += 1;
	}

	// pipe setup
	int parent[2];

	pipe(parent);

	if(fork() == 0){
		// child

		dup2(parent[1], STDOUT_FILENO);

		close(parent[0]);
		close(parent[1]);
		
		// replaces curr process w/ command process
		execvp(command1[0], command1);

		perror("execvp");

		return 0;
	}

	dup2(parent[0], STDIN_FILENO);

	wait(NULL);

	close(parent[1]);

	// debugging reading the output of command1
	/*char buffer[MAX_BUFFER_LEN];
	int j = 0;

	while(read(parent[0], buffer + j, 1) > 0){
		j += 1;
	}

	command2[i] = buffer;
	command2[i + 1] = NULL;
	*/

	close(parent[0]);	

	execvp(command2[0], command2);

	perror("execvp");

	return 0;
}


