//get user input for count (N)
//allocate array of N integers
//get user input for that array contents & store in the array
//dump the whole array into binary file output.dat
//using fopen, fwrite, fclose

#include <stdio.h>
#include <stdlib.h>

int main() {
	int n = 0;
	printf("How many integers? ");
	scanf("%d", &n);

	int *numbers = malloc(n * sizeof(int));
	for(int i=0; i<n; i++) {
		printf("Enter next number: ");
		scanf("%d", &numbers[i]);
	}

	//read n numbers into this array
	FILE *fp = fopen("output.dat", "wb");
	fwrite(numbers, sizeof(int), n, fp);
	fclose(fp);
}
