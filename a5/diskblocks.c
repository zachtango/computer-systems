/*
 * zxt200009 CS3377.005 Zach Tang
 * diskblocks.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int NUM_ARGS = 1;

int main(int argc, char*argv[]){
		
	if(argc - 1 > NUM_ARGS){
		printf("Too many arguments. Expected %d arguments\n", NUM_ARGS);
		return 0;
	} else if(argc - 1 < NUM_ARGS){
		printf("Too few arguments. Expected %d arguments\n", NUM_ARGS);
		return 0;
	}

    int kb = atoi(argv[1]);
    
    int storageBlocks = kb / 8;
    int overheadBlocks = ceil( (kb % 8) / 8.0 );

    printf("%d %d", storageBlocks, overheadBlocks);

    return 0;
}