#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE_LEN 100
#define MAX_OPERAND_LEN 100

int main(){
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
	char fileName[] = "data.txt";

	FILE *fp;
	char line[MAX_LINE_LEN];
	
	fp = fopen(fileName, "r");

	if(fp == NULL || fgets(line, MAX_LINE_LEN, fp) == NULL){
		printf("error\n");
		return -1;
	}

	int numOperators = (strlen(line) - 1 - 1) / 4;  
	int numOperands = numOperators + 1;
	int operands[numOperands];

	for(int i = 0; i < numOperands; i++){
		fscanf(fp, "%d", &operands[i]);

		printf("%d ", operands[i]);
	}
	printf("\n");

	// pipes
	int parent[2];
	int op2[numOperators][2]; // stream 3
	int op1[numOperators][2]; // going to dup to stdin and stdout

	pipe(parent);

	for(int i = 0; i < numOperators; i++){
		pipe(op2[i]);
		pipe(op1[i]);

		//close(op2[i][1]); // don't need write of op2

		write(op2[i][1], &operands[i + 1], sizeof(int));
		//printf("wrote to %d w value %d\n", i, operands[i + 1]);
	}
	
	write(op1[0][1], &operands[0], sizeof(int)); // write first operand

	for(int i = 0; i < numOperators; i++){
		if(fork() == 0){
			// in child

			//dup2(op1[i][0], 1); // stdin
			//dup2(op1[i][1], 0); // stdout

			// read a and 
			int a, b;
			
			printf("i: %d\n", i);
			read(op1[i][0], &a, sizeof(int));
			read(op2[i][0], &b, sizeof(int));

			printf("%d %d\n", a, b);

			int c;

			c = a + b;
			printf("c: %d\n", c);

			if(i + 1 < numOperators){ // write to input of next operation
				write(op1[i + 1][1], &c, sizeof(int));
			} else{
				write(parent[1], &c, sizeof(int));
			}

			exit(0);
		}
	}
	
	while(wait(NULL) > 0);
		
	int ans;

	read(parent[0], &ans, sizeof(int));

	printf("ans: %d\n", ans);

	return 0;
}
