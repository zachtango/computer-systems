#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXLEN 1000
char buffer[MAXLEN];

int main(int argc, char *argv[]) {
	int fd = open(argv[1], O_RDONLY);
	
	//keep on reading until EOF
	while (1) {
		memset(buffer, '\0', MAXLEN);
		int n=0;
		//read length of string
		if (read(fd, &n, sizeof(int)) != sizeof(int))
			break;
		//read the actual string contents
		read(fd, buffer, n);
		// buffer[n] = '\0';
		puts(buffer);
	}
	close(fd);
}

