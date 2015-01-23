#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
using namespace std;

int main(int argc,char *argv[])
{
//	cout << argc << endl;
	if(argc != 4)
	{
		cout << "参数个数不正确" << argv[0]  <<  " total loop  path"  << endl;
		return 1;
	}

	int total = atoi(argv[1]); //矩阵元素总数
	int loop  = atoi(argv[2]); //矩阵元素总数
	char *path = argv[3];

	vector<float> v(total,0);
	ifstream is(path);

	float itmp;
	string stmp;
	 for(int i =0; i < loop ; ++i)
	 {
		is >> stmp >> itmp; 
		for(int j =0;j < total ; ++j)
		{
			is >> itmp;
			v.at(j) += itmp;	
		}		
	 }

//	for(float k : v)
	for(int i =0 ; i < total; ++i)
	{
		cout << (v[i]/loop) << " " ;
	}	
	cout << endl;
}

