// C Program for Message Queue (Writer Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
  
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAX];
} message;
  
int main(int argc, char *argv[])
{
    key_t key;
    int msgid;
  
    // ftok to generate unique key
    key = ftok(getenv("HOME"), 1);
  
  	//fill the data in struct before sending
    message.mesg_type = 1;
    message.mesg_text[0] = NULL;
      
    // msgget creates a message queue
    // and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);
    printf("Send Key %d Msgid %d\n", key, msgid);

    int key2 = ftok(getenv("HOME"), 2);
    int msgid2 = msgget(key2, 0666 | IPC_CREAT);

    message.mesg_text = "Start";
    msgsnd(msgid, &message, sizeof(message), 0);

    while(1){
      fgets(message.mesg_text, MAX, stdin);

      msgrcv(msgid2, &message, sizeof(message), 1, 0);
      printf("Data Received is : %s \n", message.mesg_text);

      // msgsnd to send message
      msgsnd(msgid, &message, sizeof(message), 0);

      // display the message
      printf("Data send is : %s\n", message.mesg_text);

      printf("Recv Key %d Msgid %d\n", key2, msgid2);
    }

    msgctl(msgid2, IPC_RMID, NULL);

    return 0;
}
