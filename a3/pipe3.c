#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
	int tochild[2];
	int toparent[2];

	pipe(tochild);
	pipe(toparent);

	if(fork()){
		// parent
		char message[100] = "Hello there";
		write(tochild[1], message, strlen(message) + 1);

		read(toparent[0], message, 100);
		printf("Grevious: %s\n", message);
	} else {
		char buffer[100] = ".............";
		read(tochild[0], buffer, 100);
		printf("Kenobi: %s\n", buffer);
		
		char reply[] = "Kenobi";
		write(toparent[1], reply, strlen(reply) + 1);
	}


	return 0;
}
