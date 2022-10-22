#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

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
	char fileName[] = "data2.txt";

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

	int op2[numOperators][2]; // stream 3
	int op1[numOperators][2]; // going to dup to stdin and stdout

	for(int i = 0; i < numOperators; i++){
		pipe(op2[i]);
		pipe(op1[i]);

		close(op2[i][1]); // don't need write of op2 
	}

	for(int i = 0; i < numOperators; i++){
		if(fork() == 0){
			// in child

			dup2(op1[i][0], 0);
			dup2(op1[i][1], 1);

			// read a and b
			char buffer[MAX_OPERAND_LEN];
			int a, b;

			read(op1[i][0], buffer, MAX_OPERAND_LEN);

			a = atoi(buffer);

			read(op2[i][0], buffer, MAX_OPERAND_LEN);

			b = atoi(buffer);

			int c;

			c = a + b;

			sprintf(buffer, "%d", c);

			write(op1[i][1], buffer, MAX_OPERAND_LEN); // write to stdout

			if(i + 1 < numOperators){ // write to input of next operation
				write(op2[i + 1][0], buffer, MAX_OPERAND_LEN);
			}

			exit();
		}
	}

	char res[MAX_OPERAND_LEN];

	read(op1[numOperators - 1][0], res, MAX_OPERAND_LEN);

	printf("%s\n", res);

	return 0;
}
