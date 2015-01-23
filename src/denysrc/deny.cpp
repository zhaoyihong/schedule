#include "deny.h"
//#include "hungarian.hpp"
#include "munkres.h"

typedef vector<int> iv;
typedef vector<double> dv;

Deny::Deny(int total,char *path,int maxloop):total(total),path(path),maxloop(maxloop){
	init();
}

Deny::~Deny(){
}


void Deny::init()
{
	cout << "init" << endl;
	loop=0; //使用第0阶段的值进行初始化
	total_cost = 0.0;
	

	dv icost;
	iv ischedule;
	for(int i=0;i<total;++i)
	{
		ischedule.push_back(i);
	}
	
	random_shuffle(ischedule.begin(),ischedule.end()); //打乱顺序

	min_cost = 0;	
	for(int i=0;i<total;i++) //计算开销
    {
		double cost_for_app = getValue(i,ischedule[i]);
        icost.push_back(cost_for_app);
        min_cost += cost_for_app;
	}

	schedule_now = ischedule;
	
	//初始化各个cost的life
	for(int i=0;i < total; ++i)
	{
		life.push_back(vector<int>(total,LIFE_MAX));
	}

	/*
		last_cost 
	*/
	//初始化
	for(int i  = 0 ; i< total ; ++ i) //app
	{
		last_cost.push_back(vector<double>(total,-1.0));
	}
	
	//对last_cost进行初始化

	for(int i  = 0 ; i< total ; ++ i) //core
	{
		for(int j= 0 ; j< total ; ++j) //app
		{
			last_cost[i][j] = getValue(i,j);	
		}
	}


	loop=1;//正式开始时,从loop1开始	
}


double Deny::getValue(int cpu,int app)
{
        ifstream in(path);
        if(!in)
		{
		 cout << "cannot open " << path  << " ! exit!" << endl;
		 exit(0); 
		}

        int cnt=0;
        string line;
        int endline=loop*(total)+app +loop+1 ;
        while(cnt != endline)
        {
                getline(in,line);
                cnt++;
        }
		getline(in,line);
	
        cnt=0;
        istringstream is(line);
        double value=0;
        while(cnt != cpu)
        {
                is >> value;
                cnt++;
        }
        is >> value;
	
   	//cout << "getValue(" << loop << "," << cpu << ","  << app << ") : "  << value <<  endl;
        return value;

}


//打印当前的调度方案及其开销
void Deny::printCurrentSchedule()
{
	for(int i =0 ;i< total ; ++i)
	{
		cout << schedule_now[i] << " ";
	}
	cout << ",cost= " << min_cost << endl;

}

//打印上一循环中生成的调度方案及开销
void Deny::printStageSchedule()
{
	for(int i =0 ;i< total ; ++i)
	{
		cout << schedule_history.back().at(i) << " ";
	}
	
	cout << ",cost= " << total_cost_history.back() << endl;

}
//打印history
void Deny::printHistory()
{
	for(int i = 0; i < maxloop ; ++i)
	{
		cout << "第" << i+1 << "轮" << endl;
		
		cout <<  "调度方案 :" << endl;
		for(int j =0; j < total; ++ j)
		{
			cout << schedule_history[i][j] << " ";
		}
		cout << endl;
		
		cout << "开销分别为:" << endl;
		for(int j =0; j < total; ++ j)
        {
            cout << cost_history[i][j] << " ";
        }
		cout << endl;
		cout << "本轮总开销为: " << endl;
		
		cout  << total_cost_history[i] << endl;
		cout << endl;
	}
}

void Deny::printResult()
{
//	cout << "____________________________" << endl;
//	cout << "|  总开销为:" <<  total_cost << "  |"<<  endl;
//	cout << "----------------------------" << endl;

	cout << total_cost << endl;
}

