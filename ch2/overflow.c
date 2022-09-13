#include <stdio.h>

// determines whether arugments can be added w/o overflow
int uadd_ok(unsigned x, unsigned y){
    unsigned sum = x + y;
    printf("%u\n", sum);
    if(x + y < x || x + y < y){
        return 0;
    }

    return 1;
}


int main(){

    int overflow = uadd_ok(__UINT32_MAX__, 1);
    int ok = uadd_ok(0, 0);

    printf("%d %d", overflow, ok);

    return 0;
}