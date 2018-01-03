#pragma once
#include "TabuSearch.h"
#include "Graph.h"
//#define minC 200
class Population
{
	struct Pop_set{//¥Ê¥¢÷÷»∫ºØ
		int V[maxV] ;
		int conflict;
	};
	Pop_set s[6];

	int Pop_num;
	int ColorNum;
	int *subset;
	int *a;
	int *Best_soulation;

public:
	int InitPopulation(Graph &G, TabuSearch Tabu, int  ColorNum);
	void HybridSearch(Graph &G,int count,TabuSearch Tabu, int  ColorNum);
	void  ChooseParents(Graph &G, int  ColorNum,Pop_set &s1,Pop_set &s2);
	void Crossover(Graph &G, int  ColorNum, Pop_set &s1, Pop_set &s2, Pop_set& S);
	void UpdatePopulation(Graph &G, int  ColorNum, Pop_set &S);
	Population(Graph &G, int Color_num, TabuSearch Tabu);
	~Population();
};
