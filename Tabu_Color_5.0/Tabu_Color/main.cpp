
#include "TabuSearch.h"
#include "Graph.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

using namespace std;

int main()
{
	int i, j, k;
	int Color_num;
	Graph G;

	cout << "input numVertex,numEdge:";
	cin >> G.numVertex >> G.numEdge;//���붥�����ͱ���

	int *Color=new int[G.numVertex];
	CreatGraph(G);
	cout << "The inital Color_num:";
	cin >> Color_num;

	TabuSearch Tabu(G, Color_num);
	Tabu.C_TabuSearch(G, Color_num);//��������
	system("pause");
	return 0;
}





