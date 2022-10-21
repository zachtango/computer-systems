#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("One argument expected\n");
		return -1;
	}

	
	int num = atoi(argv[1]);
	
	int i = 1;
	
	while(i <= 100){
		printf("%d * %d = %d\n", num, i, num * i);
		
		if(fork() != 0){
			wait(NULL);
			exit(0);
		}
		
		i++;
	}
	

	return 0;
}
