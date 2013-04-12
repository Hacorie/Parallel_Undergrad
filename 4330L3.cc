/*
 * Author: Nathan Perry (1/4 of Team Matt)
 * 
 * Professor:  Pettey
 * Code:  4330L3

 * Purpose: Use Open MP To do warshall's algorithm.
 * 
 * Input: 
 *        Size of the square matrixes
 * 		  The 2 matrixes.
 * Output:
 * 		  Matrix at each step and then final(reachability matrix)
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <omp.h>

using namespace std;

const int MAX = 10; //max size of the square matrix

int num;			//size input by user
int matrix[MAX][MAX]; //matrix

int main()
{
	int rowNum = 0;
	int columnNum = 0;
	
	//Initialize Matrix1 to all 0's
	for(int i=0; i < MAX; i++)
		for(int j=0; j<MAX; j++)
			matrix[i][j] = 0;
	
	cout << "Please enter a number: ";
	cin >> num;
	
	//Error Checking
	if(num < 1 || num > 10)
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
	
	for(int i = 0; i < num; i++)
	{
		#pragma omp parallel private (rowNum,columnNum) shared (matrix) num_threads (num)
		{
			rowNum = omp_get_thread_num();
			
			for(columnNum = 0; columnNum < num; columnNum++)
			{									
				if(matrix[rowNum][i] == 1)
					matrix[rowNum][columnNum] = matrix[rowNum][columnNum] || matrix[i][columnNum];
				else
					matrix[rowNum][columnNum] = matrix[rowNum][columnNum];
			}
		}
		
		for(int j = 0; j < num; j++)
		{
			for(int k=0; k < num; k++)
			{
				cout << matrix[j][k] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
	
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
