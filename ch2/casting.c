#include <stdio.h>
/*
    casting exercise showing its issues with byte ordering
    the sizeof operator determines the num of bytes used by the object
*/
typedef unsigned char * byte_pointer;

void show_bytes(byte_pointer start, size_t len){
    int i;

    for(i = 0; i < len; i++){
        printf(" %.2x", start[i]);
    }

    printf("\n");
}

void show_int(int x){
    show_bytes((byte_pointer) &x, sizeof(int)); // cast makes compiler consider the pointer to a sequence of bytes rather than to an obj of the original data type
}

void show_float(float x){
    show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x){
    show_bytes((byte_pointer) &x, sizeof(void *));
}

void test_show_bytes(int val){
    int ival = val;
    float fval = (float) ival;
    int *pval = &ival;
    show_int(ival);
    show_float(fval);
    show_pointer(pval);

}

int main(){
    
    show_bytes("tesp", 5);
    
    return 0;
}