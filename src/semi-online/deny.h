
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

class swap_pair
{
public:

    swap_pair(int c1,int a1,int c2,int a2)
    {
        app1 = a1;
        core1 = c1;
        app2 = a2;
        core2 = c2;
        valid =true;
    }

    int app1;
    int core1;
    int app2;
    int core2;

    bool valid;
    /**
     * 比较两个交换对
    * @ s同对象比较的交换对
    * 返回值 1 不相等， 0 相等， -1 相反
    * 举例： swap_pair(1,2,3,4) 和自己以及 swap_pair(3,4,1,2)相等 
    *        swap_pair(1,2,3,4) 和 swap_pair(1,4,3,2) , swap_pair(3,2,1,4) 相反
    */
    int cmp(swap_pair& s)
    {

        if(app1 == s.app1 && core1 == s.core1 && app2 == s.app2 && core2 == s.core2)
        {
            return 0;
        }
        
        if(app1 == s.app2 && core1 == s.core2 && app2 == s.app1 && core2 == s.core1)
        {
            return 0;
        }

        if(core1 == s.core1 && core2 == s.core2 && app1 == s.app2 && app2 == s.app1)
        {
            return -1;
        }
 
        if(core1 == s.core2 && core2 == s.core1 && app1 == s.app1 && app2 == s.app2)
        {
            return -1;
        }

        return 1;

    }

    //修改为相反的
    void reverse()
    {
        int temp = app1;
        app1 = app2;
        app2 = temp;
    }

    
};

ostream& operator<<(ostream &out,swap_pair& s)
{
    out << s.core1 << ":" << s.app1 << "," << s.core2 << ":" << s.app2 << "," << boolalpha << s.valid;
    return out;
}

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
	vector<int> schedule_now; //当前的调度方案

    void init(); //初始化
	double getValue(int cpu ,int app); // get value from file
	double total_cost;	//总的开销
	void stage(int loop);//执行一轮循环
    void get_schedule_use_search(void);//采用两两交换的局部搜索 ,交换对在last_swap中


    void swap_over(); //比较真实的swap_pairs中的值,哪些交换后小的,就交换
    void pre_swap_over(); // 只获取交换对
    
    vector<swap_pair> current_pairs;
    vector<swap_pair> tabu_pairs;
    vector<swap_pair> bad_pairs;//交换后cost更大的交换对

    void update_tabu_pairs();
    void check_current_swap_pairs();

public :
	//得到history中	最近的一次 app在 core上的开销
	double getLastCost(int app,int core);	
    int swap_cnt = 0; //交换的次数
    int deny_cnt = 0; //防止交换的次数    

    //应用编号
    vector<int> bin_indexs;

private:
    vector<vector<double> > cost_datas; //应用之间的开销差距
};


template <typename T>
void printArray(T,int); // print 2d arry for debug
int getIntRandom(int min,int max); // get random integer in [ min ,max)
//double getHungryResult(const char *cmd);


#endif

