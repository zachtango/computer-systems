#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
	int fds[2];

	pipe(fds);

	char message[] = "Hello!";

	write(fds[1], message, strlen(message) + 1);

	char buffer[100] = ".......";
	int n = read(fds[0], buffer, 100);

	puts(buffer);
	printf("%d\n", n); 

	return 0;
}
