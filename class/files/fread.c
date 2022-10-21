#include <stdio.h>
#include <stdlib.h>

//we have no idea how many values are there
//in the file, let us start with huge array!
#define MAXITEMS 1000000
int numbers[MAXITEMS];

int main() {
	//read all the numbers stored in the file
	//and print them out
	FILE *fp = fopen("output.dat", "rb");
	int n = fread(numbers, sizeof(int), MAXITEMS, fp);
	for(int i=0; i<n; i++)
		printf("%d ", numbers[i]);
	putchar('\n');
}
