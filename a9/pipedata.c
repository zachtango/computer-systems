#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

	if(argc != 3){
		printf("pipedata \"command 1\" \"command 2\"\n");
		return 0;
    }

	char *arr[] = {"cat", "data.txt", 0};
	execvp("cat", arr);

	exit(0);
	// pipe setup
	int parent[2];

	pipe(parent);

	if(fork() == 0){
		// child

		dup2(parent[1], 1);

		close(parent[0]);
		close(parent[1]);

		//execvp(argv[1], NULL); // replaces curr process w/ command process
		
		execvp("cat", arr);

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


