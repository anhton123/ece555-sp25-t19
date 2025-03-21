#include <stdio.h>

__global__ void addArrays(int *a, int *b, int *c, int size) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < size) {
        c[idx] = a[idx] + b[idx];
    }
}

int main() {
    int size = 10;
    int bytes = size * sizeof(int);

    int h_a[10], h_b[10], h_c[10];
    for (int i = 0; i < size; i++) {
        h_a[i] = i;
        h_b[i] = i * 2;
    }

    int *d_a, *d_b, *d_c;
    cudaMalloc((void**)&d_a, bytes);
    cudaMalloc((void**)&d_b, bytes);
    cudaMalloc((void**)&d_c, bytes);

    cudaMemcpy(d_a, h_a, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, bytes, cudaMemcpyHostToDevice);

    int threadsPerBlock = 256;
    int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;
    addArrays<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, size);

    cudaMemcpy(h_c, d_c, bytes, cudaMemcpyDeviceToHost);

    printf("Result: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", h_c[i]);
    }
    printf("\n");

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    
    return 0;
}