void Deny::printChengji()
{
	cout << "last cost 矩阵:" << endl;
	for(int i=0; i< total ; ++i)
	{
		for(int j =0 ; j < total ;++j)
		{
			cout << last_cost[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	
	
	cout << "life 矩阵:" << endl;
	for(int i=0; i< total ; ++i)
	{
		for(int j =0 ; j < total ;++j)
		{
			cout << life[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	

}


double Deny::getLastCost(int app,int core)
{
	for(int i=loop; i>=0 ;--i) //在之前的循环中
	{
		if( schedule_history[i][core] == app)
		{
			return cost_history[i][core];
		}
	}

	return -1;
}

/*
	主要思想是依据之前的开销进行随机调度
*/

void Deny::start()
{
	for(loop=1; loop<=maxloop ; ++loop )
	{
		stage(loop);	
	//	printCurrentSchedule();
	//	printStageSchedule();
	//	total_cost += min_cost;
		printChengji();		
	//	cout << "total cost : "  << total_cost << endl << endl;
	}
}

//使用匈牙利算法对last_cost矩阵计算开销
double Deny::useHungarian(vector<int> & choosen)
{
		Matrix<double> matrix(total, total);
		for(int i =0; i < total; ++i)
		{
			for(int j=0; j < total; ++j)
			{
				matrix(i,j) = last_cost[i][j];
			}
		}

		
		Munkres m;
		m.solve(matrix);
		

		//计算choosen矩阵
		
		
		double sum = 0;
		for(int i = 0; i < total; ++i)
		{
		  for(int j =0; j < total; ++j)
		  {
				if(matrix(i,j) == 0)
				{
					choosen[i]=j;
					sum += last_cost[i][j];
					break;
				}
		  }
		}

		return sum;

}

double Deny::useHungarian_withlife(vector<int> & choosen)
{
		Matrix<double> matrix(total, total);
		for(int i =0; i < total; ++i)
		{
			for(int j=0; j < total; ++j)
			{
				matrix(i,j) = life[i][j];
			}
		}

		
		Munkres m;
		m.solve(matrix);
		

		//计算choosen矩阵
		
		
		double sum = 0;
		for(int i = 0; i < total; ++i)
		{
		  for(int j =0; j < total; ++j)
		  {
				if(matrix(i,j) == 0)
				{
					choosen[i]=j; 
					sum += last_cost[i][j];
					break;
				}
		  }
		}

		return sum;
}
//选出新的方案,并更新min_value 和 schedule_now
void Deny::stage(int loop)
{

	//更新life 和 cost
	for(int i=0;i<total;++i)
	  for(int j=0;j<total;++j)
	  {
		if(life[i][j]>0)
		  life[i][j]--;
		else
		  last_cost[i][j]=0.0;
	  }


/*
	iv v0;
	for(int i=0;i < total; ++i)
	{
		v0.push_back(i);
	}
	
	vector<vector<int> > vlist; //随机生成的4种方案的容器
	for(int i = 0 ; i< total; ++ i)
	{
		vlist.push_back(v0);
	}
	
	//打乱顺序
	for(vector<vector<int> >::iterator it = vlist.begin(); it != vlist.end(); ++ it)
	{
		random_shuffle(it->begin(),it->end()); //打乱顺序
	}
	
	int min_id = 0;
	double min_cost = 10000.0;
	//计算开销最低
	// 再加点随机性? 
	for(int k = 0 ; k < total ; ++ k) // vlist 方案号
	{
		double  c  = 0; //cost
		for(int i = 0; i < total ; ++i) //core
		{
			int app = vlist[k][i];
			//c += chengji[i][app];
			c += last_cost[i][app];
		}

		if(c  < min_cost )
		{
			min_id = k; 
			min_cost = c;
		}
	}

*/

	cout << "第" << loop << "轮调度计算开始:" << endl;
	//使用匈牙利算法计算choosen和开销
	iv choosen(total); //匈牙利算法算出来的开销
	
	/*
	//if(random()%2 == 0) //使用随机算法生成解决方案 
	if(loop%4 == 1) //使用随机算法生成解决方案 
	{
		cout << "use random" << endl;
		for(int i = 0 ; i < total; ++i)
		{
			choosen[i] = i;
		}
		random_shuffle(choosen.begin(),choosen.end());
	}
	else if(loop%4 ==1)
	{
	
	}
	else //使用匈牙利生成解决方案
	{
		cout << "use hungry" << endl;
		useHungarian(choosen);	
	}
	
	*/

	switch(loop%4)
	{
		case 3://匈牙利算法 
			cout << "use hungarian" << endl;
			useHungarian(choosen);	
			break;
		/*
		case 2://随机算法
			cout << "use random" << endl;
			for(int i = 0 ; i < total; ++i)
			{
				choosen[i] = i;
			}
			random_shuffle(choosen.begin(),choosen.end());
			break;
		*/
		default:
			//life调度: 思路是对life矩阵进行匈牙利算法,进行调度
			//本意就是: 找到life最小的矩阵,尽可能的快过期的last_cost进行更新
			cout << "use hungarian with life" << endl;
			useHungarian_withlife(choosen);	
			;
	}

	
	//cout << "min cost:" << min_cost << endl; 

	
	//计算开销
	//总开销
	double  cost_this_loop = 0.0; //使用choosen方案的cost
	double  cost_this_loop_old = 0.0; //使用上一轮方案的cost schedule_now
	//各个app的开销
	dv vcost_this_loop;
	dv vcost_this_loop_old;
	for(int i=0; i< total; i++)
	{
	     	double cost_for_app = getValue(i,choosen[i]);
            vcost_this_loop.push_back(cost_for_app);
            cost_this_loop += cost_for_app;	


	     	double cost_for_app_old = getValue(i,schedule_now[i]);
            vcost_this_loop_old.push_back(cost_for_app_old);
            cost_this_loop_old += cost_for_app_old;	
	}

	//比较开销,如果较小才采用新方案,否则schedule_now还是上轮的
	//2014-11-24 这里用last_cost代替当前的开销不合适,应该用本轮的开销
	
	//更新最新开销
	for(int i = 0 ;i < total ; ++ i) //内核
	{
		int app;
		double cost;
	
		app= choosen[i];	 //i 内核上的app
		cost = vcost_this_loop[i]; // app 在 i核上的cost		
		last_cost[i][app]= cost;// 更新最新的cost
		life[i][app]=LIFE_MAX; //更新这个开销的有效性.暂时life最大值是total
	
		app= schedule_now[i];	 //i 内核上的app
		cost = vcost_this_loop_old[i]; // app 在 i核上的cost		
		last_cost[i][app]= cost;// 更新最新的cost
		life[i][app]=LIFE_MAX; //更新这个开销的有效性.暂时life最大值是total
	}


	//当新的调度方案优于旧的调度方案
	cout << "new:" << cost_this_loop << " ";
	cout << "old:" << cost_this_loop_old << endl;
	if(cost_this_loop < cost_this_loop_old)
	{
	//	min_cost = cost_this_loop; //设置为最小开销
		schedule_now = choosen; //设定为当前调度方案
		total_cost += cost_this_loop;

		schedule_history.push_back(schedule_now);
		cost_history.push_back(vcost_this_loop);
		total_cost_history.push_back(cost_this_loop);
	} 
	else
	{	//旧的更好
		total_cost += cost_this_loop_old;
		schedule_history.push_back(schedule_now);	
		cost_history.push_back(vcost_this_loop_old);
		total_cost_history.push_back(cost_this_loop_old);

	}

	
	//计算总开销
	/*
	for(int i = 0 ;i < total ; ++ i) //内核
	{
		int app = choosen[i];	 //i 内核上的app
		double cost = vcost_this_loop[i]; // app 在 i核上的cost		
		total_cost += cost; //总开销
	}
	*/

	/*
	//横着更新成绩
	for(int i= 0 ; i < total ; ++i) //内核
	{
		multimap<double,int> sort_map;
		for(int j =0 ; j < total ; ++ j) //app
		{
			if(last_cost[i][j] > 0) //如果有成绩
			{
				sort_map.insert(make_pair<double,int>(last_cost[i][j],j)); //set自动从小到大排序
			}
		}

		//排序之后重新打分,从90开始递减5分
		int value = 1000;
		for(multimap<double,int>::iterator it =sort_map.begin(); it != sort_map.end(); ++it)
		{
			value -= total;	
			int app = it->second;
			chengji[i][app] = value; 
		}
	}

	//竖着更新成绩(同一app在不同核上的排序)
	for(int i=0; i < total ; ++ i) //app
	{
		multimap<double,int> sort_map;
		for(int j=0; j< total ; ++ j) //内核
		{
			if(last_cost[j][i] > 0) //如果有成绩
				sort_map.insert(make_pair<double,int>(last_cost[j][i],j)); //set自动从小到大排序
		}
		
		//排序之后重新打分,各递减1分
		int value = 1;
		for(multimap<double,int>::iterator it =sort_map.begin(); it != sort_map.end(); ++it)
		{
			int core = it->second;
			chengji[core][i] -= value;
			value++;
		}

	}
		
	*/
	

}


template <typename T>
void printArray(T array,int total)
{
	for(int j=0;j<total;j++)
	{
		cout << array[j] << " " ;		
	}	
	cout << endl;
}



int getIntRandom(int min,int max)
{
	assert(min < max);
	return random()%(max-min+1)+min;

}


void printUsage()
{
	cout << "usage :" << endl;
	cout << "./a.out path total maxloops" << endl;
}


int main(int argc,char *argv[])
{
	if(argc  != 4)
	{
		printUsage();
		return -1;
	}	

	time_t t=time(NULL);
	srandom(t);
	
	char *path = argv[1];
	int total = atoi(argv[2]);	
	int maxloop = atoi(argv[3]);	

	Deny deny(total,path,maxloop);
//	deny.init();
	deny.start();		
	deny.printHistory();
	deny.printChengji();
	deny.printResult();

	return 0;
}

