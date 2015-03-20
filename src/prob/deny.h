
#ifndef DENY_H
#define DENY_H


#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <cmath> 
#include <vector>
#include <map>
using namespace std;


class Deny
{
public:
	Deny(int total,char *path,int maxloop);
	~Deny();
	void start(); //开始调度
	void printResult(); //打印结果
	void printCurrentSchedule(); //打印当前调度方案
	void printStageSchedule();
	void printHistory();
	void printChengji(); //打印last_cost 和chengji 矩阵
private:
    int total;  //cpu数量
	char *path; //matrix文件存放路径
	int loop;  //local search loop数
	int maxloop;  //最大loop数
	vector<vector<double> > cost_history;  //存放调度方案对应的开销     
	vector<vector<int> > schedule_history; //存放调度方案
	vector<double> total_cost_history;
	vector<vector<int> > chengji; //app在各个内核上的成绩 	
	vector<vector<int> > life; //app在各个内核上的cost的有效性	
	vector<vector<double> > last_cost; //app在各个内核上最进的一次开销
	vector<int> schedule_now; //当前的调度方案

    void init(); //初始化
	double getValue(int cpu ,int app); // get value from file
	double total_cost;	//总的开销
	double min_cost; //当前调度方案的开销
	void stage(int loop);//执行一轮循环
	const int LIFE_MAX = total + total/2 ;
    void get_schedule_use_probability(vector<int>& choosen);//依赖概率的调度方法


public :
	//得到history中	最近的一次 app在 core上的开销
	double getLastCost(int app,int core);	

};


template <typename T>
void printArray(T,int); // print 2d arry for debug
int getIntRandom(int min,int max); // get random integer in [ min ,max)
//double getHungryResult(const char *cmd);


#endif

