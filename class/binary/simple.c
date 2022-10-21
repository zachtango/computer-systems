// ask user for full name, age, gpa
// write to binary file

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
//	printf("%d %d %d %d %d\n", sizeof(int), sizeof(long), sizeof(long long), sizeof(float), sizeof(double));
	
	char name[100] = "";
	int age = 0;
	double gpa = 0;

	printf("Enter full name: ");
	fgets(name, 100, stdin); // get whole line (100 chars)
	
	printf("Enter your age: ");
	scanf("%d", &age);

	printf("Enter your gpa: ");
	scanf("%f", &gpa);

	int fd = open("simple.out", O_WRONLY | O_CREAT | O_TRUNC);
	write(fd, name, strlen(name) + 1);
	write(fd, &age, sizeof(int));
	write(fd, &gpa, sizeof(double));
	close(fd);

	return 0;
}
