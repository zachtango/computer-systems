#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE_LEN 100
#define MAX_OPERAND_LEN 100

int main(int argc, char *argv[]){
	/*
		a * b + c

		need 2 pipes (1 pipe per operator)

		first pipe input will accept a and b

		second pipe will accept output of first pipe and c

		answer will be in output of second pipe

		for an operation:
			read from input pipe
				input will be into buffer of char type
				a space b
			extract a and b from buffer

			perform operation

			output b as char type into output pipe

		
		numOperations = 2
		op2[numOperations][2] // stream 3
		op1[numOperations][2]

		for i in (0, numOperations):
			pipe(op2[i]) // gives op2 stream 3
			pipe(op1[i]) // gives op1 stream greater than 3 --> will dup to stream 0 and 1

			close(op2[i][1]) // don't need write pipe of op2

		for i in (0, numOperations):
			if fork() == 0:
				// we're in the child here

				dup2(op1[i][0], 0) map read to stdin of child
				dup2(op1[i][1], 1) map read to stdout of child

				read a from op1[i][0]
				read b from op2[i][0]

				c = a operation b

				write c to op1[i][1] to write to stdout of child
				
				if i + 1 < numOperations:
					write to op2[i + 1][0] to write to input of next operation

				exit()
	*/

	/*
		retrieving data

		get the whole first line into a buffer

		iterate through this buffer and count the number of operators

		w/ the num of operators you know the num of operands will be num operators + 1
	*/

	if(argc != 2){
		printf("calculator <arg>\n");
		return 0;
	}

	char *fileName = argv[1];

	FILE *fp;
	char line[MAX_LINE_LEN];
	
	fp = fopen(fileName, "r");

	if(fp == NULL || fgets(line, MAX_LINE_LEN, fp) == NULL){
		printf("error\n");
		return -1;
	}

	int numOperators = (strlen(line) - 1 - 1) / 4;  
	int numOperands = numOperators + 1;
	char operators[numOperators];
	int operands[numOperands];

	int counter = 0;
	// extr operators
	for(int i = 0; i < strlen(line); i++){
		char c = line[i];

		if(c == '+' || c == '-' || c == '*' || c == '/'){
			operators[counter] = c;
			counter += 1;
		}
	}

	while(fscanf(fp, "%d", &operands[0]) != EOF){
		// extr operands
		for(int i = 1; i < numOperands; i++){
			fscanf(fp, "%d", &operands[i]);

			// printf("%d ", operands[i]);
		}

		// pipe setup
		int parent[2];
		int op1[numOperators][2]; // going to dup to stdin and stdout
		int op2[numOperators][2]; // stream 3

		pipe(parent);

		for(int i = 0; i < numOperators; i++){
			pipe(op1[i]);
			pipe(op2[i]);

			// printf("%d: %d\n", i, op1[i][1]);
		}

		// pipe set up for children
		for(int i = 0; i < numOperators; i++){
			char op = operators[i];
			if(fork() == 0){
				// in child
				
				// stdin
				dup2(op1[i][0], 0);
				
				// stdout
				if(i + 1 < numOperators){
					dup2(op1[i + 1][1], 1);
				} else{
					dup2(parent[1], 1);
				}


				// stream 3
				dup2(op2[i][0], 3);
			
				// clean up pipes
				for(int j = 0; j < numOperators; j++){
					close(op1[j][0]);
					close(op1[j][1]); // don't write to this
					close(op2[j][0]);	
					close(op2[j][1]); // dont write to this
				}
				close(parent[0]); // don't read from this
				close(parent[1]);

				if(op == '+')
					execvp("./add", NULL);
				else if(op == '-')
					execvp("./subtract", NULL);
				else if(op == '*')
					execvp("./multiply", NULL);
				else 
					execvp("./divide", NULL);

				exit(0);
			}

		}
		
		close(parent[1]); // dont write to parent pipe

		// input operands into pipes
		write(op1[0][1], &operands[0], sizeof(int));

		for(int i = 0; i < numOperators; i++){
			close(op1[i][0]); // don't read from children pipes
			close(op2[i][0]); 

			write(op2[i][1], &operands[i + 1], sizeof(int));
			close(op2[i][1]);
			close(op1[i][1]);
		}

		while(wait(NULL) > 0);
			
		int ans;

		read(parent[0], &ans, sizeof(int));
		close(parent[0]);
		printf("%d\n", ans);
	}

	return 0;
}


