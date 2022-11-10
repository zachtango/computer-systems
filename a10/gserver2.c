
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXWORDS 100000
#define MAXLEN 100

char *words[MAXWORDS];
int numWords = 0;
int pipeCounter = 0;
char* answer;
char* covered;

int main() {
	char line[MAXLEN];

	FILE *fp = fopen("dictionary.txt", "r");
	if (!fp) {
		puts("dictionary.txt cannot be opened for reading.");
		exit(1);
	}

	int i=0;
	//read one line at a time, allocate memory, then copy the line into array
	while (fgets(line, MAXLEN, fp)) {
		words[i] = (char *) malloc (strlen(line)+1);
		strcpy(words[i], line);
		i++;
	}
	numWords = i;
	printf("%d words were read.\n", numWords);

	srand(getpid() + time(NULL) + getuid());
	
	//create a named pipes to read client's requests
	char filename[MAXLEN];
	sprintf(filename, "/tmp/%s", getenv("USER"));
	mkfifo(filename, 0600);
	chmod(filename, 0622);
	printf("Send your requests to %s\n", filename);

    fp = fopen(filename, "r");
		printf("Opened %s to read...\n", filename);
 	if (!fp) {
			printf("FIFO %s cannot be opened for reading.\n", filename);
			exit(2);
	}
		while (fgets(line, MAXLEN, fp)) { //read new client's fifo

			if (fork() > 0)
				continue;

			char *cptr = strchr(line, '\n');
				if (cptr) 
					*cptr = '\0';

			FILE *clientfp = fopen(line, "w");
			printf("%s: ", line);

			char newPipe[MAXLEN];
			sprintf(newPipe, "/tmp/%s", getenv("USER"));
			mkfifo(newPipe, 0600);
			chmod(newPipe, 0622);
	
			printf("%s: ", newPipe);
			fprintf(clientfp, "%s\n", newPipe);
			fflush(clientfp);

			FILE *serverfp = fopen(newPipe, "r");

			if (clientfp) {
				int wordNumber = rand() % numWords;
				printf("%s", words[wordNumber]);
				
				char* display;
				char* answer;
				display = (char*) malloc(20);
				answer = (char*) malloc(20);
				strcpy(display, words[wordNumber]);
				strcpy(answer, words[wordNumber]);

				for(int i = 0; i < strlen(display) - 1; i++) {
					display[i] = '*';
				}

				char prompt[] = "Enter a letter in word > ";

				while (1) {
				   fprintf(clientfp, "%s%s\n", prompt, display);
				   fflush(clientfp);

				   char response[20];
				   fgets(response, 1, serverfp);
				   printf("user input: %c\n", response[0]);
				}
			}
			fclose(clientfp);
			putchar('\n');
		}
		fclose(fp);
}
