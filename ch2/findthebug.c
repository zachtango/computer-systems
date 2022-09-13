#include <stdio.h>
// warning: buggy code
float sum_elements(float a[], unsigned length){
    int i;
    float result = 0;
    
    for(i = 0; i <= length - 1; i++)
        // the bug is in length - 1, because length is unsigned, 
        // length - 1 goes to umax because length is unsigned --> unsigned arithmetic
        // the comparison <= is also an unsigned comparison, so anything is less than umax --> attempts to access invalid elements of the array
        // change length to type int to fix the code
        result += a[i];

    return result;

}


int main(){

    float a[0];

    float ans = sum_elements(a, 0);
    /* when sum elements ran with argument length 0, this code
        should return 0.0; however, it encounters a memory error

        why? answer up above

    */

    printf("%f", ans);

    return 0;
}