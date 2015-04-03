#include "deny.h"
#include    "sys/time.h"

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
		last_cost.push_back(vector<double>(total,0.0));
	}

    /*
	//对last_cost进行初始化

	for(int i  = 0 ; i< total ; ++ i) //core
	{
		for(int j= 0 ; j< total ; ++j) //app
		{
			last_cost[i][j] = getValue(i,j);	
		}
	}
    */

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

	cout << "第" << loop << "轮调度计算开始:" << endl;
	iv choosen(total); //匈牙利算法算出来的开销

    cout << "use robin" << endl;

    for(int i=0;i<total-1;++i)
    {
        choosen[i+1] = schedule_now[i];
    }
    choosen[0] = schedule_now[total-1];
	
	//计算开销
	//总开销
	double  cost_this_loop = 0.0; //使用choosen方案的cost
	//各个app的开销
	dv vcost_this_loop;
	for(int i=0; i< total; i++)
	{
	     	double cost_for_app = getValue(i,choosen[i]);
            vcost_this_loop.push_back(cost_for_app);
            cost_this_loop += cost_for_app;	
	}


        cout << "new:" << cost_this_loop << " ";
		
        schedule_now = choosen; //设定为当前调度方案
		total_cost += cost_this_loop;

		schedule_history.push_back(schedule_now);
		cost_history.push_back(vcost_this_loop);
		total_cost_history.push_back(cost_this_loop);

	

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


       
    struct timeval tv_start,tv_end;
     gettimeofday(&tv_start,NULL);
    deny.start();		
     gettimeofday(&tv_end,NULL);
        double time_total = tv_end.tv_sec-tv_start.tv_sec+(tv_end.tv_usec-tv_start.tv_usec)/1000000.0;


    deny.printHistory();
	deny.printChengji();
    cout << time_total << endl;
	deny.printResult();

	return 0;
}

