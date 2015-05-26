// Code to read graph instances from a file.  Uses the Boost Graph Library (BGL).

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <fstream>
#include <time.h>

#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace std;
using namespace boost;

int const NONE = -1;  // Used to represent a node that does not exist]
int numcolors;

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

struct VertexProperties
{
	pair<int, int> cell; // maze cell (x,y) value
	Graph::vertex_descriptor pred;

	int ID;
	int color = 0;
	bool visited = false;
	bool marked;
	int weight;
};

// Create a struct to hold properties for each edge
struct EdgeProperties
{
	int weight;
	bool visited;
	bool marked;
};

void initializeGraph(Graph &g, ifstream &fin)
// Initialize g using data from fin.  
{
	int n, e2, h, j;
	fin >> numcolors;
	if (numcolors < 1) throw rangeError("ERROR: There must be at least one color");
	fin >> n >> e2;
	if (n < 1) throw rangeError("ERROR: There must be at least one node");
	if (e2 < 0) throw rangeError("ERROR: There must be nonnegative number of edges");
	Graph::vertex_descriptor v;

	// Add nodes.
	for (int i = 0; i < n; i++)
	{
		v = add_vertex(g);
		g[v].ID = i;
	}

	for (int i = 0; i < e2; i++)
	{
		fin >> h >> j;
		add_edge(h, j, g);  // Assumes vertex list is type vecS
	}
}

void setNodeWeights(Graph &g, int w)
// Set all node weights to w.
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);

	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].weight = w;
	}
}

//for timing
clock_t clock1, clock2;

//gets the adjacent nodes of each vertex which are unvisited
vector<Graph::vertex_descriptor> getUnvisitedNeighbors(Graph::vertex_descriptor v, const Graph &g)
{
	pair<Graph::adjacency_iterator, Graph::adjacency_iterator>
		vItrRange = adjacent_vertices(v, g);
	vector<Graph::vertex_descriptor> lst;

	for (Graph::adjacency_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		if (boost::edge(v, *vItr, g).second == true && !g[*vItr].visited)
		{
			lst.push_back(*vItr);
		}
	}
	return lst;
}
//counts the number of conflicts
int colorConflicts(Graph &g)
{
	vector<Graph::vertex_descriptor> lst;
	int conflicts = 0;

	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].visited = true;
		lst = getUnvisitedNeighbors(*vItr, g);
		int x = lst.size();
		for (int j = 0; j < lst.size(); j++)
		{
			if (g[*vItr].color == g[lst[j]].color)
				conflicts++;
		}
	}
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange2 = vertices(g);
	for (Graph::vertex_iterator vItr2 = vItrRange2.first; vItr2 != vItrRange2.second; ++vItr2)
	{
		g[*vItr2].visited = false;

	}
	return conflicts;
}
void printColors(Graph &g)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		cout << g[*vItr].color << "--" << g[*vItr].ID << endl;
	}
	cout << "-----------------------------" << endl << endl;
}
//enables program to go through all possible color combinations
void assignColor(Graph &g, int numcolors, int t)
{
	time_t timer;
	double seconds = 0;
	time(&timer);
	bool ret = false;
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		printColors(g);
		g[*vItr].color = 1;
		int bestConflicts = colorConflicts(g);
		int bestColor = 1;
		int currConflicts;
		while (g[*vItr].color <= numcolors)
		{
			g[*vItr].color = g[*vItr].color + 1;
			currConflicts = colorConflicts(g);

			if (bestConflicts > currConflicts) bestColor = g[*vItr].color; bestConflicts = currConflicts;
		}
		g[*vItr].color = bestColor;
		//if (difftime(time(NULL), timer) > t) cout << "The specified time has elapsed!"; return;
	}
}





//exhaustive coloring algorithm runs for t seconds, gets numcolors from the input file, 
//run on graph g from the input
void greedyColoring(Graph &g, int numcolors, long t)
{
	if (t < 0 || t > 600) throw rangeError("ERROR: The max runtime of the algorithm must be between 0 and 600 sec");
	Graph temp(g);
	int minconflicts, conflicts;
	bool flag = true;
	int count = 0;

	assignColor(g, numcolors, t);
}
void printSolution(Graph g, string fileName)
{
	string fileNameO;

	ofstream fout;
	fileNameO = fileName.substr(0, fileName.size() - 5) + "output";

	fout.open(fileNameO.c_str());
	if (!fout)
	{
		cerr << "Cannot open " << fileNameO << endl;
		exit(1);
	}

	int numConflicts = 0;

	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange2 = vertices(g);

	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		for (Graph::vertex_iterator vItr2 = vItrRange2.first; vItr2 != vItrRange2.second; ++vItr2)
		{
			if (boost::edge(*vItr, *vItr2, g).second == true && g[*vItr].color == g[*vItr2].color)
				numConflicts++;
		}
	}


	fout << numConflicts << endl;
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
		fout << g[*vItr].ID << " " << g[*vItr].color << endl;

	cout << "it took: " << ((double)(clock() - clock1)) / CLOCKS_PER_SEC << endl;
	cout << "Solution has been printed to  " << fileNameO << endl;

	fout.close();
}

int main()
{
	ifstream fin;
	string fileName;

	// Read the name of the graph from the keyboard or
	// hard code it here for testing.

	fileName = "color192-8.input";

	//cout << "Enter filename : " << endl;
	//cin >> fileName;

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		cout << "Reading graph" << endl;
		Graph g;
		initializeGraph(g, fin);


		cout << "Num nodes: " << num_vertices(g) << endl;
		cout << "Num edges: " << num_edges(g) << endl;
		cout << endl;

		greedyColoring(g, numcolors, 600);
		printSolution(g, fileName);
		system("pause");
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