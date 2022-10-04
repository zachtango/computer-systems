#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage: mathtable <number>\n");
		exit(1);
	}

	int num = atoi(argv[1]);

	for(int i = 1; i <= 10; i++){
		if(!fork()){
			printf("%d * %d = %d\n", num, i, num * num);

			return;
		}
	}

	return 0;
}
