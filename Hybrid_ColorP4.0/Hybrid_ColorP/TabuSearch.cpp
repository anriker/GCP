#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "TabuSearch.h"
#include <ctime>
using namespace std;

int iter = 1;
int best_p = 100;
TabuSearch::TabuSearch(Graph &G, int Color_num) :TabuTenure(new int[maxV*maxV]), Adjacent_Color_Table(new int[maxV*maxV]())
{
	int punishment;
	int temp;
}
int TabuSearch::C_TabuSearch(Graph &G, int*init_color, int Color_num) {//��������

	Find_u_vi_vj Betmove = { 0,0,0,G.numVertex };
	int i;
	iter = 1;
	//Change_Color(G, Color_num);//��ʼ��ɫ
	for (i = 0; i < G.numVertex; i++) {
		G.vex[i] = *(init_color+i);//��ʼ��ɫ
		//cout << G.vex[i] <<" ";
	}//cout << endl;
	memset(Adjacent_Color_Table, 0, G.numVertex*Color_num * sizeof(Adjacent_Color_Table));//��ʼ�ڽ���ɫ��
	tabuList(G, Color_num);
	punishment = funcation(G, Color_num);
	best_p = punishment;
	//clock_t start_time = clock();//��ʼʱ��
	while (iter<50000) {

		if (punishment == 0) {
			//clock_t end_time = clock();
			BestSolution(G, Color_num);
			//cout << endl << "time:" << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;//���㲢�������ʱ��
			//cout << endl;
			return 0;
		}
		FindMove(G, Color_num, Betmove);
		MakeMove(G, Color_num, Betmove);
		iter++;
	}
	for (i = 0; i < G.numVertex; i++) {

	
		*(init_color + i) = G.vex[i];
	}

	return punishment;
}

void TabuSearch::Change_Color(Graph &G, int Color_num) {//�����ʼ��ɫ
	int i;
	for (i = 0; i < G.numVertex; i++) { //�����ʼ�����н�
										//srand(time(NULL));
		G.vex[i] = rand() % Color_num;
	}
}

void TabuSearch::tabuList(Graph &G, int C_Length) {//TabuTenure[N][K],��ʼ���ɱ�Ϊ0

	for (int k = 0; k < G.numVertex; k++)
		for (int l = 0; l < C_Length; l++) {
			*(TabuTenure + k * C_Length + l) = 0;
		}
}

void TabuSearch::aspiration_color_table(Graph &G, int Color_num)//�����ڽ���ɫ��Adjacent_Color_Table[N][K]
{
	int  i, j, k;
	for (i = 0; i < G.numVertex; i++) {
		temp = i * Color_num;
		for (k = 0; k < G.numVertex; k++) {
			if (G.arc[i][k] == 1)
				for (j = 0; j < Color_num; j++) {
					if (j == G.vex[k]) {
						Adjacent_Color_Table[temp + j]++;
					}
				}
		}
	}
}

void TabuSearch::update_aspiration_color_table(Graph &G, int Color_num, Find_u_vi_vj &U) {//��ɫ�����ݸ���
	int i;
	//int temp;
	for (i = 0; i < G.numVertex; i++) {
		temp = i * Color_num;
		if (G.arc[U.u][i] == 1) {
			//if (G.vex[i] == U.vi)
			Adjacent_Color_Table[temp + U.vi]--;
			//if (G.vex[i] == U.vj) {
			Adjacent_Color_Table[temp + U.vj]++;
			//}
		}
	}

}

