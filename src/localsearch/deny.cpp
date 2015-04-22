#include "deny.h"
#include  <sys/time.h>


typedef vector<int> iv;
typedef vector< vector<int> > iiv;
typedef vector<double> dv;
typedef vector< vector<double> > ddv;
Deny::Deny(int total,char *path,int maxloop):total(total),path(path),maxloop(maxloop){
	init();
}
Deny::~Deny(){
}
void Deny::init()
{
//	cout << "init" << endl;
	loop=0; //使用第0阶段的值进行初始化
	total_cost = 0.0;
	dv icost;
	iv ischedule;
	for(int i=0;i<total;++i)
	{
		ischedule.push_back(i);
	}

    
    #ifdef RANDOM_START     
    random_shuffle(ischedule.begin(),ischedule.end()); //打乱顺序
    #endif


    min_cost = 0;	
	schedule_now = ischedule;
	//初始化各个cost的life
	for(int i=0;i < total; ++i)
	{
		//life.push_back(vector<int>(total,LIFE_MAX));
		life.push_back(vector<int>(total,0));
	}
	/*
		last_cost 
	*/
	//初始化
	for(int i  = 0 ; i< total ; ++ i) //app
	{
		last_cost.push_back(vector<double>(total,0.0));
	}
    
    //search函数中用到的.上一次交换记录
    last_swap.assign(total,-1); 
	
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
//		cout << "第" << i+1 << "轮" << endl;
//		cout <<  "调度方案 :" << endl;
		for(int j =0; j < total; ++ j)
		{
			cout << schedule_history[i][j] << " ";
		}
		cout << endl;
	//	cout << "开销分别为:" << endl;
		for(int j =0; j < total; ++ j)
        {
  //          cout << cost_history[i][j] << " ";
        }
//		cout << endl;
//		cout << "本轮总开销为: " << endl;
		cout  << total_cost_history[i] << endl;
//		cout << endl;
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
//		printChengji();		
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

   // cout << "第" << loop << "轮调度计算开始:" << endl;
	//使用匈牙利算法计算choosen和开销
	iv choosen(total); //新的算法的开销
    choosen  = schedule_now;

    get_schedule_use_search();
    swap_cnt += total/2;

    //将choosen变为交换的
    for(int i=0;i<total;++i)
    {
       choosen[i] = schedule_now[last_swap[i]]; 
    }

   // cout << "choosen:" << endl;
 //   printArray(choosen,choosen.size());
   // cout << "old:" << endl;
   // printArray(schedule_now,schedule_now.size());
    
    //cout << "min cost:" << min_cost << endl; 
	//计算开销
	//总开销
	double  cost_this_loop = 0.0; //使用choosen方案的cost
	double  cost_this_loop_old = 0.0; //使用上一轮方案的cost schedule_now
	//各个app的开销
	dv vcost_this_loop; //choosen对应的开销
	dv vcost_this_loop_old; //schedule_now对应的开销
	for(int i=0; i< total; i++)
	{
        double cost_for_app = getValue(i,choosen[i]);
        vcost_this_loop.push_back(cost_for_app);
        cost_this_loop += cost_for_app;	
        double cost_for_app_old = getValue(i,schedule_now[i]);
        vcost_this_loop_old.push_back(cost_for_app_old);
        cost_this_loop_old += cost_for_app_old;	
	}
    

	//更新life矩阵和last_cost矩阵
	for(int i = 0 ;i < total ; ++ i) //内核
	{
		int app1,app2;
		double cost1,cost2;
		app1= choosen[i];	 //i 内核上的app
        cost1 = vcost_this_loop[i]; // app 在 i核上的cost		
		
        app2= schedule_now[i];	 //i 内核上的app
        cost2 = vcost_this_loop_old[i]; // app 在 i核上的cost		
		
        last_cost[i][app1]= cost1;// 更新最新的cost
        last_cost[i][app2]= cost2;// 更新最新的cost

        life[i][app1]=LIFE_MAX; //更新这个开销的有效性
        life[i][app2]=LIFE_MAX; //更新这个开销的有效性
       
    }
	//当新的调度方案优于旧的调度方案
//	cout << "new:" << cost_this_loop << " ";
//	cout << "old:" << cost_this_loop_old << endl;
	if(cost_this_loop < cost_this_loop_old)
	{
		min_cost = cost_this_loop; //设置为最小开销
		schedule_now = choosen; //设定为当前调度方案
		total_cost += cost_this_loop;
		schedule_history.push_back(schedule_now);
		cost_history.push_back(vcost_this_loop);
		total_cost_history.push_back(cost_this_loop);
	    //swap_cnt += total/2;
    } 
	else
	{	//旧的更好.search由于是swap交换的,比较swap

        swap_over(); //根据last_swap对,对schedule_now进行改进
  //      cout << "swap over:" << endl;
//    printArray(schedule_now,schedule_now.size());

        vcost_this_loop_old.clear();

        double cost_swap = 0.0;//本轮开销
        for(int i=0; i< total; i++)
        {
            double cost_for_app = getValue(i,schedule_now[i]);
            vcost_this_loop_old.push_back(cost_for_app);
            cost_swap += cost_for_app;	
        }
       
//        cout << "cost:" << cost_swap << endl; 
		min_cost = cost_swap; //设置为最小开销
		total_cost += cost_swap;
		schedule_history.push_back(schedule_now);	
		cost_history.push_back(vcost_this_loop_old);
		total_cost_history.push_back(cost_swap);
	}
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

void Deny::get_schedule_use_random(vector<int> &choosen)
{
    for(int i=0;i<total;++i)
	{
		choosen[i] = i;
	}
    
    random_shuffle(choosen.begin(),choosen.end()); 
}


void Deny::get_schedule_use_search(void)
{

    swap_pairs.clear();

    iv cores;

    for(int i=0;i<total;++i)
    {
        cores.push_back(i);
    }

    random_shuffle(cores.begin(),cores.end());

    iv be_choosed(total,0);

    //只有前面的一半的核才可以选择交换的对象
    
    int core1,swapid;
    for(int i=0;i<total/2;++i)
    {
        core1 = cores[i];

        while(1)
        {
            int j = getIntRandom(total/2,total-1);
            swapid = cores[j];

            if(0 == be_choosed[swapid])
            {
                be_choosed[swapid] = 1;
                break;
            }
        }
   
        assert(swapid != -1);
        //记录下来,作为这次的交换对
        last_swap[core1] = swapid;
        last_swap[swapid] = core1;
       // cout << "swap" << core1 << " " << swapid << endl;
        swap_pairs.insert(pair<int,int>(core1,swapid));
    
    }

}



void Deny::swap_over()
{

    int cnt =0;
    for(map<int,int>::iterator it = swap_pairs.begin(); it != swap_pairs.end(); ++it)
    {
        int core1 = it->first;
        int core2 = it->second;
        int app1 = schedule_now[core1];
        int app2 = schedule_now[core2];

        double cost_now = getValue(core1,app1) + getValue(core2,app2);
        double cost_swap = getValue(core1,app2) + getValue(core2,app1);

        if(cost_now > cost_swap) //交换后的开销更小.
        {
         //   cout << "in swap:" << core1 << " " << core2 << endl;
            //交换core1和core2上的app
            schedule_now[core1] = app2;
            schedule_now[core2] = app1;
            cnt++;
        }
    }
    swap_cnt += (total/2-cnt);//cnt是指保留的交换，total/2-cnt是指又要进行的交换。



}

double Deny::get_swap_cost(int i,int j)
{
    int appid1 = schedule_now[i];
    int appid2 = schedule_now[j];
    double cost_after = last_cost[i][appid2] + last_cost[j][appid1];

    return cost_after;

}

int getIntRandom(int min,int max)
{
	assert(min <= max);
	return random()%(max-min+1)+min;
}
void printUsage(const char *binname)
{
	cout << "usage :" << endl;
	cout << binname << " path total maxloops" << endl;
}

int main(int argc,char *argv[])
{
	if(argc  != 4)
	{
		printUsage(argv[0]);
		return -1;
	}	
	time_t t=time(NULL);
	srandom(t);
	char *path = argv[1];
	int total = atoi(argv[2]);	
	int maxloop = atoi(argv[3]);	
	Deny deny(total,path,maxloop);
    
    struct timeval tv_start,tv_end;
     gettimeofday(&tv_start,NULL);
    deny.start();		
     gettimeofday(&tv_end,NULL);
    double time_total = tv_end.tv_sec-tv_start.tv_sec+(tv_end.tv_usec-tv_start.tv_usec)/1000000.0;

    
    deny.printHistory();
    //deny.printChengji();
    cout << deny.swap_cnt << endl;
    cout << time_total << endl;
    deny.printResult();
    return 0;
}
