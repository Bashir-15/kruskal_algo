/**

Andrew Atkinson
3229071
Assignment 2 Question 2

**/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <float.h>

int main( int argc, char *argv[]){

	int id, size, interval, remainder;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Status stat;

	int a = atoi(argv[1]), b = atoi(argv[2]), i;
	double h = atof(argv[3]);
	int n = (b-a)/h;
	double x;
	double area = 0;
	double retArea[size];
	interval = n/size;
	if(id == size)
		remainder = n%size;

	for( i = id*interval; i < (id*interval)+interval+remainder; i++){
		x = a + i*h;
		area = area + (1 + x + sin(2*x));
		if( i%1000 == 0)
			area = area;
	}

	area = area*h;

	if(id == 0){
		for(i=1;i<size;i++) {
      			MPI_Recv(&retArea[i],1,MPI_DOUBLE,i,i,
				MPI_COMM_WORLD,&stat);
			area = area + retArea[i];
		}
	}else{
		MPI_Send(&area,1,MPI_DOUBLE,0,id,
		MPI_COMM_WORLD);
	}

	MPI_Finalize();

	if (id == 0)
		printf("\narea = %lf\na = %d, b = %d, h = 1/%lf\n", area, a, b, 1/h);
}
