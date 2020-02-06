/**

Andrew Atkinson
3229071
Assignment 3 Question 2 (b)

**/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main( int argc, char *argv[]){

	clock_t begin, end;
	double time_spent;

	begin = clock();

	int id, size, i = 0;
	double x = atof(argv[1]), tempx=1, result = x;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Status stat;

	if(!id){
		MPI_Send(&x,1,MPI_DOUBLE,id+1,id,MPI_COMM_WORLD);
		while(++i < size){
			MPI_Recv(&tempx,1,MPI_DOUBLE,i,i,MPI_COMM_WORLD,&stat);
			result+=tempx;
		}
		
	}
	else{
		double fact = 1;
		MPI_Recv(&x,1,MPI_DOUBLE,id-1,id-1,MPI_COMM_WORLD,&stat);
		for(i = 1; i <= id*2+1; i++){
			tempx*=x;
			fact*=i;
		}
		tempx/=fact;
		if(!(id%2)){
			if(id+1 < size)
				MPI_Send(&x,1,MPI_DOUBLE,id+1,id,MPI_COMM_WORLD);
			MPI_Send(&tempx,1,MPI_DOUBLE,0,id,MPI_COMM_WORLD);
		}else{
			tempx*=-1;
			if(id+1 < size)
				MPI_Send(&x,1,MPI_DOUBLE,id+1,id,MPI_COMM_WORLD);
			MPI_Send(&tempx,1,MPI_DOUBLE,0,id,MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();

	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	if (!id)
		printf("sin(%lf) ≈ %lf\nruntime = %lf\n", x, result, time_spent);	
}
