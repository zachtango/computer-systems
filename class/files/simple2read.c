#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	char name[100] = "";
	int age = 0;
	double gpa = 0;

	int fd = open("simple2.out", O_RDONLY);
	//write null char at end of string too.
	int n = 0;
	read(fd, &n, sizeof(int)); //read the length of name
	read(fd, name, n); //read the whole name, since we know the length
	read(fd, &age, sizeof(int));
	read(fd, &gpa, sizeof(double));
	printf("%s: %d %lf\n", name, age, gpa);
	close(fd);
}

