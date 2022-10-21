/*
 * zxt200009 CS3377.005 Zach Tang
 * diskblocks.c
 */

#include <stdio.h>
#include <stdlib.h>

const int NUM_ARGS = 1;

int main(int argc, char*argv[]){
		
	if(argc - 1 > NUM_ARGS){
		printf("Too many arguments. Expected %d arguments\n", NUM_ARGS);
		return 0;
	} else if(argc - 1 < NUM_ARGS){
		printf("Too few arguments. Expected %d arguments\n", NUM_ARGS);
		return 0;
	}

    unsigned long long kb = atoll(argv[1]);
    
    unsigned long long storageBlocks = (kb + 7) / 8;
    unsigned long long overheadBlocks = 0;


	if(storageBlocks <= 12){	
		printf("%lu %lu\n", storageBlocks, overheadBlocks);
		return 0;
	}

	storageBlocks -= 12;
	overheadBlocks += 1;

	if(storageBlocks <= 2048){	
		printf("%lu %lu\n", storageBlocks, overheadBlocks);
		return 0;
	}

	storageBlocks -= 2048;
	overheadBlocks += 1;

	if(storageBlocks <= 2048 * 2048){
		overheadBlocks += storageBlocks / 2048;
		storageBlocks %= 2048;
		if(storageBlocks > 0){
			overheadBlocks += 1;
		}

		printf("%lu %lu\n", storageBlocks, overheadBlocks);
		return 0;
	}

	storageBlocks -= 2048 * 2048;
	overheadBlocks += 2048;

	if(storageBlocks <= 2048L * 2048 * 2048){
		overheadBlocks += storageBlocks / 2048 / 2048;
		overheadBlocks += storageBlocks / 2048;

		storageBlocks %= (2048 * 2048);
		storageBlocks %= 2048;
		
		overheadBlocks += 1;
		if(storageBlocks > 0){
			overheadBlocks += 2;
		}

		printf("%lu %lu\n", storageBlocks, overheadBlocks);	
	} else{
		
		printf("-1");
	}


    return 0;
}


