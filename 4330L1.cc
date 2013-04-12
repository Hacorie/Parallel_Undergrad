/*
 * Author: Nathan Perry
 * Professor:  Pettey
 * Code:  4330L1

 * Purpose: To get accustomed to using basic pthreading. This program will
 * take any two 2x2 to 10x10 matrix and add them by using a thread for each row.
 * 
 * Input: 
 *        Size of the square matrixes
 * 		  The 2 matrixes.
 * Output:
 * 		  The added matrixes
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>

using namespace std;

const int MAX = 10; //max size of the square matrix
int matrix1[MAX][MAX]; //matrix
int matrix2[MAX][MAX]; //second matrix
int sumMatrix[MAX][MAX]; //matrix to hold the sum

void *add_row_function(void *ptr);

int main()
{

	
	int num = 0; //Any number 2-10
	pthread_t threadHolder[MAX]; //array to hold p_threads
	int args[MAX];
	
	//Initialize Matrix1 to all 0's
	for(int i=0; i < MAX; i++)
		for(int j=0; j<MAX; j++)
			matrix1[i][j] = 0;

	//Initialize Matrix2 to all 0's
	for(int i=0; i < MAX; i++)
		for(int j=0; j<MAX; j++)
			matrix2[i][j] = 0;
			
	//Initialize sumMatrix to all 0's
	for(int i=0; i < MAX; i++)
		for(int j=0; j<MAX; j++)
			sumMatrix[i][j] = 0;
	
	cout << "Please enter a number: ";
	cin >> num;
	
	//Error Checking
	if(num <= 1 || num > 10)
	{
		cout << "Error: Number entered is larger or smaller than intended." << endl;
		return -1;
	}
	
	//Read in values and place them in the matrix
	cout << "Please input matrix 1: ";
	for(int i = 0; i < num; i++)
		for(int j = 0; j<num; j++)
			cin >> matrix1[i][j];
			
	cout << "Please input matrix 2: ";
	for(int i = 0; i < num; i++)
		for(int j = 0; j<num; j++)
			cin >> matrix2[i][j];
	
	//create a thread for each row.		
	for(int i=0;i<num;i++)
	{
		args[i] = i;
		pthread_create(&threadHolder[i],NULL,add_row_function,(void *)&args[i]);
	}
	
	//join the threads back together
	for(int i=0;i<num;i++)
	{
		pthread_join(threadHolder[i], NULL);
	}
	
	for(int i = 0; i < num; i++)
	{
		for(int j = 0; j<num; j++)
			cout << sumMatrix[i][j] << " ";
		cout << endl;
	}	
	
    return 0;
}


void *add_row_function(void* ptr)
{
	int newptr = *(int*)ptr;
	
	for(int i=0; i < MAX; i++)
	{
		sumMatrix[newptr][i] = matrix1[newptr][i] + matrix2[newptr][i];
	}
}
