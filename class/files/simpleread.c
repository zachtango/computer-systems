//ask the user for full name, age, gpa
//write them to a binary file

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	// printf("%d %d %d %d %d\n", sizeof(int), sizeof(long), sizeof(long long),
		// sizeof(float), sizeof(double));
	char name[100] = "";
	int age = 0;
	double gpa = 0;

	int fd = open("simple.out", O_RDONLY);
	
	//we have no idea how long the name is.
	//so read one char at a time, until we see \0
	int i=0;
	do {
		read(fd, &name[i], 1);
	} while (name[i++] != '\0');

	read(fd, &age, sizeof(int));
	read(fd, &gpa, sizeof(double));
	printf("%s: %d %lf\n", name, age, gpa);
	close(fd);
}

