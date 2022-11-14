#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t numDigits(uint32_t num){
    uint32_t counter = 0;
    
    while(num > 0){
        counter += 1;
        num /= 10;
    }

    return counter;
}

int main(){

    char *src1 = "a";
    char *src2 = "";
    char *src3 = "";

    char *dst = malloc(strlen(src1) + strlen(src2) + strlen(src3) + 1);
    strcpy(dst, src1);
    printf("%s\n", dst);
    strcat(dst, src2);
    printf("%s\n", dst);
    strcat(dst, src3);
    printf("%s\n", dst);

    char *dst2;
    uint32_t test = 3121231233;    
    sprintf(dst2, "%zu", test);

    printf("%d", numDigits(1946267830));

    return 0;
}