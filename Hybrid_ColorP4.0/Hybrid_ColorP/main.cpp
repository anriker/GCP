#include "TabuSearch.h"
#include "Graph.h"
#include "Population.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <ctime>

using namespace std;
int main()
{
	int i, j, k;
	int Color_num;
	int count = 0;
	Graph G;
	cout << "input numVertex,numEdge:";
	cin >> G.numVertex >> G.numEdge;//输入顶点数和边数
	CreatGraph(G);
	cout << "The inital Color_num:";
	cin >> Color_num;
	TabuSearch Tabu(G, Color_num);
	Population Hybrid(G, Color_num,Tabu);
	Hybrid.HybridSearch(G,count,Tabu,Color_num);
	clock_t end_time = clock();

	system("pause");
	return 0;
}





