/*
 * Author: Nathan Perry
 * Professor: Pettey
 * Lab: 6
 * Purpose: Simulates turtles nomnoming ducks as they reproduce in a pond.
 * 
 * 
 * Usage:
 * Compile with: mpic++ 4330L6.cc
 * Run with: mpiexec -f mpd.hosts -n 9 ./a.out

*/

#include <iostream>
#include <fstream>
#include <mpi.h>

using namespace std;

int main(int argc, char * argv[])
{
	//declare variables for mpi next use
	int nprocs, myrank;
	MPI_Status status;
	
	//declare variables for program to use
	int myval, prev, next, pond[3][3], turtle, temp;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	
	//if master process then open the file and get the input data
	if(myrank == 0)
	{
		//open file
		ifstream ifs;
		ifs.open("4330L6.dat");
		
		//check and see if file exists
		if (!ifs)
		{
			cout << "File does not exist.\n" << endl;
			MPI_Finalize();
		}
		
		//read in the pond data
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				ifs >> pond[i][j];
				
		//read in the turtle
		ifs >> turtle;
		
		//Print the initial pond
		cout << "Initial" << endl;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0;j < 3; j++)
				cout << pond[i][j] << " ";
			cout << endl;
		}
		
		ifs.close();			
	}

	//send turtle and pond information to all processes
	MPI_Bcast(&turtle,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(pond,1,MPI_INT,&myval,1,MPI_INT,0,MPI_COMM_WORLD);	
	
	// assign movements
	switch (myrank)
	{
		case 0: 
			next = 1; 
			prev = 4; 
			break;
		case 1: 
			next = 2; 
			prev = 0; 
			break;
		case 2: 
			next = 5; 
			prev = 1; 
			break;
		case 3: 
			next = 4; 
			prev = 6; 
			break;
		case 4: 
			next = 0; 
			prev = 3; 
			break;
		case 5: 
			next = 8; 
			prev = 2; 
			break;
		case 6: 
			next = 3; 
			prev = 7; 
			break;
		case 7: 
			next = 6; 
			prev = 8; 
			break;
		case 8: 
			next = 7; 
			prev = 5; 
			break;
		};
	
	//go for 10 generations
	for(int gen = 0; gen < 10; gen++)
	{
		//Clone those ducks
		if( myval == 1 || myval == 3 || myval == 4)
			myval++;
		else if (myval == 2)
			myval+=2;
		
		//eat those ducks OMNOMNOMNOMNOMNOMNOMNOMNOMNOM
		if (turtle == myrank)
		{
			if (myval == 1 || myval == 2)
				myval = 0;
			else if (myval != 0)
				myval -= 2;
		}
			
		//Where should our turtle friend go eat its next snack?
		switch (turtle)
		{
			case 0: 
				turtle += 4;
				break;
			case 1: 
				turtle -= 1;
				break;
			case 2: 
				turtle -= 1;
				break;			
			case 3: 
				turtle += 3;
				break;			
			case 4: 
				turtle -= 1;
				break;
			case 5: 
				turtle -= 3;
				break;			
			case 6: 
				turtle += 1;
				break;
			case 7: 
				turtle += 1;
				break;
			case 8: 
				turtle -= 3;
				break;
		}
		
		//send and recieve and gather the pond info
		MPI_Sendrecv(&myval,1,MPI_INT,next,1,&temp,1,MPI_INT,prev,1,MPI_COMM_WORLD,&status);
		myval=temp;
		MPI_Gather(&myval,1,MPI_INT,pond,1,MPI_INT,0,MPI_COMM_WORLD);

		//Print generation after cloning and omnomning
		if(myrank == 0)
		{
			cout << "After generation " << gen << endl;
			for(int i = 0; i < 3; i++)
			{
				for(int j = 0; j < 3; j++)
					cout << pond[i][j] << " ";
				cout << endl;
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	
	//fin
	MPI_Finalize();
	
	return 0;
}
