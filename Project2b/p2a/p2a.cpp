// Project 2a: Kevin Scalabrini and Richard Lamourine 
// 

#include "StdAfx.h"
#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <time.h>
#include <string>
#include <sstream>

using namespace std;

#include "d_except.h"
#include "d_matrix.h"
#include "graph.h"
#include "knapsack.h"


//Greedy algorithm for the knapsack
void greedyKnapsack(knapsack &k)
{
	for (int i = 0; i<k.getNumObjects(); i++)
	{
		k.select(i);

		if (k.getCost()>k.getCostLimit())
		{
			k.unSelect(i);
		}
	}
}

int main()
{
	char x;
	ifstream fin;
	stack <int> moves;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	fileName = "knapsack48.input";

	//cout << "Enter filename" << endl;
	//cin >> fileName;

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		cout << "Reading knapsack instance" << endl;
		knapsack k(fin);

		cout << "Max Cost = " << k.getCostLimit() << endl;

		greedyKnapsack(k);

		//print out solution and create ouput file
		cout << endl << "Best solution" << endl;
		k.printSolution();
		k.printSolution(fileName);

		system("pause");
		return 0;

	}

	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
}