/**

Andrew Atkinson
3229071
Assignment 3 Question 1

**/

#include <math.h>
#include <stdio.h>
#include <time.h>

#define TPB 1

__global__ void findArea( int *a, double *h, double *area, double *y, double *n){

		int index = threadIdx.x + blockIdx.x * blockDim.x;
		if( index < *n){
			y[index] = (*a) + index * (*h);
			area[index] = 1 + y[index] + sin(2*y[index]);
		}
}

int main( int argc, char *argv[]){

	clock_t begin, end;
	double time_spent;

	begin = clock();

	int a = atoi(argv[1]), b = atoi(argv[2]), i, N;
	double h;
	sscanf(argv[3], "%lf", &h);
	double n = (b-a)/h;
	double area = 0;
	N = n/TPB;

	int doubleSize = N*TPB*sizeof(double);

	int *d_a;
	double *d_h, *d_area, *d_x, *d_n;
	double * newArea = (double *)malloc(doubleSize);

	cudaMalloc((void **) &d_a, sizeof(int));

	cudaMalloc((void **) &d_h, sizeof(double));
	cudaMalloc((void **) &d_area, doubleSize);
	cudaMalloc((void **) &d_x, doubleSize);
	cudaMalloc((void **) &d_n, sizeof(double));

	cudaMemcpy(d_a, &a, sizeof(int), cudaMemcpyHostToDevice);

	cudaMemcpy(d_h, &h, sizeof(double), cudaMemcpyHostToDevice);
	cudaMemcpy(d_n, &n, sizeof(double), cudaMemcpyHostToDevice);
	
	findArea<<<N,TPB>>>( d_a, d_h, d_area, d_x, d_n);

	cudaMemcpy(newArea, d_area, doubleSize, cudaMemcpyDeviceToHost);

	for(i = 0; i < N*TPB; i++){
		area = area + newArea[i];
	}

	area = area*h;

	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("\narea = %lf\na = %d, b = %d, h = 1/%lf\nexecution time = %lf\n", area, a, b, 1/h, time_spent);

	cudaFree(d_a);
	cudaFree(d_h);
	cudaFree(d_area);
	cudaFree(d_x);
	cudaFree(d_n);
}
