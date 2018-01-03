#pragma once
#include "Graph.h"
#define maxV 1001 //最大顶点个数
#define INF 500000 //最大冲突数
#define MaxIter 10000//最大迭代次数
//extern int Adjacent_Color_Table[maxV][maxV];
//extern int TabuTenure[maxV][maxV];
extern int iter;
typedef long VertexType;
typedef long EdgeType;
struct Graph {
	VertexType vex[maxV];//顶点表，下标表示顶点，值表示顶点的颜色值
	EdgeType    arc[maxV][maxV];// 邻接矩阵
	int numVertex, numEdge;
};
struct edge {
	int vectori[INF][2];
};
void C_file(Graph &G, edge &E);
//int C_file(edge M, char *filename);
//int C_file( string &file);
void CreatGraph(Graph &G);
void color_Inital(Graph &G, int Color_num, int *Color);
void	BestSolution(Graph &G, int Color_num);
int funcation(Graph &G);
class C_Graph
{
public:
	//Graph();
	//~Graph();


};


