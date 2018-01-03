#pragma once
#include "Graph.h"

//using namespace std;

class TabuSearch
{
	struct Find_u_vi_vj {
		int u;
		int vi;
		int vj;
		int delt;
	};

	//Find_u_vi_vj FindMove;
	int *TabuTenure;
	int *Adjacent_Color_Table;
	int *Bestsol;//保存最优解
	int punishment;
	int temp;
public:
	//int funcation(Graph &G);
	void tabuList(Graph &G, int C_Length);
	//void color_Inital(Graph &G, int Color_num, int *Color);
	void aspiration_color_table(Graph &G, int Color_num);
	void	BestSolution(Graph &G, int Color_num);
	int funcation(Graph &G, int Color_num);
	int C_TabuSearch(Graph &G, int Color_num);
	void update_aspiration_color_table(Graph &G, int Color_num, Find_u_vi_vj &U);
	//void Updata_tabulist(Graph &G, int C_Length, Find_u_vi_vj &U);
	void MakeMove(Graph &G, int Color_num, Find_u_vi_vj &U);
	void FindMove(Graph &G, int Color_num, Find_u_vi_vj &U);
	void Change_Color(Graph &G, int Color_num);
	void updata_color(Graph &G, int Color_num);
	TabuSearch(Graph &G, int Color_num);
	TabuSearch();
	~TabuSearch();
};



