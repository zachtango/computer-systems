#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BLOCK_SIZE 4096

uint32_t n, m, blocksPerThread;
FILE *fp;

uint32_t jenkinsHash(const uint8_t* key, size_t len){
    size_t i = 0;
    uint32_t hash = 0;
    while(i != len){
        hash += key[i];
        i += 1;

        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

uint32_t numDigits(uint32_t num){
    uint32_t counter = 0;
    
    while(num > 0){
        counter += 1;
        num /= 10;
    }

    return counter;
}

/*
    file split into n blocks
    m threads
    n is divisible by m

    block size = 4096 bytes --> n = file size / 4096
    num_threads given by param of executable

    htree filename numThreads

*/


/*
    file split into n blocks
    tree of threads
    leaf thread computes hash value of n / m consecutive blocks assigned to it
    interior threads take hash values returned from children and combine them w/ hash value of blocks the interior computes
        hash values combined to form a string computed hash value + left child hash + right child hash
        then takes hash value of concatenated str and returns val to parent
    
    each thread assigned a number when it's created
    number of root thread is 0
        number of left child is 2 i + 1 and number of right child is r i + 2

    thread with num j corresponds to n / m blocks starting from j * n / m

*/

void *hash( void *ptr )
{
	int i = (int) ptr;

	// if (i >= m)  // not a node (base case)
	// 	return "";

    int left = 2 * i + 1;
    int right = left + 1;

    pthread_t thread1, thread2;

    // calc child hashes
    if(left < m) pthread_create(&thread1, NULL, hash, (void *) (2 * i + 1));
    if(right < m) pthread_create(&thread2, NULL, hash, (void *) (2 * i + 2));

    // calc assigned hash (i --> from i * n / m to i * n / m + n / m)
    uint8_t key[blocksPerThread];
    
    // read n / m blocks into key
    fseek(fp, i * blocksPerThread * BLOCK_SIZE, SEEK_SET);
    fread(key, 1, blocksPerThread * BLOCK_SIZE, fp);

    // compute hash for key
    uint32_t h = jenkinsHash(key, blocksPerThread);

    char *H;
    sprintf(H, "%zu", h);

    char *leftH = "", *rightH = "";

    if(left < m) pthread_join(thread1, &leftH);
    if(right < m) pthread_join(thread2, &rightH); 

    char *conc = malloc(strlen(H) + strlen(leftH) + strlen(rightH) + 1);
    strcpy(conc, H);
    strcat(conc, leftH);
    strcat(conc, rightH);

    // compute final hash
    h = jenkinsHash(conc, strlen(conc));

    return (void *) h;
}

int main(int argc, char *argv[]) {	
	if (argc != 3) {
		puts("Usage: htell <filename> <num_threads>");
		exit(1);
	}

    fp = fopen(argv[1], "r");
    if(fp == NULL){
        printf("Can't open source file.\n");
        exit(1);
    }

    struct stat st;
    int fd = fileno(fp);
    fstat(fd, &st);
    size_t size = st.st_size;

    n = size / BLOCK_SIZE;    
    m = atoi(argv[2]);
    blocksPerThread = n / m;

    uint32_t h = (uint32_t) hash(0);

    return 0;
}