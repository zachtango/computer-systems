//ask the user to keep inputting the names and write all the names 
//to a binary file.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//let us assume the filename comes as the argument
//
#define MAXLEN 1000
char buffer[MAXLEN];

int main(int argc, char *argv[]) {
	int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	
	//after reading each name from the user, write it to file.
	//consider whole line!
	while (fgets(buffer, MAXLEN, stdin)) {
		char *cp = strchr(buffer, '\n');
		if (cp)
			*cp = '\0';
		write(fd, buffer, strlen(buffer)+1);
	}
	close(fd);
}

