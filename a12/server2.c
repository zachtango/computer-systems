// C Program for Message Queue (Reader Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define MAX 100

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAX];
} message;

char *words[100000];
int numWords;

int hangman(int msgidRead, int msgidWrite, char *word);

int main()
{
	char line[MAX];

	FILE *fp = fopen("dictionary.txt", "r");
	if (!fp) {
		puts("dictionary.txt cannot be opened for reading.");
		exit(1);
	}

	int i = 0;

	//read one line at a time, allocate memory, then copy the line into array
	while (fgets(line, MAX, fp)) {
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
	
	fclose(fp);

	key_t key;
	int msgid;

    // ftok to generate unique key - main server
    key = ftok(getenv("HOME"), 1);
  
  	int counter = 2;

	while (1) {
    	// msgget creates a message queue and returns identifier
    	msgid = msgget(key, 0666 | IPC_CREAT);
  		printf("Key %d Msgid %d\n", key, msgid);

    	// msgrcv to receive message
    	msgrcv(msgid, &message, sizeof(message), 1, 0);
    	// to destroy the message queue
    	msgctl(msgid, IPC_RMID, NULL);
  
    	// display the message
    	printf("Data Received is : %s \n", message.mesg_text);
		int clientPid = atoi(message.mesg_text);

		if(fork() == 0){
			//use clientPid to come up with key & msgid to respond
			int key2 = ftok(getenv("HOME"), clientPid);
			int msgid2 = msgget(key2, 0666 | IPC_CREAT); // for writing
			
			message.mesg_text[0] = counter;
			msgsnd(msgid2, &message, sizeof(message), 0);

			int key3 = ftok(getenv("HOME"), counter);
			int msgid3 = msgget(key3, 0666 | IPC_CREAT); // for reading

			counter += 1;

			while(1){
				// hangman goes here
				char* word = "test";
				hangman(msgid2, msgid3, word);
			}
		}
  	} 
  
    return 0;
}


int hangman(int msgidRead, int msgidWrite, char *word){
	int n = strlen(word);
	char display[n + 1];

	for(int i = 0; i < n; i++)
		display[i] = '*';

	display[n] = '\0';

	int hidden = n;
	int wrongGuesses = 0;

	while(hidden != 0){
		sprintf(message.mesg_text, "(Guess) Enter a letter in the word %s > ", display);
		msgsnd(msgidWrite, &message, sizeof(message), 0);
		printf("Data Sent is : %s\n", message.mesg_text);

		msgrcv(msgidRead, &message, sizeof(message), 1, 0);

		printf("Data Received is : %s \n", message.mesg_text);

		char guess = message.mesg_text[0];

		int wrong = 1;
		for(int i = 0; i < n; i++){
			if(guess == word[i]){
				wrong = 0;

				if(guess == display[i]){
					sprintf(message.mesg_text, "%c is already in the word.\n\0", guess);
					break;
				}

				strcpy(message.mesg_text, "Good guess!\n\0");
				display[i] = guess;
				hidden -= 1;
			}
		}

		if(wrong){
			sprintf(message.mesg_text, "%c is not in the word.\n\0", guess);
			wrongGuesses += 1;
		}

		msgsnd(msgidWrite, &message, sizeof(message), 0);

	}
}


