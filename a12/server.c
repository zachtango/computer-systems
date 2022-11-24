// C Program for Message Queue (Reader Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define MAXWORDS 100000
#define MAXLEN 1000

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;
  
char *words[MAXWORDS];
int numWords = 0;

int main()
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

	fclose(fp);

	key_t key;
	int msgid;

    // ftok to generate unique key
    key = ftok(getenv("HOME"), 1);
  
  	int counter=3;

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

		//assign dedicated project #s for communication to client
		int key2 = ftok(getenv("HOME"), 2);
    	int msgid2 = msgget(key2, 0666 | IPC_CREAT);
		sprintf(message.mesg_text, "%d %d", counter, counter+1);
    	msgsnd(msgid2, &message, sizeof(message), 0);
    	printf("Data Sent is : %s\n", message.mesg_text);

		counter += 2;
  	} 
  
    return 0;
}
