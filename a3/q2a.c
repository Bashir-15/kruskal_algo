/**

Andrew Atkinson
3229071
Assignment 3 Question 2 (a)

**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double taylor( double x, int level);

int main( int argc, char *argv[]){

	clock_t begin, end;
	double time_spent;

	begin = clock();
	
	double x = atof(argv[1]), result;
	printf("x = %lf\n", x);
	double fact = 1;

	result = taylor(x,0);

	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("sin(%lf) ≈ %lf\nruntime = %lf\n", x, result, time_spent);	
}

double taylor( double x, int level){

	int i;
	double tempx=1, fact=1;
	for(i = 1; i <= level*2+1; i++){
		tempx*=x;
		fact*=i;
	}
	tempx/=fact;
	printf("tempx = %lf, level = %d\n", tempx, level);
	if(level%2)
		tempx*=-1;
	if(level >= 3)
		return tempx;
	else
		return tempx+=taylor(x,level+1);
}
		
