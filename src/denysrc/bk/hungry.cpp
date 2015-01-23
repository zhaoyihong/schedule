


#include<iostream>
#include<stdlib.h>
#include<ctime>
#include<stdio.h>
#include<fstream>
#include<vector>
//#include"hungry.h"

using namespace std;


const int N = 4;

//void hungry(double (* juzhen)[N]);
//double hungry(vector<vector<double>>&,vector<int>&,int size);
/*
int main(int argc,char **argv)
{

	//double juzhen[N][N];
	vector<vector<double>> juzhen;
	

	vector<int> choosen(N);
	int i, j;
	
	vector<double> v(N,0);
	for(i=0;i<N;i++)
	{
		juzhen.push_back(v);
	}

	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
		{
			cin >> juzhen[i][j];
		}

	//juzhen[i][j] = rand()/(RAND_MAX*0.1);
	
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
			cout << juzhen[i][j] << " ";
		cout << endl;
	}
	
	//先生成一个15*15的矩阵


	int sum = hungry(juzhen,choosen,N);

	for(int i = 0; i <N; ++i)
	{
		cout << choosen[i] << " ";
	}
	cout << endl;

	cout << sum << endl;

	return 0;
}
//void hungry(double (* juzhen)[N])
*/		

double hungry(vector<vector<double>>& juzhen,vector<int>& count,int size)
{
		for(int i = 0 ; i < size ; ++i)
		{
			for(int j = 0 ; j < size ; ++j)
			{
				cout << juzhen[i][j] << " ";
			}
			cout << endl;
		}


	//double start, finish;
	int i, j;
//	start = (double)clock();

	double min = 0, sum = 0;
	double max;
	int k, l, t, m=0, n;
	
	int same[size], compare[size];
	for (i = 0; i < size; i++)
	{
		min = juzhen[i][0];
		l = 0;
		for (j = 0; j < size; j++){
			if (juzhen[i][j] < min)
			{
				min = juzhen[i][j];
				l = j;
			}
		}
		count[i] = l;
	}
	/*
	//找出每一行的最小值，并记住每一行的最小值的列数
	for (i = 0; i < size; i++)
	cout << count[i] << " ";
	cout << endl;
	*/

	cout << "1" << endl;
	//将每一行最小数的列数输出
	for (i = 0; i < size; i++)
	{
		t = count[i];
		k = 0;
		for (j = 0; j < size; j++)
		{
			if (t == count[j])
				k++;
		}
		same[i] = k;
	}
	//检查每一行最小数在相同一列的次数
	/*
	for (i = 0; i < size; i++)
		cout << same[i] << " ";
	cout << endl;
	*/
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		if (i == count[j]) break;
		if (j == size)
			compare[i] = 0;
		else
			compare[i] = 1;
	}
	cout << "2" << endl;
	/*
	//检查哪列有最小值哪列没有，并标记。
	for (i = 0; i < size; i++)
	cout << compare[i] << " ";
	cout << endl;
	*/

	for (i = 0; i < size; i++)
	{
		cout << "3" << endl;
 		if (compare[i] == 0)
		{
			cout << "4" << endl;
			for (j = 0; j<size; j++)
			{
				cout << "5" << endl;
				if (same[j]>1)
				{
					max = 0;
					for (k = 0; k < size; k++)
					{
						if (count[k] == count[j])
						if (max < juzhen[k][count[k]])//找出同一列中各行最小数当中最大的数字，兵把他付给没有最小数的该列。
						{
							max = juzhen[k][count[k]];
							m = k;
						}
					}
				
				}
			}
		//	count[m] = i;
		}
		for (n = 0; n < size; n++)
		{
			t = count[n];
			k = 0;
			for (j = 0; j < size; j++)
			{
				if (t == count[j])
					k++;
			}
			same[n] = k;
		}
		for (n = 0; n < size; n++)
		{
			for (j = 0; j < size; j++)
			if (n == count[j]) break;
			if (j == size)
				compare[n] = 0;
			else
				compare[n] = 1;
		}
	}


	for (i = 0; i < size; i++)
	{
		cout << "第" << i + 1 << "行 第" << count[i] + 1 << "列为" << juzhen[i][count[i]] << endl;
		sum = sum + juzhen[i][count[i]];
	}


	//cout  << sum << endl;
	return sum;
//	finish = (double)clock();
	//printf("运行时间:%.4fms\n", (finish - start) / 1000);
}


