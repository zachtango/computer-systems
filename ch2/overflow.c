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

int isLessOrEqual(int x, int y) {
  // x <= y if x - y <= 0 --> x - y (not >) 0
  y = ~y + 1; // negate y

  int s = 1 << 31; // 1000...000, nonegative anded with s --> 0, !!(negative anded s) is 1

//   printf("%d", s & (x + y));

  return !!( !(x + y) | (s & (x + y)) ); // is 0 or is negative 
  
}

int main(){
    // 0x30 <= x <= 0x39
    for(int i = 0x30 - 1000; i <= 0x39 + 1000; i++){
        // isAsciiDigit(i);
        // isLessOrEqual(i, i + 1);
        printf("%d", isLessOrEqual(i + 3, i + 2));

    }
    return 0;
}