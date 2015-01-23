
#ifndef LOCAL_H
#define LOCAL_H


#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <cmath> 
using namespace std;


class Local
{
public:
	Local(int,char *path,int maxloop);
	~Local();
        void init(); //初始化
	void start(); //开始调度
	double result(); //打印结果
	void printCurrentSchedule();

private:
        int size;  //cpu数量
	int total; 
	int maxloop;
	int loop;  //local search loop数
	char *path; //matrix文件存放路径
	double* data;  //存放调度方案对应的开销     
	int* schedule; //存放调度方案
	
	double* data_bk;  //存放调度方案对应的开销     
	int* schedule_bk; //存放调度方案
	
	void initSchedule(); //初始化调度方案
	void initData();  //初始化调度方案对应开销
	double getValue(int cpu ,int app); // get value from file
	//4个阶段的交换
	void stepA();	
	void stepB();	
	void stepC();	
	void stepD();
	bool swap(int ,int);
	double getCurrentCost(int isbk = 0 ); //获得本轮的开销
	double total_cost;	//总的开销
};


template <typename T>
void printArray(T,int); // print 2d arry for debug
int getIntRandom(int min,int max); // get random integer in [ min ,max)



#endif

