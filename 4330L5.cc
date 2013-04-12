/*
 * Author: Nathan Perry
 * Professor: Pettey
 * Lab: 5
 * Purpose: To find the integral of
 * 
 * from 0 to x sqrt(3(sin^3) (x/2))dx
 * 
 * 
 * Usage:
 * Compile with: mpic++ 4330L5.cc
 * Run with: mpiexec -f mpd.hosts -n 16 ./a.out

*/

#include <iostream>
#include <cmath>
#include <mpi.h>

using namespace std;

const double PI = atan(1.0)*4;

int main(int argc, char *argv[])
{
	//declare varables used by MPI
	int nproc;
	int myrank;
	
	MPI_Status status;
	
	//get number of processes and rank
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	
	//error checking
	if(nproc > 16)
	{
		cout << "usage error: number of processes must be less that 16" << endl;
		return -1;
	}
	
	//initalize sum
	double sum = 0;
	
	//find area of a rectangle
	double width = PI / nproc;
	double leftEndPoint = width * myrank;
	double midPoint = leftEndPoint + ((.5)*(width));
	double rectangleArea = double(sqrt(3 * sin(midPoint/2) * sin(midPoint/2) * sin(midPoint/2)) * width);
	
	//find the integral
	if (myrank == 0)
	{
		sum = rectangleArea;
		cout << "starting sum ring with " << sum << endl;
		MPI_Send(&sum,1,MPI_DOUBLE,1,1,MPI_COMM_WORLD);
		MPI_Recv(&sum,1,MPI_DOUBLE,nproc-1,1,MPI_COMM_WORLD,&status);
		cout << "finishing sum ring with " << sum << endl;
	}
	else
	{
		MPI_Recv(&sum,1,MPI_DOUBLE,myrank-1,1,MPI_COMM_WORLD,&status);
		sum += rectangleArea;
		cout << "rank " << myrank << " is sending "<< sum << " on to next rank" << endl;
		if (myrank == nproc-1)
			MPI_Send(&sum,1,MPI_DOUBLE,0,1,MPI_COMM_WORLD);
		else
			MPI_Send(&sum,1,MPI_DOUBLE,myrank+1,1,MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	
	return 0;
}
