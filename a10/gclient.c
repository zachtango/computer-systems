/* make up unique pipename for client
 * send it to the server (public pipe)
 * get another unique pipename from the server
 * repeat
 * 	read a line including starred word from the client pipe
 *  display that line to the user 
 *  check whether game is over?
 *  get the user's guess letter
 *  send to the server using server pipe
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXLEN 1000

int main(int argc, char *argv[]) {	
	if (argc !=2) {
		puts("Usage: qclient <server-fifo-name>");
		exit(1);
	}

	// argv[1] is the server fifo name

    char clientfifo[MAXLEN];
	sprintf(clientfifo, "/tmp/%s-%d", getenv("USER"), getpid());
	mkfifo(clientfifo, 0600);
	chmod(clientfifo, 0622);
	
	//open argv[1] for writing, send clientfifo
	FILE *fp = fopen(argv[1], "w");

	//fputs(clientfifo, fp);
	//add \n after clientfifo to avoid potential race conditions in server
	fprintf(fp, "%s\n", clientfifo);
	fclose(fp);

	//open clientfifo for reading and read the quote & print in the screen - improve your life! :-)
	FILE *clientfp = fopen(clientfifo, "r");
	
    //read the new server-fifo, then open it for writing! as serverfp
	char serverfifo[MAXLEN];
	
    //fgets(serverfifo, clientfp); -- fscanf() is better! No need to worry about \n
	fscanf(clientfp, "%s", serverfifo);
	char line[MAXLEN];
	fgets(line, MAXLEN, clientfp); //get rid of \n

	FILE *serverfp = fopen(serverfifo, "w");

    fgets(line, MAXLEN, clientfp); //get rid of \n
    line[strlen(line) - 1] = '\0';
    puts(line);

	if (fork()) {
		//parent - responsible for getting user input and sending to server
		while(1) {
			//get the user input - one char at a time and store it in guess
			fgets(line, MAXLEN, stdin);
		
			// send the char to the server
			// to avoid output getting stuck in the local buffer,
			// we need 2 things: \n and fflush()
			fprintf(serverfp, "%s\n", line);
			fflush(serverfp);
		}
    } else { 
		//child - responsible for getting responses and displaying
		while (1) {
			fgets(line, MAXLEN, clientfp);
			printf("%s", line);
		}
    }

	fclose(clientfp);

	unlink(clientfifo);
}
