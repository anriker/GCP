#pragma once
#include "Graph.h"
#define maxV 1001 //��󶥵����
#define INF 500000 //����ͻ��
#define MaxIter 10000//����������
//extern int Adjacent_Color_Table[maxV][maxV];
//extern int TabuTenure[maxV][maxV];
extern int iter;
typedef long VertexType;
typedef long EdgeType;
struct Graph {
	VertexType vex[maxV];//������±��ʾ���㣬ֵ��ʾ�������ɫֵ
	EdgeType    arc[maxV][maxV];// �ڽӾ���
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


