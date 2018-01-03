#include "Population.h"
#include "TabuSearch.h"
#include <ctime>
#include <iostream>
#include <cstdio>

using namespace std;
int Best_conflict = 10000;
Population::Population(Graph &G, int Color_num,TabuSearch Tabu) :subset(new int[Color_num]), Best_soulation(new int[G.numVertex]),Pop_num(6),a(new int[2]) //������Ⱥ�������
{
	InitPopulation(G, Tabu,Color_num);

}

int Population::InitPopulation(Graph &G, TabuSearch Tabu,int  Color_num) {//��ʼ����Ⱥ��
	int i, j = 0, k = 0, m = 0;
	cout << Color_num << endl;
	for (k = 0; k < Pop_num; k++) {
		srand((unsigned int)clock());
		for (i = 0; i < G.numVertex; i++) {
			s[k].V[i] = rand() % Color_num;//��ʼ��Ⱥ��
		}
		
		s[k].conflict= Tabu.C_TabuSearch(G, s[k].V, Color_num);//���������Գ�ʼ���Ż�
	}
	return 1;
}

void Population::HybridSearch(Graph &G,int count,TabuSearch Tabu, int  Color_num) {
	clock_t start_time = clock();//��ʼʱ��
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
			cout << endl << "time:" << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;//���㲢�������ʱ��
			start_time = end_time;
			clock_t temp = clock();
			cout << "The Crossover_count: " << count << endl;
			InitPopulation(G, Tabu, Color_num);
			start_time = end_time+(clock()-temp);
			count = 1;
			ChooseParents(G, Color_num, s1, s2);
			Crossover(G, Color_num, s1, s2, S);
			S.conflict = Tabu.C_TabuSearch(G, S.V, Color_num);
			cout << S.conflict << " ";//�ظ�����
		}
		UpdatePopulation(G, Color_num,S);
		if (S.conflict < Best_conflict)				   //�������Ž�
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
	srand(time(NULL));//ͨ����ȡʱ��ֵ���趨��������ӣ��Ա�֤ÿ�����н����ͬ��
	a[0] = rand() % Pop_num;
	a[1] = rand() % Pop_num;
	while (a[0] = a[1]) {
		a[1]= rand() % Pop_num;
	}
	/*for (i = 0; i < 2; )//�ж�ÿ�εõ�����ֵ�Ƿ��ظ������ظ��򲻽���i���Լ�
	{
		a[i] = rand() % Pop_num; //�õ�һ��0-Pop_num�е��������
		for (j = 0; j < i; j++)
			if (a[j] == a[i])break;//�����ظ���������ѭ��

		if (j == i) i++;//���i��j��ȣ�������ѭ���Ǵﵽ�����˳��ģ�������ͨ��break,Ҳ����û���ظ��
	}*/
	for (i = 0; i < G.numVertex; i++) {
		s1.V[i] = s[a[0]].V[i];//��ʼ��s1����
		s2.V[i] = s[a[1]].V[i];//��ʼ��s2����
	}


}

void Population::Crossover(Graph &G, int  Color_num,Pop_set &s1,Pop_set &s2,Pop_set &S) {
	int i, item = 0, j, k;
	int max_InSubset_num;//����������ģֵ
	int max_InSubset_color;//������������ɫֵ
	int *temp = new int[G.numVertex];
	int sameSet_number;//������������ͬ�ĸ�������
	//ChooseParents(G);
	for (i = 0; i < Color_num; i++) {//��s1��s2������ȡ�������Ӽ����������Ӽ�
		max_InSubset_num = 0;
		if (Color_num % 2 != 0)
			temp = s1.V;
		else {
			temp = s2.V;
		}
		for (j = 0; j < Color_num; j++) {
			subset[j] = 0;//��ʼ��������С���±��ʾ��ɫ����ֵ��ʾ����ɫ��Ӧ�Ķ�������������ϴ�С��
		}
		for (j = 0; j < G.numVertex; j++)                        //����������Ӽ��Ĵ�С
		{
			subset[temp[j]]++;
		}
		for (j = 0; j < Color_num; j++)                           //ͳ�����������Ĵ�С�����Ӧ����ɫ
		{
			if (subset[j] > max_InSubset_num)
			{
				sameSet_number = 1;
				max_InSubset_num = subset[j];
				max_InSubset_color = j;
				
			}
			else if (subset[j] == max_InSubset_num) {
				sameSet_number++;
				double p = (double)rand() / RAND_MAX;//ˮ���������ѡ��
				if (p < double(1 / sameSet_number)) {
					max_InSubset_num = subset[j];
					max_InSubset_color = j;
				}
			}
			//cout <<"max_InSubset_color:"<< max_InSubset_color << endl;
		}
		for (j = 0; j < G.numVertex; j++) {//���������е�Ԫ�ش����Ӵ���ɾ���������Ѵ�Ԫ��
			if (temp[j] == max_InSubset_color) {//�ҵ���Ӧ��Ԫ��
				S.V[j] = max_InSubset_color;//�Ӵ���ֵ
				s1.V[j] = -1;//��Ǹ����д�Ԫ��
				s2.V[j] = -1;
			}
			
		}
	}
		for (int i = 0; i < G.numVertex; i++)     //Ѱ������������ʣ���ֵ����������䵽�Ӵ���
		{
			if ((s1.V[i] != (-1)) && (s2.V[i] != -1)){
				S.V[i] = rand() % Color_num;
			}
		}

}

void Population::UpdatePopulation(Graph &G, int  Color_num,Pop_set &S) {//������һ����

	int *temp=new int[G.numVertex];
	temp = 0;
	if (s[a[0]].conflict < s[a[1]].conflict)                            //�жϸ����нϲ�Ľ�
	{
		temp = s[a[1]].V;
	}
	else if (s[a[0]].conflict > s[a[1]].conflict)
	{
		temp = s[a[0]].V;
	}
	else
	{
		temp = (rand() % 2 == 0) ? s[a[0]].V : s[a[1]].V;//�������ʱ���ѡ��һ��
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
