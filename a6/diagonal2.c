/*
 *	Zach Tang CS 3377
 *	zxt200009
 *
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define LINESIZE 16

//use one command line argument
int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: diagonal2 <textstring>\n");
		return -1;
	}
	
	//create a file so that 16 rows of empty will appear with od -c command
	int fd = open("diagonal2.out", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	char space = ' ';
	for(int line = 0; line < ((argc - 1) * 16); line++)
		for(int column = 0; column < LINESIZE; column++)
			write(fd, &space, 1);

	int offset = 0;
	int left = 1;

	for(int i = 1; i < argc; i++){
		//Each line of od outputs 16 characters 
		//So, to make the output diagonal2, we will use 0, 17, 34, ....
		
		int n = strlen(argv[i]);
		if(left){

			for(int j = 0; j < n; j++) {
				lseek(fd, (LINESIZE + 1) * j + offset, SEEK_SET);
				write(fd, &argv[i][j], 1);
			}
			
			left = 0;
			offset += 272;
		} else {
			
			for(int j = 0; j < n; j++) {
				lseek(fd, (LINESIZE  - 1) * j - 1 + offset, SEEK_SET);
				write(fd, &argv[i][j], 1);
			}

			left = 1;
			
			offset += 240;
		}
	}	
			
	
	close(fd);
	puts("diagonal2.out has been created. Use od -c diagonal2.out to see the contents.");
}