void TabuSearch::FindMove(Graph &G, int Color_num, Find_u_vi_vj &U) {

	//ȷ��<u,vi,vj>
	int delt = 0, not_tabu_sameNumber = 0, tabu_sameNumber = 0;
	int i, k, m;
	Find_u_vi_vj tabu_U = { 0,0,0,G.numEdge }, not_tabu_U = { 0,0,0,G.numEdge };
	//Find_u_vi_vj *tabu_U1 = new Find_u_vi_vj[G.numVertex*Color_num]{ 0,0,0,G.numVertex },
	//*not_tabu_U1 = new Find_u_vi_vj[G.numVertex*Color_num]{ 0,0,0,G.numVertex };  //���ҹ����еĽ��ŵĶ���,�����������з���ѡ��																					  //Find_u_vi_vj U = { 0,0,0 };
	for (i = 0; i < G.numVertex; i++) {
		temp = i * Color_num;
		if (Adjacent_Color_Table[temp + G.vex[i]] > 0) {
			//cout << i << endl;
			for (k = 0; k < Color_num; k++) {
				if (k != G.vex[i]) {
					delt = Adjacent_Color_Table[temp + k] - Adjacent_Color_Table[temp + G.vex[i]];
					if (TabuTenure[temp + k] < iter) {//update the tabu best move;
													
						if (delt < not_tabu_U.delt) {
							not_tabu_sameNumber = 1;//һ���������Ŷ�������ͬ����������1
							not_tabu_U = { i , G.vex[i],k,delt };	//���·ǽ���ʱ�Ľ��Ŷ���
							//not_tabu_U1[0] = { i , G.vex[i],k,delt };
																	
						}
						else if (delt == not_tabu_U.delt)              //������ͬ����
						{
							not_tabu_sameNumber++;
							/*1.0 double p = (double)rand() / RAND_MAX;
							if (p < double(1 / not_tabu_sameNumber))*/

							/*2.0(�ڴ治��,ռ�ڴ棩 not_tabu_U1[not_tabu_sameNumber++] = { i , G.vex[i],k,delt };
							m = rand() % not_tabu_sameNumber;
							not_tabu_U = not_tabu_U1[m];*/
							if (rand() % not_tabu_sameNumber == 0)
								not_tabu_U = { i , G.vex[i],k,delt };	
						}

					}
					else//update the non-tabu best move
					{
						if (delt < tabu_U.delt)
						{
							tabu_sameNumber = 1;
							tabu_U = { i ,G.vex[i],k,delt };  //���½���ʱ�Ľ��Ŷ���
							//tabu_U1[0] = { i , G.vex[i],k,delt };
						}
						else if (delt == tabu_U.delt)              //��������ͬ�����
						{
							tabu_sameNumber++;
							/*double p = (double)rand() / RAND_MAX;
							if (p < double(1 / tabu_sameNumber))*/

							/*tabu_U1[tabu_sameNumber++] = { i , G.vex[i],k,delt };
							m = rand() % tabu_sameNumber;
							tabu_U = tabu_U1[m];*/
							if (rand() % tabu_sameNumber == 0)
								tabu_U = { i , G.vex[i],k,delt };
						}
					}
				}
			}
		}
	}
	/*if (tabu_U1) {//�ռ��ͷ�
	delete tabu_U1;
	}
	if (not_tabu_U1)
		delete not_tabu_U1;*/
	if ((tabu_U.delt < not_tabu_U.delt) &&((punishment + tabu_U.delt) < best_p)) //������������ɽ����ڵ�ǰ���ҵķǽ��ɽ�����õ���������ʷ���Ž�
	{
		U = tabu_U;
	}
	else
		U = not_tabu_U;

}

void TabuSearch::MakeMove(Graph &G, int Color_num, Find_u_vi_vj &U) {
	G.vex[U.u] = U.vj;
	punishment = punishment + U.delt;
	/* 2.0(�޶��Ч����if (punishment <= 10) {
	TabuTenure[U.u*Color_num + U.vi] = punishment + rand() % 10 + iter + (10 - punishment);
	}
	else */
	TabuTenure[U.u*Color_num + U.vi] = punishment + rand() % 10 + iter;//�޸Ľ��ɱ�
	update_aspiration_color_table(G, Color_num, U);//�����ڽ���ɫ��
	if (punishment<best_p)                           //���³�ͻ��С�Ľ�
	{
		best_p = punishment;
	}
}

int TabuSearch::funcation(Graph &G, int Color_num) {//Ŀ�꺯��������¼�ͷ�����
	int punish = 0;
	aspiration_color_table(G, Color_num);
	for (int i = 0; i < G.numVertex; i++) {
		temp = i * Color_num;
		punish = punish + Adjacent_Color_Table[temp + G.vex[i]];
	}
	punish /= 2;
	return punish;
}
void TabuSearch::BestSolution(Graph &G, int Color_num) {//������ý⣬����ǰ��ý����ɫBestSol[N]��vex[maxV]��

	cout << endl << "Color_num=" << Color_num << ",the Feasible soulation:";
	for (int i = 0; i < G.numVertex; i++) {
		cout << G.vex[i] << " ";
	}cout << endl;
	cout << "The iter=" << iter << endl;
}

TabuSearch::~TabuSearch()
{
	/*if (TabuTenure)
		delete[] TabuTenure;
	if (Adjacent_Color_Table)
		delete[] Adjacent_Color_Table;
		*/	
}

