#include "Graph.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

using namespace std;

//C_Graph::C_Graph(Grap &G) {

//}
void CreatGraph(Graph &G) {//�����ڽӾ���
	int i, j, k;
	edge E;

	for (i = 0; i < G.numVertex; i++) { //�����ʼ��������ɫinital-solution
		*(G.vex+i) = rand() % G.numVertex;
	}
	for (i = 0; i < G.numVertex; i++) {//��ʼ���ڽӾ���
		for (j = 0; j < G.numVertex; j++) {
			G.arc[i][j] = 0;
		}
	}
	 C_file(G, E);
	for (k = 0; k < G.numEdge; k++) {//��������ͼ���ڽӾ���
									 //cout << "input two vertices of the edge:" << endl;
									 //cin >> i >> j;
		G.arc[E.vectori[k][0]-1][E.vectori[k][1]-1] = 1;
		G.arc[E.vectori[k][1]-1][E.vectori[k][0]-1] = 1;
	}
	
}


void  C_file(Graph &G, edge &E) {
	fstream infile;
	char *line=new char[INF];
	//string file;
	//int vectori[INF][2];
	char *file=new char[40];
	//infile.getline(cin, file, '\n');
	cout << "please input filename:";
	cin >> file;
	infile.open(file, ios::in | ios::out);
	if (!infile) {
	cout << "error" << endl;
	cout << "please input filename:";
	cin >> file;
	infile.open(file, ios::in | ios::out);
	}
	infile.getline(line, INF);
	while (!infile.eof()) {
		for (int i = 0; i<G.numEdge; i++)
			for (int j = 0; j<2; j++)
				infile >> E.vectori[i][j];		 //��������

	}
	if (line) {
		delete line;
	}
	if (file) {
		delete file;
	}
	infile.close();

}



void  color_Inital(Graph &G, int Color_num, int *Color) {//��ɫ�����ʼ��
	int k;
	for (k = 0; k < Color_num; k++) {
		Color[k] = k;
	}
}



