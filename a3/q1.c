/**

Andrew Atkinson
3229071
Assignment 3 Question 1

**/


#include <math.h>
#include <stdio.h>
#include <time.h>

#define N 16

__global__
void area( int *a, int *b, int *interval, double *h, double *area, double *x)
{
	int i;
	for(i = interval*blockIdx.x; i < interval*blockIdx.x+interval; i++){
		x[blockIdx.x] = a + i*h;
		area[blockIdx.x] = area[blockIdx.x] + (1 + x[blockIdx.x] + sin(2*x[blockIdx.x]));
	}
}

int main( int argc, char *argv[]){

	int a = atoi(argv[1]), b = atoi(argv[2]), i;
	double h;
	sscanf(argv[3], "%lf", &h);

	printf("h = %Lf\n", h);

	double n = (b-a)/h;
	double x;
	double area = 0;

	int *d_a, *d_b, *d_interval;
	double *d_h, *d_area, *d_x;

	cudaMalloc((void **) &d_a, N*sizeof(int));
	cudaMalloc((void **) &d_b, N*sizeof(int));
	cudaMalloc((void **) &d_interval, N*sizeof(int));

	cudaMalloc((void **) &d_h, N*sizeof(double));
	cudaMalloc((void **) &d_area, N*sizeof(double));
	cudaMalloc((void **) &d_x, N*sizeof(double));

	for( i = 0; i < n; i++){
		x = a + i*h;
		area = area + (1 + x + sin(2*x));
	}

	area = area*h;

	printf("\narea = %lf\na = %d, b = %d, h = 1/%lf\n", area, a, b, 1/h);
}
