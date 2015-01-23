/*
	UTF-8 encoding
	归一化矩阵
	
	矩阵文件的格式是 
	第一行是信息
	接下来N行是N*N矩阵
	
	输入矩阵的大小N 和 矩阵文件的路径

*/

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main(int argc,char *argv[])
{
	if(argc != 3)
	{
		cout << "请输入矩阵的大小 和 路径" << endl;
		return 1;
	}		
	
	const int size = atoi(argv[1]);
	const string path = string(argv[2]);
	//cout << size << " " << path << endl;	
	
	ifstream fin(argv[2]);
	if(!fin)
	{
		cout << "文件打开失败." << endl;
		return 1;
	}

	string line;
	int cnt = 0;
	int flag = 0;
	double* array = new double[size * size];
	string loop;
	int loopnum;
	int total = size*size;
	while(fin)
	{
		fin >> loop >> loopnum;
		if(!fin) { break ;}
		double max=-1,min=1000;
		for(int i=0;i<total; i++)
		{
			fin >> array[i];

			if(array[i] < min)
			{
				min=array[i];
			}
			if(array[i] > max)
			{
				max=array[i];
			}
		}		

/*	
		cout << loop << " " << loopnum << endl;
		for(int i=0;i<total; i++)
		{
			cout << array[i] << " "; 
		}
		cout << "end" <<  endl;
*/
		
		cout << loop << " " << loopnum << endl;
		for(int i=0;i<total;i++)
		{
			cout << (array[i]-min)/(max-min) << " ";
			if(i % size == size-1 )
			{
				cout << endl;
			}
		}
		
	}

	delete[] array;
	fin.close();	
}
