#include <stdio.h>
#include <mpi.h>

int main(int argc, char * argv[])
{
	int nproc, myrank;
	int token;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	if (myrank == 0)
	{
		token = 0;
		printf("starting token ring with %d\n",token);
		MPI_Send(&token,1,MPI_INT,1,1,MPI_COMM_WORLD);
		MPI_Recv(&token,1,MPI_INT,nproc-1,1,MPI_COMM_WORLD,&status);
		printf("finishing token ring with %d\n",token);
	}
	else
	{
		MPI_Recv(&token,1,MPI_INT,myrank-1,1,MPI_COMM_WORLD,&status);
		token++;
		printf("rank %d is sending %d on to next rank\n",myrank,token);
		if (myrank == nproc-1)
			MPI_Send(&token,1,MPI_INT,0,1,MPI_COMM_WORLD);
		else
			MPI_Send(&token,1,MPI_INT,myrank+1,1,MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}
