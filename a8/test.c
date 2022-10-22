#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

//any argument coming in - will be considered as enabling debug output
int main(int argc, char *argv[])
{
	int op1[2], op2[2], toParent[2];
    pipe(op1);
    pipe(op2);
    pipe(toParent);

    if(fork() == 0){
        // child

        dup2(op1[0], 0);
        close(op1[0]);

        dup2(toParent[1], 1);
        close(toParent[1]);

        dup2(op2[0], 3);
        close(op2[0]);

        close(op1[1]); // don't write to this
        close(toParent[0]); // don't read from this
        close(op2[1]); // dont write to this

        execvp("./add", NULL);
    } else{
        close(op1[0]); // don't read from this
        close(op2[0]); // don't read from this
        close(toParent[1]); // don't write to this
        
        write(op1[1], 2, sizeof(int));
        write(op2[1], 2, sizeof(int));

        close(op1[1]);
        close(op2[1]);

        wait(0);

        int res;

        read(toParent[0], res, sizeof(int));

        printf("%d", res);

    }



    return 0;
}	
