#include <stdio.h>

void addArrays(int *a, int *b, int *c, int size) {
    for (int i = 0; i < size; i++) {
        c[i] = a[i] + b[i];
    }
}

int main() {
    int size = 10;
    int h_a[10], h_b[10], h_c[10];
    
    for (int i = 0; i < size; i++) {
        h_a[i] = i;
        h_b[i] = i * 2;
    }

    addArrays(h_a, h_b, h_c, size);

    printf("Result: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", h_c[i]);
    }
    printf("\n");
    
    return 0;
}
