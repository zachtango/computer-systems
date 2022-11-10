/*
 * read dictionary file to array of words & get ready to play the hangman!
if you are using fgets() to read the word
	  cptr = strchr(line, '\n');
	  if (cptr)
	  	*cptr = '\0';
 However, since we are dealing with words, you can use fscanf(...,"%s", ...) instead!

 * wait for a request to come in (client specifies unique pipename)
 * select a random word using rand()
 * fork() to create a child to handle this client! (dedicated server process for that client)
 * fork() enables games to proceed in parallel. Parent returns to wait for new client requests
 * respond with another unique server-side pipename 
 *
 * send a bunch of stars (indicating the word length)
 * for each guess the client sends in, respond with a message 
 * and send updated display word.
 *
 * Whenever you send strings through named pipes, 
 * terminate with '\n' and also do fflush() for that data to make it to other side without getting stuck
 * in buffers.
 *
 * open public fifo
 * while (fgets()) {
 * }
 *
 *


srand(....);

wait for a client connection
rand() to select a word
fork()
	child process:
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

//read all the quotes from quotes.txt
//when client asks for a motivational quote, select one randomly and send it out.

#define MAXWORDS 10000
#define MAXLEN 1000

int hangman(FILE*, FILE*, char*);

char *words[MAXWORDS];
int numWords = 0;

int main() {
	char line[MAXLEN];

	FILE *fp = fopen("words.txt", "r");
	if (!fp) {
		puts("words.txt cannot be opened for reading.");
		exit(1);
	}

	int i = 0;

	//read one line at a time, allocate memory, then copy the line into array
	while (fgets(line, MAXLEN, fp)) {
		char *c = line + strlen(line) - 1;
		if(*c == '\n'){
			*c = '\0'; // remove \n ending
		}

		words[i] = (char *) malloc (strlen(line) + 1);
		strcpy(words[i], line);
		i++;
	}

	numWords = i;
	printf("%d words were read.\n", numWords);

	srand(getpid() + time(NULL) + getuid());
	
	//create a named pipes to read client's requests
	char filename[MAXLEN];
	sprintf(filename, "/tmp/%s-%d", getenv("USER"), getpid());
	mkfifo(filename, 0600);
	chmod(filename, 0622);
	printf("Send your requests to %s\n", filename);

	while (1) {
		FILE *fp = fopen(filename, "r");
		if (!fp) {
			printf("FIFO %s cannot be opened for reading.\n", filename);
			exit(2);
		}
		printf("Opened %s to read...\n", filename);

		//wait for clients' requests
		while (fgets(line, MAXLEN, fp)) {

			char *cptr = strchr(line, '\n');
			if (cptr) 
				*cptr = '\0';
	
			//create a child to work with this client
			if (fork() == 0) {
				FILE *clientfp = fopen(line, "w");
				//create and send new server fifo to the client
				//for private one-on-one communcations
				char serverfifo[MAXLEN];
				sprintf(serverfifo, "/tmp/%s-%d", getenv("USER"), getpid());
				mkfifo(serverfifo, 0600);
				chmod(serverfifo, 0622);

				// sending unique pipename to the client to start the hangeman game
				fprintf(clientfp, "%s\n", serverfifo);
				fflush(clientfp);

				FILE *serverfp = fopen(serverfifo, "r");
				
				hangman(serverfp, clientfp, words[0]);

				exit(0);
			}
		}

		fclose(fp);
	}
}

int hangman(FILE* serverfp, FILE* clientfp, char* word){
	
	int n = strlen(word);
	char* display;

	strcpy(display, word);

	for(int i = 0; i < n; i++){
		display[i] = '*';
	}

	int hidden = n;
	char buffer[MAXLEN];
	int wrongGuesses = 0;

	char guess;

	while(1){
		fprintf(clientfp, "Enter a letter in word %s > \n", display);
		fflush(clientfp);

		fscanf(serverfp, "%s", &guess);
		printf("%s", guess);
		// if(isalpha(guess)){
		// 	// int wrong = 1;

		// 	printf("test\n");
		// 	// for(int i = 0; i < n; i++){
		// 	// 	if(guess == word[i]){
		// 	// 		wrong = 0;

		// 	// 		if(guess == display[i]){
		// 	// 			printf("%c already\n", guess);
		// 	// 			//fprintf(clientfp, "%c is already in the word.\n", guess);
		// 	// 			//fflush(clientfp);
		// 	// 			break;
		// 	// 		}

		// 	// 		// correct guess
		// 	// 		display[i] = guess;
		// 	// 		hidden -= 1;
		// 	// 	}
		// 	// }
			

		// 	// if(wrong){
		// 	// 	printf("wrng\n");
		// 	// 	//fprintf(clientfp, "%c is not in the word.\n", guess);
		// 	// 	//fflush(clientfp);
		// 	// 	wrongGuesses += 1;
		// 	// }
		// }
		
	}

	fprintf(clientfp, "The word is %s.\n", word);
	fprintf(clientfp, "You missed %d times.\n", wrongGuesses);
	fflush(clientfp);
}
