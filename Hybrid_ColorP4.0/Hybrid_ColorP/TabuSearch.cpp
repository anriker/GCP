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
int TabuSearch::C_TabuSearch(Graph &G, int*init_color, int Color_num) {//禁忌搜索

	Find_u_vi_vj Betmove = { 0,0,0,G.numVertex };
	int i;
	iter = 1;
	//Change_Color(G, Color_num);//初始颜色
	for (i = 0; i < G.numVertex; i++) {
		G.vex[i] = *(init_color+i);//初始颜色
		//cout << G.vex[i] <<" ";
	}//cout << endl;
	memset(Adjacent_Color_Table, 0, G.numVertex*Color_num * sizeof(Adjacent_Color_Table));//初始邻接颜色表
	tabuList(G, Color_num);
	punishment = funcation(G, Color_num);
	best_p = punishment;
	//clock_t start_time = clock();//初始时间
	while (iter<50000) {

		if (punishment == 0) {
			//clock_t end_time = clock();
			BestSolution(G, Color_num);
			//cout << endl << "time:" << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;//计算并输出运行时间
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

void TabuSearch::Change_Color(Graph &G, int Color_num) {//随机初始颜色
	int i;
	for (i = 0; i < G.numVertex; i++) { //随机初始化可行解
										//srand(time(NULL));
		G.vex[i] = rand() % Color_num;
	}
}

void TabuSearch::tabuList(Graph &G, int C_Length) {//TabuTenure[N][K],初始禁忌表都为0

	for (int k = 0; k < G.numVertex; k++)
		for (int l = 0; l < C_Length; l++) {
			*(TabuTenure + k * C_Length + l) = 0;
		}
}

void TabuSearch::aspiration_color_table(Graph &G, int Color_num)//定义邻接颜色表Adjacent_Color_Table[N][K]
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

void TabuSearch::update_aspiration_color_table(Graph &G, int Color_num, Find_u_vi_vj &U) {//颜色表数据更新
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

	//确定<u,vi,vj>
	int delt = 0, not_tabu_sameNumber = 0, tabu_sameNumber = 0;
	int i, k, m;
	Find_u_vi_vj tabu_U = { 0,0,0,G.numEdge }, not_tabu_U = { 0,0,0,G.numEdge };
	//Find_u_vi_vj *tabu_U1 = new Find_u_vi_vj[G.numVertex*Color_num]{ 0,0,0,G.numVertex },
	//*not_tabu_U1 = new Find_u_vi_vj[G.numVertex*Color_num]{ 0,0,0,G.numVertex };  //查找过程中的较优的动作,并存入数组中方便选择																					  //Find_u_vi_vj U = { 0,0,0 };
	for (i = 0; i < G.numVertex; i++) {
		temp = i * Color_num;
		if (Adjacent_Color_Table[temp + G.vex[i]] > 0) {
			//cout << i << endl;
			for (k = 0; k < Color_num; k++) {
				if (k != G.vex[i]) {
					delt = Adjacent_Color_Table[temp + k] - Adjacent_Color_Table[temp + G.vex[i]];
					if (TabuTenure[temp + k] < iter) {//update the tabu best move;
													
						if (delt < not_tabu_U.delt) {
							not_tabu_sameNumber = 1;//一旦遇到更优动作，相同动作计数归1
							not_tabu_U = { i , G.vex[i],k,delt };	//更新非禁忌时的较优动作
							//not_tabu_U1[0] = { i , G.vex[i],k,delt };
																	
						}
						else if (delt == not_tabu_U.delt)              //遇到相同动作
						{
							not_tabu_sameNumber++;
							/*1.0 double p = (double)rand() / RAND_MAX;
							if (p < double(1 / not_tabu_sameNumber))*/

							/*2.0(内存不足,占内存） not_tabu_U1[not_tabu_sameNumber++] = { i , G.vex[i],k,delt };
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
							tabu_U = { i ,G.vex[i],k,delt };  //更新禁忌时的较优动作
							//tabu_U1[0] = { i , G.vex[i],k,delt };
						}
						else if (delt == tabu_U.delt)              //当遇到相同的情况
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
	/*if (tabu_U1) {//空间释放
	delete tabu_U1;
	}
	if (not_tabu_U1)
		delete not_tabu_U1;*/
	if ((tabu_U.delt < not_tabu_U.delt) &&((punishment + tabu_U.delt) < best_p)) //解禁条件：禁忌解优于当前查找的非禁忌解中最好的且优于历史最优解
	{
		U = tabu_U;
	}
	else
		U = not_tabu_U;

}

void TabuSearch::MakeMove(Graph &G, int Color_num, Find_u_vi_vj &U) {
	G.vex[U.u] = U.vj;
	punishment = punishment + U.delt;
	/* 2.0(无多大效果）if (punishment <= 10) {
	TabuTenure[U.u*Color_num + U.vi] = punishment + rand() % 10 + iter + (10 - punishment);
	}
	else */
	TabuTenure[U.u*Color_num + U.vi] = punishment + rand() % 10 + iter;//修改禁忌表
	update_aspiration_color_table(G, Color_num, U);//更新邻接颜色表
	if (punishment<best_p)                           //更新冲突最小的解
	{
		best_p = punishment;
	}
}

int TabuSearch::funcation(Graph &G, int Color_num) {//目标函数，即记录惩罚次数
	int punish = 0;
	aspiration_color_table(G, Color_num);
	for (int i = 0; i < G.numVertex; i++) {
		temp = i * Color_num;
		punish = punish + Adjacent_Color_Table[temp + G.vex[i]];
	}
	punish /= 2;
	return punish;
}
void TabuSearch::BestSolution(Graph &G, int Color_num) {//保持最好解，即当前最好解的颜色BestSol[N]（vex[maxV]）

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

