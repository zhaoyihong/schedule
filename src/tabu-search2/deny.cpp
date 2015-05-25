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

	schedule_now = ischedule;
	
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
//		printChengji();		
	//	cout << "total cost : "  << total_cost << endl << endl;
	}
}
//选出新的方案,并更新min_value 和 schedule_now
void Deny::stage(int loop)
{
   // cout << "第" << loop << "轮调度计算开始:" << endl;
	
    iv choosen(total); //新的算法的开销
    choosen  = schedule_now;

    //获取了本轮交换对
    get_schedule_use_search();

    //对照禁忌表 检查交换对
    check_current_swap_pairs();
   
    cout << "swap_pair:" << endl;
    //对照本轮交换对，生成新的对调方案
    for(int i=0;i<(int)(current_pairs.size());++i)
    {
        cout << current_pairs[i] << endl;
        
        if(current_pairs[i].valid )
        {

            int app1 = current_pairs[i].app1;     
            int app2 = current_pairs[i].app2;     
            int core1 = current_pairs[i].core1;     
            int core2 = current_pairs[i].core2;     
                
            choosen[core1] = app2;
            choosen[core2] = app1;

        }
    }

    // 依据本轮交换对获取下一次的方法
    cout << "choosen:" << endl;
    printArray(choosen,choosen.size());
    cout << "old:" << endl;
    printArray(schedule_now,schedule_now.size());
    
	//总开销
	double  cost_this_loop = 0.0; //使用choosen方案的cost
	double  cost_this_loop_old = 0.0; //使用上一轮方案的cost schedule_now
	
    //各个app的开销
	dv vcost_this_loop; //choosen对应的开销
	dv vcost_this_loop_old; //schedule_now对应的开销
	for(int i=0; i< total; i++)
	{
        //新调度算法总开销计算
        double cost_for_app = getValue(i,choosen[i]);
        vcost_this_loop.push_back(cost_for_app);
        cost_this_loop += cost_for_app;	
        
        //旧调度方法总开销计算
        double cost_for_app_old = getValue(i,schedule_now[i]);
        vcost_this_loop_old.push_back(cost_for_app_old);
        cost_this_loop_old += cost_for_app_old;	
	}
    
	//当新的调度方案优于旧的调度方案
	cout << "new:" << cost_this_loop << " ";
	cout << "old:" << cost_this_loop_old << endl;
	
    //采样后更新禁忌表
    update_tabu_pairs();
    
    cout << "tabu_pairs:" << endl;
    for(int i=0;i<tabu_pairs.size();++i)
    {
        cout << tabu_pairs[i] << endl;
    }

    //比较新旧调度算法开销
    if(cost_this_loop < cost_this_loop_old)
	{
        cout << "choose new" << endl;
		schedule_now = choosen; //设定为当前调度方案
		total_cost += cost_this_loop;
		schedule_history.push_back(schedule_now);
		cost_history.push_back(vcost_this_loop);
		total_cost_history.push_back(cost_this_loop);
    } 
	else
	{	//旧的更好.search由于是swap交换的,比较swap

        //对旧调度方法进行改进
        //      cout << "swap over:" << endl;
  
        cout << "in update:" << endl;
        //对照禁忌交换对，生成新的对调方案
        //for(int i=0;i<(int)(tabu_pairs.size());++i)
 /* 
        cout << "badpair size" << bad_pairs.size() << endl;

        for(int i=0;i<bad_pairs.size();++i)
        {
            swap_pair tmp = bad_pairs[i];
            cout << tmp << endl;
            tmp.reverse();
            int app1 = tmp.app1;
            int core1 = tmp.core1;
            int app2 = tmp.app2;
            int core2 = tmp.core2;

            choosen[core1] = app2;
            choosen[core2] = app1;
        }

        schedule_now = choosen;
*/

        int cnt= 0;
        for(int i=tabu_pairs.size()-1;i>tabu_pairs.size()-1-total/2;--i)//只根据当前回合新增的禁忌做改变
        {
            int app1 = tabu_pairs[i].app1;     
            int app2 = tabu_pairs[i].app2;     
            int core1 = tabu_pairs[i].core1;     
            int core2 = tabu_pairs[i].core2;     
            
            //出现了交换对,就可以进行交换
            if(schedule_now[core1] == app1 && schedule_now[core2] == app2)
            {
                cout << "get from tabu_pairs:"  << tabu_pairs[i] << endl;
                schedule_now[core1] = app2;
                schedule_now[core2] = app1;
                cnt++; //不会换回来的次数
            }
        }
        
        //current_cnt表示有效有效交换对个数
        //current_cnt - cnt 表示有效交换对中要交换还原的个数
        swap_cnt += (current_cnt - cnt)*2;
        cout << "swap cnt + " << (current_cnt-cnt)*2 << endl;

        
        cout << "update old:" << endl;
        printArray(schedule_now,schedule_now.size());

        //重新计算旧方法的开销
        vcost_this_loop_old.clear();
        double cost_swap = 0.0;//本轮开销
        for(int i=0; i< total; i++)
        {
            double cost_for_app = getValue(i,schedule_now[i]);
            vcost_this_loop_old.push_back(cost_for_app);
            cost_swap += cost_for_app;
        }
       
        cout << "cost:" << cost_swap << " "<< boolalpha <<  (cost_swap <= cost_this_loop_old)  << endl;       

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


void Deny::get_schedule_use_search(void)
{
    current_pairs.clear();
    
    iv cores;

    for(int i=0;i<total;++i)
    {
        cores.push_back(i);
    }

    random_shuffle(cores.begin(),cores.end());

    iv be_choosed(total,0);

    //只有前面的一半的核才可以选择交换的对象
    
    int core1,core2; //core1 ,core2

    for(int i=0;i<total/2;++i)
    {
        core1 = cores[i];

        while(1)
        {
            int j = getIntRandom(total/2,total-1);
            core2 = cores[j];

            if(0 == be_choosed[core2])
            {
                be_choosed[core2] = 1;
                break;
            }
        }
   
        assert(core2 != -1);
        current_pairs.push_back(swap_pair(core1,schedule_now[core1],core2,schedule_now[core2])); 

    }

}



void Deny::check_current_swap_pairs()
{
    current_cnt = 0;
    //对比禁忌表与当前交换对，如果有和禁忌表中相反的，直接设置为无效交换对
    int cnt=0;
    for(int i=0;i<current_pairs.size();++i)
    {   
        //swap_pair cs = current_pairs[i];
        for(int j=0;j<tabu_pairs.size();++j) //如果禁忌表使用hash结构，那么这一步应该是O(1)
        {
            if(current_pairs[i].cmp(tabu_pairs[j]) == -1)
            {
                current_pairs[i].valid = false;
                deny_cnt ++;
                cnt++;
                break;
            }
        }
    }
    cout << "cnt:" << cnt << endl; //拒绝交换的次数
    current_cnt = total/2-cnt;
    cout << "swap,caiyang + " << current_cnt *2 << endl;
    swap_cnt += current_cnt * 2; //每轮探索前进行的交换次数
    caiyang_cnt += current_cnt *2 ; 
}


void Deny::update_tabu_pairs()
{
    bad_pairs.clear();

    //检查所有的交换对
    for(int i=0;i<current_pairs.size();++i)
    {
        int app1 = current_pairs[i].app1;
        int app2 = current_pairs[i].app2;
        int core1 = current_pairs[i].core1;
        int core2 = current_pairs[i].core2;
        bool valid =  current_pairs[i].valid;

        if(valid)
        {
            double cost_before = getValue(core1,app1)+getValue(core2,app2);
            double cost_after = getValue(core1,app2)+getValue(core2,app1);
        
            swap_pair tmp = current_pairs[i];
            if(cost_after > cost_before)
            {
                bad_pairs.push_back(tmp);               
                tmp.reverse();
            }             
            
            bool not_in = true;
            for(int j=0;j<tabu_pairs.size();++j)
            {
                int cmp = tabu_pairs[j].cmp(tmp);

                if(cmp == 0)
                {
                    //已有相同的不加入
                    not_in = false;
                    break;
                }

                if(cmp == -1)
                {
                    //拥有相反的，将相反的反过来
                    tabu_pairs[j].reverse();
                    not_in = false;
                    break;
                } 
            }

            //不在禁忌表中
            if(not_in)
            {
                tabu_pairs.push_back(tmp);
            }
        }
    }
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

    cout << deny.caiyang_cnt << endl;

    cout << deny.swap_cnt << endl;
    cout << time_total << endl;
    
   

    deny.printResult();
    return 0;
}
