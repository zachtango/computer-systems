#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

	for(int i = 0; i < 3; i++){
		fork();
		printf("test\n");

	}
	wait(NULL);

	return 0;
}

