/*
 * Author: Nathan Perry (1/4 of Team Matt)
 * 
 * Professor:  Pettey
 * Code:  4330L2

 * Purpose: Use Pthread's lock and unlock mechanisms to do Warshall's algorithm.
 * 
 * Input: 
 *        Size of the square matrixes
 * 		  The 2 matrixes.
 * Output:
 * 		  Matrix at each step and then final(reachability matrix)
 * 
 * For this lab, you will write a pthreads program that will calculate the reachability matrix for a square matrix up to size 10X10 using Warshalls algorithm.  
 * Your program should ask for the size, and then spawn enough threads so that each thread is responsible for calculating one row of each intermediate matrix.  
 * As each intermediate matrix is calculated, it should be printed by the last thread to finish it's row.  
 * The main thread will print the reachability matrix when the child threads are done. 
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>

using namespace std;


const int MAX = 10; //max size of the square matrix

int num;			//size input by user
int matrix[MAX][MAX]; //matrix
int columnsDone = 0; //count of which columns have been processed
int rowsDone = 0;  //count of which rows have been processed

//pthread lock and unlock variables
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

//function to or rows with ones in the i(th) column
void *add_row_function(void* ptr);

int main()
{
	pthread_t threadHolder[MAX]; //array to hold p_threads
	int args[MAX];
	
	//Initialize Matrix1 to all 0's
	for(int i=0; i < MAX; i++)
		for(int j=0; j<MAX; j++)
			matrix[i][j] = 0;

	
	cout << "Please enter a number: ";
	cin >> num;
	
	//Error Checking
	if(num <= 1 || num > 10)
	{
		cout << "Error: Number entered is larger or smaller than intended." << endl;
		return -1;
	}
	
	//Read in values and place them in the matrix
	cout << "Please input matrix: ";
	for(int i = 0; i < num; i++)
		for(int j = 0; j<num; j++)
			cin >> matrix[i][j];
			
	cout << endl << endl;
	
	//spawn threads dependant on size		
	for(int k = 0; k < num; k++)
	{
		args[k] = k;
		pthread_create(&threadHolder[k],NULL,add_row_function,(void *)&args[k]);
	}

	//Join the threads
	for(int i=0; i < num; i++)
		pthread_join(threadHolder[i], NULL);
	
	//Print the reachability matrix	
	cout << "The Reachablility Matrix is: " << endl;
	for(int i = 0; i < num; i++)
	{
		for(int j = 0; j<num; j++)
			cout << matrix[i][j] << " ";
		cout << endl;
	}
		
	return 0;
			
}

void *add_row_function(void* ptr)
{	
	int newptr = *(int*)ptr;

	//for every column check for 1 in the column and then or if so
	for(columnsDone = 0; columnsDone < num;)
	{
		if(matrix[newptr][columnsDone])
		{
			for(int j = 0; j < num; j++)
			{
				matrix[newptr][j] |= matrix[columnsDone][j];
			}
		}
	
		//lock rowsDone while it is updating
		pthread_mutex_lock(&mutex1);
		rowsDone++;
		pthread_mutex_unlock(&mutex1);

		//Lock matrix while waiting
		pthread_mutex_lock(&mutex1);
		
		//check to see if all threads are finished
		if(rowsDone < num)
		{
			//if not wait
			pthread_cond_wait(&condition_var, &mutex1);
		}
		else
		{
			//reset rowsDone -> increment columnsdone
			rowsDone = 0;
			columnsDone +=1;
			
			//print the matrix
			if(columnsDone < num)
			{
				cout << "Matrix: " << columnsDone << endl;
				for(int i=0; i < num; i++)
				{
					for(int j=0; j < num; j++)
					{
						 cout << matrix[i][j] << " ";
					}
					cout << endl;
				}
				cout << endl;
			}
			//wake all threads up
			pthread_cond_broadcast(&condition_var);
		}
		//unlock the matrix
		pthread_mutex_unlock(&mutex1);
	}
}
