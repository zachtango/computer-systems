#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_ARGS 15

int main(int argc, char *argv[]){

	if(argc != 3){
		printf("pipedata \"command 1\" \"command 2\"\n");
		return 0;
    }

	char *command1[MAX_ARGS];
	char *command2[MAX_ARGS];

	int i = 0;
	char *token = strtok(command1, " ");
	while(token){
		command1[i] = token;
		i += 1;
	}
	command1[i] = NULL;
	i = 0;

	token = strtok(command2, " ");
	while(token){
		command2[i] = token;
		i += 1;
	}
	command2[i] = NULL;

	// pipe setup
	int parent[2];

	pipe(parent);

	if(fork() == 0){
		// child

		dup2(parent[1], 1);

		close(parent[0]);
		close(parent[1]);
		
		// replaces curr process w/ command process
		execvp(command1[0], command1);

		perror("execvp");

		return 0;
	}

	wait(NULL);

	close(parent[1]);

	char buffer[10];

	read(parent[0], buffer, 1);
	close(parent[0]);

	printf("%s\n", buffer);

	return 0;
}


