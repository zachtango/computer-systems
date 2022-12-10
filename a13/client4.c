#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#define MAXLEN 1000

int main(int argc, char *argv[])
{

    int sockfd = 0, n = 0;
    char recvBuff[MAXLEN];
    struct sockaddr_in serv_addr; 

    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> <port #> \n",argv[0]);
        return 1;
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
	int port = atoi(argv[2]);
    serv_addr.sin_port = htons(port); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    read(sockfd, recvBuff, sizeof(recvBuff)-1);
    printf("%s", recvBuff);

	//read a line from the user and send it to the server
	//read a line from the server and display it
	while (fgets(recvBuff, sizeof(recvBuff), stdin) > 0) {
		printf("test\n");
        write(sockfd, recvBuff, strlen(recvBuff)+1);

		if ((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0) {
        	recvBuff[n] = 0;
			puts(recvBuff);
		} else
			break;

        read(sockfd, recvBuff, sizeof(recvBuff)-1);
        printf("%s", recvBuff);
        read(sockfd, recvBuff, sizeof(recvBuff)-1);
        printf("%s", recvBuff);
	}

    return 0;
}
