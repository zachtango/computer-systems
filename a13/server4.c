#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

#define MAXWORDS 100000
#define MAXLEN 1000

char *words[MAXWORDS];
int numWords = 0;

void pexit(char *errmsg) {
	fprintf(stderr, "%s\n", errmsg);
	exit(1);
}

int main(int argc, char *argv[])
{
	char line[MAXLEN];

	FILE *fp = fopen("dictionary.txt", "r");
	if(!fp){
		puts("dictionary.txt cannot be opened for reading.");
		exit(1);
	}

	int i = 0;
	// read one line at a time, allocate memory, then copy the line
	while (fgets(line, MAXLEN, fp)) {
		char *c = line + strlen(line) - 1;
		if(*c == '\n') {
			*c = '\0';
		}
		
		words[i] = (char *) malloc (strlen(line) + 1);
		strcpy(words[i], line);
		i += 1;
	}

	numWords = i;
	printf("%d words were read\n", numWords);

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char buffer[1025];
    time_t ticks; 

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		pexit("socket() error.");
		
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buffer, '0', sizeof(buffer)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int port = 4999;
	do {
		port++;
    	serv_addr.sin_port = htons(port); 
    } while (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0);
	printf("bind() succeeds for port #%d\n", port);

    if (listen(listenfd, 10) < 0)
		pexit("listen() error.");

	int counter=0;
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
		counter++;
		printf("connected to client %d.\n", counter);
		if (fork() > 0)
			continue;

		// rand word
		srand(getpid() + time(NULL) + getuid());
		int r = (rand() * rand()) % numWords;
		while(r < 0 || r >= numWords){
			r = (rand() * rand()) % numWords;
		}
		printf("Random word %s\n", words[r]);

		hangman(connfd, words[r]);

		printf("served client %d.\n", counter);
		
        close(connfd);
		
		exit(0); //this is child server process. It is done!
     }
}

int hangman(int connfd, char* word){
	int wrongGuesses = 0;
	int m = strlen(word);
	char display[m + 1];
	printf("%s %d\n", word, m);

	for(int i = 0; i < m; i++)
		display[i] = '*';
	display[m] = '\0';
	int hidden = m;

	char sndMsg[MAXLEN];
	sprintf(sndMsg, "(Guess) Enter a letter in the word %s >", display);

	write(connfd, sndMsg, strlen(sndMsg) + 1);

	char buffer[MAXLEN];
	int n;
	//read a line from the client
	while ((n = read(connfd, buffer, sizeof(buffer) - 1)) > 0) {
		char guess = buffer[0];

		printf("char: %c\n", guess);

		printf("%s %s\n", word, display);
		int wrong = 1;
		for(int i = 0; i < m; i++){
			if(guess == word[i]){
				wrong = 0;

				if(guess == display[i]){
					strcpy(sndMsg, "  is already in the word.\n");
					sndMsg[0] = guess;
					break;
				}

				strcpy(sndMsg, "Good guess!\n");
				display[i] = guess;
				hidden -= 1;
			}
		}

		if(wrong){
			strcpy(sndMsg, "  is not in the word.\n");
			sndMsg[0] = guess;
			wrongGuesses += 1;
		}

		if(hidden == 0) break;
		
		char cat[MAXLEN];
		sprintf(cat, "(Guess) Enter a letter in the word %s >", display);
		strcat(sndMsg, cat);

		printf("%s\n", sndMsg);
		write(connfd, sndMsg, strlen(sndMsg) + 1);
	}
}
