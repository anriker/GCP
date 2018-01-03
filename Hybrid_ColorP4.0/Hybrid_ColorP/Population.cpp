#include "Population.h"
#include "TabuSearch.h"
#include <ctime>
#include <iostream>
#include <cstdio>

using namespace std;
int Best_conflict = 10000;
Population::Population(Graph &G, int Color_num,TabuSearch Tabu) :subset(new int[Color_num]), Best_soulation(new int[G.numVertex]),Pop_num(6),a(new int[2]) //声明种群及其个数
{
	InitPopulation(G, Tabu,Color_num);

}

int Population::InitPopulation(Graph &G, TabuSearch Tabu,int  Color_num) {//初始化种群解
	int i, j = 0, k = 0, m = 0;
	cout << Color_num << endl;
	for (k = 0; k < Pop_num; k++) {
		srand((unsigned int)clock());
		for (i = 0; i < G.numVertex; i++) {
			s[k].V[i] = rand() % Color_num;//初始种群解
		}
		
		s[k].conflict= Tabu.C_TabuSearch(G, s[k].V, Color_num);//禁忌搜索对初始解优化
	}
	return 1;
}

void Population::HybridSearch(Graph &G,int count,TabuSearch Tabu, int  Color_num) {
	clock_t start_time = clock();//初始时间
	Pop_set s1 = { 0,0 }, s2 = { 0,0 }, S = { 0,0 };
	while (1) {
		count++;
		ChooseParents(G,Color_num,s1,s2);
		Crossover(G,Color_num,s1,s2,S);
		/*for (int j = 0; j < G.numVertex; j++)
			cout << S.V[j] << " ";
		cout << endl;*/
		S.conflict = Tabu.C_TabuSearch(G,S.V, Color_num);
		cout << S.conflict << " ";
		if (S.conflict == 0){
			clock_t end_time = clock();
			cout << endl << "time:" << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;//计算并输出运行时间
			start_time = end_time;
			clock_t temp = clock();
			cout << "The Crossover_count: " << count << endl;
			InitPopulation(G, Tabu, Color_num);
			start_time = end_time+(clock()-temp);
			count = 1;
			ChooseParents(G, Color_num, s1, s2);
			Crossover(G, Color_num, s1, s2, S);
			S.conflict = Tabu.C_TabuSearch(G, S.V, Color_num);
			cout << S.conflict << " ";//重复搜索
		}
		UpdatePopulation(G, Color_num,S);
		if (S.conflict < Best_conflict)				   //更新最优解
		{
			Best_conflict = S.conflict;
			for (int i = 0; i < G.numVertex; i++)
			{
				Best_soulation[i] = S.V[i];
			}
		}

	}
}
void Population::ChooseParents(Graph &G, int  Color_num,Pop_set &s1, Pop_set &s2) {
	int  i, j, m = 0;
	srand(time(NULL));//通过获取时间值来设定随机数种子，以保证每次运行结果不同。
	a[0] = rand() % Pop_num;
	a[1] = rand() % Pop_num;
	while (a[0] = a[1]) {
		a[1]= rand() % Pop_num;
	}
	/*for (i = 0; i < 2; )//判断每次得到的数值是否重复，如重复则不进行i的自加
	{
		a[i] = rand() % Pop_num; //得到一个0-Pop_num中的随机数。
		for (j = 0; j < i; j++)
			if (a[j] == a[i])break;//发现重复，结束内循环

		if (j == i) i++;//如果i和j相等，代表内循环是达到条件退出的，即不是通过break,也就是没有重复项。
	}*/
	for (i = 0; i < G.numVertex; i++) {
		s1.V[i] = s[a[0]].V[i];//初始化s1集合
		s2.V[i] = s[a[1]].V[i];//初始化s2集合
	}


}

void Population::Crossover(Graph &G, int  Color_num,Pop_set &s1,Pop_set &s2,Pop_set &S) {
	int i, item = 0, j, k;
	int max_InSubset_num;//最大独立集的模值
	int max_InSubset_color;//最大独立集的颜色值
	int *temp = new int[G.numVertex];
	int sameSet_number;//独立集个数相同的个数计数
	//ChooseParents(G);
	for (i = 0; i < Color_num; i++) {//从s1、s2中轮流取出独立子集个数最大的子集
		max_InSubset_num = 0;
		if (Color_num % 2 != 0)
			temp = s1.V;
		else {
			temp = s2.V;
		}
		for (j = 0; j < Color_num; j++) {
			subset[j] = 0;//初始独立集大小（下标表示颜色数，值表示该颜色对应的顶点个数，即集合大小）
		}
		for (j = 0; j < G.numVertex; j++)                        //计算各独立子集的大小
		{
			subset[temp[j]]++;
		}
		for (j = 0; j < Color_num; j++)                           //统计最大独立集的大小及其对应的颜色
		{
			if (subset[j] > max_InSubset_num)
			{
				sameSet_number = 1;
				max_InSubset_num = subset[j];
				max_InSubset_color = j;
				
			}
			else if (subset[j] == max_InSubset_num) {
				sameSet_number++;
				double p = (double)rand() / RAND_MAX;//水塘抽样随机选择
				if (p < double(1 / sameSet_number)) {
					max_InSubset_num = subset[j];
					max_InSubset_color = j;
				}
			}
			//cout <<"max_InSubset_color:"<< max_InSubset_color << endl;
		}
		for (j = 0; j < G.numVertex; j++) {//最大独立集中的元素传于子代，删除父代中已传元素
			if (temp[j] == max_InSubset_color) {//找到相应的元素
				S.V[j] = max_InSubset_color;//子代赋值
				s1.V[j] = -1;//标记父代中传元素
				s2.V[j] = -1;
			}
			
		}
	}
		for (int i = 0; i < G.numVertex; i++)     //寻找两个父代中剩余的值，并随机分配到子代中
		{
			if ((s1.V[i] != (-1)) && (s2.V[i] != -1)){
				S.V[i] = rand() % Color_num;
			}
		}

}

void Population::UpdatePopulation(Graph &G, int  Color_num,Pop_set &S) {//更新上一代解

	int *temp=new int[G.numVertex];
	temp = 0;
	if (s[a[0]].conflict < s[a[1]].conflict)                            //判断父代中较差的解
	{
		temp = s[a[1]].V;
	}
	else if (s[a[0]].conflict > s[a[1]].conflict)
	{
		temp = s[a[0]].V;
	}
	else
	{
		temp = (rand() % 2 == 0) ? s[a[0]].V : s[a[1]].V;//两者相等时随机选择一个
	}
	if (temp == s[a[0]].V) {
		s[a[0]].conflict = S.conflict;
	}	
	else
		s[a[1]].conflict = S.conflict;
	for (int i = 0; i < G.numVertex; i++)
	{
		temp[i] = S.V[i];
	}

}

Population::~Population()
{
	if (subset)
		delete[] subset;
	if (a)
		delete[] a;
}
