#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

	if(argc != 2){
		printf("pipedata \"command 1\" \"command 2\"\n");
		return 0;
    }

	// pipe setup
	int parent[2];

	pipe(parent);
	pipe(child);

	if(fork() == 0){
		// child
		close(parent[0]);

		dup2(parent[1], STDOUT_FILENO);

		execvp(argv[1], NULL); // replaces curr process w/ command process

		perror("execvp");

		return 0;
	}

	close(parent[1]);

	char buffer[10];

	read(parent[0], 10);
	close(parent[0]);

	printf("%s\n", buffer);

	return 0;
}


