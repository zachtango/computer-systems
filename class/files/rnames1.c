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
		int i=0;
		do {
			if (read(fd, &buffer[i], 1) != 1)
				exit(0);
		} while (buffer[i++] != '\0');
		puts(buffer);
	}
}

