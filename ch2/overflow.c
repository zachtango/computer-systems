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

// determines whether signed integers (2s complements) can be added w/o overflow
int tadd_ok(int x, int y){
    int sum = x + y;

    if(x > 0 && y > 0 && sum <= 0){
        return 1;
    } else if(x < 0 && y < 0 && sum >= 0){
        return 2;
    }

    return 0;
}

int main(){

    int overflow = tadd_ok(__INT_MAX__, __INT_MAX__);
    int overflow2 = tadd_ok(-__INT_MAX__, -__INT_MAX__);
    int ok = uadd_ok(0, 0);

    printf("%d %d %d", overflow, overflow2, ok);

    return 0;
}