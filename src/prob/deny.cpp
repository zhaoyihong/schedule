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
	//使用匈牙利算法计算choosen和开销
	iv choosen(total); //新的算法的开销

    //使用概率算法获取N种调度方案
    iiv choosens; //存放N种解决方案
    dv cost_for_choosens;
    iv life_for_choosens;
    for(int i=0;i<total;++i)
    {
        iv temp(total);
        get_schedule_use_probability(temp);

        //计算每种解决方案在last_cost中的开销
        double sumcost_for_temp = 0.0;
        int sumlife_for_temp = 0;
        for(int j=0;j<total;++j)
        {
            sumcost_for_temp += last_cost[j][temp[j]];
            sumlife_for_temp += life[j][temp[j]];
        }
        cost_for_choosens.push_back(sumcost_for_temp);
        life_for_choosens.push_back(sumlife_for_temp);
   
        printArray(temp,temp.size());
        cout << sumcost_for_temp << endl;
        cout << sumlife_for_temp << endl;

        if(sumcost_for_temp != (LIFE_MAX-1)*total)
            choosens.push_back(temp);
    }

    //找到choosens中开销最小
    int imin = 0;
    double mincost = 10000;
    for(int i=0;i<cost_for_choosens.size();++i)
    {
        if(cost_for_choosens[i] < mincost)
        {
            imin = i;
            mincost = cost_for_choosens[i];
        }
    }
    choosen = choosens[imin];

    cout << "choosen:" << endl;
    printArray(choosen,choosen.size());
    cout << "old:" << endl;
    printArray(schedule_now,schedule_now.size());
    
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
	//比较开销,如果较小才采用新方案,否则schedule_now还是上轮的
	//2014-11-24 这里用last_cost代替当前的开销不合适,应该用本轮的开销
	//更新最新开销
	for(int i = 0 ;i < total ; ++ i) //内核
	{
		int app1,app2;
		double cost1,cost2;
		app1= choosen[i];	 //i 内核上的app
        cost1 = vcost_this_loop[i]; // app 在 i核上的cost		
		
        app2= schedule_now[i];	 //i 内核上的app
        cost2 = vcost_this_loop_old[i]; // app 在 i核上的cost		
		

        //开销更小的内核设置为0
        if(cost1  < cost2)
        {
            //2015-03-20 -- 待修改,开销较小且life为0的,将cost置0,life不为0的.更新cost 
            //life[i][app1]=-1; //更新这个开销的有效性.
            if( life[i][app1] >0) //cost1较小且life>0时,才更新
            {
                last_cost[i][app1]= cost1;// 更新最新的cost
            }
            else
            {
                last_cost[i][app1]= 0;// 更新最新的cost
                life[i][app1] = -1;
            }
            life[i][app2]=LIFE_MAX; //更新这个开销的有效性
	        last_cost[i][app2]= cost2;// 更新最新的cost
        }
        else if(cost1 > cost2)
        {
            if(life[i][app2] > 0)
            {
                last_cost[i][app2]= cost2;// 更新最新的cost
            }
            else
            {
                last_cost[i][app2]= 0;
                life[i][app2] = -1;
            }

            last_cost[i][app1]= cost1;// 更新最新的cost
            life[i][app1]=LIFE_MAX; //更新这个开销的有效性.
        
        }
        else
        {
            //life[i][app1]=-1; //更新这个开销的有效性
            //life[i][app2]=-1; //更新这个开销的有效性
        }

    
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


void Deny::get_schedule_use_probability(vector<int> &choosen)
{
    //内核打乱顺序
    iv vcore; //内核列表 
    iv vapp;  //app列表,用来存放尚未调度的app
    for(int i=0;i < total; ++i)
	{
		vcore.push_back(i);
	    vapp.push_back(i);
    }
	
    vector<int>::iterator it;
	random_shuffle(vcore.begin(),vcore.end()); //打乱顺序
   

    //每个内核依次进行选择应用
    for(it = vcore.begin(); it != vcore.end(); ++ it)
    {
        //计算每个应用的概率 

        int coreid = *it;
        //cout << "core:" << coreid << endl;

        double sum = 0; //开销的累加和
        //cout << "last cost:";
        vector<int>::iterator it2;
        for(it2=vapp.begin();it2!=vapp.end();++it2)
        {
          //  cout << last_cost[coreid][*it2] << " ";
            sum += last_cost[coreid][*it2];
        }
        //cout << endl;
       
        //可能会出现全0的情况
        /* 
        if(sum - 0.0000001 < 0.00001 && sum - 0.0000001 > -0.001)
        {
            sum = 1;
        }
        */

        sum = sum + 1;
        iv prob; //概率大小 : 总和 - 个体
        // cout << "last cost trans:" ;
        for(it2=vapp.begin();it2!=vapp.end();++it2)
        {
            int intprob = (sum-last_cost[coreid][*it2])*10000 ;
            
            if(last_cost[coreid][*it2] <  0.000001 )
            {
                intprob *= 10;
            }
            prob.push_back(intprob);

            //cout << prob.back() << " ";
          //  cout << intprob  << " ";
        }
       // cout << endl;

        //转换一下,用轮盘法来选择
        
        for(int i=1;i<(int)prob.size();++i)
        {
            prob[i] += prob[i-1];
           
        }

        int rint = getIntRandom(0,prob.back());
        int choose_app = 0;
        
        for(int i=0;i<(int)prob.size();++i)
        {
            if(rint <= prob[i])
            {
                choose_app = vapp[i];    
                break;
            }
        }
      
    
        //cout <<  "vapp:";
        //printArray(vapp,vapp.size());
        //cout << "prob:";
        //printArray(prob,prob.size());    
       // cout << "rint:" << rint << endl;
       // cout <<"choose:" << choose_app << endl;
        //cout << endl;

       //将choonsen_app添加到choosen中,并从vapp中删除
        vapp.erase(remove(vapp.begin(),vapp.end(),choose_app),vapp.end());
        choosen[coreid] = choose_app ;
    }

    //cout << "choosen:" ;
    //printArray(choosen,choosen.size());
    //cout << endl;
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
    deny.printChengji();
    cout << time_total << endl;
    deny.printResult();
	return 0;
}
