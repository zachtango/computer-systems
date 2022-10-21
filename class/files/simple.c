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

	printf("Enter full name: ");
	fgets(name, 100, stdin); //get the whole line
	// if newline char is found, change it to '\0'
	char *cptr = strchr(name, '\n');
	if (cptr) //valid pointer? (cptr) means (cptr != NULL)
		*cptr = '\0';

	printf("Enter your age: ");
	scanf("%d", &age);

	printf("Enter your gpa: ");
	scanf("%lf", &gpa);

	//use system calls to write this data to binary file
	int fd = open("simple.out", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	//write null char at end of string too.
	write(fd, name, strlen(name)+1);
	write(fd, &age, sizeof(int));
	write(fd, &gpa, sizeof(double));
	close(fd);
}

