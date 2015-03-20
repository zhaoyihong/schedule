
#include "local.h"
#include  <sys/time.h>

Local::Local(int total,char *path,int maxloop):total(total),path(path),maxloop(maxloop){
	init();
}

Local::~Local(){
	delete[] data;
	delete[] schedule;
}


void Local::init()
{
	loop=1;
	size=(int)(sqrt(total)); //其实size的意思就是限制一行中有几个核
	if(size * size  != total )
	{
		//cout <<  "total must can be 4 16 64 128" << endl;
		//exit(0);
		size=2; //如果
	}

	total_cost = 0.0;
	
	data=new double[total];
	data_bk=new double[total];
	schedule=new int[total];
	schedule_bk=new int[total];

	for(int i=0;i<total;++i)
	{
		data[i]=-1.0f;
		data_bk[i]=-1.0f;
		schedule[i]=i;
		schedule_bk[i]=i;
	}
	
	random_shuffle(schedule,schedule+total); //打乱顺序
	//printArray(schedule,total); 
	initData();
	
}


void Local::initData()
{
	for(int i=0;i<total;i++)
	{					
		data[i]=getValue(i,schedule[i]);
		data_bk[i]=getValue(i,schedule_bk[i]);
	}
}


double Local::getValue(int cpu,int app)
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


void Local::start()
{
    struct timeval tv_start,tv_end; 
    gettimeofday(&tv_start,NULL);

	for(int i=1;i<=maxloop;++i)
	{
		int step=i%4;
		if(size == 2) //如果只有4个内核,则只执行A C 两个阶段
		{
			if(total==2)
			{
				//双核时 做 偶奇行 和 偶奇列互换
				if(step == 0) {step = 0; }
				else if(step == 1) {step = 2; }
				else if(step == 2) {step = 0; }
				else if(step == 3) {step = 2; }
			}
			else 
			{
				/*
					8核的分布
					* *
					* *
					* *
					* *
					
					只有2列 不做奇偶列互换
				*/
                if(step == 3) {step = 0; }		
		
			}
		}
		//cout << "step" << step << endl;

		

		initData();
		double oldCost = getCurrentCost();

		for(int k = 0 ;k < total ; ++k)
		{
			schedule_bk[k] = schedule[k];
		}


		//cout << "old cost:" << oldCost << endl;

		switch(step)
		{
			case 0:
			stepA();
			break;	
			
			case 1:
			stepB();
			break;	

			case 2:
			stepC();
			break;

			case 3:
			stepD();
			break;	
			
			default:
			break;	
		}

		initData();
		double newCost = getCurrentCost(1);  //bk的值 ,bk的值在swap中是直接交换的, 非bk版的值在swap中是比较后交换
		double currentCost = 0;
		if(newCost < oldCost)   // 如果newCost > oldCost ,直接按照bk的结果来
		{
			for(int k=0;k < total ; ++k)
			{
				schedule[k] = schedule_bk[k];
				data[k] = data_bk[k];	
			}	
			currentCost = oldCost;	
			
		}
		else  
		{
 			currentCost =  getCurrentCost();
		}

		total_cost+=currentCost;
			
		//cout << "current cost:" << getCurrentCost() << endl;
        printCurrentSchedule();	
		cout <<  getCurrentCost() << endl;
		//cout << endl;
		loop ++;
	}	

 
     gettimeofday(&tv_end,NULL);
     time_total = tv_end.tv_sec-tv_start.tv_sec+(tv_end.tv_usec-tv_start.tv_usec)/1000000.0;
 

}

void Local::stepA()
{
	// 偶奇列互换
	for(int i=0;i<total;++i)
	{
		if(i%size%2 == 0)
		{
			swap(i,i+1);
		}
	}
}

void Local::stepB()
{
	//奇偶行互换
	int i = 0;
/*
	for(;i<size -1;i++)
	{
		if(i%2 == 1)
		{
			swap(i,i+1);
		}
	}

*/
	for(i = 0;i<total - size - 1;i++)
	{
		if(i/size%2 == 1)
		{
			swap(i,i+size);
		}
	}

/*
	for(i=total-size;i<size-1;i++ )
	{
		
		if(i%2 == 1)
		{
			swap(i,i+1);
		}
	}
*/
}
void Local::stepC()
{
	//偶奇行互换
	for(int i = 0 ; i < total ; ++i)
	{
		if(i/size%2 == 0)
		{
			swap(i,i+size);
		}
	}

}
void Local::stepD()
{

	//奇偶列互换		
	for(int i=0;i<total;i++)
	{
		int col = i%size;
		if( col%2  == 1 && col != size-1 ) //奇数列且非最后一列
		{
			swap(i,i+1);
		}
	}

}


// 第 index1核上的app 和 index2核上的app互换
bool Local::swap(int index1,int index2)
{
	
	//cout << "swap(" << index1 << "," << index2 << ")" << endl;
	//原来的app  调度规则
	int schedule1 = schedule[index1];
	int schedule2 = schedule[index2];
	double cost_before = data[index1]+data[index2];

	//在bk中直接交换	
	schedule_bk[index1] = schedule2;
	schedule_bk[index2] = schedule1;
	

	//互换后的   互换后的调度负责是 index1-> schedule2 ,index2->schedule1
	double cost1 = getValue(index1,schedule2);
	double cost2 =  getValue(index2,schedule1);
	double cost_after = cost1 + cost2;
	
	if(cost_after < cost_before)
	{
		//如果互换后更好,则互换
	//	cout << "good:" << schedule1 << " " << schedule2 <<  " "<<  cost_before << " "<< cost_after<< endl;
		schedule[index1] = schedule2;
		schedule[index2] = schedule1;
		return true;
	}
	
	return false;
}


double Local::getCurrentCost(int isbk )
{
	double result = 0.0;
	for(int i=0;i<total;i++)
	{
		if(!isbk)
		{
			result += data[i];	
		}
		else
		{
			result += data_bk[i];	
		}
	}
	return result;
}

void Local::printCurrentSchedule()
{
	for(int i=0;i<total;i++)
	{
		cout << schedule[i] << " ";	
	}
	cout << endl;
}


double Local::result()
{
	return total_cost;
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
	return random()%(max-min)+min;

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

	//getValue(path,1,1,1) ;	
	Local local(total,path,maxloop);
	local.start();
	cout << local.time_total << endl;
	cout << local.result() << endl;

	return 0;
}

