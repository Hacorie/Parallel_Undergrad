//Author: Nathan Perry
//LAB: 4
//Prof: Pettey
//Purpose: to simulate the daily actions of an ATM

#include <iostream>
#include <fstream>
#include <omp.h>

using namespace std;

//define constants
const int MAX_QUEUE_SIZE = 3;
const int MAX_NUM_ATMS = 4;
const int MAX_NUM_CUSTOMERS = 4;

//structure for an account
struct account
{
    int account_num;
    float balance;
};

// structure for a single item that will go in the queue
struct itemInfo
{
    int atm;
    char action;
    int account;
    float amount;
    float curBalance;
};

//structure for account specific information


int main()
{
	cout << " I started this way too late :D" << endl;
	
	return 0;
}
