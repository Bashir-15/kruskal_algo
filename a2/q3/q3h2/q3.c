#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

int master_program( int id, int size, int interval,
				int a, int b, int n,
				double h, double area);

int worker_program( int id, int size, int interval,
				int a, int b, int n,
				double h, double area);

int main( int argc, char *argv[]){

	clock_t start = clock(), finish;

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
	interval = 10000;

	if(id == 0){
		master_program( id, size, interval, a, b, n, h, area);
	}
	else{
		worker_program( id, size, interval, a, b, n, h, area);
	}

	MPI_Finalize();

	finish = clock();
	double time = (double)(finish - start)/CLOCKS_PER_SEC;
}

int master_program( int id, int size, int interval,
				int a, int b, int n,
				double h, double area) {

	double retArea, x;
	int nextRange = 0, working = 0, currId, steps = n/interval;
	MPI_Status stat;

	for (int p = 1; p < size; p++) {
		MPI_Send(&nextRange, 1, MPI_INT, p, p, MPI_COMM_WORLD);
		nextRange++;
		working++;
	}

	while (	working > 0){

		MPI_Recv(&retArea,1,MPI_DOUBLE,MPI_ANY_SOURCE,MPI_ANY_TAG,
			MPI_COMM_WORLD,&stat);

		working--;
		currId = stat.MPI_SOURCE;

		if( nextRange < steps){

			MPI_Send( &nextRange,1,MPI_INT,currId,currId,
				MPI_COMM_WORLD);
			working++;
			nextRange++;
		}else{

			MPI_Send( &nextRange,1,MPI_INT,currId,0,
				MPI_COMM_WORLD);
		}

		area = area + retArea;
	}

	if(size == 1){
		for(int i = 0; i < n; i++){
			x = a + i*h;
			area = area + (1 + x + sin(2*x));
		}
	}
		

	area = area * h;

	printf("\narea = %lf\na = %d, b = %d, h = 1/%lf\n", area, a, b, 1/h);

	return 1;
}

int worker_program( int id, int size, int interval,
				int a, int b, int n,
				double h, double area) {
	
	int nextRange;
	double x;
	MPI_Status stat;
	
  	while( ((MPI_Recv(&nextRange,1,MPI_INT,0,MPI_ANY_TAG,
				MPI_COMM_WORLD,&stat)) == MPI_SUCCESS) &&
				(stat.MPI_TAG != 0)){
		area = 0;

		for(int i = nextRange*interval; i < nextRange*interval+interval; i++){
			x = a + i*h;
			area = area + (1 + x + sin(2*x));
		}

		MPI_Send(&area,1,MPI_DOUBLE,0,id,
			MPI_COMM_WORLD);
	}

	return 1;
}
