#include <cstdlib>
#include <stdio.h>

inline unsigned int cdiv(unsigned int a, unsigned int b) {
  return (a + b - 1) / b;
}

__global__ void vecAddKernel(float *A, float *B, float *C, int n) {
  int i = threadIdx.x + blockDim.x * blockIdx.x;
  if (i < n) {
    C[i] = A[i] + B[i];
  }
}

void vecAdd(float *A_h, float *B_h, float *C_h, int n) {
  // allocate memory on device to init *_d variables
  int size = n * sizeof(float);
  float *A_d, *B_d, *C_d;
  cudaMalloc((void **)&A_d, size);
  cudaMalloc((void **)&B_d, size);
  cudaMalloc((void **)&C_d, size);

  if (!A_d || !B_d || !C_d) {
    printf("CUDA memory allocation failed\n");
    exit(1);
  }
  // copy from host _h to device _d
  cudaMemcpy(A_d, A_h, size, cudaMemcpyHostToDevice);
  cudaMemcpy(B_d, B_h, size, cudaMemcpyHostToDevice);
  // compute
  vecAddKernel<<<cdiv(n, 1024), 1024>>>(A_d, B_d, C_d, n);
  // copy from device to host
  cudaMemcpy(C_h, C_d, size, cudaMemcpyDeviceToHost);
  // free allocated memory on device
  cudaFree(A_d);
  cudaFree(B_d);
  cudaFree(C_d);
}

int main() {
  //   float A_h[] = {1, 2, 3};
  //   float B_h[] = {9, 8, 7};
  //   float C_h[3];
  const int size = 100000;
  float A_h[size];
  float B_h[size];
  float C_h[size];
  for (int i = 0; i < size; i++) {
    A_h[i] = (float)rand() / RAND_MAX;
    B_h[i] = (float)rand() / RAND_MAX;
  }
  cudaEvent_t start, end;
  cudaEventCreate(&start);
  cudaEventCreate(&end);
  cudaEventRecord(start, 0);
  vecAdd(A_h, B_h, C_h, size);
  cudaEventRecord(end, 0);
  cudaEventSynchronize(end);
  float milliseconds = 0;
  cudaEventElapsedTime(&milliseconds, start, end);
  printf("Kernel execution time: %fms\n", milliseconds);
  cudaEventDestroy(start);
  cudaEventDestroy(end);
}
